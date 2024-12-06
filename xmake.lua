local project_name = "demo"
local target_dir = "build"

add_rules("mode.debug", "mode.release")
set_project(project_name)
set_version('1.0.0')

-- 设置工具链
toolchain("arm-none-eabi")
set_description("arm-none-eabi-gcc compiler")
set_kind("standalone")
set_bindir('/opt/arm-gcc.13.3/bin')
set_toolset("cc", "arm-none-eabi-gcc")
set_toolset("as", "arm-none-eabi-gcc")
set_toolset("ld", "arm-none-eabi-gcc")
toolchain_end()

-- 设置编译目标
target(project_name)
-- 设置平台
set_plat("cross")
-- 设置架构
set_arch("m4")
-- 设置生成的文件名称
set_filename(project_name .. ".elf")
-- 设置编译链
set_toolchains("arm-none-eabi")
-- 生成二进制文件
set_kind("binary")
-- 启用所有警告
set_warnings("all")
-- 设置优化等级none smallest(os) optimize(o1) faster(02) fastest(o3)
set_optimize("smallest")
-- 设置编译文件的目录
set_targetdir(target_dir)
-- 添加链接库
add_links("c", "m", "nosys", "rdimon");
-- 排除的文件，不参与编译 模板文件可以直接删除
-- remove_files()
-- 源文件
add_files("HW/*.c", "HW/**/*.c", "Core/*.c", "Drivers/CMSIS/GD/GD32F4xx/Source/*.c",
    "Drivers/GD32F4xx_HAL_Driver/Source/*.c", "Drivers/CMSIS/GD/GD32F4xx/Source/GCC/startup_stm32f427xx.s")

-- 设置头文件搜索路径
add_includedirs("Core", "Drivers/CMSIS", "Drivers/CMSIS/GD/GD32F4xx/Include", "Drivers/GD32F4xx_HAL_Driver/Include",
    "HW")

-- 递归搜索子目录
local src_path = {'HW'}
for _, index in ipairs(src_path) do
    for _, dir in ipairs(os.dirs(index .. '/**')) do
        add_files(dir .. '/*.c')
        add_includedirs(dir)
    end
end

if is_mode('debug') then
    add_cflags("-Og", "-gdwarf-2")
    add_asflags("-Og", "-gdwarf-2")
end
-- 添加宏定义
add_defines("USE_HAL_DRIVER", "STM32F427xx", "STM32_THREAD_SAFE_STRATEGY=2", "GD32F427")

local MCU = {"-mcpu=cortex-m4", -- 指定目标处理器的 CPU 架构为 Cortex-M4
"-mthumb", -- 启用 Thumb 指令集 
"-mfpu=fpv4-sp-d16", -- 指定使用 fpv4-sp-d16 浮点单元。 
"-mfloat-abi=hard" -- 指定浮点运算的 ABI（Application Binary Interface）为硬件浮点
}

local FLAGS = {"-Wall", "-fdata-sections -ffunction-sections"}

-- 设置C编译参数
add_cflags(table.concat(MCU, ' '), table.concat(FLAGS, ' '), {
    force = true
} -- 强制覆盖之前的编译参数设置。
)

-- 设置汇编编译参数
add_asflags(table.concat(MCU, ' '), {
    force = true
})

-- 设置链接参数
local MapCMD = "-Wl,-Map=" .. target_dir .. '/' .. project_name .. ".map,--cref"
local LDFLAGS = {"-specs=nano.specs", "-Tlink.ld", "-lc -lm -lnosys", MapCMD, "-Wl,--gc-sections", "-u _printf_float"}
add_ldflags(table.concat(MCU, ' '), table.concat(LDFLAGS, ' '), {
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
        "openocd -f interface/cmsis-dap.cfg -f target/stm32f4x.cfg -c 'cmsis_dap_backend hid' -c 'transport select swd' -c 'program %s verify reset exit'",
        target:targetfile())
    print(cmd)
    os.exec(cmd)
end)
