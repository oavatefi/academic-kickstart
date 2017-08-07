# Get the name of the first makefile in the list (without "makefile" itself).
# This name is taken as the default name of the build directory
MNAME:=$(basename $(firstword $(filter-out makefile Makefile,$(MAKEFILE_LIST))))

# Try to determine where this file is located.  If the caller did
# include /foo/... then extract the /foo/ so that common gets
# included transparently
common_root :=
ifneq ($(MAKEFILE_LIST),)
 common_root := $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
endif

include $(common_root)gmsl

# Target name
ifndef TARGET
	TARGET:=$(MNAME)
endif

# Object directory
ifndef OBJDIR
	OBJDIR:=$(MNAME)
endif

# Output directory
ifndef OUTDIR
	OUTDIR:=$(MNAME)
endif

# Create a string containing only a space for substitution.
# This works because "+=" seperates values by spaces.
space :=
space +=

# Functions
objdirsuffix=$(addsuffix $(2),$(addprefix $(OBJDIR)/,$(notdir $(basename $(1)))))
outdirsuffix=$(addsuffix $(2),$(addprefix $(OUTDIR)/,$(notdir $(basename $(1)))))
basenamelc=$(call lc,$(notdir $(basename $(1))))

# A template for defining unique object names. If there are source files with the same base names
# (e.g. test.c and test.asm or source files with the same names in different folders) they will
# usually collide in the object directory. This template creates object names of the form
# "basename", "basename_1", "basename_2" for sources with the same basename and stores them in the
# variables "OBJ_<sourcename>".
# Parameter: Source file name
define objname
 templist:=$$(filter $(call basenamelc,$(1)),$$(namelist))
 namelist+=$(call basenamelc,$(1))
 ifeq ($$(templist),)
  suffix:=
 else
  suffix:=_$$(words $$(templist))
 endif
 OBJ_$(1):=$(call basenamelc,$(1))$$(suffix)
endef

# A template for defining multiple sets of sources (e.g. CSRC, CSRC1 ...). For each set there is
# checked if the build flags are defined (if not, an error is reported). Unique object names for
# the source files are generated and added to the associated object list (e.g. COBJ, COBJ1 ...).
# The lists "SOURCES" and "OBJECTS" are created with all source/object files.
# Parameters:
#  $1 - Set suffix (usually a number like in "CSRC5" or empty)
#  $2 - Source list name (e.g. "CSOURCES" or "ASOURCES")
#  $3 - Object list name (e.g. "COBJECTS" or "AOBJECTS")
#  $4 - Source set name (e.g. "CSRC" or "ASRC")
#  $5 - Object set name (e.g. "COBJ" or "AOBJ")
#  $6 - Flags variable name (e.g. "CFLAGS" or "ASMFLAGS")
#  $7 - Object file extension (should be ".o" if you don't have serious reasons to change this)
define src_template
 $(2)+=$$($(4)$(1))
 SOURCES+=$$($(4)$(1))
 ifneq ($(3),)
  ifneq ($$($(4)$(1)),)
   ifndef $(6)$(1)
    $$(error $(6)$(1) not defined!)
   endif
   $$(foreach srcfile,$$($(4)$(1)),$$(eval $$(call objname,$$(srcfile))))
   $(5)$(1):=$$(call objdirsuffix,$$(foreach srcfile,$$($(4)$(1)),$$(OBJ_$$(srcfile))),$(7))
   $(3)+=$$($(5)$(1))
   OBJECTS+=$$($(5)$(1))
   $$($(5)$(1)): buildflags=$$($(6)$(1))
  endif
 endif
endef

# List of source directories
SOURCEDIRS=$(dir $(SOURCES))