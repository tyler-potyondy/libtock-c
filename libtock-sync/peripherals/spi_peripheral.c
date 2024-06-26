#include "spi_peripheral.h"

struct spi_peripheral_data {
  bool fired;
  returncode_t ret;
};

static struct spi_peripheral_data result = { .fired = false };


static void cb(returncode_t ret) {
  result.fired = true;
  result.ret   = ret;
}

returncode_t libtocksync_spi_peripheral_write(const uint8_t* write,
                                              size_t         len) {
  returncode_t err;
  result.fired = false;

  err = libtock_spi_peripheral_write(write, len, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  err = libtock_spi_peripheral_allow_readonly_write(NULL, 0);
  return err;
}

returncode_t libtocksync_spi_peripheral_read_write(const uint8_t* write,
                                                   uint8_t*       read,
                                                   size_t         len) {
  returncode_t err;
  result.fired = false;

  err = libtock_spi_peripheral_read_write(write, read, len, cb);
  if (err != RETURNCODE_SUCCESS) return err;

  yield_for(&result.fired);
  if (result.ret != RETURNCODE_SUCCESS) return result.ret;

  err = libtock_spi_peripheral_allow_readonly_write(NULL, 0);
  if (err != RETURNCODE_SUCCESS) return err;
  err = libtock_spi_peripheral_allow_readwrite_read(NULL, 0);
  if (err != RETURNCODE_SUCCESS) return err;

  return err;
}
