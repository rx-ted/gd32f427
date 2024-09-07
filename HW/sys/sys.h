#ifndef __SYS_H
#define __SYS_H

#include "gd32f4xx.h"


/**
 * SYS_SUPPORT_OS用于定义系统文件夹是否支持OS
 * 0,不支持OS
 * 1,支持OS
 */
#define SYS_SUPPORT_OS          0


/* 普通函数 */
void sys_soft_reset(void);                                                      /* 系统软复位 */

/* 以下为汇编函数 */
void sys_wfi_set(void);                                                         /* 执行WFI指令 */
void sys_intx_disable(void);                                                    /* 关闭所有中断 */
void sys_intx_enable(void);                                                     /* 开启所有中断 */
void sys_msr_msp(uint32_t addr);                                                /* 设置栈顶地址 */

#endif











