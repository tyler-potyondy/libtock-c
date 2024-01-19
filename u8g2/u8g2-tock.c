#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

#include <u8g2.h>

#include <screen.h>

#include "u8g2-tock.h"

typedef struct {
  int error;
  int data1;
  int data2;
  bool done;
} ScreenReturn;

static void u8g2_callback(int   status,
                            int   data1,
                            int   data2,
                            void* ud) {
  ScreenReturn *fbr = (ScreenReturn*)ud;
  fbr->error = tock_status_to_returncode(status);
  fbr->data1 = data1;
  fbr->data2 = data2;
  fbr->done  = true;
}

static int screen_subscribe (subscribe_upcall cb, void *userdata) {
  subscribe_return_t sval = subscribe(DRIVER_NUM_SCREEN, 0, cb, userdata);
  return tock_subscribe_return_to_returncode(sval);
}

static int screen_allow (const void* ptr, size_t size) {
  allow_ro_return_t aval = allow_readonly(DRIVER_NUM_SCREEN, 0, ptr, size);
  return tock_allow_ro_return_to_returncode(aval);
}



static size_t page_size_bytes(u8g2_t* u8g2) {
  size_t cnt;
  cnt = u8g2_GetU8x8(u8g2)->display_info->tile_width;
  cnt *= u8g2->tile_buf_height;
  cnt *= 8;
  return cnt;
}


void u8g2_tock_init(u8g2_t *u8g2);


static u8x8_display_info_t u8x8_ssd1306_128x64_noname_display_info =
      {
        /* NOT RELEVANT FOR TOCK */
        /* chip_enable_level = */ 0,
        /* chip_disable_level = */ 1,
        /* post_chip_enable_wait_ns = */ 20,
        /* pre_chip_disable_wait_ns = */ 10,
        /* reset_pulse_width_ms = */ 100,   /* SSD1306: 3 us */
        /* post_reset_wait_ms = */ 100, /* far east OLEDs need much longer setup time */
        /* sda_setup_time_ns = */ 50,   /* SSD1306: 15ns, but cycle time is 100ns, so use 100/2 */
        /* sck_pulse_width_ns = */ 50,  /* SSD1306: 20ns, but cycle time is 100ns, so use 100/2, AVR: below 70: 8 MHz, >= 70 --> 4MHz clock */
        /* sck_clock_hz = */ 8000000UL, /* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
        /* spi_mode = */ 0,   /* active high, rising edge */
        /* i2c_bus_clock_100kHz = */ 4,
        /* data_setup_time_ns = */ 40,
        /* write_pulse_width_ns = */ 150, /* SSD1306: cycle time is 300ns, so use 300/2 = 150 */

        /* BASED ON THE DISPLAY */
        /* tile_width = */ 16,
        /* tile_height = */ 8,
        /* default_x_offset = */ 0,
        /* flipmode_x_offset = */ 0,
        /* pixel_width = */ 128,
        /* pixel_height = */ 64
      };

uint8_t u8x8_d_ssd1306_128x64_noname(u8x8_t *u8x8, uint8_t msg, __attribute__ ((unused)) uint8_t arg_int, __attribute__ ((unused)) void *arg_ptr) {

  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:

      size_t width, height;
      screen_get_resolution(&width, &height);

      u8x8_ssd1306_128x64_noname_display_info.tile_width = width/8;
      u8x8_ssd1306_128x64_noname_display_info.tile_height = height/8;
      u8x8_ssd1306_128x64_noname_display_info.pixel_width = width;
      u8x8_ssd1306_128x64_noname_display_info.pixel_height = height;


      u8x8_d_helper_display_setup_memory(u8x8, &u8x8_ssd1306_128x64_noname_display_info);
      break;
    default:
      return 0;
  }
  return 1;
}

// Initialize the u8g2 library for Tock use. Call this before using the rest of
// the library.
void u8g2_tock_init(u8g2_t *u8g2) {

  // Since we use syscalls mostly don't need this. Just need to setup
  // display_info.
  u8g2_SetupDisplay(u8g2, u8x8_d_ssd1306_128x64_noname, u8x8_dummy_cb, u8x8_dummy_cb, u8x8_dummy_cb);

  // Allocate the buffer for the screen.
  size_t screen_bytes = (u8g2_GetU8x8(u8g2)->display_info->pixel_width * u8g2_GetU8x8(u8g2)->display_info->pixel_height) / 8;
  uint8_t* buf = (uint8_t*) calloc(1, screen_bytes);
  // Setup the u8g2 struct.
  uint8_t tile_buf_height = u8g2_GetU8x8(u8g2)->display_info->tile_height;
  u8g2_SetupBuffer(u8g2, buf, tile_buf_height, u8g2_ll_hvline_vertical_top_lsb, U8G2_R0);

}

// Clear buffer since we don't include u8g2_buffer.c in the library.
void u8g2_ClearBuffer(u8g2_t *u8g2) {
  memset(u8g2->tile_buf_ptr, 0, page_size_bytes(u8g2));
}

// Send entire buffer via Tock syscalls.
void u8g2_SendBuffer(u8g2_t *u8g2) {

  // Set the frame to the entire region.
  uint16_t width = u8g2_GetU8x8(u8g2)->display_info->pixel_width;
  uint16_t height = u8g2_GetU8x8(u8g2)->display_info->pixel_height;
  screen_set_frame(0, 0, width, height);

  int ret = screen_allow(u8g2->tile_buf_ptr, page_size_bytes(u8g2));
  if (ret != TOCK_STATUSCODE_SUCCESS) return;

  ScreenReturn fbr;
  fbr.done = false;
  ret = screen_subscribe(u8g2_callback, &fbr);
  if (ret != TOCK_STATUSCODE_SUCCESS) return;

  syscall_return_t com = command(DRIVER_NUM_SCREEN, 200, page_size_bytes(u8g2), 0);
  ret = tock_command_return_novalue_to_returncode(com);
  if (ret == TOCK_STATUSCODE_SUCCESS) {
    yield_for(&fbr.done);
    ret = fbr.error;
  }

  screen_allow(NULL, 0);
}
