#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"

#define USART_REC_LEN 200
#define USART_EN_RX 1

extern uint8_t USART_RX_BUF[USART_REC_LEN];
extern uint16_t USART_RX_STA;
void usart_init(uint32_t bound);
#endif
