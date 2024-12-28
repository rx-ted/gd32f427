#include "hw.h"
#include "delay.h"
#include "demo.h"
#include "at24cxx.h"
#include "lv_port_disp.h"
#include "lv_port_indev.h"
#include "lv_demos.h"
#include "lvgl.h"
#include "gfx.h"
#include "touch.h"
#include "led.h"
#include "serial.h"

extern touch_dev_t tp;

void hw()
{
    printf("[I]][HW] Hardware init...\n");

    delay_init();
    usart_init(0);

    // at24cxx_init();

    lv_init();
    lv_port_disp_init();
    lv_port_indev_init();
    lv_demo_widgets();
    // lv_demo_stress();
    // lv_demo_benchmark();
    // demo_run();
    while (1)
    {
        lv_task_handler(); // lvgl的事务处理
    }
}
