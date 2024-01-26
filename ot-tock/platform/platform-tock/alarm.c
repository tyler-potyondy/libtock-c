// TODO ADD COPYRIGHT ETC

#include<openthread/platform/alarm-micro.h>
#include<openthread/platform/alarm-milli.h>

#include<libtock/alarm.h>
// TODO: need to keep track of timers on my own
// in a static var, and have a callback which
// to tell OS to unblock?
static bool              alarmFired  = false;
static TimerHandle_t     alarmTimer  = NULL;
static alarm_t           alarm       = NULL;
static SemaphoreHandle_t mutexHandle = NULL; // TODO: Question: I am a
                                                // little fuzzy on where semaphores are
                                                // in the world. Is it built into C? Or 
                                                // does it require OS support through something
                                                // in libtock?

static void alarmTimerCallback(TimerHandle_t pxTimer)
{
    alarmFired = true;
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
    printf("alarm milli start\n");
    alarm_at(aT0, aDt, alarmTimerCallback, void*(0), &alarm); // TODO: check callback
}

void otPlatAlarmMilliStop(otInstance *aInstance){
    printf("alarm milli stop\n");
    alarm_cancel(alarm);
}

uint32_t otPlatAlarmMilliGetNow(void){
    printf("alarm milli get now\n");
    struct timeval tv;
    gettimeasticks(&tv, NULL); // second arg is unused
    
    nowMicro = tv->tv_usec;
    return nowMicro / 1000;
}

