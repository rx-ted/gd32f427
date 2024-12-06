/************************************************
 * ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
 * ����delay_us,delay_ms
 ************************************************/

#include "sys.h"
#include "delay.h"

static uint16_t g_fac_us = 0; /* us��ʱ������ */

/**
 * @brief       ��ʼ���ӳٺ���
 * @param       sysclk: ϵͳʱ��Ƶ��, ��CPUƵ��(HCLK)
 * @retval      ��
 */
void delay_init(uint16_t sysclk)
{
    SysTick->CTRL = 0;                        /* ��Systick״̬���Ա���һ�����裬������￪���жϻ�ر����ж� */
    SysTick->CTRL &= ~(1 << 2);               /* SYSTICKʹ���ں�ʱ��Դ8��Ƶ,��systick�ļ��������ֵֻ��2^24 */
    g_fac_us = sysclk / 8;                    /* �����Ƿ�ʹ��OS,g_fac_us����Ҫʹ��,��Ϊ1us�Ļ���ʱ�� */
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; /* ����SYSTICK */
    SysTick->LOAD = 0XFFFFFF;                 /* ע��systick������24λ�������������������װ��ֵ */
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
}

/**
 * @brief       ��ʱnms
 * @param       nms: Ҫ��ʱ��ms�� (0< nms <= (2^32 / g_fac_us / 1000))(g_fac_usһ�����ϵͳ��Ƶ/8, �����������)
 * @retval      ��
 */
void delay_ms(uint16_t nms)
{
    delay_us((uint32_t)(nms * 1000)); /* ��ͨ��ʽ��ʱ */
}
