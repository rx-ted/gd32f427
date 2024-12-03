#include "led.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_rcu.h"

void led_init() {
  rcu_periph_clock_enable(RCU_GPIOF);
  gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
  gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_10);
  gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);
  LED0(1);
  LED1(1);
}
