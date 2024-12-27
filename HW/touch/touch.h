#ifndef __TOUCH_H
#define __TOUCH_H

#include "config.h"
#include "lcd.h"

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

// 最大触摸点数量
#define TOUCH_TP_MAX 5

/* 触摸点坐标数据结构 */
typedef struct
{
    uint16_t x;    // 触摸点X坐标
    uint16_t y;    // 触摸点Y坐标
    uint16_t size; // 触摸点大小
} touch_point_t;

typedef struct
{
    uint8_t id;
    uint8_t device_address;
    lcd_display_mode_t mode; // touch screen mode
    touch_point_t point[TOUCH_TP_MAX];
    uint8_t (*scan)();
} touch_dev_t;

void touch_init();

#endif

#endif
