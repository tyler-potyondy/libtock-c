// TODO ADD COPYRIGHT ETC

#include<openthread/platform/alarm-micro.h>
#include<openthread/platform/alarm-milli.h>

// TODO: need to keep track of timers on my own
// in a static var, and have a callback which
// to tell OS to unblock?

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
    // TODO
    static alarm_t alarm;
    alarm_at(aT0, aDt, aInstance, void*(0), &alarm); // TODO: check callback
}

void otPlatAlarmMilliStop(otInstance *aInstance){
    printf("alarm milli stop\n");
    alarm_cancel(aInstance);
}

uint32_t otPlatAlarmMilliGetNow(void){
    printf("alarm milli get now\n");
    struct timeval tv;
    gettimeasticks(&tv, NULL); // second arg is unused
    
    nowMicro = tv->tv_usec;
    return nowMicro / 1000;
}

