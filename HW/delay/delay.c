/************************************************
 * ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
 * ����delay_us,delay_ms
 ************************************************/

#include "sys.h"
#include "delay.h"

static uint16_t g_fac_us = 0; /* us��ʱ������ */

/* ���SYS_SUPPORT_OS������,˵��Ҫ֧��OS��(������UCOS) */
#if SYS_SUPPORT_OS

/* ��ӹ���ͷ�ļ� ( ucos��Ҫ�õ�) */
#include "includes.h"

/* ����g_fac_ms����, ��ʾms��ʱ�ı�����, ����ÿ�����ĵ�ms��, (����ʹ��os��ʱ��,��Ҫ�õ�) */
static uint16_t g_fac_ms = 0;

/*
 *  ��delay_us/delay_ms��Ҫ֧��OS��ʱ����Ҫ������OS��صĺ궨��ͺ�����֧��
 *  ������3���궨��:
 *      delay_osrunning    :���ڱ�ʾOS��ǰ�Ƿ���������,�Ծ����Ƿ����ʹ����غ���
 *      delay_ostickspersec:���ڱ�ʾOS�趨��ʱ�ӽ���,delay_init�����������������ʼ��systick
 *      delay_osintnesting :���ڱ�ʾOS�ж�Ƕ�׼���,��Ϊ�ж����治���Ե���,delay_msʹ�øò����������������
 *  Ȼ����3������:
 *      delay_osschedlock  :��������OS�������,��ֹ����
 *      delay_osschedunlock:���ڽ���OS�������,���¿�������
 *      delay_ostimedly    :����OS��ʱ,���������������.
 *
 *  �����̽���UCOSII��֧��,����OS,�����вο�����ֲ
 */

/* ֧��UCOSII */
#define delay_osrunning OSRunning            /* OS�Ƿ����б��,0,������;1,������ */
#define delay_ostickspersec OS_TICKS_PER_SEC /* OSʱ�ӽ���,��ÿ����ȴ��� */
#define delay_osintnesting OSIntNesting      /* �ж�Ƕ�׼���,���ж�Ƕ�״��� */

/**
 * @brief       us����ʱʱ,�ر��������(��ֹ���us���ӳ�)
 * @param       ��
 * @retval      ��
 */
static void delay_osschedlock(void)
{
    OSSchedLock(); /* UCOSII�ķ�ʽ,��ֹ���ȣ���ֹ���us��ʱ */
}

/**
 * @brief       us����ʱʱ,�ָ��������
 * @param       ��
 * @retval      ��
 */
static void delay_osschedunlock(void)
{
    OSSchedUnlock(); /* UCOSII�ķ�ʽ,�ָ����� */
}

/**
 * @brief       us����ʱʱ,�ָ��������
 * @param       ticks: ��ʱ�Ľ�����
 * @retval      ��
 */
static void delay_ostimedly(uint32_t ticks)
{
    OSTimeDly(ticks); /* UCOSII��ʱ */
}

/**
 * @brief       systick�жϷ�����,ʹ��OSʱ�õ�
 * @param       ticks: ��ʱ�Ľ�����
 * @retval      ��
 */
void SysTick_Handler(void)
{
    if (delay_osrunning == 1) /* OS��ʼ����,��ִ�������ĵ��ȴ��� */
    {
        OSIntEnter(); /* �����ж� */
        OSTimeTick(); /* ����ucos��ʱ�ӷ������ */
        OSIntExit();  /* ���������л����ж� */
    }
}

#endif

/**
 * @brief       ��ʼ���ӳٺ���
 * @param       sysclk: ϵͳʱ��Ƶ��, ��CPUƵ��(HCLK)
 * @retval      ��
 */
void delay_init(uint16_t sysclk)
{
#if SYS_SUPPORT_OS /* �����Ҫ֧��OS. */
    uint32_t reload;
#endif

    SysTick->CTRL = 0;                         /* ��Systick״̬���Ա���һ�����裬������￪���жϻ�ر����ж� */
    SysTick->CTRL &= ~(1 << 2);                /* SYSTICKʹ���ں�ʱ��Դ8��Ƶ,��systick�ļ��������ֵֻ��2^24 */
    g_fac_us = sysclk / 8;                     /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ��,��Ϊ1us�Ļ���ʱ�� */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;  /* ����SYSTICK */
    SysTick->LOAD = 0XFFFFFF;                  /* ע��systick������24λ�������������������װ��ֵ */
#if SYS_SUPPORT_OS                             /* �����Ҫ֧��OS. */
    reload = sysclk / 8;                       /* ÿ���ӵļ������� ��λΪM */
    reload *= 1000000 / delay_ostickspersec;   /* ����delay_ostickspersec�趨���ʱ��
                                                * reloadΪ24λ�Ĵ���,���ֵ:16777216,��200M��,Լ��0.64s����
                                                */
    g_fac_ms = 1000 / delay_ostickspersec;     /* ����OS������ʱ�����ٵ�λ */
    SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; /* ����SYSTICK�ж� */
    SysTick->LOAD = reload;                    /* ÿ1/delay_ostickspersec���ж�һ�� */

#endif
}

/**
 * @brief       ��ʱnus
 * @note        �����Ƿ�ʹ��OS, ������ʱ��ժȡ������us��ʱ
 * @param       nus: Ҫ��ʱ��us��.
 * @note        nusȡֵ��Χ: 0 ~ (2^32 / g_fac_us) (g_fac_usһ�����ϵͳ��Ƶ/8, �����������)
 * @retval      ��
 */
void delay_us(uint32_t nus)
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload;
    reload = SysTick->LOAD; /* LOAD��ֵ */
    ticks = nus * g_fac_us; /* ��Ҫ�Ľ����� */

#if SYS_SUPPORT_OS       /* �����Ҫ֧��OS */
    delay_osschedlock(); /* ��ֹOS���ȣ���ֹ���us��ʱ */
#endif

    told = SysTick->VAL; /* �ս���ʱ�ļ�����ֵ */
    while (1)
    {
        tnow = SysTick->VAL;

        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow; /* ����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����. */
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks)
            {
                break; /* ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�. */
            }
        }
    }

#if SYS_SUPPORT_OS         /* �����Ҫ֧��OS */
    delay_osschedunlock(); /* �ָ�OS���� */
#endif
}

/**
 * @brief       ��ʱnms
 * @param       nms: Ҫ��ʱ��ms�� (0< nms <= (2^32 / g_fac_us / 1000))(g_fac_usһ�����ϵͳ��Ƶ/8, �����������)
 * @retval      ��
 */
void delay_ms(uint16_t nms)
{

#if SYS_SUPPORT_OS                                  /* �����Ҫ֧��OS, ������������os��ʱ���ͷ�CPU */
    if (delay_osrunning && delay_osintnesting == 0) /* ���OS�Ѿ�������,���Ҳ������ж�����(�ж����治���������) */
    {
        if (nms >= g_fac_ms) /* ��ʱ��ʱ�����OS������ʱ������ */
        {
            delay_ostimedly(nms / g_fac_ms); /* OS��ʱ */
        }

        nms %= g_fac_ms; /* OS�Ѿ��޷��ṩ��ôС����ʱ��,������ͨ��ʽ��ʱ */
    }
#endif

    delay_us((uint32_t)(nms * 1000)); /* ��ͨ��ʽ��ʱ */
}
