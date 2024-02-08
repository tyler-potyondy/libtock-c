#pragma once

#include "tock.h"

#define DRIVER_NUM_NONVOLATILE_STORAGE 0x50001

#ifdef __cplusplus
extern "C" {
#endif

/*
* Set a callback to be called after reads.
*/
int nonvolatile_storage_internal_read_done_subscribe(subscribe_upcall cb, void *userdata);
/*
* Set a callback to be called after writes.
*/
int nonvolatile_storage_internal_write_done_subscribe(subscribe_upcall cb, void *userdata);

/*
* Set the buffer for read data will be read from.
*/
int nonvolatile_storage_internal_read_buffer(uint8_t* buffer, uint32_t len);
/*
* Set the buffer for writes to output to.
*/
int nonvolatile_storage_internal_write_buffer(uint8_t* buffer, uint32_t len);

/*
* Get the size of the flash.
*/
int nonvolatile_storage_internal_get_number_bytes(int* number_bytes);
/*
* Get the size of the flash.
*/
int nonvolatile_storage_internal_read(uint32_t offset, uint32_t length);
/*
* Get the size of the flash.
*/
int nonvolatile_storage_internal_write(uint32_t offset, uint32_t length);

#ifdef __cplusplus
}
#endif
