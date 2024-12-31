#ifndef W25QXX_INTERFACE_H__
#define W25QXX_INTERFACE_H__

#include "config.h"

#define F_CS(x)                                                                                                                      \
    do                                                                                                                               \
    {                                                                                                                                \
        x ? HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_SET) : HAL_GPIO_WritePin(F_CS_GPIO_Port, F_CS_Pin, GPIO_PIN_RESET); \
    } while (0)

// uint8_t spi_transmit(uint8_t *tx, uint8_t *rx, uint16_t len);
uint8_t spi_write_read(uint8_t *in_buf, uint32_t in_len, uint8_t *out_buf, uint32_t out_len);

#endif //  W25QXX_INTERFACE_H__