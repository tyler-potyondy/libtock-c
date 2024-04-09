#include "timer.h"
#include "internal/alarm.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>


/*
 * This test registers a "long running" timer and verifies that the 
 * timer fires at about the time when it's expected to.
 *
 * NOTE: Modify the "MAX_TICKS" macro in "libtock/alarm_timer.c" to 
 * test the implementation of timer_in without waiting for
 * the alarm to overflow after 2^32 ticks (which is about 36 hours 
 * with a 32768Hz oscillator).
 */

#define TIMER_LEN_MS 2000

// helper struct to send data to callback
typedef struct testing_ud {
    uint32_t timer_start_ticks;
    uint32_t timer_len_ms;
    uint32_t expected_timer_end_ticks;
} testing_ud_t;

static void timer_fire_callback(int                        last_timer_fire_ticks,
                              __attribute__ ((unused)) int unused1,
                              __attribute__ ((unused)) int unused2,
                              void*                        ud) {
    testing_ud_t* test_ud = (testing_ud_t*)ud; 
    printf("Timer Fired!\n");
    printf("\tExpected timer to fire at %ld ticks\n", test_ud->expected_timer_end_ticks);
    printf("\tActually fired at %d ticks\n", last_timer_fire_ticks);
    printf("\tDifference of %ld ticks\n", test_ud->expected_timer_end_ticks - last_timer_fire_ticks);
}

inline static uint32_t ms_to_ticks(uint32_t ms) {
  uint32_t frequency;
  alarm_internal_frequency(&frequency);

  uint32_t millihertz      = frequency / 1000; // ticks per millisecond
  uint32_t seconds         = ms / 1000;
  uint32_t leftover_millis = ms % 1000;
  return (seconds * frequency) + (leftover_millis * millihertz);
}

int main(int argc, char** argv)
{
    // need to allocate memory for a timer before giving a pointer to `timer_in`
    tock_timer_t* timer = malloc(sizeof(tock_timer_t));
    
    uint32_t now_ticks;
    alarm_internal_read(&now_ticks);
    testing_ud_t* ud = malloc(sizeof(testing_ud_t));
    ud->timer_len_ms = TIMER_LEN_MS;
    ud->timer_start_ticks = now_ticks;
    ud->expected_timer_end_ticks = now_ticks + ms_to_ticks(TIMER_LEN_MS);

    printf("Setting timer to fire at %ld ticks\n", ud->expected_timer_end_ticks);
    timer_in(TIMER_LEN_MS, timer_fire_callback, (void*)ud, timer);

    // yield back to the kernel because otherwise timer callback won't be scheduled
    yield();
    // TODO: For some reason this print is needed. Othwerwise the timer callback never
    // gets scheduled even with the yield in the previous line
    printf(":(\n");
}
