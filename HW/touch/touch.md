# touch

## inter face

when the local address is 0x14, the read cmd is 0x29, the write cmd is 0x28

7bit address: 0x14      ->  001 0100 (high 7 bits)
8bit write address 0x28 -> 0010 1000 (last 1 bit write)
8bit read address 0x29  -> 0010 1001 (last 1 bit read)

### write 时序

S-Address_W-ack-write cmd : register high-ack-write cmd : register low-ack-dataX-E

### read 时序

S-Address_W-ack-write cmd : register high-ack-write cmd : register low-ack-E
S-Address_R-ack-dataX-nack-E


## pin to pin default

|  id   | gpio  | pinout name |             touch name             |     description      |
| :---: | :---: | :---------: | :--------------------------------: | :------------------: |
|   1   |  PB0  |    T_SCK    |  T_SCK电阻触摸屏：时钟信号输出；   | 电容触摸屏：SCL信号  |
|   2   | PC13  |    T_CS     |     T_CS电阻触摸屏：片选信号；     | 电容触摸屏：复位信号 |
|   3   |  PB1  |    T_PEN    | T_PEN电阻触摸屏：笔接触中断引脚；  | 电容触摸屏：INT信号  |
|   4   | PF11  |   T_MOSI    | T_MOSI电阻触摸屏：串行数据输出端； | 电容触摸屏：SDA信号  |
|   5   |  PB2  |   T_MISO    | T_MISO电阻触摸屏：串行数据输入端； |    电容触摸屏：NC    |
