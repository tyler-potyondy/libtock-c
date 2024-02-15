#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <timer.h>
#include <rng.h>

#include <openthread/platform/flash.h>

void testFlashReadWrite(char* testName, otInstance *aInstance, uint32_t length, uint32_t flashOffset) {
    printf("%s: Starting test\n", testName);

    char* write_buf = malloc(length);
    char* read_buf = malloc(length);

    // populate the buffer with data to be written
    int num_received;
    int ret_code = rng_sync(write_buf, length, length, &num_received);
    // printf("DEBUG: created buffer of %d random bytes\n", num_received);
    
    // uncomment if you want to see the buffer being written 
    // for (uint32_t i = 0; i < length; i++) {
    //     printf("%c", write_buf[i]);
    // }
    // printf("\n");

    otPlatFlashWrite(aInstance, 0, flashOffset, write_buf, length);
    otPlatFlashRead(aInstance, 0, flashOffset, read_buf, length);

    assert(memcmp(write_buf, read_buf, length) == 0);

    printf("%s: TEST PASSED\n", testName);
    printf("\n");
}

int main(void) {
  // setting NULL otInstance since we don't need it 
  // for testing any flash functionality
  otInstance *aInstance = NULL;
  otPlatFlashInit(aInstance);

  while(true) {
    testFlashReadWrite("Write and read back 100 bytes", aInstance, 100, 0);
    delay_ms(1000);
  }
}
