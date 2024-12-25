#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"

    static void MX_USART1_UART_Init(void);
    void HAL_UART_MspInit(UART_HandleTypeDef *huart);
    void HAL_UART_MspDeInit(UART_HandleTypeDef *huart);

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */