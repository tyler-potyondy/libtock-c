#include <screen.h>
#include <stdio.h>
#include <stdlib.h>
#include <timer.h>
#include <led.h>

#include <u8g2.h>

#define BUFFER_SIZE 10 * 1024


u8g2_t u8g2;


int main(void) {
  int err;

  led_on(0);


  u8g2_tock_init(&u8g2);


  u8g2_ClearBuffer(&u8g2);



  u8g2_DrawLine(&u8g2, 50, 60, 120, 64);


  u8g2_SetFont(&u8g2, u8g2_font_6x12_tf);
  u8g2_DrawStr(&u8g2, 0,10, "TOCK");
  u8g2_SetFont(&u8g2, u8g2_font_bubble_tr);
  u8g2_DrawStr(&u8g2, 0,50, "SECURE");

  u8g2_SendBuffer(&u8g2);

  printf("screen should be updated");

}
