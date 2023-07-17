#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <console.h>
#include <kv_system.h>
#include <usb_keyboard_hid.h>

#define KEY_LEN  22
#define VAL_LEN 64

// For K-V store.
uint8_t key_buf[KEY_LEN];
uint8_t value_buf[VAL_LEN];

// For USB keyboard.
uint8_t keyboard_buffer[64];



int main(void) {
  int ret;

  printf("[KV to Keyboard] Write a key-value value to a USB keyboard\n");
  printf("\n");

  char key[] = "k2k";

  // Copy key to our buffer.
  strcpy((char*) key_buf, key);

  // Retrieve the value.
  uint32_t value_len;
  ret = kv_system_get_sync(key_buf, strlen(key), value_buf, VAL_LEN, &value_len);
  if (ret < 0) {
    printf("Could not get value: %i\n", ret);
    return -1;
  } else {
    printf("Got value: ");
    for (int i = 0; i < (int) value_len; i++) {
      printf("%c", value_buf[i]);
    }
    printf("\n");
  }

  // Write the value to the USB keyboard.
  value_buf[value_len] = '\0';
  ret = usb_keyboard_hid_send_string_sync(keyboard_buffer, (char*) value_buf, value_len);
  if (ret < 0) {
    printf("ERROR sending string with USB keyboard HID: %i\n", ret);
  }


  return 0;
}
