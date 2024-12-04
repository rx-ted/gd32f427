#include "hw.h"



/* led spark function */
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0U;

    if (timingdelaylocal)
    {

        if (timingdelaylocal < 500U)
        {
            gd_eval_led_on(LED1);
        }
        else
        {
            gd_eval_led_off(LED1);
        }

        timingdelaylocal--;
    }
    else
    {
        timingdelaylocal = 1000U;
    }
}


int main()
{
    hw();
}
