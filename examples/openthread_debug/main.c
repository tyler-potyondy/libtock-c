#include <alarm.h>
#include <assert.h>
#include <internal/alarm.h>
#include <openthread/instance.h>
#include <stdio.h>
#include <string.h>
#include <timer.h>

const int NUM_SAMPLES = 150;

int main(int argc, char *argv[]) {
    printf("Let us find some bugs\n");

    printf("Testing rollover\n");
    // Array of time results
    uint32_t clock_return[NUM_SAMPLES];
    memset(clock_return, 0, sizeof(clock_return));

    for (int i = 0; i < NUM_SAMPLES; i++) {
        int cur_time_ms = otPlatAlarmMilliGetNow();
#if FALSE
        if (cur_time_ms % 1000 > 128) {
            printf("trigger %lu on %d", cur_time_ms);
        }
#endif
        clock_return[i] = cur_time_ms;
        delay_ms(1);
    }

    // print results
    for (int i = 0; i < NUM_SAMPLES; i++) {
        uint32_t item = clock_return[i];
        printf("item \t%d:\t %lu\n", i, item);  // output wraps around at about 128
    }
    uint32_t frequency, now;

    // printf("bruh %lu %lu\n", frequency, now); // maybe getting reordered
    alarm_internal_frequency(&frequency);
    alarm_internal_read(&now);
    printf("hardware clock frequency: %lu, current tick: %lu\n", frequency, now);

    return 0; // exiting program does not flush console
}
