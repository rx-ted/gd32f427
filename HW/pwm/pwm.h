
#ifndef __PWM_H_
#define __PWM_H_

#include <sys.h>
void timer13_pwm_init(uint16_t arr, uint16_t psc, uint32_t portx, uint32_t pin);
void timer13_set_compare0(uint32_t compare);
#endif // !__PWM_H_
