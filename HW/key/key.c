#include "key.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"

void key_init() {
  rcu_periph_clock_enable(RCU_GPIOE);
  rcu_periph_clock_enable(RCU_GPIOA);
  gpio_mode_set(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_4);
  gpio_mode_set(GPIOA, GPIO_MODE_INPUT, GPIO_PUPD_PULLDOWN, GPIO_PIN_0);
}
uint8_t key_scan(uint8_t mode) {
  /*
   * TODO(rx-ted): Support press down,
   * via 1 times, continously
   */
  return NULL;
}
