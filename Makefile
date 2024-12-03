# project name
CURR_DIR=.

TARGET= $(notdir $(shell pwd))
BUILD_DIR=$(CURR_DIR)/build
DEBUG = 1
OPT = -Og

HAL_LIB=GD32F4xx
HAL_PREFIX=GD


CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
XX=$(CROSS_COMPILE)g++
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
# AS=$(CROSS_COMPILE)as
AS=$(CROSS_COMPILE)gcc -x assembler-with-cpp
OC=$(CROSS_COMPILE)objcopy
OD=$(CROSS_COMPILE)objdump
SZ=$(CROSS_COMPILE)size
HEX = $(OC) -O ihex
BIN = $(OC) -O binary -S


CPU = -mcpu=cortex-m4
FPU = -mfpu=fpv4-sp-d16
FLOAT-ABI = -mfloat-abi=hard
MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI)

AS_DEFS =

C_DEFS =  \
-DUSE_HAL_DRIVER \
-DSTM32F427xx \
-DGD32F427 \
-DSTM32_THREAD_SAFE_STRATEGY=2

AS_INCLUDES =
ASM_SOURCES = startup_stm32f427xx.s
ASMM_SOURCES =


C_INCLUDES =
C_SOURCES =
CPP_SOURCES = 

ASFLAGS = $(MCU) $(AS_DEFS) $(AS_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

CFLAGS += $(MCU) $(C_DEFS) $(C_INCLUDES) $(OPT) -Wall -fdata-sections -ffunction-sections

ifeq ($(DEBUG), 1)
CFLAGS += -g -gdwarf-2
endif

CFLAGS += -MMD -MP -MF"$(@:%.o=%.d)"

LDSCRIPT = STM32F427ZGTx_FLASH.ld

LIBS = -lc -lm -lnosys

LIBDIR =

LDFLAGS = $(MCU) -specs=nano.specs -T$(LDSCRIPT) $(LIBDIR) $(LIBS) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

include Core/subdir.mk
include Drivers/subdir.mk
include HW/subdir.mk

OBJECTS = 

all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex $(BUILD_DIR)/$(TARGET).bin
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(CPP_SOURCES:.cpp=.o)))
vpath %.cpp $(sort $(dir $(CPP_SOURCES)))

# list of ASM program objects
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))
OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASMM_SOURCES:.S=.o)))
vpath %.S $(sort $(dir $(ASMM_SOURCES)))

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR) 
	@echo "[CC] $<"
	@$(CC) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.c=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.cpp Makefile | $(BUILD_DIR) 
	@echo "[XX] $<"
	@$(XX) -c $(CFLAGS) -Wa,-a,-ad,-alms=$(BUILD_DIR)/$(notdir $(<:.cpp=.lst)) $< -o $@

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo "[AS] $<" 
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.S Makefile | $(BUILD_DIR)
	@echo "[AS] $<" 
	@$(AS) -c $(CFLAGS) $< -o $@

$(BUILD_DIR)/$(TARGET).elf: $(OBJECTS) Makefile
	@$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo '[ELF] Finished building: $@'
	@$(SZ) $@

$(BUILD_DIR)/%.hex: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo '[HEX] Finished building: $@'
	@$(HEX) $< $@
	
$(BUILD_DIR)/%.bin: $(BUILD_DIR)/%.elf | $(BUILD_DIR)
	@echo '[BIN] Finished building: $@'
	@$(BIN) $< $@	
	
$(BUILD_DIR):
	@echo 'Finished creating: $@'
	@mkdir $@		

clean:
	-rm -fR $(BUILD_DIR)
  
-include $(wildcard $(BUILD_DIR)/*.d)

flash:
	openocd  \
		-f /usr/share/openocd/scripts/interface/cmsis-dap.cfg \
		-f /usr/share/openocd/scripts/target/stm32f4x.cfg \
		-c "cmsis_dap_backend hid" \
		-c 'transport select swd' \
		-c init -c halt \
		-c "flash write_image erase build/$(TARGET).elf" \
		-c reset -c shutdown
