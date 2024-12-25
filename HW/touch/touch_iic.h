#ifndef __TOUCH_IIC_H
#define __TOUCH_IIC_H

#include "config.h"

#if (BSP_USING_TOUCH != 0)

/* ���Ŷ��� */
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

/* IO���� */
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

/* ����IICͨѶ�Ƕ���������д���� */
#define TOUCH_IIC_WRITE 0
#define TOUCH_IIC_READ 1

/* ������� */
#define TOUCH_IIC_EOK 0   /* û�д��� */
#define TOUCH_IIC_ERROR 1 /* ���� */

/* �������� */
void touch_iic_init(void);                /* ��ʼ��IIC�ӿ� */
void touch_iic_start(void);               /* ����IIC��ʼ�ź� */
void touch_iic_stop(void);                /* ����IICֹͣ�ź� */
uint8_t touch_iic_wait_ack(void);         /* �ȴ�IICӦ���ź� */
void touch_iic_ack(void);                 /* ����ACKӦ���ź� */
void touch_iic_nack(void);                /* ������ACKӦ���ź� */
void touch_iic_send_byte(uint8_t dat);    /* IIC����һ���ֽ� */
uint8_t touch_iic_recv_byte(uint8_t ack); /* IIC����һ���ֽ� */

#endif

#endif
