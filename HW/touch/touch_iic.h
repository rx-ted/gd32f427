#ifndef __TOUCH_IIC_H
#define __TOUCH_IIC_H

#include "config.h"

#if (BSP_USING_TOUCH != 0)

/* 引脚定义 */
#define TOUCH_IIC_SCL_GPIO_PORT T_SCK_GPIO_Port
#define TOUCH_IIC_SCL_GPIO_PIN T_SCK_Pin
#define TOUCH_IIC_SCL_GPIO_CLK_ENABLE() \
    do                                             \
    {                                              \
        __HAL_RCC_GPIOB_CLK_ENABLE();              \
    } while (0)
#define TOUCH_IIC_SDA_GPIO_PORT T_MOSI_GPIO_Port
#define TOUCH_IIC_SDA_GPIO_PIN T_MOSI_Pin
#define TOUCH_IIC_SDA_GPIO_CLK_ENABLE() \
    do                                             \
    {                                              \
        __HAL_RCC_GPIOB_CLK_ENABLE();              \
    } while (0)

/* IO操作 */
#define TOUCH_IIC_SCL(x)                                                                                                                                                                                            \
    do                                                                                                                                                                                                                         \
    {                                                                                                                                                                                                                          \
        x ? HAL_GPIO_WritePin(TOUCH_IIC_SCL_GPIO_PORT, TOUCH_IIC_SCL_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(TOUCH_IIC_SCL_GPIO_PORT, TOUCH_IIC_SCL_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)

#define TOUCH_IIC_SDA(x)                                                                                                                                                                                            \
    do                                                                                                                                                                                                                         \
    {                                                                                                                                                                                                                          \
        x ? HAL_GPIO_WritePin(TOUCH_IIC_SDA_GPIO_PORT, TOUCH_IIC_SDA_GPIO_PIN, GPIO_PIN_SET) : HAL_GPIO_WritePin(TOUCH_IIC_SDA_GPIO_PORT, TOUCH_IIC_SDA_GPIO_PIN, GPIO_PIN_RESET); \
    } while (0)

#define TOUCH_IIC_READ_SDA() HAL_GPIO_ReadPin(TOUCH_IIC_SDA_GPIO_PORT, TOUCH_IIC_SDA_GPIO_PIN)

/* 控制IIC通讯是读操作还是写操作 */
#define TOUCH_IIC_WRITE 0
#define TOUCH_IIC_READ 1

/* 错误代码 */
#define TOUCH_IIC_EOK 0   /* 没有错误 */
#define TOUCH_IIC_ERROR 1 /* 错误 */

/* 操作函数 */
void touch_iic_init(void);                /* 初始化IIC接口 */
void touch_iic_start(void);               /* 产生IIC起始信号 */
void touch_iic_stop(void);                /* 产生IIC停止信号 */
uint8_t touch_iic_wait_ack(void);         /* 等待IIC应答信号 */
void touch_iic_ack(void);                 /* 产生ACK应答信号 */
void touch_iic_nack(void);                /* 不产生ACK应答信号 */
void touch_iic_send_byte(uint8_t dat);    /* IIC发送一个字节 */
uint8_t touch_iic_recv_byte(uint8_t ack); /* IIC接收一个字节 */

#endif

#endif
