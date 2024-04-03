#include "alarm.h"
#include "internal/alarm.h"
#include "timer.h"
#include <assert.h>
#include <limits.h>
#include <stdlib.h>

// #define MAX_TICKS ~0b0U
#define MAX_TICKS 40000

// Returns 0 if a <= b < c, 1 otherwise
static int within_range(uint32_t a, uint32_t b, uint32_t c) {
  return (b - a) < (b - c);
}

static alarm_t* root = NULL;

static void root_insert(alarm_t* alarm) {
  if (root == NULL) {
    root       = alarm;
    root->next = NULL;
    root->prev = NULL;
    return;
  }

  alarm_t **cur = &root;
  alarm_t *prev = NULL;
  while (*cur != NULL) {
    uint32_t cur_expiration = (*cur)->reference + (*cur)->dt;
    uint32_t new_expiration = alarm->reference + alarm->dt;
    if (!within_range(alarm->reference, cur_expiration, new_expiration)) {
      // insert before
      alarm_t *tmp = *cur;
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

static alarm_t* root_pop(void) {
  if (root == NULL) {
    return NULL;
  } else {
    alarm_t *res = root;
    root = root->next;
    if (root != NULL) {
      root->prev = NULL;
    }
    res->next = NULL;
    return res;
  }
}

static alarm_t* root_peek(void) {
  return root;
}

static void callback( __attribute__ ((unused)) int   unused0,
                      __attribute__ ((unused)) int   unused1,
                      __attribute__ ((unused)) int   unused2,
                      __attribute__ ((unused)) void* ud) {
  for (alarm_t* alarm = root_peek(); alarm != NULL; alarm = root_peek()) {
    uint32_t now;
    alarm_internal_read(&now);
    // has the alarm not expired yet? (distance from `now` has to be larger or
    // equal to distance from current clock value.
    if (alarm->dt > now - alarm->reference) {
      alarm_internal_set(alarm->reference, alarm->dt);
      break;
    } else {
      root_pop();

      if (alarm->callback) {
        uint32_t expiration = alarm->reference + alarm->dt;
        tock_enqueue(alarm->callback, now, expiration, 0, alarm->ud);
      }
    }
  }
}

int alarm_at(uint32_t reference, uint32_t dt, subscribe_upcall cb, void* ud, alarm_t* alarm) {
  alarm->reference = reference;
  alarm->dt        = dt;
  alarm->callback  = cb;
  alarm->ud        = ud;
  alarm->prev      = NULL;
  alarm->next      = NULL;

  root_insert(alarm);
  int i = 0;
  for (alarm_t* cur = root_peek(); cur != NULL; cur = cur->next) {
    i++;
  }

  if (root_peek() == alarm) {
    alarm_internal_subscribe((subscribe_upcall*)callback, NULL);

    return alarm_internal_set(alarm->reference, alarm->dt);
  }
  return RETURNCODE_SUCCESS;
}

inline uint32_t ticks_to_ms(uint32_t ticks) {
  uint32_t frequency;
  alarm_internal_frequency(&frequency);

  return (ticks / frequency) * 1000;
}

inline uint32_t ms_to_ticks(uint32_t ms) {
  uint32_t frequency;
  alarm_internal_frequency(&frequency);

  uint32_t millihertz = frequency / 1000; // ticks per millisecond
  uint32_t seconds = ms / 1000;
  uint32_t leftover_millis = ms % 1000;
  return seconds * frequency + leftover_millis * millihertz;
}

// user data to pass down if we are handling an alarm
// that would normally overflow
typedef struct overflow_ud {
  // how many times the underlying alarm has left to overflow
  // before we reach our alarm target
  int overflows_left;
  // number of remaining ticks after the alarm is done overflowing
  int remainder_ticks;
  // need to have an unused
  __attribute__ ((unused)) int unused;
  // original user data passed into alarm_at_ms
  void* original_ud;
  subscribe_upcall* original_cb;
  alarm_t* alarm;
} overflow_ud_t;

static void overflow_callback(int                          last_timer_fire_time,
                              __attribute__ ((unused)) int unused1,
                              __attribute__ ((unused)) int unused2,
                              void*                        ud) {

  overflow_ud_t* our_ud = (overflow_ud_t*)ud;
  printf("\tunused is %d\n", our_ud->unused);

  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
  if (our_ud->overflows_left == 0) {
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    printf("\tno more overflows left\n");
    alarm_at(
      last_timer_fire_time,
      our_ud->remainder_ticks,
      our_ud->original_cb,
      our_ud->original_ud,
      our_ud->alarm
      );
  } else {
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    printf("\t%d overflows left\n", our_ud->overflows_left);

    our_ud->overflows_left--;

    printf("last timer fire time %d\n", last_timer_fire_time);

    // TODO: board crashes after setting another alarm for the next overflow event.
    // Not sure why, but it could be we're passing something wrong to alarm_at here.
    alarm_at(
      last_timer_fire_time,
      MAX_TICKS,
      overflow_callback,
      our_ud,
      our_ud->alarm
      );
  }
}

/*
   reference time─────────────── ──┐
                                 │
   ┌─────────────────────────┐     │
   │                         │     │
   │  Kernel context         │     │ `dt`
   │  switches to another    │     │ delta time -
   │  process                │     │   length the timer
   │                         │     │   should run for
   │                         │   ──┘
   │                         │
   └─────────────────────────┘
   Timer gets started───────────
 ******Timer should not run for dt, instead return immediately*********
 */


// Timer implementation
int timer_in(uint32_t dt_ms, subscribe_upcall cb, void* ud, tock_timer_t *tock_timer) {
  alarm_t* alarm = &tock_timer->alarm;
  /**
   * TODO: add a comment explaining how we are handling overflow with a chain of callbacks
   */

  overflow_ud_t* tmp_ud = malloc(sizeof(overflow_ud_t));
  tmp_ud->overflows_left  = dt_ms / ticks_to_ms(MAX_TICKS);
  tmp_ud->remainder_ticks = dt_ms % ticks_to_ms(MAX_TICKS);
  tmp_ud->original_ud     = ud;
  tmp_ud->original_cb     = cb;
  tmp_ud->alarm = alarm;

  printf("dt_ms: %ld, max_ms: %ld\n", dt_ms, ticks_to_ms(MAX_TICKS));
  printf("struct has %d overflows_left\n", tmp_ud->overflows_left);
  printf("struct has %d remainder_ticks\n", tmp_ud->remainder_ticks);

  uint32_t now;
  int ret = alarm_internal_read(&now);
  assert(ret == RETURNCODE_SUCCESS);
  printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);

  /**
   * The counter can only count up to 2^32 ticks. We might want to set a counter that's
   * more than 2^32 ticks. Here, we check if we want to set a timer in ms that is more than
   * 2^32 ticks.
   *
   * DELETEME: `alarm_at_ms` is going to ignore the passed in `reference_ms`.
   * If a timer is in milliseconds, then context switching overhead is negligible.
   * We do not handle the case of overshooting `dt_ms` by checking against the reference time.
   * There is no free runnning millisecond timer, so we cannot check if the time in ticks has
   * overflowed since the reference time was checked. (refer to above diagram)
   * It is user error to hold onto a timer and not schedule it.
   */
  if (dt_ms > ticks_to_ms(MAX_TICKS)) {
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return alarm_at(now, MAX_TICKS, (subscribe_upcall*)overflow_callback, (void*)(&tmp_ud), alarm);
  } else {
    // No overflows needed
    printf("%s:%d in %s\n", __FILE__, __LINE__, __func__);
    return alarm_at(now, ms_to_ticks(dt_ms), cb, ud, alarm);
  }
}

void alarm_cancel(alarm_t* alarm) {
  if (alarm->prev != NULL) {
    alarm->prev->next = alarm->next;
  }
  if (alarm->next != NULL) {
    alarm->next->prev = alarm->prev;
  }

  if (root == alarm) {
    root = alarm->next;
    if (root != NULL) {
      alarm_internal_set(root->reference, root->dt);
    }
  }

  alarm->prev = NULL;
  alarm->next = NULL;

}

static void repeating_upcall( uint32_t                     now,
                              __attribute__ ((unused)) int unused1,
                              __attribute__ ((unused)) int unused2,
                              void*                        ud) {
  tock_timer_t* repeating = (tock_timer_t*)ud;
  uint32_t interval       = repeating->interval;
  uint32_t cur_exp        = repeating->alarm.reference + interval;
  alarm_at(cur_exp, interval, (subscribe_upcall*)repeating_upcall,
           (void*)repeating, &repeating->alarm);
  repeating->cb(now, cur_exp, 0, repeating->ud);
}

void timer_every(uint32_t ms, subscribe_upcall cb, void* ud, tock_timer_t* repeating) {
  uint32_t frequency;
  alarm_internal_frequency(&frequency);
  uint32_t interval = (ms / 1000) * frequency + (ms % 1000) * (frequency / 1000);

  repeating->interval = interval;
  repeating->cb       = cb;
  repeating->ud       = ud;

  uint32_t now;
  alarm_internal_read(&now);
  alarm_at(now, interval, (subscribe_upcall*)repeating_upcall,
           (void*)repeating, &repeating->alarm);
}

void timer_cancel(tock_timer_t* timer) {
  alarm_cancel(&timer->alarm);
}

static void delay_upcall(__attribute__ ((unused)) int unused0,
                         __attribute__ ((unused)) int unused1,
                         __attribute__ ((unused)) int unused2,
                         void*                        ud) {
  *((bool*)ud) = true;
}

int delay_ms(uint32_t ms) {
  bool cond = false;
  tock_timer_t timer;
  int rc;

  if ((rc = timer_in(ms, delay_upcall, &cond, &timer)) != RETURNCODE_SUCCESS) {
    return rc;
  }

  yield_for(&cond);
  return rc;
}

static void yield_for_timeout_upcall(__attribute__ ((unused)) int unused0,
                                     __attribute__ ((unused)) int unused1,
                                     __attribute__ ((unused)) int unused2,
                                     void*                        ud) {
  *((bool*)ud) = true;
}

int yield_for_with_timeout(bool* cond, uint32_t ms) {
  bool timeout = false;
  tock_timer_t timer;
  timer_in(ms, yield_for_timeout_upcall, &timeout, &timer);

  while (!*cond) {
    if (timeout) {
      return RETURNCODE_FAIL;
    }

    yield();
  }

  timer_cancel(&timer);
  return RETURNCODE_SUCCESS;
}

int gettimeasticks(struct timeval *tv, __attribute__ ((unused)) void *tzvp)
{
  uint32_t frequency, now, seconds, remainder;

  alarm_internal_frequency(&frequency);
  alarm_internal_read(&now);

  seconds   = now / frequency;
  remainder = now % frequency;

  tv->tv_sec  = seconds;
  tv->tv_usec = (remainder * 1000 * 1000) / frequency;

  return 0;
}
