#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"

    void MX_I2C1_Init(void);
    void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c);
    void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c);

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */