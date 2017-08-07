# Template for QAC specific configuration.
# TODO: rename this file to "config_qac.mk". Adjust the settings to your toolchain.

# At first define a usable shell. If nothing is defined here, the default is "sh".
# Bash is the swiss army knife of the shells. Dash has nearly the same functionality but is much faster (cygwin environment).
SHELL:=dash

PATH_T ?= D:/Tool
# Define programs and commands.
CC:=$(PATH_T)/WindRiverMPC/5_9_2_1/WIN32/bin/dcc.exe
SED:=sed
WHICH:=which
QACEXE:=qac
PYTHONEXE:=$(PATH_T)/Python/2_4/python.exe
QAC_MCM_EXEC:=chk_valeo_cm.exe

# QAC configuration
COMPILERPATH:=$(PATH_T)/WindRiverMPC/5_9_2_1/WIN32/bin/
PYTHON:=$(PATH_T)/Python/2_4/python.exe
QACBIN:="$(PATH_T)/PRQA/QAC-7.1/"
QAC_MCM_PATH:=$(patsubst %/bin,%/m2cm,$(QACBIN))
QAC_REPORT_TPL:=$(APPL)_AnalysisReportTpl.xhtml

QAC_SUPPRESS := $(EXTRAINCDIRS)
QAC_MODULE_PATH = $(TMPLDIR)gen/qacm
QAC_OPT_S = $(QAC_MODULE_PATH)/qacm_m2cm_valeo.p_s
QAC_OPT_A = $(QAC_MODULE_PATH)/qacm_m2cm.p_a
# Give a compiler personality appropriate to your compiler
QAC_OPT_C = $(QAC_MODULE_PATH)/qacm_m2cm_windriver.p_c
QAC_OPT_CONFIG_C = -fi $(APPL)_qac.h -D__QAC__
QAC_OPT_CONFIG_S = -no 3450
QAC_OPT_CONFIG_A = $(patsubst %,-i %,$(sort . $(SOURCEDIRS) $(EXTRAINCDIRS))) $(patsubst %,-q %,$(QAC_SUPPRESS)) $(CDEFS)
QAC_MAX_WARNINGS = 1000
QAC_VERBOSE =
QAC_VERSION = 7.1
