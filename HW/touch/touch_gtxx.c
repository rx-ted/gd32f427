#include "lcd.h"
#include "touch.h"
#include "touch_iic.h"
#include "delay.h"

#if (BSP_USING_TOUCH != 0)

/* RGB LCD模块触摸的PID */
#define TOUCH_PID "911"
#define TOUCH_PID1 "9147"
#define TOUCH_PID2 "9271"
#define TOUCH_PID3 "1158"

/* RGB LCD模块最大触摸点数量 */
#define TOUCH_TP_MAX 5

/* RGB LCD模块触摸部分寄存器定义 */
#define TOUCH_REG_CTRL 0x8040   /* 控制寄存器 */
#define TOUCH_REG_PID 0x8140    /* PID寄存器 */
#define TOUCH_REG_TPINFO 0x814E /* 触摸状态寄存器 */
#define TOUCH_REG_TP1 0x8150    /* 触摸点1数据寄存器 */
#define TOUCH_REG_TP2 0x8158    /* 触摸点2数据寄存器 */
#define TOUCH_REG_TP3 0x8160    /* 触摸点3数据寄存器 */
#define TOUCH_REG_TP4 0x8168    /* 触摸点4数据寄存器 */
#define TOUCH_REG_TP5 0x8170    /* 触摸点5数据寄存器 */

/* 触摸状态寄存器掩码 */
#define TOUCH_TPINFO_MASK_STA 0x80
#define TOUCH_TPINFO_MASK_CNT 0x0F

/* RGB LCD模块触摸点数据寄存器 */
static const uint16_t g_touch_tp_reg[TOUCH_TP_MAX] = {
    TOUCH_REG_TP1,
    TOUCH_REG_TP2,
    TOUCH_REG_TP3,
    TOUCH_REG_TP4,
    TOUCH_REG_TP5,
};

/* RGB LCD模块触摸状态结构体 */
static struct
{
    touch_iic_addr_t iic_addr;
} g_touch_sta;

/**
 * @brief       RGB LCD模块触摸硬件初始化
 * @param       无
 * @retval      无
 */
static void touch_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    /* 使能时钟 */
    TOUCH_PEN_GPIO_CLK_ENABLE();
    TOUCH_TCS_GPIO_CLK_ENABLE();

    /* 初始化PEN引脚 */
    gpio_init_struct.Pin = TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

    /* 初始化TCS引脚 */
    gpio_init_struct.Pin = TOUCH_TCS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_TCS_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief       RGB LCD模块触摸硬件复位
 * @param       addr: 复位后使用的IIC通讯地址
 * @retval      无
 */
static void touch_hw_reset(touch_iic_addr_t addr)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    /* 配置PEN引脚为输出 */
    gpio_init_struct.Pin = TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

    switch (addr)
    {
    case TOUCH_IIC_ADDR_14:
    {
        TOUCH_TCS(0);
        TOUCH_PEN(0);
        TOUCH_PEN(1);
        delay_ms(1);
        TOUCH_TCS(1);
        delay_ms(10);
        break;
    }
    case TOUCH_IIC_ADDR_5D:
    {
        TOUCH_TCS(0);
        TOUCH_PEN(0);
        delay_ms(1);
        TOUCH_TCS(1);
        delay_ms(10);
        break;
    }
    default:
    {
        break;
    }
    }

    /* 重新配置PEN引脚为输入 */
    gpio_init_struct.Pin = TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

    g_touch_sta.iic_addr = addr;
    delay_ms(100);
}

/**
 * @brief       写RGB LCD模块触摸寄存器
 * @param       reg: 待写寄存器地址
 *              buf: 待写入的数据
 *              len: 待写入数据的长度
 * @retval      TOUCH_EOK  : 写ATK-RGBLCD模块触摸寄存器成功
 *              TOUCH_ERROR: 写ATK-RGBLCD模块触摸寄存器失败
 */
static uint8_t touch_write_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    uint8_t ret;

    touch_iic_start();
    touch_iic_send_byte((g_touch_sta.iic_addr << 1) | TOUCH_IIC_WRITE);
    touch_iic_wait_ack();
    touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
    touch_iic_wait_ack();
    touch_iic_send_byte((uint8_t)reg & 0xFF);
    touch_iic_wait_ack();

    for (buf_index = 0; buf_index < len; buf_index++)
    {
        touch_iic_send_byte(buf[buf_index]);
        ret = touch_iic_wait_ack();
        if (ret != 0)
        {
            break;
        }
    }

    touch_iic_stop();

    if (ret != 0)
    {
        return TOUCH_ERROR;
    }

    return TOUCH_EOK;
}

/**
 * @brief       读RGB LCD模块触摸寄存器
 * @param       reg: 待读寄存器地址
 *              buf: 读取的数据
 *              len: 待读取数据的长度
 * @retval      无
 */
static void touch_read_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;

    touch_iic_start();
    touch_iic_send_byte((g_touch_sta.iic_addr << 1) | TOUCH_IIC_WRITE);
    touch_iic_wait_ack();
    touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
    touch_iic_wait_ack();
    touch_iic_send_byte((uint8_t)reg & 0xFF);
    touch_iic_wait_ack();
    touch_iic_start();
    touch_iic_send_byte((g_touch_sta.iic_addr << 1) | TOUCH_IIC_READ);
    touch_iic_wait_ack();

    for (buf_index = 0; buf_index < len - 1; buf_index++)
    {
        buf[buf_index] = touch_iic_recv_byte(1);
    }

    buf[buf_index] = touch_iic_recv_byte(0);

    touch_iic_stop();
}

/**
 * @brief       RGB LCD模块触摸软件复位
 * @param       无
 * @retval      无
 */
static void touch_sw_reset(void)
{
    uint8_t dat;

    dat = 0x02;
    touch_write_reg(TOUCH_REG_CTRL, &dat, 1);
    delay_ms(10);

    dat = 0x00;
    touch_write_reg(TOUCH_REG_CTRL, &dat, 1);
}

/**
 * @brief       获取RGB LCD模块触摸的PID
 * @param       pid: 获取到的PID（ASCII）
 * @retval      无
 */
static void touch_get_pid(char *pid)
{
    touch_read_reg(TOUCH_REG_PID, (uint8_t *)pid, 4);
    pid[4] = '\0';
}

/**
 * @brief       RGB LCD模块触摸初始化
 * @param       无
 * @retval      TOUCH_EOK  : RGB LCD模块触摸初始化成功
 *              TOUCH_ERROR: RGB LCD模块触摸初始化失败
 */
uint8_t touch_init(touch_type_t type)
{
    char pid[5];

    if (type != TOUCH_TYPE_GTXX)
    {
        return TOUCH_ERROR;
    }

    touch_hw_init();
    touch_hw_reset(TOUCH_IIC_ADDR_14);
    touch_iic_init();
    touch_get_pid(pid);

    if ((strcmp(pid, TOUCH_PID) != 0) && (strcmp(pid, TOUCH_PID1) != 0) && (strcmp(pid, TOUCH_PID2) != 0) && (strcmp(pid, TOUCH_PID3) != 0))
    {
        return TOUCH_ERROR;
    }
    touch_sw_reset();

    return TOUCH_EOK;
}

/**
 * @brief       RGB LCD模块触摸扫描
 * @note        连续调用间隔需大于10ms
 * @param       point: 扫描到的触摸点信息
 *              cnt  : 需要扫描的触摸点数量（1~TOUCH_TP_MAX）
 * @retval      0   : 没有扫描到触摸点
 *              其他: 实际获取到的触摸点信息数量
 */
uint8_t touch_scan(touch_point_t *point, uint8_t cnt)
{
    uint8_t tp_info;
    uint8_t tp_cnt;
    uint8_t point_index;
    lcd_display_dir_t dir;
    uint8_t tpn_info[6];
    touch_point_t point_raw;

    if ((cnt == 0) || (cnt > TOUCH_TP_MAX))
    {
        return 0;
    }

    for (point_index = 0; point_index < cnt; point_index++)
    {
        if (&point[point_index] == NULL)
        {
            return 0;
        }
    }

    touch_read_reg(TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));
    if ((tp_info & TOUCH_TPINFO_MASK_STA) == TOUCH_TPINFO_MASK_STA)
    {
        tp_cnt = tp_info & TOUCH_TPINFO_MASK_CNT;
        tp_cnt = (cnt < tp_cnt) ? cnt : tp_cnt;

        for (point_index = 0; point_index < tp_cnt; point_index++)
        {
            touch_read_reg(g_touch_tp_reg[point_index], tpn_info, sizeof(tpn_info));
            point_raw.x = (uint16_t)(tpn_info[1] << 8) | tpn_info[0];
            point_raw.y = (uint16_t)(tpn_info[3] << 8) | tpn_info[2];
            point_raw.size = (uint16_t)(tpn_info[5] << 8) | tpn_info[4];

            // dir = get_disp_dir();
            dir = lcddev.dir;
            switch (dir)
            {
            case LCD_DISP_DIR_0:
            {
                point[point_index].x = point_raw.x;
                point[point_index].y = point_raw.y;
                break;
            }
            case LCD_DISP_DIR_90:
            {
                point[point_index].x = point_raw.y;
                point[point_index].y = lcddev.height - point_raw.x;
                break;
            }
            case LCD_DISP_DIR_180:
            {
                point[point_index].x = lcddev.width - point_raw.x;
                point[point_index].y = lcddev.height - point_raw.y;
                break;
            }
            case LCD_DISP_DIR_270:
            {
                point[point_index].x = lcddev.width - point_raw.y;
                point[point_index].y = point_raw.x;
                break;
            }
            }

            point[point_index].size = point_raw.size;
        }

        tp_info = 0;
        touch_write_reg(TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));

        return tp_cnt;
    }
    else
    {
        return 0;
    }
}

#endif
