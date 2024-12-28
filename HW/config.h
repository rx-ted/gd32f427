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

#define USE_HAL_DRIVER
#define STM32F427xx
#define DEBUG

#define LEDn 2U

#define SERIALx 1U
#define SERIAL1 USART1

#define LCD_DATA_BITS 16

#define BSP_USING_TOUCH 1
#define BSP_USING_LVGL 1

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
