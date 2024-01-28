// TODO COPYRIGHT

#include<openthread/platform/flash.h>
#include <internal/nonvolatile_storage.h>

#include <stdio.h>
// Why did @Tyler not include this?
// uint32_t otPlatFlashGetSwapSize(otInstance *aInstance);

void otPlatFlashInit(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, const void *aData, uint32_t aSize) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData, uint32_t aSize) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}
