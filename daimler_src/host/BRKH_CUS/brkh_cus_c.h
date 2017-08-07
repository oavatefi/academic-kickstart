/*
 *	 ***************************************************************
 *	  Licensed Materials - Property of IBM
 *	  (C)Copyright IBM Corporation 1989. All Rights Reserved.
 *
 *	  Module Name: brkh_cus_c.h
 *	 ***************************************************************
 */

/* Project: LOCAL_ACTL */
/* Workarea: D:\Daten\jravaux1\stm\data\wa461\wa_platform */
/* Profile Name: brkh_cus, Version: New */
/* File Name: D:\Daten\jravaux1\stm\data\wa461\wa_platform\prt\brkh_cus\brkh_cus_c.h */
/* Date: Monday, May 08, 2017, 14:37:49 */
/* Optimizations Settings: */
/*   (ON)  - Reuse Timeout Variable Where Possible */
/*   (ON)  - Clutch Entrance To State Hierarchy */
/*   (ON)  - Merge State Sequences with no Guard on Transitions */
/*   (ON)  - Inline Default State */
/*   (ON)  - Inline Settings of "Need Another Step" bit: */
/*           - No. of Transitions: 999 */
/*   (OFF) - Inline Entering/Exiting Reactions: */
/*           - No. of Statements: 5 */
/*           - No. of Instances: 999 */
/*   (OFF) - Empty Overlapping Tests for State Hierarchy */
/* TASK/ISR Run Mode: Single Step */


#ifndef BRKH_CUS_C_H_
#define BRKH_CUS_C_H_



/* Code for: cmp_flg.h */

#define LSBYTE_FIRST
#define LSBIT_FIRST
#define GENERATE_INIT_TASK
#define USE_ON_INIT_CODE
#define USE_RESET_DATA
#define USE_TIMEOUT_FLAG


/* End of code for: cmp_flg.h */

#include "p2dal.h"
#include "brkh_cus.h"
#include "cgstuff.h"
#include "userstuf.h"


/* Code for: macro_def.h */

/* General macros used by the Code-Generator */

#ifndef UNSIGNED_MASK_TYPE
#define UNSIGNED_MASK_TYPE unsigned int
#endif

#define ON_INIT_CODE() \
  start_activity_TIMER_INCREMENT_AC;\
  start_activity_BRKH_INPUTS_AC;\
  start_activity_BRKH_CUS_AC;\
\


/* Macros for Ticks per Second/miliSecond for each Counter in   */
/* the System (as taken from the 'Time Expression Scale' Table) */

#ifndef ms_counter_TICKS_PER_SEC
#define ms_counter_TICKS_PER_SEC (1000)
#endif
#ifndef ms_counter_TICKS_PER_MSEC
#define ms_counter_TICKS_PER_MSEC (1)
#endif

#ifndef sec_counter_TICKS_PER_SEC
#define sec_counter_TICKS_PER_SEC (1)
#endif
#ifndef sec_counter_TICKS_PER_MSEC
#define sec_counter_TICKS_PER_MSEC (Error Usage of Counter)
#endif

/* Activity activation control bit(s) and Task's need-another step bit(s) */
#define BITAC_CALCULATE_DRIVEN_DISTANCE (UNSIGNED_MASK_TYPE)(0x04)
#define BITAC_CALCULATE_REM_DIST (UNSIGNED_MASK_TYPE)(0x08)
#define BITAC_APC_GEAR_AC (UNSIGNED_MASK_TYPE)(0x01)
#define BITAC_APC_SPEED_AC (UNSIGNED_MASK_TYPE)(0x02)
#define BITAC_REM_DIST_AC (UNSIGNED_MASK_TYPE)(0x10)
#define BITAC_UZL_CONTROL (UNSIGNED_MASK_TYPE)(0x20)

/* Macros for Activity activation control for Activity: REM_DIST_AC:REMAINING_DISTANCE_FC */
#define start_activity_REMAINING_DISTANCE_FC { }
#define resume_activity_REMAINING_DISTANCE_FC { }
#define stop_activity_REMAINING_DISTANCE_FC { }
#define suspend_activity_REMAINING_DISTANCE_FC { }
#define is_active_REMAINING_DISTANCE_FC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: REM_DIST_AC:CALCULATE_DRIVEN_DISTANCE */
#define start_activity_CALCULATE_DRIVEN_DISTANCE { cgGlobalFlags_brkh_cus |= BITAC_CALCULATE_DRIVEN_DISTANCE; \
    GEN_IN_BUFF(STARTED_CALCULATE_DRIVEN_DISTANCE, &(cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE)); }
#define resume_activity_CALCULATE_DRIVEN_DISTANCE { cgGlobalFlags_brkh_cus |= BITAC_CALCULATE_DRIVEN_DISTANCE; }
#define stop_activity_CALCULATE_DRIVEN_DISTANCE { cgGlobalFlags_brkh_cus &= ~BITAC_CALCULATE_DRIVEN_DISTANCE; }
#define suspend_activity_CALCULATE_DRIVEN_DISTANCE { cgGlobalFlags_brkh_cus &= ~BITAC_CALCULATE_DRIVEN_DISTANCE; }
#define is_active_CALCULATE_DRIVEN_DISTANCE ((cgGlobalFlags_brkh_cus & BITAC_CALCULATE_DRIVEN_DISTANCE) != 0)

/* Macros for Activity activation control for Activity: REM_DIST_AC:CALCULATE_REM_DIST */
#define start_activity_CALCULATE_REM_DIST { cgGlobalFlags_brkh_cus |= BITAC_CALCULATE_REM_DIST; \
    start_activity_REMAINING_DISTANCE_FC; }
#define resume_activity_CALCULATE_REM_DIST { cgGlobalFlags_brkh_cus |= BITAC_CALCULATE_REM_DIST; \
    resume_activity_REMAINING_DISTANCE_FC; }
#define stop_activity_CALCULATE_REM_DIST { cgGlobalFlags_brkh_cus &= ~BITAC_CALCULATE_REM_DIST; stop_activity_REMAINING_DISTANCE_FC; }
#define suspend_activity_CALCULATE_REM_DIST { cgGlobalFlags_brkh_cus &= ~BITAC_CALCULATE_REM_DIST; suspend_activity_REMAINING_DISTANCE_FC; }
#define is_active_CALCULATE_REM_DIST ((cgGlobalFlags_brkh_cus & BITAC_CALCULATE_REM_DIST) != 0)

/* Macros for Activity activation control for Activity: REM_DIST_AC:CTRL_REMDIST */
#define start_activity_CTRL_REMDISTcnt1 { currentState_CTRL_REMDISTcnt1 = 0; }
#define resume_activity_CTRL_REMDISTcnt1 { }
#define stop_activity_CTRL_REMDISTcnt1 { currentState_CTRL_REMDISTcnt1=0; }
#define suspend_activity_CTRL_REMDISTcnt1 { }
#define is_active_CTRL_REMDISTcnt1  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: REM_DIST_AC:REM_DIST */
#define start_activity_REM_DIST { start_activity_CTRL_REMDISTcnt1; }
#define resume_activity_REM_DIST { resume_activity_CTRL_REMDISTcnt1; }
#define stop_activity_REM_DIST { stop_activity_CALCULATE_DRIVEN_DISTANCE; stop_activity_CALCULATE_REM_DIST; \
    stop_activity_CTRL_REMDISTcnt1; }
#define suspend_activity_REM_DIST { suspend_activity_CALCULATE_DRIVEN_DISTANCE; suspend_activity_CALCULATE_REM_DIST; \
    suspend_activity_CTRL_REMDISTcnt1; }
#define is_active_REM_DIST  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:APC_GUIDANCE_SC */
#define start_activity_APC_GUIDANCE_SC { currentState_APC_GUIDANCE_SC = 0; }
#define resume_activity_APC_GUIDANCE_SC { }
#define stop_activity_APC_GUIDANCE_SC { currentState_APC_GUIDANCE_SC=0; }
#define suspend_activity_APC_GUIDANCE_SC { }
#define is_active_APC_GUIDANCE_SC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:APC_VEHSPD_SC */
#define start_activity_APC_VEHSPD_SCcnt1 { currentState_APC_VEHSPD_SCcnt1 = 0; }
#define resume_activity_APC_VEHSPD_SCcnt1 { }
#define stop_activity_APC_VEHSPD_SCcnt1 { currentState_APC_VEHSPD_SCcnt1=0; }
#define suspend_activity_APC_VEHSPD_SCcnt1 { }
#define is_active_APC_VEHSPD_SCcnt1  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:APC_GEAR_AC */
#define start_activity_APC_GEAR_AC { cgGlobalFlags_brkh_cus |= BITAC_APC_GEAR_AC; \
    start_activity_APC_GUIDANCE_SC; }
#define resume_activity_APC_GEAR_AC { cgGlobalFlags_brkh_cus |= BITAC_APC_GEAR_AC; \
    resume_activity_APC_GUIDANCE_SC; }
#define stop_activity_APC_GEAR_AC { cgGlobalFlags_brkh_cus &= ~BITAC_APC_GEAR_AC; stop_activity_APC_GUIDANCE_SC; }
#define suspend_activity_APC_GEAR_AC { cgGlobalFlags_brkh_cus &= ~BITAC_APC_GEAR_AC; suspend_activity_APC_GUIDANCE_SC; }
#define is_active_APC_GEAR_AC ((cgGlobalFlags_brkh_cus & BITAC_APC_GEAR_AC) != 0)

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:APC_SPEED_AC */
#define start_activity_APC_SPEED_AC { cgGlobalFlags_brkh_cus |= BITAC_APC_SPEED_AC; \
    start_activity_APC_VEHSPD_SCcnt1; }
#define resume_activity_APC_SPEED_AC { cgGlobalFlags_brkh_cus |= BITAC_APC_SPEED_AC; \
    resume_activity_APC_VEHSPD_SCcnt1; }
#define stop_activity_APC_SPEED_AC { cgGlobalFlags_brkh_cus &= ~BITAC_APC_SPEED_AC; stop_activity_APC_VEHSPD_SCcnt1; }
#define suspend_activity_APC_SPEED_AC { cgGlobalFlags_brkh_cus &= ~BITAC_APC_SPEED_AC; suspend_activity_APC_VEHSPD_SCcnt1; }
#define is_active_APC_SPEED_AC ((cgGlobalFlags_brkh_cus & BITAC_APC_SPEED_AC) != 0)

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:CTRL_APC_CHECK_SC */
#define start_activity_CTRL_APC_CHECK_SC { currentState_CTRL_APC_CHECK_SC = 0; }
#define resume_activity_CTRL_APC_CHECK_SC { }
#define stop_activity_CTRL_APC_CHECK_SC { cgExitActions_CTRL_APC_CHECK_SC(0); \
    currentState_CTRL_APC_CHECK_SC=0; }
#define suspend_activity_CTRL_APC_CHECK_SC { }
#define is_active_CTRL_APC_CHECK_SC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:CTRL_PPB_CHECK_SC */
#define start_activity_CTRL_PPB_CHECK_SC { currentState_CTRL_PPB_CHECK_SC = 0; }
#define resume_activity_CTRL_PPB_CHECK_SC { }
#define stop_activity_CTRL_PPB_CHECK_SC { currentState_CTRL_PPB_CHECK_SC=0; }
#define suspend_activity_CTRL_PPB_CHECK_SC { }
#define is_active_CTRL_PPB_CHECK_SC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:REM_DIST_AC */
#define start_activity_REM_DIST_AC { cgGlobalFlags_brkh_cus |= BITAC_REM_DIST_AC; \
    start_activity_REM_DIST; }
#define resume_activity_REM_DIST_AC { cgGlobalFlags_brkh_cus |= BITAC_REM_DIST_AC; \
    resume_activity_REM_DIST; }
#define stop_activity_REM_DIST_AC { cgGlobalFlags_brkh_cus &= ~BITAC_REM_DIST_AC; stop_activity_REM_DIST; }
#define suspend_activity_REM_DIST_AC { cgGlobalFlags_brkh_cus &= ~BITAC_REM_DIST_AC; suspend_activity_REM_DIST; }
#define is_active_REM_DIST_AC ((cgGlobalFlags_brkh_cus & BITAC_REM_DIST_AC) != 0)

/* Macros for Activity activation control for Activity: BRKH_CUS_AC:UZL_CONTROL_SC */
#define start_activity_UZL_CONTROL_SC { currentState_UZL_CONTROL_SC = 0; }
#define resume_activity_UZL_CONTROL_SC { }
#define stop_activity_UZL_CONTROL_SC { cgExitActions_UZL_CONTROL_SC(0); \
    currentState_UZL_CONTROL_SC=0; }
#define suspend_activity_UZL_CONTROL_SC { }
#define is_active_UZL_CONTROL_SC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_CUS_AC:BRKH_CUS_SC */
#define start_activity_BRKH_CUS_SCcnt1 { currentState_BRKH_CUS_SCcnt1 = 0; }
#define resume_activity_BRKH_CUS_SCcnt1 { }
#define stop_activity_BRKH_CUS_SCcnt1 { cgExitActions_BRKH_CUS_SCcnt1(0); \
    currentState_BRKH_CUS_SCcnt1=0; }
#define suspend_activity_BRKH_CUS_SCcnt1 { }
#define is_active_BRKH_CUS_SCcnt1  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:CHECK_APC_AC */
#define start_activity_CHECK_APC_AC { start_activity_CTRL_APC_CHECK_SC; }
#define resume_activity_CHECK_APC_AC { resume_activity_CTRL_APC_CHECK_SC; }
#define stop_activity_CHECK_APC_AC { stop_activity_APC_GEAR_AC; stop_activity_APC_SPEED_AC; stop_activity_CTRL_APC_CHECK_SC; \
    stop_activity_REM_DIST_AC; }
#define suspend_activity_CHECK_APC_AC { suspend_activity_APC_GEAR_AC; suspend_activity_APC_SPEED_AC; suspend_activity_CTRL_APC_CHECK_SC; \
    suspend_activity_REM_DIST_AC; }
#define is_active_CHECK_APC_AC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:CHECK_PPB_AC */
#define start_activity_CHECK_PPB_AC { start_activity_CTRL_PPB_CHECK_SC; }
#define resume_activity_CHECK_PPB_AC { resume_activity_CTRL_PPB_CHECK_SC; }
#define stop_activity_CHECK_PPB_AC { stop_activity_CTRL_PPB_CHECK_SC; }
#define suspend_activity_CHECK_PPB_AC { suspend_activity_CTRL_PPB_CHECK_SC; }
#define is_active_CHECK_PPB_AC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_CUS_AC:UZL_CONTROL */
#define start_activity_UZL_CONTROL { cgGlobalFlags_brkh_cus |= BITAC_UZL_CONTROL; \
    start_activity_UZL_CONTROL_SC; }
#define resume_activity_UZL_CONTROL { cgGlobalFlags_brkh_cus |= BITAC_UZL_CONTROL; \
    resume_activity_UZL_CONTROL_SC; }
#define stop_activity_UZL_CONTROL { cgGlobalFlags_brkh_cus &= ~BITAC_UZL_CONTROL; stop_activity_UZL_CONTROL_SC; }
#define suspend_activity_UZL_CONTROL { cgGlobalFlags_brkh_cus &= ~BITAC_UZL_CONTROL; suspend_activity_UZL_CONTROL_SC; }
#define is_active_UZL_CONTROL ((cgGlobalFlags_brkh_cus & BITAC_UZL_CONTROL) != 0)

/* Macros for Activity activation control for Activity: BRKH_CUS_AC:BRKH_CUS */
#define start_activity_BRKH_CUS { start_activity_BRKH_CUS_SCcnt1; }
#define resume_activity_BRKH_CUS { resume_activity_BRKH_CUS_SCcnt1; }
#define stop_activity_BRKH_CUS { stop_activity_BRKH_CUS_SCcnt1; stop_activity_UZL_CONTROL; }
#define suspend_activity_BRKH_CUS { suspend_activity_BRKH_CUS_SCcnt1; suspend_activity_UZL_CONTROL; }
#define is_active_BRKH_CUS  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: BRKH_INPUTS_AC:BRKH_INPUTS */
#define start_activity_BRKH_INPUTS { start_activity_CHECK_APC_AC; start_activity_CHECK_PPB_AC; }
#define resume_activity_BRKH_INPUTS { resume_activity_CHECK_APC_AC; resume_activity_CHECK_PPB_AC; }
#define stop_activity_BRKH_INPUTS { stop_activity_CHECK_APC_AC; stop_activity_CHECK_PPB_AC; }
#define suspend_activity_BRKH_INPUTS { suspend_activity_CHECK_APC_AC; suspend_activity_CHECK_PPB_AC; }
#define is_active_BRKH_INPUTS  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: ACTIVATION_LOGIC_TOP_AC:BRKH_CUS_AC */
#define start_activity_BRKH_CUS_AC { start_activity_BRKH_CUS; }
#define resume_activity_BRKH_CUS_AC { resume_activity_BRKH_CUS; }
#define stop_activity_BRKH_CUS_AC { stop_activity_BRKH_CUS; }
#define suspend_activity_BRKH_CUS_AC { suspend_activity_BRKH_CUS; }
#define is_active_BRKH_CUS_AC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: ACTIVATION_LOGIC_TOP_AC:BRKH_INPUTS_AC */
#define start_activity_BRKH_INPUTS_AC { start_activity_BRKH_INPUTS; }
#define resume_activity_BRKH_INPUTS_AC { resume_activity_BRKH_INPUTS; }
#define stop_activity_BRKH_INPUTS_AC { stop_activity_BRKH_INPUTS; }
#define suspend_activity_BRKH_INPUTS_AC { suspend_activity_BRKH_INPUTS; }
#define is_active_BRKH_INPUTS_AC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Activity: ACTIVATION_LOGIC_TOP_AC:TIMER_INCREMENT_AC */
#define start_activity_TIMER_INCREMENT_AC { }
#define resume_activity_TIMER_INCREMENT_AC { }
#define stop_activity_TIMER_INCREMENT_AC { }
#define suspend_activity_TIMER_INCREMENT_AC { }
#define is_active_TIMER_INCREMENT_AC  (*** Operation Not Supported ***) 

/* Macros for Activity activation control for Task: ACTIVATION_LOGIC_TOP_AC:BRKH_CUSBRAKESTATEMACHINE */
#define start_activity_BRKH_CUSBRAKESTATEMACHINE { start_activity_BRKH_CUS_AC; \
    start_activity_BRKH_INPUTS_AC; start_activity_TIMER_INCREMENT_AC; }
#define resume_activity_BRKH_CUSBRAKESTATEMACHINE { resume_activity_BRKH_CUS_AC; \
    resume_activity_BRKH_INPUTS_AC; resume_activity_TIMER_INCREMENT_AC; }
#define stop_activity_BRKH_CUSBRAKESTATEMACHINE { stop_activity_BRKH_CUS_AC; stop_activity_BRKH_INPUTS_AC; stop_activity_TIMER_INCREMENT_AC; }
#define suspend_activity_BRKH_CUSBRAKESTATEMACHINE { suspend_activity_BRKH_CUS_AC; suspend_activity_BRKH_INPUTS_AC; suspend_activity_TIMER_INCREMENT_AC; }
#define is_active_BRKH_CUSBRAKESTATEMACHINE  (*** Operation Not Supported ***) 

/* Install Alarms related to Timeout(s), to identify counter(s) overflow */
#define SET_ADDITIONAL_OVERFLOW_ALARMS() 


/* Macros defining Counter`s index */
#define MS_COUNTER_INDEX 1

/* Macros for Data-Item: APC_GUIDANCE_SC:A_APC_AUTOGEAR_REQ */
#define read_data_A_APC_AUTOGEAR_REQ 
#define write_data_A_APC_AUTOGEAR_REQ 
#define lval_A_APC_AUTOGEAR_REQ cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_AUTOGEAR_REQ_
#define A_APC_AUTOGEAR_REQ cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_AUTOGEAR_REQ_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_APC_PARKMAN_BRAKE_STATE */
#define read_data_A_APC_PARKMAN_BRAKE_STATE 
#define write_data_A_APC_PARKMAN_BRAKE_STATE 
#define lval_A_APC_PARKMAN_BRAKE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_PARKMAN_BRAKE_STATE_
#define A_APC_PARKMAN_BRAKE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_PARKMAN_BRAKE_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_APC_PARKMAN_PARKTYPE */
#define read_data_A_APC_PARKMAN_PARKTYPE 
#define write_data_A_APC_PARKMAN_PARKTYPE 
#define lval_A_APC_PARKMAN_PARKTYPE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_PARKMAN_PARKTYPE_
#define A_APC_PARKMAN_PARKTYPE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_PARKMAN_PARKTYPE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_APC_PARKMAN_STATUS */
#define read_data_A_APC_PARKMAN_STATUS 
#define write_data_A_APC_PARKMAN_STATUS 
#define lval_A_APC_PARKMAN_STATUS cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_PARKMAN_STATUS_
#define A_APC_PARKMAN_STATUS cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_PARKMAN_STATUS_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_APC_PARK_DRIVE_POS_REQUEST */
#define read_data_A_APC_PARK_DRIVE_POS_REQUEST 
#define write_data_A_APC_PARK_DRIVE_POS_REQUEST 
#define lval_A_APC_PARK_DRIVE_POS_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_PARK_DRIVE_POS_REQUEST_
#define A_APC_PARK_DRIVE_POS_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_PARK_DRIVE_POS_REQUEST_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_APC_REMAINING_DISTANCE */
#define read_data_A_APC_REMAINING_DISTANCE 
#define write_data_A_APC_REMAINING_DISTANCE 
#define lval_A_APC_REMAINING_DISTANCE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_REMAINING_DISTANCE_
#define A_APC_REMAINING_DISTANCE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_REMAINING_DISTANCE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_APC_REMAINING_DISTANCE_STATE */
#define read_data_A_APC_REMAINING_DISTANCE_STATE 
#define write_data_A_APC_REMAINING_DISTANCE_STATE 
#define lval_A_APC_REMAINING_DISTANCE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_REMAINING_DISTANCE_STATE_
#define A_APC_REMAINING_DISTANCE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_REMAINING_DISTANCE_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_APC_VEHICLE_SPEED_REQUEST */
#define read_data_A_APC_VEHICLE_SPEED_REQUEST 
#define write_data_A_APC_VEHICLE_SPEED_REQUEST 
#define lval_A_APC_VEHICLE_SPEED_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_VEHICLE_SPEED_REQUEST_
#define A_APC_VEHICLE_SPEED_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_APC_VEHICLE_SPEED_REQUEST_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_BRKH_CUS_CTRL_STATE */
#define read_data_A_BRKH_CUS_CTRL_STATE 
#define write_data_A_BRKH_CUS_CTRL_STATE 
#define lval_A_BRKH_CUS_CTRL_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_BRKH_CUS_CTRL_STATE_
#define A_BRKH_CUS_CTRL_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_BRKH_CUS_CTRL_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_INITIAL_BRAKE_TORQUE */
#define read_data_A_INITIAL_BRAKE_TORQUE 
#define write_data_A_INITIAL_BRAKE_TORQUE 
#define lval_A_INITIAL_BRAKE_TORQUE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_INITIAL_BRAKE_TORQUE_
#define A_INITIAL_BRAKE_TORQUE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_INITIAL_BRAKE_TORQUE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_LAST_BRKH_CUS_CTRL_STATE */
#define read_data_A_LAST_BRKH_CUS_CTRL_STATE 
#define write_data_A_LAST_BRKH_CUS_CTRL_STATE 
#define lval_A_LAST_BRKH_CUS_CTRL_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_LAST_BRKH_CUS_CTRL_STATE_
#define A_LAST_BRKH_CUS_CTRL_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_LAST_BRKH_CUS_CTRL_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PARK_STYLE */
#define read_data_A_PARK_STYLE 
#define write_data_A_PARK_STYLE 
#define lval_A_PARK_STYLE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PARK_STYLE_
#define A_PARK_STYLE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PARK_STYLE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_PARKMAN_BRAKE_STATE */
#define read_data_A_PPB_PARKMAN_BRAKE_STATE 
#define write_data_A_PPB_PARKMAN_BRAKE_STATE 
#define lval_A_PPB_PARKMAN_BRAKE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARKMAN_BRAKE_STATE_
#define A_PPB_PARKMAN_BRAKE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARKMAN_BRAKE_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_CONTROL_MODE_REQUEST */
#define read_data_A_PPB_PARK_CONTROL_MODE_REQUEST 
#define write_data_A_PPB_PARK_CONTROL_MODE_REQUEST 
#define lval_A_PPB_PARK_CONTROL_MODE_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_CONTROL_MODE_REQUEST_
#define A_PPB_PARK_CONTROL_MODE_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_CONTROL_MODE_REQUEST_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_DRIVE_POS_REQUEST */
#define read_data_A_PPB_PARK_DRIVE_POS_REQUEST 
#define write_data_A_PPB_PARK_DRIVE_POS_REQUEST 
#define lval_A_PPB_PARK_DRIVE_POS_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_DRIVE_POS_REQUEST_
#define A_PPB_PARK_DRIVE_POS_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_DRIVE_POS_REQUEST_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_GUIDANCE_STATE */
#define read_data_A_PPB_PARK_GUIDANCE_STATE 
#define write_data_A_PPB_PARK_GUIDANCE_STATE 
#define lval_A_PPB_PARK_GUIDANCE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_GUIDANCE_STATE_
#define A_PPB_PARK_GUIDANCE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_GUIDANCE_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_PARKSTAT */
#define read_data_A_PPB_PARK_PARKSTAT 
#define write_data_A_PPB_PARK_PARKSTAT 
#define lval_A_PPB_PARK_PARKSTAT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_PARKSTAT_
#define A_PPB_PARK_PARKSTAT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_PARKSTAT_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_PARKTYPE */
#define read_data_A_PPB_PARK_PARKTYPE 
#define write_data_A_PPB_PARK_PARKTYPE 
#define lval_A_PPB_PARK_PARKTYPE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_PARKTYPE_
#define A_PPB_PARK_PARKTYPE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_PARKTYPE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_REMAINING_DISTANCE */
#define read_data_A_PPB_PARK_REMAINING_DISTANCE 
#define write_data_A_PPB_PARK_REMAINING_DISTANCE 
#define lval_A_PPB_PARK_REMAINING_DISTANCE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_REMAINING_DISTANCE_
#define A_PPB_PARK_REMAINING_DISTANCE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_REMAINING_DISTANCE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_REMAINING_DISTANCE_STATE */
#define read_data_A_PPB_PARK_REMAINING_DISTANCE_STATE 
#define write_data_A_PPB_PARK_REMAINING_DISTANCE_STATE 
#define lval_A_PPB_PARK_REMAINING_DISTANCE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_REMAINING_DISTANCE_STATE_
#define A_PPB_PARK_REMAINING_DISTANCE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_PARK_REMAINING_DISTANCE_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_PPB_VEHICLE_SPEED_REQUEST */
#define read_data_A_PPB_VEHICLE_SPEED_REQUEST 
#define write_data_A_PPB_VEHICLE_SPEED_REQUEST 
#define lval_A_PPB_VEHICLE_SPEED_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_VEHICLE_SPEED_REQUEST_
#define A_PPB_VEHICLE_SPEED_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_PPB_VEHICLE_SPEED_REQUEST_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_CAR_POSITION_LAST */
#define read_data_A_RD_CAR_POSITION_LAST 
#define write_data_A_RD_CAR_POSITION_LAST 
#define lval_A_RD_CAR_POSITION_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_CAR_POSITION_LAST_
#define A_RD_CAR_POSITION_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_CAR_POSITION_LAST_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_DRIVEN_DIST_SINCE_INIT_DTC */
#define read_data_A_RD_DRIVEN_DIST_SINCE_INIT_DTC 
#define write_data_A_RD_DRIVEN_DIST_SINCE_INIT_DTC 
#define lval_A_RD_DRIVEN_DIST_SINCE_INIT_DTC cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DRIVEN_DIST_SINCE_INIT_DTC_
#define A_RD_DRIVEN_DIST_SINCE_INIT_DTC cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DRIVEN_DIST_SINCE_INIT_DTC_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_DRIVEN_DIST_SINCE_INIT_DTH */
#define read_data_A_RD_DRIVEN_DIST_SINCE_INIT_DTH 
#define write_data_A_RD_DRIVEN_DIST_SINCE_INIT_DTH 
#define lval_A_RD_DRIVEN_DIST_SINCE_INIT_DTH cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DRIVEN_DIST_SINCE_INIT_DTH_
#define A_RD_DRIVEN_DIST_SINCE_INIT_DTH cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DRIVEN_DIST_SINCE_INIT_DTH_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_DTC */
#define read_data_A_RD_DTC 
#define write_data_A_RD_DTC 
#define lval_A_RD_DTC cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTC_
#define A_RD_DTC cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTC_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_DTC_AT_INIT */
#define read_data_A_RD_DTC_AT_INIT 
#define write_data_A_RD_DTC_AT_INIT 
#define lval_A_RD_DTC_AT_INIT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTC_AT_INIT_
#define A_RD_DTC_AT_INIT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTC_AT_INIT_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_DTC_LAST */
#define read_data_A_RD_DTC_LAST 
#define write_data_A_RD_DTC_LAST 
#define lval_A_RD_DTC_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTC_LAST_
#define A_RD_DTC_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTC_LAST_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_DTH */
#define read_data_A_RD_DTH 
#define write_data_A_RD_DTH 
#define lval_A_RD_DTH cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTH_
#define A_RD_DTH cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTH_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_DTH_AT_INIT */
#define read_data_A_RD_DTH_AT_INIT 
#define write_data_A_RD_DTH_AT_INIT 
#define lval_A_RD_DTH_AT_INIT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTH_AT_INIT_
#define A_RD_DTH_AT_INIT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTH_AT_INIT_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_RD_DTH_LAST */
#define read_data_A_RD_DTH_LAST 
#define write_data_A_RD_DTH_LAST 
#define lval_A_RD_DTH_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTH_LAST_
#define A_RD_DTH_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_DTH_LAST_

/* Macros for Data-Item: RD_CHECK_GEAR_CHANGE:A_RD_GEAR_LAST */
#define read_data_A_RD_GEAR_LAST 
#define write_data_A_RD_GEAR_LAST 
#define lval_A_RD_GEAR_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_GEAR_LAST_
#define A_RD_GEAR_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_GEAR_LAST_

/* Macros for Data-Item: RD_CALC_DTC:A_RD_JUMP_DTC */
#define read_data_A_RD_JUMP_DTC 
#define write_data_A_RD_JUMP_DTC 
#define lval_A_RD_JUMP_DTC cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_JUMP_DTC_
#define A_RD_JUMP_DTC cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_JUMP_DTC_

/* Macros for Data-Item: RD_CALC_DTH:A_RD_JUMP_DTH */
#define read_data_A_RD_JUMP_DTH 
#define write_data_A_RD_JUMP_DTH 
#define lval_A_RD_JUMP_DTH cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_JUMP_DTH_
#define A_RD_JUMP_DTH cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_JUMP_DTH_

/* Macros for Data-Item: RD_CHECK_GEAR_CHANGE:A_RD_NUM_CYCLES */
#define read_data_A_RD_NUM_CYCLES 
#define write_data_A_RD_NUM_CYCLES 
#define lval_A_RD_NUM_CYCLES cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_NUM_CYCLES_
#define A_RD_NUM_CYCLES cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_RD_NUM_CYCLES_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_REMAINING_DISTANCE */
#define read_data_A_REMAINING_DISTANCE 
#define write_data_A_REMAINING_DISTANCE 
#define lval_A_REMAINING_DISTANCE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_REMAINING_DISTANCE_
#define A_REMAINING_DISTANCE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_REMAINING_DISTANCE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:A_REMAINING_DISTANCE_LAST */
#define read_data_A_REMAINING_DISTANCE_LAST 
#define write_data_A_REMAINING_DISTANCE_LAST 
#define lval_A_REMAINING_DISTANCE_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_REMAINING_DISTANCE_LAST_
#define A_REMAINING_DISTANCE_LAST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.A_REMAINING_DISTANCE_LAST_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:C_APC_PARK_VEHICLE_SPEED_RQ_MAX */
typedef int STYPE_C_APC_PARK_VEHICLE_SPEED_RQ_MAX;
typedef unsigned int TYPE_C_APC_PARK_VEHICLE_SPEED_RQ_MAX;
#define C_APC_PARK_VEHICLE_SPEED_RQ_MAX 700

/* Macros for Data-Item: ACTIVATION_LOGIC_GDS:C_IDLE_VEHICLE_SPEED_REQUEST */
typedef int STYPE_C_IDLE_VEHICLE_SPEED_REQUEST;
typedef unsigned int TYPE_C_IDLE_VEHICLE_SPEED_REQUEST;
#define C_IDLE_VEHICLE_SPEED_REQUEST 1000

/* Macros for Data-Item: ACTIVATION_LOGIC_GDS:C_MAX_REMAINING_DISTANCE */
typedef int STYPE_C_MAX_REMAINING_DISTANCE;
typedef unsigned int TYPE_C_MAX_REMAINING_DISTANCE;
#define C_MAX_REMAINING_DISTANCE 20150

/* Macros for Data-Item: ACTIVATION_LOGIC_GDS:C_MIN_REMAINING_DISTANCE */
typedef int STYPE_C_MIN_REMAINING_DISTANCE;
typedef unsigned int TYPE_C_MIN_REMAINING_DISTANCE;
#define C_MIN_REMAINING_DISTANCE 170

/* Macros for Data-Item: ACTIVATION_LOGIC_GDS:C_MS_COUNTER_MAX */
typedef int STYPE_C_MS_COUNTER_MAX;
typedef unsigned int TYPE_C_MS_COUNTER_MAX;
#define C_MS_COUNTER_MAX 0xFFFF

/* Macros for Data-Item: ACTIVATION_LOGIC_GDS:C_NULL_REMAINING_DISTANCE */
typedef int STYPE_C_NULL_REMAINING_DISTANCE;
typedef unsigned int TYPE_C_NULL_REMAINING_DISTANCE;
#define C_NULL_REMAINING_DISTANCE 0

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:C_PMO_REMAINING_DISTANCE */
typedef int STYPE_C_PMO_REMAINING_DISTANCE;
typedef unsigned int TYPE_C_PMO_REMAINING_DISTANCE;
#define C_PMO_REMAINING_DISTANCE 20000

/* Macros for Data-Item: RD_CALC_DTH:C_RD_DTH_JUMP_THRESHOLD_PMI */
typedef int STYPE_C_RD_DTH_JUMP_THRESHOLD_PMI;
typedef unsigned int TYPE_C_RD_DTH_JUMP_THRESHOLD_PMI;
#define C_RD_DTH_JUMP_THRESHOLD_PMI 4000

/* Macros for Data-Item: RD_CALC_DTH:C_RD_DTH_JUMP_THRESHOLD_PMO */
typedef int STYPE_C_RD_DTH_JUMP_THRESHOLD_PMO;
typedef unsigned int TYPE_C_RD_DTH_JUMP_THRESHOLD_PMO;
#define C_RD_DTH_JUMP_THRESHOLD_PMO 2000

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:C_RD_JUMP_THRESHOLD */
typedef int STYPE_C_RD_JUMP_THRESHOLD;
typedef unsigned int TYPE_C_RD_JUMP_THRESHOLD;
#define C_RD_JUMP_THRESHOLD 200

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:C_RD_MAX_CYCLES */
typedef int STYPE_C_RD_MAX_CYCLES;
typedef unsigned int TYPE_C_RD_MAX_CYCLES;
#define C_RD_MAX_CYCLES 5

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:C_RD_RAMP_VALUE */
typedef int STYPE_C_RD_RAMP_VALUE;
typedef unsigned int TYPE_C_RD_RAMP_VALUE;
#define C_RD_RAMP_VALUE 8

/* Macros for Data-Item: PM_APC_CTRL:C_REDUCED_VEHICLE_SPEED_REQUEST */
typedef int STYPE_C_REDUCED_VEHICLE_SPEED_REQUEST;
typedef unsigned int TYPE_C_REDUCED_VEHICLE_SPEED_REQUEST;
#define C_REDUCED_VEHICLE_SPEED_REQUEST 150

/* Macros for Data-Item: CTRL_PPB_CHECK_SC:C_RELEASE_TORQUE */
typedef int STYPE_C_RELEASE_TORQUE;
typedef unsigned int TYPE_C_RELEASE_TORQUE;
#define C_RELEASE_TORQUE 500

/* Macros for Data-Item: ACTIVATION_LOGIC_GDS:C_SI16_MAX */
typedef int STYPE_C_SI16_MAX;
typedef unsigned int TYPE_C_SI16_MAX;
#define C_SI16_MAX 32767

/* Macros for Data-Item: ACTIVATION_LOGIC_GDS:C_SPEED_MINIMUM_APC_KMH */
typedef int STYPE_C_SPEED_MINIMUM_APC_KMH;
typedef unsigned int TYPE_C_SPEED_MINIMUM_APC_KMH;
#define C_SPEED_MINIMUM_APC_KMH 300

/* Macros for Data-Item: ACTIVATION_LOGIC_GDS:C_SPEED_MINIMUM_APC_MMS */
typedef int STYPE_C_SPEED_MINIMUM_APC_MMS;
typedef unsigned int TYPE_C_SPEED_MINIMUM_APC_MMS;
#define C_SPEED_MINIMUM_APC_MMS 108

/* Macros for Data-Item: TIMING_DESIGN_GDS:C_TASK_CYCLIC_TIME */
typedef int STYPE_C_TASK_CYCLIC_TIME;
typedef unsigned int TYPE_C_TASK_CYCLIC_TIME;
#define C_TASK_CYCLIC_TIME 20

/* Macros for Data-Item: CTRL_APC_CHECK_SC:LI_BRKH_GET_PARKTYPE */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_CAR_CURRENT_GEAR */
#ifndef lval_LI_COMH_CAR_CURRENT_GEAR
#define lval_LI_COMH_CAR_CURRENT_GEAR (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_CAR_CURRENT_GEAR cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_CAR_CURRENT_GEAR_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_CAR_GEAR_LEVER_POSITION */
#ifndef lval_LI_COMH_CAR_GEAR_LEVER_POSITION
#define lval_LI_COMH_CAR_GEAR_LEVER_POSITION (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_CAR_GEAR_LEVER_POSITION cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_CAR_GEAR_LEVER_POSITION_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_ESP_AFTERRUN_CTRL_ACTV */
#ifndef lval_LI_COMH_ESP_AFTERRUN_CTRL_ACTV
#define lval_LI_COMH_ESP_AFTERRUN_CTRL_ACTV (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_ESP_AFTERRUN_CTRL_ACTV cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_ESP_AFTERRUN_CTRL_ACTV_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_ESP_BRAKE_STATE */
#ifndef lval_LI_COMH_ESP_BRAKE_STATE
#define lval_LI_COMH_ESP_BRAKE_STATE (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_ESP_BRAKE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_ESP_BRAKE_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_ESP_INTERVENTION_STATE */
#ifndef lval_LI_COMH_ESP_INTERVENTION_STATE
#define lval_LI_COMH_ESP_INTERVENTION_STATE (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_ESP_INTERVENTION_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_ESP_INTERVENTION_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_ESP_SYSTEM_STATE */
#ifndef lval_LI_COMH_ESP_SYSTEM_STATE
#define lval_LI_COMH_ESP_SYSTEM_STATE (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_ESP_SYSTEM_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_ESP_SYSTEM_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_GET_BRAKE_PEDAL_TORQUE */
#ifndef lval_LI_COMH_GET_BRAKE_PEDAL_TORQUE
#define lval_LI_COMH_GET_BRAKE_PEDAL_TORQUE (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_GET_BRAKE_PEDAL_TORQUE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_GET_BRAKE_PEDAL_TORQUE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_KL15_STATUS */
#ifndef lval_LI_COMH_KL15_STATUS
#define lval_LI_COMH_KL15_STATUS (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_KL15_STATUS cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_KL15_STATUS_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_PARK_FLT_STAT_ESP */
#ifndef lval_LI_COMH_PARK_FLT_STAT_ESP
#define lval_LI_COMH_PARK_FLT_STAT_ESP (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_PARK_FLT_STAT_ESP cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_PARK_FLT_STAT_ESP_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_PT_READY */
#ifndef lval_LI_COMH_PT_READY
#define lval_LI_COMH_PT_READY (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_PT_READY cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_PT_READY_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_COMH_REAL_TIME_VHCL_SPEED */
#ifndef lval_LI_COMH_REAL_TIME_VHCL_SPEED
#define lval_LI_COMH_REAL_TIME_VHCL_SPEED (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_COMH_REAL_TIME_VHCL_SPEED cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_COMH_REAL_TIME_VHCL_SPEED_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_DRIVING_HINT */
#ifndef lval_LI_DAPM_DRIVING_HINT
#define lval_LI_DAPM_DRIVING_HINT (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_DAPM_DRIVING_HINT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_DAPM_DRIVING_HINT_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON */
#ifndef lval_LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON
#define lval_LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_GET_DISTANCE_TO_COLLISION */
#ifndef lval_LI_DAPM_GET_DISTANCE_TO_COLLISION
#define lval_LI_DAPM_GET_DISTANCE_TO_COLLISION (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_DAPM_GET_DISTANCE_TO_COLLISION cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_DAPM_GET_DISTANCE_TO_COLLISION_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_GET_DISTANCE_TO_HINT */
#ifndef lval_LI_DAPM_GET_DISTANCE_TO_HINT
#define lval_LI_DAPM_GET_DISTANCE_TO_HINT (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_DAPM_GET_DISTANCE_TO_HINT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_DAPM_GET_DISTANCE_TO_HINT_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_MAX_VELOCITY_RQ */
#ifndef lval_LI_DAPM_MAX_VELOCITY_RQ
#define lval_LI_DAPM_MAX_VELOCITY_RQ (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_DAPM_MAX_VELOCITY_RQ cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_DAPM_MAX_VELOCITY_RQ_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_OPERATION_STATE */
#ifndef lval_LI_DAPM_OPERATION_STATE
#define lval_LI_DAPM_OPERATION_STATE (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_DAPM_OPERATION_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_DAPM_OPERATION_STATE_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LI_ODOM_CAR_POSITION */
#ifndef lval_LI_ODOM_CAR_POSITION
#define lval_LI_ODOM_CAR_POSITION (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LI_ODOM_CAR_POSITION cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LI_ODOM_CAR_POSITION_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_ESP_PARK_BRAKING_MODE_REQUEST */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_ESP_PARK_CONTROL_MODE_REQUEST */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_ESP_PARK_DANGER_MODE_REQUEST */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_ESP_PARK_GUIDANCE_STATE */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_ESP_PARK_REMAINING_DISTANCE */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_ESP_PARK_REMAINING_DISTANCE_STATE */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_ESP_PARK_TRANS_DRIVING_POSITION_REQUEST */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_ESP_PARK_VEHICLE_SPEED_REQUEST */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_PARKMAN_BRAKE_STATE */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_PARKMAN_PARKTYPE */
/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:LO_COMH_PARKMAN_STATUS */
/* Macros for Data-Item: CTRL_APC_CHECK_SC:LO_PARK_PARK_STAT */
#ifndef lval_LO_PARK_PARK_STAT
#define lval_LO_PARK_PARK_STAT (*** Operation Not Supported: Direct Assignment to Buffered Data-Item ***)
#endif
#define LO_PARK_PARK_STAT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.LO_PARK_PARK_STAT_

/* Macros for Data-Item: ACTIVATION_LOGIC_TOP_AC:MS_COUNTER */
#define read_data_MS_COUNTER 
#define write_data_MS_COUNTER 
#define lval_MS_COUNTER cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.MS_COUNTER_
#define MS_COUNTER cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.MS_COUNTER_

/* Macros for Data-Item: TIMING_DESIGN_GDS:T_APC_AUTOGEAR_DELAY */
typedef int STYPE_T_APC_AUTOGEAR_DELAY;
typedef unsigned int TYPE_T_APC_AUTOGEAR_DELAY;
#define T_APC_AUTOGEAR_DELAY 200 / C_TASK_CYCLIC_TIME

/* Macros for Data-Item: TIMING_DESIGN_GDS:T_APC_MANGEAR_DELAY */
typedef int STYPE_T_APC_MANGEAR_DELAY;
typedef unsigned int TYPE_T_APC_MANGEAR_DELAY;
#define T_APC_MANGEAR_DELAY 80 / C_TASK_CYCLIC_TIME

/* Macros for Data-Item: TIMING_DESIGN_GDS:T_REMDIST_TOLERANCE */
typedef int STYPE_T_REMDIST_TOLERANCE;
typedef unsigned int TYPE_T_REMDIST_TOLERANCE;
#define T_REMDIST_TOLERANCE 60 / C_TASK_CYCLIC_TIME

/* Macros for Data-Item: TIMING_DESIGN_GDS:T_UZL_ACTIVATION_TOL */
typedef int STYPE_T_UZL_ACTIVATION_TOL;
typedef unsigned int TYPE_T_UZL_ACTIVATION_TOL;
#define T_UZL_ACTIVATION_TOL 60 / C_TASK_CYCLIC_TIME

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_APC_CTRL_ACTIVE */
#define A_APC_CTRL_ACTIVE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0
#define lval_A_APC_CTRL_ACTIVE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0
#define MAKE_TRUE_A_APC_CTRL_ACTIVE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0 = 1)
#define MAKE_FALSE_A_APC_CTRL_ACTIVE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0 = 0)
#define IS_TRUE_A_APC_CTRL_ACTIVE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0) == 1)
#define IS_FALSE_A_APC_CTRL_ACTIVE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0) == 0)
#define A_APC_CTRL_ACTIVE_MASK (UNSIGNED_MASK_TYPE)(0x01)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_CAR_READY_P4U */
#define A_CAR_READY_P4U cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b1
#define lval_A_CAR_READY_P4U cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b1
#define MAKE_TRUE_A_CAR_READY_P4U  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b1 = 1)
#define MAKE_FALSE_A_CAR_READY_P4U  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b1 = 0)
#define IS_TRUE_A_CAR_READY_P4U  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b1) == 1)
#define IS_FALSE_A_CAR_READY_P4U  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b1) == 0)
#define A_CAR_READY_P4U_MASK (UNSIGNED_MASK_TYPE)(0x02)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_PPB_CTRL_ACTIVE */
#define A_PPB_CTRL_ACTIVE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b2
#define lval_A_PPB_CTRL_ACTIVE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b2
#define MAKE_TRUE_A_PPB_CTRL_ACTIVE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b2 = 1)
#define MAKE_FALSE_A_PPB_CTRL_ACTIVE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b2 = 0)
#define IS_TRUE_A_PPB_CTRL_ACTIVE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b2) == 1)
#define IS_FALSE_A_PPB_CTRL_ACTIVE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b2) == 0)
#define A_PPB_CTRL_ACTIVE_MASK (UNSIGNED_MASK_TYPE)(0x04)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_PPB_PREFILL_ACTIVE */
#define A_PPB_PREFILL_ACTIVE cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0
#define lval_A_PPB_PREFILL_ACTIVE cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0
#define MAKE_TRUE_A_PPB_PREFILL_ACTIVE  (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0 = 1)
#define MAKE_FALSE_A_PPB_PREFILL_ACTIVE  (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0 = 0)
#define IS_TRUE_A_PPB_PREFILL_ACTIVE  ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0) == 1)
#define IS_FALSE_A_PPB_PREFILL_ACTIVE  ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b0) == 0)
#define A_PPB_PREFILL_ACTIVE_MASK (UNSIGNED_MASK_TYPE)(0x01)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_RD_CONTINUE_FEATURE */
#define A_RD_CONTINUE_FEATURE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b3
#define lval_A_RD_CONTINUE_FEATURE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b3
#define MAKE_TRUE_A_RD_CONTINUE_FEATURE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b3 = 1)
#define MAKE_FALSE_A_RD_CONTINUE_FEATURE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b3 = 0)
#define IS_TRUE_A_RD_CONTINUE_FEATURE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b3) == 1)
#define IS_FALSE_A_RD_CONTINUE_FEATURE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b3) == 0)
#define A_RD_CONTINUE_FEATURE_MASK (UNSIGNED_MASK_TYPE)(0x08)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_RD_DTH_JUMP_FLAG */
#define A_RD_DTH_JUMP_FLAG cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b4
#define lval_A_RD_DTH_JUMP_FLAG cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b4
#define MAKE_TRUE_A_RD_DTH_JUMP_FLAG  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b4 = 1)
#define MAKE_FALSE_A_RD_DTH_JUMP_FLAG  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b4 = 0)
#define IS_TRUE_A_RD_DTH_JUMP_FLAG  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b4) == 1)
#define IS_FALSE_A_RD_DTH_JUMP_FLAG  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b4) == 0)
#define A_RD_DTH_JUMP_FLAG_MASK (UNSIGNED_MASK_TYPE)(0x10)

/* Macros for Condition: RD_CALC_DTH:A_RD_DTH_JUMP_FLAG_REINIT */
#define A_RD_DTH_JUMP_FLAG_REINIT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b5
#define lval_A_RD_DTH_JUMP_FLAG_REINIT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b5
#define MAKE_TRUE_A_RD_DTH_JUMP_FLAG_REINIT  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b5 = 1)
#define MAKE_FALSE_A_RD_DTH_JUMP_FLAG_REINIT  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b5 = 0)
#define IS_TRUE_A_RD_DTH_JUMP_FLAG_REINIT  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b5) == 1)
#define IS_FALSE_A_RD_DTH_JUMP_FLAG_REINIT  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b5) == 0)
#define A_RD_DTH_JUMP_FLAG_REINIT_MASK (UNSIGNED_MASK_TYPE)(0x20)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_RD_GEAR_OVERRIDE */
#define A_RD_GEAR_OVERRIDE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b6
#define lval_A_RD_GEAR_OVERRIDE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b6
#define MAKE_TRUE_A_RD_GEAR_OVERRIDE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b6 = 1)
#define MAKE_FALSE_A_RD_GEAR_OVERRIDE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b6 = 0)
#define IS_TRUE_A_RD_GEAR_OVERRIDE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b6) == 1)
#define IS_FALSE_A_RD_GEAR_OVERRIDE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b6) == 0)
#define A_RD_GEAR_OVERRIDE_MASK (UNSIGNED_MASK_TYPE)(0x40)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_RD_INIT */
#define A_RD_INIT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b7
#define lval_A_RD_INIT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b7
#define MAKE_TRUE_A_RD_INIT  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b7 = 1)
#define MAKE_FALSE_A_RD_INIT  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b7 = 0)
#define IS_TRUE_A_RD_INIT  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b7) == 1)
#define IS_FALSE_A_RD_INIT  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus.b7) == 0)
#define A_RD_INIT_MASK (UNSIGNED_MASK_TYPE)(0x80)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_RD_NEW_MEAS */
#define A_RD_NEW_MEAS cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b0
#define lval_A_RD_NEW_MEAS cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b0
#define MAKE_TRUE_A_RD_NEW_MEAS  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b0 = 1)
#define MAKE_FALSE_A_RD_NEW_MEAS  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b0 = 0)
#define IS_TRUE_A_RD_NEW_MEAS  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b0) == 1)
#define IS_FALSE_A_RD_NEW_MEAS  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b0) == 0)
#define A_RD_NEW_MEAS_MASK (UNSIGNED_MASK_TYPE)(0x01)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_RD_SMOOTHING_RAMP_STARTED */
#define A_RD_SMOOTHING_RAMP_STARTED cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b1
#define lval_A_RD_SMOOTHING_RAMP_STARTED cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b1
#define MAKE_TRUE_A_RD_SMOOTHING_RAMP_STARTED  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b1 = 1)
#define MAKE_FALSE_A_RD_SMOOTHING_RAMP_STARTED  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b1 = 0)
#define IS_TRUE_A_RD_SMOOTHING_RAMP_STARTED  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b1) == 1)
#define IS_FALSE_A_RD_SMOOTHING_RAMP_STARTED  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b1) == 0)
#define A_RD_SMOOTHING_RAMP_STARTED_MASK (UNSIGNED_MASK_TYPE)(0x02)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:A_START_OF_FIRST_STROKE */
#define A_START_OF_FIRST_STROKE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b2
#define lval_A_START_OF_FIRST_STROKE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b2
#define MAKE_TRUE_A_START_OF_FIRST_STROKE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b2 = 1)
#define MAKE_FALSE_A_START_OF_FIRST_STROKE  (cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b2 = 0)
#define IS_TRUE_A_START_OF_FIRST_STROKE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b2) == 1)
#define IS_FALSE_A_START_OF_FIRST_STROKE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b2) == 0)
#define A_START_OF_FIRST_STROKE_MASK (UNSIGNED_MASK_TYPE)(0x04)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LI_BRKH_APPL_EMERGENCY_REQUEST */
#define LI_BRKH_APPL_EMERGENCY_REQUEST cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b3
#ifndef lval_LI_BRKH_APPL_EMERGENCY_REQUEST
#define lval_LI_BRKH_APPL_EMERGENCY_REQUEST (*** Operation Not Supported: Direct Assignment to Buffered Condition ***)
#endif
#define MAKE_TRUE_LI_BRKH_APPL_EMERGENCY_REQUEST (--Put here set call, using 1 --)
#define MAKE_FALSE_LI_BRKH_APPL_EMERGENCY_REQUEST  (--Put here set call, using 0 --)
#define IS_TRUE_LI_BRKH_APPL_EMERGENCY_REQUEST  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b3) == 1)
#define IS_FALSE_LI_BRKH_APPL_EMERGENCY_REQUEST  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b3) == 0)
#define LI_BRKH_APPL_EMERGENCY_REQUEST_MASK (UNSIGNED_MASK_TYPE)(0x08)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LI_COMH_APC_ESP_ENABLE_STATE */
#define LI_COMH_APC_ESP_ENABLE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b4
#ifndef lval_LI_COMH_APC_ESP_ENABLE_STATE
#define lval_LI_COMH_APC_ESP_ENABLE_STATE (*** Operation Not Supported: Direct Assignment to Buffered Condition ***)
#endif
#define MAKE_TRUE_LI_COMH_APC_ESP_ENABLE_STATE ()
#define MAKE_FALSE_LI_COMH_APC_ESP_ENABLE_STATE  ()
#define IS_TRUE_LI_COMH_APC_ESP_ENABLE_STATE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b4) == 1)
#define IS_FALSE_LI_COMH_APC_ESP_ENABLE_STATE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b4) == 0)
#define LI_COMH_APC_ESP_ENABLE_STATE_MASK (UNSIGNED_MASK_TYPE)(0x10)

/* Macros for Condition: CTRL_PPB_CHECK_SC:LI_COMH_PPB_ESP_ENABLE_STATE */
#define LI_COMH_PPB_ESP_ENABLE_STATE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b5
#ifndef lval_LI_COMH_PPB_ESP_ENABLE_STATE
#define lval_LI_COMH_PPB_ESP_ENABLE_STATE (*** Operation Not Supported: Direct Assignment to Buffered Condition ***)
#endif
#define MAKE_TRUE_LI_COMH_PPB_ESP_ENABLE_STATE (--Put here set call, using 1 --)
#define MAKE_FALSE_LI_COMH_PPB_ESP_ENABLE_STATE  (--Put here set call, using 0 --)
#define IS_TRUE_LI_COMH_PPB_ESP_ENABLE_STATE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b5) == 1)
#define IS_FALSE_LI_COMH_PPB_ESP_ENABLE_STATE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b5) == 0)
#define LI_COMH_PPB_ESP_ENABLE_STATE_MASK (UNSIGNED_MASK_TYPE)(0x20)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_EMERGENCY_IMMINENT */
#define LI_DAPM_EMERGENCY_IMMINENT cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b6
#ifndef lval_LI_DAPM_EMERGENCY_IMMINENT
#define lval_LI_DAPM_EMERGENCY_IMMINENT (*** Operation Not Supported: Direct Assignment to Buffered Condition ***)
#endif
#define MAKE_TRUE_LI_DAPM_EMERGENCY_IMMINENT (--Put here set call, using 1 --)
#define MAKE_FALSE_LI_DAPM_EMERGENCY_IMMINENT  (--Put here set call, using 0 --)
#define IS_TRUE_LI_DAPM_EMERGENCY_IMMINENT  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b6) == 1)
#define IS_FALSE_LI_DAPM_EMERGENCY_IMMINENT  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b6) == 0)
#define LI_DAPM_EMERGENCY_IMMINENT_MASK (UNSIGNED_MASK_TYPE)(0x40)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_EMERGENCY_REQUIRED */
#define LI_DAPM_EMERGENCY_REQUIRED cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b7
#ifndef lval_LI_DAPM_EMERGENCY_REQUIRED
#define lval_LI_DAPM_EMERGENCY_REQUIRED (*** Operation Not Supported: Direct Assignment to Buffered Condition ***)
#endif
#define MAKE_TRUE_LI_DAPM_EMERGENCY_REQUIRED (--Put here set call, using 1 --)
#define MAKE_FALSE_LI_DAPM_EMERGENCY_REQUIRED  (--Put here set call, using 0 --)
#define IS_TRUE_LI_DAPM_EMERGENCY_REQUIRED  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b7) == 1)
#define IS_FALSE_LI_DAPM_EMERGENCY_REQUIRED  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1.b7) == 0)
#define LI_DAPM_EMERGENCY_REQUIRED_MASK (UNSIGNED_MASK_TYPE)(0x80)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_IS_CTRL_ACTIVE */
#define LI_DAPM_IS_CTRL_ACTIVE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b0
#ifndef lval_LI_DAPM_IS_CTRL_ACTIVE
#define lval_LI_DAPM_IS_CTRL_ACTIVE (*** Operation Not Supported: Direct Assignment to Buffered Condition ***)
#endif
#define MAKE_TRUE_LI_DAPM_IS_CTRL_ACTIVE (--Put here set call, using 1 --)
#define MAKE_FALSE_LI_DAPM_IS_CTRL_ACTIVE  (--Put here set call, using 0 --)
#define IS_TRUE_LI_DAPM_IS_CTRL_ACTIVE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b0) == 1)
#define IS_FALSE_LI_DAPM_IS_CTRL_ACTIVE  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b0) == 0)
#define LI_DAPM_IS_CTRL_ACTIVE_MASK (UNSIGNED_MASK_TYPE)(0x01)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_IS_VEHICLE_STANDSTILL */
#define LI_DAPM_IS_VEHICLE_STANDSTILL cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b1
#ifndef lval_LI_DAPM_IS_VEHICLE_STANDSTILL
#define lval_LI_DAPM_IS_VEHICLE_STANDSTILL (*** Operation Not Supported: Direct Assignment to Buffered Condition ***)
#endif
#define MAKE_TRUE_LI_DAPM_IS_VEHICLE_STANDSTILL ()
#define MAKE_FALSE_LI_DAPM_IS_VEHICLE_STANDSTILL  ()
#define IS_TRUE_LI_DAPM_IS_VEHICLE_STANDSTILL  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b1) == 1)
#define IS_FALSE_LI_DAPM_IS_VEHICLE_STANDSTILL  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b1) == 0)
#define LI_DAPM_IS_VEHICLE_STANDSTILL_MASK (UNSIGNED_MASK_TYPE)(0x02)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LI_DAPM_STEERING_IN_STANDSTILL */
#define LI_DAPM_STEERING_IN_STANDSTILL cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b2
#ifndef lval_LI_DAPM_STEERING_IN_STANDSTILL
#define lval_LI_DAPM_STEERING_IN_STANDSTILL (*** Operation Not Supported: Direct Assignment to Buffered Condition ***)
#endif
#define MAKE_TRUE_LI_DAPM_STEERING_IN_STANDSTILL (--Put here set call, using 1 --)
#define MAKE_FALSE_LI_DAPM_STEERING_IN_STANDSTILL  (--Put here set call, using 0 --)
#define IS_TRUE_LI_DAPM_STEERING_IN_STANDSTILL  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b2) == 1)
#define IS_FALSE_LI_DAPM_STEERING_IN_STANDSTILL  ((cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus2.b2) == 0)
#define LI_DAPM_STEERING_IN_STANDSTILL_MASK (UNSIGNED_MASK_TYPE)(0x04)

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LO_BRKH_PPB_READY */
#define MAKE_TRUE_LO_BRKH_PPB_READY  (BRKH_SetEmergencyBrakeStatus(1))
#define MAKE_FALSE_LO_BRKH_PPB_READY  (BRKH_SetEmergencyBrakeStatus(0))
#define IS_TRUE_LO_BRKH_PPB_READY (--Put here retrieve call  --)
#define IS_FALSE_LO_BRKH_PPB_READY (! (--Put here retrieve call  --))
#define LO_BRKH_PPB_READY --Put here retrieve call  --

/* Macros for Condition: ACTIVATION_LOGIC_TOP_AC:LO_BRKH_SET_BRAKE_STATE */
#define MAKE_TRUE_LO_BRKH_SET_BRAKE_STATE  (BRKH_CusSetBrakeState(1))
#define MAKE_FALSE_LO_BRKH_SET_BRAKE_STATE  (BRKH_CusSetBrakeState(0))
#define IS_TRUE_LO_BRKH_SET_BRAKE_STATE (--Put here retrieve call  --)
#define IS_FALSE_LO_BRKH_SET_BRAKE_STATE (! (--Put here retrieve call  --))
#define LO_BRKH_SET_BRAKE_STATE --Put here retrieve call  --

/* Macros for Event: ACTIVATION_LOGIC_TOP_AC:EVT_EVALUATE_PARKTYPE */
#define BIT_EVT_EVALUATE_PARKTYPE (UNSIGNED_MASK_TYPE)(0x01)
#define GENERATE_EVENT_EVT_EVALUATE_PARKTYPE (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_EVT_EVALUATE_PARKTYPE)
#define EVT_EVALUATE_PARKTYPE ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus & BIT_EVT_EVALUATE_PARKTYPE) != 0)

/* Macros for Event: ACTIVATION_LOGIC_TOP_AC:EVT_UZL_FOLLOW_UP */
#define BIT_EVT_UZL_FOLLOW_UP (UNSIGNED_MASK_TYPE)(0x02)
#define GENERATE_EVENT_EVT_UZL_FOLLOW_UP (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_EVT_UZL_FOLLOW_UP)
#define EVT_UZL_FOLLOW_UP ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus & BIT_EVT_UZL_FOLLOW_UP) != 0)

/* Macros for Event: ACTIVATION_LOGIC_TOP_AC:LO_DAPM_ABORT_MANEUVER */
#define BIT_LO_DAPM_ABORT_MANEUVER (UNSIGNED_MASK_TYPE)(0x04)
#define GENERATE_EVENT_LO_DAPM_ABORT_MANEUVER (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_LO_DAPM_ABORT_MANEUVER)
#define LO_DAPM_ABORT_MANEUVER ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus & BIT_LO_DAPM_ABORT_MANEUVER) != 0)

/* Macros for Event: STARTED_CALCULATE_DRIVEN_DISTANCE */
#define BIT_STARTED_CALCULATE_DRIVEN_DISTANCE (UNSIGNED_MASK_TYPE)(0x08)
#define GENERATE_EVENT_STARTED_CALCULATE_DRIVEN_DISTANCE (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_STARTED_CALCULATE_DRIVEN_DISTANCE)
#define GEN_IN_CURRENT_STARTED_CALCULATE_DRIVEN_DISTANCE (cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_STARTED_CALCULATE_DRIVEN_DISTANCE)
#define GEN_IN_BUFF_STARTED_CALCULATE_DRIVEN_DISTANCE(BUFF) ( (BUFF)->cg_Events_brkh_cus |= BIT_STARTED_CALCULATE_DRIVEN_DISTANCE)
#define STARTED_CALCULATE_DRIVEN_DISTANCE ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus & BIT_STARTED_CALCULATE_DRIVEN_DISTANCE) != 0)

/* Macros for Event: CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION */
#define BIT_CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION (UNSIGNED_MASK_TYPE)(0x10)
#define GENERATE_EVENT_CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION)
#define GEN_IN_CURRENT_CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION (cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION)
#define GEN_IN_BUFF_CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION(BUFF) ( (BUFF)->cg_Events_brkh_cus |= BIT_CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION)
#define CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus & BIT_CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION) != 0)

/* Macros for Event: Internal Event (tm/dly) */
#define BIT_tm_999999998_brkh_cus (UNSIGNED_MASK_TYPE)(0x20)
#define GENERATE_EVENT_tm_999999998_brkh_cus (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_tm_999999998_brkh_cus)
#define GEN_IN_CURRENT_tm_999999998_brkh_cus (cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_tm_999999998_brkh_cus)
#define GEN_IN_BUFF_tm_999999998_brkh_cus(BUFF) ( (BUFF)->cg_Events_brkh_cus |= BIT_tm_999999998_brkh_cus)
#define tm_999999998_brkh_cus ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus & BIT_tm_999999998_brkh_cus) != 0)

/* Macros for Event: Internal Event (tm/dly) */
#define BIT_tm_999999997_brkh_cus (UNSIGNED_MASK_TYPE)(0x40)
#define GENERATE_EVENT_tm_999999997_brkh_cus (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_tm_999999997_brkh_cus)
#define GEN_IN_CURRENT_tm_999999997_brkh_cus (cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_tm_999999997_brkh_cus)
#define GEN_IN_BUFF_tm_999999997_brkh_cus(BUFF) ( (BUFF)->cg_Events_brkh_cus |= BIT_tm_999999997_brkh_cus)
#define tm_999999997_brkh_cus ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus & BIT_tm_999999997_brkh_cus) != 0)

/* Macros for Event: Internal Event (tm/dly) */
#define BIT_tm_999999992_brkh_cus (UNSIGNED_MASK_TYPE)(0x80)
#define GENERATE_EVENT_tm_999999992_brkh_cus (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_tm_999999992_brkh_cus)
#define GEN_IN_CURRENT_tm_999999992_brkh_cus (cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus |= BIT_tm_999999992_brkh_cus)
#define GEN_IN_BUFF_tm_999999992_brkh_cus(BUFF) ( (BUFF)->cg_Events_brkh_cus |= BIT_tm_999999992_brkh_cus)
#define tm_999999992_brkh_cus ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus & BIT_tm_999999992_brkh_cus) != 0)

/* Macros for Event: Internal Event (tm/dly) */
#define BIT_tm_999999991_brkh_cus (UNSIGNED_MASK_TYPE)(0x01)
#define GENERATE_EVENT_tm_999999991_brkh_cus (cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus1 |= BIT_tm_999999991_brkh_cus)
#define GEN_IN_CURRENT_tm_999999991_brkh_cus (cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus1 |= BIT_tm_999999991_brkh_cus)
#define GEN_IN_BUFF_tm_999999991_brkh_cus(BUFF) ( (BUFF)->cg_Events_brkh_cus1 |= BIT_tm_999999991_brkh_cus)
#define tm_999999991_brkh_cus ((cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus1 & BIT_tm_999999991_brkh_cus) != 0)

/* Macros for handling Timeout setting, */
/* linking concrete timeout with the appropriate timeout variable. */
/* sometimes, the same timeout variable will be used with few concrete timeouts. */
#define tm_999999998_brkh_cus_TM_MASK (UNSIGNED_MASK_TYPE)(0x01)
#define INSTALL_TM_tm_999999998_brkh_cus(D, C) \
                    cgTimeoutsMask_brkh_cus |= tm_999999998_brkh_cus_TM_MASK;\
                    tm_999999998_brkh_cus_TIME = (uint32)currentTick_brkh_cus + (D)

#define tm_999999997_brkh_cus_TM_MASK (UNSIGNED_MASK_TYPE)(0x02)
#define INSTALL_TM_tm_999999997_brkh_cus(D, C) \
                    cgTimeoutsMask_brkh_cus |= tm_999999997_brkh_cus_TM_MASK;\
                    tm_999999997_brkh_cus_TIME = (uint32)currentTick_brkh_cus + (D)

#define INSTALL_TM_tm_999999996_brkh_cus(D, C) \
                    cgTimeoutsMask_brkh_cus |= tm_999999998_brkh_cus_TM_MASK;\
                    tm_999999998_brkh_cus_TIME = (uint32)currentTick_brkh_cus + (D)

#define INSTALL_TM_tm_999999995_brkh_cus(D, C) \
                    cgTimeoutsMask_brkh_cus |= tm_999999998_brkh_cus_TM_MASK;\
                    tm_999999998_brkh_cus_TIME = (uint32)currentTick_brkh_cus + (D)

#define INSTALL_TM_tm_999999994_brkh_cus(D, C) \
                    cgTimeoutsMask_brkh_cus |= tm_999999998_brkh_cus_TM_MASK;\
                    tm_999999998_brkh_cus_TIME = (uint32)currentTick_brkh_cus + (D)

#define INSTALL_TM_tm_999999993_brkh_cus(D, C) \
                    cgTimeoutsMask_brkh_cus |= tm_999999998_brkh_cus_TM_MASK;\
                    tm_999999998_brkh_cus_TIME = (uint32)currentTick_brkh_cus + (D)

#define tm_999999992_brkh_cus_TM_MASK (UNSIGNED_MASK_TYPE)(0x04)
#define INSTALL_TM_tm_999999992_brkh_cus(D, C) \
                    cgTimeoutsMask_brkh_cus |= tm_999999992_brkh_cus_TM_MASK;\
                    tm_999999992_brkh_cus_TIME = (uint32)currentTick_brkh_cus + (D)

#define tm_999999991_brkh_cus_TM_MASK (UNSIGNED_MASK_TYPE)(0x08)
#define INSTALL_TM_tm_999999991_brkh_cus(D, C) \
                    cgTimeoutsMask_brkh_cus |= tm_999999991_brkh_cus_TM_MASK;\
                    tm_999999991_brkh_cus_TIME = (uint32)currentTick_brkh_cus + (D)


/* Dynamic (Run-Time) data initialization */
#ifndef RESET_DATA_brkh_cus
#define RESET_DATA_brkh_cus() {\
  MEM_SET(cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE, 0); \
  MEM_SET(cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE, 0); \
  MEM_SET(cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE, 0); \
  (*((uint8*)&(cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_BitsConditions_brkh_cus1))) = 0x4;\
  LI_COMH_REAL_TIME_VHCL_SPEED = 0; \
  LI_DAPM_GET_DISTANCE_TO_COLLISION = 0; \
  LI_DAPM_MAX_VELOCITY_RQ = C_SPEED_MINIMUM_APC_MMS; \
  lval_A_PARK_STYLE = PARK_IN; \
  lval_A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_NA; \
  lval_A_LAST_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_NA; \
  LI_DAPM_DRIVING_HINT = CAPP_DD_UNKNOWN; \
  lval_A_RD_GEAR_LAST = GEAR_POSITION_N; \
  LI_COMH_CAR_CURRENT_GEAR = GEAR_POSITION_N; \
  LI_COMH_CAR_GEAR_LEVER_POSITION = GEAR_POSITION_N; \
  LI_COMH_ESP_AFTERRUN_CTRL_ACTV = 0; \
  lval_A_APC_PARKMAN_BRAKE_STATE = BRAKING_STATE_PARKMAN_INACTIV; \
  lval_A_PPB_PARKMAN_BRAKE_STATE = BRAKING_STATE_PARKMAN_ENABLED; \
  lval_A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_IDLE; \
  lval_A_APC_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_IDLE; \
  lval_A_PPB_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_IDLE; \
  lval_A_PPB_PARK_CONTROL_MODE_REQUEST = PARK_CTRL_MODE_RQ_IDLE; \
  lval_A_PPB_PARK_GUIDANCE_STATE = PARK_GUIDANCE_STATE_INACTIVE; \
  lval_A_APC_PARKMAN_PARKTYPE = PARK_PARKTYPE_IDLE; \
  lval_A_PPB_PARK_PARKTYPE = PARK_PARKTYPE_IDLE; \
  lval_A_APC_PARKMAN_STATUS = PARK_PARK_STAT_IDLE; \
  lval_A_PPB_PARK_PARKSTAT = PARK_PARK_STAT_IDLE; \
  lval_A_APC_REMAINING_DISTANCE_STATE = REM_DIST_STAT_NO_STEP; \
  lval_A_PPB_PARK_REMAINING_DISTANCE_STATE = REM_DIST_STAT_NO_STEP; \
  LI_COMH_KL15_STATUS = KL15_ON; \
  lval_A_APC_REMAINING_DISTANCE = 0; \
  lval_A_PPB_PARK_REMAINING_DISTANCE = 0; \
  lval_A_APC_VEHICLE_SPEED_REQUEST = 0; \
  lval_A_PPB_VEHICLE_SPEED_REQUEST = 0; \
  LI_ODOM_CAR_POSITION = 0; \
  lval_A_RD_NUM_CYCLES = 0; \
  cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE = cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE;\
}
#endif

/* State Encoding bit-masks for: CTRL_REMDIST */
#define CTRL_REMDISTcnt1_BITS_RANGE 3

#define IS_IN_VALID_STATE_CTRL_REMDISTcnt1 ( \
  inLeafState(currentState_CTRL_REMDISTcnt1, DefaultOf_Chart_CTRL_REMDIST, StateInfo_CTRL_REMDISTcnt1) || \
  inLeafState(currentState_CTRL_REMDISTcnt1, CTRL_REMDISTst2, StateInfo_CTRL_REMDISTcnt1) || \
  currentState_CTRL_REMDISTcnt1 == 0 \
  )

#define FS_Chart_CTRL_REMDIST (UNSIGNED_MASK_TYPE)(0)
#define FM_Chart_CTRL_REMDIST (UNSIGNED_MASK_TYPE)(0)
#define FM3_Chart_CTRL_REMDIST (UNSIGNED_MASK_TYPE)(0)

#define FS_CTRL_REMDISTst2 (UNSIGNED_MASK_TYPE)(1)
#define FM_CTRL_REMDISTst2 (UNSIGNED_MASK_TYPE)(3)
#define FM3_CTRL_REMDISTst2 (UNSIGNED_MASK_TYPE)(3)

#define FS_DefaultOf_Chart_CTRL_REMDIST (UNSIGNED_MASK_TYPE)(2)
#define FM_DefaultOf_Chart_CTRL_REMDIST (UNSIGNED_MASK_TYPE)(3)
#define FM3_DefaultOf_Chart_CTRL_REMDIST (UNSIGNED_MASK_TYPE)(3)

/* State Encoding bit-masks for: APC_GUIDANCE_SC */
#define APC_GUIDANCE_SC_BITS_RANGE 2047

#define IS_IN_VALID_STATE_APC_GUIDANCE_SC ( \
  inLeafState(currentState_APC_GUIDANCE_SC, DefaultOf_APC_GEAR_CTRL, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, REVERSE_GEAR_WAIT, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, REVERSE_GEAR_STANDSTILL, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, REVERSE_GEAR_END_CHECK, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, R_INIT, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, R_CHECK, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, R_STANDSTILL, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, R_DRIVE, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, R_CONTINUE, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, D_INIT, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, D_CHECK, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, D_STANDSTILL, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, D_DRIVE, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, D_CONTINUE, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, APC_MANUAL_GEAR_RQ, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, APC_IDLE_GEAR, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, APC_GEAR_AUTOMATIC, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, DRIVE_GEAR_WAIT, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, DRIVE_GEAR_STANDSTILL, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, DRIVE_GEAR_END_CHECK, StateInfo_APC_GUIDANCE_SC) || \
  inLeafState(currentState_APC_GUIDANCE_SC, APC_AUTOGEAR_DELAY, StateInfo_APC_GUIDANCE_SC) || \
  currentState_APC_GUIDANCE_SC == 0 \
  )

#define FS_Chart_APC_GUIDANCE_SC (UNSIGNED_MASK_TYPE)(0)
#define FM_Chart_APC_GUIDANCE_SC (UNSIGNED_MASK_TYPE)(0)
#define FM3_Chart_APC_GUIDANCE_SC (UNSIGNED_MASK_TYPE)(0)

#define FS_APC_GEAR_CTRL (UNSIGNED_MASK_TYPE)(1)
#define FM_APC_GEAR_CTRL (UNSIGNED_MASK_TYPE)(1)
#define FM3_APC_GEAR_CTRL (UNSIGNED_MASK_TYPE)(2047)

#define FS_STATE_41 (UNSIGNED_MASK_TYPE)(3)
#define FM_STATE_41 (UNSIGNED_MASK_TYPE)(7)
#define FM3_STATE_41 (UNSIGNED_MASK_TYPE)(2047)

#define FS_APC_AUTOGEAR_DELAY (UNSIGNED_MASK_TYPE)(11)
#define FM_APC_AUTOGEAR_DELAY (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_AUTOGEAR_DELAY (UNSIGNED_MASK_TYPE)(127)

#define FS_APC_DRIVE_GEAR (UNSIGNED_MASK_TYPE)(19)
#define FM_APC_DRIVE_GEAR (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_DRIVE_GEAR (UNSIGNED_MASK_TYPE)(511)

#define FS_DRIVE_GEAR_END_CHECK (UNSIGNED_MASK_TYPE)(147)
#define FM_DRIVE_GEAR_END_CHECK (UNSIGNED_MASK_TYPE)(511)
#define FM3_DRIVE_GEAR_END_CHECK (UNSIGNED_MASK_TYPE)(511)

#define FS_DRIVE_GEAR_STANDSTILL (UNSIGNED_MASK_TYPE)(275)
#define FM_DRIVE_GEAR_STANDSTILL (UNSIGNED_MASK_TYPE)(511)
#define FM3_DRIVE_GEAR_STANDSTILL (UNSIGNED_MASK_TYPE)(511)

#define FS_DRIVE_GEAR_WAIT (UNSIGNED_MASK_TYPE)(403)
#define FM_DRIVE_GEAR_WAIT (UNSIGNED_MASK_TYPE)(511)
#define FM3_DRIVE_GEAR_WAIT (UNSIGNED_MASK_TYPE)(511)

#define FS_APC_GEAR_AUTOMATIC (UNSIGNED_MASK_TYPE)(27)
#define FM_APC_GEAR_AUTOMATIC (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_GEAR_AUTOMATIC (UNSIGNED_MASK_TYPE)(127)

#define FS_APC_IDLE_GEAR (UNSIGNED_MASK_TYPE)(35)
#define FM_APC_IDLE_GEAR (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_IDLE_GEAR (UNSIGNED_MASK_TYPE)(127)

#define FS_APC_MANUAL_GEAR_RQ (UNSIGNED_MASK_TYPE)(43)
#define FM_APC_MANUAL_GEAR_RQ (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_MANUAL_GEAR_RQ (UNSIGNED_MASK_TYPE)(127)

#define FS_APC_REQ_D (UNSIGNED_MASK_TYPE)(51)
#define FM_APC_REQ_D (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_REQ_D (UNSIGNED_MASK_TYPE)(2047)

#define FS_STATE_43 (UNSIGNED_MASK_TYPE)(179)
#define FM_STATE_43 (UNSIGNED_MASK_TYPE)(511)
#define FM3_STATE_43 (UNSIGNED_MASK_TYPE)(2047)

#define FS_D_CONTINUE (UNSIGNED_MASK_TYPE)(691)
#define FM_D_CONTINUE (UNSIGNED_MASK_TYPE)(2047)
#define FM3_D_CONTINUE (UNSIGNED_MASK_TYPE)(2047)

#define FS_D_DRIVE (UNSIGNED_MASK_TYPE)(1203)
#define FM_D_DRIVE (UNSIGNED_MASK_TYPE)(2047)
#define FM3_D_DRIVE (UNSIGNED_MASK_TYPE)(2047)

#define FS_D_STANDSTILL (UNSIGNED_MASK_TYPE)(1715)
#define FM_D_STANDSTILL (UNSIGNED_MASK_TYPE)(2047)
#define FM3_D_STANDSTILL (UNSIGNED_MASK_TYPE)(2047)

#define FS_D_CHECK (UNSIGNED_MASK_TYPE)(307)
#define FM_D_CHECK (UNSIGNED_MASK_TYPE)(511)
#define FM3_D_CHECK (UNSIGNED_MASK_TYPE)(511)

#define FS_D_INIT (UNSIGNED_MASK_TYPE)(435)
#define FM_D_INIT (UNSIGNED_MASK_TYPE)(511)
#define FM3_D_INIT (UNSIGNED_MASK_TYPE)(511)

#define FS_APC_REQ_R (UNSIGNED_MASK_TYPE)(59)
#define FM_APC_REQ_R (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_REQ_R (UNSIGNED_MASK_TYPE)(2047)

#define FS_STATE_45 (UNSIGNED_MASK_TYPE)(187)
#define FM_STATE_45 (UNSIGNED_MASK_TYPE)(511)
#define FM3_STATE_45 (UNSIGNED_MASK_TYPE)(2047)

#define FS_R_CONTINUE (UNSIGNED_MASK_TYPE)(699)
#define FM_R_CONTINUE (UNSIGNED_MASK_TYPE)(2047)
#define FM3_R_CONTINUE (UNSIGNED_MASK_TYPE)(2047)

#define FS_R_DRIVE (UNSIGNED_MASK_TYPE)(1211)
#define FM_R_DRIVE (UNSIGNED_MASK_TYPE)(2047)
#define FM3_R_DRIVE (UNSIGNED_MASK_TYPE)(2047)

#define FS_R_STANDSTILL (UNSIGNED_MASK_TYPE)(1723)
#define FM_R_STANDSTILL (UNSIGNED_MASK_TYPE)(2047)
#define FM3_R_STANDSTILL (UNSIGNED_MASK_TYPE)(2047)

#define FS_R_CHECK (UNSIGNED_MASK_TYPE)(315)
#define FM_R_CHECK (UNSIGNED_MASK_TYPE)(511)
#define FM3_R_CHECK (UNSIGNED_MASK_TYPE)(511)

#define FS_R_INIT (UNSIGNED_MASK_TYPE)(443)
#define FM_R_INIT (UNSIGNED_MASK_TYPE)(511)
#define FM3_R_INIT (UNSIGNED_MASK_TYPE)(511)

#define FS_APC_REVERSE_GEAR (UNSIGNED_MASK_TYPE)(67)
#define FM_APC_REVERSE_GEAR (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_REVERSE_GEAR (UNSIGNED_MASK_TYPE)(511)

#define FS_REVERSE_GEAR_END_CHECK (UNSIGNED_MASK_TYPE)(195)
#define FM_REVERSE_GEAR_END_CHECK (UNSIGNED_MASK_TYPE)(511)
#define FM3_REVERSE_GEAR_END_CHECK (UNSIGNED_MASK_TYPE)(511)

#define FS_REVERSE_GEAR_STANDSTILL (UNSIGNED_MASK_TYPE)(323)
#define FM_REVERSE_GEAR_STANDSTILL (UNSIGNED_MASK_TYPE)(511)
#define FM3_REVERSE_GEAR_STANDSTILL (UNSIGNED_MASK_TYPE)(511)

#define FS_REVERSE_GEAR_WAIT (UNSIGNED_MASK_TYPE)(451)
#define FM_REVERSE_GEAR_WAIT (UNSIGNED_MASK_TYPE)(511)
#define FM3_REVERSE_GEAR_WAIT (UNSIGNED_MASK_TYPE)(511)

#define FS_DefaultOf_APC_GEAR_CTRL (UNSIGNED_MASK_TYPE)(5)
#define FM_DefaultOf_APC_GEAR_CTRL (UNSIGNED_MASK_TYPE)(7)
#define FM3_DefaultOf_APC_GEAR_CTRL (UNSIGNED_MASK_TYPE)(7)

/* State Encoding bit-masks for: APC_VEHSPD_SC */
#define APC_VEHSPD_SCcnt1_BITS_RANGE 7

#define IS_IN_VALID_STATE_APC_VEHSPD_SCcnt1 ( \
  inLeafState(currentState_APC_VEHSPD_SCcnt1, DefaultOf_APC_VEHSPD_SCst2, StateInfo_APC_VEHSPD_SCcnt1) || \
  inLeafState(currentState_APC_VEHSPD_SCcnt1, NO_LIMIT, StateInfo_APC_VEHSPD_SCcnt1) || \
  currentState_APC_VEHSPD_SCcnt1 == 0 \
  )

#define FS_Chart_APC_VEHSPD_SC (UNSIGNED_MASK_TYPE)(0)
#define FM_Chart_APC_VEHSPD_SC (UNSIGNED_MASK_TYPE)(0)
#define FM3_Chart_APC_VEHSPD_SC (UNSIGNED_MASK_TYPE)(0)

#define FS_APC_VEHSPD_SCst2 (UNSIGNED_MASK_TYPE)(1)
#define FM_APC_VEHSPD_SCst2 (UNSIGNED_MASK_TYPE)(1)
#define FM3_APC_VEHSPD_SCst2 (UNSIGNED_MASK_TYPE)(7)

#define FS_NO_LIMIT (UNSIGNED_MASK_TYPE)(3)
#define FM_NO_LIMIT (UNSIGNED_MASK_TYPE)(7)
#define FM3_NO_LIMIT (UNSIGNED_MASK_TYPE)(7)

#define FS_DefaultOf_APC_VEHSPD_SCst2 (UNSIGNED_MASK_TYPE)(5)
#define FM_DefaultOf_APC_VEHSPD_SCst2 (UNSIGNED_MASK_TYPE)(7)
#define FM3_DefaultOf_APC_VEHSPD_SCst2 (UNSIGNED_MASK_TYPE)(7)

/* State Encoding bit-masks for: CTRL_APC_CHECK_SC */
#define CTRL_APC_CHECK_SC_BITS_RANGE 255

#define IS_IN_VALID_STATE_CTRL_APC_CHECK_SC ( \
  inLeafState(currentState_CTRL_APC_CHECK_SC, DefaultOf_CTRL_APC_CHECK, StateInfo_CTRL_APC_CHECK_SC) || \
  inLeafState(currentState_CTRL_APC_CHECK_SC, APC_TAKEOVER, StateInfo_CTRL_APC_CHECK_SC) || \
  inLeafState(currentState_CTRL_APC_CHECK_SC, APC_SEARCH, StateInfo_CTRL_APC_CHECK_SC) || \
  inLeafState(currentState_CTRL_APC_CHECK_SC, APC_END, StateInfo_CTRL_APC_CHECK_SC) || \
  inLeafState(currentState_CTRL_APC_CHECK_SC, APC_DISCLAIMER, StateInfo_CTRL_APC_CHECK_SC) || \
  inLeafState(currentState_CTRL_APC_CHECK_SC, APC_WAIT_CONTROL, StateInfo_CTRL_APC_CHECK_SC) || \
  inLeafState(currentState_CTRL_APC_CHECK_SC, APC_CONTROL, StateInfo_CTRL_APC_CHECK_SC) || \
  inLeafState(currentState_CTRL_APC_CHECK_SC, APC_ABORT, StateInfo_CTRL_APC_CHECK_SC) || \
  currentState_CTRL_APC_CHECK_SC == 0 \
  )

#define FS_Chart_CTRL_APC_CHECK_SC (UNSIGNED_MASK_TYPE)(0)
#define FM_Chart_CTRL_APC_CHECK_SC (UNSIGNED_MASK_TYPE)(0)
#define FM3_Chart_CTRL_APC_CHECK_SC (UNSIGNED_MASK_TYPE)(0)

#define FS_CTRL_APC_CHECK (UNSIGNED_MASK_TYPE)(1)
#define FM_CTRL_APC_CHECK (UNSIGNED_MASK_TYPE)(1)
#define FM3_CTRL_APC_CHECK (UNSIGNED_MASK_TYPE)(255)

#define FS_STATE_19 (UNSIGNED_MASK_TYPE)(3)
#define FM_STATE_19 (UNSIGNED_MASK_TYPE)(7)
#define FM3_STATE_19 (UNSIGNED_MASK_TYPE)(255)

#define FS_APC_ABORT (UNSIGNED_MASK_TYPE)(11)
#define FM_APC_ABORT (UNSIGNED_MASK_TYPE)(63)
#define FM3_APC_ABORT (UNSIGNED_MASK_TYPE)(63)

#define FS_APC_ACTIVE (UNSIGNED_MASK_TYPE)(19)
#define FM_APC_ACTIVE (UNSIGNED_MASK_TYPE)(63)
#define FM3_APC_ACTIVE (UNSIGNED_MASK_TYPE)(255)

#define FS_APC_CONTROL (UNSIGNED_MASK_TYPE)(83)
#define FM_APC_CONTROL (UNSIGNED_MASK_TYPE)(255)
#define FM3_APC_CONTROL (UNSIGNED_MASK_TYPE)(255)

#define FS_APC_WAIT_CONTROL (UNSIGNED_MASK_TYPE)(147)
#define FM_APC_WAIT_CONTROL (UNSIGNED_MASK_TYPE)(255)
#define FM3_APC_WAIT_CONTROL (UNSIGNED_MASK_TYPE)(255)

#define FS_APC_DISCLAIMER (UNSIGNED_MASK_TYPE)(27)
#define FM_APC_DISCLAIMER (UNSIGNED_MASK_TYPE)(63)
#define FM3_APC_DISCLAIMER (UNSIGNED_MASK_TYPE)(63)

#define FS_APC_END (UNSIGNED_MASK_TYPE)(35)
#define FM_APC_END (UNSIGNED_MASK_TYPE)(63)
#define FM3_APC_END (UNSIGNED_MASK_TYPE)(63)

#define FS_APC_SEARCH (UNSIGNED_MASK_TYPE)(43)
#define FM_APC_SEARCH (UNSIGNED_MASK_TYPE)(63)
#define FM3_APC_SEARCH (UNSIGNED_MASK_TYPE)(63)

#define FS_APC_TAKEOVER (UNSIGNED_MASK_TYPE)(51)
#define FM_APC_TAKEOVER (UNSIGNED_MASK_TYPE)(63)
#define FM3_APC_TAKEOVER (UNSIGNED_MASK_TYPE)(63)

#define FS_DefaultOf_CTRL_APC_CHECK (UNSIGNED_MASK_TYPE)(5)
#define FM_DefaultOf_CTRL_APC_CHECK (UNSIGNED_MASK_TYPE)(7)
#define FM3_DefaultOf_CTRL_APC_CHECK (UNSIGNED_MASK_TYPE)(7)

/* State Encoding bit-masks for: CTRL_PPB_CHECK_SC */
#define CTRL_PPB_CHECK_SC_BITS_RANGE 127

#define IS_IN_VALID_STATE_CTRL_PPB_CHECK_SC ( \
  inLeafState(currentState_CTRL_PPB_CHECK_SC, DefaultOf_PPB, StateInfo_CTRL_PPB_CHECK_SC) || \
  inLeafState(currentState_CTRL_PPB_CHECK_SC, DefaultOf_PPB_READY, StateInfo_CTRL_PPB_CHECK_SC) || \
  inLeafState(currentState_CTRL_PPB_CHECK_SC, PPB_PREFILL, StateInfo_CTRL_PPB_CHECK_SC) || \
  inLeafState(currentState_CTRL_PPB_CHECK_SC, EMERGENCY_READY, StateInfo_CTRL_PPB_CHECK_SC) || \
  inLeafState(currentState_CTRL_PPB_CHECK_SC, CTRL_WAIT, StateInfo_CTRL_PPB_CHECK_SC) || \
  inLeafState(currentState_CTRL_PPB_CHECK_SC, CTRL_CHECK, StateInfo_CTRL_PPB_CHECK_SC) || \
  inLeafState(currentState_CTRL_PPB_CHECK_SC, EMERGENCY_IDLE, StateInfo_CTRL_PPB_CHECK_SC) || \
  currentState_CTRL_PPB_CHECK_SC == 0 \
  )

#define FS_Chart_CTRL_PPB_CHECK_SC (UNSIGNED_MASK_TYPE)(0)
#define FM_Chart_CTRL_PPB_CHECK_SC (UNSIGNED_MASK_TYPE)(0)
#define FM3_Chart_CTRL_PPB_CHECK_SC (UNSIGNED_MASK_TYPE)(0)

#define FS_PPB (UNSIGNED_MASK_TYPE)(1)
#define FM_PPB (UNSIGNED_MASK_TYPE)(1)
#define FM3_PPB (UNSIGNED_MASK_TYPE)(127)

#define FS_STATE_15st1 (UNSIGNED_MASK_TYPE)(3)
#define FM_STATE_15st1 (UNSIGNED_MASK_TYPE)(7)
#define FM3_STATE_15st1 (UNSIGNED_MASK_TYPE)(127)

#define FS_EMERGENCY_IDLE (UNSIGNED_MASK_TYPE)(11)
#define FM_EMERGENCY_IDLE (UNSIGNED_MASK_TYPE)(31)
#define FM3_EMERGENCY_IDLE (UNSIGNED_MASK_TYPE)(31)

#define FS_PPB_CTRL (UNSIGNED_MASK_TYPE)(19)
#define FM_PPB_CTRL (UNSIGNED_MASK_TYPE)(31)
#define FM3_PPB_CTRL (UNSIGNED_MASK_TYPE)(127)

#define FS_CTRL_CHECK (UNSIGNED_MASK_TYPE)(51)
#define FM_CTRL_CHECK (UNSIGNED_MASK_TYPE)(127)
#define FM3_CTRL_CHECK (UNSIGNED_MASK_TYPE)(127)

#define FS_CTRL_WAIT (UNSIGNED_MASK_TYPE)(83)
#define FM_CTRL_WAIT (UNSIGNED_MASK_TYPE)(127)
#define FM3_CTRL_WAIT (UNSIGNED_MASK_TYPE)(127)

#define FS_PPB_READY (UNSIGNED_MASK_TYPE)(27)
#define FM_PPB_READY (UNSIGNED_MASK_TYPE)(31)
#define FM3_PPB_READY (UNSIGNED_MASK_TYPE)(127)

#define FS_EMERGENCY_READY (UNSIGNED_MASK_TYPE)(59)
#define FM_EMERGENCY_READY (UNSIGNED_MASK_TYPE)(127)
#define FM3_EMERGENCY_READY (UNSIGNED_MASK_TYPE)(127)

#define FS_PPB_PREFILL (UNSIGNED_MASK_TYPE)(91)
#define FM_PPB_PREFILL (UNSIGNED_MASK_TYPE)(127)
#define FM3_PPB_PREFILL (UNSIGNED_MASK_TYPE)(127)

#define FS_DefaultOf_PPB_READY (UNSIGNED_MASK_TYPE)(123)
#define FM_DefaultOf_PPB_READY (UNSIGNED_MASK_TYPE)(127)
#define FM3_DefaultOf_PPB_READY (UNSIGNED_MASK_TYPE)(127)

#define FS_DefaultOf_PPB (UNSIGNED_MASK_TYPE)(5)
#define FM_DefaultOf_PPB (UNSIGNED_MASK_TYPE)(7)
#define FM3_DefaultOf_PPB (UNSIGNED_MASK_TYPE)(7)

/* State Encoding bit-masks for: UZL_CONTROL_SC */
#define UZL_CONTROL_SC_BITS_RANGE 511

#define IS_IN_VALID_STATE_UZL_CONTROL_SC ( \
  inLeafState(currentState_UZL_CONTROL_SC, DefaultOf_Chart_UZL_CONTROL_SC, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, UZL_CONTROL_TERMINATE_TO_FOLLOW_UP, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, UZL_PASSIVE, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, UZL_ABORT, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, DefaultOf_PM_PPB, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, PM_PPB_PREFILLst1, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, PM_PPB_CTRL, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, DefaultOf_APC_PPB_CONTROL, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, PM_PPB_PREFILLst2, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, APC_PPB_CTRL, StateInfo_UZL_CONTROL_SC) || \
  inLeafState(currentState_UZL_CONTROL_SC, APC_ONLY_CONTROL, StateInfo_UZL_CONTROL_SC) || \
  currentState_UZL_CONTROL_SC == 0 \
  )

#define FS_Chart_UZL_CONTROL_SC (UNSIGNED_MASK_TYPE)(0)
#define FM_Chart_UZL_CONTROL_SC (UNSIGNED_MASK_TYPE)(0)
#define FM3_Chart_UZL_CONTROL_SC (UNSIGNED_MASK_TYPE)(0)

#define FS_STATE_6 (UNSIGNED_MASK_TYPE)(1)
#define FM_STATE_6 (UNSIGNED_MASK_TYPE)(3)
#define FM3_STATE_6 (UNSIGNED_MASK_TYPE)(511)

#define FS_APC_CTRL (UNSIGNED_MASK_TYPE)(5)
#define FM_APC_CTRL (UNSIGNED_MASK_TYPE)(31)
#define FM3_APC_CTRL (UNSIGNED_MASK_TYPE)(511)

#define FS_APC_ONLY_CONTROL (UNSIGNED_MASK_TYPE)(37)
#define FM_APC_ONLY_CONTROL (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_ONLY_CONTROL (UNSIGNED_MASK_TYPE)(127)

#define FS_APC_PPB_CONTROL (UNSIGNED_MASK_TYPE)(69)
#define FM_APC_PPB_CONTROL (UNSIGNED_MASK_TYPE)(127)
#define FM3_APC_PPB_CONTROL (UNSIGNED_MASK_TYPE)(511)

#define FS_APC_PPB_CTRL (UNSIGNED_MASK_TYPE)(197)
#define FM_APC_PPB_CTRL (UNSIGNED_MASK_TYPE)(511)
#define FM3_APC_PPB_CTRL (UNSIGNED_MASK_TYPE)(511)

#define FS_PM_PPB_PREFILLst2 (UNSIGNED_MASK_TYPE)(325)
#define FM_PM_PPB_PREFILLst2 (UNSIGNED_MASK_TYPE)(511)
#define FM3_PM_PPB_PREFILLst2 (UNSIGNED_MASK_TYPE)(511)

#define FS_DefaultOf_APC_PPB_CONTROL (UNSIGNED_MASK_TYPE)(453)
#define FM_DefaultOf_APC_PPB_CONTROL (UNSIGNED_MASK_TYPE)(511)
#define FM3_DefaultOf_APC_PPB_CONTROL (UNSIGNED_MASK_TYPE)(511)

#define FS_PM_PPB (UNSIGNED_MASK_TYPE)(9)
#define FM_PM_PPB (UNSIGNED_MASK_TYPE)(31)
#define FM3_PM_PPB (UNSIGNED_MASK_TYPE)(127)

#define FS_PM_PPB_CTRL (UNSIGNED_MASK_TYPE)(41)
#define FM_PM_PPB_CTRL (UNSIGNED_MASK_TYPE)(127)
#define FM3_PM_PPB_CTRL (UNSIGNED_MASK_TYPE)(127)

#define FS_PM_PPB_PREFILLst1 (UNSIGNED_MASK_TYPE)(73)
#define FM_PM_PPB_PREFILLst1 (UNSIGNED_MASK_TYPE)(127)
#define FM3_PM_PPB_PREFILLst1 (UNSIGNED_MASK_TYPE)(127)

#define FS_DefaultOf_PM_PPB (UNSIGNED_MASK_TYPE)(105)
#define FM_DefaultOf_PM_PPB (UNSIGNED_MASK_TYPE)(127)
#define FM3_DefaultOf_PM_PPB (UNSIGNED_MASK_TYPE)(127)

#define FS_UZL_ACTIVE_SWITCH (UNSIGNED_MASK_TYPE)(13)
#define FM_UZL_ACTIVE_SWITCH (UNSIGNED_MASK_TYPE)(31)
#define FM3_UZL_ACTIVE_SWITCH (UNSIGNED_MASK_TYPE)(127)

#define FS_UZL_ABORT (UNSIGNED_MASK_TYPE)(45)
#define FM_UZL_ABORT (UNSIGNED_MASK_TYPE)(127)
#define FM3_UZL_ABORT (UNSIGNED_MASK_TYPE)(127)

#define FS_UZL_PASSIVE (UNSIGNED_MASK_TYPE)(77)
#define FM_UZL_PASSIVE (UNSIGNED_MASK_TYPE)(127)
#define FM3_UZL_PASSIVE (UNSIGNED_MASK_TYPE)(127)

#define FS_UZL_CONTROL_TERMINATE_TO_FOLLOW_UP (UNSIGNED_MASK_TYPE)(17)
#define FM_UZL_CONTROL_TERMINATE_TO_FOLLOW_UP (UNSIGNED_MASK_TYPE)(31)
#define FM3_UZL_CONTROL_TERMINATE_TO_FOLLOW_UP (UNSIGNED_MASK_TYPE)(31)

#define FS_DefaultOf_Chart_UZL_CONTROL_SC (UNSIGNED_MASK_TYPE)(2)
#define FM_DefaultOf_Chart_UZL_CONTROL_SC (UNSIGNED_MASK_TYPE)(3)
#define FM3_DefaultOf_Chart_UZL_CONTROL_SC (UNSIGNED_MASK_TYPE)(3)

/* State Encoding bit-masks for: BRKH_CUS_SC */
#define BRKH_CUS_SCcnt1_BITS_RANGE 255

#define IS_IN_VALID_STATE_BRKH_CUS_SCcnt1 ( \
  inLeafState(currentState_BRKH_CUS_SCcnt1, DefaultOf_BRKH_CUS_SCst2, StateInfo_BRKH_CUS_SCcnt1) || \
  inLeafState(currentState_BRKH_CUS_SCcnt1, UZL_CONTROL_ACTIVE, StateInfo_BRKH_CUS_SCcnt1) || \
  inLeafState(currentState_BRKH_CUS_SCcnt1, PM_NA, StateInfo_BRKH_CUS_SCcnt1) || \
  inLeafState(currentState_BRKH_CUS_SCcnt1, PM_INIT, StateInfo_BRKH_CUS_SCcnt1) || \
  inLeafState(currentState_BRKH_CUS_SCcnt1, PM_INACTIVE, StateInfo_BRKH_CUS_SCcnt1) || \
  inLeafState(currentState_BRKH_CUS_SCcnt1, PM_FOLLOW_UP_CONTROL, StateInfo_BRKH_CUS_SCcnt1) || \
  currentState_BRKH_CUS_SCcnt1 == 0 \
  )

#define FS_Chart_BRKH_CUS_SC (UNSIGNED_MASK_TYPE)(0)
#define FM_Chart_BRKH_CUS_SC (UNSIGNED_MASK_TYPE)(0)
#define FM3_Chart_BRKH_CUS_SC (UNSIGNED_MASK_TYPE)(0)

#define FS_BRKH_CUS_SCst2 (UNSIGNED_MASK_TYPE)(1)
#define FM_BRKH_CUS_SCst2 (UNSIGNED_MASK_TYPE)(1)
#define FM3_BRKH_CUS_SCst2 (UNSIGNED_MASK_TYPE)(255)

#define FS_STATE_10st2 (UNSIGNED_MASK_TYPE)(3)
#define FM_STATE_10st2 (UNSIGNED_MASK_TYPE)(7)
#define FM3_STATE_10st2 (UNSIGNED_MASK_TYPE)(255)

#define FS_PM_H_INACTIVE (UNSIGNED_MASK_TYPE)(11)
#define FM_PM_H_INACTIVE (UNSIGNED_MASK_TYPE)(63)
#define FM3_PM_H_INACTIVE (UNSIGNED_MASK_TYPE)(255)

#define FS_PM_FOLLOW_UP_CONTROL (UNSIGNED_MASK_TYPE)(75)
#define FM_PM_FOLLOW_UP_CONTROL (UNSIGNED_MASK_TYPE)(255)
#define FM3_PM_FOLLOW_UP_CONTROL (UNSIGNED_MASK_TYPE)(255)

#define FS_PM_INACTIVE (UNSIGNED_MASK_TYPE)(139)
#define FM_PM_INACTIVE (UNSIGNED_MASK_TYPE)(255)
#define FM3_PM_INACTIVE (UNSIGNED_MASK_TYPE)(255)

#define FS_PM_INIT (UNSIGNED_MASK_TYPE)(19)
#define FM_PM_INIT (UNSIGNED_MASK_TYPE)(63)
#define FM3_PM_INIT (UNSIGNED_MASK_TYPE)(63)

#define FS_PM_NA (UNSIGNED_MASK_TYPE)(27)
#define FM_PM_NA (UNSIGNED_MASK_TYPE)(63)
#define FM3_PM_NA (UNSIGNED_MASK_TYPE)(63)

#define FS_UZL_CONTROL_ACTIVE (UNSIGNED_MASK_TYPE)(35)
#define FM_UZL_CONTROL_ACTIVE (UNSIGNED_MASK_TYPE)(63)
#define FM3_UZL_CONTROL_ACTIVE (UNSIGNED_MASK_TYPE)(63)

#define FS_DefaultOf_BRKH_CUS_SCst2 (UNSIGNED_MASK_TYPE)(5)
#define FM_DefaultOf_BRKH_CUS_SCst2 (UNSIGNED_MASK_TYPE)(7)
#define FM3_DefaultOf_BRKH_CUS_SCst2 (UNSIGNED_MASK_TYPE)(7)


/* End of code for: macro_def.h */


/* Code for: type_def.h */

typedef void (* TASK_H)(void);
typedef uint16 SW_COUNTER_H_brkh_cus;
/* user defined types: */
typedef enum DAPM_operation_abort_reason_E EN_SPECIFIC_CTRL_ABORT_REASON_E;/* ACTIVATION_LOGIC_TOP_AC:EN_SPECIFIC_CTRL_ABORT_REASON_E */  
typedef park_flt_stat_esp_E EN_PARK_FAULT_STAT_ESP;/* ACTIVATION_LOGIC_TOP_AC:EN_PARK_FAULT_STAT_ESP */  
typedef enum DAPM_operation_state_E EN_OPERATION_STATE;/* ACTIVATION_LOGIC_TOP_AC:EN_OPERATION_STATE */  
typedef KL15_state_E EN_KL_15_TYPE;/* DAIMLER_BR213_GDS:EN_KL_15_TYPE */  
typedef esp_intervention_E EN_ESP_INTERVENTION;/* ACTIVATION_LOGIC_TOP_AC:EN_ESP_INTERVENTION */  
typedef engine_running_state_E EN_COMH_POWERTRAIN_READY_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_POWERTRAIN_READY_TYPE */  
typedef remaining_distance_stat_E EN_COMH_PARK_REMIN_DIST_STATE_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_PARK_REMIN_DIST_STATE_TYPE */  
typedef park_park_stat_E EN_COMH_PARK_PARKMAN_STAT_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_PARK_PARKMAN_STAT_TYPE */  
typedef park_parktype_E EN_COMH_PARK_PARKMAN_MODE_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_PARK_PARKMAN_MODE_TYPE */  
typedef park_guidance_state_E EN_COMH_PARK_GUIDANCE_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_PARK_GUIDANCE_TYPE */  
typedef danger_mode_rq_E EN_COMH_PARK_DANGER_MODE_REQUEST_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_PARK_DANGER_MODE_REQUEST_TYPE */  
typedef park_control_mode_rq_E EN_COMH_PARK_CONTROL_MODE_REQUEST_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_PARK_CONTROL_MODE_REQUEST_TYPE */  
typedef park_brake_mode_E EN_COMH_PARK_BRAKE_MODE_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_PARK_BRAKE_MODE_TYPE */  
typedef gear_request_E EN_COMH_GEAR_POSITION_REQUEST_TYPE;/* DAIMLER_BR213_GDS:EN_COMH_GEAR_POSITION_REQUEST_TYPE */  
typedef esp_system_state_E EN_COMH_ESP_SYSTEM_STATE_TYPE;/* Sensor activation type.  */  
typedef esp_brake_state_E EN_COMH_ESP_STATUS_TYPE;/* ESP System Status */  
typedef ldc_afterrun_ctrl_E EN_COMH_ESP_AFTERRUN_CTRL_TYPE;/* ACTIVATION_LOGIC_TOP_AC:EN_COMH_ESP_AFTERRUN_CTRL_TYPE */  
typedef gear_position_E EN_COMH_AUTO_TRANSMISSION_GEAR_STATE_TYPE;/* ACTIVATION_LOGIC_GDS:EN_COMH_AUTO_TRANSMISSION_GEAR_STATE_TYPE */  
typedef enum CAPP_dr_dir_E EN_CAPP_DR_DIR_TYPE;/* CAPP driving direction enumeration type.  */  
typedef enum EN_BRKH_CUS_CTRL_STATE_TYPE {BRKH_CUS_PM_NA , BRKH_CUS_PM_INIT , BRKH_CUS_PM_INACTIVE , BRKH_CUS_PM_FOLLOW_UP_CTRL 
, BRKH_CUS_PM_APC_CTRL , BRKH_CUS_PM_RPA_CTRL , BRKH_CUS_PM_UNDO_RPA_CTRL 
, BRKH_CUS_PM_DAA_CTRL , BRKH_CUS_PM_PPB_CTRL , BRKH_CUS_PM_RCTA_CTRL 
, BRKH_CUS_PM_RCTA_PREFILL , BRKH_CUS_PM_HIT_MODE} EN_BRKH_CUS_CTRL_STATE_TYPE;/* DAIMLER_BR213_GDS:EN_BRKH_CUS_CTRL_STATE_TYPE */  
typedef brkh_maneuver_type_E EN_ACTL_PARK_STYLE;/* ACTIVATION_LOGIC_GDS:EN_ACTL_PARK_STYLE */  

typedef uint8 StateInfo_CTRL_REMDISTcnt1;
typedef uint16 StateInfo_APC_GUIDANCE_SC;
typedef uint8 StateInfo_APC_VEHSPD_SCcnt1;
typedef uint8 StateInfo_CTRL_APC_CHECK_SC;
typedef uint8 StateInfo_CTRL_PPB_CHECK_SC;
typedef uint16 StateInfo_UZL_CONTROL_SC;
typedef uint8 StateInfo_BRKH_CUS_SCcnt1;
typedef si16 STYPE_A_APC_REMAINING_DISTANCE;
typedef unsigned int TYPE_A_APC_REMAINING_DISTANCE;
typedef u16 STYPE_A_APC_VEHICLE_SPEED_REQUEST;
typedef unsigned int TYPE_A_APC_VEHICLE_SPEED_REQUEST;
typedef si16 STYPE_A_INITIAL_BRAKE_TORQUE;
typedef unsigned int TYPE_A_INITIAL_BRAKE_TORQUE;
typedef si16 STYPE_A_PPB_PARK_REMAINING_DISTANCE;
typedef unsigned int TYPE_A_PPB_PARK_REMAINING_DISTANCE;
typedef u16 STYPE_A_PPB_VEHICLE_SPEED_REQUEST;
typedef unsigned int TYPE_A_PPB_VEHICLE_SPEED_REQUEST;
typedef u32 STYPE_A_RD_CAR_POSITION_LAST;
typedef unsigned int TYPE_A_RD_CAR_POSITION_LAST;
typedef u32 STYPE_A_RD_DRIVEN_DIST_SINCE_INIT_DTC;
typedef unsigned int TYPE_A_RD_DRIVEN_DIST_SINCE_INIT_DTC;
typedef u32 STYPE_A_RD_DRIVEN_DIST_SINCE_INIT_DTH;
typedef unsigned int TYPE_A_RD_DRIVEN_DIST_SINCE_INIT_DTH;
typedef si16 STYPE_A_RD_DTC;
typedef unsigned int TYPE_A_RD_DTC;
typedef si16 STYPE_A_RD_DTC_AT_INIT;
typedef unsigned int TYPE_A_RD_DTC_AT_INIT;
typedef si16 STYPE_A_RD_DTC_LAST;
typedef unsigned int TYPE_A_RD_DTC_LAST;
typedef si16 STYPE_A_RD_DTH;
typedef unsigned int TYPE_A_RD_DTH;
typedef si16 STYPE_A_RD_DTH_AT_INIT;
typedef unsigned int TYPE_A_RD_DTH_AT_INIT;
typedef si16 STYPE_A_RD_DTH_LAST;
typedef unsigned int TYPE_A_RD_DTH_LAST;
typedef si32 STYPE_A_RD_JUMP_DTC;
typedef unsigned int TYPE_A_RD_JUMP_DTC;
typedef si32 STYPE_A_RD_JUMP_DTH;
typedef unsigned int TYPE_A_RD_JUMP_DTH;
typedef u8 STYPE_A_RD_NUM_CYCLES;
typedef unsigned int TYPE_A_RD_NUM_CYCLES;
typedef si16 STYPE_A_REMAINING_DISTANCE;
typedef unsigned int TYPE_A_REMAINING_DISTANCE;
typedef si16 STYPE_A_REMAINING_DISTANCE_LAST;
typedef unsigned int TYPE_A_REMAINING_DISTANCE_LAST;

typedef si16 STYPE_LI_COMH_GET_BRAKE_PEDAL_TORQUE;
typedef unsigned int TYPE_LI_COMH_GET_BRAKE_PEDAL_TORQUE;
typedef uint16 STYPE_LI_COMH_REAL_TIME_VHCL_SPEED;
typedef unsigned int TYPE_LI_COMH_REAL_TIME_VHCL_SPEED;
typedef sint16 STYPE_LI_DAPM_GET_DISTANCE_TO_COLLISION;
typedef unsigned int TYPE_LI_DAPM_GET_DISTANCE_TO_COLLISION;
typedef sint16 STYPE_LI_DAPM_GET_DISTANCE_TO_HINT;
typedef unsigned int TYPE_LI_DAPM_GET_DISTANCE_TO_HINT;
typedef sint16 STYPE_LI_DAPM_MAX_VELOCITY_RQ;
typedef unsigned int TYPE_LI_DAPM_MAX_VELOCITY_RQ;
typedef u32 STYPE_LI_ODOM_CAR_POSITION;
typedef unsigned int TYPE_LI_ODOM_CAR_POSITION;




typedef si16 STYPE_LO_COMH_ESP_PARK_REMAINING_DISTANCE;
typedef unsigned int TYPE_LO_COMH_ESP_PARK_REMAINING_DISTANCE;



typedef u16 STYPE_LO_COMH_ESP_PARK_VEHICLE_SPEED_REQUEST;
typedef unsigned int TYPE_LO_COMH_ESP_PARK_VEHICLE_SPEED_REQUEST;




typedef u16 STYPE_MS_COUNTER;
typedef unsigned int TYPE_MS_COUNTER;

struct cgDoubleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE_type {
  uint8 cg_Events_brkh_cus;  /* ACTIVATION_LOGIC_TOP_AC:EVT_EVALUATE_PARKTYPE :  0x01 */  
  /* ACTIVATION_LOGIC_TOP_AC:EVT_UZL_FOLLOW_UP :  0x02 */  
  /* ACTIVATION_LOGIC_TOP_AC:LO_DAPM_ABORT_MANEUVER :  0x04 */  
  /* STARTED_CALCULATE_DRIVEN_DISTANCE :  0x08 */  
  /* CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION :  0x10 */  
  /* Internal Event (tm/dly) :  0x20 */  
  /* Internal Event (tm/dly) :  0x40 */  
  /* Internal Event (tm/dly) :  0x80 */  

  uint8 cg_Events_brkh_cus1;  /* Internal Event (tm/dly) :  0x01 */  

  AMCBitsStruct8 cg_BitsConditions_brkh_cus;  /* ACTIVATION_LOGIC_TOP_AC:A_PPB_PREFILL_ACTIVE : b0 */  

};
typedef struct cgDoubleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE_type cgDoubleBufferType_BRKH_CUSBRAKESTATEMACHINE;

struct cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE_type {
  AMCBitsStruct8 cg_BitsConditions_brkh_cus;  /* APC state to send to UZL : b0 *//* @Traceability :Sys_APC_PTS_Logic_4119*/  
  /* ACTIVATION_LOGIC_TOP_AC:A_CAR_READY_P4U : b1 */  
  /* PPB Control state active : b2 */  
  /* Internal variable for continue feature : b3 */  
  /* Flag to indicate, that there was a jump detected : b4 */  
  /* RD_CALC_DTH:A_RD_DTH_JUMP_FLAG_REINIT : b5 */  
  /* Flag to indicate gear change is going on : b6 */  
  /* Init flag for Remaining Distance calculation : b7 */  

  AMCBitsStruct8 cg_BitsConditions_brkh_cus1;  /* Flag to indicate that new measurement flag was set once : b0 */  
  /* Indicates, if the ramping to a value was already started : b1 */  
  /* Indicates the beginning of the first stroke : b2 */  
  /* ACTIVATION_LOGIC_TOP_AC:LI_BRKH_APPL_EMERGENCY_REQUEST : b3 */  
  /* APC is enabled in ESP : b4 *//* @Traceability :ID-256*/  
  /* CTRL_PPB_CHECK_SC:LI_COMH_PPB_ESP_ENABLE_STATE : b5 */  
  /* ACTIVATION_LOGIC_TOP_AC:LI_DAPM_EMERGENCY_IMMINENT : b6 */  
  /* ACTIVATION_LOGIC_TOP_AC:LI_DAPM_EMERGENCY_REQUIRED : b7 */  

  AMCBitsStruct8 cg_BitsConditions_brkh_cus2;  /* ACTIVATION_LOGIC_TOP_AC:LI_DAPM_IS_CTRL_ACTIVE : b0 */  
  /* variable to detect vehicle standstill : b1 */  
  /* Wait steering to finish before requesting to change the gear : b2 *//* @Traceability :ID-*/  

  uint16 LI_COMH_REAL_TIME_VHCL_SPEED_;/* Real time vehicle speed *//* @Traceability :ID-265*/  
  sint16 LI_DAPM_GET_DISTANCE_TO_COLLISION_;/* Read the distance to an object with collision threat in driving direction *//* @Traceability :ID-*/  
  sint16 LI_DAPM_GET_DISTANCE_TO_HINT_;/* Read the distance to next switching point in driving tube *//* @Traceability :ID-*/  
  sint16 LI_DAPM_MAX_VELOCITY_RQ_;/* Max VehSpd requested by DAPM (VCTL/VIPO) *//* @Traceability :ID-*/  
  EN_ACTL_PARK_STYLE A_PARK_STYLE_;/* ACTIVATION_LOGIC_TOP_AC:A_PARK_STYLE */  
  EN_BRKH_CUS_CTRL_STATE_TYPE A_BRKH_CUS_CTRL_STATE_;/* local variable returns UZL status */  
  EN_BRKH_CUS_CTRL_STATE_TYPE A_LAST_BRKH_CUS_CTRL_STATE_;/* ACTIVATION_LOGIC_TOP_AC:A_LAST_BRKH_CUS_CTRL_STATE */  
  EN_CAPP_DR_DIR_TYPE LI_DAPM_DRIVING_HINT_;/* Gets the driving hint.  *//* @Traceability :ID-*/  
  EN_COMH_AUTO_TRANSMISSION_GEAR_STATE_TYPE A_RD_GEAR_LAST_;/* RD_CHECK_GEAR_CHANGE:A_RD_GEAR_LAST */  
  EN_COMH_AUTO_TRANSMISSION_GEAR_STATE_TYPE LI_COMH_CAR_CURRENT_GEAR_;/* ACTIVATION_LOGIC_TOP_AC:LI_COMH_CAR_CURRENT_GEAR */  
  EN_COMH_AUTO_TRANSMISSION_GEAR_STATE_TYPE LI_COMH_CAR_GEAR_LEVER_POSITION_;/* Current gear lever position *//* @Traceability :ID-*/  
  EN_COMH_ESP_AFTERRUN_CTRL_TYPE LI_COMH_ESP_AFTERRUN_CTRL_ACTV_;/* ESP afterrun is active *//* @Traceability :ID-*/  
  EN_COMH_ESP_STATUS_TYPE A_APC_PARKMAN_BRAKE_STATE_;/* ACTIVATION_LOGIC_TOP_AC:A_APC_PARKMAN_BRAKE_STATE */  
  EN_COMH_ESP_STATUS_TYPE A_PPB_PARKMAN_BRAKE_STATE_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_PARKMAN_BRAKE_STATE */  
  EN_COMH_ESP_STATUS_TYPE LI_COMH_ESP_BRAKE_STATE_;/* ESP ParkMan brake state *//* @Traceability :ID-71*/  
  EN_COMH_ESP_SYSTEM_STATE_TYPE LI_COMH_ESP_SYSTEM_STATE_;/* ESP system state *//* @Traceability :ID-36*/  
  EN_COMH_GEAR_POSITION_REQUEST_TYPE A_APC_AUTOGEAR_REQ_;/* Internal variable to handle gear request */  
  EN_COMH_GEAR_POSITION_REQUEST_TYPE A_APC_PARK_DRIVE_POS_REQUEST_;/* Park driven position request send to UZL from APC */  
  EN_COMH_GEAR_POSITION_REQUEST_TYPE A_PPB_PARK_DRIVE_POS_REQUEST_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_DRIVE_POS_REQUEST */  
  EN_COMH_PARK_CONTROL_MODE_REQUEST_TYPE A_PPB_PARK_CONTROL_MODE_REQUEST_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_CONTROL_MODE_REQUEST */  
  EN_COMH_PARK_GUIDANCE_TYPE A_PPB_PARK_GUIDANCE_STATE_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_GUIDANCE_STATE */  
  EN_COMH_PARK_PARKMAN_MODE_TYPE A_APC_PARKMAN_PARKTYPE_;/* ACTIVATION_LOGIC_TOP_AC:A_APC_PARKMAN_PARKTYPE */  
  EN_COMH_PARK_PARKMAN_MODE_TYPE A_PPB_PARK_PARKTYPE_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_PARKTYPE */  
  EN_COMH_PARK_PARKMAN_STAT_TYPE A_APC_PARKMAN_STATUS_;/* ACTIVATION_LOGIC_TOP_AC:A_APC_PARKMAN_STATUS */  
  EN_COMH_PARK_PARKMAN_STAT_TYPE A_PPB_PARK_PARKSTAT_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_PARKSTAT */  
  EN_COMH_PARK_PARKMAN_STAT_TYPE LO_PARK_PARK_STAT_;/* CTRL_APC_CHECK_SC:LO_PARK_PARK_STAT */  
  EN_COMH_PARK_REMIN_DIST_STATE_TYPE A_APC_REMAINING_DISTANCE_STATE_;/* ACTIVATION_LOGIC_TOP_AC:A_APC_REMAINING_DISTANCE_STATE */  
  EN_COMH_PARK_REMIN_DIST_STATE_TYPE A_PPB_PARK_REMAINING_DISTANCE_STATE_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_REMAINING_DISTANCE_STATE */  
  EN_COMH_POWERTRAIN_READY_TYPE LI_COMH_PT_READY_;/* Powertrain Ready *//* @Traceability :ID-141*/  
  EN_ESP_INTERVENTION LI_COMH_ESP_INTERVENTION_STATE_;/* ACTIVATION_LOGIC_TOP_AC:LI_COMH_ESP_INTERVENTION_STATE */  
  EN_KL_15_TYPE LI_COMH_KL15_STATUS_;/* Clamp15 ignition *//* @Traceability :ID-10*/  
  EN_OPERATION_STATE LI_DAPM_OPERATION_STATE_;/* ACTIVATION_LOGIC_TOP_AC:LI_DAPM_OPERATION_STATE */  
  EN_PARK_FAULT_STAT_ESP LI_COMH_PARK_FLT_STAT_ESP_;/* ACTIVATION_LOGIC_TOP_AC:LI_COMH_PARK_FLT_STAT_ESP */  
  EN_SPECIFIC_CTRL_ABORT_REASON_E LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON_;/* ACTIVATION_LOGIC_TOP_AC:LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON */  
  si16 A_APC_REMAINING_DISTANCE_;/* APC remaining distance local variable */  
  si16 A_INITIAL_BRAKE_TORQUE_;/* ACTIVATION_LOGIC_TOP_AC:A_INITIAL_BRAKE_TORQUE */  
  si16 A_PPB_PARK_REMAINING_DISTANCE_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_PARK_REMAINING_DISTANCE */  
  si16 A_RD_DTC_;/* Calculated value for DTC */  
  si16 A_RD_DTC_AT_INIT_;/* Value for DTC at time of init */  
  si16 A_RD_DTC_LAST_;/* Calculated value for DTC from cycle n-1 */  
  si16 A_RD_DTH_;/* Calculated/smoothed value for DTH */  
  si16 A_RD_DTH_AT_INIT_;/* Value of DTH at time of init */  
  si16 A_RD_DTH_LAST_;/* Calculated/smoothed value from cycle n-1 for DTH */  
  si16 A_REMAINING_DISTANCE_;/* Calculated remaining distance */  
  si16 A_REMAINING_DISTANCE_LAST_;/* Calculated remaining distance from t-1 step */  
  si16 LI_COMH_GET_BRAKE_PEDAL_TORQUE_;/* ACTIVATION_LOGIC_TOP_AC:LI_COMH_GET_BRAKE_PEDAL_TORQUE */  
  si32 A_RD_JUMP_DTC_;/* Calculated jump of DTC in current cycle */  
  si32 A_RD_JUMP_DTH_;/* Calculated jump of DTH in current cycle */  
  u16 A_APC_VEHICLE_SPEED_REQUEST_;/* internal variable of LO_COMH_ESP_PARK_VEHICLE_SPEED_REQUEST */  
  u16 A_PPB_VEHICLE_SPEED_REQUEST_;/* ACTIVATION_LOGIC_TOP_AC:A_PPB_VEHICLE_SPEED_REQUEST */  
  u16 MS_COUNTER_;/* ACTIVATION_LOGIC_TOP_AC:MS_COUNTER */  
  u32 A_RD_CAR_POSITION_LAST_;/* Car position at cycle n-1 */  
  u32 A_RD_DRIVEN_DIST_SINCE_INIT_DTC_;/* Driven distance of car since last init */  
  u32 A_RD_DRIVEN_DIST_SINCE_INIT_DTH_;/* Driven distance of car since last init */  
  u32 LI_ODOM_CAR_POSITION_;/* car position from Odometry *//* @Traceability :ID-*/  
  u8 A_RD_NUM_CYCLES_;/* RD_CHECK_GEAR_CHANGE:A_RD_NUM_CYCLES */  
};
typedef struct cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE_type cgSingleBufferType_BRKH_CUSBRAKESTATEMACHINE;


#if defined __HC12__ || defined __HC08__
extern  void cgEnterActions_CTRL_REMDISTcnt1(StateInfo_CTRL_REMDISTcnt1 staySame_brkh_cusCTRL_REMDISTcnt1, StateInfo_CTRL_REMDISTcnt1 nextState_brkh_cusCTRL_REMDISTcnt1);
#else
extern void cgEnterActions_CTRL_REMDISTcnt1(StateInfo_CTRL_REMDISTcnt1 staySame_brkh_cusCTRL_REMDISTcnt1, StateInfo_CTRL_REMDISTcnt1 nextState_brkh_cusCTRL_REMDISTcnt1);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgEnterActions_APC_GUIDANCE_SC(StateInfo_APC_GUIDANCE_SC staySame_brkh_cusAPC_GUIDANCE_SC, StateInfo_APC_GUIDANCE_SC nextState_brkh_cusAPC_GUIDANCE_SC);
#else
extern void cgEnterActions_APC_GUIDANCE_SC(StateInfo_APC_GUIDANCE_SC staySame_brkh_cusAPC_GUIDANCE_SC, StateInfo_APC_GUIDANCE_SC nextState_brkh_cusAPC_GUIDANCE_SC);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgEnterActions_APC_VEHSPD_SCcnt1(StateInfo_APC_VEHSPD_SCcnt1 staySame_brkh_cusAPC_VEHSPD_SCcnt1, StateInfo_APC_VEHSPD_SCcnt1 nextState_brkh_cusAPC_VEHSPD_SCcnt1);
#else
extern void cgEnterActions_APC_VEHSPD_SCcnt1(StateInfo_APC_VEHSPD_SCcnt1 staySame_brkh_cusAPC_VEHSPD_SCcnt1, StateInfo_APC_VEHSPD_SCcnt1 nextState_brkh_cusAPC_VEHSPD_SCcnt1);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgEnterActions_CTRL_APC_CHECK_SC(StateInfo_CTRL_APC_CHECK_SC staySame_brkh_cusCTRL_APC_CHECK_SC, StateInfo_CTRL_APC_CHECK_SC nextState_brkh_cusCTRL_APC_CHECK_SC);
#else
extern void cgEnterActions_CTRL_APC_CHECK_SC(StateInfo_CTRL_APC_CHECK_SC staySame_brkh_cusCTRL_APC_CHECK_SC, StateInfo_CTRL_APC_CHECK_SC nextState_brkh_cusCTRL_APC_CHECK_SC);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgExitActions_CTRL_APC_CHECK_SC(StateInfo_CTRL_APC_CHECK_SC staySame_brkh_cusCTRL_APC_CHECK_SC);
#else
extern void cgExitActions_CTRL_APC_CHECK_SC(StateInfo_CTRL_APC_CHECK_SC staySame_brkh_cusCTRL_APC_CHECK_SC);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgEnterActions_CTRL_PPB_CHECK_SC(StateInfo_CTRL_PPB_CHECK_SC staySame_brkh_cusCTRL_PPB_CHECK_SC, StateInfo_CTRL_PPB_CHECK_SC nextState_brkh_cusCTRL_PPB_CHECK_SC);
#else
extern void cgEnterActions_CTRL_PPB_CHECK_SC(StateInfo_CTRL_PPB_CHECK_SC staySame_brkh_cusCTRL_PPB_CHECK_SC, StateInfo_CTRL_PPB_CHECK_SC nextState_brkh_cusCTRL_PPB_CHECK_SC);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgEnterActions_UZL_CONTROL_SC(StateInfo_UZL_CONTROL_SC staySame_brkh_cusUZL_CONTROL_SC, StateInfo_UZL_CONTROL_SC nextState_brkh_cusUZL_CONTROL_SC);
#else
extern void cgEnterActions_UZL_CONTROL_SC(StateInfo_UZL_CONTROL_SC staySame_brkh_cusUZL_CONTROL_SC, StateInfo_UZL_CONTROL_SC nextState_brkh_cusUZL_CONTROL_SC);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgExitActions_UZL_CONTROL_SC(StateInfo_UZL_CONTROL_SC staySame_brkh_cusUZL_CONTROL_SC);
#else
extern void cgExitActions_UZL_CONTROL_SC(StateInfo_UZL_CONTROL_SC staySame_brkh_cusUZL_CONTROL_SC);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgEnterActions_BRKH_CUS_SCcnt1(StateInfo_BRKH_CUS_SCcnt1 staySame_brkh_cusBRKH_CUS_SCcnt1, StateInfo_BRKH_CUS_SCcnt1 nextState_brkh_cusBRKH_CUS_SCcnt1);
#else
extern void cgEnterActions_BRKH_CUS_SCcnt1(StateInfo_BRKH_CUS_SCcnt1 staySame_brkh_cusBRKH_CUS_SCcnt1, StateInfo_BRKH_CUS_SCcnt1 nextState_brkh_cusBRKH_CUS_SCcnt1);
#endif
#if defined __HC12__ || defined __HC08__
extern  void cgExitActions_BRKH_CUS_SCcnt1(StateInfo_BRKH_CUS_SCcnt1 staySame_brkh_cusBRKH_CUS_SCcnt1);
#else
extern void cgExitActions_BRKH_CUS_SCcnt1(StateInfo_BRKH_CUS_SCcnt1 staySame_brkh_cusBRKH_CUS_SCcnt1);
#endif
#if defined __HC12__ || defined __HC08__
extern  void genTmEvent_BRKH_CUSBRAKESTATEMACHINE(uint16 currentTickVar, cgDoubleBufferType_BRKH_CUSBRAKESTATEMACHINE* buff, uint8 counterIndex);
#else
extern void genTmEvent_BRKH_CUSBRAKESTATEMACHINE(uint16 currentTickVar, cgDoubleBufferType_BRKH_CUSBRAKESTATEMACHINE* buff, uint8 counterIndex);
#endif


extern  EN_ACTL_PARK_STYLE BRKH_SetParkStyle(void);

extern  void BRKH_Overflow(void);





/* End of code for: type_def.h */


/* Code for: glob_dat.h */



extern uint8 cgGlobalFlags_brkh_cus;
extern uint8 cgTimeoutsMask_brkh_cus;
extern StateInfo_CTRL_REMDISTcnt1 currentState_CTRL_REMDISTcnt1;
extern StateInfo_APC_VEHSPD_SCcnt1 currentState_APC_VEHSPD_SCcnt1;
extern StateInfo_CTRL_APC_CHECK_SC currentState_CTRL_APC_CHECK_SC;
extern StateInfo_CTRL_PPB_CHECK_SC currentState_CTRL_PPB_CHECK_SC;
extern StateInfo_BRKH_CUS_SCcnt1 currentState_BRKH_CUS_SCcnt1;
extern uint16 currentTick_brkh_cus;
extern StateInfo_APC_GUIDANCE_SC currentState_APC_GUIDANCE_SC;
extern StateInfo_UZL_CONTROL_SC currentState_UZL_CONTROL_SC;
extern uint32 tm_999999998_brkh_cus_TIME;
extern uint32 tm_999999997_brkh_cus_TIME;
extern uint32 tm_999999992_brkh_cus_TIME;
extern uint32 tm_999999991_brkh_cus_TIME;
extern cgDoubleBufferType_BRKH_CUSBRAKESTATEMACHINE cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE;
extern cgDoubleBufferType_BRKH_CUSBRAKESTATEMACHINE cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE;
extern cgSingleBufferType_BRKH_CUSBRAKESTATEMACHINE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE;

/* End of code for: glob_dat.h */


/* Code for: os_decl.h */

extern void onMS_COUNTER_OVERFLOW(void);
extern void BRKH_CUSBRAKESTATEMACHINE(void);

/* End of code for: os_decl.h */


#endif

/* End of generated file */
