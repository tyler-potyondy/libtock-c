// TODO COPYRIGHT

#include<openthread/platform/flash.h>
#include <internal/nonvolatile_storage.h>

// Why did @Tyler not include this?
// uint32_t otPlatFlashGetSwapSize(otInstance *aInstance);

void otPlatFlashInit(otInstance *aInstance) {
    // TODO
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    // TODO
}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, const void *aData, uint32_t aSize) {
    // TODO
}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData, uint32_t aSize) {
    // TODO
}
