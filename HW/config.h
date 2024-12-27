#ifndef __CONFIG_H__
#define __CONFIG_H__

#define GD32F427ZGT6_VERSION "1.0"

#include "main.h"
#include <stm32f4xx.h>
#include <stm32f427xx.h>
#include "stm32f4xx_hal.h"
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "stdbool.h"

#ifndef DEBUG
#define DEBUG
#endif
#ifndef USE_HAL_DRIVER
#define USE_HAL_DRIVER
#endif
#ifndef STM32F427xx
#define STM32F427xx
#endif

#define LEDn 2U
#define LED0_PIN GPIO_PIN_9
#define LED0_GPIO_Port GPIOF
#define LED1_PIN GPIO_PIN_10
#define LED1_GPIO_Port GPIOF

#define SERIALx 1U
#define SERIAL1 USART1

#define LCD_DATA_BITS 16

#define BSP_USING_TOUCH 1

// !USART1 interface
#define USART1_TX_Pin GPIO_PIN_9
#define USART1_TX_GPIO_Port GPIOA
#define USART1_RX_Pin GPIO_PIN_10
#define USART1_RX_GPIO_Port GPIOA

// !ATC24XX interface
#define AT24CXX_SCK_Pin GPIO_PIN_8
#define AT24CXX_SCK_GPIO_Port GPIOB
#define AT24CXX_SDA_Pin GPIO_PIN_9
#define AT24CXX_SDA_GPIO_Port GPIOB

// !LCD interface
#define LCD_BL_Pin GPIO_PIN_15
#define LCD_BL_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_12
#define LCD_RS_GPIO_Port GPIOF
#define LCD_RST_Pin GPIO_PIN_6
#define LCD_RST_GPIO_Port GPIOG
#define LCD_RD_Pin GPIO_PIN_4
#define LCD_RD_GPIO_Port GPIOD
#define LCD_WR_Pin GPIO_PIN_5
#define LCD_WR_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOG

#define LCD_D0_Pin GPIO_PIN_14
#define LCD_D0_GPIO_Port GPIOD
#define LCD_D1_Pin GPIO_PIN_15
#define LCD_D1_GPIO_Port GPIOD
#define LCD_D2_Pin GPIO_PIN_0
#define LCD_D2_GPIO_Port GPIOD
#define LCD_D3_Pin GPIO_PIN_1
#define LCD_D3_GPIO_Port GPIOD
#define LCD_D4_Pin GPIO_PIN_7
#define LCD_D4_GPIO_Port GPIOE
#define LCD_D5_Pin GPIO_PIN_8
#define LCD_D5_GPIO_Port GPIOE
#define LCD_D6_Pin GPIO_PIN_9
#define LCD_D6_GPIO_Port GPIOE
#define LCD_D7_Pin GPIO_PIN_10
#define LCD_D7_GPIO_Port GPIOE
#define LCD_D8_Pin GPIO_PIN_11
#define LCD_D8_GPIO_Port GPIOE
#define LCD_D9_Pin GPIO_PIN_12
#define LCD_D9_GPIO_Port GPIOE
#define LCD_D10_Pin GPIO_PIN_13
#define LCD_D10_GPIO_Port GPIOE
#define LCD_D11_Pin GPIO_PIN_14
#define LCD_D11_GPIO_Port GPIOE
#define LCD_D12_Pin GPIO_PIN_15
#define LCD_D12_GPIO_Port GPIOE
#define LCD_D13_Pin GPIO_PIN_8
#define LCD_D13_GPIO_Port GPIOD
#define LCD_D14_Pin GPIO_PIN_9
#define LCD_D14_GPIO_Port GPIOD
#define LCD_D15_Pin GPIO_PIN_10
#define LCD_D15_GPIO_Port GPIOD

// !Touch interface
#define T_CS_Pin GPIO_PIN_13
#define T_CS_GPIO_Port GPIOC
#define T_SCK_Pin GPIO_PIN_0
#define T_SCK_GPIO_Port GPIOB
#define T_PEN_Pin GPIO_PIN_1
#define T_PEN_GPIO_Port GPIOB
#define T_MISO_Pin GPIO_PIN_2
#define T_MISO_GPIO_Port GPIOB
#define T_MOSI_Pin GPIO_PIN_11
#define T_MOSI_GPIO_Port GPIOF

typedef enum
{
    DEVICE_EOK = 0,   // 没有错误
    DEVICE_ERROR,     // 错误
    DEVICE_EINVAL,    // 非法参数
    DEVICE_NOT_FOUND, // 未找到
    DEVICE_EBUSY,     // 忙
    DEVICE_TIMEOUT,   // 超时
    DEVICE_CONTINUE,  // 继续
} device_result_t;

typedef enum
{
    DEVICE_ON = 1,
    DEVICE_OFF = 0,
} device_state_t;

#endif // !__CONFIG_H__
