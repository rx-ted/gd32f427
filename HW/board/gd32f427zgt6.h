#ifndef GD32F427ZGT6
#define GD32F427ZGT6

#ifdef _cplusplus
extern "C"
{
#endif
#include "gd32f4xx.h"
#include <stdio.h>
#include "main.h"
#include "sys.h"
#include "delay.h"

    typedef enum
    {
        KEY_MODE_GPIO = 0,
        KEY_MODE_EXTI = 1
    } keymode_typedef_enum;

    // two leds

#define LEDn 2U
    typedef struct
    {
        uint32_t led_pin;            // GPIO_PIN_x
        uint32_t led_port;           // GPIOx
        rcu_periph_enum rcu_led_clk; // RCU_GPIOx
    } Led_t;

// KEY
#define KEYn 2
#define KEY0_PIN GPIO_PIN_4
#define KEY0_GPIO_PORT GPIOE
#define KEY0_GPIO_CLK RCU_GPIOE

#define WAKEUP_KEY_PIN GPIO_PIN_0
#define WAKEUP_KEY_GPIO_PORT GPIOA
#define WAKEUP_KEY_GPIO_CLK RCU_GPIOA

// usb serial
// PA10:txd  PA9:rxd
#define COM 0 // define use serial0
#define Serialx 1U
    typedef struct
    {
        uint32_t com;                   // USARTx
        rcu_periph_enum rcu_periph_clk; // RCU_USARTx
        uint32_t tx_pin;                // GPIO_PIN_x
        uint32_t tx_port;               // GPIOx
        rcu_periph_enum rcu_tx_clk;     // RCU_GPIOx
        uint32_t rx_pin;                // GPIO_PIN_x
        uint32_t rx_port;               // GPIOx
        rcu_periph_enum rcu_rx_clk;     // RCU_GPIOx
        uint32_t af;                    // GPIO_AF_x
    } Serial_t;

    // spi flash

    // spi lcd

    // i2c eeprom

    // USB SLAVE/HOST

    // SD Card 4 line

    // sram

    // 8080 lcd

    // carame dvp

    /* FUNCTION DECLARATIONS*/
    /* configures led GPIO */
    void
    gd_eval_led_init(uint8_t lednum);
    /* turn on selected led */
    void gd_eval_led_on(uint8_t lednum);
    /* turn off selected led */
    void gd_eval_led_off(uint8_t lednum);
    /* toggle the selected led */
    void gd_eval_led_toggle(uint8_t lednum);
    /* configure key */
    void gd_eval_key_init(uint8_t key_num, keymode_typedef_enum key_mode);
    /* return the selected button state */
    uint8_t gd_eval_key_state_get(uint8_t button);
    void gd_eval_com_init(uint8_t com);

#ifdef _cplusplus
}
#endif // _cplusplus

#endif // !GD32F427ZGT6
