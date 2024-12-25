#ifndef __DMA2D_H__
#define __DMA2D_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"

    void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d);
    void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d);

#ifdef __cplusplus
}
#endif

#endif /* __DMA2D_H__ */