CURRENCT_DIR=Core
CORE_DIRS := $(shell find $(CURRENCT_DIR) -maxdepth 2 -type d )
CORE_HDRS := $(foreach dir, $(CORE_DIRS), $(wildcard $(dir)/*.h))

CORE_INCS := $(foreach dir, $(sort $(dir $(CORE_HDRS))), -I$(dir))
CORE_SRCS := $(foreach dir, $(CORE_DIRS), $(wildcard $(dir)/*.c))

INCS += $(CORE_INCS)
