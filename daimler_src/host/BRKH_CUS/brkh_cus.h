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
/*                                            */
/*      Source:         brkh_cus.h                                                  */
/*      Revision:       1.1.1.3                                                  */
/*      Author:         kistner_andreas                                       */
/*      UserID:         BRODRIG2                                                  */
/*      Date            14-JUL-2015 17:13:41                                                 */
/*                                              */
/******************************************************************************/
/*   MODULE: BRKH_CUS (customer specific code (brake communication) of the    */
/*                     BRKH-module)                                           */
/*   FILE: BRKH_CUS.H                                                         */
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
#ifndef I_BRKH_CUS_H
#define I_BRKH_CUS_H

#include "dstdbool.h"
#include "dstdint.h"

#include "vctc.h"
#include "platform_types.h"
#include "p2dal.h"
#include "comh.h"

/* declarations for BRKH/cgstuff.h */
#define INT8_T
#define INT16_T
#define INT32_T
#define SINT8_T
#define SINT16_T
#define SINT32_T
#define UINT8_T
#define UINT16_T
#define UINT32_T
#define DOUBLE_T
#define MAX

/******************************************************************************/
/*                 Definition of exported symbolic constants                  */
/******************************************************************************/

/******************************************************************************/
/*                Definition of exported function like macros                 */
/******************************************************************************/

/******************************************************************************/
/*          Definition of exported types (typdefs, enums, struct, union)      */
/******************************************************************************/
struct BRKH_cus_input_S
{
    u8 brake_ecu_state;
    u8 app_brake_abort_reason;
};

typedef enum
{
	BRKH_APP_NA=0,
	BRKH_APP_INIT,
	BRKH_APP_READY,
	BRKH_APP_FOLLOW_UP,
	BRKH_APP_BRAKE_ACTIVE
}brkh_app_states_E;

typedef enum
{
  BRKH_APP_SUB_REQ = 0,
  BRKH_APP_SUB_ACKN
}brkh_app_sub_states_E;


typedef enum
{
    BRKH_ACTIVE_EMERGENCY_PREFILL=0,
    BRKH_ACTIVE_EMERGENCY_BRAKE,
    BRKH_ACTIVE_P4U_ONLY,
    BRKH_ACTIVE_P4U_EMERGENCY_PREFILL,
    BRKH_ACTIVE_P4U_EMERGENCY_BRAKE,
    BRKH_ACTIVE_P4U_ABORT,
    BRKH_ACTIVE_P4U_FINISH,
    BRKH_ACTIVE_EMERGENCY_ABORT,
    BRKH_ACTIVE_EMERGENCY_FINISH
}brkh_active_sub_states_E;



typedef enum
{


    PARK_GUIDANCE_STATE_INACTIVE=0,
    PARK_GUIDANCE_STATE_ACTIVE,
    PARK_GUIDANCE_STATE_INACTIVE_LONG_ACTIVE
}park_guidance_state_E;

typedef enum
{
    PARK_PARK_STAT_IDLE=0,
    PARK_PARK_STAT_SEARCH,
    PARK_PARK_STAT_FOUND,
    PARK_PARK_STAT_PARK_OUT,
    PARK_PARK_STAT_DISCLAIMER,
    PARK_PARK_STAT_BACK_STROKE,
    PARK_PARK_STAT_FWD_STROKE,
    PARK_PARK_STAT_RPA_PREP,
    PARK_PARK_STAT_END,
    PARK_PARK_STAT_CANCEL_DRV,
    PARK_PARK_STAT_FLT,
    PARK_PARK_STAT_RPA_APP_ACTV,
    PARK_PARK_STAT_SEARCH_HU_ACTV,
    PARK_PARK_STAT_FOUND_HU_ACTV
}park_park_stat_E;

typedef enum
{
    PARK_CTRL_MODE_RQ_IDLE=0,
    PARK_CTRL_MODE_RQ_LIMIT,
    PARK_CTRL_MODE_RQ_SPEED,
    PARK_CTRL_MODE_RQ_REMOTE
}park_control_mode_rq_E;

typedef enum
{
    REM_DIST_STAT_NO_STEP = 0,
    REM_DIST_STAT_NEW_MEASURE,
    REM_DIST_STAT_DANGER,
    REM_DIST_STAT_RPA,
    REM_DIST_STAT_DAA,
    REM_DIST_STAT_PPB,
    REM_DIST_STAT_CTA
}remaining_distance_stat_E;



typedef enum
{
    PARK_PARKTYPE_IDLE=0,       //  No parking
    PARK_PARKTYPE_IN_LONG,      //  Parallel park-in
    PARK_PARKTYPE_IN_LTRL,      //  Perpendicular parkyin
    PARK_PARKTYPE_OUT_LONG,     //  Parallel park-out
    PARK_PARKTYPE_OUT_LTRL,     //  Perpendicular park-out
    PARK_PARKTYPE_REMOTE_PARK   //  Remote Parking
}park_parktype_E;

typedef enum
{
    PARK_IN=0,
    PARK_OUT
}brkh_maneuver_type_E;


typedef enum
{
    DANGER_MODE_INACTIVE,
    DANGER_MODE_ACTIVE
}danger_mode_rq_E;

typedef enum
{
    PARK_BRK_MODE_DISABLED=0,
    PARK_BRK_MODE_EMERGENCY_STOP,
    PARK_BRK_MODE_COMFORT_STOP
}park_brake_mode_E;

#ifdef USE_DIFFERENT_TRANSMISSIONS
typedef enum
{
    MANUAL_TRANSMISSION,
    AUTOMATIC_TRANSMISSION
}transmission_type_E;
#endif


/******************************************************************************/
/*                    Declaration of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of exported constant data                     */
/******************************************************************************/

/******************************************************************************/
/*               Declaration of exported function prototypes                  */
/******************************************************************************/

void                       BRKH_CusInit(void);
void                       BRKH_CusBrakeStateMachine(void);

u8                         BRKH_CusGetBrakeAbortReason(void);
u8                         BRKH_CusGetStateBrakeSys(bool_T* p_lot_ext_sys_state);

bool_T                     BRKH_CusIsBrakeEcuReady(void);
bool_T                     BRKH_CusIsBrakeEcuActive(void);
void                       BRKH_CusReset(void);
void                       BRKH_CusResetAbortReason(void);
void                       BRKH_CusDeactivateBrake(void);
bool_T                     BRKH_CusActivateBrake(void);
bool_T                     BRKH_CusActivateEmergencyBrake(void);
void                       BRKH_CusDeActivateEmergencyBrake(void);
bool_T                     BRKH_CusIsEmergencyBrakeActive(void);
u16                        BRKH_CalcDistanceToStop(void);

void BRKH_EvaluateParktype(void);
park_parktype_E BRKH_GetParktype(void);
bool_T BRKH_DasActivatingRequest(void);
uint16 BRKH_GetSpeedKph(void);
bool_T BRKH_ParkEnblStatEspAPC(void);
bool_T BRKH_ParkEnblStatEspPPB(void);

gear_position_E BRKH_CarCurrentGear(void);

void BRKH_SetEmergencyBrakeStatus(bool_T value);

uint16                     BRKH_GetVehSpdRq(void);
sint16                     BRKH_GetRemainDist(void);
park_park_stat_E           BRKH_GetParkParkStat(void);
esp_brake_state_E          BRKH_GetParkBrkStat(void);
park_brake_mode_E          BRKH_GetParkBrkMdRq(void);
park_control_mode_rq_E     BRKH_GetParkCtrlMdRq(void);
danger_mode_rq_E           BRKH_GetParkDangerMdRq(void);
park_guidance_state_E      BRKH_GetParkGuidStat(void);
remaining_distance_stat_E  BRKH_GetRemainDistStat(void);
gear_request_E             BRKH_GetGearPosRq(void);
park_parktype_E            BRKH_GetParkParkType(void);

void BRKH_SetRemainDistStat(remaining_distance_stat_E value);
void BRKH_CusSetBrakeState(bool_T state);

void BRKH_CusDeactivateBrake(void);

void BRKH_CusSetBrakeState(bool_T state);
void BRKH_SetEmergencyBrakeStatus(bool_T value);

bool_T BRKH_CusActivateEmergencyBrake(void);
uint16 BRKH_GetSpeedKph(void);
void BRKH_CusDeActivateEmergencyBrake(void);
bool_T BRKH_CusIsEmergencyBrakeActive(void);

bool_T BRKH_DasActivatingRequest(void);
u8 BRKH_GetMainState(void);
u8 BRKH_GetActiveSubState(void);
gear_position_E BRKH_CarCurrentGear(void);
u32 BRKH_GetPosition(void);
gear_position_E BRKH_GearLeverPos(void);
bool_T BRKH_ParkEnblStatEspAPC(void);

bool_T BRKH_LimitMaxSpeed(void);
bool_T BRKH_ParkEnblStatEspPPB(void);
void BRKH_SetVehSpdRq(u16 value);
void BRKH_SetRemainDist(sint16 value);
void BRKH_SetParkParkStat(park_park_stat_E value);
void BRKH_SetParkBrkStat(esp_brake_state_E value);
void BRKH_SetParkBrkMdRq(park_brake_mode_E value);
void BRKH_SetParkCtrlMdRq(park_control_mode_rq_E value);
void BRKH_SetParkDangerMdRq(danger_mode_rq_E value);
void  BRKH_SetParkGuidStat(park_guidance_state_E value);
void BRKH_SetRemainDistStat(remaining_distance_stat_E value);
void BRKH_SetGearPosRq(gear_request_E value);
void BRKH_SetParkParkType(park_parktype_E value);
u16 BRKH_GetVehSpdRq(void);
sint16 BRKH_GetRemainDist(void);
park_park_stat_E BRKH_GetParkParkStat(void);
esp_brake_state_E BRKH_GetParkBrkStat(void);
park_brake_mode_E BRKH_GetParkBrkMdRq(void);
park_control_mode_rq_E BRKH_GetParkCtrlMdRq(void);
danger_mode_rq_E BRKH_GetParkDangerMdRq(void);
park_guidance_state_E  BRKH_GetParkGuidStat(void);
remaining_distance_stat_E BRKH_GetRemainDistStat(void);
gear_request_E BRKH_GetGearPosRq(void);
park_parktype_E BRKH_GetParkParkType(void);
void BRKH_TightenEpb (void);

#endif /* I_BRKH_CUS_H */
/******************************************************************************/
/*                                            */
/* END  Source:         brkh_cus.h                                                  */
/*                                              */
/******************************************************************************/
