#ifndef __SYS_H
#define __SYS_H

#include "gd32f4xx.h"

/**
 * SYS_SUPPORT_OS���ڶ���ϵͳ�ļ����Ƿ�֧��OS
 * 0,��֧��OS
 * 1,֧��OS
 */
#define SYS_SUPPORT_OS 0

/* ��ͨ���� */
void sys_soft_reset(void); /* ϵͳ��λ */

/* ����Ϊ��ຯ�� */
void sys_wfi_set(void);          /* ִ��WFIָ�� */
void sys_intx_disable(void);     /* �ر������ж� */
void sys_intx_enable(void);      /* ���������ж� */
void sys_msr_msp(uint32_t addr); /* ����ջ����ַ */

#endif
