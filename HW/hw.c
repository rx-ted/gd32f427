#include "hw.h"

void hw()
{
  while (1)
  {
    gd_eval_led_on(0);
    gd_eval_led_off(1);
    delay_ms(100);
    gd_eval_led_on(1);
    gd_eval_led_off(0);
    delay_ms(100);
  }
}
