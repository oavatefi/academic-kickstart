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
#define PLATFORM_CAR_VARIANT Tuam_Daimler_idx

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

#define XPLATFORM_GetStatusEps						COMH_GetStatusEps
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
#if defined (FW_USE_LINUX)
#warning  "don't Forget to map #  define XPLATFORM_GetTimer2us PIT_GetTimer2us"
#	define XPLATFORM_GetTimer2us                    OSW_GetTimer2us
#else
#	define XPLATFORM_GetTimer2us                    PIT_GetTimer2us
#endif

#include "platform_interfaces.h"
#endif /* APPL_PLATFORM_PROJECT_PDF_H_ */
