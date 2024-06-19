#include <libtock-sync/services/alarm.h>
#include <libtock/interface/led.h>

struct alarm_cb_data {
  bool fired;
};

static struct alarm_cb_data data = { .fired = false };

static void alarm_cb(__attribute__ ((unused)) uint32_t now,
                     __attribute__ ((unused)) uint32_t scheduled,
                     __attribute__ ((unused)) void*    opaque) {
  data.fired = true;
}

int main(void) {

  while (1) {
    data.fired = false;
    libtock_alarm_data_t alarm;
    libtock_alarm_in_ms(1500, alarm_cb, NULL, &alarm);

    int ret = libtocksync_alarm_yield_for_with_timeout(&data.fired, 1000);
    if (ret == RETURNCODE_SUCCESS) {
      libtock_led_on(0);
    } else {
      libtock_led_off(0);
    }

    ret = libtocksync_alarm_yield_for_with_timeout(&data.fired, 1000);
    if (ret == RETURNCODE_SUCCESS) {
      libtock_led_on(0);
    } else {
      libtock_led_off(0);
    }
  }
}
