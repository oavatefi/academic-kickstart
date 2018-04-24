# platform configuration (by default everything is switched on for *_DIS_* variables and off for _EN_ variables in the platform

project_path := $(shell dirname $(lastword $(MAKEFILE_LIST)))

PROJECT_DIST_PATH := $(project_path)/dist
PLATFORM_ECU_TYPE := MFAS
#PLATFORM_ECU_TYPE := PARKMAN

PROJECT_NAME := DAIMLER

PROJECT_VCON_PATH := $(project_path)/tool/vcon/cfgd/
PROJECT_VCON_CAR_VARIANTS := Bietigheim_Br_213_s_ml Bietigheim_Br213_lb_vs_117 \
	Bietigheim_Car_4x4_AMG_Line Tuam_Daimler
    
PROJECT_VCON_XML := \
	$(PROJECT_VCON_PATH)/cfgd.xml
PROJECT_VCON_DB_XML := \
	$(PROJECT_VCON_PATH)/CFG/cfgd_CFG.xml


#PLATFORM_DIS_BDA_HZTR_COUPLING := 1
PLATFORM_DIS_REMOTE_ULS := 1
PLATFORM_DIS_DAS_ON_ARM := 1
PLATFORM_DIS_P4UHOME_FEATURE := 1
# PLATFORM_DIS_BDA := 1
# PLATFORM_DIS_ECU_VARIANT_SVS := 1
# PLATFORM_DIS_MENU_BAR := 1
# PLATFORM_DIS_UPA_MENUS_ENABLED := 1
# PLATFORM_DIS_VIEWS := 1 
# PLATFORM_DIS_P4U := 1
# PLATFORM_DIS_LOT_CNTRL_SUPP := 1
# PLATFORM_DIS_REMOTE_P4U_SUPP := 1
