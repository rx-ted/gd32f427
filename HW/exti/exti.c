#include "exti.h"
#include "key.h"
#include "led.h"

void extix_init() {
  rcu_periph_clock_enable(RCU_SYSCFG);
  key_init();
  syscfg_exti_line_config(EXTI_SOURCE_GPIOA, EXTI_SOURCE_PIN0);
  syscfg_exti_line_config(EXTI_SOURCE_GPIOE, EXTI_SOURCE_PIN4);
  exti_init(EXTI_0, EXTI_INTERRUPT, EXTI_TRIG_RISING);
  exti_init(EXTI_4, EXTI_INTERRUPT, EXTI_TRIG_RISING);
  nvic_irq_enable(EXTI0_IRQn, 0, 2);
  nvic_irq_enable(EXTI4_IRQn, 1, 2);
  exti_interrupt_flag_clear(EXTI_0);
  exti_interrupt_flag_clear(EXTI_4);
}

void EXTI0_IRQHandler() {
  exti_interrupt_flag_clear(EXTI_0);
  /* TODO(rx-ted): it will do something
   * when host is ran. */
  if (KEY_WEAK_UP == 1) {
    LED0_TOGGLE();
  }
}
void EXTI4_IRQHandler() {
  exti_interrupt_flag_clear(EXTI_4);
  /* TODO(rx-ted): it will do something when host is ran */
  if (KEY0 == 1) {
    LED1_TOGGLE();
  }
}
