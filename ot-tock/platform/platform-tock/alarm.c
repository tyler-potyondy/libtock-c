// TODO ADD COPYRIGHT ETC

#include <alarm.h>
#include <openthread/platform/alarm-micro.h>
#include <openthread/platform/alarm-milli.h>
#include <stdio.h>
#include <timer.h>

// Based on reference implementations (https://github.com/openthread/ot-nxp/blob/main/src/common/alarm_freertos.c and https://github.com/openthread/ot-nrf528xx/blob/main/src/src/alarm.c), it seems there should only be one timer.
static alarm_t alarm;


static void callback(int now, int interval, int arg2, void *aInstance) {
    printf("callback called\n");
    otPlatAlarmMilliFired(aInstance);
}

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
    OT_UNUSED_VARIABLE(aInstance);
    printf("alarm milli start\n");
    // DEBUGGING:
    printf("alarm: setting alarm %lums after %lums. It's currently %lu\n", aDt, aT0,
           otPlatAlarmMilliGetNow());
    // aDt = 10;
    int return_code = alarm_at(aT0, aDt, callback, (void *)aInstance, &alarm);  // TODO: check callback
                       // this needs to be in clock units, not ms

    printf("alarm returncode %d\n", return_code);
}

void otPlatAlarmMilliStop(otInstance *aInstance) {
    OT_UNUSED_VARIABLE(aInstance);
    printf("alarm milli stop\n");
    alarm_cancel(&alarm);
}

uint32_t otPlatAlarmMilliGetNow(void) {
    printf("alarm milli get now: ");
    struct timeval tv;
    gettimeasticks(&tv, NULL);  // second arg is unused
    
    uint32_t nowSeconds = tv.tv_sec;
    uint32_t nowMicro = tv.tv_usec;
    uint32_t nowMilli32bit = (nowSeconds * 1000) + (nowMicro / 1000);

    printf("%lu\n", nowMilli32bit);
    return nowMilli32bit;
    // return 0;
}
