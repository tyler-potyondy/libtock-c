// TODO COPYRIGHT

#include <openthread/platform/misc.h>
#include <stdio.h>
void otPlatReset(otInstance *aInstance) {
    // TODO
    // Not **strictly** required. I did not find an API
    // for this in libtock. Isn't this hardware specific?
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

otPlatResetReason otPlatGetResetReason(otInstance *aInstance){
    // TODO
    // Not **strictly** required.
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return OT_PLAT_RESET_REASON_POWER_ON;
}

void otPlatWakeHost(void){
    // TODO
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}
