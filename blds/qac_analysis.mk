# Source files not to check.
# Define all 3rd party sources here, which you don't want to QA check (upper/lowercase has to be identical to CSRC..)

QAC_CHECK_DAPM_DIDH_ONLY ?= 0

ifeq ($(QAC_CHECK_DAPM_DIDH_ONLY),0)
QAC_IGNORE = $(filter-out $(DASDIR)/%.c,$(CSRC)) \
	$(VCTLPATH)/vpex/VPEXgen.c \
	$(VCTLPATH)/velc/velcctrl.c \
	$(VCTLPATH)/paco/pacoctrl.c
else
QAC_IGNORE = $(filter-out $(DAPMPATH)/%.c $(DIDHPATH)/%.c,$(CSRC))
endif
# Defined constants
# Keep in mind that source code sections may not be visible to QAC due to
# compiler conditionals (ifdefs). So define all constants like in your build
# configuration.
CDEFS = $(DEFINES)

# Additional include directories (e.g. path to compiler specific headers)
# EXTRAINCDIRS = $(COMPILERPATH)/ansi

# Application name (e.g. AMLB)
APPL = DAS_3_2

# QAC messages to disable (please comment all disabled messages to make Q people happy)
# QAC_DISABLE_MESSAGES:=1011,5900,3344,781,857
QAC_DISABLE_MESSAGES:=

# Include the basic qac makefile, which defines all build rules
include base_qac.mk