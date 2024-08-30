/************************************************
 * 串口初始化代码(一般是串口0)，支持printf
 ************************************************/

#include "usart.h"
#include "sys.h"

/* 如果使用os,则包括下面的头文件即可. */
#if SYS_SUPPORT_OS
#include "includes.h" /* os 使用 */
#endif

/******************************************************************************************/
/* 加入以下代码, 支持printf函数, 而不需要选择use MicroLIB */

#if 1

#if (__ARMCC_VERSION >= 6010050)           /* 使用AC6编译器时 */
__asm(".global __use_no_semihosting\n\t"); /* 声明不使用半主机模式 */
__asm(
    ".global __ARM_use_no_argv \n\t"); /* AC6下需要声明main函数为无参数格式，否则部分例程可能出现半主机模式
                                        */

#else
/* 使用AC5编译器时, 要在这里定义__FILE 和 不使用半主机模式 */
#pragma import(__use_no_semihosting)

struct __FILE {
  int handle;
  /* Whatever you require here. If the only file you are using is */
  /* standard output using printf() for debugging, no file handling */
  /* is required. */
};

#endif

/* 不使用半主机模式，至少需要重定义_ttywrch\_sys_exit\_sys_command_string函数,以同时兼容AC6和AC5模式
 */
int _ttywrch(int ch) {
  ch = ch;
  return ch;
}

/* 定义_sys_exit()以避免使用半主机模式 */
void _sys_exit(int x) { x = x; }

char *_sys_command_string(char *cmd, int len) { return NULL; }

/* FILE 在 stdio.h里面定义. */
FILE __stdout;

/* MDK下需要重定义fputc函数, printf函数最终会通过调用fputc输出字符串到串口 */
int fputc(int ch, FILE *f) {
  while (RESET == usart_flag_get(USART0, USART_FLAG_TC))
    ; /* 等待上一个字符发送完成 */

  usart_data_transmit(USART0,
                      (uint8_t)ch); /* 将要发送的字符 ch 写入到DR寄存器 */
  return ch;
}
#endif
/******************************************************************************************/

#if USART_EN_RX /* 如果使能了接收 */

/* 接收缓冲, 最大USART_REC_LEN个字节. */
uint8_t USART_RX_BUF[USART_REC_LEN];

/*  接收状态
 *  bit15，      接收完成标志
 *  bit14，      接收到0x0d
 *  bit13~0，    接收到的有效字节数目
 */
uint16_t USART_RX_STA = 0;

/**
 * @brief       串口0中断服务函数
 * @param       无
 * @retval      无
 */
void USART0_IRQHandler(void) {
  uint8_t res;
#if SYSTEM_SUPPORT_OS /* 如果SYS_SUPPORT_OS为真，则需要支持OS. */
  OSIntEnter();
#endif
  if (usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE) !=
      RESET) /* 接收到数据 */
  {
    res = usart_data_receive(USART0); /* 读取接收到的数据 */

    if ((USART_RX_STA & 0x8000) == 0) /* 接收未完成 */
    {
      if (USART_RX_STA & 0x4000) /* 接收到了0x0d  */
      {
        if (res != 0x0a) /* 接收到了0x0a? (必须先接收到到0x0d,才检查0x0a) */
        {
          USART_RX_STA = 0; /* 接收错误,重新开始 */
        } else {
          USART_RX_STA |= 0x8000; /* 收到了0x0a,标记接收完成了 */
        }
      } else /* 还没收到0X0d  */
      {
        if (res == 0x0d) {
          USART_RX_STA |= 0x4000; /* 标记接收到了 0x0d  */
        } else {
          USART_RX_BUF[USART_RX_STA & 0X3FFF] =
              res; /* 存储数据到USART_RX_BUF */
          USART_RX_STA++;
          if (USART_RX_STA > (USART_REC_LEN - 1))
            USART_RX_STA = 0; /* 接收数据溢出,重新开始接收 */
        }
      }
    }
  }

#if SYSTEM_SUPPORT_OS /* 如果SYS_SUPPORT_OS为真，则需要支持OS. */
  OSIntExit();
#endif
}
#endif

/**
 * @brief       串口0初始化函数
 * @param       bound: 波特率, 根据自己需要设置波特率值
 * @retval      无
 */
void usart_init(uint32_t bound) {
  /* IO 及 时钟配置 */
  rcu_periph_clock_enable(RCU_GPIOA);  /* 使能GPIOA时钟 */
  rcu_periph_clock_enable(RCU_USART0); /* 使能串口时钟 */

  /* 设置USART0_Tx的复用功能选择 */
  gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_9);

  /* 设置USART0_Rx的复用功能选择 */
  gpio_af_set(GPIOA, GPIO_AF_7, GPIO_PIN_10);

  /* USART0_Tx的模式设置 */
  gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_9);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);

  /* USART0_Rx的模式设置 */
  gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_10);
  gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);

  /* 配置USART的参数 */
  usart_deinit(USART0);                       /* 复位USART0 */
  usart_baudrate_set(USART0, bound);          /* 设置波特率 */
  usart_stop_bit_set(USART0, USART_STB_1BIT); /* 一个停止位 */
  usart_word_length_set(USART0, USART_WL_8BIT); /* 字长为8位数据格式 */
  usart_parity_config(USART0, USART_PM_NONE); /* 无奇偶校验位 */
  usart_transmit_config(USART0, USART_TRANSMIT_ENABLE); /* 使能发送 */
#if USART_EN_RX /* 如果使能了接收 */
  usart_receive_config(USART0, USART_RECEIVE_ENABLE); /* 使能接收 */
  usart_interrupt_enable(USART0, USART_INT_RBNE); /* 使能接收缓冲区非空中断 */
  /* 配置NVIC，并设置中断优先级 */
  nvic_irq_enable(USART0_IRQn, 3, 3); /* 抢占优先级3，子优先级3 */
#endif
  usart_enable(USART0); /* 使能串口 */
}
/************************************************
 * 使用SysTick的普通计数模式对延迟进行管理
 * 包括delay_us,delay_ms
 ************************************************/

#include "delay.h"
#include "sys.h"


static uint16_t g_fac_us = 0; /* us延时倍乘数 */

/* 如果SYS_SUPPORT_OS定义了,说明要支持OS了(不限于UCOS) */
#if SYS_SUPPORT_OS

/* 添加公共头文件 ( ucos需要用到) */
#include "includes.h"

/* 定义g_fac_ms变量, 表示ms延时的倍乘数, 代表每个节拍的ms数,
 * (仅在使能os的时候,需要用到) */
static uint16_t g_fac_ms = 0;

/*
 *  当delay_us/delay_ms需要支持OS的时候需要三个与OS相关的宏定义和函数来支持
 *  首先是3个宏定义:
 *      delay_osrunning :用于表示OS当前是否正在运行,以决定是否可以使用相关函数
 *      delay_ostickspersec:用于表示OS设定的时钟节拍,delay_init将根据这个参数来初始化systick
 *      delay_osintnesting
 * :用于表示OS中断嵌套级别,因为中断里面不可以调度,delay_ms使用该参数来决定如何运行
 *  然后是3个函数:
 *      delay_osschedlock  :用于锁定OS任务调度,禁止调度
 *      delay_osschedunlock:用于解锁OS任务调度,重新开启调度
 *      delay_ostimedly    :用于OS延时,可以引起任务调度.
 *
 *  本例程仅作UCOSII的支持,其他OS,请自行参考着移植
 */

/* 支持UCOSII */
#define delay_osrunning OSRunning /* OS是否运行标记,0,不运行;1,在运行 */
#define delay_ostickspersec OS_TICKS_PER_SEC /* OS时钟节拍,即每秒调度次数 */
#define delay_osintnesting OSIntNesting /* 中断嵌套级别,即中断嵌套次数 */

/**
 * @brief       us级延时时,关闭任务调度(防止打断us级延迟)
 * @param       无
 * @retval      无
 */
static void delay_osschedlock(void) {
  OSSchedLock(); /* UCOSII的方式,禁止调度，防止打断us延时 */
}

/**
 * @brief       us级延时时,恢复任务调度
 * @param       无
 * @retval      无
 */
static void delay_osschedunlock(void) {
  OSSchedUnlock(); /* UCOSII的方式,恢复调度 */
}

/**
 * @brief       us级延时时,恢复任务调度
 * @param       ticks: 延时的节拍数
 * @retval      无
 */
static void delay_ostimedly(uint32_t ticks) {
  OSTimeDly(ticks); /* UCOSII延时 */
}

/**
 * @brief       systick中断服务函数,使用OS时用到
 * @param       ticks: 延时的节拍数
 * @retval      无
 */
void SysTick_Handler(void) {
  if (delay_osrunning == 1) /* OS开始跑了,才执行正常的调度处理 */
  {
    OSIntEnter(); /* 进入中断 */
    OSTimeTick(); /* 调用ucos的时钟服务程序 */
    OSIntExit();  /* 触发任务切换软中断 */
  }
}

#endif

/**
 * @brief       初始化延迟函数
 * @param       sysclk: 系统时钟频率, 即CPU频率(HCLK)
 * @retval      无
 */
void delay_init(uint16_t sysclk) {
#if SYS_SUPPORT_OS /* 如果需要支持OS. */
  uint32_t reload;
#endif
  SysTick->CTRL =
      0; /* 清Systick状态，以便下一步重设，如果这里开了中断会关闭其中断 */
  SysTick->CTRL &= ~(
      1 << 2); /* SYSTICK使用内核时钟源8分频,因systick的计数器最大值只有2^24 */
  g_fac_us =
      sysclk / 8; /* 不论是否使用OS,g_fac_us都需要使用,作为1us的基础时基 */
  SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* 开启SYSTICK */
  SysTick->LOAD =
      0XFFFFFF; /* 注意systick计数器24位，所以这里设置最大重装载值 */
#if SYS_SUPPORT_OS     /* 如果需要支持OS. */
  reload = sysclk / 8; /* 每秒钟的计数次数 单位为M */
  reload *=
      1000000 /
      delay_ostickspersec; /* 根据delay_ostickspersec设定溢出时间
                            * reload为24位寄存器,最大值:16777216,在200M下,约合0.64s左右
                            */
  g_fac_ms = 1000 / delay_ostickspersec; /* 代表OS可以延时的最少单位 */
  SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; /* 开启SYSTICK中断 */
  SysTick->LOAD = reload; /* 每1/delay_ostickspersec秒中断一次 */

#endif
}

/**
 * @brief       延时nus
 * @note        无论是否使用OS, 都是用时钟摘取法来做us延时
 * @param       nus: 要延时的us数.
 * @note        nus取值范围: 0 ~ (2^32 / g_fac_us) (g_fac_us一般等于系统主频/8,
 * 自行套入计算)
 * @retval      无
 */
void delay_us(uint32_t nus) {
  uint32_t ticks;
  uint32_t told, tnow, tcnt = 0;
  uint32_t reload;
  reload = SysTick->LOAD; /* LOAD的值 */
  ticks = nus * g_fac_us; /* 需要的节拍数 */

#if SYS_SUPPORT_OS     /* 如果需要支持OS */
  delay_osschedlock(); /* 阻止OS调度，防止打断us延时 */
#endif

  told = SysTick->VAL; /* 刚进入时的计数器值 */
  while (1) {
    tnow = SysTick->VAL;

    if (tnow != told) {
      if (tnow < told) {
        tcnt +=
            told - tnow; /* 这里注意一下SYSTICK是一个递减的计数器就可以了. */
      } else {
        tcnt += reload - tnow + told;
      }
      told = tnow;
      if (tcnt >= ticks) {
        break; /* 时间超过/等于要延迟的时间,则退出. */
      }
    }
  }

#if SYS_SUPPORT_OS       /* 如果需要支持OS */
  delay_osschedunlock(); /* 恢复OS调度 */
#endif
}

/**
 * @brief       延时nms
 * @param       nms: 要延时的ms数 (0< nms <= (2^32 / g_fac_us /
 * 1000))(g_fac_us一般等于系统主频/8, 自行套入计算)
 * @retval      无
 */
void delay_ms(uint16_t nms) {

#if SYS_SUPPORT_OS /* 如果需要支持OS, 则根据情况调用os延时以释放CPU */
  if (delay_osrunning &&
      delay_osintnesting ==
          0) /* 如果OS已经在跑了,并且不是在中断里面(中断里面不能任务调度) */
  {
    if (nms >= g_fac_ms) /* 延时的时间大于OS的最少时间周期 */
    {
      delay_ostimedly(nms / g_fac_ms); /* OS延时 */
    }

    nms %= g_fac_ms; /* OS已经无法提供这么小的延时了,采用普通方式延时 */
  }
#endif

  delay_us((uint32_t)(nms * 1000)); /* 普通方式延时 */
}
/*!
    \file    gd32f4xx_it.c
    \brief   interrupt service routines

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

#include "gd32f4xx_it.h"


/*!
    \brief    this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
}

/*!
    \brief    this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while(1) {
    }
}

/*!
    \brief    this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief    this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief    this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief    this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
}
/*!
    \file  system_gd32f4xx.c
    \brief CMSIS Cortex-M4 Device Peripheral Access Layer Source File for
           GD32F4xx Device Series
*/

/* Copyright (c) 2012 ARM LIMITED

   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:
   - Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
   - Neither the name of ARM nor the names of its contributors may be used
     to endorse or promote products derived from this software without
     specific prior written permission.
   *
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
   ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
   POSSIBILITY OF SUCH DAMAGE.
   ---------------------------------------------------------------------------*/

/* This file refers the CMSIS standard, some adjustments are made according to GigaDevice chips */

#include "gd32f4xx.h"

/* system frequency define */
#define __IRC16M          (IRC16M_VALUE)            /* internal 16 MHz RC oscillator frequency */
#define __HXTAL           (HXTAL_VALUE)             /* high speed crystal oscillator frequency */
#define __SYS_OSC_CLK     (__IRC16M)                /* main oscillator frequency */

/* select a system clock by uncommenting the following line */
//#define __SYSTEM_CLOCK_IRC16M                   (uint32_t)(__IRC16M)
//#define __SYSTEM_CLOCK_HXTAL                    (uint32_t)(__HXTAL)
//#define __SYSTEM_CLOCK_120M_PLL_IRC16M          (uint32_t)(120000000)
//#define __SYSTEM_CLOCK_120M_PLL_8M_HXTAL        (uint32_t)(120000000)
//#define __SYSTEM_CLOCK_120M_PLL_25M_HXTAL       (uint32_t)(120000000)
//#define __SYSTEM_CLOCK_168M_PLL_IRC16M          (uint32_t)(168000000)
//#define __SYSTEM_CLOCK_168M_PLL_8M_HXTAL        (uint32_t)(168000000)
//#define __SYSTEM_CLOCK_168M_PLL_25M_HXTAL       (uint32_t)(168000000)
//#define __SYSTEM_CLOCK_200M_PLL_IRC16M          (uint32_t)(200000000)
//#define __SYSTEM_CLOCK_200M_PLL_8M_HXTAL        (uint32_t)(200000000)
//#define __SYSTEM_CLOCK_200M_PLL_25M_HXTAL        (uint32_t)(200000000)
#if defined (GD32F450) || defined (GD32F405) || defined (GD32F407) || defined (GD32F425) || defined (GD32F427)
#define __SYSTEM_CLOCK_200M_PLL_8M_HXTAL         (uint32_t)(200000000)
#endif

#if defined (GD32F470)
#define __SYSTEM_CLOCK_240M_PLL_25M_HXTAL         (uint32_t)(240000000)
#endif

//#define __SYSTEM_CLOCK_240M_PLL_IRC16M          (uint32_t)(240000000)
//#define __SYSTEM_CLOCK_240M_PLL_8M_HXTAL        (uint32_t)(240000000)
//#define __SYSTEM_CLOCK_240M_PLL_25M_HXTAL       (uint32_t)(240000000)

#define RCU_MODIFY(__delay)     do{                                     \
                                    volatile uint32_t i;                \
                                    if(0 != __delay){                   \
                                        RCU_CFG0 |= RCU_AHB_CKSYS_DIV2; \
                                        for(i=0; i<__delay; i++){       \
                                        }                               \
                                        RCU_CFG0 |= RCU_AHB_CKSYS_DIV4; \
                                        for(i=0; i<__delay; i++){       \
                                        }                               \
                                    }                                   \
                                }while(0)

#define SEL_IRC16M      0x00U
#define SEL_HXTAL       0x01U
#define SEL_PLLP        0x02U
                        
/* set the system clock frequency and declare the system clock configuration function */
#ifdef __SYSTEM_CLOCK_IRC16M
uint32_t SystemCoreClock = __SYSTEM_CLOCK_IRC16M;
static void system_clock_16m_irc16m(void);
#elif defined (__SYSTEM_CLOCK_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_HXTAL;
static void system_clock_hxtal(void);
#elif defined (__SYSTEM_CLOCK_120M_PLL_IRC16M)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_120M_PLL_IRC16M;
static void system_clock_120m_irc16m(void);
#elif defined (__SYSTEM_CLOCK_120M_PLL_8M_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_120M_PLL_8M_HXTAL;
static void system_clock_120m_8m_hxtal(void);
#elif defined (__SYSTEM_CLOCK_120M_PLL_25M_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_120M_PLL_25M_HXTAL;
static void system_clock_120m_25m_hxtal(void);
#elif defined (__SYSTEM_CLOCK_168M_PLL_IRC16M)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_168M_PLL_IRC16M;
static void system_clock_168m_irc16m(void);
#elif defined (__SYSTEM_CLOCK_168M_PLL_8M_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_168M_PLL_8M_HXTAL;
static void system_clock_168m_8m_hxtal(void);
#elif defined (__SYSTEM_CLOCK_168M_PLL_25M_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_168M_PLL_25M_HXTAL;
static void system_clock_168m_25m_hxtal(void);
#elif defined (__SYSTEM_CLOCK_200M_PLL_IRC16M)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_200M_PLL_IRC16M;
static void system_clock_200m_irc16m(void);
#elif defined (__SYSTEM_CLOCK_200M_PLL_8M_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_200M_PLL_8M_HXTAL;
static void system_clock_200m_8m_hxtal(void);
#elif defined (__SYSTEM_CLOCK_200M_PLL_25M_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_200M_PLL_25M_HXTAL;
static void system_clock_200m_25m_hxtal(void);
#elif defined (__SYSTEM_CLOCK_240M_PLL_IRC16M)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_240M_PLL_IRC16M;
static void system_clock_240m_irc16m(void);
#elif defined (__SYSTEM_CLOCK_240M_PLL_8M_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_240M_PLL_8M_HXTAL;
static void system_clock_240m_8m_hxtal(void);
#elif defined (__SYSTEM_CLOCK_240M_PLL_25M_HXTAL)
uint32_t SystemCoreClock = __SYSTEM_CLOCK_240M_PLL_25M_HXTAL;
static void system_clock_240m_25m_hxtal(void);

#endif /* __SYSTEM_CLOCK_IRC16M */

/* configure the system clock */
static void system_clock_config(void);

/*!
    \brief      setup the microcontroller system, initialize the system
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SystemInit (void)
{
    /* FPU settings */
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
    SCB->CPACR |= ((3UL << 10*2)|(3UL << 11*2));  /* set CP10 and CP11 Full Access */
#endif
    /* Reset the RCU clock configuration to the default reset state */
    /* Set IRC16MEN bit */
    RCU_CTL |= RCU_CTL_IRC16MEN;
    while(0U == (RCU_CTL & RCU_CTL_IRC16MSTB)){
    }
    RCU_MODIFY(0x50);
    
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    
    /* Reset HXTALEN, CKMEN and PLLEN bits */
    RCU_CTL &= ~(RCU_CTL_PLLEN | RCU_CTL_CKMEN | RCU_CTL_HXTALEN);

    /* Reset HSEBYP bit */
    RCU_CTL &= ~(RCU_CTL_HXTALBPS);
    
    /* Reset CFG0 register */
    RCU_CFG0 = 0x00000000U;

    /* wait until IRC16M is selected as system clock */
    while(0 != (RCU_CFG0 & RCU_SCSS_IRC16M)){
    }

    /* Reset PLLCFGR register */
    RCU_PLL = 0x24003010U;

    /* Disable all interrupts */
    RCU_INT = 0x00000000U;
         
    /* Configure the System clock source, PLL Multiplier and Divider factors, 
        AHB/APBx prescalers and Flash settings */
    system_clock_config();
}
/*!
    \brief      configure the system clock
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_config(void)
{
#ifdef __SYSTEM_CLOCK_IRC16M
    system_clock_16m_irc16m();
#elif defined (__SYSTEM_CLOCK_HXTAL)
    system_clock_hxtal();
#elif defined (__SYSTEM_CLOCK_120M_PLL_IRC16M)
    system_clock_120m_irc16m();
#elif defined (__SYSTEM_CLOCK_120M_PLL_8M_HXTAL)
    system_clock_120m_8m_hxtal();
#elif defined (__SYSTEM_CLOCK_120M_PLL_25M_HXTAL)
    system_clock_120m_25m_hxtal();
#elif defined (__SYSTEM_CLOCK_168M_PLL_IRC16M)
    system_clock_168m_irc16m();
#elif defined (__SYSTEM_CLOCK_168M_PLL_8M_HXTAL)
    system_clock_168m_8m_hxtal();
#elif defined (__SYSTEM_CLOCK_168M_PLL_25M_HXTAL)
    system_clock_168m_25m_hxtal();
#elif defined (__SYSTEM_CLOCK_200M_PLL_IRC16M)
    system_clock_200m_irc16m();
#elif defined (__SYSTEM_CLOCK_200M_PLL_8M_HXTAL)
    system_clock_200m_8m_hxtal();
#elif defined (__SYSTEM_CLOCK_200M_PLL_25M_HXTAL)
    system_clock_200m_25m_hxtal();
#elif defined (__SYSTEM_CLOCK_240M_PLL_IRC16M)
    system_clock_240m_irc16m();
#elif defined (__SYSTEM_CLOCK_240M_PLL_8M_HXTAL)
    system_clock_240m_8m_hxtal();
#elif defined (__SYSTEM_CLOCK_240M_PLL_25M_HXTAL)
    system_clock_240m_25m_hxtal();
#endif /* __SYSTEM_CLOCK_IRC16M */   
}

#ifdef __SYSTEM_CLOCK_IRC16M
/*!
    \brief      configure the system clock to 16M by IRC16M
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_16m_irc16m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable IRC16M */
    RCU_CTL |= RCU_CTL_IRC16MEN;
    
    /* wait until IRC16M is stable or the startup time is longer than IRC16M_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_IRC16MSTB);
    }while((0U == stab_flag) && (IRC16M_STARTUP_TIMEOUT != timeout));
    
    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_IRC16MSTB)){
        while(1){
        }
    }
    
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;
    
    /* select IRC16M as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_IRC16M;
    
    /* wait until IRC16M is selected as system clock */
    while(0 != (RCU_CFG0 & RCU_SCSS_IRC16M)){
    }
}

#elif defined (__SYSTEM_CLOCK_HXTAL)
/*!
    \brief      configure the system clock to HXTAL
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;
    
    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));
    
    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }
    
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV1;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV1;
    
    /* select HXTAL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_HXTAL;
    
    /* wait until HXTAL is selected as system clock */
    while(0 == (RCU_CFG0 & RCU_SCSS_HXTAL)){
    }
}

#elif defined (__SYSTEM_CLOCK_120M_PLL_IRC16M)
/*!
    \brief      configure the system clock to 120M by PLL which selects IRC16M as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_120m_irc16m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable IRC16M */
    RCU_CTL |= RCU_CTL_IRC16MEN;

    /* wait until IRC16M is stable or the startup time is longer than IRC16M_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_IRC16MSTB);
    }while((0U == stab_flag) && (IRC16M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_IRC16MSTB)){
        while(1){
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* IRC16M is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 16, PLL_N = 240, PLL_P = 2, PLL_Q = 5 */ 
    RCU_PLL = (16U | (240U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_IRC16M) | (5U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 120 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_120M_PLL_8M_HXTAL)
/*!
    \brief      configure the system clock to 120M by PLL which selects HXTAL(8M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_120m_8m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 8, PLL_N = 240, PLL_P = 2, PLL_Q = 5 */ 
    RCU_PLL = (8U | (240U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_HXTAL) | (5U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 120 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_120M_PLL_25M_HXTAL)
/*!
    \brief      configure the system clock to 120M by PLL which selects HXTAL(25M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_120m_25m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 25, PLL_N = 240, PLL_P = 2, PLL_Q = 5 */ 
    RCU_PLL = (25U | (240U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_HXTAL) | (5U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 120 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_168M_PLL_IRC16M)
/*!
    \brief      configure the system clock to 168M by PLL which selects IRC16M as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_168m_irc16m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable IRC16M */
    RCU_CTL |= RCU_CTL_IRC16MEN;

    /* wait until IRC16M is stable or the startup time is longer than IRC16M_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_IRC16MSTB);
    }while((0U == stab_flag) && (IRC16M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_IRC16MSTB)){
        while(1){
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* IRC16M is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 16, PLL_N = 336, PLL_P = 2, PLL_Q = 7 */ 
    RCU_PLL = (16U | (336U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_IRC16M) | (7U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 168 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_168M_PLL_8M_HXTAL)
/*!
    \brief      configure the system clock to 168M by PLL which selects HXTAL(8M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_168m_8m_hxtal(void)
{
    uint32_t timeout = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    while((0U == (RCU_CTL & RCU_CTL_HXTALSTB)) && (HXTAL_STARTUP_TIMEOUT != timeout++)){
    }

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }

    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;
    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 8, PLL_N = 336, PLL_P = 2, PLL_Q = 7 */ 
    RCU_PLL = (8U | (336 << 6U) | (((2 >> 1U) -1U) << 16U) |
                   (RCU_PLLSRC_HXTAL) | (7 << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
  
    /* Enable the high-drive to extend the clock frequency to 168 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    }

    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_168M_PLL_25M_HXTAL)
/*!
    \brief      configure the system clock to 168M by PLL which selects HXTAL(25M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_168m_25m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 25, PLL_N = 336, PLL_P = 2, PLL_Q = 7 */ 
    RCU_PLL = (25U | (336U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_HXTAL) | (7U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 168 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_200M_PLL_IRC16M)
/*!
    \brief      configure the system clock to 200M by PLL which selects IRC16M as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_200m_irc16m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable IRC16M */
    RCU_CTL |= RCU_CTL_IRC16MEN;

    /* wait until IRC16M is stable or the startup time is longer than IRC16M_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_IRC16MSTB);
    }while((0U == stab_flag) && (IRC16M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_IRC16MSTB)){
        while(1){
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* IRC16M is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 16, PLL_N = 400, PLL_P = 2, PLL_Q = 9 */ 
    RCU_PLL = (16U | (400U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_IRC16M) | (9U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 200 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_200M_PLL_8M_HXTAL)
/*!
    \brief      configure the system clock to 200M by PLL which selects HXTAL(8M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_200m_8m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 8, PLL_N = 400, PLL_P = 2, PLL_Q = 9 */ 
    RCU_PLL = (8U | (400U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_HXTAL) | (9U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 200 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_200M_PLL_25M_HXTAL)
/*!
    \brief      configure the system clock to 200M by PLL which selects HXTAL(25M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_200m_25m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 25, PLL_N = 400, PLL_P = 2, PLL_Q = 9 */ 
    RCU_PLL = (25U | (400U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_HXTAL) | (9U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 200 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_240M_PLL_IRC16M)
/*!
    \brief      configure the system clock to 240M by PLL which selects IRC16M as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_240m_irc16m(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable IRC16M */
    RCU_CTL |= RCU_CTL_IRC16MEN;

    /* wait until IRC16M is stable or the startup time is longer than IRC16M_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_IRC16MSTB);
    }while((0U == stab_flag) && (IRC16M_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_IRC16MSTB)){
        while(1){
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* IRC16M is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 16, PLL_N = 480, PLL_P = 2, PLL_Q = 10 */ 
    RCU_PLL = (16U | (480U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_IRC16M) | (10U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 240 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_240M_PLL_8M_HXTAL)
/*!
    \brief      configure the system clock to 240M by PLL which selects HXTAL(8M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_240m_8m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 8, PLL_N = 480, PLL_P = 2, PLL_Q = 10 */ 
    RCU_PLL = (8U | (480U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_HXTAL) | (10U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 240 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}

#elif defined (__SYSTEM_CLOCK_240M_PLL_25M_HXTAL)
/*!
    \brief      configure the system clock to 240M by PLL which selects HXTAL(25M) as its clock source
    \param[in]  none
    \param[out] none
    \retval     none
*/
static void system_clock_240m_25m_hxtal(void)
{
    uint32_t timeout = 0U;
    uint32_t stab_flag = 0U;
    
    /* enable HXTAL */
    RCU_CTL |= RCU_CTL_HXTALEN;

    /* wait until HXTAL is stable or the startup time is longer than HXTAL_STARTUP_TIMEOUT */
    do{
        timeout++;
        stab_flag = (RCU_CTL & RCU_CTL_HXTALSTB);
    }while((0U == stab_flag) && (HXTAL_STARTUP_TIMEOUT != timeout));

    /* if fail */
    if(0U == (RCU_CTL & RCU_CTL_HXTALSTB)){
        while(0U == (RCU_CTL & RCU_CTL_HXTALSTB))
        {
        }
    }
         
    RCU_APB1EN |= RCU_APB1EN_PMUEN;
    PMU_CTL |= PMU_CTL_LDOVS;

    /* HXTAL is stable */
    /* AHB = SYSCLK */
    RCU_CFG0 |= RCU_AHB_CKSYS_DIV1;
    /* APB2 = AHB/2 */
    RCU_CFG0 |= RCU_APB2_CKAHB_DIV2;
    /* APB1 = AHB/4 */
    RCU_CFG0 |= RCU_APB1_CKAHB_DIV4;

    /* Configure the main PLL, PSC = 25, PLL_N = 480, PLL_P = 2, PLL_Q = 10 */ 
    RCU_PLL = (25U | (480U << 6U) | (((2U >> 1U) - 1U) << 16U) |
                   (RCU_PLLSRC_HXTAL) | (10U << 24U));

    /* enable PLL */
    RCU_CTL |= RCU_CTL_PLLEN;

    /* wait until PLL is stable */
    while(0U == (RCU_CTL & RCU_CTL_PLLSTB)){
    }
    
    /* Enable the high-drive to extend the clock frequency to 240 Mhz */
    PMU_CTL |= PMU_CTL_HDEN;
    while(0U == (PMU_CS & PMU_CS_HDRF)){
    }
    
    /* select the high-drive mode */
    PMU_CTL |= PMU_CTL_HDS;
    while(0U == (PMU_CS & PMU_CS_HDSRF)){
    } 
    
    /* select PLL as system clock */
    RCU_CFG0 &= ~RCU_CFG0_SCS;
    RCU_CFG0 |= RCU_CKSYSSRC_PLLP;

    /* wait until PLL is selected as system clock */
    while(0U == (RCU_CFG0 & RCU_SCSS_PLLP)){
    }
}
#endif /* __SYSTEM_CLOCK_IRC16M */
/*!
    \brief      update the SystemCoreClock with current core clock retrieved from cpu registers
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SystemCoreClockUpdate(void)
{
    uint32_t sws;
    uint32_t pllpsc, plln, pllsel, pllp, ck_src, idx, clk_exp;
    
    /* exponent of AHB, APB1 and APB2 clock divider */
    const uint8_t ahb_exp[16] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};

    sws = GET_BITS(RCU_CFG0, 2, 3);
    switch(sws){
    /* IRC16M is selected as CK_SYS */
    case SEL_IRC16M:
        SystemCoreClock = IRC16M_VALUE;
        break;
    /* HXTAL is selected as CK_SYS */
    case SEL_HXTAL:
        SystemCoreClock = HXTAL_VALUE;
        break;
    /* PLLP is selected as CK_SYS */
    case SEL_PLLP:
        /* get the value of PLLPSC[5:0] */
        pllpsc = GET_BITS(RCU_PLL, 0U, 5U);
        plln = GET_BITS(RCU_PLL, 6U, 14U);
        pllp = (GET_BITS(RCU_PLL, 16U, 17U) + 1U) * 2U;
        /* PLL clock source selection, HXTAL or IRC8M/2 */
        pllsel = (RCU_PLL & RCU_PLL_PLLSEL);
        if (RCU_PLLSRC_HXTAL == pllsel) {
            ck_src = HXTAL_VALUE;
        } else {
            ck_src = IRC16M_VALUE;
        }
        SystemCoreClock = ((ck_src / pllpsc) * plln) / pllp;
        break;
    /* IRC16M is selected as CK_SYS */
    default:
        SystemCoreClock = IRC16M_VALUE;
        break;
    }
    /* calculate AHB clock frequency */
    idx = GET_BITS(RCU_CFG0, 4, 7);
    clk_exp = ahb_exp[idx];
    SystemCoreClock = SystemCoreClock >> clk_exp;
}
/************************************************
 WKS GD32F427ZGT6