# Keyboard Scan

## Introduction

The `key_scan` function detects key presses. Initially, the global variable `count` is set to the current tick. When `key0` is pressed, `count` increments by 1ms. On subsequent presses, `count` continues to increment by 1ms.

To determine the state of `key0`, we check the value of `count`:

- If `count` exceeds `DEBOUNCING=100`, `key0` is confirmed to be pressed.
- There are two scenarios:
    1. If `count` is between `DEBOUNCING=100` and `LONG_PRESS_INTERVAL=1000` and then released, `key0` was pressed.
    2. If `count` exceeds `LONG_PRESS_INTERVAL=1000` without being released and is eventually released, `key0` was long pressed.

The `lock` variable prevents multiple detections of the same press.

Implement the `onClick` and `onLongPress` functions in your code:

- `onClick` is called when `key0` is pressed, toggling `led0`.
- `onLongPress` is called when `key0` is long pressed, toggling `led1`.

## demo

```c
#include <key.h>
#include <led.h>
void onClick()
{
    /** Inplement this in your code **/
    printf("============= onClick\n");
    led_toggle(0);
}
void onLongPress()
{
    /** Inplement this in your code **/
    printf("============= onLongPress\n");
    led_toggle(1);
}
```
