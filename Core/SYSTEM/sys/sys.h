/************************************************
 * 串口初始化代码(一般是串口0)，支持printf
 ************************************************/

#ifndef __USART_H
#define __USART_H

#include "stdio.h"
#include "sys.h"

#define USART_REC_LEN 200 /* 定义最大接收字节数 200 */
#define USART_EN_RX 1 /* 使能（1）/禁止（0）串口0接收 */

extern uint8_t USART_RX_BUF[USART_REC_LEN]; /* 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符
                                             */
extern uint16_t USART_RX_STA;               /* 接收状态标记 */

void usart_init(uint32_t bound); /* 串口初始化函数 */

#endif
/************************************************
 * 使用SysTick的普通计数模式对延迟进行管理
 * 包括delay_us,delay_ms
 ************************************************/

#ifndef __DELAY_H
#define __DELAY_H

#include "sys.h"

void delay_init(uint16_t sysclk); /* 初始化延迟函数 */
void delay_ms(uint16_t nms);      /* 延时nms */
void delay_us(uint32_t nus);      /* 延时nus */

#endif
/*!
    \file    gd32f4xx_libopt.h
    \brief   library optional for gd32f4xx

    \version 2023-06-25, V3.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef GD32F4XX_LIBOPT_H
#define GD32F4XX_LIBOPT_H

#if defined (GD32F450) || defined (GD32F405) || defined (GD32F407) || defined (GD32F470) || defined (GD32F425) || defined (GD32F427)
#include "gd32f4xx_rcu.h"
#include "gd32f4xx_adc.h"
#include "gd32f4xx_can.h"
#include "gd32f4xx_crc.h"
#include "gd32f4xx_ctc.h"
#include "gd32f4xx_dac.h"
#include "gd32f4xx_dbg.h"
#include "gd32f4xx_dci.h"
#include "gd32f4xx_dma.h"
#include "gd32f4xx_exti.h"
#include "gd32f4xx_fmc.h"
#include "gd32f4xx_fwdgt.h"
#include "gd32f4xx_gpio.h"
#include "gd32f4xx_syscfg.h"
#include "gd32f4xx_i2c.h"
#include "gd32f4xx_iref.h"
#include "gd32f4xx_pmu.h"
#include "gd32f4xx_rtc.h"
#include "gd32f4xx_sdio.h"
#include "gd32f4xx_spi.h"
#include "gd32f4xx_timer.h"
#include "gd32f4xx_trng.h"
#include "gd32f4xx_usart.h"
#include "gd32f4xx_wwdgt.h"
#include "gd32f4xx_misc.h"
#endif

#if defined (GD32F450) || defined (GD32F470)
#include "gd32f4xx_enet.h"
#include "gd32f4xx_exmc.h"
#include "gd32f4xx_ipa.h"
#include "gd32f4xx_tli.h"
#endif

#if defined (GD32F407) || defined (GD32F427)
#include "gd32f4xx_enet.h"
#include "gd32f4xx_exmc.h"
#endif

#endif /* GD32F4XX_LIBOPT_H */
/*!
    \file    gd32f4xx_it.h
    \brief   the header file of the ISR

    \version 2023-06-25, V3.1.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2023, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#ifndef GD32F4XX_IT_H
#define GD32F4XX_IT_H

#include "gd32f4xx.h"

/* function declarations */
/* this function handles NMI exception */
void NMI_Handler(void);
/* this function handles HardFault exception */
void HardFault_Handler(void);
/* this function handles MemManage exception */
void MemManage_Handler(void);
/* this function handles BusFault exception */
void BusFault_Handler(void);
/* this function handles UsageFault exception */
void UsageFault_Handler(void);
/* this function handles SVC exception */
void SVC_Handler(void);
/* this function handles DebugMon exception */
void DebugMon_Handler(void);
/* this function handles PendSV exception */
void PendSV_Handler(void);
/* this function handles SysTick exception */
void SysTick_Handler(void);

#endif /* GD32F4XX_IT_H */
