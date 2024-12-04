#ifndef GD32F427ZGT6
#define GD32F427ZGT6

#ifdef _cplusplus
extern "C"
{
#endif

#include "gd32f4xx.h"

    typedef enum
    {
        LED0 = 0,
        LED1
    } led_typedef_enum;

    typedef enum
    {
        KEY0 = 0,
        KEY_WAKEUP = 1,
    } key_typedef_enum;

    typedef enum
    {
        KEY_MODE_GPIO = 0,
        KEY_MODE_EXTI = 1
    } keymode_typedef_enum;

// two leds
#define LEDn 2
#define LED0_PIN GPIO_PIN_9
#define LED0_GPIO_PORT GPIOF
#define LED0_GPIO_CLK RCU_GPIOF

#define LED1_PIN GPIO_PIN_10
#define LED1_GPIO_PORT GPIOF
#define LED1_GPIO_CLK RCU_GPIOF

// KEY
#define KEYn 2
#define KEY0_PIN GPIO_PIN_4
#define KEY0_GPIO_PORT GPIOE
#define KEY0_GPIO_CLK RCU_GPIOE

#define WAKEUP_KEY_PIN GPIO_PIN_0
#define WAKEUP_KEY_GPIO_PORT GPIOA
#define WAKEUP_KEY_GPIO_CLK RCU_GPIOA

    // usb serial
    // ?PA10:txd  PA0:rxd

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
    gd_eval_led_init(led_typedef_enum lednum);
    /* turn on selected led */
    void gd_eval_led_on(led_typedef_enum lednum);
    /* turn off selected led */
    void gd_eval_led_off(led_typedef_enum lednum);
    /* toggle the selected led */
    void gd_eval_led_toggle(led_typedef_enum lednum);
    /* configure key */
    void gd_eval_key_init(key_typedef_enum key_num, keymode_typedef_enum key_mode);
    /* return the selected button state */
    uint8_t gd_eval_key_state_get(key_typedef_enum button);

#ifdef _cplusplus
}
#endif // _cplusplus

#endif // !GD32F427ZGT6
