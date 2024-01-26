// TODO COPYRIGHT

#include<openthread/platform/entropy.h>

#include <rng.h>

otError otPlatEntropyGet(uint8_t *aOutput, uint16_t aOutputLength){
    if (aOutput == NULL) {
        return OT_ERROR_INVALID_ARGS;
    }
    
    // Synchronously fill buffer
    int count;
    int returnCode = rng_sync(aOutput, (uint32_t) aOutputLength, (uint32_t) aOutputLength, &count);

    // TODO: test printing count in tock example app. The documentation doesn't make sense.
    if (returnCode == RETURNCODE_SUCCESS) { // RETURNCODE_SUCCESS is 0
        return OT_ERROR_NONE;
    }
    return OT_ERROR_FAILED;
}