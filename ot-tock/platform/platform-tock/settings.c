#include <openthread/platform/settings.h>
#include <stdio.h>
void otPlatSettingsInit(otInstance *aInstance, const uint16_t *aSensitiveKeys,
                        uint16_t aSensitiveKeysLength) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    OT_UNUSED_VARIABLE(aInstance);
}

void otPlatSettingsDeinit(otInstance *aInstance) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    OT_UNUSED_VARIABLE(aInstance);
}

otError otPlatSettingsGet(otInstance *aInstance, uint16_t aKey, int aIndex, uint8_t *aValue,
                          uint16_t *aValueLength) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    OT_UNUSED_VARIABLE(aInstance);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsSet(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue,
                          uint16_t aValueLength) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    OT_UNUSED_VARIABLE(aInstance);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsAdd(otInstance *aInstance, uint16_t aKey, const uint8_t *aValue,
                          uint16_t aValueLength) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    OT_UNUSED_VARIABLE(aInstance);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatSettingsDelete(otInstance *aInstance, uint16_t aKey, int aIndex) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    OT_UNUSED_VARIABLE(aInstance);
    return OT_ERROR_NOT_IMPLEMENTED;
}

void otPlatSettingsWipe(otInstance *aInstance) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    OT_UNUSED_VARIABLE(aInstance);
}