// TODO COPYRIGHT

#include <openthread/platform/radio.h>
#include <stdio.h>

// Some functions require returning pointer to an otRadioFrame (TODO)
static otRadioFrame tempFrame;
static otRadioCaps tempRadioCaps;

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

void otPlatRadioSetPanId(otInstance *aInstance, uint16_t aPanid) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

bool otPlatRadioIsEnabled(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return true;
}

otError otPlatRadioEnable(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioDisable(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioSleep(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return &tempFrame;
}

int8_t otPlatRadioGetRssi(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return 0;
}

otRadioCaps otPlatRadioGetCaps(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return tempRadioCaps;
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return true;
}

void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

void otPlatRadioClearSrcMatchShortEntries(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

void otPlatRadioClearSrcMatchExtEntries(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return OT_ERROR_NOT_IMPLEMENTED;
}

int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return 0;
}