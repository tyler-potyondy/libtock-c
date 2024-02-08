// TODO COPYRIGHT

#include <ieee802154.h>
#include <openthread/platform/radio.h>
#include <stdio.h>

// Some functions require returning pointer to an otRadioFrame (TODO)
static otRadioFrame tempTransmitBuffer;
static otRadioCaps tempRadioCaps = 0;

void otPlatRadioGetIeeeEui64(otInstance *aInstance, uint8_t *aIeeeEui64) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otPlatRadioSetPanId(otInstance *aInstance, uint16_t aPanid) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otPlatRadioSetExtendedAddress(otInstance *aInstance, const otExtAddress *aExtAddress) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    int retCode = ieee802154_set_address_long((unsigned char*) aExtAddress);
    if (retCode != 0) printf("Error setting long address");
}

void otPlatRadioSetShortAddress(otInstance *aInstance, uint16_t aShortAddress) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    int retCode = ieee802154_set_address(aShortAddress);
    if (retCode != 0) printf("Error setting address");
}

bool otPlatRadioIsEnabled(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    return ieee802154_is_up();
}

otError otPlatRadioEnable(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    int retCode = ieee802154_up();
    if (retCode == 0)
        return OT_ERROR_NONE;
    else {
        printf("Initializing Radio Failed!\n");
        return OT_ERROR_FAILED;
    }
}

otError otPlatRadioDisable(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    int retCode = ieee802154_down();

    if (retCode == 0)
        return OT_ERROR_NONE;
    else {
        printf("Disable Radio Failed!\n");
        return OT_ERROR_FAILED;
    }
}

otError otPlatRadioSleep(otInstance *aInstance) {
    // TODO: There is no sleep function.
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioReceive(otInstance *aInstance, uint8_t aChannel) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioTransmit(otInstance *aInstance, otRadioFrame *aFrame) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otRadioFrame *otPlatRadioGetTransmitBuffer(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    return &tempTransmitBuffer;
}

int8_t otPlatRadioGetRssi(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return 0;
}

otRadioCaps otPlatRadioGetCaps(otInstance *aInstance) {
    // TODO: This is temporarily saying there are no capabilities.
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return tempRadioCaps;
}

bool otPlatRadioGetPromiscuous(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return true;
}

void otPlatRadioSetPromiscuous(otInstance *aInstance, bool aEnable) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otPlatRadioEnableSrcMatch(otInstance *aInstance, bool aEnable) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

otError otPlatRadioAddSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioAddSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioClearSrcMatchShortEntry(otInstance *aInstance, uint16_t aShortAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioClearSrcMatchExtEntry(otInstance *aInstance, const otExtAddress *aExtAddress) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

void otPlatRadioClearSrcMatchShortEntries(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otPlatRadioClearSrcMatchExtEntries(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

otError otPlatRadioEnergyScan(otInstance *aInstance, uint8_t aScanChannel, uint16_t aScanDuration) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioGetTransmitPower(otInstance *aInstance, int8_t *aPower) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioSetTransmitPower(otInstance *aInstance, int8_t aPower) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioGetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t *aThreshold) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

otError otPlatRadioSetCcaEnergyDetectThreshold(otInstance *aInstance, int8_t aThreshold) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_ERROR_NONE;
}

int8_t otPlatRadioGetReceiveSensitivity(otInstance *aInstance) {
    // TODO
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return 0;
}