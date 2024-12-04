CURRENCT_DIR=HW

HW_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 5 -type d )
HW_HDRS = $(foreach dir, $(HW_DIRS), $(wildcard $(dir)/*.h))
C_INCLUDES += $(foreach dir, $(sort $(dir $(HW_HDRS))), -I$(dir))

C_SOURCES += $(foreach dir, $(HW_DIRS), $(wildcard $(dir)/*.c))
CPP_SOURCES += $(foreach dir, $(HW_DIRS), $(wildcard $(dir)/*.cpp))
