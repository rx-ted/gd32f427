#ifndef __DELAY_H__
#define __DELAY_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "config.h"
    void delay_init();
    void delay_ms(uint32_t ms);
    void delay_us(uint32_t us);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __DELAY_H__
