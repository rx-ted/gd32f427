# GD32F427

the board has 64KB, supports lcd, w25qxx and etc.

## Support

- [x] blink
- [x] communitation: usart, spi, i2c and etc.
- [x] lcd
- [x] w25qxx
- [x] debugging & tracing
- [x] use nvim/vim
- [x] write or read monery from board

## Usage

```bash
# compile the project
make -j100
# generate compile_command_json
make clean &&  bear -- make 
# use openocd or stlink write to board
# No require.
make flash
# or use usart to write
# requirement: set boot to 1
make upload
# Not support read from board
# XXX
```

## debug and trace

requirement:

- nvim
  - finish to depend
- arm-gdb
- openocd
- stlink driver
- stm32 board

### how to debug or trace

First to press down F5, there is a graphic or UI before we need to switch boot to on.

## contact

mailto: <gjy.18sui@gmail.com>
