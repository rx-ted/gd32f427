#ifndef __TM16XX_H
#define __TM16XX_H

// TM1624 驱动代码 7X11
/*
TM1624 Pinout:
             +----v----+
        DIO -|1      24|- GRD1
        CLK -|2      23|- GRD2
        STB -|3      22|- GND
        VDD -|4      21|- GRD3
       SEG1 -|5      20|- GRD4
       SEG2 -|6      19|- GND
       SEG3 -|7      18|- SEG14/GRD5
       SEG4 -|8      17|- SEG13/GRD6
       SEG5 -|9      16|- SEG12/GRD7
       SEG6 -|10     15|- SEG11
       SEG7 -|11     14|- SEG10
       SEG8 -|12     13|- SEG9
             +---------+
       SEG1-SEG11 +
       GRD1-GRD7  -
*/

#define TM1624_MAX_POS 7
#define TM1624_MAX_SEG 11

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint32_t dataPort;
    uint32_t dataPin;
    uint32_t clkPort;
    uint32_t clkPin;
    uint32_t stbPort;
    uint32_t stbPin;
    uint8_t maxDisplays; // 0-7
    uint8_t maxSegments;
    bool activateDisplay;
} Module;

#define TM16XX_CMD_DATA_AUTO 0x40
#define TM16XX_CMD_DATA_READ 0x42 // command to read data used on two wire interfaces of TM1637
#define TM16XX_CMD_DATA_FIXED 0x44
#define TM16XX_CMD_DISPLAY 0x80
#define TM16XX_CMD_ADDRESS 0xC0

#define GrayScale_OFF 0x80 // 关显示
#define GrayScale_ON 0x81  // 开显示

#define GrayScale1 0x88 // 灰度等级1
#define GrayScale2 0x89 // 灰度等级2
#define GrayScale3 0x8A // 灰度等级3
#define GrayScale4 0x8B // 灰度等级4
#define GrayScale5 0x8C // 灰度等级5
#define GrayScale6 0x8D // 灰度等级6
#define GrayScale7 0x8E // 灰度等级7
#define GrayScale8 0x8F // 灰度等级8

void tm16xx_init(Module *module);

void sendCommand(Module *module, uint8_t cmd);
void sendData(Module *module, uint8_t data);

void start(Module *module);
void stop(Module *module);
void clearDisplay(Module *module);

void setupDisplay(Module *module, uint8_t mode);
void setSegments(Module *module, uint8_t address, uint8_t data);

#endif
