// TODO COPYRIGHT

#include <internal/nonvolatile_storage.h>
#include <openthread/platform/flash.h>
#include <stdio.h>
#include <string.h>

#include <app_state.h>
#include <tock.h>

#define OT_TOCK_APP_STATE_SIZE 2048

struct ot_tock_app_state_t {
    // create a continguous chunk of flash that openthread can freely write to
    uint8_t data[OT_TOCK_APP_STATE_SIZE];
    uint8_t swapData[OT_TOCK_APP_STATE_SIZE];
};

APP_STATE_DECLARE(struct ot_tock_app_state_t, ot_tock_app_state);

// Why did @Tyler not include this?
// uint32_t otPlatFlashGetSwapSize(otInstance *aInstance);

void otPlatFlashInit(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);

    // load app state from flash into memory
    int ret;

    ret = app_state_load_sync();
    if (ret < 0) {
        printf("Error loading application state: %s\n", tock_strrcode(ret));
    }
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    OT_UNUSED_VARIABLE(aInstance);

    // zero out both data and swapData
    memset(ot_tock_app_state.data, 0, OT_TOCK_APP_STATE_SIZE);
    memset(ot_tock_app_state.swapData, 0, OT_TOCK_APP_STATE_SIZE);
}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset,
                      const void *aData, uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);

    printf("OT_TOCK_FLASH: Thread wants to write %d bytes to offset %x. Swap? %d\n", aSize, aOffset, aSwapIndex);

    if ((aOffset + aSize) > OT_TOCK_APP_STATE_SIZE) {
        printf("OT_TOCK_FLASH: ERROR!!! Thread is trying to write to a region outside allocated flash area");
        return;
    }

    if (aSwapIndex) {
        memcpy(ot_tock_app_state.swapData + aOffset, aData, aSize);
    } else {
        memcpy(ot_tock_app_state.data + aOffset, aData, aSize);
    }

    // TODO: no idea if we should call this here
    int ret = app_state_save_sync();
    if (ret != 0) {
        printf("OT_TOCK_FLASH: ERROR!!! saving application state: %s\n", tock_strrcode(ret));
        return;
    }
}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData,
                     uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);

    printf("OT_TOCK_FLASH: Thread wants to read %d bytes from offset %x. Swap? %d\n", aSize, aOffset, aSwapIndex);

    if ((aOffset + aSize) > OT_TOCK_APP_STATE_SIZE) {
        printf("OT_TOCK_FLASH: ERROR!!! Thread is trying to read from a region outside allocated flash area");
        return;
    }

    if (aSwapIndex) {
        memcpy(aData, ot_tock_app_state.swapData + aOffset, aSize);
    } else {
        memcpy(aData, ot_tock_app_state.data + aOffset, aSize);
    }
}
