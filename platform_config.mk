# platform configuration (by default everything is switched on for *_DIS_* variables and off for _EN_ variables in the platform

project_path := $(shell dirname $(lastword $(MAKEFILE_LIST)))

PROJECT_DIST_PATH := $(project_path)/dist
PLATFORM_ECU_TYPE := MFAS
#if ParkMan is slected the GPS data will not be sent as it's impelemented in the CAN (GPS data is used in the cloud Parking)
#PLATFORM_ECU_TYPE := PARKMAN

PROJECT_NAME := DAIMLER

# PLATFORM_DIS_DAS_ON_ARM := 1
# PLATFORM_DIS_P4UHOME_FEATURE := 1
PLATFORM_DIS_BDA := 1
# PLATFORM_DIS_ECU_VARIANT_SVS := 1
# PLATFORM_DIS_MENU_BAR := 1
# PLATFORM_DIS_UPA_MENUS_ENABLED := 1
# PLATFORM_DIS_VIEWS := 1 
# PLATFORM_DIS_P4U := 1
# PLATFORM_DIS_LOT_CNTRL_SUPP := 1
# PLATFORM_DIS_REMOTE_P4U_SUPP := 1