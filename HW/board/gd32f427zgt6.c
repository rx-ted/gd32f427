#include "gd32f427zgt6.h"

static Serial_t SERIALX[Serialx] = {
    {USART0, RCU_USART0, GPIO_PIN_10, GPIOA, RCU_GPIOA, GPIO_PIN_9, GPIOA, RCU_GPIOA, GPIO_AF_1},
};

static Led_t LEDX[LEDn] = {
    {GPIO_PIN_9, GPIOF, RCU_GPIOF},
    {GPIO_PIN_10, GPIOF, RCU_GPIOF},
};

static uint32_t KEY_GPIO_PORT[KEYn] = {KEY0_GPIO_PORT, WAKEUP_KEY_GPIO_PORT};
static uint32_t KEY_GPIO_PIN[KEYn] = {KEY0_PIN, WAKEUP_KEY_PIN};
static uint32_t KEY_GPIO_CLK[KEYn] = {KEY0_GPIO_CLK, WAKEUP_KEY_GPIO_CLK};

void gd_eval_led_init(uint8_t lednum)
{
    if (lednum > LEDn - 1)
    {
        printf("LED: there are not more led number for %d\n", lednum);
        return;
    }
    Led_t led = LEDX[lednum];
    rcu_periph_clock_enable(led.rcu_led_clk);
    gpio_mode_set(led.led_port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, led.led_pin);
    gpio_output_options_set(led.led_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, led.led_pin);
    GPIO_BC(led.led_port) = led.led_pin;
}
void gd_eval_led_on(uint8_t lednum)
{
    if (lednum < LEDn)
    {
        GPIO_BOP(LEDX[lednum].led_port) = LEDX[lednum].led_pin;
    }
}
void gd_eval_led_off(uint8_t lednum)
{
    if (lednum < LEDn)
    {
        GPIO_BC(LEDX[lednum].led_port) = LEDX[lednum].led_pin;
    }
}
void gd_eval_led_toggle(uint8_t lednum)
{
    if (lednum < LEDn)
    {
        GPIO_TG(LEDX[lednum].led_port) = LEDX[lednum].led_pin;
    }
}

void usart_send(uint8_t data)
{
    while (usart_flag_get(SERIALX[COM].com, USART_FLAG_TBE) == RESET)
        ;
    usart_data_transmit(SERIALX[COM].com, data);
}

void gd_eval_com_init(uint8_t com)
{
    if (com > Serialx - 1)
    {
        printf("Serial: there are not more serial number for %d\n", com);
        return;
    }
    Serial_t serial = SERIALX[com];

    rcu_periph_clock_enable(serial.rcu_periph_clk);
    rcu_periph_clock_enable(serial.rcu_tx_clk);
    rcu_periph_clock_enable(serial.rcu_rx_clk);

    gpio_af_set(serial.tx_port, serial.af, serial.tx_pin);
    gpio_af_set(serial.rx_port, serial.af, serial.rx_pin);

    gpio_mode_set(serial.tx_port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, serial.tx_pin);
    gpio_mode_set(serial.rx_port, GPIO_MODE_AF, GPIO_PUPD_PULLUP, serial.rx_pin);

    gpio_output_options_set(serial.tx_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, serial.tx_pin);
    gpio_output_options_set(serial.rx_port, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, serial.rx_pin);

    nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
    nvic_irq_enable(USART0_IRQn, 0, 1);

    usart_deinit(serial.com);

    usart_baudrate_set(serial.com, 115200U);
    usart_stop_bit_set(serial.com, USART_STB_1BIT);
    usart_word_length_set(serial.com, USART_WL_8BIT);
    usart_parity_config(serial.com, USART_PM_NONE);
    usart_receive_config(serial.com, USART_RECEIVE_ENABLE);
    usart_transmit_config(serial.com, USART_TRANSMIT_ENABLE);
    usart_hardware_flow_cts_config(serial.com, USART_CTS_DISABLE);
    usart_hardware_flow_rts_config(serial.com, USART_RTS_DISABLE);

    usart_enable(serial.com);

    USART_STAT0(serial.com) &= ~USART_STAT0_TC;   // CLEAR TC send finish flag.
    USART_STAT0(serial.com) &= ~USART_STAT0_RBNE; // CLEAR RBNE receive finish flag.
    USART_CTL0(serial.com) |= USART_CTL0_RBNEIE;  // Make PE. RX receive interrupt
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while (RESET == usart_flag_get(USART0, USART_FLAG_TBE))
        ;
    return ch;
}

void gd_eval_key_init(uint8_t key_num, keymode_typedef_enum key_mode);
uint8_t gd_eval_key_state_get(uint8_t button);