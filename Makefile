# project name
TARGET_NAME=f427
OUTPUT_DIR = output
BUILD_DIR = build
TARGET=$(OUTPUT_DIR)/$(TARGET_NAME)
DEBUG = 1
OPT = -Os

CROSS_COMPILE=arm-none-eabi-
#CROSS_COMPILE =/opt/arm-2011.03/bin/arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
AS=$(CROSS_COMPILE)as
OC=$(CROSS_COMPILE)objcopy
OD=$(CROSS_COMPILE)objdump
SZ=$(CROSS_COMPILE)size

INCS=

#######################################
# CFLAGS
#######################################
# cpu
CPU = -mcpu=cortex-m4



MCU = $(CPU) -mthumb $(FPU) $(FLOAT-ABI) -Wl,--no-warn-rwx-segment


DEFFLAGS=-DDEBUG -DUSE_STDPERIPH_DRIVER -DGD32F427


FLAGS= $(DEFFLAGS) -c \
	$(MCU) \
	-g3 --specs=nano.specs -mfloat-abi=soft \
	-MMD -MP -MF"$(@:%.o=%.d)" \
	-MT"$@"


CFLAGS= $(FLAGS) \
	-std=c11 -O0 \
	-ffunction-sections \
	-fdata-sections -Wall\
	-fstack-usage

SFLAGS= $(FLAGS) -DDEBUG \
	-x assembler-with-cpp


LDSCRIPT= $(LD_SRCS)
LDFLAGS	= $(MCU) -T"$(LDSCRIPT)" --specs=nosys.specs -Wl,-Map="$(TARGET).map" -Wl,--gc-sections -static --specs=nano.specs  -Wl,--start-group -lc -lm -Wl,--end-group


include Core/subdir.mk
include HW/subdir.mk
include Drivers/subdir.mk
include Startup/subdir.mk
# if you need to expand other library, pls add the path here.



SRCS = \
	$(FW_SRCS) \
	$(HW_SRCS) \
	$(CORE_SRCS) \
	$(HAL_SRCS) \


.PHONY : clean all

all: $(TARGET).bin $(TARGET).hex  $(TARGET).list

OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SRCS:.c=.o)))
vpath %.c $(sort $(dir $(SRCS)))

OBJECTS += $(addprefix $(BUILD_DIR)/,$(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))


clean:
	rm -rf $(BUILD_DIR)/*
	rm -rf $(OUTPUT_DIR)/*

$(OUTPUT_DIR):
	mkdir -p $@

$(BUILD_DIR):
	mkdir -p $@

$(TARGET).list: $(TARGET).elf
	@echo '[LIST] Finished building: $@'
	@$(OD) -h -S "$<" > "$@"

$(TARGET).hex: $(TARGET).elf
	@echo '[HEX] Finished building: $@'
	@$(OC) -Oihex "$<" "$@"

$(TARGET).bin: $(TARGET).elf
	@echo '[BIN] Finished building: $@'
	@$(OC) -Obinary "$<" "$@"

$(TARGET).elf: $(OBJECTS) Makefile | $(OUTPUT_DIR)
	@$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	@echo '[ELF] Finished building: $@'
	@$(SZ) "$@"

$(BUILD_DIR)/%.o: %.c Makefile | $(BUILD_DIR)
	@echo "[CC] $<"
	@$(CC) $(INCS) $(CFLAGS)  "$<" -o "$@"

$(BUILD_DIR)/%.o: %.s Makefile | $(BUILD_DIR)
	@echo "[AS] $<"
	@$(CC) $(INCS) $(SFLAGS)  "$<" -o "$@"


# -f /usr/share/openocd/scripts/interface/stlink.cfg 

# Usually use openocd in the stlink to write image.
flash:
	openocd -f /usr/share/openocd/scripts/interface/cmsis-dap.cfg \
		-f /usr/share/openocd/scripts/target/stm32f4x.cfg \
		-c "cmsis_dap_backend hid" \
		-c 'transport select swd' \
		-c init -c halt \
		-c "flash write_image erase $(TARGET).hex" \
		-c reset -c shutdown
