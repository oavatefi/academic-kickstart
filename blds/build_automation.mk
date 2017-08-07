#  /******************************************************************************/
#  /*                                                                            */
#  /*   Valeo Schalter und Sensoren GmbH                                         */
#  /*   Laiernstrasse 12                                                         */
#  /*   74321 Bietigheim-Bissingen                                               */
#  /*                                                                            */
#  /*   All rights reserved. Distribution or duplication without previous        */
#  /*   written agreement of the owner prohibited.                               */
#  /*                                                                            */
#  /******************************************************************************/

#  /** \file zdas_build_automation_config.mk
#   *
#   * Contains the build configurations of the DAS_3_1 test environment. It sets the defines depending on the
#   * defined configuration.
#   *
#   * 
#   * <table border:= 1 "0" cellspacing="0" cellpadding="0">
#   * <tr> <td> Source:   </td> <td> zdas_build_automation_config.mk                  </td></tr>
#   * <tr> <td> Revision: </td> <td> 1.22.1.5                  </td></tr>
#   * <tr> <td> Status:   </td> <td> ACCEPTED                  </td></tr>
#   * <tr> <td> Author:   </td> <td> T. Seifert, CDV-E-SW-P1  </td></tr>
#   * <tr> <td> Date:     </td> <td> 10-OCT-2014 14:50:31                </td></tr>
#   * </table>
#   * 
#   * \n
#   * <table border:= 1 "0" cellspacing="0" cellpadding="0">
#   * <tr> <td> COMPONENT: </td> <td> ZDAS               </td></tr>
#   * <tr> <td> SCOPE:     </td> <td> <Public>      </td></tr>
#   * <tr> <td> TARGET:    </td> <td> <Freescale/ST MPC56xx>  </td></tr>
#   * </table>
#   */
DEFINES += -D ZDAPM_BUILD_AUTOMATION

#  /******************************************************************************/
#  /*         Configurations of defined function variants                        */
#  /******************************************************************************/

#maximal build configuration for P4U
ifneq ($(BA_CONF_MAX_P4U),)
BA_EN_PROD_UPA := 1
BA_EN_PROD_P4U := 1
#BA_EN_PROD_BSD := 1                       # BSD causes config failed
BA_EN_FUNC_HAPTIC:= 1
BA_EN_VCTM_STEERING_TYPE_ANGLE := 1
#BA_EN_VCTM_STEERING_TYPE_TORQUE := 1      # angle is default
BA_EN_FUNC_BRAKE := 1
BA_EN_FUNC_PARK_PARA_BW := 1
#BA_EN_FUNC_PARK_PARA_FW := 1              # not implemented yet
BA_EN_FUNC_PARK_PERP_BW := 1
BA_EN_FUNC_PARK_PERP_FW := 1
BA_EN_FUNC_PARK_OUT_PARA_FW := 1
BA_EN_FUNC_PARK_OUT_PERP_FW := 1
BA_EN_FUNC_PARK_OUT_PERP_BW := 1
BA_EN_FUNC_DRIVE_EXTERN_GOAL := 1
BA_EN_FUNC_INTERNAL_ODOMETRY := 1
#BA_EN_FUNC_EXTERNAL_ODOMETRY := 1
BA_EN_ENABLE_RUNTIME_MEASUR := 1
BA_EN_FUNC_EXPLORATION := 1
BA_EN_FUNC_P4U_HOME := 1
BA_EN_FUNC_PATH_FOLLOW := 1
BA_EN_FUNC_DYN_VEL_PROFILE := 1
#BA_EN_KOOL := 1
BA_EN_GMPS := 1
BA_EN_CAM_FOR_STAT_MAP := 1
BA_EN_CAM_FOR_DYN_MAP := 1
BA_EN_CALC_DTC_ON_DYNAMIC_OBJ := 1 
BA_EN_CAM_FOR_LINE_MAP := 1
BA_EN_LAS_FOR_STAT_MAP := 1
BA_EN_RAD_FOR_STAT_MAP := 1
BA_EN_STOCHASTIC_MAP := 1
BA_EN_ENABLE_ERRT_SUPPORT := 1
BA_EN_FUNC_LOT_CTRL := 1
BA_EN_DAPM_TSK_EXEC_DAPM_VCT := 1
BA_EN_DAPM_TSK_EXEC_DAPM_ULS := 1
BA_EN_DAPM_TSK_EXEC_DAPM_FUN := 1
BA_EN_DAPM_SEND_ZPY_DATA := 1
BA_EN_DAPM_ENABLE_ASYNC_ULS := 1
BA_EN_DAPM_ENABLE_CURBLIFTER := 1
endif

#maximal build configuration for UPA
ifneq ($(BA_CONF_MAX_UPA),)
BA_EN_PROD_UPA := 1
#BA_EN_PROD_P4U := 1                       # not relevant for UPA
#BA_EN_PROD_BSD := 1                       # not relevant for UPA
#BA_EN_FUNC_HAPTIC:= 1                     # not relevant for UPA
#BA_EN_VCTM_STEERING_TYPE_ANGLE := 1       # not relevant for UPA
#BA_EN_VCTM_STEERING_TYPE_TORQUE := 1      # angle is default
#BA_EN_FUNC_BRAKE := 1                     # not relevant for UPA
#BA_EN_FUNC_PARK_PARA_BW := 1              # not relevant for UPA
#BA_EN_FUNC_PARK_PARA_FW := 1              # not implemented yet
#BA_EN_FUNC_PARK_PERP_BW := 1              # not relevant for UPA
#BA_EN_FUNC_PARK_PERP_FW := 1              # not relevant for UPA
#BA_EN_FUNC_PARK_OUT_PARA_FW := 1          # not relevant for UPA
#BA_EN_FUNC_PARK_OUT_PERP_FW := 1          # not relevant for UPA
#BA_EN_FUNC_PARK_OUT_PERP_BW := 1 		   # not relevant for UPA
#BA_EN_FUNC_DRIVE_EXTERN_GOAL := 1         # not relevant for UPA
#BA_EN_FUNC_EXTERNAL_ODOMETRY := 1
#BA_EN_KOOL := 1
#BA_EN_GMPS := 1
BA_EN_ENABLE_RUNTIME_MEASUR := 1
BA_EN_FUNC_INTERNAL_ODOMETRY := 1
#BA_EN_FUNC_EXPLORATION := 1               # not relevant for UPA
#BA_EN_FUNC_P4U_HOME := 1
#BA_EN_FUNC_PATH_FOLLOW := 1
#BA_EN_FUNC_DYN_VEL_PROFILE := 1           # not relevant for UPA
#BA_EN_CAM_FOR_STAT_MAP := 1
#BA_EN_CAM_FOR_DYN_MAP := 1
#BA_EN_CALC_DTC_ON_DYNAMIC_OBJ := 1 
#BA_EN_CAM_FOR_LINE_MAP := 1
#BA_EN_LAS_FOR_STAT_MAP := 1
#BA_EN_RAD_FOR_STAT_MAP := 1
BA_EN_STOCHASTIC_MAP := 1
BA_EN_ENABLE_ERRT_SUPPORT := 1
#BA_EN_FUNC_LOT_CTRL := 1                  # not relevant for UPA
BA_EN_DAPM_TSK_EXEC_DAPM_ULS := 1
BA_EN_DAPM_TSK_EXEC_DAPM_FUN := 1
BA_EN_DAPM_SEND_ZPY_DATA := 1
BA_EN_DAPM_ENABLE_ASYNC_ULS := 1
#BA_EN_DAPM_ENABLE_CURBLIFTER := 1         # not relevant for UPA
endif

ifeq ($(BA_MAX_SENSORS),)
BA_MAX_SENSORS = 6
endif
ifeq ($(BA_NUM_SENSORS_FRONT),)
BA_NUM_SENSORS_FRONT = 6
endif
ifeq ($(BA_NUM_SENSORS_REAR),)
BA_NUM_SENSORS_REAR = 6
endif

ifeq ($(BA_NUM_SECTORS_FRONT),)
BA_NUM_SECTORS_FRONT = 4
endif
ifeq ($(BA_NUM_SECTORS_REAR),)
BA_NUM_SECTORS_REAR = 4
endif
ifeq ($(BA_NUM_SECTORS_SIDE),)
BA_NUM_SECTORS_SIDE = 4
endif

#  /******************************************************************************/
#  /*               Disable switches for each configuraiton option               */
#  /******************************************************************************/

ifneq ($(BA_USE_ANSI_C),)
ENABLE_ANSI_C = TRUE
endif

#  /******************************************************************************/
#  /*               Disable switches for each configuraiton option               */
#  /******************************************************************************/
ifneq ($(BA_DIS_PROD_BSD),)
BA_EN_PROD_BSD :=
endif

ifneq ($(BA_DIS_FUNC_HAPTIC),)
BA_EN_FUNC_HAPTIC :=
endif

ifneq ($(BA_DIS_VCTM_STEERING_TYPE_ANGLE),)
BA_EN_VCTM_STEERING_TYPE_ANGLE :=
endif

ifneq ($(BA_DIS_VCTM_STEERING_TYPE_TORQUE),)
BA_EN_VCTM_STEERING_TYPE_TORQUE :=
endif

ifneq ($(BA_DIS_FUNC_BRAKE),)
BA_EN_FUNC_BRAKE :=
endif

ifneq ($(BA_DIS_FUNC_PARK_PARA_BW),)
BA_EN_FUNC_PARK_PARA_BW :=
endif

ifneq ($(BA_DIS_FUNC_PARK_PARA_FW),)
BA_EN_FUNC_PARK_PARA_FW :=
endif

ifneq ($(BA_DIS_FUNC_PARK_PERP_BW),)
BA_EN_FUNC_PARK_PERP_BW :=
endif

ifneq ($(BA_DIS_FUNC_PARK_PERP_FW),)
BA_EN_FUNC_PARK_PERP_FW :=
endif

ifneq ($(BA_DIS_FUNC_PARK_OUT_PARA_FW),)
BA_EN_FUNC_PARK_OUT_PARA_FW :=
endif

ifneq ($(BA_DIS_FUNC_PARK_OUT_PERP_BW),)
BA_EN_FUNC_PARK_OUT_PERP_BW :=
endif

ifneq ($(BA_DIS_FUNC_PARK_OUT_PERP_FW),)
BA_EN_FUNC_PARK_OUT_PERP_FW :=
endif

ifneq ($(BA_DIS_ENABLE_RUNTIME_MEASUR),)
BA_EN_ENABLE_RUNTIME_MEASUR :=
endif

ifneq ($(BA_DIS_FUNC_DRIVE_EXTERN_GOAL),)
BA_EN_FUNC_DRIVE_EXTERN_GOAL :=
endif

ifneq ($(BA_DIS_FUNC_EXPLORATION),)
BA_EN_FUNC_EXPLORATION :=
endif

ifneq ($(BA_DIS_FUNC_P4U_HOME),)
BA_EN_FUNC_P4U_HOME :=
endif

ifneq ($(BA_DIS_FUNC_PATH_FOLLOW),)
BA_EN_FUNC_PATH_FOLLOW :=
endif

ifneq ($(BA_DIS_FUNC_DYN_VEL_PROFILE),)
BA_EN_FUNC_DYN_VEL_PROFILE :=
endif

ifneq ($(BA_DIS_CAM_FOR_STAT_MAP),)
BA_EN_CAM_FOR_STAT_MAP :=
endif
ifneq ($(BA_DIS_CAM_FOR_DYN_MAP),)
BA_EN_CAM_FOR_DYN_MAP :=
endif

ifneq ($(BA_DIS_CAM_FOR_LINE_MAP),)
BA_EN_CAM_FOR_LINE_MAP :=
endif
ifneq ($(BA_DIS_LAS_FOR_STAT_MAP),)
BA_EN_LAS_FOR_STAT_MAP :=
endif
ifneq ($(BA_DIS_RAD_FOR_STAT_MAP),)
BA_EN_RAD_FOR_STAT_MAP :=
endif

ifneq ($(BA_DIS_CALC_DTC_ON_DYNAMIC_OBJ),)
BA_EN_CALC_DTC_ON_DYNAMIC_OBJ :=
endif

ifneq ($(BA_DIS_ENABLE_ERRT_SUPPORT),)
BA_EN_ENABLE_ERRT_SUPPORT :=
endif

ifneq ($(BA_DIS_FUNC_EXTERNAL_ODOMETRY),)
BA_EN_FUNC_EXTERNAL_ODOMETRY :=
endif

ifneq ($(BA_DIS_FUNC_INTERNAL_ODOMETRY),)
BA_EN_FUNC_INTERNAL_ODOMETRY :=
endif

ifneq ($(BA_DIS_FUNC_LOT_CTRL),)
BA_EN_FUNC_LOT_CTRL :=
endif

ifneq ($(BA_DIS_STOCHASTIC_MAP),)
BA_EN_STOCHASTIC_MAP :=
endif

ifneq ($(BA_DIS_KOOL),)
BA_EN_KOOL :=
endif

ifneq ($(BA_DIS_GMPS),)
BA_EN_GMPS :=
endif

ifneq ($(BA_DIS_DAPM_TSK_EXEC_DAPM_ULS),)
BA_EN_DAPM_TSK_EXEC_DAPM_ULS :=
endif
ifneq ($(BA_DIS_DAPM_TSK_EXEC_DAPM_FUN),)
BA_EN_DAPM_TSK_EXEC_DAPM_FUN :=
endif
ifneq ($(BA_DIS_DAPM_TSK_EXEC_DAPM_VCT),)
BA_EN_DAPM_TSK_EXEC_DAPM_VCT :=
endif

ifneq ($(BA_DIS_DAPM_SEND_ZPY_DATA),)
BA_EN_DAPM_SEND_ZPY_DATA :=
endif

ifneq ($(BA_DIS_DAPM_ENABLE_ASYNC_ULS),)
BA_EN_DAPM_ENABLE_ASYNC_ULS :=
endif

ifneq ($(BA_DIS_DAPM_ENABLE_CURBLIFTER),)
BA_EN_DAPM_ENABLE_CURBLIFTER :=
endif

#  /******************************************************************************/
#  /*                         DAPM-Configuration                                 */
#  /******************************************************************************/
ifneq ($(BA_EN_DAPM_ENABLE_ASYNC_ULS),)
DEFINES += -D XDAPM_ENABLE_ASYNC_ULS
endif

ifneq ($(BA_EN_DAPM_TSK_EXEC_DAPM_ULS),)
DEFINES += -D XDAPM_EXEC_DAPM_ULS
ifneq ($(BA_EN_DAPM_SEND_ZPY_DATA),)
DEFINES += -D XDAPM_SEND_ULS_ZPY_DATA
endif
endif
ifneq ($(BA_EN_DAPM_TSK_EXEC_DAPM_FUN),)
DEFINES += -D XDAPM_EXEC_DAPM_FUN
ifneq ($(BA_EN_DAPM_SEND_ZPY_DATA),)
DEFINES += -D XDAPM_SEND_FUN_ZPY_DATA
endif
endif
ifneq ($(BA_EN_DAPM_TSK_EXEC_DAPM_VCT),)
DEFINES += -D XDAPM_EXEC_DAPM_VCT
ifneq ($(BA_EN_DAPM_SEND_ZPY_DATA),)
DEFINES += -D XDAPM_SEND_VCT_ZPY_DATA
endif
endif
#  /* activate UPA relevant modules */
ifneq ($(BA_EN_PROD_UPA),)
DEFINES += -D XDAPM_PROD_UPA
endif

#  /* activate P4U relevant modules */
ifneq ($(BA_EN_PROD_P4U),)
DEFINES += -D XDAPM_PROD_P4U
endif

#   /* activate BSD relevant modules */
ifneq ($(BA_EN_PROD_BSD),)
DEFINES += -D XDAPM_PROD_BSD
endif

ifneq ($(BA_EN_BSD_10CHANNEL),)
DEFINES += -D XDAPM_BSD_10CHANNEL_VARIANT
endif

ifneq ($(BA_EN_SIDE_ASSIST),)
DEFINES += -D XDAPM_ENABLE_SIDE_ASSIST
endif

ifneq ($(BA_EN_DYN_DIST_FILTER),)
DEFINES += -D XDAPM_ENABLE_DYN_DIST_FILTER
endif

ifneq ($(BA_EN_FUNC_HAPTIC),)
DEFINES += -D XDAPM_FUNC_HAPTIC
endif

#  /* activate runtime measurement */
ifneq ($(BA_EN_ENABLE_RUNTIME_MEASUR),)
DEFINES += -D XDAPM_ENABLE_RUNTIME_MEASUR
endif

#  /* activate error trace support */
ifneq ($(BA_EN_ENABLE_ERRT_SUPPORT),)
DEFINES += -D XDAPM_ENABLE_ERRT_SUPPORT -DREPLACE_ASSERT_BY_ERRT
endif

ifneq ($(BA_EN_FUNC_LOT_CTRL),)
DEFINES += -D XDAPM_FUNC_LOT_CTRL
endif

ifneq ($(BA_DIS_ENABLE_NEW_CFGD),)
DEFINES += -D XDAPM_USE_OLD_CFGD
endif

ifneq ($(BA_EN_DAPM_ENABLE_ASYNC_ULS),)
DEFINES += -D XDAPM_ENABLE_ASYNC_ULS
endif

ifneq ($(BA_EN_DAPM_ENABLE_CURBLIFTER),)
DEFINES += -D XDAPM_ENABLE_CURBLIFTER
endif

# /* sensor configuration */
DEFINES += -D XDAPM_MAX_SENSORS=$(BA_MAX_SENSORS)      \
    -D XDAPM_NUM_SENSORS_FRONT=$(BA_NUM_SENSORS_FRONT) \
	-D XDAPM_NUM_SENSORS_REAR=$(BA_NUM_SENSORS_REAR)

# /* sector configuration */
DEFINES += -D XDAPM_NUM_SECTORS_FRONT=$(BA_NUM_SECTORS_FRONT) \
	-D XDAPM_NUM_SECTORS_REAR=$(BA_NUM_SECTORS_REAR)          \
	-D XDAPM_NUM_SECTORS_SIDE=$(BA_NUM_SECTORS_SIDE)


#  /******************************************************************************/
#  /***                       VCTM-Configuration                               ***/
#  /******************************************************************************/

#  /* DAPM - Driving Assistance system Product Master */
#  /* steering type if STRC module is used */
ifneq ($(BA_EN_VCTM_STEERING_TYPE_ANGLE),)
DEFINES += -D XDAPM_VCTM_STEERING_TYPE_ANGLE
endif

ifneq ($(BA_EN_VCTM_STEERING_TYPE_TORQUE),)
DEFINES += -D XDAPM_VCTM_STEERING_TYPE_TORQUE
endif

#  /******************************************************************************/
#  /***                       VDIM-Configuration                               ***/
#  /******************************************************************************/


#  /******************************************************************************/
#  /***                       ODOM-Configuration                               ***/
#  /******************************************************************************/

#  /******************************************************************************/
#  /*                         MOPL-Configuration                                 */
#  /******************************************************************************/
#  /* activate BRAKE */
ifneq ($(BA_EN_FUNC_BRAKE),)
DEFINES += -D XDAPM_FUNC_BRAKE
endif

#  /* activate PARK PARALLEL BW */
ifneq ($(BA_EN_FUNC_PARK_PARA_BW),)
DEFINES += -D XDAPM_FUNC_PARK_PARA_BW
endif

#  /* activate PARK PARALLEL FW - not activated yet */
ifneq ($(BA_EN_FUNC_PARK_PARA_FW),)
DEFINES += -D XDAPM_FUNC_PARK_PARA_FW
endif

#  /* activate PARK PERP BW */
ifneq ($(BA_EN_FUNC_PARK_PERP_BW),)
DEFINES += -D XDAPM_FUNC_PARK_PERP_BW
endif

ifneq ($(BA_EN_FUNC_PARK_PERP_FW),)
DEFINES += -D XDAPM_FUNC_PARK_PERP_FW
endif

#  /* activate PARK OUT */
ifneq ($(BA_EN_FUNC_PARK_OUT_PARA_FW),)
DEFINES += -D XDAPM_FUNC_PARK_OUT_PARA_FW
endif

ifneq ($(BA_EN_FUNC_PARK_OUT_PERP_FW),)
DEFINES += -D XDAPM_FUNC_PARK_OUT_PERP_FW
endif

ifneq ($(BA_EN_FUNC_PARK_OUT_PERP_BW),)
DEFINES += -D XDAPM_FUNC_PARK_OUT_PERP_BW
endif

ifneq ($(BA_EN_FUNC_DRIVE_EXTERN_GOAL),)
DEFINES += -D XDAPM_ENABLE_DRIVE_EXTERN_GOAL
endif

#  /* activate external odometry */
ifneq ($(BA_EN_FUNC_INTERNAL_ODOMETRY),)
DEFINES += -D XDAPM_FUNC_INTERNAL_ODOMETRY
endif

#  /* activate external odometry */
ifneq ($(BA_EN_FUNC_EXTERNAL_ODOMETRY),)
DEFINES += -D XDAPM_FUNC_EXTERNAL_ODOMETRY
endif

ifneq ($(BA_EN_FUNC_P4U_HOME),)
DEFINES += -D XDAPM_FUNC_P4U_HOME
endif

ifneq ($(BA_EN_DASW_SUPPORT_EXTERNAL_ODOMETRY),)
DEFINES += -D XDASW_SUPPORT_EXTERNAL_ODOMETRY
endif


ifneq ($(BA_EN_FUNC_PATH_FOLLOW),)
DEFINES += -D XDAPM_FUNC_PATH_FOLLOW
endif

ifneq ($(BA_EN_FUNC_DYN_VEL_PROFILE),)
DEFINES += -D XDAPM_ENABLE_DYN_VEL_PROFILE
endif

#  /* activate exploration mode support */
ifneq ($(BA_EN_FUNC_EXPLORATION),)
DEFINES += -D XDAPM_FUNC_EXPLORATION
endif

#  /* activate processing of camera features (external features) */
ifneq ($(BA_EN_CAM_FOR_STAT_MAP),)
DEFINES += -D XDAPM_ENABLE_CAM_FOR_STAT_MAP
endif
ifneq ($(BA_EN_CAM_FOR_DYN_MAP),)
DEFINES += -D XDAPM_ENABLE_CAM_FOR_DYN_MAP
endif

ifneq ($(BA_EN_CALC_DTC_ON_DYNAMIC_OBJ),)
DEFINES += -D XDAPM_CALC_DTC_ON_DYNAMIC_OBJ
endif

ifneq ($(BA_EN_CAM_FOR_LINE_MAP),)
DEFINES += -D XDAPM_ENABLE_CAM_FOR_LINE_MAP
endif

ifneq ($(BA_EN_RAD_FOR_STAT_MAP),)
DEFINES += -D XDAPM_ENABLE_RAD_FOR_STAT_MAP
endif

ifneq ($(BA_EN_LAS_FOR_STAT_MAP),)
DEFINES += -D XDAPM_ENABLE_LAS_FOR_STAT_MAP
endif

#  /******************************************************************************/
#  /*                         OBJD-Configuration                                 */
#  /******************************************************************************/
#
#  /* disable FPU usage in OBJD */
ifeq ($(BA_EN_STOCHASTIC_MAP),)
DEFINES += -D XDAPM_CFG_NON_STOCHASTIC_MAP
endif

#  /* disable FPU usage in OBJD */
ifneq ($(BA_EN_KOOL),)
DEFINES += -D XDAPM_ENABLE_KOOL
endif

ifneq ($(BA_EN_GMPS),)
DEFINES += -D XDAPM_ENABLE_GMPS
endif
#  /******************************************************************************/
#  /*                      Application-Configuration                             */
#  /******************************************************************************/
#
#disabled to remove dynamic configurations
ifneq ($(BA_APPL_CAR_CONFIG),)
DEFINES += -D APPL_CAR_CONFIG=$(BA_APPL_CAR_CONFIG)
endif

ifneq ($(BA_DIS_ASSERTS),)
DEFINES += -D XAPPL_DISABLE_ASSERT
endif

ifneq ($(BA_ENABLE_DIDH_DEBUG),)
DEFINES += -D XDIDH_DBG_DEBUGGING
endif
