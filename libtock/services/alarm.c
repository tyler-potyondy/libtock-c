#include "alarm.h"
#include <limits.h>
#include <stdlib.h>

// Returns 0 if a <= b < c, 1 otherwise
static int within_range(uint32_t a, uint32_t b, uint32_t c) {
  return (b - a) < (b - c);
}

/** \brief Convert milliseconds to clock ticks
 *
 * WARNING: This function will silently overflow if the output
 * number of ticks is larger than `UINT32_MAX`.
 *
 * This conversion is accurate to within 1 millisecond of a true
 * fractional conversion.
 *
 * \param ms the milliseconds to convert to ticks
 * \return ticks a number of clock ticks that
 * correspond to the given number of milliseconds
 */
static uint32_t ms_to_ticks(uint32_t ms) {
  uint32_t frequency;
  libtock_alarm_command_get_frequency(&frequency);

  // Note the following code is mathematically equivalent to the following
  // more readable conversion.
  // ```
  //   uint32_t seconds         = ms / 1000;
  //   uint32_t leftover_millis = ms % 1000;
  //   uint32_t millihertz      = frequency / 1000; // ticks per millisecond
  //   return (seconds * frequency) + (leftover_millis * millihertz);
  // ```
  // We can check the logical correctness of this conversion by doing dimensional
  // analysis of the units.
  // For the first half, `seconds` (sec) * `frequency` (ticks per second) = a quantity in ticks.
  // And for the second half, `leftover_millis` (milliseconds) * `millihertz` (ticks per millisecond)
  // = a quantity in ticks.
  // Thus, we know the calculation is logically correct as the units cancel out to return ticks.
  //
  // Although these are logically equivalent, this human readable implementation is
  // faulty. I will prove this by (1.) showing an upper bound on the error of this conversion,
  // and then (2.) explaining why this commented human readable implementation is
  // equivalent to the true implementation.
  //
  // Part 1:
  // The splitting of the input milliseconds has no error.
  uint32_t seconds         = ms / 1000;
  uint32_t leftover_millis = ms % 1000;
  // In other words, `seconds` + `leftover_millis` is always == `ms`.

  // The first half of the output converts the seconds component to ticks with no error.
  // This multiplication has no error because the board frequency is given in Hertz (ticks per second).
  uint32_t ticks = seconds * frequency;

  // Now that we have converted the full seconds part of `ms` to ticks, we must
  // convert the `leftover_millis`.
  // The reason for splitting this conversion rather than doing a direct conversion
  // is due to 32-bit overflow. `leftover_millis` can only take values in the range
  // [0, 1000). Thus, this multiplication is upper bounded by `frequency`*1000. This
  // calculation will not overflow as long as
  //   `frequency`*1000 < `UINT32_MAX` -> frequency <= 4,294,967
  // If this multiplication was performed on the whole input `ms`, it would overflow
  // for large inputs. The only error introduced is by the division step. Division loses
  // all fractional components, so you lose 3 decimal significant figures by doing the division.
  // This does not matter as the remainder lost to integer division is a fraction of a
  // millisecond. It is especially important that this conversion is lower than the
  // true value of the conversion. In summary, the error in `ms_to_ticks` is from losing
  // significant figures, but this loss is less than 1 millisecond of error.
  //
  // Note that the division happens after the multiplication. Let us take a look at a
  // concrete example of a worst case conversion.
  // For a board with an oscillator frequency of 32,768Hz (NRF52840dk), observe the
  // error introduced by arithmetic in the "human readable" implementation of this
  // same line of the code:
  //   uint32_t millihertz      = frequency / 1000; // ticks per millisecond
  //   ticks += leftover_millis * millihertz;
  //
  //   // The worst case is when the remainder `leftover_millis` is `frequency` - 1
  //   ticks += 32767 * millihertz;
  // But what is the value of millihertz here? It is `32768 / 1000` = `32`! This is
  // 2.4% error, has only 2 significant figures, and, worst of all, the division causes the
  // denominator to be smaller than it should be. Dividing by a smaller number causes the
  // entire value of `ticks` to be an overestimate. This is a critical correctness issue.
  // Let's trace an example execution of calculating overflows in `timer_in`. The code uses
  // this line to calculate how many overflows are needed.
  //     const uint32_t max_ticks_in_ms = ticks_to_ms(MAX_TICKS);
  // If `ticks_to_ms` is an overestimate, then a timer set for slightly longer than the
  // true value of `MAX_VALUE` in ms will cause the check for `length of timer` <
  // `max_ticks_in_ms` to be true. This incorrect check will set an overflowed timer and fire
  // too early.
  uint32_t milliseconds_per_second = 1000;
  ticks += (leftover_millis * frequency) / milliseconds_per_second;

  // Part 2:
  // The human readable and actual implementation are logically equivalent.
  // The only difference is the location of the division by 1000.
  //   leftover_millis * millihertz = leftover_millis * (frequency / 1000)
  //   = leftover_millis * frequency / 1000 = (leftover_millis * frequency) / 1000
  // Thus, by a series of equivalences, these are doing the same calculation.

  return ticks;
}

static libtock_alarm_t* root = NULL;

static void root_insert(libtock_alarm_t* alarm) {
  if (root == NULL) {
    root       = alarm;
    root->next = NULL;
    root->prev = NULL;
    return;
  }

  libtock_alarm_t** cur = &root;
  libtock_alarm_t* prev = NULL;
  while (*cur != NULL) {
    uint32_t cur_expiration = (*cur)->reference + (*cur)->dt;
    uint32_t new_expiration = alarm->reference + alarm->dt;
    if (!within_range(alarm->reference, cur_expiration, new_expiration)) {
      // insert before
      libtock_alarm_t* tmp = *cur;
      *cur        = alarm;
      alarm->next = tmp;
      alarm->prev = prev;
      tmp->prev   = alarm;
      return;
    }
    prev = *cur;
    cur  = &prev->next;
  }
  // didn't return, so prev points to the last in the list
  prev->next  = alarm;
  alarm->prev = prev;
  alarm->next = NULL;

}

static libtock_alarm_t* root_pop(void) {
  if (root == NULL) {
    return NULL;
  } else {
    libtock_alarm_t* res = root;
    root = root->next;
    if (root != NULL) {
      root->prev = NULL;
    }
    res->next = NULL;
    return res;
  }
}

static libtock_alarm_t* root_peek(void) {
  return root;
}

static void alarm_upcall(__attribute__ ((unused)) int   kernel_now,
                         __attribute__ ((unused)) int   scheduled,
                         __attribute__ ((unused)) int   unused2,
                         __attribute__ ((unused)) void* opaque) {
  for (libtock_alarm_t* alarm = root_peek(); alarm != NULL; alarm = root_peek()) {
    uint32_t now;
    libtock_alarm_command_read(&now);
    // has the alarm not expired yet? (distance from `now` has to be larger or
    // equal to distance from current clock value.
    if (alarm->dt > now - alarm->reference) {
      libtock_alarm_command_set_absolute(alarm->reference, alarm->dt);
      break;
    } else {
      root_pop();

      if (alarm->callback) {
        uint32_t expiration = alarm->reference + alarm->dt;
        alarm->callback(now, expiration, alarm->ud);
      }
    }
  }
}

static int libtock_alarm_at_internal(uint32_t reference, uint32_t dt, libtock_alarm_callback cb, void* ud,
                                     libtock_alarm_t* alarm) {
  alarm->reference = reference;
  alarm->dt        = dt;
  alarm->callback  = cb;
  alarm->ud        = ud;
  alarm->prev      = NULL;
  alarm->next      = NULL;

  root_insert(alarm);
  int i = 0;
  for (libtock_alarm_t* cur = root_peek(); cur != NULL; cur = cur->next) {
    i++;
  }

  if (root_peek() == alarm) {
    libtock_alarm_set_upcall((subscribe_upcall*)alarm_upcall, NULL);

    return libtock_alarm_command_set_absolute(alarm->reference, alarm->dt);
  }
  return RETURNCODE_SUCCESS;
}

int libtock_alarm_at(uint32_t reference, uint32_t dt, libtock_alarm_callback cb, void* opaque, libtock_alarm_t* alarm) {
  return libtock_alarm_at_internal(reference, dt, cb, opaque, alarm);
}

void libtock_alarm_cancel(libtock_alarm_t* alarm) {
  if (alarm->prev != NULL) {
    alarm->prev->next = alarm->next;
  }
  if (alarm->next != NULL) {
    alarm->next->prev = alarm->prev;
  }

  if (root == alarm) {
    root = alarm->next;
    if (root != NULL) {
      libtock_alarm_command_set_absolute(root->reference, root->dt);
    }
  }

  alarm->prev = NULL;
  alarm->next = NULL;

}

int libtock_alarm_in_ms(uint32_t ms, libtock_alarm_callback cb, void* opaque, libtock_alarm_t* alarm) {
  uint32_t interval = ms_to_ticks(ms);
  uint32_t now;
  libtock_alarm_command_read(&now);
  return libtock_alarm_at(now, interval, cb, opaque, alarm);
}

static void alarm_repeating_cb(uint32_t now, __attribute__ ((unused)) uint32_t scheduled, void* opaque) {
  libtock_alarm_repeating_t* repeating = (libtock_alarm_repeating_t*) opaque;
  uint32_t interval = repeating->interval;
  uint32_t cur_exp  = repeating->alarm.reference + interval;
  libtock_alarm_at_internal(cur_exp, interval, (libtock_alarm_callback)alarm_repeating_cb,
                            (void*)repeating, &repeating->alarm);
  repeating->cb(now, cur_exp, repeating->ud);
}


void libtock_alarm_repeating_every(uint32_t ms, libtock_alarm_callback cb, void* opaque,
                                   libtock_alarm_repeating_t* repeating) {
  uint32_t interval = ms_to_ticks(ms);

  repeating->interval = interval;
  repeating->cb       = cb;
  repeating->ud       = opaque;

  uint32_t now;
  libtock_alarm_command_read(&now);
  libtock_alarm_at_internal(now, interval, alarm_repeating_cb,
                            (void*)repeating, &repeating->alarm);
}

void libtock_alarm_repeating_cancel(libtock_alarm_repeating_t* repeating) {
  libtock_alarm_cancel(&repeating->alarm);
}

int libtock_alarm_gettimeasticks(struct timeval* tv, __attribute__ ((unused)) void* tzvp) {
  uint32_t frequency, now, seconds, remainder;
  const uint32_t microsecond_scaler = 1000000;

  libtock_alarm_command_get_frequency(&frequency);
  libtock_alarm_command_read(&now);

  assert(frequency > 0);

  // Obtain seconds and remainder due to integer divison
  seconds   = now / frequency;
  remainder = now % frequency;

  // (ticks) * (1e6 us / s) * (s / ticks) = us
  // Because remainder is by definition less than
  // frequency, we must be sure to first scale remainder
  // or else floor(remainder / frequency) = 0. To prevent
  // an overflow, we cast remainder to be a `uint64_t`. By
  // integer promotion rules, microsecond_scaler and frequency
  // will subsequently be cast to `uint64_t`.
  uint64_t us = ((uint64_t) remainder * microsecond_scaler) / frequency;

  tv->tv_sec = seconds;

  // The maximum micro second value is 999999us,
  // as any value greater than this is a second.
  // Subsequently, we can safely cast the uint64_t
  // to a uint32_t.
  tv->tv_usec = (uint32_t) us;

  return 0;
}
