#ifndef __LED_H_
#define __LED_H_
#include "sys.h"

#define LED0(x)                                                                \
  x ? gpio_bit_write(GPIOF, GPIO_PIN_9, SET)                                   \
    : gpio_bit_write(GPIOF, GPIO_PIN_9, RESET);

#define LED1(x)                                                                \
  x ? gpio_bit_write(GPIOF, GPIO_PIN_10, SET)                                  \
    : gpio_bit_write(GPIOF, GPIO_PIN_10, RESET);

#define LED_TOGGLE()                                                           \
  {                                                                            \
    gpio_bit_toggle(GPIOF, GPIO_PIN_9);                                        \
    gpio_bit_toggle(GPIOF, GPIO_PIN_10);                                       \
  }

#define LED0_TOGGLE()                                                          \
  { gpio_bit_toggle(GPIOF, GPIO_PIN_9); }

#define LED1_TOGGLE()                                                          \
  { gpio_bit_toggle(GPIOF, GPIO_PIN_10); }

void led_init();
#endif // !__LED_H_
