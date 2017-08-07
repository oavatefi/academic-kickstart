# List common C source files here.

CSRC += \
	../daimler_src/host/comh/comh.c \
	../daimler_src/host/comh/comh_can.c \
	../daimler_src/host/comh/comh_ble.c \
	../daimler_src/host/STMH_CUS/stmh_cus.c \
	../daimler_src/host/BRKH_CUS/brkh_cus.c \
	../daimler_src/host/BRKH_CUS/brkh_cus_c.c \
	../daimler_src/host/comh/E2E/E2E_platform.c

EXTRAINCDIRS += ../daimler_src/host/comh/E2E/include .. ../daimler_src