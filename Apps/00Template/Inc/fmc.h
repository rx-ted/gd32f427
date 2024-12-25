#ifndef __FMC_H__
#define __FMC_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"

    static uint32_t FMC_Initialized = 0;
    static uint32_t FMC_DeInitialized = 0;

    void MX_FMC_Init(void);
    void HAL_FMC_MspInit(void);
    void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram);
    void HAL_FMC_MspDeInit(void);
    void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef *hsram);

#ifdef __cplusplus
}
#endif

#endif /* __FMC_H__ */