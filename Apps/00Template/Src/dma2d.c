#include "dma2d.h"

extern DMA2D_HandleTypeDef hdma2d;

/**
 * @brief DMA2D Initialization Function
 * @param None
 * @retval None
 */
static void MX_DMA2D_Init(void)
{

    /* USER CODE BEGIN DMA2D_Init 0 */

    /* USER CODE END DMA2D_Init 0 */

    /* USER CODE BEGIN DMA2D_Init 1 */

    /* USER CODE END DMA2D_Init 1 */
    hdma2d.Instance = DMA2D;
    hdma2d.Init.Mode = DMA2D_M2M;
    hdma2d.Init.ColorMode = DMA2D_OUTPUT_RGB565;
    hdma2d.Init.OutputOffset = 0;
    hdma2d.LayerCfg[1].InputOffset = 0;
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB565;
    hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hdma2d.LayerCfg[1].InputAlpha = 0;
    if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
    {
        Error_Handler();
    }
    if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
    {
        Error_Handler();
    }
    /* USER CODE BEGIN DMA2D_Init 2 */

    /* USER CODE END DMA2D_Init 2 */
}

/**
 * @brief DMA2D MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hdma2d: DMA2D handle pointer
 * @retval None
 */
void HAL_DMA2D_MspInit(DMA2D_HandleTypeDef *hdma2d)
{
    if (hdma2d->Instance == DMA2D)
    {
        /* USER CODE BEGIN DMA2D_MspInit 0 */

        /* USER CODE END DMA2D_MspInit 0 */
        /* Peripheral clock enable */
        __HAL_RCC_DMA2D_CLK_ENABLE();
        /* USER CODE BEGIN DMA2D_MspInit 1 */

        /* USER CODE END DMA2D_MspInit 1 */
    }
}

/**
 * @brief DMA2D MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hdma2d: DMA2D handle pointer
 * @retval None
 */
void HAL_DMA2D_MspDeInit(DMA2D_HandleTypeDef *hdma2d)
{
    if (hdma2d->Instance == DMA2D)
    {
        /* USER CODE BEGIN DMA2D_MspDeInit 0 */

        /* USER CODE END DMA2D_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_DMA2D_CLK_DISABLE();
        /* USER CODE BEGIN DMA2D_MspDeInit 1 */

        /* USER CODE END DMA2D_MspDeInit 1 */
    }
}
