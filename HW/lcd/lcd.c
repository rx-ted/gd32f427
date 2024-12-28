
#include "lcd.h"
#include "fmc.h"
#include "lcd_ex.h"

typedef struct
{
    uint8_t id;
    uint16_t read_id_cmd;    // lcd register to read id
    uint8_t read_id_length;  // get the length of id from LCD register
    uint16_t read_id;        // get id from LCD register
    void (*register_init)(); // lcd register init
    uint16_t factory_width;  // default width
    uint16_t factory_height; // default height
} lcd_param_t;

static const lcd_param_t lcd_params[] = {
    {0, 0XD3 /*0x7796*/, 5, 0x7796, lcd_ex_st7796_reginit, 0, 0},            // the read id length, i'm not sure
    {1, 0X04 /*0x7789,0x8552*/, 5, 0x7789, lcd_ex_st7789_reginit, 320, 240}, // the read id length, i'm not sure
    {2, 0XD4 /*0x5310*/, 5, 0x5310, lcd_ex_nt35310_reginit, 320, 480},
};

lcd_dev_t lcddev;

static void lcd_opt_delay(uint32_t i)
{
    while (i--)
        ;
}
// LCD写数据
void lcd_wr_data(volatile uint16_t data)
{
    data = data;
    LCD->LCD_RAM = data;
}
// LCD写寄存器编号/地址
void lcd_wr_regno(volatile uint16_t regno)
{
    regno = regno;
    LCD->LCD_REG = regno;
}
// LCD写寄存器的值
void lcd_write_reg(uint16_t regno, uint16_t data)
{
    LCD->LCD_RAM = data;
    LCD->LCD_REG = regno;
}
uint16_t lcd_rd_data(void)
{
    volatile uint16_t ram;
    lcd_opt_delay(2);
    ram = LCD->LCD_RAM;
    return ram;
}

void lcd_set_cursor(uint16_t x, uint16_t y)
{
    lcd_wr_regno(LCD_SETXCMD);
    lcd_wr_data(x >> 8);
    lcd_wr_data(x & 0XFF);
    lcd_wr_regno(LCD_SETYCMD);
    lcd_wr_data(y >> 8);
    lcd_wr_data(y & 0XFF);
}

// 搜索lcd型号所有的可能
device_result_t check_lcd_setup_param_by_id()
{
    uint8_t lcd_param_indexs = sizeof(lcd_params) / sizeof(lcd_params[0]);
    for (uint8_t index = 0; index < lcd_param_indexs; index++)
    {
        lcd_param_t lcd_param = lcd_params[index];
        lcd_wr_regno(lcd_param.read_id_cmd);
        for (uint8_t i = 0; i < lcd_param.read_id_length - 2; i++)
        {
            lcd_rd_data(); // dump, not meaning
        }
        // get the last twos id
        uint16_t id = lcd_rd_data() << 8 | lcd_rd_data();
        if (id == lcd_param.read_id)
        {
            lcddev.id = index;
            lcddev.width = lcd_param.factory_width;
            lcddev.height = lcd_param.factory_height;
            lcd_param.register_init();
            printf("[lcd]: Successfully Initialized, ID: 0x%x\n", id);
            return DEVICE_EOK;
        }
    }
    printf("[E][lcd]: Not found lcd controler!\n");
    return DEVICE_NOT_FOUND;
}

void exmc_sram_init(void)
{
    // 初始化EXMC
    // TODO(ben): in the main.c file, there is a function called MX_FMC_Init. It is used to initialize the FMC. I think we don't need to initialize the FMC here. Because the lcd initialization function and other nor flash initialization functions are the same. So, I think we can initialize the FMC in the main.c file.
}

// 初始化LCD
void lcd_init(void)
{
    exmc_sram_init();       // 初始化EXMC NOR/SRAM配置
    lcd_opt_delay(0X1FFFF); // 初始化EXMC后,等待一定时间才能开始初始化
    LCD_RST(1);
    delay_ms(10);
    LCD_RST(0);
    delay_ms(50);
    LCD_RST(1);
    delay_ms(200);

    if (check_lcd_setup_param_by_id() != DEVICE_EOK)
    {
        return;
    }
    set_lcd_display_direction(LCD_DISP_DIR_0, VERITAL_MODE); // 默认为竖屏
    LCD_BL(1);                                               // 点亮背光
}

void _set_lcd_address_mode()
{
    lcd_write_reg(0X36, lcddev.dir);
    lcd_wr_regno(LCD_SETXCMD);
    lcd_wr_data(0);
    lcd_wr_data(0);
    lcd_wr_data((lcddev.width - 1) >> 8);
    lcd_wr_data((lcddev.width - 1) & 0XFF);
    lcd_wr_regno(LCD_SETYCMD);
    lcd_wr_data(0);
    lcd_wr_data(0);
    lcd_wr_data((lcddev.height - 1) >> 8);
    lcd_wr_data((lcddev.height - 1) & 0XFF);
}

/* 设置屏幕显示方向 */
device_result_t set_lcd_display_direction(lcd_display_dir_t dir, lcd_display_mode_t mode)
{
    device_result_t res = DEVICE_EOK;
    lcd_param_t lcd_param = lcd_params[lcddev.id];

    if (mode == VERITAL_MODE)
    {
        if (dir == LCD_DISP_DIR_0 || dir == LCD_DISP_DIR_180)
        {

            lcddev.width = lcd_param.factory_width;
            lcddev.height = lcd_param.factory_height;
        }
        else if (dir == LCD_DISP_DIR_90 || dir == LCD_DISP_DIR_270)
        {
            lcddev.height = lcd_param.factory_width;
            lcddev.width = lcd_param.factory_height;
        }
        else
            res = DEVICE_EINVAL;
    }
    else if (mode == HORIZONTAL_MODE)
    {
        if (dir == LCD_DISP_DIR_0 || dir == LCD_DISP_DIR_180)
        {
            lcddev.height = lcd_param.factory_width;
            lcddev.width = lcd_param.factory_height;
        }
        else if (dir == LCD_DISP_DIR_90 || dir == LCD_DISP_DIR_270)
        {
            lcddev.width = lcd_param.factory_width;
            lcddev.height = lcd_param.factory_height;
        }
        else
            res = DEVICE_EINVAL;
    }
    else
        res = DEVICE_EINVAL;

    if (res == DEVICE_EOK)
    {
        lcddev.dir = dir;
        lcddev.mode = mode;
        _set_lcd_address_mode();
    }
    return res;
}

void lcd_display_on(void)
{
    lcd_wr_regno(0X29); // 开启显示
}

void lcd_display_off(void)
{
    lcd_wr_regno(0X28); // 关闭显示
}

void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t twidth, theight;
    twidth = sx + width - 1;
    theight = sy + height - 1;

    lcd_wr_regno(LCD_SETXCMD);
    lcd_wr_data(sx >> 8);
    lcd_wr_data(sx & 0XFF);
    lcd_wr_data(twidth >> 8);
    lcd_wr_data(twidth & 0XFF);
    lcd_wr_regno(LCD_SETYCMD);
    lcd_wr_data(sy >> 8);
    lcd_wr_data(sy & 0XFF);
    lcd_wr_data(theight >> 8);
    lcd_wr_data(theight & 0XFF);
}
