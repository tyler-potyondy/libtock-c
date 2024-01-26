// TODO COPYRIGHT

#include <openthread/platform/misc.h>

void otPlatReset(otInstance *aInstance) {
    // TODO
    // Not **strictly** required. I did not find an API
    // for this in libtock. Isn't this hardware specific?
}

otPlatResetReason otPlatGetResetReason(otInstance *aInstance){
    // TODO
    // Not **strictly** required.
    return OT_PLAT_RESET_REASON_POWER_ON;
}

void otPlatWakeHost(void){
    // TODO
}
