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
