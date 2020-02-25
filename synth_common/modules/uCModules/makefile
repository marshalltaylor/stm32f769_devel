# uCModules module... yeah
SYNTH_COMMON_ROOT := ../..
REPO_ROOT = ../$(SYNTH_COMMON_ROOT)

C_SOURCES = \
src/uCModules.cpp \
src/flagMessaging.cpp \
src/HardwareInterfaces.cpp \
src/Panel.cpp \
src/PanelComponents.cpp \
src/PanelDataObjects.cpp \
src/timeKeeper.cpp \
src/timeKeeper32.cpp \
src/timerModule.cpp \
src/timerModule32.cpp

SOURCE_DIRS = ./ \
src

C_INCLUDES = \
$(REPO_ROOT)/bsp \

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = uCModules.a

default: info all

include $(SYNTH_COMMON_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(C_INCLUDES)
	@echo $(OBJECTS)
