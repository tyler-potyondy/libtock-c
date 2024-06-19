#include <stdio.h>

#include <openthread/instance.h>
#include <openthread/platform/alarm-milli.h>
#include <openthread/platform/radio.h>

#include <net/ieee802154.h>

#include "openthread-system.h"
#include "plat.h"


// This platform file serves to handle platform specific logic
// that must occur when not performing OpenThread specific work.
// The `otSysProcessDrivers` must be called upon each iteration
// of the main loop. The logic for handling radio upcalls from
// the kernel and passing the received data to the OpenThread
// instance is implemented here.
//
// Passing the radio buffer to the OpenThread instance becomes
// challenging given the sync/async nature of the kernel upcalls.
// Apps only receive upcalls when they have yielded. Conversely,
// this means that an app will receive a pending upcall whenever
// yield is called. This can lead to strange behavior and receiving
// packets out of order if an app yields before completing copying
// the packet from the buffer shared with the kernel to the buffer
// passed to OpenThread. A ring buffer is used to handle receiving
// and buffering multiple packets within the kernel (share ring buffer
// with kernel). We also use a userspace ring buffer to hand the
// aforementioned async issues. This allows the packet reception
// for an OpenThread app to work as follows:
//  1. App shares a kernel ring buffer with the kernel (read/write buffer).
//  2. Kernel schedules an upcall to notify app.
//  3. App yields allowing upcall to be handled.
//  4. App copies data from kernel ring buffer to userspace ring buffer.
//  5. App copies data from userspace ring buffer to OpenThread instance.
//
// Although the userspace ring buffer may appear redundant, it is necessary
// as OpenThread may call libtock functions that yield while OpenThread
// possess the buffer holding the receiving packet. The use of a ring buffer
// allows for us to prevent the receive upcall from overwriting the buffer
// OpenThread is currently copying to the OpenThread instance (leading to packets)
// being receiving out of order or potentially overwritten.

static libtock_ieee802154_rxbuf rx_buf_a;
static libtock_ieee802154_rxbuf rx_buf_b;

#define USER_RX_BUF_FRAME_COUNT 3
static char buf[libtock_ieee802154_FRAME_LEN * USER_RX_BUF_FRAME_COUNT];
ring_buffer usr_rx_buffer = {
  .buffer      = buf,
  .write_index = 0,
  .read_index  = 0,
  .new         = false
};

static uint8_t rx_mPsdu[OT_RADIO_FRAME_MAX_SIZE];
static otRadioFrame receiveFrame = {
  .mPsdu   = rx_mPsdu,
  .mLength = OT_RADIO_FRAME_MAX_SIZE
};

typedef struct otTock {
  ring_buffer* usr_rx_buffer;
  libtock_ieee802154_rxbuf* kernel_rx_buf;
  otInstance* instance;
} otTock;

typedef struct otTock otTock;
otTock otTockInstance = {
  .usr_rx_buffer = &usr_rx_buffer,
  .kernel_rx_buf = &rx_buf_a,
  .instance      = NULL,
};

// Helper utility to provide a new ring buffer to the kernel and
// return the ring buffer previously held by the kernel
static libtock_ieee802154_rxbuf* swap_shared_kernel_buf(otTock* instance);

static void rx_callback(__attribute__ ((unused)) int   pans,
                        __attribute__ ((unused)) int   dst_addr,
                        __attribute__ ((unused)) int   src_addr);

static void ring_reset_cb(__attribute__ ((unused)) int   pans,
                          __attribute__ ((unused)) int   dst_addr,
                          __attribute__ ((unused)) int   src_addr,
			  __attribute__ ((unused)) void* ud) {
  rx_callback(pans, dst_addr, src_addr);
}

static libtock_ieee802154_rxbuf* swap_shared_kernel_buf(otTock* instance) {
  libtock_ieee802154_rxbuf* rx_buf;

  if (instance->kernel_rx_buf == &rx_buf_a) {
    instance->kernel_rx_buf = &rx_buf_b;
    rx_buf = &rx_buf_a;
  } else {
    instance->kernel_rx_buf = &rx_buf_a;
    rx_buf = &rx_buf_b;
  }

  libtock_reset_ring_buf(instance->kernel_rx_buf, ring_reset_cb, NULL);
  return rx_buf;
}

void otSysInit(int argc, char *argv[]){
  OT_UNUSED_VARIABLE(argc);
  OT_UNUSED_VARIABLE(argv);
  init_otPlatAlarm();
}

bool otSysPseudoResetWasRequested(void) {
  // TODO
  return false;
}

void otSysProcessDrivers(otInstance *aInstance){
  // If new data exists, we copy the data from the returned kernel
  // ring buffer into the user space ring buffer
  if (usr_rx_buffer.new) {
    int offset;

    // loop through data until all new data is read
    while (usr_rx_buffer.read_index != usr_rx_buffer.write_index) {
      offset = usr_rx_buffer.read_index * libtock_ieee802154_FRAME_LEN;
      char* rx_buf       = usr_rx_buffer.buffer;
      int header_len  = rx_buf[offset];
      int payload_len = rx_buf[offset + 1];
      int mic_len     = rx_buf[offset + 2];

      // this does not seem necessary since we implement the csma backoff in the radio driver
      // receiveFrame.mInfo.mRxInfo.mTimestamp = otPlatAlarmMilliGetNow() * 1000;
      receiveFrame.mInfo.mRxInfo.mRssi      = 50;
      receiveFrame.mLength = payload_len + header_len + mic_len + 2;
      receiveFrame.mInfo.mRxInfo.mTimestamp = 0;
      receiveFrame.mInfo.mRxInfo.mLqi       = 0x7f;

      // copy data
      for (int i = 0; i < (receiveFrame.mLength + libtock_ieee802154_FRAME_META_LEN); i++) {
        receiveFrame.mPsdu[i] = rx_buf[i + libtock_ieee802154_FRAME_META_LEN + offset];
      }

      // notify openthread instance that a frame has been received
      otPlatRadioReceiveDone(aInstance, &receiveFrame, OT_ERROR_NONE);
      usr_rx_buffer.read_index++;
      if (usr_rx_buffer.read_index == USER_RX_BUF_FRAME_COUNT) {
        usr_rx_buffer.read_index = 0;
      }
    }
    usr_rx_buffer.new = false;
  }

}


static void rx_callback(__attribute__ ((unused)) int   pans,
                        __attribute__ ((unused)) int   dst_addr,
                        __attribute__ ((unused)) int   src_addr) {

  /* It is important to avoid sync operations that yield (i.e. printf)
     as this may cause a new upcall to be handled and data to be received
     out of order and/or lost. */

  uint8_t* rx_buf = swap_shared_kernel_buf(&otTockInstance)[0];

  uint8_t* head_index = &rx_buf[0];
  uint8_t* tail_index = &rx_buf[1];

  int offset = 2 + *head_index * libtock_ieee802154_FRAME_LEN;

  // | head active # | tail active # | frame 0 | frame 1 | ... | frame n |
  while (*head_index != *tail_index) {
    int header_len = rx_buf[offset];
    int payload_len = rx_buf[offset + 1];
    int mic_len        = rx_buf[offset + 2];

    int receive_frame_length = payload_len + header_len + mic_len;
    int ring_buffer_offset = usr_rx_buffer.write_index * libtock_ieee802154_FRAME_LEN;

    for (int i = 0; i < (libtock_ieee802154_FRAME_META_LEN + receive_frame_length); i++) {
      usr_rx_buffer.buffer[ring_buffer_offset + i] = rx_buf[i + offset];
      rx_buf[i + offset] = 0;
    }

    usr_rx_buffer.new = true;
    usr_rx_buffer.write_index++;
    if (usr_rx_buffer.write_index == USER_RX_BUF_FRAME_COUNT) {
      usr_rx_buffer.write_index = 0;
    }

    *head_index = (*head_index + 1) % libtock_ieee802154_MAX_RING_BUF_FRAMES;
    offset     += libtock_ieee802154_FRAME_LEN;

    if (*head_index == 0) {
      offset = 2;
    }
  }

  assert(*head_index == *tail_index);

}


otError otTockStartReceive(uint8_t aChannel, otInstance *aInstance) {
  if (otTockInstance.instance == NULL) {
    otTockInstance.instance = aInstance;
  }
  if (aChannel != 26) {
    return OT_ERROR_NOT_IMPLEMENTED;
  }

  int res = libtock_ieee802154_receive(otTockInstance.kernel_rx_buf, rx_callback);

  if (res != RETURNCODE_SUCCESS) return OT_ERROR_FAILED;

  return OT_ERROR_NONE;
}
