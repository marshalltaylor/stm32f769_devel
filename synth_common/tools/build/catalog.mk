#######################################
# Paths
#######################################
# Inputs:
# REPO_ROOT
# SYNTH_COMMON_ROOT
# board

PATH_OPENOCD:=$(SYNTH_COMMON_ROOT)/tools/debugger
#PATH_OPENOCD=$(SYNTH_COMMON_ROOT)/tools/debugger


$(REPO_ROOT)/bsp/build/bsp_$(board).a:
	@echo "######## Building $(REPO_ROOT)/bsp_$(board)"
	$(MAKE) -C $(REPO_ROOT)/bsp board=$(board)

$(SYNTH_COMMON_ROOT)/FreeRTOS/build/FreeRTOS.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/FreeRTOS"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/FreeRTOS

$(SYNTH_COMMON_ROOT)/modules/logging/build/logging.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/logging"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/logging

$(SYNTH_COMMON_ROOT)/modules/midi47fx/build/midi47fx.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/midi47fx"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/midi47fx

$(SYNTH_COMMON_ROOT)/modules/SegmentVideo/build/SegmentVideo.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/SegmentVideo"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/SegmentVideo

$(SYNTH_COMMON_ROOT)/modules/uCModules/build/uCModules.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/uCModules"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/uCModules

$(SYNTH_COMMON_ROOT)/modules/midiUtils/build/midiUtils.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/midiUtils"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/midiUtils

$(SYNTH_COMMON_ROOT)/modules/TeensyView/build/TeensyView.a:
	@echo "######## Building $(SYNTH_COMMON_ROOT)/modules/TeensyView"
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/TeensyView

clean_all:
	$(MAKE) -C $(REPO_ROOT)/bsp clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/FreeRTOS clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/logging clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/uCModules clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/SegmentVideo clean
	$(MAKE) -C $(SYNTH_COMMON_ROOT)/modules/midi47fx clean
