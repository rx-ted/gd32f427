#include "touch_iic.h"
#include "delay.h"

#if (BSP_USING_TOUCH != 0)

/**
 * @brief       IIC�ӿ���ʱ���������ڿ���IIC��д�ٶ�
 * @param       ��
 * @retval      ��
 */
static inline void touch_iic_delay(void)
{
    delay_us(2);
}

/**
 * @brief       ��ʼ��IIC�ӿ�
 * @param       ��
 * @retval      ��
 */
void touch_iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    /* ʹ��SCL��SDA����GPIO��ʱ�� */
    TOUCH_IIC_SCL_GPIO_CLK_ENABLE();
    TOUCH_IIC_SDA_GPIO_CLK_ENABLE();

    /* ��ʼ��SCL���� */
    gpio_init_struct.Pin = TOUCH_IIC_SCL_GPIO_PIN; /* SCL���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;              /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                      /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;            /* ���� */
    HAL_GPIO_Init(TOUCH_IIC_SCL_GPIO_PORT, &gpio_init_struct);

    /* ��ʼ��SDA���� */
    gpio_init_struct.Pin = TOUCH_IIC_SDA_GPIO_PIN; /* SDA���� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;              /* ��©��� */
    HAL_GPIO_Init(TOUCH_IIC_SDA_GPIO_PORT, &gpio_init_struct);

    touch_iic_stop();
}

/**
 * @brief       ����IIC��ʼ�ź�
 * @param       ��
 * @retval      ��
 */
void touch_iic_start(void)
{
    TOUCH_IIC_SDA(1);
    TOUCH_IIC_SCL(1);
    touch_iic_delay();
    TOUCH_IIC_SDA(0);
    touch_iic_delay();
    TOUCH_IIC_SCL(0);
    touch_iic_delay();
}

/**
 * @brief       ����IICֹͣ�ź�
 * @param       ��
 * @retval      ��
 */
void touch_iic_stop(void)
{
    TOUCH_IIC_SDA(0);
    touch_iic_delay();
    TOUCH_IIC_SCL(1);
    touch_iic_delay();
    TOUCH_IIC_SDA(1);
    touch_iic_delay();
}

/**
 * @brief       �ȴ�IICӦ���ź�
 * @param       ��
 * @retval      0: Ӧ���źŽ��ճɹ�
 *              1: Ӧ���źŽ���ʧ��
 */
uint8_t touch_iic_wait_ack(void)
{
    uint8_t waittime = 0;
    uint8_t rack = 0;

    TOUCH_IIC_SDA(1);
    touch_iic_delay();
    TOUCH_IIC_SCL(1);
    touch_iic_delay();

    while (TOUCH_IIC_READ_SDA())
    {
        waittime++;

        if (waittime > 250)
        {
            touch_iic_stop();
            rack = 1;
            break;
        }
    }

    TOUCH_IIC_SCL(0);
    touch_iic_delay();

    return rack;
}

/**
 * @brief       ����ACKӦ���ź�
 * @param       ��
 * @retval      ��
 */
void touch_iic_ack(void)
{
    TOUCH_IIC_SDA(0);
    touch_iic_delay();
    TOUCH_IIC_SCL(1);
    touch_iic_delay();
    TOUCH_IIC_SCL(0);
    touch_iic_delay();
    TOUCH_IIC_SDA(1);
    touch_iic_delay();
}

/**
 * @brief       ������ACKӦ���ź�
 * @param       ��
 * @retval      ��
 */
void touch_iic_nack(void)
{
    TOUCH_IIC_SDA(1);
    touch_iic_delay();
    TOUCH_IIC_SCL(1);
    touch_iic_delay();
    TOUCH_IIC_SCL(0);
    touch_iic_delay();
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       dat: Ҫ���͵�����
 * @retval      ��
 */
void touch_iic_send_byte(uint8_t dat)
{
    uint8_t t;

    for (t = 0; t < 8; t++)
    {
        TOUCH_IIC_SDA((dat & 0x80) >> 7);
        touch_iic_delay();
        TOUCH_IIC_SCL(1);
        touch_iic_delay();
        TOUCH_IIC_SCL(0);
        dat <<= 1;
    }
    TOUCH_IIC_SDA(1);
}

/**
 * @brief       IIC����һ���ֽ�
 * @param       ack: ack=1ʱ������ack; ack=0ʱ������nack
 * @retval      ���յ�������
 */
uint8_t touch_iic_recv_byte(uint8_t ack)
{
    uint8_t i;
    uint8_t dat = 0;

    for (i = 0; i < 8; i++)
    {
        dat <<= 1;
        TOUCH_IIC_SCL(1);
        touch_iic_delay();

        if (TOUCH_IIC_READ_SDA())
        {
            dat++;
        }

        TOUCH_IIC_SCL(0);
        touch_iic_delay();
    }

    if (ack == 0)
    {
        touch_iic_nack();
    }
    else
    {
        touch_iic_ack();
    }

    return dat;
}

#endif
