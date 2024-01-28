// TODO COPYRIGHT
#include <openthread-system.h>
#include <stdio.h>
void otSysInit(int argc, char *argv[]) {
    // TODO
    
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
}

bool otSysPseudoResetWasRequested(void) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    return false;
}

void otSysProcessDrivers(otInstance *aInstance) {
    // TODO
    printf("%s:%d in %s", __FILE__, __LINE__, __func__);
    OT_UNUSED_VARIABLE(aInstance);
}