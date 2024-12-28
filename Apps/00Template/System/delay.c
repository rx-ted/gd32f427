#include "delay.h"

uint32_t g_fac_us = 0;

void delay_init()
{
  SysTick->CTRL = 0;
  SysTick->CTRL &= ~(1 << 2);
  g_fac_us = SystemCoreClock / 1000000U / 8U;
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
  SysTick->LOAD = 0xffffff;
}

void delay_ms(uint32_t ms)
{
  // HAL_Delay(ms);
  delay_us(1000 * ms);
}

void delay_us(uint32_t us)
{

  uint32_t ticks;
  uint32_t told, tnow, tcnt = 0;
  uint32_t reload;
  reload = SysTick->LOAD; /* LOAD的值 */
  ticks = us * g_fac_us;  /* 需要的节拍数 */

  told = SysTick->VAL; /* 刚进入时的计数器值 */
  while (1)
  {
    tnow = SysTick->VAL;

    if (tnow != told)
    {
      if (tnow < told)
      {
        tcnt += told - tnow; /* 这里注意一下SYSTICK是一个递减的计数器就可以了. */
      }
      else
      {
        tcnt += reload - tnow + told;
      }
      told = tnow;
      if (tcnt >= ticks)
      {
        break; /* 时间超过/等于要延迟的时间,则退出. */
      }
    }
  }
}