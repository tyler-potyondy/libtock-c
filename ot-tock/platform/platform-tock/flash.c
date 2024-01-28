// TODO COPYRIGHT

#include<openthread/platform/flash.h>
#include <internal/nonvolatile_storage.h>

// Why did @Tyler not include this?
// uint32_t otPlatFlashGetSwapSize(otInstance *aInstance);

void otPlatFlashInit(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, const void *aData, uint32_t aSize) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData, uint32_t aSize) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
}
