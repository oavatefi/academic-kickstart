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
REMOVE:=rm -rf
MKDIR:=mkdir -p
SED:=sed
ECHO:=echo

COMPILER_VERSION ?= 5.3.0

ifneq ($(PATH_T),)
override PATH_T := $(subst \,/,$(PATH_T))
else
override PATH_T := //BIE-PVCSBS-01/Tool
endif

CC:=$(PATH_T)/gcc/MinGW/bin/gcc.exe
CXX:=$(PATH_T)/gcc/MinGW/bin/g++.exe
LD:=$(PATH_T)/gcc/MinGW/bin/g++.exe
MKDEPS:=$(PATH_T)/gcc/MinGW/bin/g++.exe
AS:=$(PATH_T)/gcc/MinGW/bin/as.exe
# TODO: adjust the following programs if necessary.

# Compiler version check
# TODO: adjust the following variables to your compiler. E.g. for GHS compiler:
#   ccpath_dos:=$(shell $(WHICH) $(CC) | $(SED) -e 's%/cygdrive/\([a-z]\)%\1:%')
#   CCVER_REQUIRED:=Green Hills Software, MULTI v5.1.7 PPC
#   CCVER_CURRENT:=$(shell gversion $(ccpath_dos) | $(SED) -n -e '1s/.*: \(.*[^ ]\).*/\1/p')
CCVER_REQUIRED = $(COMPILER_VERSION)
CCVER_CURRENT = $(shell $(CC) -dumpversion)

# TODO: Specify the list of build goals and the list of file extension for those goals. The following
#   configuration is the default for MLB projects; the "exe" goal produces an .elf file, "lib"
#   produces a .a file and "obj" will create a .obj file. 
#
#   BUILD_GOALS:=exe lib obj
#   TARGET_EXT:=.elf .a .obj

BUILD_GOALS:=exe
TARGET_EXT:=.exe
CFLAGS = $(DEFINES) -Wall
CXXFLAGS = -std=c++11 -std=gnu++11 
ifdef ENABLE_DEBUG
CFLAGS += -g -O0
else
CFLAGS += -g -Ofast
endif
CXXFLAGS += $(CFLAGS)
ASFLAGS =
LDFLAGS = -static
LINKFILE =

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
RUN_CC=$(CC) -c $(buildflags) $(INCLUDE) $(1) -o $(2)
RUN_CXX=$(CXX) -c $(buildflags) $(INCLUDE) $(1) -o $(2)
RUN_AS=$(AS) $(buildflags) $(INCLUDE) -o $(2) $(1)
DONTLINK=
RUN_LD_exe=$(LD) -o $$(2) $(LDFLAGS) $$(filter-out $$(DONTLINK),$$(1)) -lwsock32 $(LIBS)

# Temp files created during build (deleted by "clean").
# TODO: enter the list created by your toolchain. E.g. for GHS toolchain:
#   TMPEXT_TARG:=.elf .map .dla .dle .dnm .graph .run .dba .a .obj
#   TMPEXT_C:=.lst .dbo
#   TMPEXT_ASM:=.lst .dbo
#   GARBAGE:=
TMPEXT_TARG:=.elf .map .dla .dle .dnm .graph .run .dba .a .obj .out
GARBAGE:=
GARBAGE:=

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
GENDEPFLAGS= -std=c++11 -std=gnu++11 -MM -I$(ccpath)/../arm-none-eabi/include $(INCLUDE) $(DEFINES)
# Define the default goal. If you use the sample configuration above then the following line
#   maps the default goal "all" to the "exe" goal.
#   all: exe
.PHONY: all
all: $(call outdirsuffix,$(TARGET),.srec)
$(call outdirsuffix,$(TARGET),.srec): exe
	@$(PATH_T)/gcc/MinGW/bin/objcopy -S -I binary -O srec $(call outdirsuffix,$(TARGET),$(TARGET_EXT)) $@