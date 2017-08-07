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
/*      Source:         brkh_cus.c                                                  */
/*      Revision:       1.1.1.4                                                  */
/*      Author:         kistner_andreas                                       */
/*      UserID:         BRODRIG2                                                  */
/*      Date            14-JUL-2015 17:14:00                                                 */
/*                                              */
/******************************************************************************/
/*   MODULE: BRKH_CUS (customer specific code (brake communication) of the    */
/*                     BRKH-module)                                           */
/*   FILE: BRKH_CUS.C                                                         */
/******************************************************************************/
/* Defines for conditional compiling (to be defined in PDF-Header):           */
/******************************************************************************/

/******************************************************************************/
/*              Include of common and project definitions header              */
/******************************************************************************/
#include "dstdbool.h"
#include "dstdint.h"
#include "dassert.h"
#include "tmp_pdf.h"

/******************************************************************************/
/*                      Include external modul header                         */
/******************************************************************************/

/******************************************************************************/
/*                      Include internal modul header                         */
/******************************************************************************/
#include "brkh_cus.h"
#include "brkh_cus_c.h"


/******************************************************************************/
/*                   Definition of local module constants                     */
/******************************************************************************/

/******************************************************************************/
/*                     Definition of local module macros                      */
/******************************************************************************/

#define USE_ONLY_P2_RECOMMENDED_SPEED STD_OFF
#define BRKH_USE_P2_COMMAND STD_OFF
#define USE_DIFFERENT_TRANSMISSIONS STD_OFF

/**
 * contains the send time of the state which is send to the brake ecu
 * the time is longer than 200ms because of the state change timing
 */

#define BRKH_STATE_SEND_TIME_MS  400

#define BRKH_INVALID_SLOT_ID ((uint8)201)
#define BRKH_MAX_REMAINING_DISTANCE ((sint16)20150)
#define BRKH_MAX_PARK_OUT_REMAINING_DISTANCE ((sint16)20000)
#define BRKH_IDLE_VEH_SPD_RQ ((uint16)1000)
#define BRKH_RD_DTH_JUMP_THRESHOLD_PMO ((sint16)2000)
#define BRKH_RD_DTH_JUMP_THRESHOLD_PMI ((sint16)4000)
#define BRKH_RD_JUMP_THRESHOLD (200)
#define BRKH_MIN_REMAINING_DISTANCE ((sint16)170)

#define BRKH_BIT_PARK_ENBL_STAT_ESP_APC (1u)
#define BRKH_BIT_PARK_ENBL_STAT_ESP_PPB (16u)

#define BRKH_PEB_CAR_TO_SLOT_Y (1000)
#define BRKH_PEF_CAR_TO_SLOT_Y (-2000)

#define BRKH_RD_RAMP_VALUE 8
#define BRKH_RD_MAX_CYCLES 5

#define BRKH_SPEED_MINIMUM_APC_MMS ((sint16)108)
/* APC Minimim speed 3kph */
#define BRKH_MIN_APC_SPEED         ((sint16)300)


#define BRKH_DLY_AUTOGEAR (4)
#define BRKH_DLY_STANDSTILL (10)
#define BRKH_DLY_REMDIST_TOLERANCE (3)
#define BRKH_DLY_ACTIVATION_ECU (25)

#define BRKH_UINT8_MAX (0xFF)

#define BRKH_SINT16_MAX ((sint16)32767)

#define STMA_ext_sys_state_E        VCTC_ext_sys_state_E
#define STMA_EXT_SYS_STATE_INACTIVE VCTC_EXT_SYS_STATE_INACTIVE
#define STMA_EXT_SYS_STATE_ACTIVE   VCTC_EXT_SYS_STATE_ACTIVE
#define STMA_EXT_SYS_STATE_ERROR    VCTC_EXT_SYS_STATE_ERROR

#define STMA_state_E            VCTC_ctrl_state_E
#define STMA_STATE_ACTIVE       VCTC_STATE_ACTIVE
#define STMA_STATE_ACTIVATING   VCTC_STATE_ACTIVATING
#define STMA_STATE_ACTIVATED    VCTC_STATE_ACTIVATED
#define STMA_STATE_DEACTIVATING VCTC_STATE_DEACTIVATING
#define STMA_STATE_STANDBY      VCTC_STATE_STANDBY

/******************************************************************************/
/*                     Definition of local module types                       */
/******************************************************************************/

typedef RTE_hu_single_park_slot_info_S RTE_hu_slot_found_T[8];

typedef struct
{
    boolean is_pos_ok_para;
    boolean is_pos_ok_bwd;
    boolean is_pos_ok_fwd;
    enum CAPP_slot_align_E slot_align;
    boolean para_perp_slot;
    uint8 slot_idx;
} RTE_dapm_selectable_park_slot_info_S;

typedef struct
{
    enum CAPP_spatial_alloc_E spatial_alloc;
    RTE_dapm_selectable_park_slot_info_S available_slots;
    sint16 veh_x_mm;
    sint16 veh_y_mm;
    uint16 length_mm;
    uint16 veh_theta_u16f;
} RTE_das_all_park_slot_info_S;

typedef RTE_das_all_park_slot_info_S RTE_das_all_park_slot_info_T[8];

typedef struct
{
    uint8 number_selectable_slots;
    RTE_das_all_park_slot_info_T selectable_slots_info;
} RTE_das_selectable_slots;
/******************************************************************************/
/*                 Definition of local module constant data                   */
/******************************************************************************/

/******************************************************************************/
/*                      Definition of local types                             */
/******************************************************************************/

typedef enum
{
    APC_GEAR_AUTOMATIC,
    APC_REVERSE_GEAR_REQ,
    APC_R_ENGAGED,
    APC_DRIVE_GEAR_REQ,
    APC_D_ENGAGED,
    APC_IDLE_GEAR,
    APC_MANUAL_GEAR_RQ,
    APC_AUTOGEAR_DELAY
}brkh_apc_guidance_states_E;

typedef enum
{
    GEAR_REQ_WAIT,
    GEAR_REQ_STANDSTILL,
    GEAR_REQ_END_CHECK
}brkh_apc_guidance_gear_request_substates_E;

typedef enum
{
    GEAR_ENGAGED_INIT,
    GEAR_ENGAGED_CHECK,
    GEAR_ENGAGED_DRIVE,
    GEAR_ENGAGED_STILL,
    GEAR_ENGAGED_CONTINUE
}brkh_apc_guidance_gear_engaged_substates_E;


typedef enum
{
    APC_STAT_SEARCH,
    APC_STAT_DISCLAIMER,
    APC_STAT_CONTROL
}brkh_apc_states_E;

/******************************************************************************/
/*                   Definition of local module variables                     */
/******************************************************************************/
/** contains the brake state for the STMA module */
static uint8      brkh_brake_state;

static u8 brkh_brake_abort_reason;
static brkh_app_sub_states_E brkh_app_sub_state;
static bool_T                                     brkh_is_brake_ecu_ready;
static brkh_app_states_E                          brkh_app_main_state;
static brkh_active_sub_states_E                   brkh_active_sub_state;
static brkh_apc_guidance_states_E                 brkh_apc_guidance_main_state;
static brkh_apc_guidance_gear_request_substates_E brkh_reverse_gear_request;
static brkh_apc_guidance_gear_request_substates_E brkh_drive_gear_request;
static brkh_apc_guidance_gear_engaged_substates_E brkh_reverse_gear_engaged;
static brkh_apc_guidance_gear_engaged_substates_E brkh_drive_gear_engaged;

static brkh_apc_states_E brkh_apc_states;

static bool_T brkh_activation_request  = FALSE;
static bool_T brkh_Ebrake_activation_request = FALSE;

static bool_T brkh_exit_control = FALSE;

static uint8 brkh_active_timer=0;
static uint16 brkh_active_rdv=0;

/* Outputs to the brake */
static uint16 vehicle_speed_rq = BRKH_IDLE_VEH_SPD_RQ;
static sint16 remaining_distance = BRKH_MAX_REMAINING_DISTANCE;
static park_park_stat_E park_park_stat = PARK_PARK_STAT_IDLE;
static esp_brake_state_E park_brk_stat = BRAKING_STATE_PARKMAN_INIT;
static park_brake_mode_E park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
static park_control_mode_rq_E park_ctrlmd_rq = PARK_CTRL_MODE_RQ_IDLE;
static danger_mode_rq_E park_dangermd_rq = DANGER_MODE_INACTIVE;
static park_guidance_state_E parkguid_stat = PARK_GUIDANCE_STATE_INACTIVE;
static remaining_distance_stat_E remaining_distance_stat = REM_DIST_STAT_NO_STEP;
static gear_request_E gear_position_rq = GEAR_POSITION_RQ_IDLE;
static park_parktype_E park_parktype = PARK_PARKTYPE_IDLE;


/* Datas from remaining distance equivalent to statemate implementation */
static park_parktype_E a_park_parktype;
static brkh_maneuver_type_E brkh_maneuver_type;
static bool_T a_rd_continue_feature=FALSE;
//static brkh_gear_position_E a_rd_gear_last;
static sint16 a_remaining_distance;
static bool_T a_rd_init;
static uint8 a_rd_num_cycles;
static bool_T a_rd_gear_override;
static sint16 a_remaining_distance_last;
static uint32 a_rd_driven_dist_since_init_dth;
static uint32 a_rd_driven_dist_since_init_dtc;
static bool_T a_rd_new_meas = FALSE;
static bool_T a_rd_smoothing_ramp_started=FALSE;
static bool_T a_rd_dth_jump_flag=FALSE;
static sint16 a_rd_dth_at_init;
static sint16 a_rd_dtc_at_init;
static sint16 a_rd_dth_last;
static sint16 a_rd_dth;
static sint32 a_rd_jump_dth;
static bool_T a_rd_dth_jump_flag_reinit;
static sint16 a_rd_dtc;
static sint16 a_rd_dtc_last;
static sint32 a_rd_jump_dtc;
static uint32 a_rd_car_position_last;
static sint16 a_apc_remaining_distance;
static bool_T a_start_of_first_stroke;

static remaining_distance_stat_E a_apc_remaining_distance_state = REM_DIST_STAT_NO_STEP;
static gear_request_E a_apc_gear_pos_rq=GEAR_POSITION_RQ_IDLE;
static gear_position_E last_gear_lever_position;
static bool_T changed_gear_lever_position;
static gear_position_E current_gear_lever_position;

static park_park_stat_E a_park_park_stat;
static bool_T brkh_p2_active=FALSE;
static bool_T brkh_ppb_ready=FALSE;

/******************************************************************************/
/*                      Definition of extern variables                        */
/******************************************************************************/



/******************************************************************************/
/*                     Definition of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of exported constant data                     */
/******************************************************************************/

/******************************************************************************/
/*                   Declaration of local module function prototyps           */
/******************************************************************************/

static bool_T CheckBRKHMainTransition(brkh_app_states_E current_state, brkh_app_states_E* next_state);
static void ExecuteBRKHMainEnteringDuringReaction (brkh_app_states_E new_state);
static void ExecuteBRKHActiveEnteringDuringReaction(brkh_active_sub_states_E new_state);
static bool_T BRKH_EmergencyBrakeRequired(void);
static bool_T BRKH_EmergencyBrakeImminent(void);
static void BRKH_CommonApcEspOutputs(void);
static bool_T BRKH_P4U_Abort(void);
static bool_T BRKH_P4U_Finish(void);
static bool_T BRKH_P4U_AbortTakeOver(void);
static bool_T BRKH_EspAbort(void);
static bool_T BRKH_EmergencyBrakeAbort(void);
static void BRKH_RdCalcDtc(void);
static void BRKH_RdCalcRd(void);
static void BRKH_RdNewMeas(void);
static void BRKH_RdSmoothing(void);
static void BRKH_RdGearOverride(void);
static void BRKH_EvaluateGearPositionRq(void);
static void BRKH_CalculateRemainingDistance(void);
static void BRKH_CalculateDrivenDistance(void);
static void BRKH_RdCheckGearChange(void);
static void BRKH_RdCalcDth(void);
static brkh_apc_guidance_states_E EvaluateDefaultAPCGuidanceState(void);
static brkh_maneuver_type_E BRKH_GetManeuverType(void);
static void ExecuteBRKHMainStateDuring(brkh_app_states_E current_state);
static void ExecuteBRKHApcGuidanceEnteringDuringReaction(brkh_apc_guidance_states_E state);
static uint16 BRKH_EvaluateVehicleSpeedRq(void);
static bool_T IsCarReadyForP4U (void);
static bool_T IsCarReadyForEmergency(void);
static park_park_stat_E BRKH_EvaluateParkParkStat(void);
static bool_T BRKH_GetFrozenSlotInfo(u8 *tmp_id, enum CAPP_spatial_alloc_E *allocation, si16 *car_to_slot_y);
static void BRKH_GetParkSlotInfo(RTE_das_selectable_slots *tmp_slot_buffer);

#if (BRKH_USE_P2_COMMAND == STD_ON)
static gear_request_E BRKH_EvalP2GearChange(void);
#endif

/******************************************************************************/
/* BEGIN Source: */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of local module functions                     */
/******************************************************************************/

#if (BRKH_USE_P2_COMMAND == STD_ON)
static gear_request_E BRKH_EvalP2GearChange(void)
{
    enum CAPP_dr_dir_E tmp_P2_gear;
    tmp_P2_gear = P2DAL_GetCmdTransmissionDir();

    switch (  tmp_P2_gear )
    {
    case CAPP_DD_BACKWARD:
        a_apc_gear_pos_rq = GEAR_POSITION_RQ_R;
        break;
    case CAPP_DD_FORWARD:
        a_apc_gear_pos_rq = GEAR_POSITION_RQ_D;
        break;
    default:
        break;
    }
}
#endif

/******************************************************************************/
/**
 * Checks for emergency brake car abort
 *
 *
 * \return none
 */
/******************************************************************************/

static bool_T BRKH_EmergencyBrakeAbort(void)
{
    bool_T ret = TRUE;
    static bool_T brake_was_active_ppb = FALSE;

    if(((TRUE == brake_was_active_ppb) || (brkh_active_timer >= brkh_active_rdv)) &&
        (BRAKING_STATE_PPB_MODE != COMH_GetEspBrakeState()))
    {
        ret = FALSE;
    }
    if(BRAKING_STATE_PPB_MODE == COMH_GetEspBrakeState())
    {
        brake_was_active_ppb = TRUE;
    }

    return ret;
}

/******************************************************************************/
/**
 * Checks for car availability for P4U
 *
 *
 * \return none
 */
/******************************************************************************/

static bool_T IsCarReadyForP4U (void)
{

    bool_T ret = FALSE;

    static bool_T brake_was_active;
    if(FALSE == BRKH_ParkEnblStatEspAPC())
    {
        ret = FALSE;
    }
    else
    {
        switch(brkh_apc_states)
        {
        case APC_STAT_SEARCH:
        case APC_STAT_DISCLAIMER:
            switch(brkh_app_main_state)
            {
            case BRKH_APP_NA:
            case BRKH_APP_INIT:
            case BRKH_APP_READY:
                brkh_brake_state = FALSE;
                break;
            case BRKH_APP_FOLLOW_UP:
                if((BRAKING_STATE_APC_MODE != COMH_GetEspBrakeState()) && (BRAKING_STATE_PPB_MODE == COMH_GetEspBrakeState()))
                {
                    brkh_brake_state = FALSE;
                }
                break;
            case BRKH_APP_BRAKE_ACTIVE:
            default:
                break;
            }
            brake_was_active = FALSE;
            if(((BRAKING_STATE_PARKMAN_INACTIV == COMH_GetEspBrakeState()) || (BRAKING_STATE_APC_MODE == COMH_GetEspBrakeState())
                || (BRAKING_STATE_PPB_MODE == COMH_GetEspBrakeState())) &&
                ((park_park_stat == PARK_PARK_STAT_BACK_STROKE) || (park_park_stat == PARK_PARK_STAT_FWD_STROKE) ||
                                                                      (a_park_park_stat == PARK_PARK_STAT_DISCLAIMER)))
            {
                ret = TRUE;
            }
            break;
        case APC_STAT_CONTROL:
            ret = TRUE;
            if(((TRUE == brake_was_active) || (brkh_active_timer >= brkh_active_rdv))
                                        && (BRAKING_STATE_APC_MODE != COMH_GetEspBrakeState()))
            {
                ret = FALSE;
                brkh_brake_state = FALSE;
            }
            if(BRAKING_STATE_APC_MODE == COMH_GetEspBrakeState())
            {
                brkh_brake_state = TRUE;
                brake_was_active = TRUE;
            }
            break;
        default:
            break;

        }
    }
    return ret;
}


static park_park_stat_E BRKH_EvaluateParkParkStat(void)
{
    return a_park_park_stat;
}

static void BRKH_CalculateParkParkStat(void)
{

    switch(brkh_apc_states)
    {
    case APC_STAT_SEARCH:
        if(TRUE == P2DAL_IsVehicleStandstill())
        {
            brkh_apc_states = APC_STAT_DISCLAIMER;
            a_park_park_stat = PARK_PARK_STAT_FOUND;

        }
        else if((GEAR_POSITION_D == current_gear_lever_position) && (PARK_IN == BRKH_GetManeuverType()))
        {
            a_park_park_stat = PARK_PARK_STAT_SEARCH;
        }
        else if((GEAR_POSITION_R == current_gear_lever_position) && (PARK_IN == BRKH_GetManeuverType()))
        {
            a_park_park_stat = PARK_PARK_STAT_IDLE;
        }
        else if((PARK_OUT == BRKH_GetManeuverType()))
        {
            a_park_park_stat = PARK_PARK_STAT_PARK_OUT;
        }
        break;
    case APC_STAT_DISCLAIMER:
        a_park_park_stat = PARK_PARK_STAT_DISCLAIMER;
        if((TRUE == IsCarReadyForP4U()) && ((GEAR_POSITION_R == current_gear_lever_position) || ((GEAR_POSITION_N == current_gear_lever_position)))
            && (PARK_IN == BRKH_GetManeuverType()))
        {
            a_park_park_stat = PARK_PARK_STAT_BACK_STROKE;
        }
        else if((TRUE == IsCarReadyForP4U()) && (GEAR_POSITION_R == current_gear_lever_position) && (PARK_OUT == BRKH_GetManeuverType()))
        {
            a_park_park_stat = PARK_PARK_STAT_BACK_STROKE;
        }
        else if((TRUE == IsCarReadyForP4U()) && (GEAR_POSITION_D == current_gear_lever_position) && (PARK_OUT == BRKH_GetManeuverType()))
        {
            a_park_park_stat = PARK_PARK_STAT_FWD_STROKE;
        }
        else
        {

        }
        switch(brkh_active_sub_state)
        {
        case BRKH_ACTIVE_P4U_ONLY:
        case BRKH_ACTIVE_P4U_EMERGENCY_PREFILL:
        case BRKH_ACTIVE_P4U_EMERGENCY_BRAKE:
            brkh_apc_states = APC_STAT_CONTROL;
            break;
        default:
            break;
        }
        break;

    case APC_STAT_CONTROL:
        if(GEAR_POSITION_R == current_gear_lever_position)
        {
            a_park_park_stat = PARK_PARK_STAT_BACK_STROKE;
        }
        else if (GEAR_POSITION_D == current_gear_lever_position)
        {
            a_park_park_stat = PARK_PARK_STAT_FWD_STROKE;
        }
        else
        {
            /* Leave old value */
        }
        if(brkh_app_main_state != BRKH_APP_BRAKE_ACTIVE)
        {
            brkh_apc_states = APC_STAT_SEARCH;
        }
        break;
    default:
        break;



    }

}

/******************************************************************************/
/**
 * Checks for car availability for emergency braking
 *
 *
 * \return none
 */
/******************************************************************************/

static bool_T IsCarReadyForEmergency(void)
{
    bool_T ret = TRUE;

    if(FALSE == BRKH_ParkEnblStatEspPPB())
    {
        ret = FALSE;
    }
    else if(ESP_INTERVENTION_ACTIVE == COMH_GetESPIntervention())
    {
        ret = FALSE;
    }
    else if (ESP_STATE_ESP_FLT == COMH_GetParkFltStatEsp())
    {
        ret = FALSE;
    }
    else if((ESP_SYSTEM_STATE_ERROR == COMH_GetEspSystemState()) || (ESP_SYSTEM_STATE_DIAGNOSTIC == COMH_GetEspSystemState()))
    {
        ret = FALSE;
    }
    else
    {

    }

    return ret;
}

/******************************************************************************/
/**
 * calculates vehicle speed request during the maneuver
 *
 *
 * \return none
 */
/******************************************************************************/

static uint16 BRKH_EvaluateVehicleSpeedRq(void)
{
    uint16 ret;
#if (USE_ONLY_P2_RECOMMENDED_SPEED == STD_OFF)
    if (P2DAL_GetDrivableVelInterpMms() < BRKH_SPEED_MINIMUM_APC_MMS)
    {
        /* Assign signed value to unsigned and
         * Convert from mm/s to km/h with 0.01 scaling. 7km/h = 700.
         */

        ret = BRKH_MIN_APC_SPEED;
    }
    else
    {
#endif
        ret = (P2DAL_GetDrivableVelInterpMms() * 36) /100;
#if (USE_ONLY_P2_RECOMMENDED_SPEED == STD_OFF)
    }
#endif

    return ret;
}

/******************************************************************************/
/**
 * Evaluates parktype calculated at the beginning of the maneuver
 *
 *
 * \return none
 */
/******************************************************************************/
void BRKH_EvaluateParktype(void)
{
    u8 tmp_id;
    enum CAPP_maneuver_strategy_E tmp_ms;

    P2DAL_GetFreezeInfo(&tmp_id, &tmp_ms);

    switch (tmp_ms)
    {
    case CAPP_MS_OAF:
        a_park_parktype = PARK_PARKTYPE_OUT_LONG;
        break;
    case CAPP_MS_PAF:
    case CAPP_MS_PAB:
        a_park_parktype = PARK_PARKTYPE_IN_LONG;
        break;
    case CAPP_MS_OEF:
    case CAPP_MS_OEB:
        a_park_parktype =PARK_PARKTYPE_OUT_LTRL;
        break;
    case CAPP_MS_PEF:
    case CAPP_MS_PEB:
        a_park_parktype = PARK_PARKTYPE_IN_LTRL;
        break;
    case CAPP_MS_GPF:
    case CAPP_MS_GPB:
    case CAPP_MS_EPA:
    case CAPP_MS_EWA:
    case CAPP_MS_EXP:
    case CAPP_MS_UNKNOWN:
    default:
        a_park_parktype = PARK_PARKTYPE_IN_LONG;
        break;
    }


}

/******************************************************************************/
/**
 * Returns parktype calculated at the beginning of the maneuver
 *
 *
 * \return none
 */
/******************************************************************************/
park_parktype_E BRKH_GetParktype(void)
{
    return a_park_parktype;
}

/******************************************************************************/
/**
 * Check for default APC Guidance state
 *
 *
 * \return none
 */
/******************************************************************************/
static brkh_apc_guidance_states_E EvaluateDefaultAPCGuidanceState(void)
{
    brkh_apc_guidance_states_E ret;

    if((PARK_OUT == BRKH_GetManeuverType()) && (GEAR_POSITION_R == current_gear_lever_position)
        && (CAPP_DD_FORWARD == P2DAL_GetDrivingHint())

    )
    {
        ret = APC_DRIVE_GEAR_REQ;
        brkh_drive_gear_request = GEAR_REQ_WAIT;
    }
    else if((PARK_OUT == BRKH_GetManeuverType()) && (GEAR_POSITION_D == current_gear_lever_position)
        && (CAPP_DD_BACKWARD == P2DAL_GetDrivingHint())
        )
    {
        ret = APC_REVERSE_GEAR_REQ;
        brkh_reverse_gear_request = GEAR_REQ_WAIT;
    }
    else
    {
        ret = APC_GEAR_AUTOMATIC;

    }
    ExecuteBRKHApcGuidanceEnteringDuringReaction(ret);

    return ret;
}

/******************************************************************************/
/**
 * Transition check for State machine for gear position request
 *
 *
 * \return none
 */
/******************************************************************************/

static bool_T CheckBRKHApcGuidanceTransition(brkh_apc_guidance_states_E current_state, brkh_apc_guidance_states_E* next_state)
{
    bool_T ret = FALSE;

    static uint8 apc_guidance_timer=0;
    static uint16 apc_guidance_rdv=0;

    switch(current_state)
    {
    case APC_GEAR_AUTOMATIC:
        if(((GEAR_POSITION_D == current_gear_lever_position) && (CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (0 == P2DAL_GetDistToHint()))
            || ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() > BRKH_MIN_REMAINING_DISTANCE))
            || ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() <= BRKH_MIN_REMAINING_DISTANCE))
            )
        {
            ret = TRUE;
            *next_state = APC_REVERSE_GEAR_REQ;
            brkh_reverse_gear_request = GEAR_REQ_WAIT;
        }
        else if(((GEAR_POSITION_R == current_gear_lever_position) && (CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (0 == P2DAL_GetDistToHint()))
            || ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() > BRKH_MIN_REMAINING_DISTANCE))
            || ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() <= BRKH_MIN_REMAINING_DISTANCE))
            )
        {
            ret = TRUE;
            *next_state = APC_DRIVE_GEAR_REQ;
            brkh_drive_gear_request = GEAR_REQ_WAIT;
        }
        else if((CAPP_DD_UNKNOWN == P2DAL_GetDrivingHint()) && (TRUE == P2DAL_IsVehicleStandstill()))
        {
            ret = TRUE;
            *next_state = APC_IDLE_GEAR;
        }
        break;

    case APC_REVERSE_GEAR_REQ:
        switch(brkh_reverse_gear_request)
        {
        case GEAR_REQ_WAIT:
            if( current_gear_lever_position == GEAR_POSITION_R )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_R_ENGAGED;
                brkh_reverse_gear_engaged = GEAR_ENGAGED_INIT;
                apc_guidance_rdv = apc_guidance_timer + BRKH_DLY_REMDIST_TOLERANCE;
            }
            else if ((changed_gear_lever_position) && (current_gear_lever_position == GEAR_POSITION_N) )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_MANUAL_GEAR_RQ;
            }
            else if((CAPP_DD_UNKNOWN == P2DAL_GetDrivingHint()) && (TRUE == P2DAL_IsVehicleStandstill()))
            {
                *next_state = APC_IDLE_GEAR;
                ret = TRUE;
                a_start_of_first_stroke = FALSE;
            }
            else if(TRUE == P2DAL_IsVehicleStandstill())
            {
                brkh_reverse_gear_request = GEAR_REQ_STANDSTILL;
                apc_guidance_rdv = apc_guidance_timer + BRKH_DLY_STANDSTILL;
            }
            else
            {

            }
            break;
        case GEAR_REQ_STANDSTILL:
            if( current_gear_lever_position == GEAR_POSITION_R )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_R_ENGAGED;
                brkh_reverse_gear_engaged = GEAR_ENGAGED_INIT;
                apc_guidance_rdv = apc_guidance_timer + BRKH_DLY_REMDIST_TOLERANCE;
            }
            else if ((changed_gear_lever_position) && (current_gear_lever_position == GEAR_POSITION_N) )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_MANUAL_GEAR_RQ;
            }
            else if((TRUE == P2DAL_IsVehicleStandstill()) && (CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) &&
                (P2DAL_GetDistToHint()> BRKH_MIN_REMAINING_DISTANCE) && (P2DAL_GetDistToCollision()>BRKH_MIN_REMAINING_DISTANCE))
            {
                *next_state = APC_D_ENGAGED;
                ret = TRUE;
                a_rd_continue_feature = TRUE;
                a_start_of_first_stroke = FALSE;
                brkh_drive_gear_engaged = GEAR_ENGAGED_DRIVE;

            }
            else if(apc_guidance_timer >= apc_guidance_rdv)
            {
                brkh_reverse_gear_request = GEAR_REQ_END_CHECK;
            }
            else
            {

            }
            break;
        case GEAR_REQ_END_CHECK:
            if( current_gear_lever_position == GEAR_POSITION_R )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_R_ENGAGED;
                brkh_reverse_gear_engaged = GEAR_ENGAGED_INIT;
                apc_guidance_rdv = apc_guidance_timer + BRKH_DLY_REMDIST_TOLERANCE;
            }
            else if ((changed_gear_lever_position) && (current_gear_lever_position == GEAR_POSITION_N) )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_MANUAL_GEAR_RQ;
            }
            else
            {

            }
            break;
        default:
            if( current_gear_lever_position == GEAR_POSITION_R )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_R_ENGAGED;
                brkh_reverse_gear_engaged = GEAR_ENGAGED_INIT;
                apc_guidance_rdv = apc_guidance_timer + BRKH_DLY_REMDIST_TOLERANCE;
            }
            else if ((changed_gear_lever_position) && (current_gear_lever_position == GEAR_POSITION_N) )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_MANUAL_GEAR_RQ;
            }
            else
            {

            }
            break;
        }
        break;
    case APC_R_ENGAGED:
        switch (brkh_reverse_gear_engaged)
        {
        case GEAR_ENGAGED_INIT:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_R != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if(apc_guidance_timer >= apc_guidance_rdv)
            {
                brkh_reverse_gear_engaged = GEAR_ENGAGED_CHECK;
            }
            else
            {

            }
            break;
        case GEAR_ENGAGED_CHECK:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_R != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if(FALSE == P2DAL_IsSteerActiveInStandstill())
            {
                brkh_reverse_gear_engaged = GEAR_ENGAGED_DRIVE;
            }
            else
            {

            }
            break;
        case GEAR_ENGAGED_DRIVE:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_R != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if( ((FALSE == changed_gear_lever_position) || (GEAR_POSITION_R != current_gear_lever_position)) &&
                ( (DAPM_OS_PARKING_ACTIVE == P2DAL_GetOperationState()) &&
                    ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) ||
                        ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (a_remaining_distance<=BRKH_MIN_REMAINING_DISTANCE))))
                        )
            {
                ret = TRUE;
                *next_state = APC_DRIVE_GEAR_REQ;
                brkh_drive_gear_request = GEAR_REQ_WAIT;
            }
            else if(TRUE == P2DAL_IsVehicleStandstill())
            {
                brkh_reverse_gear_engaged = GEAR_ENGAGED_STILL;
            }
            else
            {

            }
            break;
        case GEAR_ENGAGED_STILL:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_R != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if( ((FALSE == changed_gear_lever_position) || (GEAR_POSITION_R != current_gear_lever_position)) &&
                ( (DAPM_OS_PARKING_ACTIVE == P2DAL_GetOperationState()) &&
                    ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) ||
                        ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (a_remaining_distance<=BRKH_MIN_REMAINING_DISTANCE))))
                        )
            {
                ret = TRUE;
                *next_state = APC_DRIVE_GEAR_REQ;
                brkh_drive_gear_request = GEAR_REQ_WAIT;
            }
            else if((TRUE == P2DAL_IsVehicleStandstill()) && (TRUE == a_rd_new_meas) &&
                    (P2DAL_GetDistToHint()>BRKH_MIN_REMAINING_DISTANCE) && (P2DAL_GetDistToCollision()>BRKH_MIN_REMAINING_DISTANCE))
            {
                a_rd_continue_feature = TRUE;
                brkh_reverse_gear_engaged = GEAR_ENGAGED_CONTINUE;
            }
            else if(FALSE == P2DAL_IsVehicleStandstill())
            {
                brkh_reverse_gear_engaged = GEAR_ENGAGED_DRIVE;
            }
            else
            {

            }
            break;
        case GEAR_ENGAGED_CONTINUE:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_R != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if( ((FALSE == changed_gear_lever_position) || (GEAR_POSITION_R != current_gear_lever_position)) &&
                ( (DAPM_OS_PARKING_ACTIVE == P2DAL_GetOperationState()) &&
                    ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) ||
                        ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (a_remaining_distance<=BRKH_MIN_REMAINING_DISTANCE))))
                        )
            {
                ret = TRUE;
                *next_state = APC_DRIVE_GEAR_REQ;
                brkh_drive_gear_request = GEAR_REQ_WAIT;
            }
            else if (FALSE == P2DAL_IsVehicleStandstill())
            {
                brkh_reverse_gear_engaged = GEAR_ENGAGED_DRIVE;
            }
            else
            {

            }
            break;
        default:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_D != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            break;
        }
        break;
    case APC_DRIVE_GEAR_REQ:
        switch(brkh_drive_gear_request)
        {
        case GEAR_REQ_WAIT:
            if( current_gear_lever_position == GEAR_POSITION_D )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_D_ENGAGED;
            }
            else if ((changed_gear_lever_position) && (current_gear_lever_position == GEAR_POSITION_N) )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_MANUAL_GEAR_RQ;
            }
            else if((CAPP_DD_UNKNOWN == P2DAL_GetDrivingHint()) && (TRUE == P2DAL_IsVehicleStandstill()))
            {
                *next_state = APC_IDLE_GEAR;
                ret = TRUE;
                a_start_of_first_stroke = FALSE;
            }
            else if(TRUE == P2DAL_IsVehicleStandstill())
            {
                brkh_drive_gear_request = GEAR_REQ_STANDSTILL;
                apc_guidance_rdv = apc_guidance_timer + BRKH_DLY_STANDSTILL;
            }
            else
            {

            }
            break;
        case GEAR_REQ_STANDSTILL:
            if( current_gear_lever_position == GEAR_POSITION_D )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_D_ENGAGED;
            }
            else if ((changed_gear_lever_position) && (current_gear_lever_position == GEAR_POSITION_N) )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_MANUAL_GEAR_RQ;
            }
            else if((TRUE == P2DAL_IsVehicleStandstill()) && (CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) &&
                (P2DAL_GetDistToHint()> BRKH_MIN_REMAINING_DISTANCE) && (P2DAL_GetDistToCollision()>BRKH_MIN_REMAINING_DISTANCE))
            {
                *next_state = APC_R_ENGAGED;
                ret = TRUE;
                a_rd_continue_feature = TRUE;
                a_start_of_first_stroke = FALSE;
                brkh_reverse_gear_engaged = GEAR_ENGAGED_DRIVE;

            }
            else if(apc_guidance_timer >= apc_guidance_rdv)
            {
                brkh_drive_gear_request = GEAR_REQ_END_CHECK;
            }
            else
            {

            }
            break;
        case GEAR_REQ_END_CHECK:
            if( current_gear_lever_position == GEAR_POSITION_D )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_D_ENGAGED;
            }
            else if ((changed_gear_lever_position) && (current_gear_lever_position == GEAR_POSITION_N) )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_MANUAL_GEAR_RQ;
            }
            else
            {

            }
            break;
        default:
            if( current_gear_lever_position == GEAR_POSITION_D )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_D_ENGAGED;
            }
            else if ((changed_gear_lever_position) && (current_gear_lever_position == GEAR_POSITION_N) )
            {
                a_start_of_first_stroke = FALSE;
                ret = TRUE;
                *next_state = APC_MANUAL_GEAR_RQ;
            }
            else
            {

            }
            break;
        }
        break;
    case APC_D_ENGAGED:
        switch (brkh_drive_gear_engaged)
        {
        case GEAR_ENGAGED_INIT:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_D != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if(apc_guidance_timer >= apc_guidance_rdv)
            {
                brkh_drive_gear_engaged = GEAR_ENGAGED_CHECK;
            }
            break;
        case GEAR_ENGAGED_CHECK:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_D != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if(FALSE == P2DAL_IsSteerActiveInStandstill())
            {
                brkh_drive_gear_engaged = GEAR_ENGAGED_DRIVE;
            }
            break;
        case GEAR_ENGAGED_DRIVE:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_D != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if( ((FALSE == changed_gear_lever_position) || (GEAR_POSITION_D != current_gear_lever_position)) &&
                ( (DAPM_OS_PARKING_ACTIVE == P2DAL_GetOperationState()) &&
                    ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) ||
                        ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (a_remaining_distance<=BRKH_MIN_REMAINING_DISTANCE))))
                        )
            {
                ret = TRUE;
                *next_state = APC_REVERSE_GEAR_REQ;
                brkh_reverse_gear_request = GEAR_REQ_WAIT;
            }
            else if(TRUE == P2DAL_IsVehicleStandstill())
            {
                brkh_drive_gear_engaged = GEAR_ENGAGED_STILL;
            }
            else
            {

            }
            break;
        case GEAR_ENGAGED_STILL:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_D != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if( ((FALSE == changed_gear_lever_position) || (GEAR_POSITION_D != current_gear_lever_position)) &&
                ( (DAPM_OS_PARKING_ACTIVE == P2DAL_GetOperationState()) &&
                    ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) ||
                        ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (a_remaining_distance<=BRKH_MIN_REMAINING_DISTANCE))))
                        )
            {
                ret = TRUE;
                *next_state = APC_REVERSE_GEAR_REQ;
                brkh_reverse_gear_request = GEAR_REQ_WAIT;
            }
            else if((TRUE == P2DAL_IsVehicleStandstill()) && (TRUE == a_rd_new_meas) &&
                    (P2DAL_GetDistToHint()>BRKH_MIN_REMAINING_DISTANCE) && (P2DAL_GetDistToCollision()>BRKH_MIN_REMAINING_DISTANCE))
            {
                a_rd_continue_feature = TRUE;
                brkh_drive_gear_engaged = GEAR_ENGAGED_CONTINUE;
            }
            else if(FALSE == P2DAL_IsVehicleStandstill())
            {
                brkh_drive_gear_engaged = GEAR_ENGAGED_DRIVE;
            }
            else
            {

            }
            break;
        case GEAR_ENGAGED_CONTINUE:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_D != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            else if( ((FALSE == changed_gear_lever_position) || (GEAR_POSITION_D != current_gear_lever_position)) &&
                ( (DAPM_OS_PARKING_ACTIVE == P2DAL_GetOperationState()) &&
                    ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) ||
                        ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (a_remaining_distance<=BRKH_MIN_REMAINING_DISTANCE))))
                        )
            {
                ret = TRUE;
                *next_state = APC_REVERSE_GEAR_REQ;
                brkh_reverse_gear_request = GEAR_REQ_WAIT;
            }
            else if (FALSE == P2DAL_IsVehicleStandstill())
            {
                brkh_drive_gear_engaged = GEAR_ENGAGED_DRIVE;
            }
            break;
        default:
            if((TRUE == changed_gear_lever_position) && (GEAR_POSITION_D != current_gear_lever_position))
            {
                *next_state = APC_MANUAL_GEAR_RQ;
                ret = TRUE;
            }
            break;
        }
        break;
    case APC_IDLE_GEAR:
        if(  ((PARK_IN == BRKH_GetManeuverType()) &&
                  ( ( (current_gear_lever_position == GEAR_POSITION_D) &&
                        (CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() == 0) )
                 || ( ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() > BRKH_MIN_REMAINING_DISTANCE)) ||
                      ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() <= BRKH_MIN_REMAINING_DISTANCE)) ) ) )
          || ((PARK_OUT == BRKH_GetManeuverType()) &&
                  ( ( (current_gear_lever_position == GEAR_POSITION_D) &&
                         ( (CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) || (P2DAL_GetDistToHint() <= 0)) )
                 || ( (current_gear_lever_position == GEAR_POSITION_R) &&
                          (P2DAL_GetDistToHint() >= BRKH_MIN_REMAINING_DISTANCE)) ) )
          )
        {
            *next_state = APC_REVERSE_GEAR_REQ;
            brkh_reverse_gear_request = GEAR_REQ_WAIT;
            ret = TRUE;
        }
        else if(  ((PARK_IN == BRKH_GetManeuverType()) &&
                  ( ( (current_gear_lever_position == GEAR_POSITION_R) &&
                        (CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() == 0) )
                 || ( ((CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() > BRKH_MIN_REMAINING_DISTANCE)) ||
                      ((CAPP_DD_BACKWARD == P2DAL_GetDrivingHint()) && (P2DAL_GetDistToHint() <= BRKH_MIN_REMAINING_DISTANCE)) ) ) )
          || ((PARK_OUT == BRKH_GetManeuverType()) &&
                  ( ( (current_gear_lever_position == GEAR_POSITION_R) &&
                         ( (CAPP_DD_FORWARD == P2DAL_GetDrivingHint()) || (P2DAL_GetDistToHint() <= 0)) )
                 || ( (current_gear_lever_position == GEAR_POSITION_D) &&
                          (P2DAL_GetDistToHint() >= BRKH_MIN_REMAINING_DISTANCE)) ) )
          )
        {
            *next_state = APC_DRIVE_GEAR_REQ;
            brkh_drive_gear_request = GEAR_REQ_WAIT;
            ret = TRUE;
        }
        else
        {

        }
        break;
    case APC_MANUAL_GEAR_RQ:
        if((FALSE == P2DAL_IsSteerActiveInStandstill()) &&
            ( ((GEAR_POSITION_R == COMH_GetCarCurrentGear()) && (GEAR_POSITION_R == current_gear_lever_position))
            ||((GEAR_POSITION_D == COMH_GetCarCurrentGear()) && (GEAR_POSITION_D == current_gear_lever_position)) )
        )
        {
            *next_state = APC_AUTOGEAR_DELAY;
            ret = TRUE;
            apc_guidance_rdv = apc_guidance_timer + BRKH_DLY_AUTOGEAR;
        }
        break;
    case APC_AUTOGEAR_DELAY:
        if(apc_guidance_timer >= apc_guidance_rdv)
        {
            *next_state = APC_GEAR_AUTOMATIC;
            ret = TRUE;
        }
        break;
    default:
        break;
    }

    if(apc_guidance_timer != BRKH_UINT8_MAX)
    {
        apc_guidance_timer++;
    }
    else
    {
        apc_guidance_timer=0;
        if(apc_guidance_rdv > BRKH_UINT8_MAX)
        {
            apc_guidance_rdv -= (uint16) ((uint16) 1 + BRKH_UINT8_MAX);
        }
    }
    if (apc_guidance_timer >= apc_guidance_rdv)
    {
        apc_guidance_rdv = 0;
    }

    return ret;
}


/******************************************************************************/
/**
 * Entering & during reaction for gear change statemachine request since there is no difference here
 *
 *
 * \return none
 */
/******************************************************************************/

static void ExecuteBRKHApcGuidanceEnteringDuringReaction(brkh_apc_guidance_states_E state)
{
    switch(state)
    {
    case APC_GEAR_AUTOMATIC:
        if(a_start_of_first_stroke)
        {
            a_apc_remaining_distance = 0;
        }
        else
        {
            a_apc_remaining_distance = a_remaining_distance;
        }
        switch(current_gear_lever_position)
        {
        case GEAR_POSITION_N:
            a_apc_gear_pos_rq = GEAR_POSITION_RQ_N;
            break;
        case GEAR_POSITION_P:
            a_apc_gear_pos_rq = GEAR_POSITION_RQ_P;
            break;
        case GEAR_POSITION_D:
            a_apc_gear_pos_rq = GEAR_POSITION_RQ_D;
            break;
        case GEAR_POSITION_R:
            a_apc_gear_pos_rq = GEAR_POSITION_RQ_R;
            break;
        default:
        break;
        }
        break;

    case APC_R_ENGAGED:
        a_apc_gear_pos_rq = GEAR_POSITION_RQ_R;

        switch(brkh_reverse_gear_engaged)
        {
        case GEAR_ENGAGED_INIT:
        case GEAR_ENGAGED_CHECK:
            if(TRUE == P2DAL_IsSteerActiveInStandstill())
            {
                a_apc_remaining_distance=0;
            }
            else
            {
                a_apc_remaining_distance = a_remaining_distance;
            }
            break;
        case GEAR_ENGAGED_DRIVE:
        case GEAR_ENGAGED_STILL:
        case GEAR_ENGAGED_CONTINUE:
            a_apc_remaining_distance = a_remaining_distance;
            break;
        default:
            break;
        }
        break;

    case APC_DRIVE_GEAR_REQ:

        if(a_start_of_first_stroke)
        {
            a_apc_remaining_distance = 0;
        }
        else
        {
            a_apc_remaining_distance = a_remaining_distance;
        }
        switch(brkh_drive_gear_request)
        {
        case GEAR_REQ_WAIT:
            break;

        case GEAR_REQ_END_CHECK:
            if((TRUE == P2DAL_IsVehicleStandstill()) && (DAPM_OS_PARKING_ACTIVE == P2DAL_GetOperationState()))
            {
                a_apc_gear_pos_rq = GEAR_POSITION_RQ_D;
            }
            break;
        case GEAR_REQ_STANDSTILL:
        default:
            break;
        }
        break;
    case APC_D_ENGAGED:
        a_apc_gear_pos_rq = GEAR_POSITION_RQ_D;
        switch(brkh_drive_gear_engaged)
        {
        case GEAR_ENGAGED_INIT:
        case GEAR_ENGAGED_CHECK:
            if(TRUE == P2DAL_IsSteerActiveInStandstill())
            {
                a_apc_remaining_distance=0;
            }
            else
            {
                a_apc_remaining_distance = a_remaining_distance;
            }
            break;
        case GEAR_ENGAGED_DRIVE:
        case GEAR_ENGAGED_STILL:
        case GEAR_ENGAGED_CONTINUE:
            a_apc_remaining_distance = a_remaining_distance;
            break;
        default:
            break;
        }
        break;
    case APC_IDLE_GEAR:
        if(TRUE == a_start_of_first_stroke)
        {
            a_apc_remaining_distance = 0;
        }
        else
        {
            a_apc_remaining_distance=a_remaining_distance;
        }
        switch(current_gear_lever_position)
        {
        case GEAR_POSITION_N:
            a_apc_gear_pos_rq = GEAR_POSITION_RQ_N;
            break;
        case GEAR_POSITION_P:
            a_apc_gear_pos_rq = GEAR_POSITION_RQ_P;
            break;
        case GEAR_POSITION_D:
            a_apc_gear_pos_rq = GEAR_POSITION_RQ_D;
            break;
        case GEAR_POSITION_R:
            a_apc_gear_pos_rq = GEAR_POSITION_RQ_R;
            break;
        default:
            break;
        }
        break;
    case APC_MANUAL_GEAR_RQ:
        a_apc_remaining_distance = 0;
        break;

    case APC_AUTOGEAR_DELAY:
        a_apc_remaining_distance = a_remaining_distance;
        break;
    case APC_REVERSE_GEAR_REQ:
        if(a_start_of_first_stroke)
        {
            a_apc_remaining_distance = 0;
        }
        else
        {
            a_apc_remaining_distance = a_remaining_distance;
        }
        switch(brkh_reverse_gear_request)
        {
        case GEAR_REQ_WAIT:
            break;

        case GEAR_REQ_END_CHECK:
            if((TRUE == P2DAL_IsVehicleStandstill()) && (DAPM_OS_PARKING_ACTIVE == P2DAL_GetOperationState()))
            {
                a_apc_gear_pos_rq = GEAR_POSITION_RQ_R;
            }
            break;
        case GEAR_REQ_STANDSTILL:
        default:
            break;
        }
        break;

    default:
        break;
    }
}

/******************************************************************************/
/**
 * State machine for gear position request
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_EvaluateGearPositionRq(void)
{
    brkh_apc_guidance_states_E next_state;

    switch(brkh_apc_guidance_main_state)
    {
    case APC_GEAR_AUTOMATIC:
    case APC_REVERSE_GEAR_REQ:
    case APC_R_ENGAGED:
    case APC_DRIVE_GEAR_REQ:
    case APC_D_ENGAGED:
    case APC_IDLE_GEAR:
    case APC_MANUAL_GEAR_RQ:
    case APC_AUTOGEAR_DELAY:
        if(TRUE == CheckBRKHApcGuidanceTransition(brkh_apc_guidance_main_state, &next_state))
        {
            ExecuteBRKHApcGuidanceEnteringDuringReaction(next_state);
            brkh_apc_guidance_main_state = next_state;
        }
        else
        {
            ExecuteBRKHApcGuidanceEnteringDuringReaction(brkh_apc_guidance_main_state);
        }
        break;
    default:
        break;
    }


}

/******************************************************************************/
/**
 * RD_CHECK_GEAR_CHANGE
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_RdCheckGearChange(void)
{
/* RD_CHECK_GEAR_CHANGE */
    if(a_rd_continue_feature == TRUE)
    {
        a_rd_continue_feature = FALSE;
        a_remaining_distance = BRKH_MAX_REMAINING_DISTANCE;
        a_rd_num_cycles=0;
        a_rd_gear_override=FALSE;
        a_rd_init=FALSE;
        a_rd_driven_dist_since_init_dth=0;
        a_rd_driven_dist_since_init_dtc=0;
        a_rd_new_meas=FALSE;
        a_apc_remaining_distance_state=REM_DIST_STAT_NO_STEP;
        a_rd_dth_jump_flag=FALSE;
        a_rd_dth_at_init=P2DAL_GetDistToHint();
        a_rd_dtc_at_init=P2DAL_GetDistToCollision();
        a_rd_smoothing_ramp_started=FALSE;
    }
    else
    {
        if((changed_gear_lever_position) || (TRUE==a_rd_init) || (a_rd_num_cycles>0))
        {
            if((a_rd_num_cycles>=BRKH_RD_MAX_CYCLES) || (TRUE==a_rd_init))
            {
                a_rd_continue_feature = FALSE;
                a_remaining_distance_last = BRKH_MAX_REMAINING_DISTANCE;
                a_rd_num_cycles=0;
                a_rd_gear_override=FALSE;
                a_rd_init=FALSE;
                a_rd_driven_dist_since_init_dth=0;
                a_rd_driven_dist_since_init_dtc=0;
                a_rd_new_meas=FALSE;
                a_apc_remaining_distance_state=REM_DIST_STAT_NO_STEP;
                a_rd_dth_jump_flag=FALSE;
                a_rd_dth_at_init=P2DAL_GetDistToHint();
                a_rd_dtc_at_init=P2DAL_GetDistToCollision();
                a_rd_smoothing_ramp_started=FALSE;
            }
            else
            {
               a_rd_gear_override=TRUE;
               a_rd_num_cycles++;
            }
        }
    }
}

/******************************************************************************/
/**
 * RD_CALC_DTH
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_RdCalcDth(void)
{
    if( ((PARK_OUT == BRKH_GetManeuverType()) && (a_rd_dth_last>BRKH_RD_DTH_JUMP_THRESHOLD_PMO) &&
        (P2DAL_GetDistToHint() >= BRKH_RD_DTH_JUMP_THRESHOLD_PMO)) ||
        ((PARK_IN == BRKH_GetManeuverType()) && (a_rd_dth_last>BRKH_RD_DTH_JUMP_THRESHOLD_PMI) &&
                (P2DAL_GetDistToHint() >= BRKH_RD_DTH_JUMP_THRESHOLD_PMI)) )

    {
        a_rd_dth = P2DAL_GetDistToHint();
    }
    else
    {
        if (((PARK_OUT == BRKH_GetManeuverType()) && (a_rd_dth_last>BRKH_RD_DTH_JUMP_THRESHOLD_PMO) &&
        (P2DAL_GetDistToHint() < BRKH_RD_DTH_JUMP_THRESHOLD_PMO)) ||
        ((PARK_IN == BRKH_GetManeuverType()) && (a_rd_dth_last>BRKH_RD_DTH_JUMP_THRESHOLD_PMI) &&
                (P2DAL_GetDistToHint() < BRKH_RD_DTH_JUMP_THRESHOLD_PMI)) )
        {
            a_rd_driven_dist_since_init_dth=0;
            a_apc_remaining_distance_state=REM_DIST_STAT_NO_STEP;
            a_rd_dth_jump_flag=FALSE;
            a_rd_dth_at_init=P2DAL_GetDistToHint();
            if (PARK_IN == BRKH_GetManeuverType())
            {
                a_rd_jump_dth = BRKH_RD_DTH_JUMP_THRESHOLD_PMI-(a_rd_dth_at_init-a_rd_driven_dist_since_init_dth);
            }
            else
            {
                a_rd_jump_dth = BRKH_RD_DTH_JUMP_THRESHOLD_PMO-(a_rd_dth_at_init-a_rd_driven_dist_since_init_dth);
            }
        }
        else
        {
            a_rd_jump_dth=P2DAL_GetDistToHint() - (a_rd_dth_at_init - a_rd_driven_dist_since_init_dth);
        }

        if((a_rd_jump_dth<=0) && (FALSE == a_rd_dth_jump_flag))
        {
            a_rd_dth = P2DAL_GetDistToHint() - a_rd_jump_dth;
        }
        else
        {
            if(a_rd_jump_dth<=BRKH_RD_JUMP_THRESHOLD)
            {
                a_rd_dth=a_rd_dth_last;
            }
            else
            {
                if((a_rd_dth_last + BRKH_RD_RAMP_VALUE)<=P2DAL_GetDistToHint())
                {
                    a_rd_dth = a_rd_dth_last + BRKH_RD_RAMP_VALUE;
                }
                else
                {
                    a_rd_dth = P2DAL_GetDistToHint();
                }
            }
            if((TRUE == a_rd_dth_jump_flag) && (P2DAL_GetDistToHint() <= a_rd_dth))
            {
                a_rd_dth=a_rd_dth_last;
                a_rd_dth_at_init=a_rd_dth;
                a_rd_driven_dist_since_init_dth=0;
                a_rd_dth_jump_flag_reinit=TRUE;
            }

            a_rd_dth_jump_flag=TRUE;
            if(a_rd_dth_jump_flag_reinit==TRUE)
            {
                a_rd_dth_jump_flag=FALSE;
            }
            a_rd_dth_jump_flag_reinit=FALSE;
        }


    }
}

/******************************************************************************/
/**
 * RD_CALC_DTC
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_RdCalcDtc(void)
{
    if(P2DAL_GetDistToCollision() >= BRKH_MAX_REMAINING_DISTANCE)
    {
        a_rd_dtc = P2DAL_GetDistToCollision();
        a_rd_dtc_at_init = P2DAL_GetDistToCollision();
        a_rd_driven_dist_since_init_dtc = 0;
    }
    else
    {
        a_rd_jump_dtc = P2DAL_GetDistToCollision() - (a_rd_dtc_at_init-a_rd_driven_dist_since_init_dtc);

        if(a_rd_jump_dtc<=0)
        {
            a_rd_dtc=P2DAL_GetDistToCollision();
        }
        else
        {
            if(a_rd_jump_dtc <= BRKH_RD_JUMP_THRESHOLD)
            {
                a_rd_dtc = a_rd_dtc_last;
            }
            else
            {
                if(((sint32)(a_rd_dtc_last+BRKH_RD_RAMP_VALUE)) < BRKH_SINT16_MAX)
                {
                    a_rd_dtc = a_rd_dtc_last + BRKH_RD_RAMP_VALUE;
                }
                else
                {
                    a_rd_dtc = BRKH_SINT16_MAX;
                }
                if((P2DAL_GetDistToCollision()) < a_rd_dtc)
                {
                    a_rd_dtc_at_init=P2DAL_GetDistToCollision();
                    a_rd_driven_dist_since_init_dtc=0;
                }
            }
        }
    }
}

/******************************************************************************/
/**
 * RD_CALC_RD
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_RdCalcRd(void)
{
    if(a_rd_dth<=a_rd_dtc)
    {
        a_remaining_distance=a_rd_dth;
    }
    else
    {
        a_remaining_distance=a_rd_dtc;
    }
}

/******************************************************************************/
/**
 * RD_NEW_MEAS
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_RdNewMeas(void)
{
    if((P2DAL_GetDistToHint() < 0) || (a_rd_new_meas==TRUE))
    {
        a_rd_new_meas=TRUE;
        a_apc_remaining_distance_state=REM_DIST_STAT_NEW_MEASURE;
        if(P2DAL_GetDistToCollision() < a_rd_dth)
        {
            a_remaining_distance = P2DAL_GetDistToCollision();
        }
        else
        {
            a_remaining_distance=a_rd_dth;
        }
    }
}

/******************************************************************************/
/**
 * RD_SMOOTHING
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_RdSmoothing(void)
{
    if((a_remaining_distance_last >= a_remaining_distance) || (a_remaining_distance >= BRKH_MAX_REMAINING_DISTANCE))
    {
        a_rd_smoothing_ramp_started=FALSE;
    }
    else
    {
        if((a_rd_smoothing_ramp_started == FALSE) && ((sint32)((sint32)a_remaining_distance_last + (sint32)BRKH_RD_JUMP_THRESHOLD)>a_remaining_distance))
        {
            /* TODO if exploration mode then do nothing */
            a_remaining_distance=a_remaining_distance_last;
        }
        else
        {
            a_remaining_distance=a_remaining_distance_last+BRKH_RD_RAMP_VALUE;
            a_rd_smoothing_ramp_started=TRUE;
        }
    }
}

/******************************************************************************/
/**
 * RD_GEAR_OVERRIDE
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_RdGearOverride(void)
{
    if(TRUE==a_rd_gear_override)
    {
        a_remaining_distance = a_remaining_distance_last;
    }
    else
    {
        a_rd_dth_last = a_rd_dth;
        a_rd_dtc_last = a_rd_dtc;
        a_remaining_distance_last = a_remaining_distance;
    }
}

/******************************************************************************/
/**
 * Calculation for driven distance
 *
 *
 * \return none
 */
/******************************************************************************/
static void BRKH_CalculateDrivenDistance(void)
{
    struct CAPP_position32_S tmp_position;
    P2DAL_GetGlobalVehiclePosition(&tmp_position);
    a_rd_driven_dist_since_init_dth += (tmp_position.s_mm - a_rd_car_position_last);
    a_rd_driven_dist_since_init_dtc += (tmp_position.s_mm - a_rd_car_position_last);
    a_rd_car_position_last = tmp_position.s_mm;
}

/******************************************************************************/
/**
 * Calculation for remaining distance
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_CalculateRemainingDistance(void)
{
    BRKH_CalculateDrivenDistance();
    BRKH_RdCheckGearChange();
    BRKH_RdCalcDth();
    BRKH_RdCalcDtc();
    BRKH_RdCalcRd();
    BRKH_RdNewMeas();
    BRKH_RdSmoothing();
    BRKH_RdGearOverride();
}

/******************************************************************************/
/**
 * Check ESP abort
 *
 *
 * \return bool_T
 */
/******************************************************************************/

static bool_T BRKH_EspAbort(void)
{
    bool_T ret=FALSE;


    if((ESP_INTERVENTION_ACTIVE == COMH_GetESPIntervention()) || (FALSE == IsCarReadyForP4U()))
    {
        ret = TRUE;
    }

    return ret;
}

/******************************************************************************/
/**
 * Check the emergency brake imminent flag from P2
 *
 *
 * \return bool_T
 */
/******************************************************************************/
static bool_T BRKH_EmergencyBrakeRequired()
{
    bool_T ret;

    ret = P2DAL_IsEmergencyBrakeRequired();

    return ret;
}
/******************************************************************************/
/**
 * Check the emergency brake required flag from P2
 *
 *
 * \return bool_T
 */
/******************************************************************************/
static bool_T BRKH_EmergencyBrakeImminent()
{
    bool_T ret;

    ret = P2DAL_IsEmergencyBrakeImminent();

    return ret;
}

/******************************************************************************/
/**
 * Common ESP outputs sets in all APC modes
 *
 *
 * \return bool_T
 */
/******************************************************************************/

static void BRKH_CommonApcEspOutputs(void)
{

    park_brk_stat = BRAKING_STATE_APC_MODE;

    park_parktype = BRKH_GetParktype();

    parkguid_stat = PARK_GUIDANCE_STATE_ACTIVE;

    vehicle_speed_rq = BRKH_EvaluateVehicleSpeedRq();

    gear_position_rq = a_apc_gear_pos_rq;
}

/******************************************************************************/
/**
 * Check of P4U take over abort
 *
 *
 * \return bool_T
 */
/******************************************************************************/

static bool_T BRKH_P4U_AbortTakeOver(void)
{
    bool_T ret = FALSE;
    esp_brake_state_E tmp_esp_brake_state;

    tmp_esp_brake_state = COMH_GetEspBrakeState();

    if((ESP_INTERVENTION_ACTIVE == COMH_GetESPIntervention()) || ((tmp_esp_brake_state != BRAKING_STATE_APC_MODE) && (tmp_esp_brake_state != BRAKING_STATE_PARKMAN_INACTIV)) )
    {
        ret = TRUE;
    }

    return ret;
}

/******************************************************************************/
/**
 * Check of P4U aborts
 *
 *
 * \return bool_T
 */
/******************************************************************************/

static bool_T BRKH_P4U_Abort(void)
{
    bool_T ret = FALSE;

    if( ((FALSE == P2DAL_IsCtrlSystemActive()) || (DAPM_OS_ABORTING == P2DAL_GetOperationState()))
            && (DAPM_OAR_NONE != P2DAL_GetSpecificCtrlAbortReason()) )
    {
        ret = TRUE;
    }
    else if (TRUE == BRKH_EspAbort())
    {
        ret = TRUE;
    }


    return ret;
}

/******************************************************************************/
/**
 * Set Park-In or Park-Out
 *
 *
 * \return none
 */
/******************************************************************************/

static void BRKH_SetManeuverType(void)
{
    u8 tmp_id;
    enum CAPP_maneuver_strategy_E tmp_ms;
    switch(brkh_active_sub_state)
    {
    case BRKH_ACTIVE_P4U_ONLY:
    case BRKH_ACTIVE_P4U_EMERGENCY_PREFILL:
    case BRKH_ACTIVE_P4U_EMERGENCY_BRAKE:
        P2DAL_GetFreezeInfo(&tmp_id, &tmp_ms);

        switch (tmp_ms)
        {
        case CAPP_MS_OAF:
        case CAPP_MS_OEF:
        case CAPP_MS_OEB:
            brkh_maneuver_type = PARK_OUT;
            break;

        case CAPP_MS_PAF:
        case CAPP_MS_PAB:
        case CAPP_MS_PEF:
        case CAPP_MS_PEB:
        case CAPP_MS_GPF:
        case CAPP_MS_GPB:
        case CAPP_MS_EPA:
        case CAPP_MS_EWA:
        case CAPP_MS_EXP:
        case CAPP_MS_UNKNOWN:
        default:
            brkh_maneuver_type = PARK_IN;
            break;
        }
        break;
    case BRKH_ACTIVE_P4U_ABORT:
    case BRKH_ACTIVE_P4U_FINISH:
        /* leave last value */
        break;
    default:
        brkh_maneuver_type = PARK_IN;
        break;
    }


}

/******************************************************************************/
/**
 * Check for Park-In or Park-Out
 *
 *
 * \return none
 */
/******************************************************************************/
static brkh_maneuver_type_E BRKH_GetManeuverType(void)
{
    return brkh_maneuver_type;
}

/******************************************************************************/
/**
 * Check maneuver finish
 *
 *
 * \return bool_T
 */
/******************************************************************************/

static bool_T BRKH_P4U_Finish(void)
{
    bool_T ret = FALSE;

    if( ((FALSE == P2DAL_IsCtrlSystemActive())  ||  (DAPM_OS_FINISHING == P2DAL_GetOperationState()))
         && (DAPM_OAR_NONE == P2DAL_GetSpecificCtrlAbortReason()) )
    {
        ret = TRUE;
    }

    return ret;
}


/******************************************************************************/
/**
 * Entering reactions for handshake Statemachine with brake ECU
 *
 *
 * \return none
 */
/******************************************************************************/

static void ExecuteBRKHMainEnteringDuringReaction (brkh_app_states_E new_state)
{

#if (USE_DIFFERENT_TRANSMISSIONS == STD_ON)
    transmission_type_E transmission_type;
#endif

    switch(new_state)
    {
    case BRKH_APP_NA:
        park_brk_stat = BRAKING_STATE_PARKMAN_NOT_AVAILABLE;
        park_parktype = PARK_PARKTYPE_IDLE;
        park_dangermd_rq = DANGER_MODE_INACTIVE;
        remaining_distance = BRKH_MAX_REMAINING_DISTANCE;
        remaining_distance_stat = REM_DIST_STAT_NO_STEP;
        park_brkmd_rq = PARK_BRK_MODE_DISABLED;
        vehicle_speed_rq = BRKH_IDLE_VEH_SPD_RQ;
        gear_position_rq = GEAR_POSITION_RQ_IDLE;
        parkguid_stat = PARK_GUIDANCE_STATE_INACTIVE;
        park_ctrlmd_rq = PARK_CTRL_MODE_RQ_IDLE;
        park_park_stat = PARK_PARK_STAT_IDLE;
        break;

    case BRKH_APP_INIT:
#if (USE_DIFFERENT_TRANSMISSIONS == STD_ON)
        transmission_type = GetTransmissionStyle();

        if(transmission_type == AUTOMATIC_TRANSMISSION)
        {
            park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
        }
        else
        {
            park_brkmd_rq = PARK_BRK_MODE_EMERGENCY_STOP;
        }
#else
        park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
#endif

        park_brk_stat = BRAKING_STATE_PARKMAN_INIT;

        park_parktype = PARK_PARKTYPE_IDLE;
        park_dangermd_rq = DANGER_MODE_INACTIVE;
        remaining_distance = BRKH_MAX_REMAINING_DISTANCE;
        remaining_distance_stat = REM_DIST_STAT_NO_STEP;
        vehicle_speed_rq = BRKH_IDLE_VEH_SPD_RQ;
        gear_position_rq = GEAR_POSITION_RQ_IDLE;
        parkguid_stat = PARK_GUIDANCE_STATE_INACTIVE;
        park_ctrlmd_rq = PARK_CTRL_MODE_RQ_IDLE;
        park_park_stat = PARK_PARK_STAT_IDLE;
        break;

    case BRKH_APP_READY:
#if (USE_DIFFERENT_TRANSMISSIONS == STD_ON)
        transmission_type = GetTransmissionStyle();

        if(transmission_type == AUTOMATIC_TRANSMISSION)
        {
            park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
        }
        else
        {
            park_brkmd_rq = PARK_BRK_MODE_EMERGENCY_STOP;
        }
#else
        park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
#endif

        park_brk_stat = BRAKING_STATE_PARKMAN_INACTIV;

        park_parktype = BRKH_GetParktype();

        park_park_stat = BRKH_EvaluateParkParkStat();

        park_dangermd_rq = DANGER_MODE_INACTIVE;
        remaining_distance = BRKH_MAX_REMAINING_DISTANCE;
        remaining_distance_stat = REM_DIST_STAT_NO_STEP;
        vehicle_speed_rq = BRKH_IDLE_VEH_SPD_RQ;
        gear_position_rq = GEAR_POSITION_RQ_IDLE;

        park_ctrlmd_rq = PARK_CTRL_MODE_RQ_IDLE;
        parkguid_stat = PARK_GUIDANCE_STATE_INACTIVE;

        break;

    case BRKH_APP_FOLLOW_UP:
#if (USE_DIFFERENT_TRANSMISSIONS == STD_ON)
        transmission_type = GetTransmissionStyle();

        if(transmission_type == AUTOMATIC_TRANSMISSION)
        {
            park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
        }
        else
        {
            park_brkmd_rq = PARK_BRK_MODE_EMERGENCY_STOP;
        }
#else
        park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
#endif

        park_brk_stat = BRAKING_STATE_PARKMAN_INACTIV;
        park_dangermd_rq = DANGER_MODE_INACTIVE;

        switch(brkh_active_sub_state)
        {
        case BRKH_ACTIVE_P4U_ONLY:
        case BRKH_ACTIVE_P4U_EMERGENCY_PREFILL:
        case BRKH_ACTIVE_P4U_EMERGENCY_BRAKE:
        case BRKH_ACTIVE_P4U_ABORT:
        case BRKH_ACTIVE_P4U_FINISH:
            park_parktype = BRKH_GetParktype();
            /* park_park_stat = BRKH_EvaluateParkParkStat(); keep old value */
            remaining_distance = a_apc_remaining_distance;
            vehicle_speed_rq = BRKH_EvaluateVehicleSpeedRq();
            break;

        case BRKH_ACTIVE_EMERGENCY_PREFILL:
        case BRKH_ACTIVE_EMERGENCY_BRAKE:
        case BRKH_ACTIVE_EMERGENCY_ABORT:
        case BRKH_ACTIVE_EMERGENCY_FINISH:
            park_parktype = PARK_PARKTYPE_IDLE;
            vehicle_speed_rq = BRKH_IDLE_VEH_SPD_RQ;
            break;
        default:
            break;
        }


        remaining_distance_stat = REM_DIST_STAT_NO_STEP;

        parkguid_stat = PARK_GUIDANCE_STATE_INACTIVE;


        park_ctrlmd_rq = PARK_CTRL_MODE_RQ_IDLE;

        break;
    case BRKH_APP_BRAKE_ACTIVE:
        ExecuteBRKHActiveEnteringDuringReaction(brkh_active_sub_state);
        break;
    }
}

/******************************************************************************/
/**
 * Entering reaction for Active Braking sub states
 *
 *
 * \return none
 */
/******************************************************************************/

static void ExecuteBRKHActiveEnteringDuringReaction(brkh_active_sub_states_E new_state)
{
    esp_brake_state_E tmp_esp_brake_state;

    switch(new_state)
    {
    case BRKH_ACTIVE_EMERGENCY_PREFILL:

        park_dangermd_rq = DANGER_MODE_ACTIVE;

        break;

    case BRKH_ACTIVE_EMERGENCY_BRAKE:

#if (USE_DIFFERENT_TRANSMISSIONS == STD_ON)
        transmission_type = GetTransmissionStyle();

        if(transmission_type == AUTOMATIC_TRANSMISSION)
        {
            park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
        }
        else
        {
            park_brkmd_rq = PARK_BRK_MODE_EMERGENCY_STOP;
        }
#else
        park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
#endif

        parkguid_stat = PARK_GUIDANCE_STATE_INACTIVE;

        park_brk_stat = BRAKING_STATE_PPB_MODE;

        park_ctrlmd_rq = PARK_CTRL_MODE_RQ_IDLE;

        remaining_distance_stat = REM_DIST_STAT_PPB;

        vehicle_speed_rq = BRKH_GetSpeedKph();

        remaining_distance = P2DAL_GetDistToCollision();
        if(remaining_distance > BRKH_MAX_REMAINING_DISTANCE)
        {
            remaining_distance = BRKH_MAX_REMAINING_DISTANCE;
        }

        if(current_gear_lever_position == GEAR_POSITION_D)
        {
            park_park_stat = PARK_PARK_STAT_FWD_STROKE;
        }
        else if (current_gear_lever_position == GEAR_POSITION_R)
        {
            park_park_stat = PARK_PARK_STAT_BACK_STROKE;
        }
        else
        {

        }
        gear_position_rq = GEAR_POSITION_RQ_IDLE;
        park_parktype = PARK_PARKTYPE_IDLE;
        break;

    case BRKH_ACTIVE_P4U_ONLY:

        park_dangermd_rq = DANGER_MODE_INACTIVE;
        if(current_gear_lever_position == GEAR_POSITION_D)
        {
            park_park_stat = PARK_PARK_STAT_FWD_STROKE;
        }
        else if (current_gear_lever_position == GEAR_POSITION_R)
        {
            park_park_stat = PARK_PARK_STAT_BACK_STROKE;
        }
        else
        {

        }
        park_dangermd_rq = DANGER_MODE_INACTIVE;

        remaining_distance = a_apc_remaining_distance;
        if((remaining_distance > BRKH_MAX_PARK_OUT_REMAINING_DISTANCE) && (PARK_OUT == BRKH_GetManeuverType()))
        {
            remaining_distance = BRKH_MAX_PARK_OUT_REMAINING_DISTANCE;
        }

        remaining_distance_stat = a_apc_remaining_distance_state;

        BRKH_CommonApcEspOutputs();

        break;
    case BRKH_ACTIVE_P4U_EMERGENCY_PREFILL:
        park_dangermd_rq = DANGER_MODE_ACTIVE;

        remaining_distance = a_apc_remaining_distance;
        if((remaining_distance > BRKH_MAX_PARK_OUT_REMAINING_DISTANCE) && (PARK_OUT == BRKH_GetManeuverType()))
        {
            remaining_distance = BRKH_MAX_PARK_OUT_REMAINING_DISTANCE;
        }

        remaining_distance_stat = a_apc_remaining_distance_state;

        BRKH_CommonApcEspOutputs();

        break;

    case BRKH_ACTIVE_P4U_EMERGENCY_BRAKE:
        park_dangermd_rq = DANGER_MODE_INACTIVE;
        if(current_gear_lever_position == GEAR_POSITION_D)
        {
            park_park_stat = PARK_PARK_STAT_FWD_STROKE;
        }
        else if (current_gear_lever_position == GEAR_POSITION_R)
        {
            park_park_stat = PARK_PARK_STAT_BACK_STROKE;
        }
        else
        {

        }

        park_dangermd_rq = DANGER_MODE_INACTIVE;

        remaining_distance = a_apc_remaining_distance;
        if((remaining_distance > BRKH_MAX_PARK_OUT_REMAINING_DISTANCE) && (PARK_OUT == BRKH_GetManeuverType()))
        {
            remaining_distance = BRKH_MAX_PARK_OUT_REMAINING_DISTANCE;
        }

        remaining_distance_stat = REM_DIST_STAT_PPB;

        gear_position_rq = a_apc_gear_pos_rq;

        BRKH_CommonApcEspOutputs();

        break;
    case BRKH_ACTIVE_P4U_ABORT:

        park_park_stat = PARK_PARK_STAT_FLT;

        remaining_distance = 0;

        parkguid_stat = PARK_GUIDANCE_STATE_ACTIVE;

        P2DAL_AbortManeuver();

        tmp_esp_brake_state = COMH_GetEspBrakeState();

        if(tmp_esp_brake_state != BRAKING_STATE_APC_MODE)
        {
            brkh_exit_control = TRUE;
        }
        break;
    case BRKH_ACTIVE_P4U_FINISH:

        tmp_esp_brake_state = COMH_GetEspBrakeState();
        park_park_stat = PARK_PARK_STAT_END;

        if(tmp_esp_brake_state != BRAKING_STATE_APC_MODE)
        {
            brkh_exit_control = TRUE;
        }
        if((tmp_esp_brake_state == BRAKING_STATE_APC_MODE) && (PARK_IN == BRKH_GetManeuverType()))
        {
            gear_position_rq = GEAR_POSITION_RQ_P;
        }
        if(PARK_IN == BRKH_GetManeuverType())
        {
            remaining_distance = 0;
        }
        else
        {
            remaining_distance = BRKH_MAX_PARK_OUT_REMAINING_DISTANCE;
        }
        break;
    case BRKH_ACTIVE_EMERGENCY_ABORT:
        park_park_stat = PARK_PARK_STAT_FLT;

        remaining_distance = BRKH_MAX_REMAINING_DISTANCE;

        brkh_exit_control = TRUE;
        break;
    case BRKH_ACTIVE_EMERGENCY_FINISH:
        park_brk_stat = BRAKING_STATE_PARKMAN_INACTIV;

        remaining_distance_stat = REM_DIST_STAT_NO_STEP;

        park_parktype = PARK_PARKTYPE_IDLE;

        gear_position_rq = GEAR_POSITION_RQ_IDLE;

        vehicle_speed_rq = BRKH_IDLE_VEH_SPD_RQ;

        remaining_distance = BRKH_MAX_REMAINING_DISTANCE;

        brkh_exit_control = TRUE;
        break;

    default:
        break;
    }
}

/******************************************************************************/
/**
 * Transition check for Braking active sub states
 *
 *
 * \return none
 */
/******************************************************************************/

u32 BRKH_GetPosition(void)
{
    u32 ret;
    struct CAPP_position32_S temp_position;
    P2DAL_GetGlobalVehiclePosition(&temp_position);
    ret = temp_position.s_mm;
    return ret;
}
static bool_T CheckBRKHActiveTransition(brkh_active_sub_states_E current_state, brkh_active_sub_states_E* next_state)
{
    bool_T ret = FALSE;
    struct CAPP_position32_S temp_position;
    switch(current_state)
    {
    case BRKH_ACTIVE_EMERGENCY_PREFILL:
        if((TRUE == IsCarReadyForP4U()) && (TRUE == BRKH_DasActivatingRequest()))
        {
            if ((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeRequired()))
            {
                *next_state = BRKH_ACTIVE_P4U_EMERGENCY_BRAKE;
            }
            else if((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeImminent()))
            {
                *next_state = BRKH_ACTIVE_P4U_EMERGENCY_PREFILL;
            }
            else
            {
                *next_state = BRKH_ACTIVE_P4U_ONLY;
            }
            BRKH_SetManeuverType();
            BRKH_EvaluateParktype();
            ret = TRUE;
            a_rd_init = TRUE;
            a_start_of_first_stroke = TRUE;
            brkh_apc_guidance_main_state = EvaluateDefaultAPCGuidanceState();

            P2DAL_GetGlobalVehiclePosition(&temp_position);

            a_rd_car_position_last = temp_position.s_mm;
            a_rd_driven_dist_since_init_dth=0;
            a_rd_driven_dist_since_init_dtc=0;
        }
        else if(FALSE == IsCarReadyForEmergency())
        {
            brkh_exit_control = TRUE;
        }
        else if(TRUE == BRKH_EmergencyBrakeRequired())
        {
            *next_state = BRKH_ACTIVE_EMERGENCY_BRAKE;
            ret = TRUE;
        }
        else
        {

        }
        break;
    case BRKH_ACTIVE_EMERGENCY_BRAKE:
        if((TRUE == IsCarReadyForP4U()) && (TRUE == BRKH_DasActivatingRequest()))
        {
            if ((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeRequired()))
            {
                *next_state = BRKH_ACTIVE_P4U_EMERGENCY_BRAKE;
            }
            else if((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeImminent()))
            {
                *next_state = BRKH_ACTIVE_P4U_EMERGENCY_PREFILL;
            }
            else
            {
                *next_state = BRKH_ACTIVE_P4U_ONLY;
            }
            BRKH_SetManeuverType();
            BRKH_EvaluateParktype();
            ret = TRUE;
            a_rd_init = TRUE;
            a_start_of_first_stroke = TRUE;
            brkh_apc_guidance_main_state = EvaluateDefaultAPCGuidanceState();
            P2DAL_GetGlobalVehiclePosition(&temp_position);

            a_rd_car_position_last = temp_position.s_mm;
            a_rd_driven_dist_since_init_dth=0;
            a_rd_driven_dist_since_init_dtc=0;
        }
        else if (TRUE == BRKH_EmergencyBrakeAbort())
        {
            *next_state = BRKH_ACTIVE_EMERGENCY_ABORT;
            ret = TRUE;
        }
        else if(TRUE == P2DAL_IsVehicleStandstill())
        {
            *next_state = BRKH_ACTIVE_EMERGENCY_FINISH;
            ret = TRUE;
        }
        else
        {

        }
        break;

    case BRKH_ACTIVE_P4U_ONLY:
        if(TRUE == BRKH_P4U_Abort() && (brkh_active_timer >= brkh_active_rdv))
        {
            *next_state = BRKH_ACTIVE_P4U_ABORT;
            ret = TRUE;
        }
        else if(TRUE == BRKH_P4U_Finish() && (brkh_active_timer >= brkh_active_rdv))
        {
            *next_state = BRKH_ACTIVE_P4U_FINISH;
            ret = TRUE;
        }
        else if ((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeRequired()))
        {
            *next_state = BRKH_ACTIVE_P4U_EMERGENCY_BRAKE;
            ret = TRUE;
        }
        else if((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeImminent()))
        {
            *next_state = BRKH_ACTIVE_P4U_EMERGENCY_PREFILL;
            ret = TRUE;
        }
        else
        {

        }
        break;

    case BRKH_ACTIVE_P4U_EMERGENCY_PREFILL:
        if(TRUE == BRKH_P4U_Abort() && (brkh_active_timer >= brkh_active_rdv))
        {
            *next_state = BRKH_ACTIVE_P4U_ABORT;
            ret = TRUE;
        }
        else if(TRUE == BRKH_P4U_Finish() && (brkh_active_timer >= brkh_active_rdv))
        {
            *next_state = BRKH_ACTIVE_P4U_FINISH;
            ret = TRUE;
        }
        else if ((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeRequired()))
        {
            *next_state = BRKH_ACTIVE_P4U_EMERGENCY_BRAKE;
            ret = TRUE;
        }
        else if((FALSE == IsCarReadyForEmergency()) || (FALSE == BRKH_EmergencyBrakeImminent()))
        {
            *next_state = BRKH_ACTIVE_P4U_ONLY;
            ret = TRUE;
        }
        else
        {

        }
        break;

    case BRKH_ACTIVE_P4U_EMERGENCY_BRAKE:
        if(TRUE == BRKH_P4U_Abort() && (brkh_active_timer >= brkh_active_rdv))
        {
            *next_state = BRKH_ACTIVE_P4U_ABORT;
            ret = TRUE;
        }
        else if(TRUE == BRKH_P4U_Finish() && (brkh_active_timer >= brkh_active_rdv))
        {
            *next_state = BRKH_ACTIVE_P4U_FINISH;
            ret = TRUE;
        }
        else if ((TRUE == BRKH_EmergencyBrakeAbort()) || (TRUE == P2DAL_IsVehicleStandstill()))
        {
            *next_state = BRKH_ACTIVE_P4U_ONLY;
            ret = TRUE;
        }
        else
        {

        }
        break;
    case BRKH_ACTIVE_P4U_FINISH:
        if(TRUE == BRKH_P4U_AbortTakeOver())
        {
            *next_state = BRKH_ACTIVE_P4U_ABORT;
            ret = TRUE;
        }
        break;
    case BRKH_ACTIVE_EMERGENCY_ABORT:
    case BRKH_ACTIVE_EMERGENCY_FINISH:
    case BRKH_ACTIVE_P4U_ABORT:
    default:
        break;
    }
    if(brkh_active_timer != BRKH_UINT8_MAX)
    {
        brkh_active_timer++;
    }
    else
    {
        brkh_active_timer=0;
        if(brkh_active_rdv > BRKH_UINT8_MAX)
        {
            brkh_active_rdv -= (uint16) ((uint16) 1 + BRKH_UINT8_MAX);
        }
    }
    if(brkh_active_timer >= brkh_active_rdv)
    {
        brkh_active_rdv = 0;
    }
    return ret;
}

/******************************************************************************/
/**
 * Transition check for handshake Statemachine with brake ECU
 *
 *
 * \return TRUE if we take a transition, FALSE otherwise
 */
/******************************************************************************/

static bool_T CheckBRKHMainTransition(brkh_app_states_E current_state, brkh_app_states_E* next_state)
{
    bool_T ret=FALSE;
    KL15_state_E kl15_status;
    engine_running_state_E engine_state;
    esp_brake_state_E tmp_esp_brake_state;
    ldc_afterrun_ctrl_E tmp_esp_ldc_afterrun_ctrl;
    struct CAPP_position32_S temp_position;

    switch (current_state)
    {
    case  BRKH_APP_NA:
        kl15_status = COMH_GetKl15();
        if(kl15_status == KL15_ON)
        {
            *next_state = BRKH_APP_INIT;
            ret = TRUE;
        }
        break;
    case  BRKH_APP_INIT:
        kl15_status = COMH_GetKl15();
        if(kl15_status == KL15_OFF)
        {
            *next_state = BRKH_APP_NA;
            ret = TRUE;
        }
        else
        {
            engine_state = COMH_GetEngineState();
            if(engine_state == ENGINE_RUNNING)
            {
                *next_state = BRKH_APP_READY;
                ret = TRUE;
            }
        }
        break;
    case  BRKH_APP_READY:
        engine_state = COMH_GetEngineState();
        if(engine_state == ENGINE_NOT_RUNNING)
        {
            *next_state = BRKH_APP_INIT;
            ret = TRUE;
        }
        else
        {
            if((TRUE == IsCarReadyForP4U()) && (TRUE == BRKH_DasActivatingRequest()))
            {
                if ((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeRequired()))
                {
                    *next_state = BRKH_APP_BRAKE_ACTIVE;
                    brkh_active_sub_state = BRKH_ACTIVE_P4U_EMERGENCY_BRAKE;
                }
                else if((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeImminent()))
                {
                    *next_state = BRKH_APP_BRAKE_ACTIVE;
                    brkh_active_sub_state = BRKH_ACTIVE_P4U_EMERGENCY_PREFILL;
                }
                else
                {
                    *next_state = BRKH_APP_BRAKE_ACTIVE;
                    brkh_active_sub_state = BRKH_ACTIVE_P4U_ONLY;
                }
                BRKH_SetManeuverType();
                BRKH_EvaluateParktype();
                ret = TRUE;
                a_rd_init = TRUE;
                a_start_of_first_stroke = TRUE;
                brkh_apc_guidance_main_state = EvaluateDefaultAPCGuidanceState();

                P2DAL_GetGlobalVehiclePosition(&temp_position);

                a_rd_car_position_last = temp_position.s_mm;
                a_rd_driven_dist_since_init_dth=0;
                a_rd_driven_dist_since_init_dtc=0;
            }
            else if((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeRequired()) )
            {
                *next_state = BRKH_APP_BRAKE_ACTIVE;
                brkh_active_sub_state = BRKH_ACTIVE_EMERGENCY_BRAKE;
                ret = TRUE;
            }
            else if((TRUE == IsCarReadyForEmergency()) && (TRUE == BRKH_EmergencyBrakeImminent()))
            {
                *next_state = BRKH_APP_BRAKE_ACTIVE;
                brkh_active_sub_state = BRKH_ACTIVE_EMERGENCY_PREFILL;
                ret = TRUE;
            }
            else
            {

            }
            if(ret == TRUE)
            {
                brkh_active_rdv = brkh_active_timer + BRKH_DLY_ACTIVATION_ECU;
            }
        }
        break;
    case  BRKH_APP_FOLLOW_UP:
        engine_state = COMH_GetEngineState();
        if(engine_state == ENGINE_NOT_RUNNING)
        {
            *next_state = BRKH_APP_INIT;
            ret = TRUE;
        }
        else
        {
            tmp_esp_brake_state = COMH_GetEspBrakeState();
            tmp_esp_ldc_afterrun_ctrl = COMH_GetEspAfterrunControl();
            if((tmp_esp_brake_state == BRAKING_STATE_PARKMAN_INACTIV) &&
                                (tmp_esp_ldc_afterrun_ctrl == ESP_LDC_AFTERRUNCTRL_NO_FOLLOW_UP))
            {
                *next_state = BRKH_APP_READY;
                ret = TRUE;
            }
        }

        break;

    case  BRKH_APP_BRAKE_ACTIVE:
        if(TRUE == brkh_exit_control)
        {
            *next_state = BRKH_APP_FOLLOW_UP;
            ret = TRUE;
            brkh_exit_control = FALSE;
            BRKH_EvaluateParktype();
        }
        break;
    default:
        break;
    }

    return ret;
}

/******************************************************************************/
/*                  Definition of exported module functions                   */
/******************************************************************************/

/******************************************************************************/
/**
 * inits the BRKH_CUS module
 * It is called by the BRKH module.
 *
 * \return nothing
 */
/******************************************************************************/
void BRKH_CusInit(void)
{
  brkh_app_main_state = BRKH_APP_NA;
  /* Outputs to the brake */
  vehicle_speed_rq = BRKH_IDLE_VEH_SPD_RQ;
  remaining_distance = BRKH_MAX_REMAINING_DISTANCE;
  park_park_stat = PARK_PARK_STAT_IDLE;
  park_brk_stat = BRAKING_STATE_PARKMAN_INIT;
  park_brkmd_rq = PARK_BRK_MODE_COMFORT_STOP;
  park_ctrlmd_rq = PARK_CTRL_MODE_RQ_IDLE;
  park_dangermd_rq = DANGER_MODE_INACTIVE;
  parkguid_stat = PARK_GUIDANCE_STATE_INACTIVE;
  remaining_distance_stat = REM_DIST_STAT_NO_STEP;
  gear_position_rq = GEAR_POSITION_RQ_IDLE;
  park_parktype = PARK_PARKTYPE_IDLE;

  ExecuteBRKHMainEnteringDuringReaction(BRKH_APP_NA);

  brkh_brake_state = FALSE;
}



static void ExecuteBRKHMainStateDuring(brkh_app_states_E current_state)
{
    switch(current_state)
    {
    case BRKH_APP_BRAKE_ACTIVE:
        if((FALSE == brkh_p2_active) && (TRUE == P2DAL_IsCtrlSystemActive()) &&
            (((P2DAL_GetDistToHint() > 1000) && (PARK_IN == BRKH_GetManeuverType())) ||
            ((PARK_OUT == BRKH_GetManeuverType()) && ((P2DAL_GetDistToHint() > 100))) ) )
        {
            brkh_p2_active = TRUE;
        }
        switch (brkh_active_sub_state)
        {
        case BRKH_ACTIVE_P4U_ONLY:
        case BRKH_ACTIVE_P4U_EMERGENCY_PREFILL:
        case BRKH_ACTIVE_P4U_EMERGENCY_BRAKE:
            if(brkh_p2_active == TRUE)
            {
                BRKH_CalculateRemainingDistance();

#if (BRKH_USE_P2_COMMAND == STD_ON)
            a_apc_gear_pos_rq = BRKH_EvalP2GearChange();
#else

                BRKH_EvaluateGearPositionRq();
            }
#endif
            break;

        default:
            brkh_p2_active = FALSE;
            break;
        }
        break;
    default:
        break;
    }
}

/******************************************************************************/
/**
 * Statemachine for handshake with brake ecu
 * It is called by a cyclic BRKH function
 *
 * \return nothing
 */
/******************************************************************************/

void BRKH_CusBrakeStateMachine(void)
{
#if USE_MANUAL_BRKH_CODE
    brkh_app_states_E next_state;
    brkh_active_sub_states_E next_active_sub_state;

    current_gear_lever_position = BRKH_GearLeverPos();
    BRKH_CalculateParkParkStat();

    switch (brkh_app_main_state)
    {
    case  BRKH_APP_BRAKE_ACTIVE:


        if(TRUE == CheckBRKHMainTransition(brkh_app_main_state, &next_state))
        {
            ExecuteBRKHMainEnteringDuringReaction(next_state);
            brkh_app_main_state = next_state;
        }
        else
        {
            ExecuteBRKHMainStateDuring(brkh_app_main_state);

            if(TRUE == CheckBRKHActiveTransition(brkh_active_sub_state, &next_active_sub_state))
            {
                ExecuteBRKHActiveEnteringDuringReaction(next_active_sub_state);
                brkh_active_sub_state = next_active_sub_state;
            }
            else
            {
                ExecuteBRKHActiveEnteringDuringReaction(brkh_active_sub_state);
            }
        }

        break;
    case  BRKH_APP_NA:
    case  BRKH_APP_INIT:
    case  BRKH_APP_READY:
    case  BRKH_APP_FOLLOW_UP:
        brkh_p2_active = FALSE;
        a_remaining_distance = 0;
        if(TRUE == CheckBRKHMainTransition(brkh_app_main_state, &next_state))
        {
            ExecuteBRKHMainEnteringDuringReaction(next_state);
            brkh_app_main_state = next_state;
        }
        else
        {
            ExecuteBRKHMainEnteringDuringReaction(brkh_app_main_state);
        }

        break;
    }
#else
    BRKH_CUSBRAKESTATEMACHINE();
#endif
}


/******************************************************************************/
/**
 * returns the abort reason of the braking system for debug purpose
 *
 * \return the abort reason of the braking system
 */
/******************************************************************************/
u8 BRKH_CusGetBrakeAbortReason(void)
{
  return (u8)brkh_brake_abort_reason;
}

/******************************************************************************/
/**
 * returns the actual brake state of the statemachine (needed as input
 * for the STMA/STMH-module)
 *
 * \return the actual brake state of the statemachine
 */
/******************************************************************************/
u8 BRKH_CusGetStateBrakeSys(bool_T * p_lot_ext_sys_state)
{
    *p_lot_ext_sys_state = brkh_brake_state;
    return DAPM_SIGNAL_VALID;
}

/******************************************************************************/
/**
 * returns if the braking system is ready for activating the brake access
 * (brake system is initialized)
 *
 * \return if the braking system is initialized (TRUE) or not (FALSE)
 */
/******************************************************************************/
bool_T BRKH_CusIsBrakeEcuReady(void)
{
  return brkh_is_brake_ecu_ready;
}

/******************************************************************************/
/**
 * returns if the braking system is active
 * (brake system is initialized)
 *
 * \return if the braking system is active (TRUE) or not (FALSE)
 */
/******************************************************************************/
bool_T BRKH_CusIsBrakeEcuActive(void)
{
  return (brkh_brake_state);
}

/******************************************************************************/
/**
 * resets the BRKH_CUS module without influencing the variables of the
 * BrakeStateMachine
 *
 * \return nothing
 */
/******************************************************************************/
void BRKH_CusReset(void)
{
  brkh_brake_state = FALSE;
  BRKH_CusResetAbortReason();
}

/******************************************************************************/
/**
 * resets the abort reason of the brake system to the init value
 *
 * \return nothing
 */
/******************************************************************************/
void BRKH_CusResetAbortReason(void)
{
  brkh_brake_abort_reason = 0;
}

/******************************************************************************
 * Call this function to request the activation of the brake ECU
 * \return if TRUE if the activation request is accepted else returns FALSE
******************************************************************************/
bool_T BRKH_CusActivateBrake(void)
{
    bool_T ret = FALSE;
    if(brkh_activation_request == FALSE)
    {
        brkh_activation_request = TRUE;
        ret = TRUE;
    }
    return ret;
}

/******************************************************************************
 * Call this function to request the de-activation of the brake ECU
******************************************************************************/
void BRKH_CusDeactivateBrake(void)
{
    brkh_activation_request = FALSE;
}

void BRKH_CusSetBrakeState(bool_T state)
{
    brkh_brake_state = state;
}

void BRKH_SetEmergencyBrakeStatus(bool_T value)
{
    brkh_ppb_ready = value;
}

bool_T BRKH_CusActivateEmergencyBrake(void)
{
    bool_T ret = FALSE;

    if(TRUE == brkh_ppb_ready)
    {
        ret = TRUE;
        brkh_Ebrake_activation_request = TRUE;
    }
    else
    {
        brkh_Ebrake_activation_request = FALSE;
    }

    return ret;
}


uint16 BRKH_GetSpeedKph(void)
{
    uint16 ret;
    (void)COMH_GetSpeed_th100kph(&ret);
    return ret;
}
void BRKH_CusDeActivateEmergencyBrake(void)
{
    brkh_Ebrake_activation_request = FALSE;
}
bool_T BRKH_CusIsEmergencyBrakeActive(void)
{
    return (brkh_Ebrake_activation_request == TRUE);
}

bool_T BRKH_DasActivatingRequest(void)
{
    return (bool_T)P2DAL_IsLotCtrlRequired();
}

brkh_app_states_E BRKH_GetMainState(void)
{
    return brkh_app_main_state;
}

brkh_app_sub_states_E BRKH_GetActiveSubState(void)
{
    return brkh_app_sub_state;
}

gear_position_E BRKH_CarCurrentGear(void)
{
    gear_position_E ret;

    ret = COMH_GetCarCurrentGear();

    return ret;
}

gear_position_E BRKH_GearLeverPos(void)
{
    gear_position_E ret=GEAR_POSITION_N;
    enum CAPP_gear_lever_position_E tmp_gear_lever;

    COMH_GetGearLeverPosition(&tmp_gear_lever);

    switch(tmp_gear_lever)
    {
    case CAPP_GLP_POSITION_D:
        ret = GEAR_POSITION_D;
        break;
    case CAPP_GLP_POSITION_N:
        ret = GEAR_POSITION_N;
        break;
    case CAPP_GLP_POSITION_R:
        ret = GEAR_POSITION_R;
        break;
    case CAPP_GLP_POSITION_P:
        ret = GEAR_POSITION_P;
        break;
    default:
        ret = last_gear_lever_position;
        break;
    }

    if(ret != last_gear_lever_position)
    {
        changed_gear_lever_position = TRUE;
    }
    else
    {
        changed_gear_lever_position = FALSE;
    }

    last_gear_lever_position = ret;

    return ret;
}

bool_T BRKH_ParkEnblStatEspAPC(void)
{
    bool_T ret = FALSE;

    if (  (COMH_GetParkEnblStatEsp() & BRKH_BIT_PARK_ENBL_STAT_ESP_APC)
       == BRKH_BIT_PARK_ENBL_STAT_ESP_APC)
    {
      ret = TRUE;
    }
    else
    {
      ret = FALSE;
    }

    return ret;

}

bool_T BRKH_LimitMaxSpeed(void)
{
    si16 car_to_slot_y;
    enum CAPP_spatial_alloc_E allocation;
    bool_T ret=FALSE;
    u8 tmp_id;
    enum CAPP_maneuver_strategy_E tmp_ms;

    P2DAL_GetFreezeInfo(&tmp_id, &tmp_ms);

    if(TRUE == BRKH_GetFrozenSlotInfo(&tmp_id, &allocation, &car_to_slot_y))
    {

        current_gear_lever_position = BRKH_GearLeverPos();
        switch (tmp_ms)
        {
        case CAPP_MS_PEF:
            if(current_gear_lever_position == GEAR_POSITION_D)
            {
            switch(allocation)
            {

            case CAPP_SPA_LEFT        :
            case CAPP_SPA_REAR_LEFT   :
            case CAPP_SPA_FRONT_LEFT  :
                if(car_to_slot_y > BRKH_PEF_CAR_TO_SLOT_Y)
                {
                    ret = TRUE;
                }
                else
                {
                    ret = FALSE;
                }
                break;
            case CAPP_SPA_RIGHT       :
            case CAPP_SPA_FRONT_RIGHT :
            case CAPP_SPA_REAR_RIGHT  :
                if(car_to_slot_y < (-BRKH_PEF_CAR_TO_SLOT_Y))
                {
                    ret = TRUE;
                }
                else
                {
                    ret = FALSE;
                }
                break;
            case CAPP_SPA_UNKNOWN     :
            case CAPP_SPA_FRONT       :
            case CAPP_SPA_REAR        :
            default:
                ret = FALSE;
                break;
            }
            }
            break;
        case CAPP_MS_PEB:
            if(current_gear_lever_position == GEAR_POSITION_R)
            {
            switch(allocation)
            {

            case CAPP_SPA_LEFT        :
            case CAPP_SPA_REAR_LEFT   :
            case CAPP_SPA_FRONT_LEFT  :
                if(car_to_slot_y > BRKH_PEB_CAR_TO_SLOT_Y)
                {
                    ret = TRUE;
                }
                else
                {
                    ret = FALSE;
                }
                break;
            case CAPP_SPA_RIGHT       :
            case CAPP_SPA_FRONT_RIGHT :
            case CAPP_SPA_REAR_RIGHT  :
                if(car_to_slot_y < (-BRKH_PEB_CAR_TO_SLOT_Y))
                {
                    ret = TRUE;
                }
                else
                {
                    ret = FALSE;
                }
                break;
            case CAPP_SPA_UNKNOWN     :
            case CAPP_SPA_FRONT       :
            case CAPP_SPA_REAR        :
            default:
                ret = FALSE;
                break;
            }
            }
            break;

        case CAPP_MS_PAF:
        case CAPP_MS_PAB:
        case CAPP_MS_OAF:
        case CAPP_MS_OEF:
        case CAPP_MS_OEB:
        case CAPP_MS_GPF:
        case CAPP_MS_GPB:
        case CAPP_MS_EPA:
        case CAPP_MS_EWA:
        case CAPP_MS_EXP:
        case CAPP_MS_UNKNOWN:
        default:
            ret = FALSE;
            break;

        }
    }
    return ret;
}

static bool_T BRKH_GetFrozenSlotInfo(u8 *tmp_id, enum CAPP_spatial_alloc_E *allocation, si16 *car_to_slot_y)
{
    RTE_das_selectable_slots tmp_slot_buffer = {0};

    u8 i=0;
    bool_T ret=FALSE;

    BRKH_GetParkSlotInfo(&tmp_slot_buffer);

    while((ret == FALSE) && (i < tmp_slot_buffer.number_selectable_slots))
    {
        if(*tmp_id == tmp_slot_buffer.selectable_slots_info[i].available_slots.slot_idx)
        {
            *allocation = tmp_slot_buffer.selectable_slots_info[i].spatial_alloc;
            *car_to_slot_y = tmp_slot_buffer.selectable_slots_info[i].veh_y_mm;
            ret = TRUE;
        }
        else
        {
            i++;
        }
    }

    return ret;
}


static void BRKH_GetParkSlotInfo(RTE_das_selectable_slots *tmp_slot_buffer)
{
    const struct DAPM_park_slot_info_S *tmp_results;

    u8 num_results;
    u8 temp_side_index;
    u8 temp_index;
    u8 number_of_sides;
    u8 u8_index_slot = 0;
    u8 tmp_max_no_of_slots = ((2 * XDAPM_NUM_SLOTS_SIDE) + (XDAPM_NUM_SLOTS_FRONT));
    u8 tmp_max_no_of_slots_rte = sizeof(tmp_slot_buffer->selectable_slots_info)
            / sizeof(RTE_das_all_park_slot_info_S);

    enum CAPP_spatial_alloc_E a_park_side_e[3] = {CAPP_SPA_LEFT, CAPP_SPA_RIGHT, CAPP_SPA_FRONT};

    /* Get the number of sides as a number of elements in array a_park_side_e */
    number_of_sides = (sizeof(a_park_side_e) / sizeof(enum CAPP_spatial_alloc_E));
    tmp_slot_buffer->number_selectable_slots = 0;

    for (temp_side_index = 0;temp_side_index < number_of_sides;temp_side_index++)
    {
        /* Get the slot information related to the selected side  */
        tmp_results = P2DAL_GetParkSlotInfo(a_park_side_e[temp_side_index], &num_results);

        if (tmp_results != NULL)
        {
            /* Update the number of current found slots */
            tmp_slot_buffer->number_selectable_slots += num_results;

            for (temp_index = 0;temp_index < num_results;temp_index++)
            {
                /* Accept the slots only if the number of slots are equal to or less than requested by dapm_pdf.h */
                /* And index_slot is checked for maximum slots accepted by RTE to prevent stack overflow */

                if ((u8_index_slot < tmp_max_no_of_slots) && (u8_index_slot < tmp_max_no_of_slots_rte))
                {

                    /* Check if if a perpendicular backward  position ok is available */
                    if(tmp_results[temp_index].pos_ok_info[DAPM_MS_PEB_OEB].pos_ok_typ == MOPC_POS_OK_STANDARD)
                    {
                    	tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.is_pos_ok_bwd = TRUE;
                    }
                    else
                    {
                    	tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.is_pos_ok_bwd = FALSE;
                    }

                    /* Check if if a perpendicular forward  position ok is available */
                    if(tmp_results[temp_index].pos_ok_info[DAPM_MS_PEF_OEF].pos_ok_typ == MOPC_POS_OK_STANDARD)
                    {
                    	tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.is_pos_ok_fwd = TRUE;
                    }
                    else
                    {
                    	tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.is_pos_ok_fwd = FALSE;
                    }

                    /* Check if if a parallel backward  position ok is available */
                    if(tmp_results[temp_index].pos_ok_info[DAPM_MS_PAB_OAF].pos_ok_typ == MOPC_POS_OK_STANDARD)
                    {
                    	tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.is_pos_ok_para = TRUE;
                    }
                    else
                    {
                    	tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.is_pos_ok_para = FALSE;
                    }

                    /* Set this flag to zero as it is not more needed */
                    tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.para_perp_slot =
                        0;

                    /* Get the slot alignment */
                    tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.slot_align =
                        tmp_results[temp_index].slot_align;

                    /* Get the slot ID */
                    tmp_slot_buffer->selectable_slots_info[u8_index_slot].available_slots.slot_idx =
                        tmp_results[temp_index].slot_id;

                    /* Get the spatial allocation of the related slot */
                    tmp_slot_buffer->selectable_slots_info[u8_index_slot].spatial_alloc =
                        tmp_results[temp_index].allocation;

                    /* Get the car position (X) relative to slot */
                    tmp_slot_buffer->selectable_slots_info[u8_index_slot].veh_x_mm =
                        tmp_results[temp_index].veh_x_mm;

                    /* Get the car position (Y) relative to slot */
                    tmp_slot_buffer->selectable_slots_info[u8_index_slot].veh_y_mm =
                        tmp_results[temp_index].veh_y_mm;

                    /* Get the car position (theta angle) relative to slot */
                    tmp_slot_buffer->selectable_slots_info[u8_index_slot].veh_theta_u16f =
                        tmp_results[temp_index].veh_theta_u16f;

                    /* Get the length of the parking slot */
                    tmp_slot_buffer->selectable_slots_info[u8_index_slot].length_mm =
                        tmp_results[temp_index].length_mm;

                    u8_index_slot++;
                }
                else
                {
                    /* The number of slots is more than expected from P2 or more than RTE buffer size */
                }
            }
        }
        else
        {
            /* Do Nothing. The variable is already initialized at the beginning */
        }
    }
}

bool_T BRKH_ParkEnblStatEspPPB(void)
{
    bool_T ret = FALSE;

    if (  (COMH_GetParkEnblStatEsp() & BRKH_BIT_PARK_ENBL_STAT_ESP_PPB)
       == BRKH_BIT_PARK_ENBL_STAT_ESP_PPB)
    {
      ret = TRUE;
    }
    else
    {
      ret = FALSE;
    }
    return ret;
}

/******************************************************************************/
/**
 * Functions called by COMH to read output to the ESP
 *
 */
/******************************************************************************/

void BRKH_SetVehSpdRq(uint16 value)
{
    vehicle_speed_rq = value;
}
void BRKH_SetRemainDist(sint16 value)
{
    remaining_distance = value;
}
void BRKH_SetParkParkStat(park_park_stat_E value)
{
    park_park_stat = value;
}
void BRKH_SetParkBrkStat(esp_brake_state_E value)
{
    park_brk_stat = value;
}
void BRKH_SetParkBrkMdRq(park_brake_mode_E value)
{
    park_brkmd_rq = value;
}
void BRKH_SetParkCtrlMdRq(park_control_mode_rq_E value)
{
    park_ctrlmd_rq = value;
}
void BRKH_SetParkDangerMdRq(danger_mode_rq_E value)
{
    park_dangermd_rq = value;
}
void  BRKH_SetParkGuidStat(park_guidance_state_E value)
{
    parkguid_stat = value;
}
void BRKH_SetRemainDistStat(remaining_distance_stat_E value)
{
    remaining_distance_stat = value;
}
void BRKH_SetGearPosRq(gear_request_E value)
{
    gear_position_rq = value;
}
void BRKH_SetParkParkType(park_parktype_E value)
{
    park_parktype = value;
}

/* Getter functions */


uint16 BRKH_GetVehSpdRq(void)
{
    return vehicle_speed_rq;
}
sint16 BRKH_GetRemainDist(void)
{
    return remaining_distance;
}
park_park_stat_E BRKH_GetParkParkStat(void)
{
    return park_park_stat;
}
esp_brake_state_E BRKH_GetParkBrkStat(void)
{
    return park_brk_stat;
}
park_brake_mode_E BRKH_GetParkBrkMdRq(void)
{
    return park_brkmd_rq;
}
park_control_mode_rq_E BRKH_GetParkCtrlMdRq(void)
{
    return park_ctrlmd_rq;
}
danger_mode_rq_E BRKH_GetParkDangerMdRq(void)
{
    return park_dangermd_rq;
}
park_guidance_state_E  BRKH_GetParkGuidStat(void)
{
    return parkguid_stat;
}
remaining_distance_stat_E BRKH_GetRemainDistStat(void)
{
    return remaining_distance_stat;
}
gear_request_E BRKH_GetGearPosRq(void)
{
    return gear_position_rq;
}
park_parktype_E BRKH_GetParkParkType(void)
{
    return park_parktype;
}

void BRKH_TightenEpb (void)
{
    /*un-Implemented yet */

}

/******************************************************************************/
/*                                            */
/* END  Source:         brkh_cus.c                                                  */
/*                                              */
/******************************************************************************/

