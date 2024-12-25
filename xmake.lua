local project_name = '00Template'

set_project(project_name)
set_version('1.0')
add_rules("mode.debug")

set_config('cc', 'arm-none-eabi-gcc')
set_config('as', 'arm-none-eabi-gcc')
set_config('ld', 'arm-none-eabi-gcc')

target(project_name)

local build_dir = 'build'
local home_dir = table.concat({'Apps', project_name}, '/')
local target_dir = table.concat({home_dir, build_dir}, '/')
local ld_file = home_dir .. '/Startup/stm32f427zgtx_flash.ld'

set_kind("binary")
set_optimize("none")
set_symbols("none")
set_arch('cross')
set_plat('cross')
set_languages("c99", "c++11")
set_targetdir(target_dir)
set_filename(project_name .. '.elf')
add_defines("USE_HAL_DRIVER", "STM32F427xx", "DEBUG")
add_files(string.format('%s/Startup/startup_stm32f427xx.s', home_dir))
add_files(home_dir .. '/Src/*.c', "Drivers/STM32F4xx_HAL_Driver/Src/*.c")
add_includedirs(home_dir .. '/Inc', "Drivers/STM32F4xx_HAL_Driver/Inc", "Drivers/CMSIS/Include",
    "Drivers/CMSIS/Device/ST/STM32F4xx/Include")

remove_files("Drivers/STM32F4xx_HAL_Driver/Src/*template.c")

-- HW, please add your hardware driver here.
add_includedirs("HW")
add_includedirs("HW/board")
add_includedirs("HW/lcd")
add_includedirs("HW/at24cxx")
add_includedirs("HW/touch")
add_includedirs("HW/delay")

add_files("HW/*.c")
add_files("HW/**/*.c")

-- Middlewares, please add your middleware here.
add_includedirs("Middlewares/lvgl")
add_includedirs("Middlewares/lvgl/examples")
add_includedirs("Middlewares/lvgl/demos")
add_files("Middlewares/lvgl/src/**/*.c")
add_files("Middlewares/lvgl/demos/**/*.c")
add_files("Middlewares/lvgl/examples/*.c")


local MCU = {"-mcpu=cortex-m4", "-mthumb", "-mfpu=fpv4-sp-d16", "-mfloat-abi=hard"}

add_cflags(table.concat(MCU, ' '), "-O3 -g -gdwarf-2 -Wall -fdata-sections -ffunction-sections", {
    force = true
})
add_asflags(table.concat(MCU, ' '), "-x assembler-with-cpp -O3 -g -gdwarf-2 -Wall -fdata-sections -ffunction-sections",
    {
        force = true
    })

add_ldflags(table.concat(MCU, ' '), "-specs=nano.specs", string.format(
    "-T%s -lc -lm -lnosys -Wl,-Map=%s/%s.map,--cref -Wl,--gc-sections", ld_file, target_dir, project_name), {
    force = true
})

target_end()

-- 构建完成后的回调函数
after_build(function(target)
    import("core.project.task")
    cprint("${bright black onwhite}********************储存空间占用情况*****************************")
    os.exec(string.format("arm-none-eabi-objcopy -O ihex %s %s.hex", target:targetfile(),
        target_dir .. '/' .. project_name))
    os.exec(string.format("arm-none-eabi-objcopy -O binary %s %s.bin", target:targetfile(),
        target_dir .. '/' .. project_name))
    print("生成已完成!")
    os.exec(string.format("arm-none-eabi-size -Ax %s", target:targetfile()))
    os.exec(string.format("arm-none-eabi-size -Bd %s", target:targetfile()))
    cprint(
        "${bright black onwhite}heap-堆、stack-栈、.data-已初始化的变量全局/静态变量,bss-未初始化的data、.text-代码和常量")
end)

on_run(function(target)
    local cmd = string.format(
        "openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg -c 'cmsis_dap_backend hid' -c 'transport select swd' -c 'program %s.hex verify reset exit'",
        target_dir .. '/' .. project_name)
    print(cmd)
    os.exec(cmd)
end)
