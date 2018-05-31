# List common C source files here.

project_path := $(shell dirname $(lastword $(MAKEFILE_LIST)))

ifeq ($(PLATFORM_ECU_TYPE),ICAM)
CSRC := \
	$(project_path)/daimler_src/host/comh/comh_can/comh.c \
	$(project_path)/daimler_src/host/comh/comh_can/arch/linux/comh_can.c\
	$(project_path)/daimler_src/host/comh/comh_can/comh_can_common.c
endif

ifeq ($(PLATFORM_ECU_TYPE),MFAS)
CSRC := \
	$(project_path)/daimler_src/host/comh/comh_can/comh.c \
	$(project_path)/daimler_src/host/comh/comh_can/arch/ppc/comh_can.c\
	$(project_path)/daimler_src/host/comh/comh_can/comh_can_common.c
endif

ifeq ($(PLATFORM_ECU_TYPE),PARKMAN)
CSRC := \
	$(project_path)/daimler_src/host/comh/comh_fr/comh.c
endif

CSRC += \
	$(project_path)/daimler_src/host/STMH_CUS/stmh_cus.c \
	$(project_path)/daimler_src/host/BRKH_CUS/brkh_cus.c \
	$(project_path)/daimler_src/host/BRKH_CUS/brkh_cus_c.c \
	$(project_path)/daimler_src/host/comh/E2E/E2E_platform.c \
	$(project_path)/daimler_src/host/comh/comh_ble.c

EXTRAINCDIRS := $(project_path)/daimler_src/host/comh/E2E/include \
	$(project_path) $(project_path)/daimler_src