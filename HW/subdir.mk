CURRENCT_DIR=HW
# HW_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 2 -type d -printf '%P ')
HW_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 2 -type d )

HW_SRCS := $(foreach dir, $(HW_DIRS), $(wildcard $(dir)/*.c))

HW_INCS := $(foreach dir, $(sort $(dir $(HW_SRCS))), -I$(dir))


INCS += $(HW_INCS)