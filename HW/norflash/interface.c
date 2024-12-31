#include "interface.h"

extern SPI_HandleTypeDef hspi3;

uint8_t spi_write_read(uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len)
{
    uint8_t res;
    F_CS(0);
    if (in_len > 0)
    {
        res = HAL_SPI_Transmit(&hspi3, in_buf, in_len, 1000);
        if (res != HAL_OK)
        {
            F_CS(1);
            return DEVICE_TIMEOUT;
        }
    }
    if (out_len > 0)
    {
        res = HAL_SPI_Receive(&hspi3, out_buf, out_len, 1000);
        if (res != HAL_OK)
        {
            F_CS(1);
            return DEVICE_TIMEOUT;
        }
    }
    F_CS(1);
    return DEVICE_EOK;
}