// TODO COPYRIGHT
#include <openthread-system.h>
#include <stdio.h>

void otSysInit(int argc, char *argv[]) {
    // TODO
    OT_UNUSED_VARIABLE(argc);
    OT_UNUSED_VARIABLE(argv);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otSysDeinit(void) {
    // TODO: Need to check if there is functionality for reloading drivers/hardware
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}
bool otSysPseudoResetWasRequested(void) {
    // TODO
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return false;
}

void otSysProcessDrivers(otInstance *aInstance) {
    // This function gets called a lot by the main loop.
    // We do not have to call into any drivers as Tock will handle it.
    OT_UNUSED_VARIABLE(aInstance);
}