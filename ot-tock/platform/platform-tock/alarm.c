// TODO ADD COPYRIGHT ETC

#include<openthread/platform/alarm-micro.h>
#include<openthread/platform/alarm-milli.h>

#include<alarm.h>
#include<timer.h>
#include<stdio.h>
// TODO: need to keep track of timers on my own
// in a static var, and have a callback which
// to tell OS to unblock?
static bool              alarmFired  = false;
// static TimerHandle_t     alarmTimer  = NULL;
static alarm_t           alarm       = {};
// static SemaphoreHandle_t mutexHandle = NULL; // TODO: Question: I am a
                                                // little fuzzy on where semaphores are
                                                // in the world. Is it built into C? Or 
                                                // does it require OS support through something
                                                // in libtock?

static void alarmTimerCallback()//(TimerHandle_t pxTimer)
{
    alarmFired = true;
    // otPlatAlarmMilliFired(aInstance);
}

// TODO: #include <libtock/alarm.h>

// NOTE: Micro is not necessary

// void otPlatAlarmMicroStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
//     printf("alarm micro start\n");
//     static alarm_t alarm;
//     alarm_at(aT0, aDt, aInstance, void*(0), &alarm); // TODO: check callback
// }

// void otPlatAlarmMicroStop(otInstance *aInstance){
//     printf("alarm micro stop\n");
//     alarm_cancel(aInstance);
// }

// uint32_t otPlatAlarmMicroGetNow(void){
//     printf("alarm micro get now\n");
//     // TODO
//     return 0;
// }

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
    OT_UNUSED_VARIABLE(aInstance);
    printf("alarm milli start\n");
    // DEBUGGING:
    printf("alarm: setting alarm %lums after %lums. It's currently %lu\n", aDt, aT0, otPlatAlarmMilliGetNow());
    aDt = 10;
    delay_ms(1000);
    otPlatAlarmMilliFired(aInstance);
    alarm_at(aT0, aDt, otPlatAlarmMilliFired, (void*) aInstance, &alarm); // TODO: check callback
                                                                          // this needs to be in clock units, not ms
}

void otPlatAlarmMilliStop(otInstance *aInstance){
    OT_UNUSED_VARIABLE(aInstance);
    printf("alarm milli stop\n");
    alarm_cancel(&alarm);
}

uint32_t otPlatAlarmMilliGetNow(void){
    printf("alarm milli get now: ");
    struct timeval tv;
    gettimeasticks(&tv, NULL); // second arg is unused
    
    uint32_t nowSeconds = tv.tv_sec;
    uint32_t nowMicro = tv.tv_usec;
    uint32_t nowMilli32bit = (nowSeconds * 1000) + (nowMicro / 1000);

    printf("%lu\n", nowMilli32bit);
    return nowMilli32bit;
}
