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
ifneq ($(PATH_T),)
override PATH_T := $(subst \,/,$(PATH_T))
else
override PATH_T := //BIE-PVCSBS-01/Tool
endif

REMOVE:=rm -rf
MKDIR:=mkdir -p
SED:=sed
ECHO:=echo

ifeq ($(ARM_ENV_PATH),)
$(error Application needs to define variable ARM_ENV_PATH!)
endif

COMPILER_VERSION ?= 4.7.4
COMPILER_PATH = $(PATH_T)/gcc/gcc-arm-none-eabi-4_7-2013q3/bin
CC:=$(COMPILER_PATH)/arm-none-eabi-gcc.exe
CXX:=$(COMPILER_PATH)/arm-none-eabi-g++.exe
LD:=$(COMPILER_PATH)/arm-none-eabi-gcc.exe
MKDEPS:=$(COMPILER_PATH)/arm-none-eabi-g++.exe
AS:=$(COMPILER_PATH)/arm-none-eabi-as.exe
# TODO: adjust the following programs if necessary.

# Compiler version check
# TODO: adjust the following variables to your compiler. E.g. for GHS compiler:
#   ccpath_dos:=$(shell $(WHICH) $(CC) | $(SED) -e 's%/cygdrive/\([a-z]\)%\1:%')
#   CCVER_REQUIRED:=Green Hills Software, MULTI v5.1.7 PPC
#   CCVER_CURRENT:=$(shell gversion $(ccpath_dos) | $(SED) -n -e '1s/.*: \(.*[^ ]\).*/\1/p')
CCVER_REQUIRED = $(COMPILER_VERSION)
CCVER_CURRENT = $(shell $(CC) -dumpversion)

include ../platform/make/download_svn_configure_ti.mk
# TODO: Specify the list of build goals and the list of file extension for those goals. The following
#   configuration is the default for MLB projects; the "exe" goal produces an .elf file, "lib"
#   produces a .a file and "obj" will create a .obj file. 
#
#   BUILD_GOALS:=exe lib obj
#   TARGET_EXT:=.elf .a .obj
BUILD_GOALS:=exe
TARGET_EXT:=.out
DEFINES += -Dxdc_target_name__=A15F -Dxdc_target_types__=gnu/targets/arm/std.h -DTDA2XX_FAMILY_BUILD
EXTRAINCDIRS += $(addsuffix /packages,$(TI_DEPS))
COMMON_LANG_FLAGS = $(DEFINES) -mcpu=cortex-a15 -mfpu=neon-vfpv4 -mtune=cortex-a15 -marm -mfloat-abi=hard -fno-exceptions -specs=rdimon.specs -ffast-math -g
ifdef ENABLE_DEBUG
COMMON_LANG_FLAGS += -O1
else
COMMON_LANG_FLAGS += -Ofast
endif
CXXFLAGS += $(COMMON_LANG_FLAGS)
CFLAGS = $(COMMON_LANG_FLAGS) -std=gnu99
ASFLAGS =
LDFLAGS = -mfloat-abi=hard -fno-exceptions -g -Wall -v -Wl,-Map,$(call outdirsuffix,$(TARGET),.map) -nostartfiles -Wl,--gc-sections -Wl,--start-group
LINKFILE = $(OUTDIR)/configPkg/linker.cmd

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
RUN_CC=$(CC) -c $(buildflags) @include_paths.txt $(1) -o $(2)
RUN_CXX=$(CXX) -c $(buildflags) @include_paths.txt $(1) -o $(2)
RUN_AS=$(AS) $(buildflags) @include_paths.txt -o $(2) $(1)
DONTLINK=
LINK_LIBS = $(addprefix $(DIST_PKG_DIR)/, \
		$(shell awk '{if($$5 == "ALL" || $$5 == "a15") printf("%s$(DIST_PKG_NAME_SFX)-%s-a15/lib/lib%s$(DIST_PKG_NAME_SFX).a\\n",$$1,$$2,$$1);}' $(DIST_PKG_DEP_FILE)))
RUN_LD_exe=$(LD) $$(LDFLAGS) -o $$(2) -Wl,@objects.txt $(LINK_LIBS) -lstdc++ -Wl,-T$$(LINKFILE) -Wl,--end-group -lgcc -lc -lm -lrdimon

# Temp files created during build (deleted by "clean").
# TODO: enter the list created by your toolchain. E.g. for GHS toolchain:
#   TMPEXT_TARG:=.elf .map .dla .dle .dnm .graph .run .dba .a .obj
#   TMPEXT_C:=.lst .dbo
#   TMPEXT_ASM:=.lst .dbo
#   GARBAGE:=
TMPEXT_TARG:=.elf .map .dla .dle .dnm .graph .run .dba .a .obj .out
GARBAGE:= $(PATH_PLATOFRM)/package.mak \
	$(PATH_PLATOFRM)/.[A-Za-z]* \
	$(PATH_PLATOFRM)/Platform.xdc \
	$(PATH_PLATOFRM)/package \
	$(OUTDIR)/configPkg \
	$(TI_CFG_PATH)/src \
	$(call outdirsuffix,tmp_pdf,.h) \
	$(call outdirsuffix,$(TARGET),.bin) \
	$(call outdirsuffix,*,.su)

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
ccpath:=$(dir $(CC))
GENDEPFLAGS=-MM -mfpu=neon-vfpv4 -mfloat-abi=hard -I$(ccpath)/../arm-none-eabi/include $(DEFINES) @include_paths.txt

# Define the default goal. If you use the sample configuration above then the following line
#   maps the default goal "all" to the "exe" goal.
#   all: exe
.PHONY: all
all: $(call outdirsuffix,$(TARGET),.bin)

PREDEPS := $(PREDEPS) $(OUTDIR)/configPkg

$(DOC_OUT_DIR)/su_arm.log: $(OBJECTS)
	$(PERL) avstack.pl --objdump=$(ccpath)/arm-none-eabi-objdump.exe $(OBJECTS) > $@
	
.PHONY: res
res: $(DOC_OUT_DIR)/su_arm.log 
ifeq ($(VERSION),)
	$(error Caller needs to define variable VERSION!)
endif
	amap -f $(call outdirsuffix,$(TARGET),.map)
	{ echo "STACK USAGE"; cat $< | sed -e 's/[> \t]\+\([^ \t]\+\) *\([0-9]\+\)[ \t]\+\([0-9]\+\).*/\1\t\3\t\2/g'; cat $(call outdirsuffix,$(TARGET),.map.all) | $(PERL) amapfilter.pl; } | $(PERL) res2xls.pl $(DOC_OUT_DIR)/DAS_MemoryConsumption.xls $(subst build_configs/,,$(TARGET)) $(VERSION)
	cp $(call outdirsuffix,$(TARGET),.map.all) $(DOC_OUT_DIR)