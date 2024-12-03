#include "hw.h"
#include "tm16xx.h"

uint8_t x = 0;
uint8_t lcd_id[12]; /* 存放LCD ID字符串 */
uint16_t colors[] = {
    WHITE,      /* 白色 */
    BLACK,      /* 黑色 */
    RED,        /* 红色 */
    GREEN,      /* 绿色 */
    BLUE,       /* 蓝色 */
    MAGENTA,    /* 品红色/紫红色 = BLUE + RED */
    YELLOW,     /* 黄色 = GREEN + RED */
    CYAN,       /* 青色 = GREEN + BLUE */
    BROWN,      /* 棕色 */
    BRRED,      /* 棕红色 */
    GRAY,       /* 灰色 */
    DARKBLUE,   /* 深蓝色 */
    LIGHTBLUE,  /* 浅蓝色 */
    GRAYBLUE,   /* 灰蓝色 */
    LIGHTGREEN, /* 浅绿色 */
    LGRAY,      /* 浅灰色(PANNEL),窗体背景色 */
    LGRAYBLUE,  /* 浅灰蓝色(中间层颜色) */
    LBBLUE,     /* 浅棕蓝色(选择条目的反色) */

};

uint16_t color_size = sizeof(colors) / sizeof(colors[0]);

Module module = {
    .clkPort = GPIOF,
    .clkPin = GPIO_PIN_1,
    .dataPort = GPIOF,
    .dataPin = GPIO_PIN_3,
    .stbPort = GPIOF,
    .stbPin = GPIO_PIN_12,
    .maxDisplays = 7,
    .maxSegments = 11,
};

void setup()
{
  delay_init(200);    /* 延时初始化 */
  usart_init(115200); /* 串口初始化为115200 */
  led_init();
  extix_init();

  // timer13_pwm_init(500 - 1, 100 - 1, GPIOF, GPIO_PIN_9);
  // lcd_init(); /* 初始化LCD */
  sprintf((char *)lcd_id, "LCD ID:%04X",
          lcddev.id);                 /* 将LCD ID打印到lcd_id数组 */
  rcu_periph_clock_enable(RCU_GPIOF); /* 使能GPIOF时钟 */

  // tm16xx_init(&module);
  delay_ms(20);
  // delay_ms(20);
  // setupDisplay(&module);
  uint8_t i = 8;
  while (i--)
  {

    gpio_bit_write(module.clkPort, module.clkPin, SET);
    gpio_bit_write(module.dataPort, module.dataPin, SET);
    gpio_bit_write(module.stbPort, module.stbPin, SET);
    delay_ms(1000);

    gpio_bit_write(module.clkPort, module.clkPin, RESET);
    gpio_bit_write(module.dataPort, module.dataPin, RESET);
    gpio_bit_write(module.stbPort, module.stbPin, RESET);
    delay_ms(1000);
  }
}

void lcd_example()
{
  lcd_clear(colors[x]);
  lcd_show_string(10, 40, 200, 32, 32, "GD32F427 ^_^", RED);
  lcd_show_string(10, 80, 200, 24, 24, "TFTLCD TEST", RED);
  lcd_show_string(10, 110, 200, 16, 16, "WKS SMART", RED);
  lcd_show_string(10, 130, 200, 16, 16, (char *)lcd_id, RED); /* 显示LCD ID */
  x++;

  if (x == color_size)
    x = 0;
  delay_ms(500);
};
void loop()
{
  gpio_bit_toggle(module.clkPort, module.clkPin);
  gpio_bit_toggle(module.dataPort, module.dataPin);
  gpio_bit_toggle(module.stbPort, module.stbPin);

  delay_ms(500);
  // for (uint8_t i = 0; i < 7; i++)
  // {
  //   setSegments(&module, 0x0c | i, 0xff);
  // }
  delay_ms(100);
}

void hw()
{
  setup();
  while (1)
    loop();
}
