C_INCLUDES += -IDrivers/CMSIS/Device/ST/$(HAL_LIB)/Include
C_INCLUDES += -IDrivers/CMSIS/Include

CURRENCT_DIR=Drivers/$(HAL_LIB)_HAL_Driver
HAL_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 1 -type d )
HAL_HDRS = $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.h))
C_INCLUDES += $(foreach dir, $(sort $(dir $(HAL_HDRS))), -I$(dir))

C_SOURCES+= $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.c))
CPP_SOURCES+= $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.cpp))
ASM_SOURCES+= $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.s))
ASMM_SOURCES+= $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.S))
