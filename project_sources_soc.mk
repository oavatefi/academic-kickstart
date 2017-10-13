# List common C source files here.
P1_SPECIFIC_FILES:=$(TUAM_TRUNK)/sw/vayu/P1_Config
project_path := $(shell dirname $(lastword $(MAKEFILE_LIST)))

FRAMEWORK_CSOURCES += \
	$(P1_SPECIFIC_FILES)/Daimler/bmp/bmp_images.c
FRAMEWORK_CXXSOURCES += \
	$(P1_SPECIFIC_FILES)/Daimler/p1_ovl/p1_ovlrefstaticoverlayinfo.cpp

PROJECT_FRAMEWORK_INCDIRS := $(P1_SPECIFIC_FILES)/Daimler/app_params

EXTRAINCDIRS += \
    $(PROJECT_FRAMEWORK_INCDIRS) \
    $(project_path) $(project_path)/daimler_src
