#include "led.h"

typedef struct
{
    uint8_t id;
    device_state_t is_init;
    uint16_t led_pin;
    GPIO_TypeDef *led_port;
} led_t;

led_t leds[LEDn] = {
    {0, DEVICE_OFF, LED0_Pin, LED0_GPIO_Port},
    {1, DEVICE_OFF, LED1_Pin, LED1_GPIO_Port},
};

void set_led_is_init(uint8_t led_num, device_state_t state)
{
    leds[led_num].is_init = state;
}

device_result_t led_init(uint8_t led_num)
{
    if (led_num > LEDn - 1)
    {
        printf("[E][led]: Pls check LED number!\n");
        return DEVICE_EINVAL;
    }
    led_t l = leds[led_num];

    if (l.is_init == DEVICE_OFF)
    {
        set_led_is_init(led_num, DEVICE_ON);
        GPIO_InitTypeDef GPIO_InitStruct = {0};
        HAL_GPIO_WritePin(l.led_port, l.led_pin, GPIO_PIN_RESET);
        GPIO_InitStruct.Pin = l.led_pin;
        GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
        HAL_GPIO_Init(l.led_port, &GPIO_InitStruct);
        printf("[I][led]: LED %d init okay!\n", l.id);
    }
    return DEVICE_EOK;
}

void led_on(uint8_t led_num)
{
    if (led_init(led_num) == DEVICE_EOK)
        HAL_GPIO_WritePin(leds[led_num].led_port, leds[led_num].led_pin, GPIO_PIN_SET);
}

void led_off(uint8_t led_num)
{
    if (led_init(led_num) == DEVICE_EOK)
        HAL_GPIO_WritePin(leds[led_num].led_port, leds[led_num].led_pin, GPIO_PIN_RESET);
}

void led_toggle(uint8_t led_num)
{
    if (led_init(led_num) == DEVICE_EOK)
        HAL_GPIO_TogglePin(leds[led_num].led_port, leds[led_num].led_pin);
}
