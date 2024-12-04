#include "hw.h"

void setup() {
  systick_config();

  gd_eval_led_init(LED1);
  gd_eval_led_init(LED0);
}
void loop() {
  gd_eval_led_on(LED0);
  gd_eval_led_on(LED1);
  delay_ms(500);

  gd_eval_led_off(LED0);
  gd_eval_led_off(LED1);
  delay_ms(500);
}

void hw() {
  setup();
  while (1)
    loop();
}
