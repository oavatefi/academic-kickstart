# List common C source files here.

project_path := $(shell dirname $(lastword $(MAKEFILE_LIST)))

CSRC := \
	$(project_path)/daimler_src/host/comh/comh.c \
	$(project_path)/daimler_src/host/comh/comh_can.c \
	$(project_path)/daimler_src/host/comh/comh_ble.c \
	$(project_path)/daimler_src/host/STMH_CUS/stmh_cus.c \
	$(project_path)/daimler_src/host/BRKH_CUS/brkh_cus.c \
	$(project_path)/daimler_src/host/BRKH_CUS/brkh_cus_c.c \
	$(project_path)/daimler_src/host/comh/E2E/E2E_platform.c

EXTRAINCDIRS := $(project_path)/daimler_src/host/comh/E2E/include $(project_path) $(project_path)/daimler_src