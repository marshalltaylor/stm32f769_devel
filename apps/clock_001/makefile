# STM32F4xx bsp uart test
REPO_ROOT := ../..
SYNTH_COMMON_ROOT := $(REPO_ROOT)/synth_common

ifeq ($(strip $(board)),)
ifneq ('$(MAKECMDGOALS)','clean')
$(info no board specified, setting to clock)
board = clock
endif
endif

C_SOURCES =  \
main.c \
App.cpp \
BlinkerPanel.cpp \
midiTime.cpp \
StatusPanel.cpp

SOURCE_DIRS = . \

C_INCLUDES = \
$(REPO_ROOT)/bsp \
$(SYNTH_COMMON_ROOT)/modules/logging \
$(SYNTH_COMMON_ROOT)/modules/SegmentVideo \
$(SYNTH_COMMON_ROOT)/modules/midi47fx \
$(SYNTH_COMMON_ROOT)/modules/uCModules

STATIC_LIBS = \
$(SYNTH_COMMON_ROOT)/modules/SegmentVideo/build/SegmentVideo.a \
$(SYNTH_COMMON_ROOT)/modules/midi47fx/build/midi47fx.a \
$(SYNTH_COMMON_ROOT)/modules/uCModules/build/uCModules.a \
$(SYNTH_COMMON_ROOT)/modules/logging/build/logging.a \
$(REPO_ROOT)/bsp/build/bsp_$(board).a

# objects
OBJECTS = $(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES)))

# rules
default: all

include $(SYNTH_COMMON_ROOT)/tools/build/catalog.mk

include $(SYNTH_COMMON_ROOT)/tools/build/common.mk

# linker
$(BUILD_DIR)/output.elf: build_dirs $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo "#### Linking into elf"
	$(PP) -o $(TARGET_EXEC) $(LDFLAGS) -std=c++11 $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS)) $(STATIC_LIBS)
	$(SZ) $@
	@echo ""
	@echo "Built for $(board) board"
	@echo ""
	$(OBJD) -S --disassemble $(TARGET_EXEC) > $(TARGET_EXEC).dump
	@echo

$(BUILD_DIR)/output.hex: build_dirs $(TARGET_EXEC)
	@echo "#### Elf to hex"
	$(HEX) $< $@
	@echo

$(BUILD_DIR)/output.bin: build_dirs $(TARGET_EXEC)
	@echo "#### Elf to bin"
	$(BIN) $< $@
	@echo

.PHONY: $(STATIC_LIBS)

all: $(STATIC_LIBS) $(TARGET_EXEC)

clean:: clean_all

openocd:
	cygstart cmd /k $(shell cygpath -w $(PATH_OPENOCD)/launch_openocd.bat)

gdb:
	@echo ""
	@echo "gdb needs special ctrl-c treatment."
	@echo "Launch cmd.exe from start and enter these commands:"
	@echo ""
	@echo "E:"
	@echo "cd $(shell cygpath -w $(shell pwd))"
	@echo "$(shell cygpath -w $(GDB)) -x gdbinit.txt build/output.elf"

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES) 
