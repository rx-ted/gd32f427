#include "gfx.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "gfx_font.h"
#include "lcd.h"

gfx_param_t gfx_param;

#define GFX_WIDTH gfx_param.width
#define GFX_HEIGHT gfx_param.height

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) \
    {                       \
        int16_t t = a;      \
        a = b;              \
        b = t;              \
    }
#endif

void gfx_init()
{
    lcd_init();
    gfx_param.width = lcddev.width;
    gfx_param.height = lcddev.height;
    fill_clear(0, 0, GFX_WIDTH, GFX_HEIGHT, WHITE);
}

void check_buffer_valid(uint16_t *x, uint16_t *y)
{

    if (*x >= GFX_WIDTH)
    {
        *x = GFX_WIDTH - 1;
    }
    else if (*x < 0)
    {
        *x = 0;
    }

    if (*y >= GFX_HEIGHT)
    {
        *y = GFX_HEIGHT - 1;
    }
    else if (*y < 0)
    {
        *y = 0;
    }
}

//! draw function
void draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_set_cursor(x, y);
    lcd_wr_regno(LCD_WRITE_RAM_CMD);
    lcd_wr_data(color);
}

void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    check_buffer_valid(&x1, &y1);
    check_buffer_valid(&x2, &y2);
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    delta_x = x2 - x1; // 计算坐标增量
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
        incx = 1; // 设置单步方向
    else if (delta_x == 0)
        incx = 0; // 垂直线
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
        incy = 1;
    else if (delta_y == 0)
        incy = 0; // 水平线
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x; // 选取基本增量坐标轴
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++) // 画线输出
    {
        draw_point(row, col, color); // 画点
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

void draw_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    draw_line(x1, y1, x2, y1, color);
    draw_line(x1, y2, x2, y2, color);
    draw_line(x1, y1, x1, y2, color);
    draw_line(x2, y1, x2, y2, color);
}

void draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    check_buffer_valid(&x, &y);

    int32_t x_t;
    int32_t y_t;
    int32_t error;
    int32_t error2;

    x_t = -r;
    y_t = 0;
    error = 2 - 2 * r;

    do
    {
        draw_point(x - x_t, y + y_t, color);
        draw_point(x + x_t, y + y_t, color);
        draw_point(x + x_t, y - y_t, color);
        draw_point(x - x_t, y - y_t, color);

        error2 = error;
        if (error2 <= y_t)
        {
            y_t++;
            error = error + (y_t * 2 + 1);
            if ((-x_t == y_t) && (error2 <= x_t))
            {
                error2 = 0;
            }
        }

        if (error2 > x_t)
        {
            x_t++;
            error = error + (x_t * 2 + 1);
        }
    } while (x_t <= 0);
}

void draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                   int16_t y2, uint16_t color)
{
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x0, y0, x2, y2, color);
}

//! fill function
void fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color)
{
    check_buffer_valid(&x1, &y1);
    check_buffer_valid(&x2, &y2);
    uint16_t height, width;
    uint16_t i, j;
    width = x2 - x1 + 1;  /* 得到填充的宽度 */
    height = y2 - y1 + 1; /* 高度 */

    for (i = 0; i < height; i++)
    {
        lcd_set_cursor(x1, y1 + i); /* 设置光标位置 */
        lcd_wr_regno(LCD_WRITE_RAM_CMD);
        for (j = 0; j < width; j++)
        {
            lcd_wr_data(color[i * width + j]);
        }
    }
}

void fill_clear(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    check_buffer_valid(&x1, &y1);
    check_buffer_valid(&x2, &y2);
    uint16_t height, width;
    uint16_t i, j;
    width = x2 - x1 + 1;
    height = y2 - y1 + 1;

    for (i = 0; i < height; i++)
    {
        lcd_set_cursor(x1, y1 + i);
        lcd_wr_regno(LCD_WRITE_RAM_CMD);
        for (j = 0; j < width; j++)
        {
            lcd_wr_data(color);
        }
    }
}

void lcd_draw_hline(uint16_t x, uint16_t y, uint16_t len, uint16_t color)
{
    if ((len == 0) || (x > GFX_WIDTH) || (y > GFX_HEIGHT))
        return;

    fill_rect(x, y, x + len - 1, y, color);
}

void fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    check_buffer_valid(&x, &y);
    uint32_t i;
    uint32_t imax = ((uint32_t)r * 707) / 1000 + 1;
    uint32_t sqmax = (uint32_t)r * (uint32_t)r + (uint32_t)r / 2;
    uint32_t xr = r;

    lcd_draw_hline(x - r, y, 2 * r, color);

    for (i = 1; i <= imax; i++)
    {
        if ((i * i + xr * xr) > sqmax)
        {
            /* draw lines from outside */
            if (xr > imax)
            {
                lcd_draw_hline(x - i + 1, y + xr, 2 * (i - 1), color);
                lcd_draw_hline(x - i + 1, y - xr, 2 * (i - 1), color);
            }

            xr--;
        }

        /* draw lines from inside (center) */
        lcd_draw_hline(x - xr, y + i, 2 * xr, color);
        lcd_draw_hline(x - xr, y - i, 2 * xr, color);
    }
}

#if (GFX_USE_FONT == 1)
void show_char(uint16_t x, uint16_t y, char chr, gfx_font_t font, uint16_t color)
{
    const uint8_t *ch_code;
    uint8_t ch_width;
    uint8_t ch_height;
    uint8_t ch_size;
    uint8_t ch_offset;
    uint8_t byte_index;
    uint8_t byte_code;
    uint8_t bit_index;
    uint8_t width_index = 0;
    uint8_t height_index = 0;
    ch_offset = chr - ' ';
    switch (font)
    {
#if (GFX_USE_FONT_12 != 0)
    case GFX_FONT_12:
    {
        ch_code = gfx_font_1206[ch_offset];
        ch_width = GFX_FONT_12_CHAR_WIDTH;
        ch_height = GFX_FONT_12_CHAR_HEIGHT;
        ch_size = GFX_FONT_12_CHAR_SIZE;
        break;
    }
#endif
#if (GFX_USE_FONT_16 != 0)
    case GFX_FONT_16:
    {
        ch_code = gfx_font_1608[ch_offset];
        ch_width = GFX_FONT_16_CHAR_WIDTH;
        ch_height = GFX_FONT_16_CHAR_HEIGHT;
        ch_size = GFX_FONT_16_CHAR_SIZE;
        break;
    }
#endif
#if (GFX_USE_FONT_24 != 0)
    case GFX_FONT_24:
    {
        ch_code = gfx_font_2412[ch_offset];
        ch_width = GFX_FONT_24_CHAR_WIDTH;
        ch_height = GFX_FONT_24_CHAR_HEIGHT;
        ch_size = GFX_FONT_24_CHAR_SIZE;
        break;
    }
#endif
#if (GFX_USE_FONT_32 != 0)
    case GFX_FONT_32:
    {
        ch_code = gfx_font_3216[ch_offset];
        ch_width = GFX_FONT_32_CHAR_WIDTH;
        ch_height = GFX_FONT_32_CHAR_HEIGHT;
        ch_size = GFX_FONT_32_CHAR_SIZE;
        break;
    }
#endif
    default:
    {
        return;
    }
    }

    if ((x + ch_width > GFX_WIDTH) || (y + ch_height > GFX_HEIGHT))
    {
        return;
    }

    for (byte_index = 0; byte_index < ch_size; byte_index++)
    {
        byte_code = ch_code[byte_index];
        for (bit_index = 0; bit_index < 8; bit_index++)
        {
            if ((byte_code & 0x80) != 0)
            {
                draw_point(x + width_index, y + height_index, color);
            }
            height_index++;
            if (height_index == ch_height)
            {
                height_index = 0;
                width_index++;
                break;
            }
            byte_code <<= 1;
        }
    }
}

void show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, gfx_font_t font, uint16_t color)
{
    show_xnum(x, y, num, len, font, GFX_NUM_SHOW_ZERO, color);
}

static uint32_t gfx_pow(uint8_t x, uint8_t y)
{
    uint8_t loop;
    uint32_t res = 1;

    for (loop = 0; loop < y; loop++)
    {
        res *= x;
    }

    return res;
}

void show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, gfx_font_t font, gfx_num_mode_t mode, uint16_t color)
{
    uint8_t ch_width;
    uint8_t len_index;
    uint8_t num_index;
    uint8_t first_nozero = 0;
    char pad;

    switch (font)
    {
#if (GFX_USE_FONT_12 != 0)
    case GFX_FONT_12:
    {
        ch_width = GFX_FONT_12_CHAR_WIDTH;
        break;
    }
#endif
#if (GFX_USE_FONT_16 != 0)
    case GFX_FONT_16:
    {
        ch_width = GFX_FONT_16_CHAR_WIDTH;
        break;
    }
#endif
#if (GFX_USE_FONT_24 != 0)
    case GFX_FONT_24:
    {
        ch_width = GFX_FONT_24_CHAR_WIDTH;
        break;
    }
#endif
#if (GFX_USE_FONT_32 != 0)
    case GFX_FONT_32:
    {
        ch_width = GFX_FONT_32_CHAR_WIDTH;
        break;
    }
#endif
    default:
    {
        return;
    }
    }

    switch (mode)
    {
    case GFX_NUM_SHOW_NOZERO:
    {
        pad = ' ';
        break;
    }
    case GFX_NUM_SHOW_ZERO:
    {
        pad = '0';
        break;
    }
    default:
    {
        return;
    }
    }

    for (len_index = 0; len_index < len; len_index++)
    {
        num_index = (num / gfx_pow(10, len - len_index - 1)) % 10;
        if ((first_nozero == 0) && (len_index < (len - 1)))
        {
            if (num_index == 0)
            {
                show_char(x + ch_width * len_index, y, pad, font, color);
                continue;
            }
            else
            {
                first_nozero = 1;
            }
        }

        show_char(x + ch_width * len_index, y, num_index + '0', font, color);
    }
}

void show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *str, gfx_font_t font, uint16_t color)
{
    uint8_t ch_width;
    uint8_t ch_height;
    uint16_t x_raw;
    uint16_t y_raw;
    uint16_t x_limit;
    uint16_t y_limit;

    switch (font)
    {
#if (GFX_USE_FONT_12 != 0)
    case GFX_FONT_12:
    {
        ch_width = GFX_FONT_12_CHAR_WIDTH;
        ch_height = GFX_FONT_12_CHAR_HEIGHT;
        break;
    }
#endif
#if (GFX_USE_FONT_16 != 0)
    case GFX_FONT_16:
    {
        ch_width = GFX_FONT_16_CHAR_WIDTH;
        ch_height = GFX_FONT_16_CHAR_HEIGHT;
        break;
    }
#endif
#if (GFX_USE_FONT_24 != 0)
    case GFX_FONT_24:
    {
        ch_width = GFX_FONT_24_CHAR_WIDTH;
        ch_height = GFX_FONT_24_CHAR_HEIGHT;
        break;
    }
#endif
#if (GFX_USE_FONT_32 != 0)
    case GFX_FONT_32:
    {
        ch_width = GFX_FONT_32_CHAR_WIDTH;
        ch_height = GFX_FONT_32_CHAR_HEIGHT;
        break;
    }
#endif
    default:
    {
        return;
    }
    }

    x_raw = x;
    y_raw = y;
    x_limit = ((x + width + 1) > GFX_WIDTH) ? GFX_WIDTH : (x + width + 1);
    y_limit = ((y + height + 1) > GFX_HEIGHT) ? GFX_HEIGHT : (y + height + 1);

    while ((*str >= ' ') && (*str <= '~'))
    {
        if (x + ch_width >= x_limit)
        {
            x = x_raw;
            y += ch_height;
        }

        if (y + ch_height >= y_limit)
        {
            y = x_raw;
            x = y_raw;
        }

        show_char(x, y, *str, font, color);

        x += ch_width;
        str++;
    }
}
#endif

#ifdef GFX_DEMO
int main(int argc, char const *argv[])
{

    uint16_t x = 10, y = 10;

    gfx_init(x, y);

    uint16_t c[] = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    fill_rect(0, 0, 2, 2, c, MULTIPLE_COLORS);
    fill_rect(5, 5, 8, 8, 1, SINGLE_COLOR);

    uint16_t *fb;

    get_buffer(&fb);

    for (size_t i = 0; i < y; i++)
    {
        for (size_t j = 0; j < x; j++)
        {
            printf("%d ", fb[i * x + j]);
        }
        printf("\n");
    }

    return 0;
}
#endif
