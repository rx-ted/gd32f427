CURRENCT_DIR=Drivers/GD32F4xx_HAL_Driver
HAL_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 1 -type d )
HAL_HDRS := $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.h))

HAL_INCS := $(foreach dir, $(sort $(dir $(HAL_HDRS))), -I$(dir))
HAL_SRCS := $(foreach dir, $(HAL_DIRS), $(wildcard $(dir)/*.c))

INCS += $(HAL_INCS)
