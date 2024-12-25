#include "gd32f427zgt6.h"

UART_HandleTypeDef huart1; // TODO: Only one serial, not have more.

static Led_t LEDX[LEDn] = {
    {GPIO_PIN_9, GPIOF},
    {GPIO_PIN_10, GPIOF},
};

static Serial_t SERIALX[Serialx] = {
    {USART1},
};
#ifdef DEBUG

#ifdef __GNUC__
/* retarget the C library printf function to the USART */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#else

int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#endif

#endif // DEBUG

void gd_eval_com_init(uint8_t com)
{
    if (com > Serialx - 1)
    {
        printf("Serial: Pls check Serial number!\n");
        return;
    }
    __HAL_RCC_GPIOA_CLK_ENABLE(); // TODO: PA9 PA10, other serial is not support.
    Serial_t serial = SERIALX[com];
    huart1.Instance = serial.serial;
    huart1.Init.BaudRate = 115200;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

void gd_eval_led_init(uint8_t lednum)
{
    if (lednum > LEDn - 1)
    {
        printf("LED: Pls check LED number!\n");
        return;
    }
    Led_t l = LEDX[lednum];
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    set_gpio_clock(l.led_port);
    HAL_GPIO_WritePin(l.led_port, l.led_pin, GPIO_PIN_RESET);
    GPIO_InitStruct.Pin = l.led_pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(l.led_port, &GPIO_InitStruct);
}
/* turn on selected led */
void gd_eval_led_on(uint8_t lednum)
{
    if (lednum < LEDn)
    {
        HAL_GPIO_WritePin(LEDX[lednum].led_port, LEDX[lednum].led_pin, GPIO_PIN_RESET);
    }
}
/* turn off selected led */
void gd_eval_led_off(uint8_t lednum)
{
    if (lednum < LEDn)
    {
        HAL_GPIO_WritePin(LEDX[lednum].led_port, LEDX[lednum].led_pin, GPIO_PIN_SET);
    }
}
/* toggle the selected led */
void gd_eval_led_toggle(uint8_t lednum)
{
    if (lednum < LEDn)
    {
        HAL_GPIO_TogglePin(LEDX[lednum].led_port, LEDX[lednum].led_pin);
    }
}
/* configure key */
void gd_eval_key_init(uint8_t key_num, keymode_typedef_enum key_mode);
/* return the selected button state */
uint8_t gd_eval_key_state_get(uint8_t button);

void set_gpio_clock(GPIO_TypeDef *gpiox)
{
    if (gpiox == GPIOA)
        __HAL_RCC_GPIOA_CLK_ENABLE();
    else if (gpiox == GPIOB)
        __HAL_RCC_GPIOB_CLK_ENABLE();
    else if (gpiox == GPIOC)
        __HAL_RCC_GPIOC_CLK_ENABLE();
    else if (gpiox == GPIOD)
        __HAL_RCC_GPIOD_CLK_ENABLE();
    else if (gpiox == GPIOE)
        __HAL_RCC_GPIOE_CLK_ENABLE();
    else if (gpiox == GPIOF)
        __HAL_RCC_GPIOF_CLK_ENABLE();
    else if (gpiox == GPIOG)
        __HAL_RCC_GPIOG_CLK_ENABLE();
    else if (gpiox == GPIOH)
        __HAL_RCC_GPIOH_CLK_ENABLE();
    else if (gpiox == GPIOI)
        __HAL_RCC_GPIOI_CLK_ENABLE();
    else if (gpiox == GPIOJ)
        __HAL_RCC_GPIOJ_CLK_ENABLE();
    else if (gpiox == GPIOK)
        __HAL_RCC_GPIOK_CLK_ENABLE();
    else
    {
        printf("GPIO: Please check all gpios!\n");
    }
}