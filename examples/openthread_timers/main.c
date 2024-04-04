#include <assert.h>
#include <openthread/dataset_ftd.h>
#include <openthread/instance.h>
#include <openthread/platform/alarm-milli.h>
#include <stdio.h>
#include <timer.h>


static void overflow_callback(int                          last_timer_fire_time,
                              __attribute__ ((unused)) int unused1,
                              __attribute__ ((unused)) int unused2,
                              void*                        ud) {

  printf("\tLETS GO OOOOOOO\n");
  // TODO: write some documentation that you have to malloc timer struct and should free it in your callback
}

volatile int x = 0;
static void busy_wait(int num) {
    int loop = 0;
    while (loop < num) {
        loop++;
        x = loop;
    }
}

int main(int argc, char *argv[])
{
    busy_wait(10000000);
    // TODO: ask why you have to malloc this. It should not go out of scope on stack?
    tock_timer_t* timer = malloc(sizeof(tock_timer_t));
    timer_in(2000, overflow_callback, NULL, timer);
    printf("time is %dms\n", time);
    busy_wait(10000000);
    printf("time is %dms\n", time);
    // while (1){} // TODO: this nevers yields back to the kernel for some reason.
    // Why does the kernel not interrupt this process?
}

