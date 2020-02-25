# STM32F4xx bsp
SYNTH_COMMON_ROOT := ..
REPO_ROOT = ../$(SYNTH_COMMON_ROOT)

C_SOURCES =  \
Source/portable/MemMang/heap_4.c \
Source/portable/GCC/ARM_CM4F/port.c \
Source/croutine.c \
Source/event_groups.c \
Source/list.c \
Source/queue.c \
Source/tasks.c \
Source/timers.c

# List of directories which contain C_SOURCES.
# This list is created within the BUILD_DIR and used for artifacts
SOURCE_DIRS = .\
Source/portable/MemMang \
Source/portable/GCC/ARM_CM4F \
Source \

C_INCLUDES = .\
$(REPO_ROOT)/bsp/UserProject/AltDrivers \
$(REPO_ROOT)/bsp/UserProject/Core/Inc \
$(REPO_ROOT)/bsp/CubeProject/Drivers/STM32F4xx_HAL_Driver/Inc \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Include \
$(REPO_ROOT)/bsp/CubeProject/Drivers/CMSIS/Device/ST/STM32F4xx/Include \
Source/portable/MemMang \
Source/portable/GCC/ARM_CM4F \
Source/include

#PROJECT_OBJECTS
OBJECTS = $(patsubst %s,%o,$(patsubst %c,%o,$(patsubst %cpp,%o,$(C_SOURCES))))

OUTPUT_LIB_NAME = FreeRTOS.a

default: all

include $(SYNTH_COMMON_ROOT)/tools/build/common.mk

all: $(BUILD_DIR)/$(OUTPUT_LIB_NAME)

info:
	@echo "***** ***** ***** DEBUG ***** ***** *****"
	@echo $(addprefix $(BUILD_DIR)/obj/, $(OBJECTS))
	@echo $(addprefix -I,$(C_INCLUDES))
