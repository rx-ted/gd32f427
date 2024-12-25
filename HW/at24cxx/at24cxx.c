#include "at24cxx.h"
#include "delay.h"
#include "i2c.h"

I2C_HandleTypeDef hi2c1;

/**
 * @brief       初始化IIC接口
 * @param       无
 * @retval      无
 */
void at24cxx_init(void)
{

    MX_I2C1_Init();  // Pls see Apps/XXexample/Src/i2c.c.
    at24cxx_check(); // check iic if normal.
}

/**
 * @brief       检查AT24CXX是否正常
 *   @note      检测原理: 在器件的末地址写如0X55, 然后再读取, 如果读取值为0X55
 *              则表示检测正常. 否则,则表示检测失败.
 *
 * @param       无
 * @retval      检测结果
 *              0: 检测成功
 *              1: 检测失败
 */
uint8_t at24cxx_check(void)
{
    uint8_t temp;
    uint16_t addr = EE_TYPE;
    uint8_t data = 0x55;

    at24cxx_read(addr, &temp, 1);
    if (temp == data)
    {
        printf("AT24Cxx: Check Okay!\n");
        return 0;
    }
    else /* 排除第一次初始化的情况 */
    {
        at24cxx_write(addr, &data, 1);
        at24cxx_read(addr, &temp, 1);
        if (temp != data)
        {
            printf("AT24Cxx: Check Failed!\n");
            return 1;
        }
    }
    return 0;
}

/**
 * @brief       在AT24CXX里面的指定地址开始读出指定个数的数据
 * @param       addr    : 开始读出的地址 对24c02为0~255
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要读出数据的个数
 * @retval      无
 */
void at24cxx_read(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    HAL_I2C_Mem_Read(&hi2c1, AT24C_DEV_ADDR, addr, I2C_MEMADD_SIZE_8BIT, pbuf, datalen, HAL_MAX_DELAY);
}

/**
 * @brief       在AT24CXX里面的指定地址开始写入指定个数的数据
 * @param       addr    : 开始写入的地址 对24c02为0~255
 * @param       pbuf    : 数据数组首地址
 * @param       datalen : 要写入数据的个数
 * @retval      无
 */
void at24cxx_write(uint16_t addr, uint8_t *pbuf, uint16_t datalen)
{
    HAL_I2C_Mem_Write(&hi2c1, AT24C_DEV_ADDR, addr, I2C_MEMADD_SIZE_8BIT, pbuf, datalen, HAL_MAX_DELAY);
}
