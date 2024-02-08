// TODO COPYRIGHT

#include <internal/nonvolatile_storage.h>
#include <openthread/platform/flash.h>
#include <stdio.h>

// Force flash operations to be synchronous
static bool done = false;

// Why did @Tyler not include this?
// uint32_t otPlatFlashGetSwapSize(otInstance *aInstance);

static void read_done(int length, __attribute__((unused)) int arg1,
                      __attribute__((unused)) int arg2, __attribute__((unused)) void *ud) {
    printf("\tFinished read! %i\n", length);
    done = true;
}

static void write_done(int length, __attribute__((unused)) int arg1,
                       __attribute__((unused)) int arg2, __attribute__((unused)) void *ud) {
    printf("\tFinished write! %i\n", length);
    done = true;
}

void otPlatFlashInit(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
}

void otPlatFlashErase(otInstance *aInstance, uint8_t aSwapIndex) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

    int num_bytes;
    nonvolatile_storage_internal_get_number_bytes(&num_bytes);
    printf("Have %i bytes of nonvolatile storage\n", num_bytes);

    // Write all zeroes to flash
    uint8_t *zeroes = calloc(num_bytes, sizeof(uint8_t));

    otPlatFlashWrite(aInstance, aSwapIndex, 0, zeroes, num_bytes);

    free(zeroes);
}

void otPlatFlashWrite(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset,
                      const void *aData, uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aSwapIndex);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    int ret;

    // Sets up the buffer to store the output of the read
    ret = nonvolatile_storage_internal_write_buffer((void *)aData, aSize);
    if (ret != RETURNCODE_SUCCESS) {
        printf("\tERROR setting write buffer\n");
        return;
    }

    ret = nonvolatile_storage_internal_write_done_subscribe(write_done, NULL);
    if (ret != RETURNCODE_SUCCESS) {
        printf("\tERROR setting write done callback\n");
        return;
    }

    done = false;
    ret = nonvolatile_storage_internal_write(aOffset, aSize);
    if (ret != 0) {
        printf("\tERROR calling write\n");
        return;
    }
    // wait until callback fires
    yield_for(&done);
}

void otPlatFlashRead(otInstance *aInstance, uint8_t aSwapIndex, uint32_t aOffset, void *aData,
                     uint32_t aSize) {
    OT_UNUSED_VARIABLE(aInstance);
    OT_UNUSED_VARIABLE(aSwapIndex);
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    int ret;

    // Sets up the buffer to store the output of the read
    ret = nonvolatile_storage_internal_read_buffer(aData, aSize);
    if (ret != RETURNCODE_SUCCESS) {
        printf("\tERROR setting read buffer\n");
        return;
    }

    ret = nonvolatile_storage_internal_read_done_subscribe(read_done, NULL);
    if (ret != RETURNCODE_SUCCESS) {
        printf("\tERROR setting read done callback\n");
        return;
    }

    done = false;
    ret = nonvolatile_storage_internal_read(aOffset, aSize);
    if (ret != 0) {
        printf("\tERROR calling read\n");
        return;
    }
    // wait until callback fires
    yield_for(&done);
}
