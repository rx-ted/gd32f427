#include "hw.h"

int main()
{
    SysTick_Config(SystemCoreClock / 1000U);

    /* configure the systick handler priority */
    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(SysTick_IRQn, 0, 0);

    delay_init(200);
    gd_eval_com_init(0);

    gd_eval_led_init(0);
    gd_eval_led_init(1);

    hw();
}
