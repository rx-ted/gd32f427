#include "gd32f427zgt6.h"

static uint32_t LED_GPIO_PORT[LEDn] = {LED0_GPIO_PORT, LED1_GPIO_PORT};
static uint32_t LED_GPIO_PIN[LEDn] = {LED0_PIN, LED1_PIN};
static uint32_t LED_GPIO_CLK[LEDn] = {LED0_GPIO_CLK, LED1_GPIO_CLK};

static uint32_t KEY_GPIO_PORT[KEYn] = {KEY0_GPIO_PORT, WAKEUP_KEY_GPIO_PORT};
static uint32_t KEY_GPIO_PIN[KEYn] = {KEY0_PIN, WAKEUP_KEY_PIN};
static uint32_t KEY_GPIO_CLK[KEYn] = {KEY0_GPIO_CLK, WAKEUP_KEY_GPIO_CLK};

void gd_eval_led_init(led_typedef_enum lednum)
{
    rcu_periph_clock_enable(LED_GPIO_CLK[lednum]);
    gpio_mode_set(LED_GPIO_PORT[lednum], GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_GPIO_PIN[lednum]);
    gpio_output_options_set(LED_GPIO_PORT[lednum], GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, LED_GPIO_PIN[lednum]);
    GPIO_BC(LED_GPIO_PORT[lednum]) = LED_GPIO_PIN[lednum];
}
void gd_eval_led_on(led_typedef_enum lednum)
{
    GPIO_BOP(LED_GPIO_PORT[lednum]) = LED_GPIO_PIN[lednum];
}
void gd_eval_led_off(led_typedef_enum lednum)
{
    GPIO_BC(LED_GPIO_PORT[lednum]) = LED_GPIO_PIN[lednum];
}
void gd_eval_led_toggle(led_typedef_enum lednum)
{
    GPIO_TG(LED_GPIO_PORT[lednum]) = LED_GPIO_PIN[lednum];
}
void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
uint8_t gd_eval_key_state_get(key_typedef_enum button);