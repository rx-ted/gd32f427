# lcd pin out

## pin to pin default

|  id   | gpio  | default  |            lcd default             |     description      |
| :---: | :---: | :------: | :--------------------------------: | :------------------: |
|   1   |  PB0  |  T_SCK   |  T_SCK电阻触摸屏：时钟信号输出；   | 电容触摸屏：SCL信号  |
|   2   | PC13  |   T_CS   |     T_CS电阻触摸屏：片选信号；     | 电容触摸屏：复位信号 |
|   3   |  PB1  |  T_PEN   | T_PEN电阻触摸屏：笔接触中断引脚；  | 电容触摸屏：INT信号  |
|   4   | PF11  |  T_MOSI  | T_MOSI电阻触摸屏：串行数据输出端； | 电容触摸屏：SDA信号  |
|   5   |  PB2  |  T_MISO  | T_MISO电阻触摸屏：串行数据输入端； |    电容触摸屏：NC    |
|   6   |  NC   |  LCD5V   |               LCD5V                |         电源         |
|   7   |  NC   |   GND    |                GND                 |        电源地        |
|   8   |  NC   |   GND    |                GND                 |        电源地        |
|   9   |  NC   |  LCDVDD  |              LCD3.3V               |         电源         |
|  10   |  NC   |  LCDVDD  |              LCD3.3V               |         电源         |
|  11   | PB15  |  LCD_BL  |               LCD_BL               |      背光控制脚      |
|  12   | PD10  | EXMC_D15 |              LCD_D15               |    LCD数据线 高位    |
|  13   |  PD9  | EXMC_D14 |              LCD_D14               |      LCD数据线       |
|  14   |  PD8  | EXMC_D13 |              LCD_D13               |      LCD数据线       |
|  15   | PE15  | EXMC_D12 |              LCD_D12               |      LCD数据线       |
|  16   | PE14  | EXMC_D11 |              LCD_D11               |      LCD数据线       |
|  17   | PE13  | EXMC_D10 |              LCD_D10               |      LCD数据线       |
|  18   | PE12  | EXMC_D9  |               LCD_D9               |      LCD数据线       |
|  19   | PE11  | EXMC_D8  |               LCD_D8               |      LCD数据线       |
|  20   | PE10  | EXMC_D7  |               LCD_D7               |      LCD数据线       |
|  21   |  PE9  | EXMC_D6  |               LCD_D6               |      LCD数据线       |
|  22   |  PE8  | EXMC_D5  |               LCD_D5               |      LCD数据线       |
|  23   |  PE7  | EXMC_D4  |               LCD_D4               |      LCD数据线       |
|  24   |  PD1  | EXMC_D3  |               LCD_D3               |      LCD数据线       |
|  25   |  PD0  | EXMC_D2  |               LCD_D2               |      LCD数据线       |
|  26   | PD15  | EXMC_D1  |               LCD_D1               |      LCD数据线       |
|  27   | PD14  | EXMC_D0  |               LCD_D0               |    LCD数据线低位     |
|  28   |  PG6  | LCD_RST  |              LCD_RST               |     LCD复位信号      |
|  29   |  PD4  | EXMC_NOE |               LCD_RD               |      LCD读信号       |
|  30   |  PD5  | EXMC_NWE |               LCD_WR               |      LCD写信号       |
|  31   | PF12  | EXMC_A6  |               LCD_RS               |   LCD命令/数据选择   |
|  32   | PG12  | EXMC_NE3 |               LCD_CS               |     LCD片选信号      |
