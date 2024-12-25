# XMake

Compate XMake with Makefile, Cmake, I think xmake this app can be better by me. these only need a few command and get we needed.
expect these, I can learn other languages, such as Lua, it's so easy for syntax, can let user quickly use it.

## compile

Run `xmake` and get elf file.
If  you need to know more, you can run `xmake p make` and get makefile as new file.

## upload or download to the board

run `xmake run demo`, or using other way to upload the board, such as `stm32flash`, `OpenOCD`, or `PyOCD`, etc.


## how to debug

Find `Run & Debug` on the left menu bar and click `Debug with OpenOCD`.  
when debugging, we default set to `break main`.  
If you want to check a function for the ELF progamming, you can modify for this function.
