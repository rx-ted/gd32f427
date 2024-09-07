#include "hw.h"

#include "delay.h"
#include "sys.h"
#include "usart.h"

void setup()
{
    delay_init(200);    /* 延时初始化 */
    usart_init(115200); /* 串口初始化为115200 */

    rcu_periph_clock_enable(RCU_GPIOF); /* 使能GPIOF时钟 */

    /* 设置PF9推挽输出模式 */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_9);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_9);
    /* 设置PF10推挽输出模式 */
    gpio_mode_set(GPIOF, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, GPIO_PIN_10);
    gpio_output_options_set(GPIOF, GPIO_OTYPE_PP, GPIO_OSPEED_MAX, GPIO_PIN_10);
}
void loop()
{
    gpio_bit_reset(GPIOF, GPIO_PIN_9); /* PF9置0，LED0亮 */
    gpio_bit_set(GPIOF, GPIO_PIN_10);  /* PF10置1，LED1灭 */
    delay_ms(200);
    gpio_bit_set(GPIOF, GPIO_PIN_9);    /* PF9置1，LED0灭 */
    gpio_bit_reset(GPIOF, GPIO_PIN_10); /* PF10置0，LED1亮 */
    delay_ms(200);
}

void hw()
{
    setup();
    while (1)
        loop();
}