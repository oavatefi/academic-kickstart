/******************************************************************************/
/*                                                                            */
/*   Valeo Schalter und Sensoren GmbH                                         */
/*   Laiernstrasse 12                                                         */
/*   74321 Bietigheim-Bissingen                                               */
/*                                                                            */
/*   All rights reserved. Distribution or duplication without previous        */
/*   written agreement of the owner prohibited.                               */
/*                                                                            */
/******************************************************************************/
/*                                           */
/*    Source:       comh.h                                                     */
/*    Revision:     1.112.1.4.1                                                     */
/*    Author:       kistner_andreas, modified Raoul L. Pagoup, J. Kreutz     */
/*    last Editor:  BRODRIG2                                                     */
/*    Date:         14-JUL-2015 17:10:51                                                    */
/*                                             */
/******************************************************************************/
/*   MODULE: COMH (COMmunication Handler)                                     */
/*   FILE:  Module Header File                                                */
/******************************************************************************/
/* This module contains the communication between the CAN network and the PAM */
/* application logic                                                          */
/******************************************************************************/
#ifndef I_COMH_H
#define I_COMH_H

#include "dstdbool.h"
#include "dstdint.h"

#include "dapm_typ.h"
#include "vctc.h"
#include "odoc.h"
#include "capp.h"
#include "platform_types.h"
#include "comh_typ.h"


/******************************************************************************/
/*                 Definition of exported symbolic constants                  */
/******************************************************************************/
#define XP2GPA_CAN_ID_PARA_RECEIVE      0x700
#define XISTP_RESP_CAN_ID               0x774
enum COMH_wheel_info_E
{
  COMH_WHEEL_INFO_INVALID = 0,
  COMH_WHEEL_INFO_FORWARD,
  COMH_WHEEL_INFO_BACKWARD,
  COMH_WHEEL_INFO_STANDSTILL
};

/* generic gear states of automatic transmission (mGetriebe_1) */
enum COMH_gear_lever_position_E
{
  COMH_GLP_IDLE = 0,
  COMH_GLP_POSITION_R,
  COMH_GLP_POSITION_N,
  COMH_GLP_POSITION_D,
  COMH_GLP_POSITION_P
};


enum AutomaticTransmissionGearState_E
{
  POS_IN_BETWEEN = 0x00,
  POSITION_1     = 0x01,
  POSITION_2     = 0x02,
  POSITION_3     = 0x03,
  POSITION_4     = 0x04,
  POSITION_D     = 0x05,
  POSITION_N     = 0x06,
  POSITION_R     = 0x07,
  POSITION_P     = 0x08,
  POSITION_RSP   = 0x09,
  POSITION_Z1    = 0x0A,
  POSITION_Z2    = 0x0B,
  POSITION_S     = 0x0C,
  POSITION_L     = 0x0D,
  TIPPFUNCTION   = 0x0E,
  GEAR_ERROR     = 0x0F
};

#define COMH_NUM_SECTORS_MAX A_DAS_NUM_SECTORS_REAR
#if A_DAS_NUM_SECTORS_FRONT > A_DAS_NUM_SECTORS_REAR
#  undef COMH_NUM_SECTORS_MAX
#  define COMH_NUM_SECTORS_MAX A_DAS_NUM_SECTORS_FRONT
#endif
#if A_DAS_NUM_SECTORS_SIDE > A_DAS_NUM_SECTORS_FRONT
#  undef COMH_NUM_SECTORS_MAX
#  define COMH_NUM_SECTORS_MAX A_DAS_NUM_SECTORS_SIDE
#endif

enum COMH_spa_E
{
    COMH_SPA_FRONT = 0,
    COMH_SPA_REAR,
};


typedef enum
{
    BRAKING_STATE_PARKMAN_NOT_AVAILABLE=0 , /*ParkMan not available*/
    BRAKING_STATE_PARKMAN_INIT, /*ParkMan initializing*/
    BRAKING_STATE_PARKMAN_CANCEL_DRIVER, /*ParkMan cancelling due to driver*/
    BRAKING_STATE_PARKMAN_CANCEL_FAULT, /*ParkMan cancelling due to fault*/
    BRAKING_STATE_PARKMAN_ENABLED, /*ParkMan enabled*/
    BRAKING_STATE_PARKMAN_INACTIV, /*ParkMan inactive*/
    BRAKING_STATE_APC_MODE, /*ParkMan control active, Active Park Control mode*/
    BRAKING_STATE_RPA_MODE, /*ParkMan control active, Remote Park Assist mode*/
    BRAKING_STATE_RPA_UNDO_MODE, /*ParkMan control active, Remote Park Assist undo mode*/
    BRAKING_STATE_DAA_MODE, /*ParkMan control active, Drive Away Assist mode*/
    BRAKING_STATE_PPB_MODE, /*ParkMan control active, Park Plus Brake mode*/
    BRAKING_STATE_CTA_MODE /*ParkMan control active, Crossing Traffic Assist mode*/
}esp_brake_state_E;

typedef enum
{
    ESP_INTERVENTION_INACTIVE,
    ESP_INTERVENTION_ACTIVE
}esp_intervention_E;

typedef enum
{
    ENGINE_NOT_RUNNING = 0,
    ENGINE_RUNNING
}engine_running_state_E;

typedef enum
{
    ESP_LDC_AFTERRUNCTRL_NO_FOLLOW_UP,
    ESP_LDC_AFTERRUNCTRL_BRAKE_FOLLOW_UP,
    ESP_LDC_AFTERRUNCTRL_RELEASE_FOLLOW_UP
}ldc_afterrun_ctrl_E;

typedef enum
{
    GEAR_POSITION_RQ_IDLE=0,
    GEAR_POSITION_RQ_D,
    GEAR_POSITION_RQ_N,
    GEAR_POSITION_RQ_R,
    GEAR_POSITION_RQ_P
}gear_request_E;

typedef enum
{
    ESP_STATE_IDLE=0,
    ESP_STATE_ESP_FLT,
    ESP_STATE_LCP_PSV,
    ESP_STATE_PM_FLT,
    ESP_STATE_SPCR_FLT,
    ESP_STATE_EPB_FLT,
    ESP_STATE_LDC_SATURATED,
    ESP_STATE_MAX_SPD,
    ESP_STATE_REMAIN_DIST,
    ESP_STATE_MAX_SLOPE,
    ESP_STATE_ESP_CTRL,
    ESP_STATE_TCM_FLT,
    ESP_STATE_ECM_FLT,
    ESP_STATE_ENG_OFF,
    ESP_STATE_DRV_ACTV,
    ESP_STATE_DRV_ABSENCE,
    ESP_STATE_LDC_OVERRULED,
    ESP_STATE_RPA_FLT,
    ESP_STATE_RPA_MAX_SPD,
    ESP_STATE_RPA_UNDO_MAX_SLOPE,
    ESP_STATE_RPA_MAX_SLOPE

}park_flt_stat_esp_E;

typedef enum
{
    ESP_SYSTEM_STATE_TEMPORARY_OFF,
    ESP_SYSTEM_STATE_NORMAL_OPERATION,
    ESP_SYSTEM_STATE_ERROR,
    ESP_SYSTEM_STATE_DIAGNOSTIC,
    ESP_SYSTEM_STATE_EXHAUST_EMISSION_TEST,
    ESP_SYSTEM_STATE_TEST_BENCH_CC_ON,
    ESP_SYSTEM_STATE_TEST_BENCH_CC_REGULATION_ACTIVE
}esp_system_state_E;

typedef enum
{
    GEAR_POSITION_D,
    GEAR_POSITION_R,
    GEAR_POSITION_N,
    GEAR_POSITION_P
}gear_position_E;

typedef enum
{
    KL15_OFF = 0,
    KL15_ON
}KL15_state_E;

/******************************************************************************/
/*                Definition of exported function like macros                 */
/******************************************************************************/

/******************************************************************************/
/*                    Declaration of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of exported constant data                     */
/******************************************************************************/

/******************************************************************************/
/*               Declaration of exported function prototypes                  */
/******************************************************************************/
void COMH_Init(void);
void COMH_CanTask(void);
void COMH_Cyclic20ms(void);
void COMH_Cyclic50ms(void);

#ifdef APPL_IS_PLA_SW
u16    COMH_GetWheelSpeedFrontLeft(void);
u16    COMH_GetWheelSpeedFrontRight(void);
u16    COMH_GetWheelSpeedRearLeft(void);
u16    COMH_GetWheelSpeedRearRight(void);

void   COMH_GetSteeringMomentAndSign(u16 *moment, u8 *sign);
u8     COMH_GetStatusEps(void);
void   COMH_GetSteeringAngleAndSign(u16 *angle, u8 *sign);
u8     COMH_GetPlaTerminationOfEps(void);
#endif /* #ifdef APPL_IS_PLA_SW */

bool_T COMH_GetReverseGear(void);
u8     COMH_GetTrailerPresent(void);

/* COMH_GetTrailer only for BAP- > delete if BAP is adapted to use COMH_GetTrailerPresent */
bool_T COMH_GetTrailer(void);

u16    COMH_GetSpeedRawData(void);
#ifdef APPL_IS_PLA_SW
u8     COMH_GetSpeedBackupValueActive(void);
u8     COMH_GetAsrRequest(void);
u8     COMH_GetMsrRequest(void);
u8     COMH_GetEdsIntervention(void);
u8     COMH_GetAbsIntervention(void);
u8     COMH_GetEspIntervention(void);
u8     COMH_GetEspAsrDisabled(void);
u8     COMH_GetEspSystemStatusError(void);

si16   COMH_GetLongitudinalAcceleration(void);
si16   COMH_GetBrakePedalTorque(void);

u16    COMH_GetWheelImpulsesFrontLeft(void);
u16    COMH_GetWheelImpulsesFrontRight(void);
u16    COMH_GetWheelImpulsesRearLeft(void);
u16    COMH_GetWheelImpulsesRearRight(void);
bool_T COMH_IsWheelImpValid(void);
u32    COMH_GetStWheelAngleTime512us(void);
u32    COMH_GetWheelImpTime512us(void);
enum COMH_wheel_info_E COMH_GetWheelInfoRearLeft(void);
enum COMH_wheel_info_E COMH_GetWheelInfoRearRight(void);
enum COMH_wheel_info_E COMH_GetWheelInfoFrontLeft(void);
enum COMH_wheel_info_E COMH_GetWheelInfoFrontRight(void);

u8 COMH_GetWheelImpulseMsgCounter(u8 *msg_counter);

u16    COMH_GetWheelCircumference(void);

u8     COMH_GetImpulseWheelNumberOfTeeth(void);

bool_T COMH_IsVehicleDoorsClosed(void);
bool_T COMH_IsVehicleStandstill(void);
#endif /* #ifdef APPL_IS_PLA_SW */
KL15_state_E     COMH_GetKl15(void);

void COMH_GetYawSpeedAndSign(u16 *yaw_speed, u8 *sign);
#ifdef XDAPM_FUNC_LOT_CTRL
u8     COMH_GetStateBrakeEcu(void);
u8     COMH_GetAppBrakeAbortReason(void);
#endif /* #ifdef XDAPM_FUNC_LOT_CTRL */

#ifdef APPL_IS_PLA_SW
bool_T COMH_IsLongAccValueValid(void);
#endif /* #ifdef APPL_IS_PLA_SW */

#ifdef XP2GPA_ENABLE_EITP_THROUGH_APP
void COMH_CanSend (u8 prio, u16 id, const u8 *p, u8 n);
#endif


u8 COMH_CalculateCrc8(u16 id, const u8 *data_ptr, u8 length);
extern u8 COMH_GetStatusEpsTimestamp(void);
Std_ReturnType COMH_GetWheelSpeedfl_th100mps(si16* wheel_speed_fl);
Std_ReturnType COMH_GetWheelSpeedrl_th100mps(si16* wheel_speed_rl);
Std_ReturnType COMH_GetWheelSpeedfr_th100mps(si16* wheel_speed_fr);
Std_ReturnType COMH_GetWheelSpeedrr_th100mps(si16* wheel_speed_rr);

/******************************************************************************/
/*   COMH GEN3 INTERFACE                                                  */
/******************************************************************************/

/*******************************/
/*   COMH_ULS                  */
/*******************************/
Std_ReturnType COMH_GetSteeringMoment(si16* steering_wheel_moment, u32* time_stamp);
Std_ReturnType COMH_GetEspDisabled(bool_T* is_esp_asr_disabled);
Std_ReturnType COMH_IsSideDoorOpen(bool_T* is_side_door_open);
Std_ReturnType COMH_IsTrailerPresent(bool_T* p_is_trailer_present);

Std_ReturnType COMH_IsSlidingDoorRightOpen(bool_T* is_sliding_door_open);
Std_ReturnType COMH_IsSlidingDoorLeftOpen(bool_T* is_sliding_door_open);

Std_ReturnType COMH_IsBrakeEcuStandstill(bool_T* is_brake_ecu_standstill);
Std_ReturnType COMH_GetBrakeIntervention(bool_T*  is_brake_intervention);

u8 COMH_GetStateEpsSys(enum VCTC_ext_sys_state_E* p_lat_ext_sys_state);

Std_ReturnType COMH_IsPark4UButtonPressed(bool_T* is_park4u_button_pressed);

/*******************************/
/*   COMH_COMMON               */
/*******************************/
Std_ReturnType COMH_GetWheelImpulse(u16* wheel_impulses, u32* time_stamp, enum ODOC_wheels_E wheel);
Std_ReturnType COMH_GetLongAcceleration(si16* longitudinal_acceleration, u32* time_stamp);
Std_ReturnType COMH_GetSpeed(u16* speed, u32* time_stamp);
Std_ReturnType COMH_GetGearLeverPosition(enum CAPP_gear_lever_position_E* gear_lever_pos);
Std_ReturnType COMH_GetSteeringWheelAngle(si16* steering_wheel_angle, u32* time_stamp);
Std_ReturnType COMH_GetSteeringWheelAnglePhys(si32* steering_wheel_angle, u32* time_stamp);
Std_ReturnType COMH_GetSteeringRackGearPos(si16* steering_rack_gear_pos, u32* time_stamp);
Std_ReturnType COMH_GetFrontWheelSteerAngle(si16* wheel_angle, u32* time_stamp);
Std_ReturnType COMH_GetRearWheelSteerAngle(si16* wheel_angle, u32* time_stamp);
Std_ReturnType COMH_GetWheelDriveDir(enum CAPP_dr_dir_E* wheel_driving_dir,
                                     u32* time_stamp,
                                     enum ODOC_wheels_E wheel);
Std_ReturnType COMH_GetWheelSpeed(si16* wheel_speed, u32* time_stamp, enum ODOC_wheels_E wheel);
Std_ReturnType COMH_GetWheelSpeedRPM(float* wheel_speed, u32* time_stamp, enum ODOC_wheels_E wheel);
Std_ReturnType COMH_GetYawSpeed(si16* yaw_speed, u32* time_stamp);
Std_ReturnType COMH_GetYawRatePhys(float* yaw_speed, u32* time_stamp);
Std_ReturnType COMH_GetLateralAcceleration(si16* lateral_acceleration, u32* time_stamp);
Std_ReturnType COMH_GetOutsideTemperature(si8* outside_temperature);

Std_ReturnType COMH_IsPdcButtonPressed(bool_T* is_pdc_button_pressed);
Std_ReturnType COMH_GetBlinkerLeft(bool_T* is_blinker_left_enabled);
Std_ReturnType COMH_GetBlinkerRight(bool_T* is_blinker_right_enabled);
//Std_ReturnType COMH_GetEnvironmentData(struct APPL_environment_S* env_data);
Std_ReturnType COMH_GetVehicleStandstill(bool_T* p_is_standstill);

void COMH_SendDevIdData(struct DIDH_dev_id_S *dev_id_data);
void COMH_SendNotification(bool_T start_of_transmission);


extern bool_T COMH_GetPAM_req_buttonRelease(void);



/* differences COMH A2GPA / A2MBE */
si16   COMH_GetSignedSteeringAngle10thDegree(void);
Std_ReturnType COMH_GetGearLeverPosition_RevGr_Engg(enum CAPP_gear_lever_position_E* gear_lever_pos);
bool_T COMH_IsReadyForGearChange(void);
u16    COMH_GetSpeedKph(void);

Std_ReturnType COMH_GetExtWarningDirection(enum CAPP_dr_dir_E *warning_direction);

enum button_state_E  COMH_GetUpaButtonState(void);
enum button_state_E  COMH_GetPark4uButtonState(void);
enum button_state_E  COMH_GetConfirmationButtonState(void);
u8 COMH_GetRequestedView(void);
u8 COMH_GetHZTROptionsButtonValue(void);
u8 COMH_GetViewsOptionsButtonValue(void);
u8 COMH_GetP4USlotsOptionsButtonValue(void);
u8 COMH_GetBDAOptionsButtonValue(void);
enum button_state_E  COMH_GetGpsButtonState(void);
enum button_state_E  COMH_GetP4uHomeTrainingButtonState(void);
enum button_state_E  COMH_GetAbortButtonState(void);
enum button_state_E  COMH_GetDeadManButtonState(void);
enum button_state_E  COMH_GetSwitchOptionsButtonState(void);
enum button_state_E  COMH_GetFuncBarBackButtonState(void);
u8 COMH_GetFuncBarOptionsButtonValue(void);
void COMH_SetFuncBarOptionsButtonValue(u8 funcbar_views);
void            COMH_GetBrakePressure     (si16 * data);
enu_break_pedal COMH_GetBreakPedal        (void);
Std_ReturnType COMH_GetGPSdata(si32* longitude , si32* latitude);

void COMH_GetBLEMessage(ble_msg_T* p);

bool COMH_IsSeatBeltRemoved(void);

bool COMH_IsDoorOpened(void);

static void Send_Debug_Msg(void);


/* Function prototypes for Daimler BR213 */
esp_brake_state_E       COMH_GetEspBrakeState(void);
esp_intervention_E      COMH_GetESPIntervention(void);
engine_running_state_E  COMH_GetEngineState(void);
ldc_afterrun_ctrl_E     COMH_GetEspAfterrunControl(void);
Std_ReturnType          COMH_GetEpbStatus(epb_status_T* epb_status);
gear_position_E         COMH_GetCarCurrentGear(void);
u8                      COMH_GetParkEnblStatEsp(void);
park_flt_stat_esp_E     COMH_GetParkFltStatEsp(void);
esp_system_state_E      COMH_GetEspSystemState(void);
Std_ReturnType          COMH_GetSpeed_th100kph(u16* vehicle_speed);

void COMH_Clear_BLE_MSGS(void);
#endif /* I_COMH_H */

/******************************************************************************/
/*                                            */
/* END  Source:         COMH.h                                                  */
/*                                              */
/******************************************************************************/




