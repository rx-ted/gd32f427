#include "touch.h"
#include "touch_iic.h"

#include "delay.h"

/* RGB LCD模块触摸部分寄存器定义 */
#define TOUCH_REG_CTRL 0x8040   // 控制寄存器
#define TOUCH_REG_PID 0x8140    // PID寄存器
#define TOUCH_REG_TPINFO 0x814E // 触摸状态寄存器

#define TOUCH_REG_TP1 0x8150 // 触摸点1数据寄存器
#define TOUCH_REG_TP2 0x8158 // 触摸点2数据寄存器
#define TOUCH_REG_TP3 0x8160 // 触摸点3数据寄存器
#define TOUCH_REG_TP4 0x8168 // 触摸点4数据寄存器
#define TOUCH_REG_TP5 0x8170 // 触摸点5数据寄存器

static const uint16_t TOUCH_REG_TPXs[TOUCH_TP_MAX] = {
    TOUCH_REG_TP1, TOUCH_REG_TP2, TOUCH_REG_TP3, TOUCH_REG_TP4, TOUCH_REG_TP5};

/* 触摸状态寄存器掩码 */
#define TOUCH_TPINFO_MASK_STA 0x80
#define TOUCH_TPINFO_MASK_PRESS 0X40
#define TOUCH_TPINFO_MASK_CNT 0x0F

touch_dev_t tp;

typedef struct
{
    uint16_t reg;
    uint8_t size;
    uint8_t *fb; // ascii code
} touch_read_info_t;

typedef struct
{
    uint8_t id;
    uint8_t device_address;
    touch_read_info_t pid;

} touch_param_t;

static touch_param_t touch_params[] = {
    /* touch register init function  */
    {0, 0x14, /* read product id */ {0x8140, 4, "1158"}},
};

device_result_t _touch_init();
uint8_t _touch_scan();

static uint8_t touch_write_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    uint8_t ret;

    touch_iic_start();
    touch_iic_send_byte((tp.device_address << 1) | TOUCH_IIC_WRITE);
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
        return DEVICE_ERROR;
    }

    return DEVICE_EOK;
}

static void touch_read_reg(uint16_t reg, uint8_t *buf, uint8_t len)
{
    uint8_t buf_index;
    touch_iic_start();
    touch_iic_send_byte((tp.device_address << 1) | TOUCH_IIC_WRITE);
    touch_iic_wait_ack();
    touch_iic_send_byte((uint8_t)(reg >> 8) & 0xFF);
    touch_iic_wait_ack();
    touch_iic_send_byte((uint8_t)reg & 0xFF);
    touch_iic_wait_ack();
    touch_iic_start();
    touch_iic_send_byte((tp.device_address << 1) | TOUCH_IIC_READ);
    touch_iic_wait_ack();

    for (buf_index = 0; buf_index < len; buf_index++)
    {
        buf[buf_index] = buf_index == len - 1 ? touch_iic_recv_byte(0) : touch_iic_recv_byte(1);
    }
    touch_iic_stop();
}

device_result_t check_touch_setup_param_by_id()
{
    printf("12345");
    uint8_t touch_param_indexs = sizeof(touch_params) / sizeof(touch_params[0]);
    for (uint8_t index = 0; index < touch_param_indexs; index++)
    {
        touch_param_t touch_param = touch_params[index];
        uint8_t read_id[touch_param.pid.size];
        touch_iic_start();
        touch_iic_send_byte((touch_param.device_address << 1) | TOUCH_IIC_WRITE);
        touch_iic_wait_ack();
        touch_iic_send_byte((uint8_t)(touch_param.pid.reg >> 8) & 0xFF);
        touch_iic_wait_ack();
        touch_iic_send_byte((uint8_t)touch_param.pid.reg & 0xFF);
        touch_iic_wait_ack();
        touch_iic_start();
        touch_iic_send_byte((touch_param.device_address << 1) | TOUCH_IIC_READ);
        touch_iic_wait_ack();

        for (uint8_t i = 0; i < touch_param.pid.size; i++)
        {
            read_id[i] = (i == touch_param.pid.size - 1 ? touch_iic_recv_byte(0) : touch_iic_recv_byte(1));
        }
        touch_iic_stop();
        read_id[touch_param.pid.size] = '\0';
        if (strcmp(read_id, touch_param.pid.size) != 0)
        {
            tp.id = index;
            tp.device_address = touch_param.device_address;

            printf("[touch]: Successfully Initialized, ID: %s\n", read_id);
            return DEVICE_EOK;
        }
    }
    printf("[E][touch]: Not found lcd controler!\n");
    return DEVICE_NOT_FOUND;
}

static void touch_hw_init(void)
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    TOUCH_PEN_GPIO_CLK_ENABLE();
    TOUCH_TCS_GPIO_CLK_ENABLE();

    gpio_init_struct.Pin = TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

    gpio_init_struct.Pin = TOUCH_TCS_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_TCS_GPIO_PORT, &gpio_init_struct);
}

static void touch_hw_reset()
{
    GPIO_InitTypeDef gpio_init_struct = {0};

    /* 配置PEN引脚为输出 */
    gpio_init_struct.Pin = TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init_struct.Pull = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

    TOUCH_TCS(0);
    TOUCH_PEN(0);
    TOUCH_PEN(1);
    delay_ms(1);
    TOUCH_TCS(1);
    delay_ms(10);

    /* 重新配置PEN引脚为输入 */
    gpio_init_struct.Pin = TOUCH_PEN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_INPUT;
    gpio_init_struct.Pull = GPIO_NOPULL;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(TOUCH_PEN_GPIO_PORT, &gpio_init_struct);

    delay_ms(100);
}

static void touch_sw_reset(void)
{
    uint8_t dat;
    dat = 0x02;
    touch_write_reg(TOUCH_REG_CTRL, &dat, 1);
    delay_ms(10);
    dat = 0x00;
    touch_write_reg(TOUCH_REG_CTRL, &dat, 1);
}

uint8_t _touch_scan()
{
    uint8_t tp_info;
    uint8_t tp_cnt;
    uint8_t point_index;
    uint8_t tpn_info[6];
    touch_point_t point_raw;

    touch_read_reg(TOUCH_REG_TPINFO, &tp_info, sizeof(tp_info));
    if ((tp_info & TOUCH_TPINFO_MASK_STA) == TOUCH_TPINFO_MASK_STA)
    {
        tp_cnt = tp_info & TOUCH_TPINFO_MASK_CNT;

        for (point_index = 0; point_index < tp_cnt; point_index++)
        {
            touch_read_reg(TOUCH_REG_TPXs[point_index], tpn_info, sizeof(tpn_info));
            point_raw.x = (uint16_t)(tpn_info[1] << 8) | tpn_info[0];
            point_raw.y = (uint16_t)(tpn_info[3] << 8) | tpn_info[2];
            point_raw.size = (uint16_t)(tpn_info[5] << 8) | tpn_info[4];

            switch (lcddev.dir)
            {
            case LCD_DISP_DIR_0:
            {
                tp.point[point_index].x = point_raw.x;
                tp.point[point_index].y = point_raw.y;
                break;
            }
            case LCD_DISP_DIR_90:
            {
                tp.point[point_index].x = point_raw.y;
                tp.point[point_index].y = lcddev.height - point_raw.x;
                break;
            }
            case LCD_DISP_DIR_180:
            {
                tp.point[point_index].x = lcddev.width - point_raw.x;
                tp.point[point_index].y = lcddev.height - point_raw.y;
                break;
            }
            case LCD_DISP_DIR_270:
            {
                tp.point[point_index].x = lcddev.width - point_raw.y;
                tp.point[point_index].y = point_raw.x;
                break;
            }
            }

            tp.point[point_index].size = point_raw.size;
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

void touch_init()
{

    tp.mode = lcddev.mode;

    touch_hw_init();
    touch_hw_reset();
    touch_iic_init();
    device_result_t res = check_touch_setup_param_by_id();
    if (res != DEVICE_EOK)
    {
        return;
    }
    touch_sw_reset();
    tp.scan = _touch_scan;
}