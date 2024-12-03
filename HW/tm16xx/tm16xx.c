
#include "tm16xx.h"
#include "gd32f4xx_gpio.h"

void tm16xx_init(Module *module)
{

    gpio_output_options_set(module->dataPort, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, module->dataPin);
    gpio_bit_set(module->dataPort, module->dataPin);
    gpio_mode_set(module->dataPort, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, module->dataPin);

    gpio_output_options_set(module->clkPort, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, module->clkPin);
    gpio_bit_set(module->clkPort, module->clkPin);
    gpio_mode_set(module->clkPort, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, module->clkPin);

    gpio_output_options_set(module->stbPort, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, module->stbPin);
    gpio_bit_set(module->stbPort, module->stbPin);
    gpio_mode_set(module->stbPort, GPIO_MODE_OUTPUT, GPIO_PUPD_PULLUP, module->stbPin);

    clearDisplay(module);
    setupDisplay(module, 2);
}

void start(Module *module) { gpio_bit_write(module->stbPort, module->stbPin, SET); }

void stop(Module *module) { gpio_bit_write(module->stbPort, module->stbPin, RESET); }

void sendCommand(Module *module, uint8_t cmd)
{
    start(module);
    stop(module);
    sendData(module, cmd);
}

void sendData(Module *module, uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        gpio_bit_write(module->dataPort, module->dataPin, data & 0x01 ? SET : RESET);
        data >>= 1;
        gpio_bit_write(module->clkPort, module->clkPin, RESET);
        gpio_bit_write(module->clkPort, module->clkPin, SET);
    }
}

void clearDisplay(Module *module)
{
    sendCommand(module, 0x03); // 7x11
    for (uint8_t i = 0; i < module->maxDisplays; i++)
    {
        sendCommand(module, 0xc0 | i);
        sendData(module, 0x00);
        start(module);
    }
}

void setupDisplay(Module *module, uint8_t mode)
{
    switch (mode)
    {
    case (0):
        sendCommand(module, GrayScale_ON);
        break;
    case (1):
        sendCommand(module, GrayScale1);
        break;
    case (2):
        sendCommand(module, GrayScale2);
        break;
    case (3):
        sendCommand(module, GrayScale3);
        break;
    case (4):
        sendCommand(module, GrayScale4);
        break;
    case (5):
        sendCommand(module, GrayScale5);
        break;
    case (6):
        sendCommand(module, GrayScale6);
        break;
    case (7):
        sendCommand(module, GrayScale7);
        break;
    case (8):
        sendCommand(module, GrayScale8);
        break;
    }
}

void setSegments(Module *module, uint8_t address, uint8_t data)
{
    sendCommand(module, 0x03);                  // 7x11
    sendCommand(module, TM16XX_CMD_DATA_FIXED); // 固定地址
    sendCommand(module, address);
    sendData(module, data);
    sendCommand(module, GrayScale8); // 固定地址
    start(module);
}
