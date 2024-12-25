#include "hw.h"
#include "delay.h"
#include "demo.h"
#include "at24cxx.h"
#include "lv_port_disp.h"
#include "lv_demos.h"
#include "lvgl.h"
#include "gfx.h"

void hw()
{
    // led init
    delay_init();

    gd_eval_led_init(0);
    gd_eval_led_init(1);
    gd_eval_com_init(0);

    // at24cxx_init();

    lv_init();
    lv_port_disp_init();
    // lv_demo_widgets();
    // lv_demo_stress();
    lv_demo_benchmark();

    // gfx_init();
    // fill_clear(0, 0, 200, 200, 0x0000);

    while (1)
    {
        lv_timer_handler();
    }

    // key init
    // serial init
}
