#Build configuration for variant P4U_MAX_ANSI_C_DISCRETE_DAI

TARGET_ARCH=leo
TARGET_VARIANT_STRING:=2048k_192k
ENABLE_DEBUG = 1

DEFINES += \
	-D XAPPL_USE_HW_LEO_DISCRETE_DAI -D XAPPL_DAS_ON_ARM -D XP4UHOME_FEATURE \
	-D XBDA_ENABLE -D XECU_VARIANT_SVS \
	-D XMENU_BAR -D XUPA_MENUS_ENABLED \
	-D XVIEWS_ENABLED -D XP4U_ENABLED -D XAPPL_LOT_CNTRL_SUPP
	
include sources.mk
include base_build.mk
