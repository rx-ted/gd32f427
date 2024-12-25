#include "fmc.h"
#include <stm32f4xx_hal_sram.h>

SRAM_HandleTypeDef hsram4;

/* FMC initialization function */
void MX_FMC_Init(void)
{
    FMC_NORSRAM_TimingTypeDef Timing = {0};
    hsram4.Instance = FMC_NORSRAM_DEVICE;
    hsram4.Extended = FMC_NORSRAM_EXTENDED_DEVICE;
    hsram4.Init.NSBank = FMC_NORSRAM_BANK4;
    hsram4.Init.DataAddressMux = FMC_DATA_ADDRESS_MUX_DISABLE;
    hsram4.Init.MemoryType = FMC_MEMORY_TYPE_SRAM;
    hsram4.Init.MemoryDataWidth = FMC_NORSRAM_MEM_BUS_WIDTH_16;
    hsram4.Init.BurstAccessMode = FMC_BURST_ACCESS_MODE_DISABLE;
    hsram4.Init.WaitSignalPolarity = FMC_WAIT_SIGNAL_POLARITY_LOW;
    hsram4.Init.WrapMode = FMC_WRAP_MODE_DISABLE;
    hsram4.Init.WaitSignalActive = FMC_WAIT_TIMING_BEFORE_WS;
    hsram4.Init.WriteOperation = FMC_WRITE_OPERATION_ENABLE;
    hsram4.Init.WaitSignal = FMC_WAIT_SIGNAL_DISABLE;
    hsram4.Init.ExtendedMode = FMC_EXTENDED_MODE_DISABLE;
    hsram4.Init.AsynchronousWait = FMC_ASYNCHRONOUS_WAIT_DISABLE;
    hsram4.Init.WriteBurst = FMC_WRITE_BURST_DISABLE;
    hsram4.Init.ContinuousClock = FMC_CONTINUOUS_CLOCK_SYNC_ONLY;
    hsram4.Init.PageSize = FMC_PAGE_SIZE_NONE;
    Timing.AddressSetupTime = 1;
    Timing.AddressHoldTime = 1;
    Timing.DataSetupTime = 1;
    Timing.BusTurnAroundDuration = 1;
    Timing.CLKDivision = 2;
    Timing.DataLatency = 2;
    Timing.AccessMode = FMC_ACCESS_MODE_A;

    if (HAL_SRAM_Init(&hsram4, &Timing, NULL) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_FMC_MspInit(void)
{
    /* USER CODE BEGIN FMC_MspInit 0 */

    /* USER CODE END FMC_MspInit 0 */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (FMC_Initialized)
    {
        return;
    }
    FMC_Initialized = 1;

    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_ENABLE();

    /** FMC GPIO Configuration
    PF12   ------> FMC_A6
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    PG12   ------> FMC_NE4
    */
    GPIO_InitStruct.Pin = LCD_RS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(LCD_RS_GPIO_Port, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_D4_Pin | LCD_D5_Pin | LCD_D6_Pin | LCD_D7_Pin | LCD_D8_Pin | LCD_D9_Pin | LCD_D10_Pin | LCD_D11_Pin | LCD_D12_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_D13_Pin | LCD_D14_Pin | LCD_D15_Pin | LCD_D0_Pin | LCD_D1_Pin | LCD_D2_Pin | LCD_D3_Pin | LCD_RD_Pin | LCD_WR_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = LCD_CS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF12_FMC;
    HAL_GPIO_Init(LCD_CS_GPIO_Port, &GPIO_InitStruct);

    /* USER CODE BEGIN FMC_MspInit 1 */

    /* USER CODE END FMC_MspInit 1 */
}

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
    /* USER CODE BEGIN SRAM_MspInit 0 */

    /* USER CODE END SRAM_MspInit 0 */
    HAL_FMC_MspInit();
    /* USER CODE BEGIN SRAM_MspInit 1 */

    /* USER CODE END SRAM_MspInit 1 */
}

void HAL_FMC_MspDeInit(void)
{
    /* USER CODE BEGIN FMC_MspDeInit 0 */

    /* USER CODE END FMC_MspDeInit 0 */
    if (FMC_DeInitialized)
    {
        return;
    }
    FMC_DeInitialized = 1;
    /* Peripheral clock enable */
    __HAL_RCC_FMC_CLK_DISABLE();

    /** FMC GPIO Configuration
    PF12   ------> FMC_A6
    PE7   ------> FMC_D4
    PE8   ------> FMC_D5
    PE9   ------> FMC_D6
    PE10   ------> FMC_D7
    PE11   ------> FMC_D8
    PE12   ------> FMC_D9
    PE13   ------> FMC_D10
    PE14   ------> FMC_D11
    PE15   ------> FMC_D12
    PD8   ------> FMC_D13
    PD9   ------> FMC_D14
    PD10   ------> FMC_D15
    PD14   ------> FMC_D0
    PD15   ------> FMC_D1
    PD0   ------> FMC_D2
    PD1   ------> FMC_D3
    PD4   ------> FMC_NOE
    PD5   ------> FMC_NWE
    PG12   ------> FMC_NE4
    */
    HAL_GPIO_DeInit(LCD_RS_GPIO_Port, LCD_RS_Pin);

    HAL_GPIO_DeInit(GPIOE, LCD_D4_Pin | LCD_D5_Pin | LCD_D6_Pin | LCD_D7_Pin | LCD_D8_Pin | LCD_D9_Pin | LCD_D10_Pin | LCD_D11_Pin | LCD_D12_Pin);

    HAL_GPIO_DeInit(GPIOD, LCD_D13_Pin | LCD_D14_Pin | LCD_D15_Pin | LCD_D0_Pin | LCD_D1_Pin | LCD_D2_Pin | LCD_D3_Pin | LCD_RD_Pin | LCD_WR_Pin);

    HAL_GPIO_DeInit(LCD_CS_GPIO_Port, LCD_CS_Pin);

    /* USER CODE BEGIN FMC_MspDeInit 1 */

    /* USER CODE END FMC_MspDeInit 1 */
}

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef *hsram)
{
    /* USER CODE BEGIN SRAM_MspDeInit 0 */

    /* USER CODE END SRAM_MspDeInit 0 */
    HAL_FMC_MspDeInit();
    /* USER CODE BEGIN SRAM_MspDeInit 1 */

    /* USER CODE END SRAM_MspDeInit 1 */
}
