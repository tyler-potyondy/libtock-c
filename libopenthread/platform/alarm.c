#include <alarm.h>
#include <assert.h>
#include <internal/alarm.h>
#include <openthread/platform/alarm-milli.h>
#include <stdio.h>
#include <plat.h>
#include <timer.h>

static alarm_t alarm;
static tock_timer_t timer_wrap;
uint32_t wrap_value = 0;
uint32_t prev_time_value;

static void callback(int __attribute__((unused)) now, int __attribute__((unused)) interval, int __attribute__(
                       (unused)) arg2, void *aInstance) {
  otPlatAlarmMilliFired(aInstance);
}

// convert ms to physical clock ticks (this should be done using the timer_in method,
// but this works in the meantime)
static uint32_t milliToTicks(uint32_t milli) {
  uint32_t frequency;
  alarm_internal_frequency(&frequency);
  return (milli / 1000) * frequency + (milli % 1000) * (frequency / 1000);;
}

void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt){
  // printf("Starting alarm at %d, with interval %d\n", aT0, aDt);
  OT_UNUSED_VARIABLE(aInstance);
  // TODO: this implementation for the alarm is temporary. Future implementation
  // should use the `timer_in` functionality to set the alarm.

  // check that aT0 and aDt are both less than max
  // uint32_t. this is a temporary fix until we implement
  // the wrap around functionality
  if ((aT0 > UINT32_MAX) || (aDt > UINT32_MAX)) {
    assert(false);
    return;
  }

  // inputs aT0, aDt are in ms, but libtock call must be in clock ticks
  uint32_t ref = milliToTicks(aT0);
  uint32_t dt  = milliToTicks(aDt);

  // NOTE: This is a problem that will have to be fixed. Thread wants to
  // be able to set a 2^32 millisecond timer. However, when converting
  // milliseconds into clock ticks, the multiplication will overflow.
  //
  // OpenThread expects to be able to use the full 32 bit range in milliseconds.
  alarm_at(ref, dt, callback, (void *)aInstance, &alarm);
}

void otPlatAlarmMilliStop(otInstance *aInstance) {
  OT_UNUSED_VARIABLE(aInstance);
  alarm_cancel(&alarm);
}



static void wrap_time_upcall(int __attribute__((unused)) now, int __attribute__((unused)) interval, int __attribute__(
                       (unused)) arg2, void __attribute__((unused)) *_ud) {
                        otPlatAlarmMilliGetNow();
                        // get tick vals to set alarm for
}

void init_otPlatAlarm(void) {
	// get when wrap will occur
	// set initial time value
                        uint32_t frequency;
                        alarm_internal_frequency(&frequency);
                        //wrap_value = (UINT32_MAX) / frequency;
                        wrap_value = 500*1000;
			prev_time_value = otPlatAlarmMilliGetNow();
	// timer will wrap at 2^32 so we set an alarm to go off at 2^30 
	// (cancel/reset this if milligetnow called)
	// frequency / {2^32}
}


uint32_t otPlatAlarmMilliGetNow(void) {
	// gettimeasticks gives sec and us value
	// check obtained value against prior
	// if smaller then add 
	// wrap at 100s 
	// prev was 89 
	// now it is 3
	// need to be 103
	// prev was 189
	// now 4 need to be 204
	//
	// (189 / 100) + 1
	// ((prev / 100) + 1) * wrap + 
	//
	// set timer that is to fire prior to two wraps occuring (set time for ?)

  timer_cancel(&timer_wrap);
	// TODO: there is a bug in this implementation causing the timer value to 
  // wrap around 500,000 milliseconds. This may be an issue with the 
  // `gettimeasticks` function or in the logic here.
  struct timeval tv;
  gettimeasticks(&tv, NULL);    // second arg is unused


  uint32_t nowSeconds    = tv.tv_sec;
  uint32_t nowMicro      = tv.tv_usec;
  uint32_t nowMilli32bit = (nowSeconds * 1000) + (nowMicro / 1000);

    if (nowMilli32bit < prev_time_value) {
    		printf("wrap occured...\n:");
	    nowMilli32bit += ((prev_time_value / wrap_value) + 1) * wrap_value;
    }

    prev_time_value = nowMilli32bit;
  timer_in(wrap_value >> 2, wrap_time_upcall, NULL, &timer_wrap); 

  return nowMilli32bit;
}
