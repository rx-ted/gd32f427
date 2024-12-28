/*
The key_scan function is used to detect key presses.
At initialization, the global variable `count` is set to get current tick.
When key0 is pressed down, `count` increments by 1ms.
On subsequent presses, `count` continues to increment by 1ms.
We need to check the value of `count`:
If `count` exceeds `DEBOUNCING=100`, we can confirm that key0 is pressed down.
There are two scenarios:
1. If `count` is between `DEBOUNCING=100` and `LONG_PRESS_INTERVAL=1000` and then released, we confirm that key0 was pressed down.
2. If `count` exceeds `LONG_PRESS_INTERVAL=1000` without being released, and then is eventually released, we confirm that key0 was long pressed.
The `lock` variable is used to prevent multiple presses from being detected.
immplement the `onClick` and `onLongPress` functions in your code.
- `onClick` is called when key0 is pressed down, and led0 toggles.
- `onLongPress` is called when key0 is long pressed, and led1 toggles.
*/

#include "key.h"

#define KEY HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)

#define DEBOUNCING 100           // debouncing delay (ms)
#define LONG_PRESS_INTERVAL 1000 // long press delay (ms)

// Please see the GPIO in the main function
typedef enum
{
    KEY_PRESSED = GPIO_PIN_SET,
    KEY_RELEASED = GPIO_PIN_RESET,
};

uint32_t count;
uint8_t lock = 0; // lock the key

void key_scan()
{
    uint8_t read = KEY;

    if (lock == 0 && read == KEY_PRESSED)
    {
        lock = 1; // lock, wait for release
        count = get_hw_count();
    }
    else if (lock == 1 && read == KEY_RELEASED)
    { // when lock=1
        uint32_t tmp_count = get_hw_count();
        if (tmp_count - count < DEBOUNCING)
        {
            return;
        }
        else if (tmp_count - count < LONG_PRESS_INTERVAL)
        {
            onClick();
        }
        else
        {
            onLongPress();
        }
        lock = 0; // release lock
    }
}

__weak void onClick()
{
    /** Inplement this in your code **/
    printf("============= onClick\n");
    led_toggle(0);
}
__weak void onLongPress()
{
    /** Inplement this in your code **/
    printf("============= onLongPress\n");
    led_toggle(1);
}
