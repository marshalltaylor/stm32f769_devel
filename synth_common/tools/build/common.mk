#######################################
# Paths
#######################################
# Inputs:
#  SYNTH_COMMON_ROOT
#  REPO_ROOT
# OUTPUT_LIB_NAME -- lib.a
#   Needed for archiver target
# TODO:
# $(STATIC_LIBS), should the common file also do abnormal things, like linking?

# Default target
common_no_target:
	@echo "Don't run common.mk with no target"
	@echo "ass"
	@exit 1

# Build directories
BUILD_DIR = build
TARGET_EXEC := $(BUILD_DIR)/output.elf

# TODO: Best place for linker file?
LINKER_FILE = $(REPO_ROOT)/bsp/STM32F446RETx_FLASH.ld

BUILD_PATHS = \
$(BUILD_DIR) \
$(BUILD_DIR)/obj \
$(BUILD_DIR)/dep \
$(addprefix $(BUILD_DIR)/obj/,$(SOURCE_DIRS)) \
$(addprefix $(BUILD_DIR)/dep/,$(SOURCE_DIRS))

# Tools
#GCC_PATH = /cygdrive/c/STM32/gcc-arm-none-eabi/bin
GCC_PATH = C:/STM32/gcc-arm-none-eabi/bin
CC = $(GCC_PATH)/arm-none-eabi-gcc
PP = $(GCC_PATH)/arm-none-eabi-g++
AS = $(GCC_PATH)/arm-none-eabi-as
AR = $(GCC_PATH)/arm-none-eabi-ar
CP = $(GCC_PATH)/arm-none-eabi-objcopy
SZ = $(GCC_PATH)/arm-none-eabi-size
OBJD = $(GCC_PATH)/arm-none-eabi-objdump
GDB = $(GCC_PATH)/arm-none-eabi-gdb

#######################################
# Flags
#######################################
# c
#  -DUSE_HAL_DRIVER -DSTM32F7xx  '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -fmessage-length=0 -Og -ffunction-sections
# -v
CFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F4xx -DSTM32F446xx -g -Wall -Werror -c -O0

# Generate dependency information
CFLAGS += -MMD -MP -MF "$(@:$(BUILD_DIR)/obj/%.o=$(BUILD_DIR)/dep/%.d)"

# Message behavior
# -fsyntax-only -fmax-errors=1 -w -Werror 
CFLAGS += -fmax-errors=1

# cpp
CPPFLAGS = $(CFLAGS) -std=c++11 -fno-rtti

# assembler
ASFLAGS = -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -g -v

# linker -lstdc++ -lsupc++  -Wl,--gc-sections  -fno-lto
LDFLAGS = -v -lc -g -lm -lstdc++ -lsupc++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -specs=nosys.specs -specs=nano.specs -T$(LINKER_FILE) -Wl,-Map=$(BUILD_DIR)/output.map -Wl,--gc-sections
#LDFLAGS = -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -specs=nano.specs -T$(LINKER_FILE) -Wl,-Map=$(BUILD_DIR)/$(TARGET).map,--cref -Wl,--gc-sections

# binaries
HEX = $(CP) -O ihex
BIN = $(CP) -O binary -S

#######################################
# dependencies
#######################################
%.d: ;

DEPFILES := $(patsubst %,$(BUILD_DIR)/dep/%.d,$(basename $(OBJECTS)))
$(DEPFILES):
#include $(wildcard $(DEPFILES))

include $(DEPFILES)

#-include $(wildcard $(BUILD_DIR)/*.d)
#include $(patsubst %,(BUILD_DIR)/dep/%.d,$(basename $(OBJECTS)))

#######################################
# Rules
#######################################
# Objects
$(BUILD_DIR)/obj/%.o: %.cpp
	@echo "#### Compiling: $<"
	$(PP) $(CPPFLAGS) $(addprefix -I,$(C_INCLUDES)) $< -o $@
	@echo

$(BUILD_DIR)/obj/%.o: %.c
	@echo "#### Compiling: $<"
	$(CC) $(CFLAGS) $(addprefix -I,$(C_INCLUDES)) $< -o $@
	@echo

$(BUILD_DIR)/obj/%.o: %.s
	@echo "#### Compiling: $<"
	@echo "$(shell cygpath -w -a $<)"
	$(AS) -c $(ASFLAGS) $(addprefix -I,$(C_INCLUDES)) $< -o $@
	@echo

# Archiver
$(BUILD_DIR)/%.a: build_dirs $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo "#### Creating archive"
	$(AR) rcs $@ $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	$(OBJD) -S --disassemble $@ > $@.dump
	@echo

# Directory creation
.PHONY: build_dirs

build_dirs: $(BUILD_PATHS)

$(BUILD_PATHS):
	mkdir -p $@

# Clean
clean::
	rm -rf $(BUILD_DIR)
