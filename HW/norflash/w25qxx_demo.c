#include "config.h"
#include "driver_w25qxx_basic.h"
#include "w25qxx_demo.h"
void w25qxx_demo()
{
    uint8_t res = w25qxx_basic_init(W25Q128, W25QXX_INTERFACE_SPI, W25QXX_BOOL_FALSE);
    if (res != 0)
    {
        printf("[E][HW] w25qxx_basic_init failed.\n");
    }
    uint8_t manufacturer;
    uint8_t device_id;
    res = w25qxx_basic_get_id((uint8_t *)&manufacturer, (uint8_t *)&device_id);
    if (res != 0)
    {
        printf("[E][HW] w25qxx_basic_get_id failed.\n");
        (void)w25qxx_basic_deinit();
    }
    printf("[I][HW] Manufacturer: 0x%02X, Device ID: 0x%02X\n", manufacturer, device_id);
    uint8_t data[8];
    // first read data from 0x00000000, check it.
    res = w25qxx_basic_read(0x00000000, (uint8_t *)data, 8);
    if (res != 0)
    {
        printf("[E][HW] w25qxx_basic_read failed.\n");
        (void)w25qxx_basic_deinit();
    }
    printf("[I][HW] Read data: ");
    for (uint8_t i = 0; i < 8; i++)
    {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
    // write 0-7 data to 0x00000000, then read it.
    for (size_t i = 0; i < 8; i++)
    {
        data[i] = i;
    }
    res = w25qxx_basic_write(0x00000000, (uint8_t *)data, 8);
    if (res != 0)
    {
        printf("[E][HW] w25qxx_basic_write failed.\n");
        (void)w25qxx_basic_deinit();
    }
    // read data from 0x00000000, check it.
    res = w25qxx_basic_read(0x00000000, (uint8_t *)data, 8);
    if (res != 0)
    {
        printf("[E][HW] w25qxx_basic_read failed.\n");
        (void)w25qxx_basic_deinit();
    }
    printf("[I][HW] Read data: ");
    for (uint8_t i = 0; i < 8; i++)
    {
        printf("0x%02X ", data[i]);
    }
    printf("\n");
    // deinit
    (void)w25qxx_basic_deinit();
}
