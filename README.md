# GD32F427

The GD32F427ZGT6 features an efficient Arm Cortex-M4 core, running at up to 200MHz, suitable for complex embedded applications.

## Rich Peripheral Interfaces

- Includes 4 USARTs, 4 UARTs, 3 I2Cs, and 6 SPIs for diverse communication needs.
- High-speed ADC and DAC for precise analog signal acquisition and output.
- Integrated TFT LCD controller and hardware graphics accelerator IPA, supporting 8-bit to 14-bit Camera video interface for graphical display and image processing applications.

## Advanced feature support

- Hardware Floating Point Unit (FPU) for accelerated single-precision floating-point math operations, enhancing computational performance.
- Memory Protection Unit (MPU) to improve application security.

## Support Features

- [x] **Delay Functions**
  - `delay_us` and `delay_ms` functions available.
- [x] **LED Control**
  - Supports controlling 2 LEDs (turn off, turn on, toggle).
- [x] **Keyboard Interrupts**
  - Detects short press and long press events.
- [x] **USART Communication**
  - Default USART1 configured.
  - Includes a print function for logging.
  - [ ] Shell Environment
- [x] **LCD Display**
  - Supports 8080 16-bit interface using FSMC.
  - Resolution: 320x480.
- [x] **Touch Input**
  - Multi-touch capable (up to 5 points).
  - Defaults to reading the first touch point.
- [x] **AT24CXX EEPROM**
  - Configured on I2C1.
  - Supports read and write operations.
- [x] **W25QXX Flash Memory**
  - Configured on SPI3.
  - Utilizes the [w25qxx library](https://github.com/libdriver/w25qxx).
- [x] **Debugging & Tracing**
  - Uses OpenOCD for debugging and tracing.
- [x] **Text Editor**
  - Supports using nvim/vim for code editing.
- [x] **Build System**
  - Utilizes xmake for building the project.
- [ ] **SD Card Support**
- [ ] **USB Connectivity**
- [ ] **Camera Integration**
- [ ] **Memory Management**
- [ ] **RTOS (Real-Time Operating System)**

## Usage

### Prerequisites

- `nvim` or `vscode`
- `arm-gdb` (version 13.3+)
- `openocd`
- ST-Link driver (`st-link`, `daplink`)
- STM32 board (`gd32f427zgt6`)

### Clone the Repository

Run the following command in the terminal:

```sh
git clone https://github.com/rx-ted/gd32f427 && cd gd32f427
```

### Build the Project

Ensure `xmake` is installed. To build the project, run:

```sh
xmake
```

For more information on `xmake`, visit [xmake](https://xmake.io/#/).

### Flash the Firmware

Ensure `openocd` and the necessary drivers (`st-link`, `daplink`, etc.) are installed. Modify the `executable` and `configFiles` in `.vscode/launch.json` as follows:

```json
"executable": "Apps/XXX/build/XXX.elf",
"configFiles": ["interface/cmsis-dap.cfg", "target/stm32f4x.cfg"]
```

After building the project, flash the firmware with:

```sh
xmake r
```

or

```sh
xmake run
```

### Clean the Project

To clean the project, run:

```sh
xmake c
```

or

```sh
xmake clean
```

### Debugging and Tracing

Press `F5` to start debugging. Ensure the boot switch is on. You will see `VARIABLES`, `WATCH`, `REGISTERS`, and other debugging information.

## Contact

For inquiries, email: <gjy.18sui@gmail.com>
