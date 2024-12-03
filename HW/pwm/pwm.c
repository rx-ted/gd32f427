#include "pwm.h"

/**
 * @brief       TIMER13的TIMER_CH_0 PWM输出初始化
 * @note
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=定时器工作频率,单位:Mhz
 *              1us =
 *              ((arr+1)*(psc+1))/200
 *              200 =((arr+1)*(psc+1))
 *
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void timer13_pwm_init(uint16_t arr, uint16_t psc, uint32_t portx,
                      uint32_t pin) {

  /* 定义初始化结构体变量 */
  timer_oc_parameter_struct timer_ocinitpara;
  timer_parameter_struct timer_initpara;

  rcu_periph_clock_enable(RCU_GPIOF);   /* 使能GPIOF时钟 */
  rcu_periph_clock_enable(RCU_TIMER13); /* 使能TIMER13时钟 */

  /* 设置复用功能选择 */
  gpio_af_set(portx, GPIO_AF_9, pin);

  /* PF9的模式设置 */
  gpio_mode_set(portx, GPIO_MODE_AF, GPIO_PUPD_PULLUP, pin);
  gpio_output_options_set(portx, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, pin);

  timer_deinit(TIMER13); /* 复位TIMER13 */

  timer_initpara.period = arr;    /* 设置自动重装载值 */
  timer_initpara.prescaler = psc; /* 设置预分频值 */
  timer_initpara.alignedmode = TIMER_COUNTER_EDGE; /* 设置对齐模式 */
  timer_initpara.counterdirection = TIMER_COUNTER_UP; /* 设置向上计数模式 */
  timer_initpara.clockdivision = TIMER_CKDIV_DIV1; /* 设置时钟分频因子 */
  timer_initpara.repetitioncounter = 0;            /* 设置重复计数值 */
  timer_init(TIMER13, &timer_initpara); /* 根据参数初始化定时器 */

  timer_ocinitpara.outputstate = TIMER_CCX_ENABLE; /* 使能通道输出 */
  timer_ocinitpara.ocpolarity =
      TIMER_OC_POLARITY_LOW; /* 设置通道输出极性为低 */

  timer_channel_output_config(TIMER13, TIMER_CH_0,
                              &timer_ocinitpara); /* 定时器通道输出配置 */

  timer_channel_output_pulse_value_config(
      TIMER13, TIMER_CH_0,
      arr /
          2); /* 设置占空比,这里默认设置比较值为自动重装载值的一半,即占空比为50%
               */
  timer_channel_output_mode_config(
      TIMER13, TIMER_CH_0, TIMER_OC_MODE_PWM0); /* 设置通道比较模式为PWM模式0 */

  timer_enable(TIMER13); /* 使能定时器TIMER13 */
}

void timer13_set_compare0(uint32_t compare) {
  timer_channel_output_pulse_value_config(TIMER13, TIMER_CH_0, compare);
}
