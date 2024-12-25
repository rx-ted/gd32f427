# touch

## pin to pin default

|  id   | gpio  | pinout name |             touch name             |     description      |
| :---: | :---: | :---------: | :--------------------------------: | :------------------: |
|   1   |  PB0  |    T_SCK    |  T_SCK电阻触摸屏：时钟信号输出；   | 电容触摸屏：SCL信号  |
|   2   | PC13  |    T_CS     |     T_CS电阻触摸屏：片选信号；     | 电容触摸屏：复位信号 |
|   3   |  PB1  |    T_PEN    | T_PEN电阻触摸屏：笔接触中断引脚；  | 电容触摸屏：INT信号  |
|   4   | PF11  |   T_MOSI    | T_MOSI电阻触摸屏：串行数据输出端； | 电容触摸屏：SDA信号  |
|   5   |  PB2  |   T_MISO    | T_MISO电阻触摸屏：串行数据输入端； |    电容触摸屏：NC    |