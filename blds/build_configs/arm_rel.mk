#Build configuration for variant leo_2m_upit

TARGET_ARCH=arm
TARGET_VARIANT_STRING:=2048k_192k
COMPILE_DAPM_FUN = 1

include sources.mk
include base_build.mk
include application_recipes.mk