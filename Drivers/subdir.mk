C_INCLUDES += -IDrivers/CMSIS/$(HAL_PREFIX)/$(HAL_LIB)/Include
C_INCLUDES += -IDrivers/CMSIS
# need to add the source file.
C_SOURCES+= Drivers/CMSIS/$(HAL_PREFIX)/$(HAL_LIB)/Source/system_gd32f4xx.c

CURRENCT_DIR=Drivers/$(HAL_LIB)_HAL_Driver
HAL_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 1 -type d )
HAL_HDRS = $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.h))
C_INCLUDES += $(foreach dir, $(sort $(dir $(HAL_HDRS))), -I$(dir))

C_SOURCES+= $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.c))
CPP_SOURCES+= $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.cpp))
