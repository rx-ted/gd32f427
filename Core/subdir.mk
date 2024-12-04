CURRENCT_DIR=Core


CORE_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 5 -type d )
CORE_HDRS = $(foreach dir, $(CORE_DIRS), $(wildcard $(dir)/*.h))
C_INCLUDES += $(foreach dir, $(sort $(dir $(CORE_HDRS))), -I$(dir))


C_SOURCES += $(foreach dir, $(CORE_DIRS), $(wildcard $(dir)/*.c))
CPP_SOURCES += $(foreach dir, $(CORE_DIRS), $(wildcard $(dir)/*.cpp))
