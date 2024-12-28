#include "serial.h"
#include "usart.h"
extern UART_HandleTypeDef huart1;

typedef struct
{
    uint8_t id;
    device_state_t is_init;
    USART_TypeDef *serial;
} serial_t;

static serial_t serials[SERIALx] = {
    {0, DEVICE_OFF, USART1},
};

void set_usart_is_init(uint8_t serial, bool state)
{
    serials[serial].is_init = state;
}

device_result_t usart_init(uint8_t serial)
{
    if (serial > SERIALx - 1)
    {
        printf("[E][usart]: Pls check Serial number!\n");
        return DEVICE_EINVAL;
    }
    serial_t s = serials[serial];

    if (s.is_init == DEVICE_OFF)
    {
        set_usart_is_init(serial, DEVICE_ON);
    }
    else
    {
        return DEVICE_EOK;
    }

    switch (s.id)
    {
    case 0:
        MX_USART1_UART_Init();
        break;

    default:
        return DEVICE_NOT_FOUND;
        break;
    }
    printf("[I][usart]: Serial %d init okay!\n", s.id);
    return DEVICE_EOK;
}

#ifdef DEBUG
//! Default debug printf function for USART1
#ifdef __GNUC__
/* retarget the C library printf function to the USART */
int __io_putchar(int ch)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#else
int fputc(int ch, FILE *f)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
#endif

#endif // DEBUG
