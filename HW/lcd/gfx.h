#ifndef __GFX_H__
#define __GFX_H__

#include "stdint.h"
#include "stdbool.h"

// ==================
//! GFX CONFIG
// #define GFX_DEMO
#define GFX_USE_FONT 1
#define GFX_USE_FONT_12 1
#define GFX_USE_FONT_16 1
#define GFX_USE_FONT_24 1
#define GFX_USE_FONT_32 1
// ==================

#if (GFX_USE_FONT == 1)
typedef enum
{
#if (GFX_USE_FONT_12 != 0)
    GFX_FONT_12, // gfx_font_1206
#endif
#if (GFX_USE_FONT_16 != 0)
    GFX_FONT_16, // gfx_font_1608
#endif
#if (GFX_USE_FONT_24 != 0)
    GFX_FONT_24, // gfx_font_2412
#endif
#if (GFX_USE_FONT_32 != 0)
    GFX_FONT_32, // gfx_font_3216
#endif
} gfx_font_t;

#endif

typedef enum
{
    GFX_NUM_SHOW_NOZERO = 0x00, /* 数字高位0不显示 */
    GFX_NUM_SHOW_ZERO,          /* 数字高位0显示 */
} gfx_num_mode_t;

typedef struct
{
    uint16_t width;
    uint16_t height;
} gfx_param_t;

void gfx_init();
void get_buffer(uint16_t **buffer);

// ! draw function
void draw_point(uint16_t x, uint16_t y, uint16_t color);
void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void draw_circle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void draw_triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2,
                   int16_t y2, uint16_t color);

// ! fill function
void fill_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t *color);
void fill_clear(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void fill_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);

// ! read from buffer
uint16_t read_point(uint16_t x, uint16_t y);
void read_rect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t **color, uint16_t *len);

#if (GFX_USE_FONT == 1)
// ! show char num string function
void show_char(uint16_t x, uint16_t y, char chr, gfx_font_t font, uint16_t color);
void show_num(uint16_t x, uint16_t y, uint32_t num, uint8_t len, gfx_font_t font, uint16_t color);
void show_xnum(uint16_t x, uint16_t y, uint32_t num, uint8_t len, gfx_font_t font, gfx_num_mode_t mode, uint16_t color);
void show_string(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char *str, gfx_font_t font, uint16_t color);
#endif

#endif