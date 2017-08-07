# Try to determine where this file is located.  If the caller did
# include /foo/... then extract the /foo/ so that common gets
# included transparently
common_root :=
ifneq ($(MAKEFILE_LIST),)
 common_root := $(dir $(word $(words $(MAKEFILE_LIST)),$(MAKEFILE_LIST)))
endif

include $(common_root)config_qac.mk
include $(common_root)common.mk

# Create some sets of source files (CSRC, CSRC1 .. CSRC9, ASRC, ASRC1 .. ASRC9)
$(eval $(call src_template,,CSOURCES,,CSRC,,,))
$(foreach srcset,1 2 3 4 5 6 7 8 9,$(eval $(call src_template,$(srcset),CSOURCES,,CSRC,,,)))
# The assembler sources are not really needed for QAC check, but the folders are used for include path detection.
$(eval $(call src_template,,ASOURCES,,ASRC,,,))
$(foreach srcset,1 2 3 4 5 6 7 8 9,$(eval $(call src_template,$(srcset),ASOURCES,,ASRC,,,)))

ALLG_OBJ:=$(filter-out $(QAC_IGNORE),$(CSOURCES))
QACOUTPATH:=$(OBJDIR)

# Default target.
all: qac

# Clean project.
clean: qacclean

# Include the QAC makefile
include $(QAC_MODULE_PATH)/qacm_gnu.mk

# Listing of phony targets.
.PHONY : all clean
