#Build configuration for variant leo_2m_upit

TARGET_ARCH=arm
TARGET_VARIANT_STRING:=2048k_192k
ENABLE_DEBUG = 1
COMPILE_DAPM_FUN = 1

include sources.mk
include base_build.mk
include application_recipes.mk