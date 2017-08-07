# Template for the build toolchain configuration. If you have a very simple configuration with just
# one build goal, you may define all toolchain configuration in this file. For the list of
# variables to define see TEMPLATE_toolchain.mk.
# You need to define a default build goal, e.g.:
#    all: $(call outdirsuffix, $(TARGET), .elf)
# This sample will create the file "debug.elf" in the output directory ("debug", if not overridden)
# if the build system was started with "make.bat debug".
#
# For more complicated toolchain configurations with multiple build goals per build configuration
# it is recommended to use this file only for common stuff and include the definitions for the
# toolchain. The configuration in this template supports all build goals specified in the variable
# BUILD_GOALS, the associated file extensions have to be listed in TARGET_EXT. Additionally it
# compares the currently used compiler (version string given in CCVER_CURRENT) with the required
# compiler of the project (CCVER_REQUIRED). If you don't need this version check then you may
# delete this section.

DOC_OUT_DIR = ../dassys_doc/

CUSTOM_RULES = include_paths.txt objects.txt

include toolchain_$(TARGET_ARCH).mk

# Definition of some helper functions
shorttarget=$(eval $1: $(call outdirsuffix,$(TARGET),$2))
setlinker=$(eval $(call outdirsuffix,$(TARGET),$2): RUN_LD=$(RUN_LD_$(1)))

# Check the used compiler
ifneq ($(CCVER_CURRENT),$(CCVER_REQUIRED))
define err_msg

  Required compiler version: $(CCVER_REQUIRED)
  Your compiler version: $(CCVER_CURRENT)
endef
$(error $(err_msg))
endif

# Define short build goals
$(call pairmap,shorttarget,$(BUILD_GOALS),$(TARGET_EXT))

# Set the linker command for different build goals
$(call pairmap,setlinker,$(BUILD_GOALS),$(TARGET_EXT))

.PHONY: $(BUILD_GOALS)

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),leo arm))
ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),leo))
GMPSPATH := $(DASDIR)/gmps
endif
#$(DASDIR)/serl/gen_out/serl_das_data_ids.h: $(DASDIR)/serl/gen_cfg.py \
	$(DAPMPATH)/dapm_typ.h \
	$(DAPMPATH)/TEMPLATE_dapm_pdf.h \
	$(DAPMPATH)/dapm_zpy.h \
	$(DIDHPATH)/didh_uls.h \
	$(DIDHPATH)/didh_odo.h \
	$(DIDHPATH)/didh_fun.h \
	$(FILAPATH)/filc/filc_zpy.h \
	$(MOPLPATH)/mopc/mopc_zpy.h \
	$(GMPSPATH)/gmpm/gmpm_zpy.h \
	$(VCTLPATH)/vctc/vctc_zpy.h \
	$(VCTLPATH)/vctc/vctc.h \
	$(OBJDPATH)/objc/objc_cam.h \
	$(OBJDPATH)/objc/objc_zpy.h \
	$(COLAPATH)/colc/colc_zpy.h \
	$(VDILPATH)/vdic/vdic_zpy.h \
	$(VMAPPATH)/vmac/vmac.h \
	$(VMAPPATH)/vmac/vmac_zpy.h \
	$(VMAPPATH)/vmac/vmac_lsr.h \
	$(DAPMPATH)/dapm_fun/dapm_prx.h
#	python $<
endif

ifeq ($(TARGET_ARCH),$(filter $(TARGET_ARCH),leo arm))
EXTRAINCDIRS += $(DASDIR) $(OUTDIR)
else
EXTRAINCDIRS += $(OUTDIR)
endif
#PREDEPS += $(DASDIR)/serl/gen_out/serl_das_data_ids.h