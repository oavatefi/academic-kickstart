# Template of a toolchain configuration.
# TODO: rename the file to "config.mk" and adjust the settings to your toolchain. The default
# settings in this file are _not_ working!

# At first define a usable shell. If nothing is defined here, the default is "sh".
# Bash is the swiss army knife of the shells. Dash has nearly the same functionality but is much faster (cygwin environment).
SHELL:=dash

# TODO: Define programs and commands. For GHS compiler this may be:
#   CC:=ccppc
#   LD:=ccppc
#   MKDEPS:=gcc-3
#   AS:=asppc
PATH_T ?= //BIE-PVCSBS-01/Tool

REMOVE:=rm -f
MKDIR:=mkdir -p
SED:=sed
ECHO:=echo

ifeq ($(TARGET_VARIANT_STRING),)
$(error Application needs to define variable TARGET_VARIANT_STRING!)
endif

COMPILER_VERSION ?= 5.9.2.1
CC:=$(PATH_T)/WindRiverMPC/$(subst .,_,$(COMPILER_VERSION))_patched_Parkman_series_version/WIN32/bin/dcc.exe
LD:=$(PATH_T)/WindRiverMPC/$(subst .,_,$(COMPILER_VERSION))_patched_Parkman_series_version/WIN32/bin/dld.exe
MKDEPS:=gcc.exe
AS:=$(PATH_T)/WindRiverMPC/$(subst .,_,$(COMPILER_VERSION))_patched_Parkman_series_version/WIN32/bin/das.exe
# TODO: adjust the following programs if necessary.


# Compiler version check
# TODO: adjust the following variables to your compiler. E.g. for GHS compiler:
#   ccpath_dos:=$(shell $(WHICH) $(CC) | $(SED) -e 's%/cygdrive/\([a-z]\)%\1:%')
#   CCVER_REQUIRED:=Green Hills Software, MULTI v5.1.7 PPC
#   CCVER_CURRENT:=$(shell gversion $(ccpath_dos) | $(SED) -n -e '1s/.*: \(.*[^ ]\).*/\1/p')
CCVER_REQUIRED:=$(COMPILER_VERSION)
CCVER_CURRENT:=$(shell $(CC) -V | grep 'Rel ' | sed -e 's/.*Rel \(.*\)/\1/g')

# TODO: Specify the list of build goals and the list of file extension for those goals. The following
#   configuration is the default for MLB projects; the "exe" goal produces an .elf file, "lib"
#   produces a .a file and "obj" will create a .obj file. 
#
#   BUILD_GOALS:=exe lib obj
#   TARGET_EXT:=.elf .a .obj
BUILD_GOALS:=exe
TARGET_EXT:=.elf

TARGET_STRING = PPCE200Z4VEF:simple
CFLAGS = -t$(TARGET_STRING)
CFLAGS += -XO -Xinline=80 -Xunroll-size=80 -Xunroll=8 -Xenum-is-best -Xbss-common-off -Xforce-declarations \
	-Xforce-prototypes -Xnested-interrupts -Xintc-eoir=1 -Xsection-split -Xsmall-data=0x0 -Xsmall-const=0x0 \
	-Xdouble-error
CFLAGS += -ee1010 -ee1025 -ee1041 -ee1060 -ee1077 -ee1112 -ee1124 -ee1173 -ee1200 \
	-ee1201 -ee1202 -ee1230 -ee1232 -ee1234 -ee1235   -ee1242 -ee1275 -ee1304 -ee1362 -ee1383 \
	-ee1395 -ee1414 -ee1420 -ee1429 -ee1477 -ee1481 -ee1485 -ee1486 -ee1495 -ee1500   -ee1501 \
	-ee1504 -ee1521 -ee1527 -ee1544 -ee1546 -ee1556 -ee1561 -ee1562 -ee1584 -ee1585 -ee1586 \
	-ee1587 -ee1608 -ee1627 -ee1672 -ee1683 -ee1704 -ee1711 -ei2273 -ei2274 -ei5387 -ee4223 -ee4513 -ee1047 \
	-ee4167 -ee4549
	
ifdef ENABLE_DEBUG
CFLAGS += -g -Xoptimized-debug-off -Xlocals-on-stack
else
CFLAGS += -g3
endif

DEFINES += \
	-D XMCU_USE_16MHZ_CRYSTAL \
	-D XMCU_SYSCLK_120MHZ \
	-D XTMPL_CAN_USE_CHANNEL_1 \
	-D XTMPL_CAN_USE_STANDARD_ID \
	-D XTMPL_CAN_USE_TX_FIFO \
	-D XTMPL_CAN_TX_BUFFER_SIZE=50 \
	-D XTMPL_USE_SPI \
	-D XTMPL_USE_WATCHDOG \
	-D XTMPL_CAN_USE_TIMESTAMP \
	-D XTMPL_CAN1_BAUD_500K \
	-D XTMPL_CAN0_ENABLE \
	-D XTMPL_CAN1_ENABLE \
	-D XTMPL_CAN0_USE_INTERRUPT \
	-D XTMPL_CAN1_USE_INTERRUPT \
	-D PREEMPTIVE_SCHEDULER	\
	-DIVEC_USE_ETIMER_INTERRUPT \
	-D USE_FLOAT \
	-DXA2LEO_USE_VLE \
	-DTMPL_USE_SCAN \
	-DXA2LEO_USE_LEOPARD \
	-DTMPL_USE_LEO_2M_ROM \
	-DINTERRUPT_KEYWORD=__interrupt__ \
	-DCOMPILER_WR \
	-D AUTOSAR_OS_NOT_USED \
	-D USE_SW_VECTOR_MODE \
	-D CHWPL_USE_DSPI2 \
	-D IVEC_USE_SIUL_INTERRUPT 

EXTRAINCDIRS += \
	$(dir $(CC))/../../include \
	$(TMPLDIR)/ppc/header

ifdef ENABLE_ANSI_C
# Default ANSI C (option needed for support of inline word)
CFLAGS += -Xkeywords=0x4 -Xdialect-ansi
EXTRAINCDIRS += $(TMPLDIR)/ppc/dstd
else
# C99 dialect
CFLAGS += -Xlibc-new
EXTRAINCDIRS += $(dir $(CC))/../../include/cnew
endif
ASFLAGS = -t$(TARGET_STRING) -Dxvector_use_vle=1 -Dmcusize=2
LDFLAGS = -t$(TARGET_STRING) -Xstack-usage=0xf -m6 -lc -lm -limpl -L$(OUTDIR) -l:crt0_$(TARGET_ARCH)_core0_vle_WR.o
LINKFILE = $(TMPLDIR)/ppc/header/testswxx_$(TARGET_ARCH)_$(TARGET_VARIANT_STRING)_WR_no_reladdr.dld
ASRC += $(TMPLDIR)/ppc/header/crt0_$(TARGET_ARCH)_core0_vle_WR.s \
	$(TMPLDIR)/ppc/header/vector_$(TARGET_ARCH)_core0_WR.s

CFLAGS += $(DEFINES)

# How to run the compiler, assembler and linker
# TODO: adjust the following command lines to your toolchain. In each expression $(1) is replaced with the input file name(s)
#   and $(2) is replaced with the output file name. For each of your build goals there is one linker command line.
#   For GHS compiler you may use:
#
#   RUN_CC=$(CC) -c $(buildflags) $(INCLUDE) $(1) -o $(2)
#   RUN_AS=$(AS) $(buildflags) $(INCLUDE) $(1) -o $(2)
#   RUN_LD_exe=$(LD) $$(LINKFILE) $$(filter-out $$(DONTLINK),$$(1)) $$(LDFLAGS) -o $$(2)
#   RUN_LD_lib=$(LD) $$(filter-out $$(DONTLINK),$$(1)) -archive -o $$(2)
#   RUN_LD_obj=$(LD) $$(filter-out $$(DONTLINK),$$(1)) -relobj -nostartfiles -map -o $$(2)
ifdef USE_CCACHE
RUN_CC=perl ccache.pl --dep-file=$(2:.o=.d) -- $(CC) -c $(buildflags) $(INCLUDE) $(1) -o $(2)
else
RUN_CC=$(CC) -c $(buildflags) $(INCLUDE) $(1) -o $(2)
endif
RUN_AS=$(AS) $(buildflags) $(INCLUDE) -o $(2) $(1)
DONTLINK=$(OBJDIR)/crt0_$(TARGET_ARCH)_core0_vle_wr.o
RUN_LD_exe=$(LD) $$(LDFLAGS) -o $$(2) $$(filter-out $$(DONTLINK),$$(1)) $$(LINKFILE) > $$(call outdirsuffix,$$(TARGET),.map)

# Temp files created during build (deleted by "clean").
# TODO: enter the list created by your toolchain. E.g. for GHS toolchain:
#   TMPEXT_TARG:=.elf .map .dla .dle .dnm .graph .run .dba .a .obj
#   TMPEXT_C:=.lst .dbo
#   TMPEXT_ASM:=.lst .dbo
#   GARBAGE:=
TMPEXT_TARG:=.elf .map .dla .dle .dnm .graph .run .dba .a .obj
GARBAGE:= $(call outdirsuffix,$(TARGET),.srec)


# Add all module and extra directories to the include path list
INCLUDE = $(patsubst %,-I%,$(sort . $(SOURCEDIRS) $(EXTRAINCDIRS)))

# Flags for creating the dependency files.
# TODO: If you use the compiler of your toolchain then you have only to specify the option for
# creating dependencies here (-MD, -MM or similar). Don't forget to give the include path here.
# If you use another compiler (gcc) then you have also to specify default system include paths
# and macro definitions. E.g. for GHS toolchain using gcc for dependency generation:
#   ccpath:=$(shell $(WHICH) $(CC) | $(SED) -e 's%/[^/]*$$%%')
#   GENDEPFLAGS:=-MM -I$(ccpath)/ansi -I$(ccpath)/include $(INCLUDE)\
#       -D__EDG__ -D__ghs__ -D__ghs_asm -D__ghs_c_int__=int -D__ppc -D__PowerPC__ \
#       -D__vle__ -D__BIG_ENDIAN__ -D__interrupt__ -D__CHAR_BIT=8 -D__FUNCPTR_BIT=32 \
#       -D__INT_BIT=32 -D__LONG_BIT=32 -D__LLONG_BIT=64 -D__PTR_BIT=32 -D__REG_BIT=32 \
#       -D__SHRT_BIT=16 -D__WCHAR_BIT=16
ccpath:=$(dir $(CC))/../..
GENDEPFLAGS=-MM -I$(ccpath)/ansi -I$(ccpath)/include $(INCLUDE) $(DEFINES) -DUINT8_MAX=255U

# Define the default goal. If you use the sample configuration above then the following line
#   maps the default goal "all" to the "exe" goal.
#   all: exe
all: $(call outdirsuffix,$(TARGET),.srec)
$(call outdirsuffix,$(TARGET),.srec): exe
	@$(dir $(CC))/ddump -Rv $(call outdirsuffix,$(TARGET),$(TARGET_EXT)) -o $@
.PHONY: all

.PHONY: res
res: $(call outdirsuffix,$(TARGET),_analysemapfile.html)
ifeq ($(VERSION),)
	$(error Caller needs to define variable VERSION!)
endif
	@cat $(call outdirsuffix,$(TARGET),_analysemapfile.html) | $(PERL) res2xls.pl $(DOC_OUT_DIR)/DAS_MemoryConsumption.xls $(subst build_configs/leo_2m_,,$(TARGET)) $(VERSION)
	cp $(call outdirsuffix,$(TARGET),_analysemapfile.html)  $(DOC_OUT_DIR)/$(subst leo_2m,DAS,$(notdir $(call outdirsuffix,$(TARGET),_analysemapfile.html)))