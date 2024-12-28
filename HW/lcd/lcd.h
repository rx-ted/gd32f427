#ifndef __LCD_H
#define __LCD_H

#include "config.h"

// !8080 inferface
#define LCD_8080_BL_Pin LCD_BL_Pin
#define LCD_8080_BL_GPIO_Port LCD_BL_GPIO_Port
#define LCD_8080_RS_Pin LCD_RS_Pin
#define LCD_8080_RS_GPIO_Port LCD_RS_GPIO_Port
#define LCD_8080_RST_Pin LCD_RST_Pin
#define LCD_8080_RST_GPIO_Port LCD_RST_GPIO_Port
#define LCD_8080_RD_Pin LCD_RD_Pin
#define LCD_8080_RD_GPIO_Port LCD_RD_GPIO_Port
#define LCD_8080_WR_Pin LCD_WR_Pin
#define LCD_8080_WR_GPIO_Port LCD_WR_GPIO_Port
#define LCD_8080_CS_Pin LCD_CS_Pin
#define LCD_8080_CS_GPIO_Port LCD_CS_GPIO_Port

/******************************************************************************************/

/* LCD重要参数集 */

// LCD Screen mode
typedef enum
{
    VERITAL_MODE,
    HORIZONTAL_MODE,
} lcd_display_mode_t;

// LCD扫描方向
typedef enum
{

    L2R_U2D = (0 << 7) | (0 << 6) | (0 << 5), // 从左到右,从上到下
    L2R_D2U = (1 << 7) | (0 << 6) | (0 << 5), // 从左到右,从下到上
    R2L_U2D = (0 << 7) | (1 << 6) | (0 << 5), // 从右到左,从上到下
    R2L_D2U = (1 << 7) | (1 << 6) | (0 << 5), // 从右到左,从下到上
    U2D_L2R = (0 << 7) | (0 << 6) | (1 << 5), // 从上到下,从左到右
    U2D_R2L = (0 << 7) | (1 << 6) | (1 << 5), // 从上到下,从右到左
    D2U_L2R = (1 << 7) | (0 << 6) | (1 << 5), // 从下到上,从左到右
    D2U_R2L = (1 << 7) | (1 << 6) | (1 << 5), // 从下到上,从右到左
} lcd_scan_dir_t;

/* LCD旋转方向枚举 */
typedef enum
{
    LCD_DISP_DIR_0 = L2R_U2D,   /* LCD顺时针旋转0°显示内容 */
    LCD_DISP_DIR_90 = U2D_R2L,  /* LCD顺时针旋转90°显示内容 */
    LCD_DISP_DIR_180 = R2L_D2U, /* LCD顺时针旋转180°显示内容 */
    LCD_DISP_DIR_270 = D2U_L2R, /* LCD顺时针旋转270°显示内容 */
} lcd_display_dir_t;

typedef struct
{
    uint16_t id;             // LCD ID
    uint16_t width;          // LCD 宽度
    uint16_t height;         // LCD 高度
    lcd_display_dir_t dir;   // LCD显示方向
    lcd_display_mode_t mode; // lcd 竖屏或者横屏
    uint16_t wramcmd;        // 开始写gram指令
    uint16_t setxcmd;        // 设置x坐标指令
    uint16_t setycmd;        // 设置y坐标指令

    // write
    void (*write)(uint16_t *fb);
    // read
    // set position
    void (*set_pos)(uint16_t x, uint16_t y);
    // set window

    uint16_t backcolor;  // background color
    uint16_t frontcolor; // front color
} lcd_dev_t;

extern lcd_dev_t lcddev;

/* LCD背光控制 */
#define LCD_BL(x)                                                                                                                              \
    do                                                                                                                                         \
    {                                                                                                                                          \
        x ? HAL_GPIO_WritePin(LCD_8080_BL_GPIO_Port, LCD_8080_BL_Pin, SET) : HAL_GPIO_WritePin(LCD_8080_BL_GPIO_Port, LCD_8080_BL_Pin, RESET); \
    } while (0)

/* LCD复位引脚 */
#define LCD_RST(x)                                                                                                                                 \
    do                                                                                                                                             \
    {                                                                                                                                              \
        x ? HAL_GPIO_WritePin(LCD_8080_RST_GPIO_Port, LCD_8080_RST_Pin, SET) : HAL_GPIO_WritePin(LCD_8080_RST_GPIO_Port, LCD_8080_RST_Pin, RESET); \
    } while (0)

/* LCD地址结构体 */
typedef struct
{
    volatile uint16_t LCD_REG;
    volatile uint16_t LCD_RAM;
} LCD_TypeDef;

#define LCD_EXMC_NEX 3 /* 使用EXMC_NE3接LCD_CS,取值范围只能是: 0~3 */
#define LCD_EXMC_AX 6  /* 使用EXMC_A6接LCD_RS,取值范围是: 0 ~ 25 */

/* LCD_BASE的详细计算方法:
 * 我们一般使用EXMC的块0(BANK0)来驱动TFTLCD液晶屏(MCU屏), 块0地址范围总大小为256MB,均分成4块:
 * 存储块0(EXMC_NE0)地址范围: 0X6000 0000 ~ 0X63FF FFFF
 * 存储块1(EXMC_NE1)地址范围: 0X6400 0000 ~ 0X67FF FFFF
 * 存储块2(EXMC_NE2)地址范围: 0X6800 0000 ~ 0X6BFF FFFF
 * 存储块3(EXMC_NE3)地址范围: 0X6C00 0000 ~ 0X6FFF FFFF
 *
 * 我们需要根据硬件连接方式选择合适的片选(连接LCD_CS)和地址线(连接LCD_RS)
 * 开发板使用EXMC_NE3连接LCD_CS, EXMC_A6连接LCD_RS ,16位数据线,计算方法如下:
 * 首先EXMC_NE3的基地址为: 0X6C00 0000;     NEx的基址为(x=0/1/2/3): 0X6000 0000 + (0X400 0000 * x)
 * EXMC_A6对应地址值: 2^6 * 2 = 0X80;       EXMC_Ay对应的地址为(y = 0 ~ 25): 2^y * 2
 *
 * LCD->LCD_REG,对应LCD_RS = 0(LCD寄存器); LCD->LCD_RAM,对应LCD_RS = 1(LCD数据)
 * 则 LCD->LCD_RAM的地址为:  0X6C00 0000 + 2^6 * 2 = 0X6C00 0080
 *    LCD->LCD_REG的地址可以为 LCD->LCD_RAM之外的任意地址.
 * 由于我们使用结构体管理LCD_REG 和 LCD_RAM(REG在前,RAM在后,均为16位数据宽度)
 * 因此 结构体的基地址(LCD_BASE) = LCD_RAM - 2 = 0X6C00 0080 -2
 *
 * 更加通用的计算公式为((片选脚EXMC_NEx)x=0/1/2/3, (RS接地址线EXMC_Ay)y=0~25):
 *          LCD_BASE = (0X6000 0000 + (0X400 0000 * x)) | (2^y * 2 -2)
 *          等效于(使用移位操作)
 *          LCD_BASE = (0X6000 0000 + (0X400 0000 * x)) | ((1 << y) * 2 -2)
 */
#define LCD_BASE (uint32_t)((0X60000000 + (0X4000000 * LCD_EXMC_NEX)) | (((1 << LCD_EXMC_AX) * 2) - 2))
#define LCD ((LCD_TypeDef *)LCD_BASE)

/* 常用画笔颜色 */
#define WHITE 0xFFFF   // 白色
#define BLACK 0x0000   // 黑色
#define RED 0xF800     // 红色
#define GREEN 0x07E0   // 绿色
#define BLUE 0x001F    // 蓝色
#define MAGENTA 0XF81F // 品红色/紫红色 = BLUE + RED
#define YELLOW 0XFFE0  // 黄色 = GREEN + RED
#define CYAN 0X07FF    // 青色 = GREEN + BLUE

/* 非常用颜色 */
#define BROWN 0XBC40      // 棕色
#define BRRED 0XFC07      // 棕红色
#define GRAY 0X8430       // 灰色
#define DARKBLUE 0X01CF   // 深蓝色
#define LIGHTBLUE 0X7D7C  // 浅蓝色
#define GRAYBLUE 0X5458   // 灰蓝色
#define LIGHTGREEN 0X841F // 浅绿色
#define LGRAY 0XC618      // 浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE 0XA651  // 浅灰蓝色(中间层颜色)
#define LBBLUE 0X2B12     // 浅棕蓝色(选择条目的反色)

#define LCD_WRITE_RAM_CMD 0x2c
#define LCD_SETXCMD 0x2a
#define LCD_SETYCMD 0x2b
#define LCD_SET_ADDRESS 0X36

void lcd_wr_data(volatile uint16_t data);          // LCD写数据
void lcd_wr_regno(volatile uint16_t regno);        // LCD写寄存器编号/地址
void lcd_write_reg(uint16_t regno, uint16_t data); // LCD写寄存器的值
uint16_t lcd_rd_data(void);

void lcd_init(void);        /* 初始化LCD */
void lcd_display_on(void);  /* 开显示 */
void lcd_display_off(void); /* 关显示 */

void lcd_set_cursor(uint16_t x, uint16_t y);

device_result_t set_lcd_display_direction(lcd_display_dir_t dir, lcd_display_mode_t mode); /* 设置屏幕显示方向 */

void lcd_set_window(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height); /* 设置窗口 */

#endif
