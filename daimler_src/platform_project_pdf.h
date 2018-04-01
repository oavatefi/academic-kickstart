/*
 * platform_project_pdf.h
 *
 *  Created on: Oct 11, 2016
 *      Author: mabdalla
 */

#ifndef APPL_PLATFORM_PROJECT_PDF_H_
#define APPL_PLATFORM_PROJECT_PDF_H_



#include "platform_types.h"




#define PLATFORM_project_Task10ms        COMH_Cyclic10ms
#define PLATFORM_project_Task20ms
#define PLATFORM_project_Task40ms
#define PLATFORM_CAR_VARIANT    Tuam_Daimler_idx

/************************************************************************************************************
**************************************    COMH External interfaces    **************************************/

#ifndef XMENU_BAR
#define XPLATFORM_GetPark4uButtonState             COMH_GetPark4uButtonState
#define XPLATFORM_GetDeadManButtonState            COMH_GetDeadManButtonState
#define XPLATFORM_GetAbortButtonState              COMH_GetAbortButtonState
#define XPLATFORM_GetReplayButtonState             COMH_GetBdaReplayButtonState
#define XPLATFORM_GetGPSButtonState                COMH_GetGpsButtonState
#define XPLATFORM_GetP4uHomeTrainingButtonState    COMH_GetP4uHomeTrainingButtonState
#define XPLATFORM_GetViewButtonStatus              COMH_GetViewButtonState
#define XPLATFORM_GetRequestedView                 COMH_GetRequestedView
#endif
#define XPLATFORM_GetHztrOptionsButtonRequest       COMH_GetHZTROptionsButtonValue
#define XPLATFORM_GetBdaOptionsButtonRequest        COMH_GetBDAOptionsButtonValue
#define XPLATFORM_SetFuncBarOptionsValue            COMH_SetFuncBarOptionsButtonValue
#define XPLATFORM_GetFuncBarOptionsButtonRequest    COMH_GetFuncBarOptionsButtonValue
#define XPLATFORM_GetFuncBarBackValue               COMH_GetFuncBarBackButtonState
#define XPLATFORM_GetBlinkerLeft                    COMH_GetBlinkerLeft
#define XPLATFORM_GetBlinkerRight                   COMH_GetBlinkerRight
#define XPLATFORM_GetGearLeverPosition              COMH_GetGearLeverPosition
#define XPLATFORM_IsVehicleStandstill               COMH_IsVehicleStandstill
#define XPLATFORM_GetSpeedKph                       COMH_GetSpeedKph
#define XPLATFORM_GetConfirmationButtonState        COMH_GetConfirmationButtonState
#define XPLATFORM_GetSwitchOptionsButtonState       COMH_GetSwitchOptionsButtonState
#define XPLATFORM_GetUpaButtonState                 COMH_GetUpaButtonState
#define XPLATFORM_GetBrakePedalstate                COMH_GetBreakPedal
#define XPLATFORM_ChooseManeuver_buttonRelease      COMH_ChooseManeuverButtonValue // button for maneuver selection
#define XPLATFORM_GetP4uOptionsButtonRequest        COMH_GetP4U_req_buttonRelease // button for  slot selection
#define XPLATFORM_GetEpbStatus                      COMH_GetEpbStatus
#define XPLATFORM_BLE_Activate                      COMH_BLE_Activate
#define XPLATFORM_BLE_Deactivate                    COMH_BLE_Deactivate
#define XPLATFORM_BLE_GetState                      COMH_BLE_GetState
#define XPLATFORM_GetBLEMessage                     COMH_GetBLEMessage
#define XPLATFORM_BLE_SetManeuverTwoMsg             COMH_BLE_SetManeuverTwoMsg
#define XPLATFORM_BLE_SetManeuverOneMsg             COMH_BLE_SetManeuverOneMsg
#define XPLATFORM_BLE_Reset                         COMH_BLE_Reset
#define XPLATFORM_GetStateBrakeEcu                  COMH_GetStateBrakeEcu
#define XPLATFORM_IsDoorOpened                      COMH_IsDoorOpened
#define XPLATFORM_IsSeatBeltRemoved                 COMH_IsSeatBeltRemoved
#define XPLATFORM_VehicleDoorsClosed_chk            COMH_IsVehicleDoorsClosed
#define XPLATFORM_BLE_Set_P4u_Active                COMH_BLE_Set_P4u_Active
#define XPLATFORM_BLE_Reset_P4U_Active              COMH_BLE_Reset_P4U_Active
#define XPLATFORM_GetOptionsButtonRequest           COMH_GetViewsOptionsButtonValue

#define XPLATFORM_GetGPSDate                        COMH_GetGPSDate
#define XPLATFORM_GetGPSTime                        COMH_GetGPSTime
#define XPLATFORM_GetGPSProperties                  COMH_GetGPSProperties
#define XPLATFORM_GetGPSHorizontalSpeed             COMH_GetGPSHorizontalSpeed
#define XPLATFORM_GetGPSPosition                    COMH_GetGPSPosition
/************************************************************************************************************
**************************************    BRKH External interfaces    **************************************/

#define XPLATFORM_CalcDistanceToStop                BRKH_GetRemainDist
#define XPLATFORM_CusActivateEmergencyBrake         BRKH_CusActivateEmergencyBrake
#define XPLATFORM_CusDeActivateEmergencyBrake       BRKH_CusDeActivateEmergencyBrake
#define XPLATFORM_IsBrakeActive                     BRKH_CusIsBrakeEcuActive
#define XPLATFORM_ActivateEpb                       BRKH_TightenEpb
#define XPLATFORM_BRKHGetMainState                  BRKH_GetMainState
#define XPLATFORM_BRKHGetActiveSubState             BRKH_GetActiveSubState
#define XPLATFORM_BRKHCusIsEmergencyBrakeActive        BRKH_CusIsEmergencyBrakeActive

/************************************************************************************************************
**************************************    STMH External interfaces    **************************************/

#define XPLATFORM_IsSteeringActive                  STMH_CusIsSteeringActive
#define XPLATFORM_STMHGetMainState                  STMH_GetMainState
#define XPLATFORM_STMHGetActiveSubState             STMH_GetActiveSubState

/*************************************************************************************************************/

extern void XPLATFORM_SetFuncBarOptionsValue(u8 funcbar_views);
extern u8 XPLATFORM_GetFuncBarOptionsButtonRequest(void);
extern enum button_state_E XPLATFORM_GetFuncBarBackValue(void);
extern enum button_state_E XPLATFORM_GetSwitchOptionsButtonState(void);
extern Std_ReturnType XPLATFORM_GetBlinkerLeft(bool_T* is_blinker_left_enabled);
extern Std_ReturnType XPLATFORM_GetBlinkerRight(bool_T* is_blinker_left_enabled);
extern Std_ReturnType XPLATFORM_GetGearLeverPosition(enum CAPP_gear_lever_position_E* gear_lever_pos);
extern enum button_state_E XPLATFORM_GetConfirmationButtonState(void);
extern u16 XPLATFORM_GetSpeedKph(void);
extern u8 XPLATFORM_ChooseManeuver_buttonRelease(void);
extern bool_T XPLATFORM_GetP4uOptionsButtonRequest(void);
extern u8 XPLATFORM_GetHztrOptionsButtonRequest(void);
extern u8 XPLATFORM_GetBdaOptionsButtonRequest(void);
extern enum button_state_E XPLATFORM_GetReplayButtonState(void);
extern enum button_state_E XPLATFORM_GetDeadManButtonState(void);
extern enum button_state_E XPLATFORM_GetAbortButtonState(void);
extern enu_break_pedal  XPLATFORM_GetBrakePedalstate(void);
extern enum button_state_E XPLATFORM_GetGPSButtonState(void);
extern enum button_state_E XPLATFORM_GetP4uHomeTrainingButtonState(void);
extern Std_ReturnType XPLATFORM_GetEpbStatus(epb_status_T* epb_status);
extern void XPLATFORM_BLE_Activate(void);
extern void XPLATFORM_BLE_Deactivate(void);
extern ble_state_T XPLATFORM_BLE_GetState(void);
extern void XPLATFORM_GetBLEMessage(ble_msg_T* p);
extern void XPLATFORM_BLE_SetManeuverTwoMsg(ble_maneuver2_T* p, enum ble_active_system active_system);
extern void XPLATFORM_BLE_SetManeuverOneMsg(ble_maneuver1_T* p, enum ble_active_system active_system);
extern void XPLATFORM_BLE_Reset(void);
extern u8 XPLATFORM_GetStateBrakeEcu(void);
extern bool XPLATFORM_IsDoorOpened(void);
extern bool XPLATFORM_IsSeatBeltRemoved(void);
extern bool_T XPLATFORM_VehicleDoorsClosed_chk(void);
extern bool_T XPLATFORM_IsVehicleStandstill(void);
extern void XPLATFORM_BLE_Reset_P4U_Active(void);
extern void XPLATFORM_BLE_Set_P4u_Active(void);
extern u8 XPLATFORM_GetRequestedView(void);
extern u8 XPLATFORM_GetViewsOptionsButtonValue(void);
extern enum button_state_E XPLATFORM_GetViewButtonStatus(void);
extern enum button_state_E XPLATFORM_GetUpaButtonState(void);

extern sint16 XPLATFORM_CalcDistanceToStop (void);
extern bool_T XPLATFORM_CusActivateEmergencyBrake(void);
extern void XPLATFORM_CusDeActivateEmergencyBrake(void);
extern bool_T  XPLATFORM_IsBrakeActive(void);
extern void XPLATFORM_ActivateEpb(void);

extern void XPLATFORM_GetGPSDate(u8* day, u8* month, u16* year);
extern void XPLATFORM_GetGPSTime(u8* seconds, u8* minutes, u8* hours);
extern void XPLATFORM_GetGPSProperties(u16* accuracy_horizontal, u16* error_latitude, u16* error_longitude, u8* quantity_satellite);
extern void XPLATFORM_GetGPSHorizontalSpeed(u16* speed_horizontal);
extern void XPLATFORM_GetGPSPosition(si32* longitude , si32* latitude);

extern brkh_app_states_E XPLATFORM_BRKHGetMainState  (void);
extern brkh_app_sub_states_E XPLATFORM_BRKHGetActiveSubState(void);
extern bool_T XPLATFORM_IsSteeringActive(void);
extern enum  stmh_app_states_E XPLATFORM_STMHGetMainState(void);
extern enum  stmh_app_sub_states_E XPLATFORM_STMHGetActiveSubState(void);
extern bool_T XPLATFORM_BRKHCusIsEmergencyBrakeActive(void);

#endif /* APPL_PLATFORM_PROJECT_PDF_H_ */
