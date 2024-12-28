#ifndef __KEY_H__
#define __KEY_H__
#include "config.h"
#ifdef __cplusplus
extern "C"
{
#endif
    void key_scan();

    __weak void onClick();
    __weak void onLongPress();

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __KEY_H__