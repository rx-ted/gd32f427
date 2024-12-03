#ifndef __KEY_H_
#define __KEY_H_

#include <stdint.h>
#define KEY0 gpio_input_bit_get(GPIOE, GPIO_PIN_4)
#define KEY_WEAK_UP gpio_input_bit_get(GPIOA, GPIO_PIN_0)

void key_init();
uint8_t key_scan(uint8_t mode);

#endif // !__KEY_H
