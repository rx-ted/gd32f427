#ifndef __TOUCH_H
#define __TOUCH_H

#include "config.h"

#if (BSP_USING_TOUCH != 0)

/* 引脚定义 */
#define TOUCH_PEN_GPIO_PORT T_PEN_GPIO_Port
#define TOUCH_PEN_GPIO_PIN T_PEN_Pin
#define TOUCH_PEN_GPIO_CLK_ENABLE()   \
    do                                \
    {                                 \
        __HAL_RCC_GPIOB_CLK_ENABLE(); \
    } while (0)
#define TOUCH_TCS_GPIO_PORT T_CS_GPIO_Port
#define TOUCH_TCS_GPIO_PIN T_CS_Pin
#define TOUCH_TCS_GPIO_CLK_ENABLE()   \
    do                                \
    {                                 \
        __HAL_RCC_GPIOC_CLK_ENABLE(); \
    } while (0)

/* IO操作 */
#define TOUCH_READ_PEN() HAL_GPIO_ReadPin(TOUCH_PEN_GPIO_PORT, TOUCH_PEN_GPIO_PIN)
#define TOUCH_PEN(x)                                                                                                                                               \
    do                                                                                                                                                             \
    {                                                                                                                                                              \
        x ? HAL_GPIO_WritePin(TOUCH_PEN_GPIO_PORT, TOUCH_PEN_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(TOUCH_PEN_GPIO_PORT, TOUCH_PEN_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)
#define TOUCH_TCS(x)                                                                                                                                               \
    do                                                                                                                                                             \
    {                                                                                                                                                              \
        x ? HAL_GPIO_WritePin(TOUCH_TCS_GPIO_PORT, TOUCH_TCS_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(TOUCH_TCS_GPIO_PORT, TOUCH_TCS_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)

/* RGB LCD触摸IIC通讯地址枚举 */
typedef enum
{
    TOUCH_IIC_ADDR_14 = 0x14, /* 0x14 */
    TOUCH_IIC_ADDR_5D = 0x5D, /* 0x5D */
    TOUCH_IIC_ADDR_38 = 0x38, /* 0x38 */
} touch_iic_addr_t;

/* 触摸点坐标数据结构 */
typedef struct
{
    uint16_t x;    /* 触摸点X坐标 */
    uint16_t y;    /* 触摸点Y坐标 */
    uint16_t size; /* 触摸点大小 */
} touch_point_t;

/* 错误代码 */
#define TOUCH_EOK 0   /* 没有错误 */
#define TOUCH_ERROR 1 /* 错误 */

/* RGB LCD触摸IC类型枚举 */
typedef enum
{
    TOUCH_TYPE_GTXX = 0x00, /* GT9111、GT9147、GT9271 */
    TOUCH_TYPE_FTXX,        /* FT5206 */
} touch_type_t;

/* 操作函数 */
uint8_t touch_init(touch_type_t type);                 /* RGB LCD模块触摸初始化 */
uint8_t touch_scan(touch_point_t *point, uint8_t cnt); /* RGB LCD模块触摸扫描 */

#endif

#endif
