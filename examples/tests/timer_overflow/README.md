Test Timer Overflow
====================

This tests the libtock timer implementation's ability to handle situations
where setting a timer would overflow the underlying hardware counter.

The userspace `timer_in` function expects a number of milliseconds when a timer
should fire in the future. This argument is a `uint32_t` which means that an
alarm should be able to be set 2^32 ms in the future (about 49 days). However,
`timer_in` calls to `alarm_at` which can only set alarms up to 2^32 clock ticks
(only about 36 hours with a 32768Hz oscillator). Therefore, `timer_in` has been
modified to handle the underlying overflow of the alarm and make the timer appear
as if it can set longer timers. Under the hood, it keeps track of how many times
the alarm needs to overflow until the target time is reached (how many 2^32 tick
intevals need to pass). Only after the last alarm does it call the original callback
provided to `timer_in` while hiding the effects of the intermediate alarms that
were scheduled.
