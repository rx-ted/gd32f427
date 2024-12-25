#include "touch_iic.h"
#include "delay.h"

#if (BSP_USING_TOUCH != 0)

/**
 * @brief       IIC接口延时函数，用于控制IIC读写速度
 * @param       无
 * @retval      无
 */
static inline void touch_iic_delay(void)
{
    delay_us(2);
}

/**
 * @brief       初始化IIC接口
 * @param       无
 * @retval      无
 */
void touch_iic_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    /* 使能SCL、SDA引脚GPIO的时钟 */
    TOUCH_IIC_SCL_GPIO_CLK_ENABLE();
    TOUCH_IIC_SDA_GPIO_CLK_ENABLE();

    /* 初始化SCL引脚 */
    gpio_init_struct.Pin = TOUCH_IIC_SCL_GPIO_PIN; /* SCL引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;              /* 推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                      /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;            /* 高速 */
    HAL_GPIO_Init(TOUCH_IIC_SCL_GPIO_PORT, &gpio_init_struct);

    /* 初始化SDA引脚 */
    gpio_init_struct.Pin = TOUCH_IIC_SDA_GPIO_PIN; /* SDA引脚 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_OD;              /* 开漏输出 */
    HAL_GPIO_Init(TOUCH_IIC_SDA_GPIO_PORT, &gpio_init_struct);

    touch_iic_stop();
}

/**
 * @brief       产生IIC起始信号
 * @param       无
 * @retval      无
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
 * @brief       产生IIC停止信号
 * @param       无
 * @retval      无
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
 * @brief       等待IIC应答信号
 * @param       无
 * @retval      0: 应答信号接收成功
 *              1: 应答信号接收失败
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
 * @brief       产生ACK应答信号
 * @param       无
 * @retval      无
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
 * @brief       不产生ACK应答信号
 * @param       无
 * @retval      无
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
 * @brief       IIC发送一个字节
 * @param       dat: 要发送的数据
 * @retval      无
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
 * @brief       IIC接收一个字节
 * @param       ack: ack=1时，发送ack; ack=0时，发送nack
 * @retval      接收到的数据
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
