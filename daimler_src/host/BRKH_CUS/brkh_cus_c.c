/*
 *	 ***************************************************************
 *	  Licensed Materials - Property of IBM
 *	  (C)Copyright IBM Corporation 1989. All Rights Reserved.
 *
 *	  Module Name: brkh_cus_c.c
 *	 ***************************************************************
 */

/* Chart: ACTIVATION_LOGIC_TOP_AC, Version: New, Mode: New, Created By: sbx1926, Modified: May  8, 2017 14:36 */
/* @Traceability :SysArch_1103*/  
/* Chart: APC_GUIDANCE_SC, Version: Renamed, Mode: New, Created By: jravaux1, Modified: Mar 29, 2017 16:47 */
  
/* Chart: APC_VEHSPD_SC, Version: New, Mode: New, Created By: sbx1938, Modified: May  5, 2017 13:20 */
  
/* Chart: BRKH_CUS_AC, Version: New, Mode: New, Created By: sbx1926, Modified: Mar 13, 2017 11:59 */
/* @Traceability :SysArch_1099*/  
/* Chart: BRKH_CUS_SC, Version: New, Mode: New, Created By: sbx892, Modified: Mar 13, 2017 13:35 */
/* @Traceability :Sys_APC_PTS_Logic_*/  
/* Chart: BRKH_INPUTS_AC, Version: Renamed, Mode: New, Created By: jravaux1, Modified: Mar 13, 2017 11:59 */
  
/* Chart: CTRL_APC_CHECK_SC, Version: New, Mode: New, Created By: jravaux1, Modified: Mar 29, 2017 16:26 */
  
/* Chart: CTRL_PPB_CHECK_SC, Version: New, Mode: New, Created By: jravaux1, Modified: May  8, 2017 14:36 */
  
/* Chart: CTRL_REMDIST, Version: New, Mode: New, Created By: jravaux1, Modified: Mar  9, 2017 17:10 */
  
/* Chart: PM_APC_CTRL, Version: New, Mode: New, Created By: sbx1926, Modified: May  5, 2017 13:30 */
  
/* Chart: RD_CALC_DTC, Version: New, Mode: New, Created By: sbx1938, Modified: Oct 19, 2016 09:30 */
  
/* Chart: RD_CALC_DTH, Version: New, Mode: New, Created By: sbx1938, Modified: Mar  8, 2017 16:38 */
  
/* Chart: RD_CALC_RD, Version: New, Mode: New, Created By: sbx1938, Modified: Oct 19, 2016 09:30 */
  
/* Chart: RD_CHECK_GEAR_CHANGE, Version: New, Mode: New, Created By: sbx1938, Modified: Mar  8, 2017 16:39 */
  
/* Chart: RD_GEAR_OVERRIDE, Version: New, Mode: New, Created By: sbx1938, Modified: Jul 27, 2016 13:46 */
  
/* Chart: RD_NEW_MEAS, Version: New, Mode: New, Created By: sbx1938, Modified: Mar  8, 2017 16:38 */
  
/* Chart: RD_SMOOTHING, Version: New, Mode: New, Created By: sbx1938, Modified: Mar  8, 2017 16:03 */
  
/* Chart: REMAINING_DISTANCE_FC, Version: New, Mode: New, Created By: sbx1938, Modified: Mar  7, 2017 16:05 */
  
/* Chart: REM_DIST_AC, Version: New, Mode: New, Created By: sbx1938, Modified: Mar  9, 2017 17:09 */
  
/* Chart: UZL_CONTROL_SC, Version: New, Mode: New, Created By: sbx1926, Modified: Mar  8, 2017 16:38 */
  

/* Project: LOCAL_ACTL */
/* Workarea: D:\Daten\jravaux1\stm\data\wa461\wa_platform */
/* Profile Name: brkh_cus, Version: New */
/* File Name: D:\Daten\jravaux1\stm\data\wa461\wa_platform\prt\brkh_cus\brkh_cus_c.c */
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


#include "tmp_pdf.h"
#include "brkh_cus_c.h"

#if defined __HC12__ || defined __HC08__
#pragma CODE_SEG DEFAULT
#elif defined(COSMIC12)
#pragma section ()
#endif


/* Code for: glob_dat.c */


#if defined __HC12__ || defined __HC08__
#pragma DATA_SEG DEFAULT
#pragma CONST_SEG DEFAULT
#pragma STRING_SEG DEFAULT
#elif defined(COSMIC12)
#pragma section []
#endif

uint8 cgGlobalFlags_brkh_cus;/* Buffer to hold Activity activation control bit(s) and Task's need-another step bit(s) */
uint8 cgTimeoutsMask_brkh_cus;/* Buffer to identify pending timeouts */
StateInfo_CTRL_REMDISTcnt1 currentState_CTRL_REMDISTcnt1;/* State variable, to manage state configuration of: CTRL_REMDIST */
StateInfo_APC_VEHSPD_SCcnt1 currentState_APC_VEHSPD_SCcnt1;/* State variable, to manage state configuration of: APC_VEHSPD_SC */
StateInfo_CTRL_APC_CHECK_SC currentState_CTRL_APC_CHECK_SC;/* State variable, to manage state configuration of: CTRL_APC_CHECK_SC */
StateInfo_CTRL_PPB_CHECK_SC currentState_CTRL_PPB_CHECK_SC;/* State variable, to manage state configuration of: CTRL_PPB_CHECK_SC */
StateInfo_BRKH_CUS_SCcnt1 currentState_BRKH_CUS_SCcnt1;/* State variable, to manage state configuration of: BRKH_CUS_SC */
uint16 currentTick_brkh_cus;/* Variable to be used when calling the "Get Current Time" API */
StateInfo_APC_GUIDANCE_SC currentState_APC_GUIDANCE_SC;/* State variable, to manage state configuration of: APC_GUIDANCE_SC */
StateInfo_UZL_CONTROL_SC currentState_UZL_CONTROL_SC;/* State variable, to manage state configuration of: UZL_CONTROL_SC */
uint32 tm_999999998_brkh_cus_TIME;/* Variable to hold pending timeout(s) expiration time */
uint32 tm_999999997_brkh_cus_TIME;/* Variable to hold pending timeout(s) expiration time */
uint32 tm_999999992_brkh_cus_TIME;/* Variable to hold pending timeout(s) expiration time */
uint32 tm_999999991_brkh_cus_TIME;/* Variable to hold pending timeout(s) expiration time */
cgDoubleBufferType_BRKH_CUSBRAKESTATEMACHINE cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE;/* Data structure to hold the next value of the double-buffered data associated with the Task: BRKH_CUSBRAKESTATEMACHINE */
cgDoubleBufferType_BRKH_CUSBRAKESTATEMACHINE cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE;/* Data structure to hold the current value of the double-buffered data associated with the Task: BRKH_CUSBRAKESTATEMACHINE */
cgSingleBufferType_BRKH_CUSBRAKESTATEMACHINE cgSingleBuffer_brkh_cusBRKH_CUSBRAKESTATEMACHINE;/* Data structure to hold the value of the (non double-buffered) data associated with the Task: BRKH_CUSBRAKESTATEMACHINE */
#if defined __HC12__ || defined __HC08__
#pragma DATA_SEG DEFAULT
#elif defined(COSMIC12)
#pragma section []
#endif

/* End of code for: glob_dat.c */


/* Code for: glob_func.c */





/* 
 *	Function : BRKH_SetParkStyle 
 * 
 *	Project : LOCAL_ACTL 
 * 
 *	Author(s) : jravaux1 
 *	Creation date : Mar  9, 2017 17:44 
 * 
 */ 
 
 
/* 
 * Global variables that are used by this Function: 
 * None 
 */ 
 
 
EN_ACTL_PARK_STYLE BRKH_SetParkStyle(void) 
{ 
EN_ACTL_PARK_STYLE ret; 
    u8 tmp_id; 
    enum CAPP_maneuver_strategy_E tmp_ms; 
        P2DAL_GetFreezeInfo(&tmp_id, &tmp_ms); 
 
        switch (tmp_ms) 
        { 
        case CAPP_MS_OAF: 
        case CAPP_MS_OEF: 
        case CAPP_MS_OEB: 
            ret = PARK_OUT; 
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
            ret = PARK_IN; 
            break; 
        } 
return ret; 
} /* BRKH_SetParkStyle */ 




/* 
 *	Procedure : OVERFLOW_PROC 
 * 
 *	Project : DAIMLER_BR222_LOGIC 
 * 
 *	Author(s) : sbx976 
 *	Creation date : May  5, 2010 14:13 
 * 
 */ 
 
#ifndef SIMULATION_FLAG 
void BRKH_Overflow(void) 
{ 
#ifndef DAIMLER_BR222_SIM    
    onMS_COUNTER_OVERFLOW(); 
#endif 
} /* OVERFLOW_PROC */ 
#else 
void ACTL_OVERFLOWPROCEDURE(void) 
{ 
} 
#endif 



/* This function is the code-body of Task: MS_COUNTER_OVERFLOW */
/* This function fixes pending timeout values when the counter: MS_COUNTER overflows. */
void onMS_COUNTER_OVERFLOW(void)
{
  if (cgTimeoutsMask_brkh_cus != 0) {
    /* Test for expired pending timeouts */
    genTmEvent_BRKH_CUSBRAKESTATEMACHINE((uint16) -1, &cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE, MS_COUNTER_INDEX);

    tm_999999998_brkh_cus_TIME -= (uint32) 1 + ((uint16) (uint16) -1);
    tm_999999997_brkh_cus_TIME -= (uint32) 1 + ((uint16) (uint16) -1);
    tm_999999992_brkh_cus_TIME -= (uint32) 1 + ((uint16) (uint16) -1);
    tm_999999991_brkh_cus_TIME -= (uint32) 1 + ((uint16) (uint16) -1);
  }
}

#if defined __HC12__ || defined __HC08__
#pragma CODE_SEG DEFAULT
#elif defined(COSMIC12)
#pragma section ()
#endif



/* End of code for: glob_func.c */


/* Code for: brkh_stm.c */


#if defined __HC12__ || defined __HC08__
#pragma CODE_SEG DEFAULT
#elif defined(COSMIC12)
#pragma section ()
#endif

/* This function executes REM_DIST_AC:REMAINING_DISTANCE_FC (Flowchart) */
void
cgDo_REMAINING_DISTANCE_FC(void)
{
  if ((A_RD_CONTINUE_FEATURE)) { /* RD_CHECK_GEAR_CHANGE:FLOW-BOX#8 */  
    A_RD_CONTINUE_FEATURE = LOGICAL_CAST(FALSE); /* RD_CHECK_GEAR_CHANGE:FLOW-BOX#9 */  
    FLOW_BOX_10st1_label: ;
    A_REMAINING_DISTANCE_LAST = C_MAX_REMAINING_DISTANCE;
    A_RD_NUM_CYCLES = 0;
    A_RD_GEAR_OVERRIDE = LOGICAL_CAST(FALSE);
    A_RD_INIT = LOGICAL_CAST(FALSE);
    A_RD_DRIVEN_DIST_SINCE_INIT_DTH = 0;
    A_RD_DRIVEN_DIST_SINCE_INIT_DTC = 0;
    A_RD_NEW_MEAS = LOGICAL_CAST(FALSE);
    A_APC_REMAINING_DISTANCE_STATE = REM_DIST_STAT_NO_STEP;
    A_RD_DTH_JUMP_FLAG = LOGICAL_CAST(FALSE);
    A_RD_DTH_AT_INIT = LI_DAPM_GET_DISTANCE_TO_HINT;
    A_RD_DTC_AT_INIT = LI_DAPM_GET_DISTANCE_TO_COLLISION;
    A_RD_SMOOTHING_RAMP_STARTED = LOGICAL_CAST(FALSE); /* Reinitialize all used values. *//* @Traceability :Sys_APC_PTS_Logic_6471;Sys_APC_PTS_Logic_6495;Sys_APC_PTS_Logic_131*/  
  }
  else  {
    if (( (A_RD_GEAR_LAST != LI_COMH_CAR_GEAR_LEVER_POSITION) || A_RD_INIT || A_RD_NUM_CYCLES > 0)) { /* RD_CHECK_GEAR_CHANGE:FLOW-BOX#3 */  
      if ((A_RD_NUM_CYCLES >= C_RD_MAX_CYCLES || A_RD_INIT)) { /* RD_CHECK_GEAR_CHANGE:FLOW-BOX#2 */  
        goto FLOW_BOX_10st1_label;
      }
      else  {
        A_RD_GEAR_OVERRIDE = LOGICAL_CAST(TRUE);
        A_RD_NUM_CYCLES++; /* RD_CHECK_GEAR_CHANGE:FLOW-BOX#6 */  
      }
    }
  }
  A_RD_GEAR_LAST = LI_COMH_CAR_GEAR_LEVER_POSITION; /* RD_CHECK_GEAR_CHANGE:FLOW-BOX#5 */  
  if (( (A_PARK_STYLE == PARK_OUT && A_RD_DTH_LAST > C_RD_DTH_JUMP_THRESHOLD_PMO 
  && LI_DAPM_GET_DISTANCE_TO_HINT >= C_RD_DTH_JUMP_THRESHOLD_PMO) ||  (A_PARK_STYLE == PARK_IN && A_RD_DTH_LAST > C_RD_DTH_JUMP_THRESHOLD_PMI 
  && LI_DAPM_GET_DISTANCE_TO_HINT >= C_RD_DTH_JUMP_THRESHOLD_PMI))) { /* RD_CALC_DTH:FLOW-BOX#14 *//* @Traceability :Sys_APC_PTS_Logic_6492*/  
    A_RD_DTH = LI_DAPM_GET_DISTANCE_TO_HINT; /* RD_CALC_DTH:FLOW-BOX#15 */  
  }
  else  {
    if (( (A_PARK_STYLE == PARK_OUT && A_RD_DTH_LAST > C_RD_DTH_JUMP_THRESHOLD_PMO 
    && LI_DAPM_GET_DISTANCE_TO_HINT < C_RD_DTH_JUMP_THRESHOLD_PMO) ||  (A_PARK_STYLE == PARK_IN && A_RD_DTH_LAST > C_RD_DTH_JUMP_THRESHOLD_PMI 
    && LI_DAPM_GET_DISTANCE_TO_HINT < C_RD_DTH_JUMP_THRESHOLD_PMI))) { /* RD_CALC_DTH:FLOW-BOX#16 */  
      A_RD_DRIVEN_DIST_SINCE_INIT_DTH = 0;
      A_RD_NEW_MEAS = LOGICAL_CAST(FALSE);
      A_APC_REMAINING_DISTANCE_STATE = REM_DIST_STAT_NO_STEP;
      A_RD_DTH_JUMP_FLAG = LOGICAL_CAST(FALSE);
      A_RD_DTH_AT_INIT = LI_DAPM_GET_DISTANCE_TO_HINT; /* RD_CALC_DTH:FLOW-BOX#17 */  
      if ( (A_PARK_STYLE == PARK_IN)) {
        A_RD_JUMP_DTH = C_RD_DTH_JUMP_THRESHOLD_PMI -  (A_RD_DTH_AT_INIT - A_RD_DRIVEN_DIST_SINCE_INIT_DTH);
      }
      else {
        A_RD_JUMP_DTH = C_RD_DTH_JUMP_THRESHOLD_PMO -  (A_RD_DTH_AT_INIT - A_RD_DRIVEN_DIST_SINCE_INIT_DTH);
      } /* RD_CALC_DTH:FLOW-BOX#18 */  
    }
    else  {
      A_RD_JUMP_DTH = LI_DAPM_GET_DISTANCE_TO_HINT -  (A_RD_DTH_AT_INIT - A_RD_DRIVEN_DIST_SINCE_INIT_DTH); /* RD_CALC_DTH:FLOW-BOX#3 */  
    }
    if ((A_RD_JUMP_DTH <= 0 && !A_RD_DTH_JUMP_FLAG)) { /* RD_CALC_DTH:FLOW-BOX#4 */  
      A_RD_DTH = LI_DAPM_GET_DISTANCE_TO_HINT - A_RD_JUMP_DTH; /* RD_CALC_DTH:FLOW-BOX#5 *//* @Traceability :Sys_APC_PTS_Logic_6488*/  
    }
    else  {
      if ((A_RD_JUMP_DTH <= C_RD_JUMP_THRESHOLD)) { /* RD_CALC_DTH:FLOW-BOX#7 */  
        A_RD_DTH = A_RD_DTH_LAST; /* RD_CALC_DTH:FLOW-BOX#8 *//* @Traceability :Sys_APC_PTS_Logic_6484*/  
      }
      else  {
        if ( ( (A_RD_DTH_LAST + C_RD_RAMP_VALUE) <= LI_DAPM_GET_DISTANCE_TO_HINT)) {
          A_RD_DTH = A_RD_DTH_LAST + C_RD_RAMP_VALUE;
        }
        else {
          A_RD_DTH = LI_DAPM_GET_DISTANCE_TO_HINT;
        } /* RD_CALC_DTH:FLOW-BOX#9 *//* @Traceability :Sys_APC_PTS_Logic_6485*/  
      }
      if ((A_RD_DTH_JUMP_FLAG &&  (LI_DAPM_GET_DISTANCE_TO_HINT <= A_RD_DTH))) { /* RD_CALC_DTH:FLOW-BOX#10 */  
        A_RD_DTH = A_RD_DTH_LAST;
        A_RD_DTH_AT_INIT = A_RD_DTH;
        A_RD_DRIVEN_DIST_SINCE_INIT_DTH = 0;
        A_RD_DTH_JUMP_FLAG_REINIT = LOGICAL_CAST(TRUE); /* RD_CALC_DTH:FLOW-BOX#12 */  
      }
      A_RD_DTH_JUMP_FLAG = LOGICAL_CAST(TRUE);
      if ( (A_RD_DTH_JUMP_FLAG_REINIT)) {
        A_RD_DTH_JUMP_FLAG = LOGICAL_CAST(FALSE);
      }
      A_RD_DTH_JUMP_FLAG_REINIT = LOGICAL_CAST(FALSE); /* RD_CALC_DTH:FLOW-BOX#6 */  
    }
  }
  if ((LI_DAPM_GET_DISTANCE_TO_COLLISION >= C_MAX_REMAINING_DISTANCE)) { /* RD_CALC_DTC:FLOW-BOX#6 */  
    A_RD_DTC = LI_DAPM_GET_DISTANCE_TO_COLLISION; /* RD_CALC_DTC:FLOW-BOX#14 */  
    FLOW_BOX_11st2_label: ;
    A_RD_DTC_AT_INIT = LI_DAPM_GET_DISTANCE_TO_COLLISION;
    A_RD_DRIVEN_DIST_SINCE_INIT_DTC = 0; /* RD_CALC_DTC:FLOW-BOX#11 */  
  }
  else  {
    A_RD_JUMP_DTC = LI_DAPM_GET_DISTANCE_TO_COLLISION -  (A_RD_DTC_AT_INIT - A_RD_DRIVEN_DIST_SINCE_INIT_DTC); /* RD_CALC_DTC:FLOW-BOX#3 */  
    if ((A_RD_JUMP_DTC <= 0)) { /* RD_CALC_DTC:FLOW-BOX#4 */  
      A_RD_DTC = LI_DAPM_GET_DISTANCE_TO_COLLISION; /* RD_CALC_DTC:FLOW-BOX#5 *//* @Traceability :Sys_APC_PTS_Logic_6480*/  
    }
    else  {
      if ((A_RD_JUMP_DTC <= C_RD_JUMP_THRESHOLD)) { /* RD_CALC_DTC:FLOW-BOX#7 */  
        A_RD_DTC = A_RD_DTC_LAST; /* RD_CALC_DTC:FLOW-BOX#8 *//* @Traceability :Sys_APC_PTS_Logic_6490*/  
      }
      else  {
        if ( ( (A_RD_DTC_LAST + C_RD_RAMP_VALUE) < C_SI16_MAX)) {
          A_RD_DTC = A_RD_DTC_LAST + C_RD_RAMP_VALUE;
        }
        else {
          A_RD_DTC = C_SI16_MAX;
        } /* RD_CALC_DTC:FLOW-BOX#9 *//* @Traceability :Sys_APC_PTS_Logic_6489*/  
      }
      if ((LI_DAPM_GET_DISTANCE_TO_COLLISION < A_RD_DTC)) { /* RD_CALC_DTC:FLOW-BOX#10 */  
        goto FLOW_BOX_11st2_label;
      }
    }
  }
  if ((A_RD_DTH <= A_RD_DTC)) { /* RD_CALC_RD:FLOW-BOX#3 *//* @Traceability :Sys_APC_PTS_Logic_6493*/  
    A_REMAINING_DISTANCE = A_RD_DTH; /* RD_CALC_RD:FLOW-BOX#4 */  
  }
  else  {
    A_REMAINING_DISTANCE = A_RD_DTC; /* RD_CALC_RD:FLOW-BOX#5 */  
  }
  if ( ((LI_DAPM_GET_DISTANCE_TO_HINT < 0 || A_RD_NEW_MEAS))) { /* RD_NEW_MEAS:FLOW-BOX#4 */  
    A_RD_NEW_MEAS = LOGICAL_CAST(TRUE);
    A_APC_REMAINING_DISTANCE_STATE = REM_DIST_STAT_NEW_MEASURE; /* RD_NEW_MEAS:FLOW-BOX#7 *//* @Traceability :Sys_APC_PTS_Logic_6471;Sys_APC_PTS_Logic_6495*/  
    if ((LI_DAPM_GET_DISTANCE_TO_COLLISION < A_RD_DTH)) { /* RD_NEW_MEAS:FLOW-BOX#3 */  
      A_REMAINING_DISTANCE = LI_DAPM_GET_DISTANCE_TO_COLLISION; /* RD_NEW_MEAS:FLOW-BOX#5 */  
    }
    else  {
      A_REMAINING_DISTANCE = A_RD_DTH; /* RD_NEW_MEAS:FLOW-BOX#6 */  
    }
  }
  if ((A_REMAINING_DISTANCE_LAST >= A_REMAINING_DISTANCE)) { /* RD_SMOOTHING:FLOW-BOX#3 */  
    A_RD_SMOOTHING_RAMP_STARTED = LOGICAL_CAST(FALSE); /* RD_SMOOTHING:FLOW-BOX#8 */  
  }
  else  {
    if ((!A_RD_SMOOTHING_RAMP_STARTED &&  ( (A_REMAINING_DISTANCE_LAST + C_RD_JUMP_THRESHOLD) > A_REMAINING_DISTANCE))) { /* RD_SMOOTHING:FLOW-BOX#4 *//* @Traceability :Sys_APC_PTS_Logic_*/  
      A_REMAINING_DISTANCE = A_REMAINING_DISTANCE_LAST; /* RD_SMOOTHING:FLOW-BOX#6 *//* @Traceability :Sys_APC_PTS_Logic_6494*/  
    }
    else  {
      A_REMAINING_DISTANCE = A_REMAINING_DISTANCE_LAST + C_RD_RAMP_VALUE;
      A_RD_SMOOTHING_RAMP_STARTED = LOGICAL_CAST(TRUE); /* RD_SMOOTHING:FLOW-BOX#5 *//* @Traceability :Sys_APC_PTS_Logic_6494*/  
    }
  }
  if ((A_RD_GEAR_OVERRIDE)) { /* RD_GEAR_OVERRIDE:FLOW-BOX#3 */  
    A_REMAINING_DISTANCE = A_REMAINING_DISTANCE_LAST; /* RD_GEAR_OVERRIDE:FLOW-BOX#4 */  
  }
  else  {
    A_RD_DTH_LAST = A_RD_DTH;
    A_RD_DTC_LAST = A_RD_DTC;
    A_REMAINING_DISTANCE_LAST = A_REMAINING_DISTANCE; /* RD_GEAR_OVERRIDE:FLOW-BOX#2 */  
  }
}





/* This function executes the Activity: REM_DIST_AC:REMAINING_DISTANCE_FC */
void
cgActivity_REMAINING_DISTANCE_FC(void)
{
  /* REM_DIST_AC:REMAINING_DISTANCE_FC */  
  cgDo_REMAINING_DISTANCE_FC();
}



/* This function executes the Activity: REM_DIST_AC:CALCULATE_DRIVEN_DISTANCE */
void
cgActivity_CALCULATE_DRIVEN_DISTANCE(void)
{
  /* REM_DIST_AC:CALCULATE_DRIVEN_DISTANCE */  
  if ((cgGlobalFlags_brkh_cus & BITAC_CALCULATE_DRIVEN_DISTANCE) != 0) {
    if (STARTED(CALCULATE_DRIVEN_DISTANCE)) {
      A_RD_CAR_POSITION_LAST = LI_ODOM_CAR_POSITION;
      A_RD_DRIVEN_DIST_SINCE_INIT_DTH = 0;
      A_RD_DRIVEN_DIST_SINCE_INIT_DTC = 0;
    }
    A_RD_DRIVEN_DIST_SINCE_INIT_DTH = A_RD_DRIVEN_DIST_SINCE_INIT_DTH +  (LI_ODOM_CAR_POSITION - A_RD_CAR_POSITION_LAST);
    A_RD_DRIVEN_DIST_SINCE_INIT_DTC = A_RD_DRIVEN_DIST_SINCE_INIT_DTC +  (LI_ODOM_CAR_POSITION - A_RD_CAR_POSITION_LAST);
    A_RD_CAR_POSITION_LAST = LI_ODOM_CAR_POSITION;
  }
}



/* This function executes the Activity: REM_DIST_AC:CALCULATE_REM_DIST */
void
cgActivity_CALCULATE_REM_DIST(void)
{
  /* REM_DIST_AC:CALCULATE_REM_DIST *//* @Traceability :Sys_APC_PTS_Logic_6477*/  
  if ((cgGlobalFlags_brkh_cus & BITAC_CALCULATE_REM_DIST) != 0) {
    cgActivity_REMAINING_DISTANCE_FC();
    stop_activity(CALCULATE_REM_DIST);
  }
}



/* This function calculates the states that were entered, and executes */
/* the appropriate enter-state reaction in chart: CTRL_REMDIST */
void
cgEnterActions_CTRL_REMDISTcnt1(StateInfo_CTRL_REMDISTcnt1 staySame_brkh_cusCTRL_REMDISTcnt1, StateInfo_CTRL_REMDISTcnt1 nextState_brkh_cusCTRL_REMDISTcnt1)
{
  if (willBeInState(nextState_brkh_cusCTRL_REMDISTcnt1 , CTRL_REMDISTst2)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_REMDISTcnt1 , CTRL_REMDISTst2)) {
      /* Enter-state reaction of State: CTRL_REMDIST:CTRL_REMDIST */
      if ( (! (is_active(CALCULATE_DRIVEN_DISTANCE)))) {
        start_activity(CALCULATE_DRIVEN_DISTANCE); 
        GENERATE_EVENT(STARTED_CALCULATE_DRIVEN_DISTANCE);
      }
      start_activity(CALCULATE_REM_DIST);
    }
  }
}



/* This function executes CTRL_REMDIST (Statechart) */
/* ------------------------------------------------     */
/* |Chart|From|To|Transition Label|Transition Note| */
/* ------------------------------------------------ */
void
cgDo_CTRL_REMDISTcnt1(void)
{
  StateInfo_CTRL_REMDISTcnt1 nextState_brkh_cusCTRL_REMDISTcnt1 = 0;
  StateInfo_CTRL_REMDISTcnt1 staySame_brkh_cusCTRL_REMDISTcnt1 = 0;
  staySame_brkh_cusCTRL_REMDISTcnt1 = FS_Chart_CTRL_REMDIST;
  if ((
  ((currentState_CTRL_REMDISTcnt1 == 0) || inLeafState(currentState_CTRL_REMDISTcnt1 , DefaultOf_Chart_CTRL_REMDIST, StateInfo_CTRL_REMDISTcnt1)))) {
    /* Transition to State: CTRL_REMDIST:CTRL_REMDIST */
      
    nextState_brkh_cusCTRL_REMDISTcnt1 = FS_CTRL_REMDISTst2/* CTRL_REMDIST:CTRL_REMDIST */  ;
  }
  else if (inLeafState(currentState_CTRL_REMDISTcnt1 , CTRL_REMDISTst2, StateInfo_CTRL_REMDISTcnt1)) {
    staySame_brkh_cusCTRL_REMDISTcnt1 = FS_CTRL_REMDISTst2;
    /* In-state reaction and actions of State: CTRL_REMDIST:CTRL_REMDIST */
    if ( (! (is_active(CALCULATE_DRIVEN_DISTANCE)))) {
      start_activity(CALCULATE_DRIVEN_DISTANCE); 
      GENERATE_EVENT(STARTED_CALCULATE_DRIVEN_DISTANCE);
    }
    start_activity(CALCULATE_REM_DIST);
  }
  else {
  }
  if (nextState_brkh_cusCTRL_REMDISTcnt1 != 0) {
    cgEnterActions_CTRL_REMDISTcnt1(staySame_brkh_cusCTRL_REMDISTcnt1, nextState_brkh_cusCTRL_REMDISTcnt1);
    currentState_CTRL_REMDISTcnt1 = nextState_brkh_cusCTRL_REMDISTcnt1;
  }
}




/* This function executes the Activity: REM_DIST_AC:CTRL_REMDIST */
void
cgActivity_CTRL_REMDISTcnt1(void)
{
  /* REM_DIST_AC:CTRL_REMDIST */  
  cgDo_CTRL_REMDISTcnt1();
}



/* This function executes the Activity: REM_DIST_AC:REM_DIST */
void
cgActivity_REM_DIST(void)
{
  /* REM_DIST_AC:REM_DIST *//* @Traceability :Sys_APC_PTS_Logic_6476*/  
  cgActivity_CTRL_REMDISTcnt1();
  cgActivity_CALCULATE_DRIVEN_DISTANCE();
  cgActivity_CALCULATE_REM_DIST();
}



/* This function calculates the states that were entered, and executes */
/* the appropriate enter-state reaction in chart: APC_GUIDANCE_SC */
void
cgEnterActions_APC_GUIDANCE_SC(StateInfo_APC_GUIDANCE_SC staySame_brkh_cusAPC_GUIDANCE_SC, StateInfo_APC_GUIDANCE_SC nextState_brkh_cusAPC_GUIDANCE_SC)
{
  if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , APC_GEAR_CTRL)) {
    if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , APC_GEAR_CTRL)) {
      /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_GEAR_CTRL */
      if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
        A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_N;
      }
      else {
        if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
          A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
        }
        else {
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
            A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
          }
          else {
            if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_P)) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_P;
            }
          }
        }
      }
      A_APC_PARK_DRIVE_POS_REQUEST = A_APC_AUTOGEAR_REQ;
    }
    if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , REVERSE_GEAR_WAIT)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , REVERSE_GEAR_WAIT)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:REVERSE_GEAR_WAIT */
        if ( (A_START_OF_FIRST_STROKE)) {
          A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
        }
        else {
          A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
        }
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , REVERSE_GEAR_STANDSTILL)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , REVERSE_GEAR_STANDSTILL)) {
        currentTick_brkh_cus = MS_COUNTER;
        INSTALL_TIMEOUT(tm_999999998_brkh_cus,T_APC_AUTOGEAR_DELAY,MS_COUNTER_INDEX);
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , REVERSE_GEAR_END_CHECK)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , REVERSE_GEAR_END_CHECK)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:REVERSE_GEAR_END_CHECK */
        if ( (LI_DAPM_IS_VEHICLE_STANDSTILL &&  (LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_ACTIVE || LI_DAPM_OPERATION_STATE == DAPM_OS_EXPLORATION_MODE_ACTIVE 
          || LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_DRIVE_TO_POS_OK))) {
          A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
        }
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , APC_REQ_R)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , APC_REQ_R)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_REQ_R */
        A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
      }
      if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , R_INIT)) {
        if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , R_INIT)) {
          /* Enter-state reaction of State: APC_GUIDANCE_SC:R_INIT */
          if ( (LI_DAPM_STEERING_IN_STANDSTILL)) {
            A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          }
          else {
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
          }
          currentTick_brkh_cus = MS_COUNTER;
          INSTALL_TIMEOUT(tm_999999998_brkh_cus,T_REMDIST_TOLERANCE,MS_COUNTER_INDEX);
        }
      }
      else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , R_CHECK)) {
        if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , R_CHECK)) {
          /* Enter-state reaction of State: APC_GUIDANCE_SC:R_CHECK */
          if ( (LI_DAPM_STEERING_IN_STANDSTILL)) {
            A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          }
          else {
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
          }
        }
      }
      else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , STATE_45)) {
        if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , STATE_45)) {
          /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_GEAR_CHANGED_TO_R */
          A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
        }
      }
      else {
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , APC_REQ_D)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , APC_REQ_D)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_REQ_D */
        A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
      }
      if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , D_INIT)) {
        if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , D_INIT)) {
          /* Enter-state reaction of State: APC_GUIDANCE_SC:D_INIT */
          if ( (LI_DAPM_STEERING_IN_STANDSTILL)) {
            A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          }
          else {
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
          }
          currentTick_brkh_cus = MS_COUNTER;
          INSTALL_TIMEOUT(tm_999999998_brkh_cus,T_REMDIST_TOLERANCE,MS_COUNTER_INDEX);
        }
      }
      else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , D_CHECK)) {
        if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , D_CHECK)) {
          /* Enter-state reaction of State: APC_GUIDANCE_SC:D_CHECK */
          if ( (LI_DAPM_STEERING_IN_STANDSTILL)) {
            A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          }
          else {
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
          }
        }
      }
      else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , STATE_43)) {
        if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , STATE_43)) {
          /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_GEAR_CHANGED_TO_D */
          A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
        }
      }
      else {
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , APC_MANUAL_GEAR_RQ)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , APC_MANUAL_GEAR_RQ)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_MANUAL_GEAR_RQ */
        A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
        if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
          A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_N;
        }
        else {
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
            A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
          }
          else {
            if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
            }
            else {
              if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_P)) {
                A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_P;
              }
            }
          }
        }
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , APC_IDLE_GEAR)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , APC_IDLE_GEAR)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_IDLE_GEAR */
        if ( (A_START_OF_FIRST_STROKE)) {
          A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
        }
        else {
          A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
        }
        if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
          A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_N;
        }
        else {
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
            A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
          }
          else {
            if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
            }
            else {
              if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_P)) {
                A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_P;
              }
            }
          }
        }
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , APC_GEAR_AUTOMATIC)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , APC_GEAR_AUTOMATIC)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_GEAR_AUTOMATIC */
        if ( (A_START_OF_FIRST_STROKE)) {
          A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
        }
        else {
          A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
        }
        if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
          A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_N;
        }
        else {
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
            A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
          }
          else {
            if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
            }
            else {
              if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_P)) {
                A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_P;
              }
            }
          }
        }
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , DRIVE_GEAR_WAIT)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , DRIVE_GEAR_WAIT)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:DRIVE_GEAR_WAIT */
        if ( (A_START_OF_FIRST_STROKE)) {
          A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
        }
        else {
          A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
        }
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , DRIVE_GEAR_STANDSTILL)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , DRIVE_GEAR_STANDSTILL)) {
        currentTick_brkh_cus = MS_COUNTER;
        INSTALL_TIMEOUT(tm_999999998_brkh_cus,T_APC_AUTOGEAR_DELAY,MS_COUNTER_INDEX);
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , DRIVE_GEAR_END_CHECK)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , DRIVE_GEAR_END_CHECK)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:DRIVE_GEAR_END_CHECK */
        if ( (LI_DAPM_IS_VEHICLE_STANDSTILL &&  (LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_ACTIVE || LI_DAPM_OPERATION_STATE == DAPM_OS_EXPLORATION_MODE_ACTIVE 
          || LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_DRIVE_TO_POS_OK))) {
          A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
        }
      }
    }
    else if (willBeInState(nextState_brkh_cusAPC_GUIDANCE_SC , APC_AUTOGEAR_DELAY)) {
      if (isNotStayingInState(staySame_brkh_cusAPC_GUIDANCE_SC , APC_AUTOGEAR_DELAY)) {
        /* Enter-state reaction of State: APC_GUIDANCE_SC:APC_AUTOGEAR_DELAY */
        A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
        currentTick_brkh_cus = MS_COUNTER;
        INSTALL_TIMEOUT(tm_999999998_brkh_cus,T_APC_MANGEAR_DELAY,MS_COUNTER_INDEX);
      }
    }
    else {
    }
  }
}



/* This function executes APC_GUIDANCE_SC (Statechart) */
/* ------------------------------------------------------------------------------------------------------------------------------------     */
/* |     Chart     |         From          |          To           |                 Transition Label                 |Transition Note| */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_GEAR_AUTOMATIC     |APC_IDLE_GEAR          |[LI_DAPM_DRIVING_HINT==CAPP_DD_UNKNOWN and LI_DAPM|               | */
/* |               |                       |                       |_IS_VEHICLE_STANDSTILL==true]                     |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|R_CHECK                |APC_GEAR_CHANGED_TO_R  |[LI_DAPM_STEERING_IN_STANDSTILL==false]           |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_IDLE_GEAR          |APC_REVERSE_GEAR       |[A_PARK_STYLE==PARK_IN and ((LI_COMH_CAR_GEAR_LEVE|               | */
/* |               |                       |                       |R_POSITION==GEAR_POSITION_D and LI_DAPM_DRIVING_HI|               | */
/* |               |                       |                       |NT==CAPP_DD_BACKWARD and LI_DAPM_GET_DISTANCE_TO_H|               | */
/* |               |                       |                       |INT==C_NULL_REMAINING_DISTANCE) or ((LI_DAPM_DRIVI|               | */
/* |               |                       |                       |NG_HINT==CAPP_DD_BACKWARD and LI_DAPM_GET_DISTANCE|               | */
/* |               |                       |                       |_TO_HINT>C_MIN_REMAINING_DISTANCE) or (LI_DAPM_DRI|               | */
/* |               |                       |                       |VING_HINT==CAPP_DD_FORWARD and LI_DAPM_GET_DISTANC|               | */
/* |               |                       |                       |E_TO_HINT<=C_MIN_REMAINING_DISTANCE)))]           |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_REVERSE_GEAR       |APC_IDLE_GEAR          |[LI_DAPM_DRIVING_HINT==CAPP_DD_UNKNOWN and LI_DAPM|               | */
/* |               |                       |                       |_IS_VEHICLE_STANDSTILL==true]/A_START_OF_FIRST_STR|               | */
/* |               |                       |                       |OKE=false;                                        |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_IDLE_GEAR          |APC_DRIVE_GEAR         |[A_PARK_STYLE==PARK_IN and ((LI_COMH_CAR_GEAR_LEVE|               | */
/* |               |                       |                       |R_POSITION==GEAR_POSITION_R and LI_DAPM_DRIVING_HI|               | */
/* |               |                       |                       |NT==CAPP_DD_FORWARD and LI_DAPM_GET_DISTANCE_TO_HI|               | */
/* |               |                       |                       |NT==C_NULL_REMAINING_DISTANCE) or ((LI_DAPM_DRIVIN|               | */
/* |               |                       |                       |G_HINT==CAPP_DD_FORWARD and LI_DAPM_GET_DISTANCE_T|               | */
/* |               |                       |                       |O_HINT>C_MIN_REMAINING_DISTANCE) or (LI_DAPM_DRIVI|               | */
/* |               |                       |                       |NG_HINT==CAPP_DD_BACKWARD and LI_DAPM_GET_DISTANCE|               | */
/* |               |                       |                       |_TO_HINT<=C_MIN_REMAINING_DISTANCE)))]            |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_DRIVE_GEAR         |APC_IDLE_GEAR          |[LI_DAPM_DRIVING_HINT==CAPP_DD_UNKNOWN and LI_DAPM|               | */
/* |               |                       |                       |_IS_VEHICLE_STANDSTILL==true]/A_START_OF_FIRST_STR|               | */
/* |               |                       |                       |OKE=false;                                        |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_DRIVE_GEAR         |D_INIT                 |[LI_COMH_CAR_GEAR_LEVER_POSITION==GEAR_POSITION_D]|               | */
/* |               |                       |                       |/A_START_OF_FIRST_STROKE=false;                   |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_GEAR_CHANGED_TO_R  |APC_DRIVE_GEAR         |[(LI_DAPM_OPERATION_STATE==DAPM_OS_PARKING_ACTIVE |               | */
/* |               |                       |                       |or LI_DAPM_OPERATION_STATE==DAPM_OS_EXPLORATION_MO|               | */
/* |               |                       |                       |DE_ACTIVE or LI_DAPM_OPERATION_STATE==DAPM_OS_PARK|               | */
/* |               |                       |                       |ING_DRIVE_TO_POS_OK) and ((LI_DAPM_DRIVING_HINT==C|               | */
/* |               |                       |                       |APP_DD_FORWARD) or (LI_DAPM_DRIVING_HINT==CAPP_DD_|               | */
/* |               |                       |                       |BACKWARD and A_REMAINING_DISTANCE<=C_MIN_REMAINING|               | */
/* |               |                       |                       |_DISTANCE))] and (not (ch(LI_COMH_CAR_GEAR_LEVER_P|               | */
/* |               |                       |                       |OSITION) and [LI_COMH_CAR_GEAR_LEVER_POSITION!=GEA|               | */
/* |               |                       |                       |R_POSITION_R]))                                   |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_REVERSE_GEAR       |R_INIT                 |[LI_COMH_CAR_GEAR_LEVER_POSITION==GEAR_POSITION_R]|               | */
/* |               |                       |                       |/A_START_OF_FIRST_STROKE=false;                   |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|D_CHECK                |APC_GEAR_CHANGED_TO_D  |[LI_DAPM_STEERING_IN_STANDSTILL==false]           |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_GEAR_CHANGED_TO_D  |APC_REVERSE_GEAR       |[(LI_DAPM_OPERATION_STATE==DAPM_OS_PARKING_ACTIVE |               | */
/* |               |                       |                       |or LI_DAPM_OPERATION_STATE==DAPM_OS_EXPLORATION_MO|               | */
/* |               |                       |                       |DE_ACTIVE or LI_DAPM_OPERATION_STATE==DAPM_OS_PARK|               | */
/* |               |                       |                       |ING_DRIVE_TO_POS_OK) and ((LI_DAPM_DRIVING_HINT==C|               | */
/* |               |                       |                       |APP_DD_BACKWARD) or (LI_DAPM_DRIVING_HINT==CAPP_DD|               | */
/* |               |                       |                       |_FORWARD and A_REMAINING_DISTANCE<=C_MIN_REMAINING|               | */
/* |               |                       |                       |_DISTANCE))] and (not (ch(LI_COMH_CAR_GEAR_LEVER_P|               | */
/* |               |                       |                       |OSITION) and [LI_COMH_CAR_GEAR_LEVER_POSITION!=GEA|               | */
/* |               |                       |                       |R_POSITION_D]))                                   |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_REQ_R              |APC_MANUAL_GEAR_RQ     |ch(LI_COMH_CAR_GEAR_LEVER_POSITION)[LI_COMH_CAR_GE|               | */
/* |               |                       |                       |AR_LEVER_POSITION!=GEAR_POSITION_R]               |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_MANUAL_GEAR_RQ     |APC_AUTOGEAR_DELAY     |[LI_DAPM_STEERING_IN_STANDSTILL==false and ((LI_CO|               | */
/* |               |                       |                       |MH_CAR_GEAR_LEVER_POSITION==GEAR_POSITION_R) or (L|               | */
/* |               |                       |                       |I_COMH_CAR_GEAR_LEVER_POSITION==GEAR_POSITION_D))]|               | */
/* |               |                       |                       |                                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_REQ_D              |APC_MANUAL_GEAR_RQ     |ch(LI_COMH_CAR_GEAR_LEVER_POSITION)[LI_COMH_CAR_GE|               | */
/* |               |                       |                       |AR_LEVER_POSITION!=GEAR_POSITION_D]               |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|DRIVE_GEAR_WAIT        |DRIVE_GEAR_STANDSTILL  |[LI_DAPM_IS_VEHICLE_STANDSTILL]                   |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|REVERSE_GEAR_WAIT      |REVERSE_GEAR_STANDSTILL|[LI_DAPM_IS_VEHICLE_STANDSTILL]                   |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_DRIVE_GEAR         |APC_MANUAL_GEAR_RQ     |ch(LI_COMH_CAR_GEAR_LEVER_POSITION)[LI_COMH_CAR_GE|               | */
/* |               |                       |                       |AR_LEVER_POSITION==GEAR_POSITION_N]/A_START_OF_FIR|               | */
/* |               |                       |                       |ST_STROKE=false;                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_REVERSE_GEAR       |APC_MANUAL_GEAR_RQ     |ch(LI_COMH_CAR_GEAR_LEVER_POSITION)[LI_COMH_CAR_GE|               | */
/* |               |                       |                       |AR_LEVER_POSITION==GEAR_POSITION_N]/A_START_OF_FIR|               | */
/* |               |                       |                       |ST_STROKE=false;                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_GEAR_AUTOMATIC     |APC_DRIVE_GEAR         |[(A_START_OF_FIRST_STROKE and LI_COMH_CAR_GEAR_LEV|               | */
/* |               |                       |                       |ER_POSITION==GEAR_POSITION_N and LI_DAPM_DRIVING_H|               | */
/* |               |                       |                       |INT==CAPP_DD_FORWARD) or (((LI_COMH_CAR_GEAR_LEVER|               | */
/* |               |                       |                       |_POSITION==GEAR_POSITION_R) and LI_DAPM_DRIVING_HI|               | */
/* |               |                       |                       |NT==CAPP_DD_FORWARD and LI_DAPM_GET_DISTANCE_TO_HI|               | */
/* |               |                       |                       |NT==C_NULL_REMAINING_DISTANCE) or ((LI_DAPM_DRIVIN|               | */
/* |               |                       |                       |G_HINT==CAPP_DD_FORWARD and LI_DAPM_GET_DISTANCE_T|               | */
/* |               |                       |                       |O_HINT>C_MIN_REMAINING_DISTANCE) or (LI_DAPM_DRIVI|               | */
/* |               |                       |                       |NG_HINT==CAPP_DD_BACKWARD and LI_DAPM_GET_DISTANCE|               | */
/* |               |                       |                       |_TO_HINT<=C_MIN_REMAINING_DISTANCE)))]            |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_GEAR_AUTOMATIC     |APC_REVERSE_GEAR       |[(A_START_OF_FIRST_STROKE and LI_COMH_CAR_GEAR_LEV|               | */
/* |               |                       |                       |ER_POSITION==GEAR_POSITION_N and LI_DAPM_DRIVING_H|               | */
/* |               |                       |                       |INT==CAPP_DD_BACKWARD) or (((LI_COMH_CAR_GEAR_LEVE|               | */
/* |               |                       |                       |R_POSITION==GEAR_POSITION_D) and LI_DAPM_DRIVING_H|               | */
/* |               |                       |                       |INT==CAPP_DD_BACKWARD and LI_DAPM_GET_DISTANCE_TO_|               | */
/* |               |                       |                       |HINT==C_NULL_REMAINING_DISTANCE) or ((LI_DAPM_DRIV|               | */
/* |               |                       |                       |ING_HINT==CAPP_DD_BACKWARD and LI_DAPM_GET_DISTANC|               | */
/* |               |                       |                       |E_TO_HINT>C_MIN_REMAINING_DISTANCE) or (LI_DAPM_DR|               | */
/* |               |                       |                       |IVING_HINT==CAPP_DD_FORWARD and LI_DAPM_GET_DISTAN|               | */
/* |               |                       |                       |CE_TO_HINT<=C_MIN_REMAINING_DISTANCE)))]          |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_IDLE_GEAR          |APC_DRIVE_GEAR         |[A_PARK_STYLE==PARK_OUT and ((LI_COMH_CAR_GEAR_LEV|               | */
/* |               |                       |                       |ER_POSITION==GEAR_POSITION_R and (LI_DAPM_DRIVING_|               | */
/* |               |                       |                       |HINT==CAPP_DD_FORWARD or LI_DAPM_GET_DISTANCE_TO_H|               | */
/* |               |                       |                       |INT<=C_MIN_REMAINING_DISTANCE)) or (LI_COMH_CAR_GE|               | */
/* |               |                       |                       |AR_LEVER_POSITION==GEAR_POSITION_D and LI_DAPM_GET|               | */
/* |               |                       |                       |_DISTANCE_TO_HINT>=C_MIN_REMAINING_DISTANCE))]    |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_IDLE_GEAR          |APC_REVERSE_GEAR       |[A_PARK_STYLE==PARK_OUT and ((LI_COMH_CAR_GEAR_LEV|               | */
/* |               |                       |                       |ER_POSITION==GEAR_POSITION_D and (LI_DAPM_DRIVING_|               | */
/* |               |                       |                       |HINT==CAPP_DD_BACKWARD or LI_DAPM_GET_DISTANCE_TO_|               | */
/* |               |                       |                       |HINT<=C_NULL_REMAINING_DISTANCE)) or ((LI_COMH_CAR|               | */
/* |               |                       |                       |_GEAR_LEVER_POSITION==GEAR_POSITION_R and LI_DAPM_|               | */
/* |               |                       |                       |GET_DISTANCE_TO_HINT>=C_MIN_REMAINING_DISTANCE)))]|               | */
/* |               |                       |                       |                                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|DRIVE_GEAR_STANDSTILL  |DRIVE_GEAR_END_CHECK   |dly(T_APC_AUTOGEAR_DELAY)                         |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|                       |DRIVE_GEAR_WAIT        |                                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|REVERSE_GEAR_STANDSTILL|REVERSE_GEAR_END_CHECK |dly(T_APC_AUTOGEAR_DELAY)                         |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|                       |REVERSE_GEAR_WAIT      |                                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|DRIVE_GEAR_STANDSTILL  |APC_GEAR_CHANGED_TO_R  |[LI_DAPM_IS_VEHICLE_STANDSTILL and (LI_DAPM_DRIVIN|               | */
/* |               |                       |                       |G_HINT==CAPP_DD_BACKWARD and (LI_DAPM_GET_DISTANCE|               | */
/* |               |                       |                       |_TO_HINT>C_MIN_REMAINING_DISTANCE and LI_DAPM_GET_|               | */
/* |               |                       |                       |DISTANCE_TO_COLLISION>C_MIN_REMAINING_DISTANCE))]/|               | */
/* |               |                       |                       |A_RD_CONTINUE_FEATURE=true;A_START_OF_FIRST_STROKE|               | */
/* |               |                       |                       |=false;                                           |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|REVERSE_GEAR_STANDSTILL|APC_GEAR_CHANGED_TO_D  |[LI_DAPM_IS_VEHICLE_STANDSTILL and (LI_DAPM_DRIVIN|               | */
/* |               |                       |                       |G_HINT==CAPP_DD_FORWARD and (LI_DAPM_GET_DISTANCE_|               | */
/* |               |                       |                       |TO_HINT>C_MIN_REMAINING_DISTANCE and LI_DAPM_GET_D|               | */
/* |               |                       |                       |ISTANCE_TO_COLLISION>C_MIN_REMAINING_DISTANCE))]/A|               | */
/* |               |                       |                       |_RD_CONTINUE_FEATURE=true;A_START_OF_FIRST_STROKE=|               | */
/* |               |                       |                       |false;                                            |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|R_INIT                 |R_CHECK                |dly(T_REMDIST_TOLERANCE)                          |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|D_INIT                 |D_CHECK                |dly(T_REMDIST_TOLERANCE)                          |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|R_DRIVE                |R_STANDSTILL           |[LI_DAPM_IS_VEHICLE_STANDSTILL==true]             |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|R_STANDSTILL           |R_DRIVE                |[LI_DAPM_IS_VEHICLE_STANDSTILL==false]            |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|R_STANDSTILL           |R_CONTINUE             |[LI_DAPM_IS_VEHICLE_STANDSTILL==true and A_RD_NEW_|               | */
/* |               |                       |                       |MEAS==true and LI_DAPM_GET_DISTANCE_TO_HINT>C_MIN_|               | */
/* |               |                       |                       |REMAINING_DISTANCE and LI_DAPM_GET_DISTANCE_TO_COL|               | */
/* |               |                       |                       |LISION>C_MIN_REMAINING_DISTANCE]/A_RD_CONTINUE_FEA|               | */
/* |               |                       |                       |TURE=true;                                        |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|R_CONTINUE             |R_DRIVE                |[LI_DAPM_IS_VEHICLE_STANDSTILL==false]            |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|                       |R_DRIVE                |                                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|D_DRIVE                |D_STANDSTILL           |[LI_DAPM_IS_VEHICLE_STANDSTILL==true]             |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|D_STANDSTILL           |D_DRIVE                |[LI_DAPM_IS_VEHICLE_STANDSTILL==false]            |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|D_STANDSTILL           |D_CONTINUE             |[LI_DAPM_IS_VEHICLE_STANDSTILL==true and A_RD_NEW_|               | */
/* |               |                       |                       |MEAS==true and LI_DAPM_GET_DISTANCE_TO_HINT>C_MIN_|               | */
/* |               |                       |                       |REMAINING_DISTANCE and LI_DAPM_GET_DISTANCE_TO_COL|               | */
/* |               |                       |                       |LISION>C_MIN_REMAINING_DISTANCE]/A_RD_CONTINUE_FEA|               | */
/* |               |                       |                       |TURE=true;                                        |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|D_CONTINUE             |D_DRIVE                |[LI_DAPM_IS_VEHICLE_STANDSTILL==false]            |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|                       |D_DRIVE                |                                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|                       |APC_GEAR_AUTOMATIC     |not (([A_PARK_STYLE==PARK_OUT and LI_COMH_CAR_GEAR|               | */
/* |               |                       |                       |_LEVER_POSITION==GEAR_POSITION_R and LI_DAPM_DRIVI|               | */
/* |               |                       |                       |NG_HINT==CAPP_DD_FORWARD]) or ([A_PARK_STYLE==PARK|               | */
/* |               |                       |                       |_OUT and LI_COMH_CAR_GEAR_LEVER_POSITION==GEAR_POS|               | */
/* |               |                       |                       |ITION_D and LI_DAPM_DRIVING_HINT==CAPP_DD_BACKWARD|               | */
/* |               |                       |                       |]))                                               |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|                       |APC_DRIVE_GEAR         |[A_PARK_STYLE==PARK_OUT and LI_COMH_CAR_GEAR_LEVER|               | */
/* |               |                       |                       |_POSITION==GEAR_POSITION_R and LI_DAPM_DRIVING_HIN|               | */
/* |               |                       |                       |T==CAPP_DD_FORWARD]                               |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|                       |APC_REVERSE_GEAR       |[A_PARK_STYLE==PARK_OUT and LI_COMH_CAR_GEAR_LEVER|               | */
/* |               |                       |                       |_POSITION==GEAR_POSITION_D and LI_DAPM_DRIVING_HIN|               | */
/* |               |                       |                       |T==CAPP_DD_BACKWARD]                              |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
/* |APC_GUIDANCE_SC|APC_AUTOGEAR_DELAY     |APC_GEAR_AUTOMATIC     |dly(T_APC_MANGEAR_DELAY)                          |               | */
/* ------------------------------------------------------------------------------------------------------------------------------------ */
void
cgDo_APC_GUIDANCE_SC(void)
{
  StateInfo_APC_GUIDANCE_SC nextState_brkh_cusAPC_GUIDANCE_SC = 0;
  StateInfo_APC_GUIDANCE_SC staySame_brkh_cusAPC_GUIDANCE_SC = 0;
  if (currentState_APC_GUIDANCE_SC == 0) {
    nextState_brkh_cusAPC_GUIDANCE_SC = FS_DefaultOf_APC_GEAR_CTRL/* Default Of: APC_GUIDANCE_SC:APC_GEAR_CTRL */  ;
  }

  else {
    if (inState(currentState_APC_GUIDANCE_SC , APC_GEAR_CTRL)) {
      staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_GEAR_CTRL;
      /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_GEAR_CTRL */
      A_APC_PARK_DRIVE_POS_REQUEST = A_APC_AUTOGEAR_REQ;
      if ((
      inLeafState(currentState_APC_GUIDANCE_SC , DefaultOf_APC_GEAR_CTRL, StateInfo_APC_GUIDANCE_SC)
       && ((A_PARK_STYLE == PARK_OUT && LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R 
      && LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD)))) {
        /* Transition to State: APC_GUIDANCE_SC:DRIVE_GEAR_WAIT */
          
        nextState_brkh_cusAPC_GUIDANCE_SC = FS_DRIVE_GEAR_WAIT/* APC_GUIDANCE_SC:DRIVE_GEAR_WAIT */  ;
      }
      else if ((
      inLeafState(currentState_APC_GUIDANCE_SC , DefaultOf_APC_GEAR_CTRL, StateInfo_APC_GUIDANCE_SC)
       && (!  ( ((A_PARK_STYLE == PARK_OUT && LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R 
      && LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD)) ||  ((A_PARK_STYLE == PARK_OUT && LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D 
      && LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD)))))) {
        /* Transition to State: APC_GUIDANCE_SC:APC_GEAR_AUTOMATIC */
          
        nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_GEAR_AUTOMATIC/* APC_GUIDANCE_SC:APC_GEAR_AUTOMATIC */  ;
      }
      else if ((
      inLeafState(currentState_APC_GUIDANCE_SC , DefaultOf_APC_GEAR_CTRL, StateInfo_APC_GUIDANCE_SC)
       && ((A_PARK_STYLE == PARK_OUT && LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D 
      && LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD)))) {
        /* Transition to State: APC_GUIDANCE_SC:REVERSE_GEAR_WAIT */
          
        nextState_brkh_cusAPC_GUIDANCE_SC = FS_REVERSE_GEAR_WAIT/* APC_GUIDANCE_SC:REVERSE_GEAR_WAIT */  ;
      }
      else if (inState(currentState_APC_GUIDANCE_SC , STATE_41)) {
        staySame_brkh_cusAPC_GUIDANCE_SC = FS_STATE_41;
        if ((inState(currentState_APC_GUIDANCE_SC , APC_DRIVE_GEAR)
         && ((LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)))) {
          /* Transition to State: APC_GUIDANCE_SC:D_INIT */
           /*@Transition Priority:2*/ 
          A_START_OF_FIRST_STROKE = LOGICAL_CAST(FALSE);
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_D_INIT/* Give P2 time to update their inputs. *//* @Traceability :Sys_APC_PTS_Logic_*/  ;
        }
        else if ((
        inState(currentState_APC_GUIDANCE_SC , APC_DRIVE_GEAR)
         && ((CHANGED(LI_COMH_CAR_GEAR_LEVER_POSITION)) &&(LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)))) {
          /* Transition to State: APC_GUIDANCE_SC:APC_MANUAL_GEAR_RQ */
           /*@Transition Priority:3*/ 
          A_START_OF_FIRST_STROKE = LOGICAL_CAST(FALSE);
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_MANUAL_GEAR_RQ/* APC_GUIDANCE_SC:APC_MANUAL_GEAR_RQ *//* @Traceability :Sys_APC_PTS_Logic_4417;Sys_APC_PTS_Logic_5339;Sys_APC_PTS_Logic_5646;Sys_APC_PTS_Logic_6197;*/  ;
        }
        else if ((
        inState(currentState_APC_GUIDANCE_SC , APC_DRIVE_GEAR)
         && ((LI_DAPM_DRIVING_HINT == CAPP_DD_UNKNOWN && LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
          /* Transition to State: APC_GUIDANCE_SC:APC_IDLE_GEAR */
           /*@Transition Priority:4*/ 
          A_START_OF_FIRST_STROKE = LOGICAL_CAST(FALSE);
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_IDLE_GEAR/* APC_GUIDANCE_SC:APC_IDLE_GEAR */  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , APC_GEAR_AUTOMATIC, StateInfo_APC_GUIDANCE_SC)
         && (( (A_START_OF_FIRST_STROKE && LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N 
        && LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD) ||  ( ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D) && LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD 
        && LI_DAPM_GET_DISTANCE_TO_HINT == C_NULL_REMAINING_DISTANCE) 
        ||  ( (LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD && LI_DAPM_GET_DISTANCE_TO_HINT > C_MIN_REMAINING_DISTANCE) 
        ||  (LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD && LI_DAPM_GET_DISTANCE_TO_HINT <= C_MIN_REMAINING_DISTANCE))))))) {
          /* Transition to State: APC_GUIDANCE_SC:REVERSE_GEAR_WAIT */
           /*@Transition Priority:1*/ 
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_REVERSE_GEAR_WAIT/* APC_GUIDANCE_SC:REVERSE_GEAR_WAIT */  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , APC_GEAR_AUTOMATIC, StateInfo_APC_GUIDANCE_SC)
         && (( (A_START_OF_FIRST_STROKE && LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N 
        && LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD) ||  ( ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R) && LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD 
        && LI_DAPM_GET_DISTANCE_TO_HINT == C_NULL_REMAINING_DISTANCE) 
        ||  ( (LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD && LI_DAPM_GET_DISTANCE_TO_HINT > C_MIN_REMAINING_DISTANCE) 
        ||  (LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD && LI_DAPM_GET_DISTANCE_TO_HINT <= C_MIN_REMAINING_DISTANCE))))))) {
          /* Transition to State: APC_GUIDANCE_SC:DRIVE_GEAR_WAIT */
           /*@Transition Priority:2*/ 
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_DRIVE_GEAR_WAIT/* APC_GUIDANCE_SC:DRIVE_GEAR_WAIT */  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , APC_GEAR_AUTOMATIC, StateInfo_APC_GUIDANCE_SC)
         && ((LI_DAPM_DRIVING_HINT == CAPP_DD_UNKNOWN && LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
          /* Transition to State: APC_GUIDANCE_SC:APC_IDLE_GEAR */
           /*@Transition Priority:3*/ 
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_IDLE_GEAR/* APC_GUIDANCE_SC:APC_IDLE_GEAR */  ;
        }
        else if ((
        inState(currentState_APC_GUIDANCE_SC , APC_REVERSE_GEAR)
         && ((LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)))) {
          /* Transition to State: APC_GUIDANCE_SC:R_INIT */
           /*@Transition Priority:2*/ 
          A_START_OF_FIRST_STROKE = LOGICAL_CAST(FALSE);
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_R_INIT/* Give P2 time to update their inputs. *//* @Traceability :Sys_APC_PTS_Logic_*/  ;
        }
        else if ((
        inState(currentState_APC_GUIDANCE_SC , APC_REVERSE_GEAR)
         && ((CHANGED(LI_COMH_CAR_GEAR_LEVER_POSITION)) &&(LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)))) {
          /* Transition to State: APC_GUIDANCE_SC:APC_MANUAL_GEAR_RQ */
           /*@Transition Priority:3*/ 
          A_START_OF_FIRST_STROKE = LOGICAL_CAST(FALSE);
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_MANUAL_GEAR_RQ/* APC_GUIDANCE_SC:APC_MANUAL_GEAR_RQ *//* @Traceability :Sys_APC_PTS_Logic_4417;Sys_APC_PTS_Logic_5339;Sys_APC_PTS_Logic_5646;Sys_APC_PTS_Logic_6197;*/  ;
        }
        else if ((
        inState(currentState_APC_GUIDANCE_SC , APC_REVERSE_GEAR)
         && ((LI_DAPM_DRIVING_HINT == CAPP_DD_UNKNOWN && LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
          /* Transition to State: APC_GUIDANCE_SC:APC_IDLE_GEAR */
           /*@Transition Priority:4*/ 
          A_START_OF_FIRST_STROKE = LOGICAL_CAST(FALSE);
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_IDLE_GEAR/* APC_GUIDANCE_SC:APC_IDLE_GEAR */  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , DRIVE_GEAR_STANDSTILL, StateInfo_APC_GUIDANCE_SC)
         && ((LI_DAPM_IS_VEHICLE_STANDSTILL &&  (LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD &&  (LI_DAPM_GET_DISTANCE_TO_HINT > C_MIN_REMAINING_DISTANCE && LI_DAPM_GET_DISTANCE_TO_COLLISION > C_MIN_REMAINING_DISTANCE)))))) {
          /* Transition to State: APC_GUIDANCE_SC:R_DRIVE */
           /*@Transition Priority:1*/ 
          A_RD_CONTINUE_FEATURE = LOGICAL_CAST(TRUE);
          A_START_OF_FIRST_STROKE = LOGICAL_CAST(FALSE);
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_R_DRIVE/* APC_GUIDANCE_SC:R_DRIVE */  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , DRIVE_GEAR_STANDSTILL, StateInfo_APC_GUIDANCE_SC)
         && (tm_999999998_brkh_cus))) {
          /* Transition to State: APC_GUIDANCE_SC:DRIVE_GEAR_END_CHECK */
           /*@Transition Priority:2*/ 
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_DRIVE_GEAR_END_CHECK/* APC_GUIDANCE_SC:DRIVE_GEAR_END_CHECK */  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , REVERSE_GEAR_STANDSTILL, StateInfo_APC_GUIDANCE_SC)
         && ((LI_DAPM_IS_VEHICLE_STANDSTILL &&  (LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD &&  (LI_DAPM_GET_DISTANCE_TO_HINT > C_MIN_REMAINING_DISTANCE && LI_DAPM_GET_DISTANCE_TO_COLLISION > C_MIN_REMAINING_DISTANCE)))))) {
          /* Transition to State: APC_GUIDANCE_SC:D_DRIVE */
           /*@Transition Priority:1*/ 
          A_RD_CONTINUE_FEATURE = LOGICAL_CAST(TRUE);
          A_START_OF_FIRST_STROKE = LOGICAL_CAST(FALSE);
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_D_DRIVE/* APC_GUIDANCE_SC:D_DRIVE */  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , APC_AUTOGEAR_DELAY, StateInfo_APC_GUIDANCE_SC)
         && (tm_999999998_brkh_cus))) {
          /* Transition to State: APC_GUIDANCE_SC:APC_GEAR_AUTOMATIC */
            
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_GEAR_AUTOMATIC/* APC_GUIDANCE_SC:APC_GEAR_AUTOMATIC */  ;
        }
        else if ((inState(currentState_APC_GUIDANCE_SC , STATE_43)
         && ( (( (LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_ACTIVE || LI_DAPM_OPERATION_STATE == DAPM_OS_EXPLORATION_MODE_ACTIVE 
        || LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_DRIVE_TO_POS_OK) 
        &&  ( (LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD) ||  (LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD && A_REMAINING_DISTANCE <= C_MIN_REMAINING_DISTANCE)))) 
        &&  ( (!  ( (CHANGED(LI_COMH_CAR_GEAR_LEVER_POSITION)) &&  ((LI_COMH_CAR_GEAR_LEVER_POSITION != GEAR_POSITION_D))))))) || 
        (
        inLeafState(currentState_APC_GUIDANCE_SC , APC_IDLE_GEAR, StateInfo_APC_GUIDANCE_SC)
         && ((A_PARK_STYLE == PARK_IN &&  ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D && LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD 
        && LI_DAPM_GET_DISTANCE_TO_HINT == C_NULL_REMAINING_DISTANCE) 
        ||  ( (LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD && LI_DAPM_GET_DISTANCE_TO_HINT > C_MIN_REMAINING_DISTANCE) 
        ||  (LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD && LI_DAPM_GET_DISTANCE_TO_HINT <= C_MIN_REMAINING_DISTANCE)))))) || 
        (
        inLeafState(currentState_APC_GUIDANCE_SC , APC_IDLE_GEAR, StateInfo_APC_GUIDANCE_SC)
         && ((A_PARK_STYLE == PARK_OUT &&  ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D &&  (LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD || LI_DAPM_GET_DISTANCE_TO_HINT <= C_NULL_REMAINING_DISTANCE)) 
        ||  ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R && LI_DAPM_GET_DISTANCE_TO_HINT >= C_MIN_REMAINING_DISTANCE))))))) {
          /* Transition to State: APC_GUIDANCE_SC:REVERSE_GEAR_WAIT */
          /* @Traceability :Sys_APC_PTS_Logic_2625;Sys_APC_PTS_Logic_2626;Sys_APC_PTS_Logic_2628;*/  
            
            
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_REVERSE_GEAR_WAIT/* APC_GUIDANCE_SC:REVERSE_GEAR_WAIT */  ;
        }
        else if ((inState(currentState_APC_GUIDANCE_SC , STATE_45)
         && ( (( (LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_ACTIVE || LI_DAPM_OPERATION_STATE == DAPM_OS_EXPLORATION_MODE_ACTIVE 
        || LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_DRIVE_TO_POS_OK) 
        &&  ( (LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD) ||  (LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD && A_REMAINING_DISTANCE <= C_MIN_REMAINING_DISTANCE)))) 
        &&  ( (!  ( (CHANGED(LI_COMH_CAR_GEAR_LEVER_POSITION)) &&  ((LI_COMH_CAR_GEAR_LEVER_POSITION != GEAR_POSITION_R))))))) || 
        (
        inLeafState(currentState_APC_GUIDANCE_SC , APC_IDLE_GEAR, StateInfo_APC_GUIDANCE_SC)
         && ((A_PARK_STYLE == PARK_IN &&  ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R && LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD 
        && LI_DAPM_GET_DISTANCE_TO_HINT == C_NULL_REMAINING_DISTANCE) 
        ||  ( (LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD && LI_DAPM_GET_DISTANCE_TO_HINT > C_MIN_REMAINING_DISTANCE) 
        ||  (LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD && LI_DAPM_GET_DISTANCE_TO_HINT <= C_MIN_REMAINING_DISTANCE)))))) || 
        (
        inLeafState(currentState_APC_GUIDANCE_SC , APC_IDLE_GEAR, StateInfo_APC_GUIDANCE_SC)
         && ((A_PARK_STYLE == PARK_OUT &&  ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R &&  (LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD || LI_DAPM_GET_DISTANCE_TO_HINT <= C_MIN_REMAINING_DISTANCE)) 
        ||  (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D && LI_DAPM_GET_DISTANCE_TO_HINT >= C_MIN_REMAINING_DISTANCE)))))) {
          /* Transition to State: APC_GUIDANCE_SC:DRIVE_GEAR_WAIT */
          /* @Traceability :Sys_APC_PTS_Logic_2617;Sys_APC_PTS_Logic_2618;Sys_APC_PTS_Logic_2620;*/  
            
            
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_DRIVE_GEAR_WAIT/* APC_GUIDANCE_SC:DRIVE_GEAR_WAIT */  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , APC_MANUAL_GEAR_RQ, StateInfo_APC_GUIDANCE_SC)
         && ((!LI_DAPM_STEERING_IN_STANDSTILL &&  ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R) ||  (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)))))) {
          /* Transition to State: APC_GUIDANCE_SC:APC_AUTOGEAR_DELAY */
            
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_AUTOGEAR_DELAY/* APC_GUIDANCE_SC:APC_AUTOGEAR_DELAY */  ;
        }
        else if ((inState(currentState_APC_GUIDANCE_SC , APC_REQ_D)
         && ((CHANGED(LI_COMH_CAR_GEAR_LEVER_POSITION)) &&(LI_COMH_CAR_GEAR_LEVER_POSITION != GEAR_POSITION_D))) || 
        (inState(currentState_APC_GUIDANCE_SC , APC_REQ_R)
         && ((CHANGED(LI_COMH_CAR_GEAR_LEVER_POSITION)) &&(LI_COMH_CAR_GEAR_LEVER_POSITION != GEAR_POSITION_R)))) {
          /* Transition to State: APC_GUIDANCE_SC:APC_MANUAL_GEAR_RQ */
            
            
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_APC_MANUAL_GEAR_RQ/* APC_GUIDANCE_SC:APC_MANUAL_GEAR_RQ *//* @Traceability :Sys_APC_PTS_Logic_4417;Sys_APC_PTS_Logic_5339;Sys_APC_PTS_Logic_5646;Sys_APC_PTS_Logic_6197;*/  ;
        }
        else if ((
        inLeafState(currentState_APC_GUIDANCE_SC , REVERSE_GEAR_STANDSTILL, StateInfo_APC_GUIDANCE_SC)
         && (tm_999999998_brkh_cus))) {
          /* Transition to State: APC_GUIDANCE_SC:REVERSE_GEAR_END_CHECK */
           /*@Transition Priority:2*/ 
          nextState_brkh_cusAPC_GUIDANCE_SC = FS_REVERSE_GEAR_END_CHECK/* APC_GUIDANCE_SC:REVERSE_GEAR_END_CHECK */  ;
        }
        else if (inState(currentState_APC_GUIDANCE_SC , APC_REVERSE_GEAR)) {
          staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_REVERSE_GEAR;
          /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_REVERSE_GEAR */
          if ( (A_START_OF_FIRST_STROKE)) {
            A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          }
          else {
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
          }
          if ((
          inLeafState(currentState_APC_GUIDANCE_SC , REVERSE_GEAR_WAIT, StateInfo_APC_GUIDANCE_SC)
           && ((LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
            /* Transition to State: APC_GUIDANCE_SC:REVERSE_GEAR_STANDSTILL */
              
            nextState_brkh_cusAPC_GUIDANCE_SC = FS_REVERSE_GEAR_STANDSTILL/* APC_GUIDANCE_SC:REVERSE_GEAR_STANDSTILL */  ;
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , REVERSE_GEAR_WAIT, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_REVERSE_GEAR_WAIT;
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , REVERSE_GEAR_STANDSTILL, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_REVERSE_GEAR_STANDSTILL;
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , REVERSE_GEAR_END_CHECK, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_REVERSE_GEAR_END_CHECK;
            /* In-state reaction and actions of State: APC_GUIDANCE_SC:REVERSE_GEAR_END_CHECK */
            if ( (LI_DAPM_IS_VEHICLE_STANDSTILL &&  (LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_ACTIVE || LI_DAPM_OPERATION_STATE == DAPM_OS_EXPLORATION_MODE_ACTIVE 
              || LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_DRIVE_TO_POS_OK))) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
            }
          }
          else {
          }
        }
        else if (inState(currentState_APC_GUIDANCE_SC , APC_REQ_R)) {
          staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_REQ_R;
          if ((
          inLeafState(currentState_APC_GUIDANCE_SC , R_CHECK, StateInfo_APC_GUIDANCE_SC)
           && ((!LI_DAPM_STEERING_IN_STANDSTILL)))) {
            /* Transition to State: APC_GUIDANCE_SC:R_DRIVE */
              
            nextState_brkh_cusAPC_GUIDANCE_SC = FS_R_DRIVE/* APC_GUIDANCE_SC:R_DRIVE */  ;
          }
          else if ((
          inLeafState(currentState_APC_GUIDANCE_SC , R_INIT, StateInfo_APC_GUIDANCE_SC)
           && (tm_999999998_brkh_cus))) {
            /* Transition to State: APC_GUIDANCE_SC:R_CHECK */
              
            nextState_brkh_cusAPC_GUIDANCE_SC = FS_R_CHECK/* APC_GUIDANCE_SC:R_CHECK */  ;
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , R_INIT, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_R_INIT;
            /* In-state reaction and actions of State: APC_GUIDANCE_SC:R_INIT */
            if ( (LI_DAPM_STEERING_IN_STANDSTILL)) {
              A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
            }
            else {
              A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
            }
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , R_CHECK, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_R_CHECK;
            /* In-state reaction and actions of State: APC_GUIDANCE_SC:R_CHECK */
            if ( (LI_DAPM_STEERING_IN_STANDSTILL)) {
              A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
            }
            else {
              A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
            }
          }
          else if (inState(currentState_APC_GUIDANCE_SC , STATE_45)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_STATE_45;
            /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_GEAR_CHANGED_TO_R.STATE#44.STATE#45 */
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
            if ((
            inLeafState(currentState_APC_GUIDANCE_SC , R_STANDSTILL, StateInfo_APC_GUIDANCE_SC)
             && ((LI_DAPM_IS_VEHICLE_STANDSTILL && A_RD_NEW_MEAS && LI_DAPM_GET_DISTANCE_TO_HINT > C_MIN_REMAINING_DISTANCE 
            && LI_DAPM_GET_DISTANCE_TO_COLLISION > C_MIN_REMAINING_DISTANCE)))) {
              /* Transition to State: APC_GUIDANCE_SC:R_CONTINUE */
               /*@Transition Priority:1*/ 
              A_RD_CONTINUE_FEATURE = LOGICAL_CAST(TRUE);
              nextState_brkh_cusAPC_GUIDANCE_SC = FS_R_CONTINUE/* APC_GUIDANCE_SC:R_CONTINUE */  ;
            }
            else if ((
            inLeafState(currentState_APC_GUIDANCE_SC , R_CONTINUE, StateInfo_APC_GUIDANCE_SC)
             && ((!LI_DAPM_IS_VEHICLE_STANDSTILL))) || (
            inLeafState(currentState_APC_GUIDANCE_SC , R_STANDSTILL, StateInfo_APC_GUIDANCE_SC)
             && ((!LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
              /* Transition to State: APC_GUIDANCE_SC:R_DRIVE */
                
               /*@Transition Priority:2*/ 
              nextState_brkh_cusAPC_GUIDANCE_SC = FS_R_DRIVE/* APC_GUIDANCE_SC:R_DRIVE */  ;
            }
            else if ((
            inLeafState(currentState_APC_GUIDANCE_SC , R_DRIVE, StateInfo_APC_GUIDANCE_SC)
             && ((LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
              /* Transition to State: APC_GUIDANCE_SC:R_STANDSTILL */
                
              nextState_brkh_cusAPC_GUIDANCE_SC = FS_R_STANDSTILL/* APC_GUIDANCE_SC:R_STANDSTILL */  ;
            }
            else {
            }
          }
          else {
          }
        }
        else if (inState(currentState_APC_GUIDANCE_SC , APC_REQ_D)) {
          staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_REQ_D;
          if ((
          inLeafState(currentState_APC_GUIDANCE_SC , D_CHECK, StateInfo_APC_GUIDANCE_SC)
           && ((!LI_DAPM_STEERING_IN_STANDSTILL)))) {
            /* Transition to State: APC_GUIDANCE_SC:D_DRIVE */
              
            nextState_brkh_cusAPC_GUIDANCE_SC = FS_D_DRIVE/* APC_GUIDANCE_SC:D_DRIVE */  ;
          }
          else if ((
          inLeafState(currentState_APC_GUIDANCE_SC , D_INIT, StateInfo_APC_GUIDANCE_SC)
           && (tm_999999998_brkh_cus))) {
            /* Transition to State: APC_GUIDANCE_SC:D_CHECK */
              
            nextState_brkh_cusAPC_GUIDANCE_SC = FS_D_CHECK/* APC_GUIDANCE_SC:D_CHECK */  ;
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , D_INIT, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_D_INIT;
            /* In-state reaction and actions of State: APC_GUIDANCE_SC:D_INIT */
            if ( (LI_DAPM_STEERING_IN_STANDSTILL)) {
              A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
            }
            else {
              A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
            }
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , D_CHECK, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_D_CHECK;
            /* In-state reaction and actions of State: APC_GUIDANCE_SC:D_CHECK */
            if ( (LI_DAPM_STEERING_IN_STANDSTILL)) {
              A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
            }
            else {
              A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
            }
          }
          else if (inState(currentState_APC_GUIDANCE_SC , STATE_43)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_STATE_43;
            /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_GEAR_CHANGED_TO_D.STATE#42.STATE#43 */
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
            if ((
            inLeafState(currentState_APC_GUIDANCE_SC , D_STANDSTILL, StateInfo_APC_GUIDANCE_SC)
             && ((LI_DAPM_IS_VEHICLE_STANDSTILL && A_RD_NEW_MEAS && LI_DAPM_GET_DISTANCE_TO_HINT > C_MIN_REMAINING_DISTANCE 
            && LI_DAPM_GET_DISTANCE_TO_COLLISION > C_MIN_REMAINING_DISTANCE)))) {
              /* Transition to State: APC_GUIDANCE_SC:D_CONTINUE */
               /*@Transition Priority:1*/ 
              A_RD_CONTINUE_FEATURE = LOGICAL_CAST(TRUE);
              nextState_brkh_cusAPC_GUIDANCE_SC = FS_D_CONTINUE/* APC_GUIDANCE_SC:D_CONTINUE */  ;
            }
            else if ((
            inLeafState(currentState_APC_GUIDANCE_SC , D_CONTINUE, StateInfo_APC_GUIDANCE_SC)
             && ((!LI_DAPM_IS_VEHICLE_STANDSTILL))) || (
            inLeafState(currentState_APC_GUIDANCE_SC , D_STANDSTILL, StateInfo_APC_GUIDANCE_SC)
             && ((!LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
              /* Transition to State: APC_GUIDANCE_SC:D_DRIVE */
                
               /*@Transition Priority:2*/ 
              nextState_brkh_cusAPC_GUIDANCE_SC = FS_D_DRIVE/* APC_GUIDANCE_SC:D_DRIVE */  ;
            }
            else if ((
            inLeafState(currentState_APC_GUIDANCE_SC , D_DRIVE, StateInfo_APC_GUIDANCE_SC)
             && ((LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
              /* Transition to State: APC_GUIDANCE_SC:D_STANDSTILL */
                
              nextState_brkh_cusAPC_GUIDANCE_SC = FS_D_STANDSTILL/* APC_GUIDANCE_SC:D_STANDSTILL */  ;
            }
            else {
            }
          }
          else {
          }
        }
        else if (inLeafState(currentState_APC_GUIDANCE_SC , APC_MANUAL_GEAR_RQ, StateInfo_APC_GUIDANCE_SC)) {
          staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_MANUAL_GEAR_RQ;
          /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_MANUAL_GEAR_RQ */
          A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
            A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_N;
          }
          else {
            if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
            }
            else {
              if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
                A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
              }
              else {
                if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_P)) {
                  A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_P;
                }
              }
            }
          }
        }
        else if (inLeafState(currentState_APC_GUIDANCE_SC , APC_IDLE_GEAR, StateInfo_APC_GUIDANCE_SC)) {
          staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_IDLE_GEAR;
          /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_IDLE_GEAR */
          if ( (A_START_OF_FIRST_STROKE)) {
            A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          }
          else {
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
          }
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
            A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_N;
          }
          else {
            if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
            }
            else {
              if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
                A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
              }
              else {
                if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_P)) {
                  A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_P;
                }
              }
            }
          }
        }
        else if (inLeafState(currentState_APC_GUIDANCE_SC , APC_GEAR_AUTOMATIC, StateInfo_APC_GUIDANCE_SC)) {
          staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_GEAR_AUTOMATIC;
          /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_GEAR_AUTOMATIC */
          if ( (A_START_OF_FIRST_STROKE)) {
            A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          }
          else {
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
          }
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
            A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_N;
          }
          else {
            if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_R;
            }
            else {
              if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
                A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
              }
              else {
                if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_P)) {
                  A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_P;
                }
              }
            }
          }
        }
        else if (inState(currentState_APC_GUIDANCE_SC , APC_DRIVE_GEAR)) {
          staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_DRIVE_GEAR;
          /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_DRIVE_GEAR */
          if ( (A_START_OF_FIRST_STROKE)) {
            A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
          }
          else {
            A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
          }
          if ((
          inLeafState(currentState_APC_GUIDANCE_SC , DRIVE_GEAR_WAIT, StateInfo_APC_GUIDANCE_SC)
           && ((LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
            /* Transition to State: APC_GUIDANCE_SC:DRIVE_GEAR_STANDSTILL */
              
            nextState_brkh_cusAPC_GUIDANCE_SC = FS_DRIVE_GEAR_STANDSTILL/* APC_GUIDANCE_SC:DRIVE_GEAR_STANDSTILL */  ;
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , DRIVE_GEAR_WAIT, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_DRIVE_GEAR_WAIT;
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , DRIVE_GEAR_STANDSTILL, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_DRIVE_GEAR_STANDSTILL;
          }
          else if (inLeafState(currentState_APC_GUIDANCE_SC , DRIVE_GEAR_END_CHECK, StateInfo_APC_GUIDANCE_SC)) {
            staySame_brkh_cusAPC_GUIDANCE_SC = FS_DRIVE_GEAR_END_CHECK;
            /* In-state reaction and actions of State: APC_GUIDANCE_SC:DRIVE_GEAR_END_CHECK */
            if ( (LI_DAPM_IS_VEHICLE_STANDSTILL &&  (LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_ACTIVE || LI_DAPM_OPERATION_STATE == DAPM_OS_EXPLORATION_MODE_ACTIVE 
              || LI_DAPM_OPERATION_STATE == DAPM_OS_PARKING_DRIVE_TO_POS_OK))) {
              A_APC_AUTOGEAR_REQ = GEAR_POSITION_RQ_D;
            }
          }
          else {
          }
        }
        else if (inLeafState(currentState_APC_GUIDANCE_SC , APC_AUTOGEAR_DELAY, StateInfo_APC_GUIDANCE_SC)) {
          staySame_brkh_cusAPC_GUIDANCE_SC = FS_APC_AUTOGEAR_DELAY;
          /* In-state reaction and actions of State: APC_GUIDANCE_SC:APC_AUTOGEAR_DELAY */
          A_APC_REMAINING_DISTANCE = A_REMAINING_DISTANCE;
        }
        else {
        }
      }
      else {
      }
    }
  }
  if (nextState_brkh_cusAPC_GUIDANCE_SC != 0) {
    cgEnterActions_APC_GUIDANCE_SC(staySame_brkh_cusAPC_GUIDANCE_SC, nextState_brkh_cusAPC_GUIDANCE_SC);
    currentState_APC_GUIDANCE_SC = nextState_brkh_cusAPC_GUIDANCE_SC;
  }
}




/* This function executes the Activity: BRKH_INPUTS_AC:APC_GUIDANCE_SC */
void
cgActivity_APC_GUIDANCE_SC(void)
{
  /* BRKH_INPUTS_AC:APC_GUIDANCE_SC */  
  cgDo_APC_GUIDANCE_SC();
}



/* This function calculates the states that were entered, and executes */
/* the appropriate enter-state reaction in chart: APC_VEHSPD_SC */
void
cgEnterActions_APC_VEHSPD_SCcnt1(StateInfo_APC_VEHSPD_SCcnt1 staySame_brkh_cusAPC_VEHSPD_SCcnt1, StateInfo_APC_VEHSPD_SCcnt1 nextState_brkh_cusAPC_VEHSPD_SCcnt1)
{
  if (willBeInState(nextState_brkh_cusAPC_VEHSPD_SCcnt1 , NO_LIMIT)) {
    if (isNotStayingInState(staySame_brkh_cusAPC_VEHSPD_SCcnt1 , NO_LIMIT)) {
      /* Enter-state reaction of State: APC_VEHSPD_SC:NO_LIMIT */
      if ( (LI_COMH_CAR_CURRENT_GEAR == LI_COMH_CAR_GEAR_LEVER_POSITION && LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
        A_APC_VEHICLE_SPEED_REQUEST = C_APC_PARK_VEHICLE_SPEED_RQ_MAX;
      }
      else {
        if ( (( (LI_DAPM_MAX_VELOCITY_RQ * 36) /  (100)) >= C_APC_PARK_VEHICLE_SPEED_RQ_MAX)) {
          A_APC_VEHICLE_SPEED_REQUEST = C_APC_PARK_VEHICLE_SPEED_RQ_MAX;
        }
        else {
          if ( (( (LI_DAPM_MAX_VELOCITY_RQ * 36) /  (100)) >= C_SPEED_MINIMUM_APC_KMH)) {
            A_APC_VEHICLE_SPEED_REQUEST =  (LI_DAPM_MAX_VELOCITY_RQ * 36) /  (100);
          }
          else {
            A_APC_VEHICLE_SPEED_REQUEST = C_SPEED_MINIMUM_APC_KMH;
          }
        }
      }
    }
  }
}



/* This function executes APC_VEHSPD_SC (Statechart) */
/* --------------------------------------------------------------     */
/* |    Chart    |From|   To   |Transition Label|Transition Note| */
/* -------------------------------------------------------------- */
/* |APC_VEHSPD_SC|    |NO_LIMIT|                |               | */
/* -------------------------------------------------------------- */
void
cgDo_APC_VEHSPD_SCcnt1(void)
{
  StateInfo_APC_VEHSPD_SCcnt1 nextState_brkh_cusAPC_VEHSPD_SCcnt1 = 0;
  StateInfo_APC_VEHSPD_SCcnt1 staySame_brkh_cusAPC_VEHSPD_SCcnt1 = 0;
  if ((currentState_APC_VEHSPD_SCcnt1 == 0) || inState(currentState_APC_VEHSPD_SCcnt1 , APC_VEHSPD_SCst2)) {
    staySame_brkh_cusAPC_VEHSPD_SCcnt1 = FS_APC_VEHSPD_SCst2;
    if ((
    ((currentState_APC_VEHSPD_SCcnt1 == 0) || inLeafState(currentState_APC_VEHSPD_SCcnt1 , DefaultOf_APC_VEHSPD_SCst2, StateInfo_APC_VEHSPD_SCcnt1)))) {
      /* Transition to State: APC_VEHSPD_SC:NO_LIMIT */
        
      nextState_brkh_cusAPC_VEHSPD_SCcnt1 = FS_NO_LIMIT/* APC_VEHSPD_SC:NO_LIMIT */  ;
    }
    else if (inLeafState(currentState_APC_VEHSPD_SCcnt1 , NO_LIMIT, StateInfo_APC_VEHSPD_SCcnt1)) {
      staySame_brkh_cusAPC_VEHSPD_SCcnt1 = FS_NO_LIMIT;
      /* In-state reaction and actions of State: APC_VEHSPD_SC:NO_LIMIT */
      if ( (LI_COMH_CAR_CURRENT_GEAR == LI_COMH_CAR_GEAR_LEVER_POSITION && LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
        A_APC_VEHICLE_SPEED_REQUEST = C_APC_PARK_VEHICLE_SPEED_RQ_MAX;
      }
      else {
        if ( (( (LI_DAPM_MAX_VELOCITY_RQ * 36) /  (100)) >= C_APC_PARK_VEHICLE_SPEED_RQ_MAX)) {
          A_APC_VEHICLE_SPEED_REQUEST = C_APC_PARK_VEHICLE_SPEED_RQ_MAX;
        }
        else {
          if ( (( (LI_DAPM_MAX_VELOCITY_RQ * 36) /  (100)) >= C_SPEED_MINIMUM_APC_KMH)) {
            A_APC_VEHICLE_SPEED_REQUEST =  (LI_DAPM_MAX_VELOCITY_RQ * 36) /  (100);
          }
          else {
            A_APC_VEHICLE_SPEED_REQUEST = C_SPEED_MINIMUM_APC_KMH;
          }
        }
      }
    }
    else {
    }
  }
  if (nextState_brkh_cusAPC_VEHSPD_SCcnt1 != 0) {
    cgEnterActions_APC_VEHSPD_SCcnt1(staySame_brkh_cusAPC_VEHSPD_SCcnt1, nextState_brkh_cusAPC_VEHSPD_SCcnt1);
    currentState_APC_VEHSPD_SCcnt1 = nextState_brkh_cusAPC_VEHSPD_SCcnt1;
  }
}




/* This function executes the Activity: BRKH_INPUTS_AC:APC_VEHSPD_SC */
void
cgActivity_APC_VEHSPD_SCcnt1(void)
{
  /* BRKH_INPUTS_AC:APC_VEHSPD_SC */  
  cgDo_APC_VEHSPD_SCcnt1();
}



/* This function executes the Activity: BRKH_INPUTS_AC:APC_GEAR_AC */
void
cgActivity_APC_GEAR_AC(void)
{
  /* BRKH_INPUTS_AC:APC_GEAR_AC */  
  if ((cgGlobalFlags_brkh_cus & BITAC_APC_GEAR_AC) != 0) {
    cgActivity_APC_GUIDANCE_SC();
  }
}



/* This function executes the Activity: BRKH_INPUTS_AC:APC_SPEED_AC */
void
cgActivity_APC_SPEED_AC(void)
{
  /* BRKH_INPUTS_AC:APC_SPEED_AC */  
  if ((cgGlobalFlags_brkh_cus & BITAC_APC_SPEED_AC) != 0) {
    cgActivity_APC_VEHSPD_SCcnt1();
  }
}



/* This function calculates the states that were entered, and executes */
/* the appropriate enter-state reaction in chart: CTRL_APC_CHECK_SC */
void
cgEnterActions_CTRL_APC_CHECK_SC(StateInfo_CTRL_APC_CHECK_SC staySame_brkh_cusCTRL_APC_CHECK_SC, StateInfo_CTRL_APC_CHECK_SC nextState_brkh_cusCTRL_APC_CHECK_SC)
{
  if (willBeInState(nextState_brkh_cusCTRL_APC_CHECK_SC , APC_TAKEOVER)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_TAKEOVER)) {
      /* Enter-state reaction of State: CTRL_APC_CHECK_SC:APC_TAKEOVER */
      if ( ( (PARK_IN == A_PARK_STYLE) &&  (LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE))) {
        A_APC_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_P;
        A_APC_PARKMAN_STATUS = PARK_PARK_STAT_END;
      }
    }
  }
  else if (willBeInState(nextState_brkh_cusCTRL_APC_CHECK_SC , APC_SEARCH)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_SEARCH)) {
      /* Enter-state reaction of State: CTRL_APC_CHECK_SC:APC_SEARCH */
      A_APC_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_IDLE;
      A_PARK_STYLE = BRKH_SetParkStyle();
      A_APC_PARKMAN_BRAKE_STATE = BRAKING_STATE_PARKMAN_INACTIV;
      A_APC_PARKMAN_PARKTYPE = PARK_PARKTYPE_IDLE;
      if ( ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D) &&  (PARK_IN == A_PARK_STYLE))) {
        A_APC_PARKMAN_STATUS = PARK_PARK_STAT_SEARCH;
      }
      else {
        if ( ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R) &&  (PARK_IN == A_PARK_STYLE))) {
          A_APC_PARKMAN_STATUS = PARK_PARK_STAT_IDLE;
        }
        else {
          if ( (PARK_OUT == A_PARK_STYLE)) {
            A_APC_PARKMAN_STATUS = PARK_PARK_STAT_PARK_OUT;
          }
        }
      }
    }
  }
  else if (willBeInState(nextState_brkh_cusCTRL_APC_CHECK_SC , APC_END)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_END)) {
      /* Enter-state reaction of State: CTRL_APC_CHECK_SC:APC_END */
      A_APC_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
      BRKH_CusSetBrakeState(FALSE);
      A_APC_PARKMAN_STATUS = PARK_PARK_STAT_END;
      if ( ( (PARK_IN == A_PARK_STYLE) &&  (LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE))) {
        A_APC_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_P;
      }
      if ( (PARK_IN == A_PARK_STYLE)) {
        A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
      }
      else {
        A_APC_REMAINING_DISTANCE = C_PMO_REMAINING_DISTANCE;
      }
    }
  }
  else if (willBeInState(nextState_brkh_cusCTRL_APC_CHECK_SC , APC_DISCLAIMER)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_DISCLAIMER)) {
      /* Enter-state reaction of State: CTRL_APC_CHECK_SC:APC_DISCLAIMER */
      A_APC_VEHICLE_SPEED_REQUEST = C_APC_PARK_VEHICLE_SPEED_RQ_MAX;
      A_PARK_STYLE = BRKH_SetParkStyle();
      if ( (A_PARK_STYLE == PARK_OUT)) {
        A_APC_PARKMAN_STATUS = PARK_PARK_STAT_PARK_OUT;
      }
      else {
        A_APC_PARKMAN_STATUS = PARK_PARK_STAT_FOUND;
      }
      A_CAR_READY_P4U = LOGICAL_CAST(FALSE);
      if ( (LI_COMH_APC_ESP_ENABLE_STATE)) {
        if ( ( (LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_PARKMAN_INACTIV || LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE 
          || LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_PPB_MODE) &&  ( (A_APC_PARKMAN_STATUS == PARK_PARK_STAT_DISCLAIMER) ||  (LO_PARK_PARK_STAT == PARK_PARK_STAT_BACK_STROKE) 
          ||  (LO_PARK_PARK_STAT == PARK_PARK_STAT_FWD_STROKE)))) {
          A_CAR_READY_P4U = LOGICAL_CAST(TRUE);
        }
      }
    }
  }
  else if (willBeInState(nextState_brkh_cusCTRL_APC_CHECK_SC , APC_ACTIVE)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_ACTIVE)) {
      currentTick_brkh_cus = MS_COUNTER;
      INSTALL_TIMEOUT(tm_999999992_brkh_cus,1000 / C_TASK_CYCLIC_TIME,MS_COUNTER_INDEX);
    }
    if (willBeInState(nextState_brkh_cusCTRL_APC_CHECK_SC , APC_WAIT_CONTROL)) {
      if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_WAIT_CONTROL)) {
        /* Enter-state reaction of State: CTRL_APC_CHECK_SC:APC_WAIT_CONTROL */
        if ( (! (is_active(APC_GEAR_AC)))) {
          start_activity(APC_GEAR_AC);
        }
        if ( (! (is_active(APC_SPEED_AC)))) {
          start_activity(APC_SPEED_AC);
        }
        A_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
        if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R || LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N)) {
          A_APC_PARKMAN_STATUS = PARK_PARK_STAT_BACK_STROKE;
        }
        else {
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
            A_APC_PARKMAN_STATUS = PARK_PARK_STAT_FWD_STROKE;
          }
        }
        BRKH_SetParkParkStat(A_APC_PARKMAN_STATUS);
        A_APC_PARKMAN_BRAKE_STATE = BRAKING_STATE_APC_MODE;
        A_APC_PARKMAN_PARKTYPE = BRKH_GetParktype();
      }
    }
    else if (willBeInState(nextState_brkh_cusCTRL_APC_CHECK_SC , APC_CONTROL)) {
      if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_CONTROL)) {
        /* Enter-state reaction of State: CTRL_APC_CHECK_SC:APC_CONTROL */
        A_PARK_STYLE = BRKH_SetParkStyle();
        if ( (! (is_active(APC_GEAR_AC)))) {
          start_activity(APC_GEAR_AC);
        }
        if ( (! (is_active(APC_SPEED_AC)))) {
          start_activity(APC_SPEED_AC);
        }
        A_CAR_READY_P4U = LOGICAL_CAST(TRUE);
        if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R ||  (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N && LI_DAPM_DRIVING_HINT == CAPP_DD_BACKWARD))) {
          A_APC_PARKMAN_STATUS = PARK_PARK_STAT_BACK_STROKE;
        }
        else {
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D ||  (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_N && LI_DAPM_DRIVING_HINT == CAPP_DD_FORWARD))) {
            A_APC_PARKMAN_STATUS = PARK_PARK_STAT_FWD_STROKE;
          }
        }
        A_APC_PARKMAN_BRAKE_STATE = BRAKING_STATE_APC_MODE;
        A_APC_PARKMAN_PARKTYPE = BRKH_GetParktype();
      }
    }
    else {
    }
  }
  else if (willBeInState(nextState_brkh_cusCTRL_APC_CHECK_SC , APC_ABORT)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_ABORT)) {
      /* Enter-state reaction of State: CTRL_APC_CHECK_SC:APC_ABORT */
      A_APC_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
      BRKH_CusSetBrakeState(FALSE);
      P2DAL_AbortManeuver();
      A_APC_PARKMAN_STATUS = PARK_PARK_STAT_FLT;
      A_APC_REMAINING_DISTANCE = C_MAX_REMAINING_DISTANCE;
    }
  }
  else {
  }
}



/* This function calculates the states that are exited, and executes */
/* the appropriate exit-state reaction in chart: CTRL_APC_CHECK_SC */
void
cgExitActions_CTRL_APC_CHECK_SC(StateInfo_CTRL_APC_CHECK_SC staySame_brkh_cusCTRL_APC_CHECK_SC)
{
  if (wasInState(currentState_CTRL_APC_CHECK_SC , APC_CONTROL)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_APC_CHECK_SC , APC_CONTROL)) {
      /* Exit-state reaction of State: CTRL_APC_CHECK_SC:APC_CONTROL */
      stop_activity(REM_DIST_AC);
      stop_activity(APC_GEAR_AC);
      stop_activity(APC_SPEED_AC);
    }
  }
}



/* This function executes CTRL_APC_CHECK_SC (Statechart) */
/* ------------------------------------------------------------------------------------------------------------------------     */
/* |      Chart      |      From      |       To       |                 Transition Label                 |Transition Note| */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|                |APC_SEARCH      |                                                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_SEARCH      |APC_DISCLAIMER  |[LI_DAPM_IS_VEHICLE_STANDSTILL==true]             |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_DISCLAIMER  |APC_SEARCH      |[LI_DAPM_IS_VEHICLE_STANDSTILL==false]            |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_DISCLAIMER  |APC_WAIT_CONTROL|[A_CAR_READY_P4U==true and true==BRKH_DasActivatin|               | */
/* |                 |                |                |gRequest()]/A_PARK_STYLE=BRKH_SetParkStyle();A_APC|               | */
/* |                 |                |                |_CTRL_ACTIVE=true;EVT_EVALUATE_PARKTYPE;A_RD_INIT=|               | */
/* |                 |                |                |true;A_START_OF_FIRST_STROKE=true;A_RD_CAR_POSITIO|               | */
/* |                 |                |                |N_LAST=LI_ODOM_CAR_POSITION;A_RD_DRIVEN_DIST_SINCE|               | */
/* |                 |                |                |_INIT_DTH=0;A_RD_DRIVEN_DIST_SINCE_INIT_DTC=0;    |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_ABORT       |APC_SEARCH      |[LI_COMH_ESP_BRAKE_STATE!=BRAKING_STATE_APC_MODE] |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_CONTROL     |APC_ABORT       |[((LI_DAPM_IS_CTRL_ACTIVE==false) or (LI_DAPM_OPER|               | */
/* |                 |                |                |ATION_STATE==DAPM_OS_ABORTING)) and (DAPM_OAR_NONE|               | */
/* |                 |                |                |!=LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON)] or [(LI_|               | */
/* |                 |                |                |COMH_ESP_INTERVENTION_STATE==ESP_INTERVENTION_ACTI|               | */
/* |                 |                |                |VE) or (LI_COMH_APC_ESP_ENABLE_STATE==false)]     |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_CONTROL     |APC_TAKEOVER    |[(A_PARK_STYLE==PARK_IN) and ((LI_DAPM_IS_CTRL_ACT|               | */
/* |                 |                |                |IVE==false) or (LI_DAPM_OPERATION_STATE==DAPM_OS_F|               | */
/* |                 |                |                |INISHING)) and (DAPM_OAR_NONE==LI_DAPM_GETSPECIFIC|               | */
/* |                 |                |                |_CTRL_ABORT_REASON)]                              |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_TAKEOVER    |APC_ABORT       |[(LI_COMH_ESP_INTERVENTION_STATE==ESP_INTERVENTION|               | */
/* |                 |                |                |_ACTIVE) or ((LI_COMH_ESP_BRAKE_STATE!=BRAKING_STA|               | */
/* |                 |                |                |TE_APC_MODE) and (LI_COMH_ESP_BRAKE_STATE!=BRAKING|               | */
/* |                 |                |                |_STATE_PARKMAN_INACTIV))]                         |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_TAKEOVER    |APC_END         |[LI_COMH_ESP_BRAKE_STATE!=BRAKING_STATE_APC_MODE] |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_WAIT_CONTROL|APC_CONTROL     |[LI_COMH_ESP_BRAKE_STATE==BRAKING_STATE_APC_MODE a|               | */
/* |                 |                |                |nd LI_DAPM_IS_CTRL_ACTIVE==true]                  |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_ACTIVE      |APC_ABORT       |dly(1000/C_TASK_CYCLIC_TIME)[LI_COMH_ESP_BRAKE_STA|               | */
/* |                 |                |                |TE!=BRAKING_STATE_APC_MODE]                       |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_END         |APC_SEARCH      |[LI_COMH_ESP_BRAKE_STATE!=BRAKING_STATE_APC_MODE] |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_WAIT_CONTROL|APC_ABORT       |[false==BRKH_DasActivatingRequest()]              |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
/* |CTRL_APC_CHECK_SC|APC_CONTROL     |APC_END         |[(A_PARK_STYLE==PARK_OUT) and ((LI_DAPM_IS_CTRL_AC|               | */
/* |                 |                |                |TIVE==false) or (LI_DAPM_OPERATION_STATE==DAPM_OS_|               | */
/* |                 |                |                |FINISHING)) and (DAPM_OAR_NONE==LI_DAPM_GETSPECIFI|               | */
/* |                 |                |                |C_CTRL_ABORT_REASON)]                             |               | */
/* ------------------------------------------------------------------------------------------------------------------------ */
void
cgDo_CTRL_APC_CHECK_SC(void)
{
  StateInfo_CTRL_APC_CHECK_SC nextState_brkh_cusCTRL_APC_CHECK_SC = 0;
  StateInfo_CTRL_APC_CHECK_SC staySame_brkh_cusCTRL_APC_CHECK_SC = 0;
  if ((currentState_CTRL_APC_CHECK_SC == 0) || inState(currentState_CTRL_APC_CHECK_SC , CTRL_APC_CHECK)) {
    staySame_brkh_cusCTRL_APC_CHECK_SC = FS_CTRL_APC_CHECK;
    if ((
    ((currentState_CTRL_APC_CHECK_SC == 0) || inLeafState(currentState_CTRL_APC_CHECK_SC , DefaultOf_CTRL_APC_CHECK, StateInfo_CTRL_APC_CHECK_SC)))) {
      /* Transition to State: CTRL_APC_CHECK_SC:APC_SEARCH */
        
      nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_SEARCH/* CTRL_APC_CHECK_SC:APC_SEARCH */  ;
    }
    else if (inState(currentState_CTRL_APC_CHECK_SC , STATE_19)) {
      staySame_brkh_cusCTRL_APC_CHECK_SC = FS_STATE_19;
      if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_DISCLAIMER, StateInfo_CTRL_APC_CHECK_SC)
       && ((!LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_SEARCH */
         /*@Transition Priority:2*/ 
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_SEARCH/* CTRL_APC_CHECK_SC:APC_SEARCH */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_DISCLAIMER, StateInfo_CTRL_APC_CHECK_SC)
       && ((A_CAR_READY_P4U && BRKH_DasActivatingRequest())))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_WAIT_CONTROL */
          
        A_PARK_STYLE = BRKH_SetParkStyle();
        A_APC_CTRL_ACTIVE = LOGICAL_CAST(TRUE);
        BRKH_EvaluateParktype();
        A_RD_INIT = LOGICAL_CAST(TRUE);
        A_START_OF_FIRST_STROKE = LOGICAL_CAST(TRUE);
        A_RD_CAR_POSITION_LAST = LI_ODOM_CAR_POSITION;
        A_RD_DRIVEN_DIST_SINCE_INIT_DTH = 0;
        A_RD_DRIVEN_DIST_SINCE_INIT_DTC = 0;
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_WAIT_CONTROL/* CTRL_APC_CHECK_SC:APC_WAIT_CONTROL */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_TAKEOVER, StateInfo_CTRL_APC_CHECK_SC)
       && (( (LI_COMH_ESP_INTERVENTION_STATE == ESP_INTERVENTION_ACTIVE) ||  ( (LI_COMH_ESP_BRAKE_STATE != BRAKING_STATE_APC_MODE) &&  (LI_COMH_ESP_BRAKE_STATE != BRAKING_STATE_PARKMAN_INACTIV)))))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_ABORT */
         /*@Transition Priority:1*/ 
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_ABORT/* CTRL_APC_CHECK_SC:APC_ABORT */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_TAKEOVER, StateInfo_CTRL_APC_CHECK_SC)
       && ((LI_COMH_ESP_BRAKE_STATE != BRAKING_STATE_APC_MODE)))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_END */
         /*@Transition Priority:2*/ 
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_END/* CTRL_APC_CHECK_SC:APC_END */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_CONTROL, StateInfo_CTRL_APC_CHECK_SC)
       && (( ( (!LI_DAPM_IS_CTRL_ACTIVE) ||  (LI_DAPM_OPERATION_STATE == DAPM_OS_ABORTING)) 
      &&  (DAPM_OAR_NONE != LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON)) || ( (LI_COMH_ESP_INTERVENTION_STATE == ESP_INTERVENTION_ACTIVE) ||  (!LI_COMH_APC_ESP_ENABLE_STATE))))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_ABORT */
         /*@Transition Priority:1*/ 
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_ABORT/* CTRL_APC_CHECK_SC:APC_ABORT */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_CONTROL, StateInfo_CTRL_APC_CHECK_SC)
       && (( (A_PARK_STYLE == PARK_IN) &&  ( (!LI_DAPM_IS_CTRL_ACTIVE) ||  (LI_DAPM_OPERATION_STATE == DAPM_OS_FINISHING)) 
      &&  (DAPM_OAR_NONE == LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON))))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_TAKEOVER */
         /*@Transition Priority:2*/ 
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_TAKEOVER/* CTRL_APC_CHECK_SC:APC_TAKEOVER */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_CONTROL, StateInfo_CTRL_APC_CHECK_SC)
       && (( (A_PARK_STYLE == PARK_OUT) &&  ( (!LI_DAPM_IS_CTRL_ACTIVE) ||  (LI_DAPM_OPERATION_STATE == DAPM_OS_FINISHING)) 
      &&  (DAPM_OAR_NONE == LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON))))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_END */
         /*@Transition Priority:3*/ 
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_END/* CTRL_APC_CHECK_SC:APC_END */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_WAIT_CONTROL, StateInfo_CTRL_APC_CHECK_SC)
       && ((!BRKH_DasActivatingRequest())))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_ABORT */
         /*@Transition Priority:1*/ 
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_ABORT/* CTRL_APC_CHECK_SC:APC_ABORT */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_ABORT, StateInfo_CTRL_APC_CHECK_SC)
       && ((LI_COMH_ESP_BRAKE_STATE != BRAKING_STATE_APC_MODE))) || (
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_END, StateInfo_CTRL_APC_CHECK_SC)
       && ((LI_COMH_ESP_BRAKE_STATE != BRAKING_STATE_APC_MODE)))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_SEARCH */
          
          
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_SEARCH/* CTRL_APC_CHECK_SC:APC_SEARCH */  ;
      }
      else if ((
      inState(currentState_CTRL_APC_CHECK_SC , APC_ACTIVE)
       && ((tm_999999992_brkh_cus) &&(LI_COMH_ESP_BRAKE_STATE != BRAKING_STATE_APC_MODE)))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_ABORT */
          
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_ABORT/* CTRL_APC_CHECK_SC:APC_ABORT */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_SEARCH, StateInfo_CTRL_APC_CHECK_SC)
       && ((LI_DAPM_IS_VEHICLE_STANDSTILL)))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_DISCLAIMER */
          
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_DISCLAIMER/* CTRL_APC_CHECK_SC:APC_DISCLAIMER */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_APC_CHECK_SC , APC_WAIT_CONTROL, StateInfo_CTRL_APC_CHECK_SC)
       && ((LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE && LI_DAPM_IS_CTRL_ACTIVE)))) {
        /* Transition to State: CTRL_APC_CHECK_SC:APC_CONTROL */
         /*@Transition Priority:2*/ 
        nextState_brkh_cusCTRL_APC_CHECK_SC = FS_APC_CONTROL/* CTRL_APC_CHECK_SC:APC_CONTROL */  ;
      }
      else if (inLeafState(currentState_CTRL_APC_CHECK_SC , APC_TAKEOVER, StateInfo_CTRL_APC_CHECK_SC)) {
        staySame_brkh_cusCTRL_APC_CHECK_SC = FS_APC_TAKEOVER;
        /* In-state reaction and actions of State: CTRL_APC_CHECK_SC:APC_TAKEOVER */
        if ( ( (PARK_IN == A_PARK_STYLE) &&  (LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE))) {
          A_APC_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_P;
          A_APC_PARKMAN_STATUS = PARK_PARK_STAT_END;
        }
      }
      else if (inLeafState(currentState_CTRL_APC_CHECK_SC , APC_SEARCH, StateInfo_CTRL_APC_CHECK_SC)) {
        staySame_brkh_cusCTRL_APC_CHECK_SC = FS_APC_SEARCH;
        /* In-state reaction and actions of State: CTRL_APC_CHECK_SC:APC_SEARCH */
        A_PARK_STYLE = BRKH_SetParkStyle();
        A_APC_PARKMAN_BRAKE_STATE = BRAKING_STATE_PARKMAN_INACTIV;
        A_APC_PARKMAN_PARKTYPE = PARK_PARKTYPE_IDLE;
        if ( ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D) &&  (PARK_IN == A_PARK_STYLE))) {
          A_APC_PARKMAN_STATUS = PARK_PARK_STAT_SEARCH;
        }
        else {
          if ( ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R) &&  (PARK_IN == A_PARK_STYLE))) {
            A_APC_PARKMAN_STATUS = PARK_PARK_STAT_IDLE;
          }
          else {
            if ( (PARK_OUT == A_PARK_STYLE)) {
              A_APC_PARKMAN_STATUS = PARK_PARK_STAT_PARK_OUT;
            }
          }
        }
      }
      else if (inLeafState(currentState_CTRL_APC_CHECK_SC , APC_END, StateInfo_CTRL_APC_CHECK_SC)) {
        staySame_brkh_cusCTRL_APC_CHECK_SC = FS_APC_END;
        /* In-state reaction and actions of State: CTRL_APC_CHECK_SC:APC_END */
        A_APC_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
        A_APC_PARKMAN_STATUS = PARK_PARK_STAT_END;
        if ( ( (PARK_IN == A_PARK_STYLE) &&  (LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE))) {
          A_APC_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_P;
        }
        if ( (PARK_IN == A_PARK_STYLE)) {
          A_APC_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
        }
        else {
          A_APC_REMAINING_DISTANCE = C_PMO_REMAINING_DISTANCE;
        }
      }
      else if (inLeafState(currentState_CTRL_APC_CHECK_SC , APC_DISCLAIMER, StateInfo_CTRL_APC_CHECK_SC)) {
        staySame_brkh_cusCTRL_APC_CHECK_SC = FS_APC_DISCLAIMER;
        /* In-state reaction and actions of State: CTRL_APC_CHECK_SC:APC_DISCLAIMER */
        A_PARK_STYLE = BRKH_SetParkStyle();
        if ( (A_PARK_STYLE == PARK_OUT)) {
          A_APC_PARKMAN_STATUS = PARK_PARK_STAT_PARK_OUT;
        }
        else {
          A_APC_PARKMAN_STATUS = PARK_PARK_STAT_DISCLAIMER;
        }
        A_CAR_READY_P4U = LOGICAL_CAST(FALSE);
        if ( (LI_COMH_APC_ESP_ENABLE_STATE)) {
          if ( ( (LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_PARKMAN_INACTIV || LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE 
            || LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_PPB_MODE) &&  ( (A_APC_PARKMAN_STATUS == PARK_PARK_STAT_DISCLAIMER) ||  (LO_PARK_PARK_STAT == PARK_PARK_STAT_BACK_STROKE) 
            ||  (LO_PARK_PARK_STAT == PARK_PARK_STAT_FWD_STROKE) || (LO_PARK_PARK_STAT == PARK_PARK_STAT_PARK_OUT)))) {
            A_CAR_READY_P4U = LOGICAL_CAST(TRUE);
          }
        }
      }
      else if (inState(currentState_CTRL_APC_CHECK_SC , APC_ACTIVE)) {
        staySame_brkh_cusCTRL_APC_CHECK_SC = FS_APC_ACTIVE;
        /* In-state reaction and actions of State: CTRL_APC_CHECK_SC:APC_ACTIVE */
        if ( ( (LI_DAPM_IS_CTRL_ACTIVE) &&  ( ( (LI_DAPM_GET_DISTANCE_TO_HINT > 1000) &&  (PARK_IN == A_PARK_STYLE)) 
          ||  ( (PARK_OUT == A_PARK_STYLE) &&  (LI_DAPM_GET_DISTANCE_TO_HINT > 100))))) {
          if ( (!is_active(REM_DIST_AC))) {
            start_activity(REM_DIST_AC);
          }
        }
        if ( (LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE)) {
          BRKH_CusSetBrakeState(TRUE);
        }
        else {
          BRKH_CusSetBrakeState(FALSE);
        }
        if (inLeafState(currentState_CTRL_APC_CHECK_SC , APC_WAIT_CONTROL, StateInfo_CTRL_APC_CHECK_SC)) {
          staySame_brkh_cusCTRL_APC_CHECK_SC = FS_APC_WAIT_CONTROL;
        }
        else if (inLeafState(currentState_CTRL_APC_CHECK_SC , APC_CONTROL, StateInfo_CTRL_APC_CHECK_SC)) {
          staySame_brkh_cusCTRL_APC_CHECK_SC = FS_APC_CONTROL;
          /* In-state reaction and actions of State: CTRL_APC_CHECK_SC:APC_CONTROL */
          A_PARK_STYLE = BRKH_SetParkStyle();
          A_CAR_READY_P4U = LOGICAL_CAST(TRUE);
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
            A_APC_PARKMAN_STATUS = PARK_PARK_STAT_BACK_STROKE;
          }
          else {
            if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
              A_APC_PARKMAN_STATUS = PARK_PARK_STAT_FWD_STROKE;
            }
          }
          A_APC_PARKMAN_BRAKE_STATE = BRAKING_STATE_APC_MODE;
          A_APC_PARKMAN_PARKTYPE = BRKH_GetParktype();
        }
        else {
        }
      }
      else if (inLeafState(currentState_CTRL_APC_CHECK_SC , APC_ABORT, StateInfo_CTRL_APC_CHECK_SC)) {
        staySame_brkh_cusCTRL_APC_CHECK_SC = FS_APC_ABORT;
        /* In-state reaction and actions of State: CTRL_APC_CHECK_SC:APC_ABORT */
        A_APC_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
        P2DAL_AbortManeuver();
        A_APC_PARKMAN_STATUS = PARK_PARK_STAT_FLT;
        A_APC_REMAINING_DISTANCE = C_MAX_REMAINING_DISTANCE;
      }
      else {
      }
    }
    else {
    }
  }
  if (nextState_brkh_cusCTRL_APC_CHECK_SC != 0) {
    cgExitActions_CTRL_APC_CHECK_SC(staySame_brkh_cusCTRL_APC_CHECK_SC);
    cgEnterActions_CTRL_APC_CHECK_SC(staySame_brkh_cusCTRL_APC_CHECK_SC, nextState_brkh_cusCTRL_APC_CHECK_SC);
    currentState_CTRL_APC_CHECK_SC = nextState_brkh_cusCTRL_APC_CHECK_SC;
  }
}




/* This function executes the Activity: BRKH_INPUTS_AC:CTRL_APC_CHECK_SC */
void
cgActivity_CTRL_APC_CHECK_SC(void)
{
  /* BRKH_INPUTS_AC:CTRL_APC_CHECK_SC */  
  cgDo_CTRL_APC_CHECK_SC();
}



/* This function calculates the states that were entered, and executes */
/* the appropriate enter-state reaction in chart: CTRL_PPB_CHECK_SC */
void
cgEnterActions_CTRL_PPB_CHECK_SC(StateInfo_CTRL_PPB_CHECK_SC staySame_brkh_cusCTRL_PPB_CHECK_SC, StateInfo_CTRL_PPB_CHECK_SC nextState_brkh_cusCTRL_PPB_CHECK_SC)
{
  if (willBeInState(nextState_brkh_cusCTRL_PPB_CHECK_SC , PPB_READY)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_PPB_CHECK_SC , PPB_READY)) {
      /* Enter-state reaction of State: CTRL_PPB_CHECK_SC:PPB_READY */
      BRKH_SetEmergencyBrakeStatus(TRUE);
    }
  }
  if (willBeInState(nextState_brkh_cusCTRL_PPB_CHECK_SC , PPB_CTRL)) {
    if (isNotStayingInState(staySame_brkh_cusCTRL_PPB_CHECK_SC , PPB_CTRL)) {
      /* Enter-state reaction of State: CTRL_PPB_CHECK_SC:PPB_CTRL */
      A_PPB_PARKMAN_BRAKE_STATE = BRAKING_STATE_PPB_MODE;
      A_PPB_PARK_GUIDANCE_STATE = PARK_GUIDANCE_STATE_INACTIVE_LONG_ACTIVE;
      A_PPB_PARK_CONTROL_MODE_REQUEST = PARK_CTRL_MODE_RQ_IDLE;
      A_PPB_PARK_PARKTYPE = PARK_PARKTYPE_IDLE;
      if ( (LI_BRKH_APPL_EMERGENCY_REQUEST && A_APC_CTRL_ACTIVE)) {
        A_PPB_PARK_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
      }
      else {
        A_PPB_PARK_REMAINING_DISTANCE = LI_DAPM_GET_DISTANCE_TO_COLLISION;
      }
      A_PPB_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_IDLE;
      A_PPB_VEHICLE_SPEED_REQUEST = LI_COMH_REAL_TIME_VHCL_SPEED;
      if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
        A_PPB_PARK_PARKSTAT = PARK_PARK_STAT_FWD_STROKE;
      }
      else {
        if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
          A_PPB_PARK_PARKSTAT = PARK_PARK_STAT_BACK_STROKE;
        }
      }
    }
    if (willBeInState(nextState_brkh_cusCTRL_PPB_CHECK_SC , CTRL_WAIT)) {
      if (isNotStayingInState(staySame_brkh_cusCTRL_PPB_CHECK_SC , CTRL_WAIT)) {
        currentTick_brkh_cus = MS_COUNTER;
        INSTALL_TIMEOUT(tm_999999997_brkh_cus,1000 / C_TASK_CYCLIC_TIME,MS_COUNTER_INDEX);
      }
    }
  }
}



/* This function executes CTRL_PPB_CHECK_SC (Statechart) */
/* ----------------------------------------------------------------------------------------------------------------------     */
/* |      Chart      |     From      |      To       |                 Transition Label                 |Transition Note| */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|               |EMERGENCY_IDLE |                                                  |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|PPB_CTRL       |EMERGENCY_IDLE |(ch(LI_COMH_CAR_GEAR_LEVER_POSITION) and [false==L|               | */
/* |                 |               |               |I_BRKH_APPL_EMERGENCY_REQUEST]) or [(true==LI_DAPM|               | */
/* |                 |               |               |_IS_VEHICLE_STANDSTILL) and (((LI_COMH_GET_BRAKE_P|               | */
/* |                 |               |               |EDAL_TORQUE>=(A_INITIAL_BRAKE_TORQUE+C_RELEASE_TOR|               | */
/* |                 |               |               |QUE)) and (A_PPB_PARK_REMAINING_DISTANCE_STATE==RE|               | */
/* |                 |               |               |M_DIST_STAT_PPB)) or ((false==LI_BRKH_APPL_EMERGEN|               | */
/* |                 |               |               |CY_REQUEST and A_PPB_PARK_REMAINING_DISTANCE_STATE|               | */
/* |                 |               |               |==REM_DIST_STAT_NEW_MEASURE)))]/A_PPB_CTRL_ACTIVE=|               | */
/* |                 |               |               |false;A_PPB_PARKMAN_BRAKE_STATE=BRAKING_STATE_PARK|               | */
/* |                 |               |               |MAN_INACTIV;A_PPB_PARK_REMAINING_DISTANCE_STATE=RE|               | */
/* |                 |               |               |M_DIST_STAT_NO_STEP;A_PPB_PARK_REMAINING_DISTANCE=|               | */
/* |                 |               |               |C_MAX_REMAINING_DISTANCE;A_PPB_PARK_GUIDANCE_STATE|               | */
/* |                 |               |               |=PARK_GUIDANCE_STATE_INACTIVE_LONG_ACTIVE;A_PPB_PA|               | */
/* |                 |               |               |RK_CONTROL_MODE_REQUEST=PARK_CTRL_MODE_RQ_IDLE;A_P|               | */
/* |                 |               |               |PB_PARK_PARKTYPE=PARK_PARKTYPE_IDLE;A_PPB_VEHICLE_|               | */
/* |                 |               |               |SPEED_REQUEST=C_IDLE_VEHICLE_SPEED_REQUEST;A_PPB_P|               | */
/* |                 |               |               |ARK_DRIVE_POS_REQUEST=GEAR_POSITION_RQ_IDLE;      |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|EMERGENCY_IDLE |EMERGENCY_READY|[(LI_COMH_PPB_ESP_ENABLE_STATE==true) and (LI_COMH|               | */
/* |                 |               |               |_ESP_INTERVENTION_STATE!=ESP_INTERVENTION_ACTIVE) |               | */
/* |                 |               |               |and (LI_COMH_PARK_FLT_STAT_ESP!=ESP_STATE_ESP_FLT)|               | */
/* |                 |               |               | and ((LI_COMH_ESP_SYSTEM_STATE!=ESP_SYSTEM_STATE_|               | */
/* |                 |               |               |ERROR) and (LI_COMH_ESP_SYSTEM_STATE!=ESP_SYSTEM_S|               | */
/* |                 |               |               |TATE_DIAGNOSTIC))]                                |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|PPB_READY      |EMERGENCY_IDLE |[not ((LI_COMH_PPB_ESP_ENABLE_STATE==true) and (LI|               | */
/* |                 |               |               |_COMH_ESP_INTERVENTION_STATE!=ESP_INTERVENTION_ACT|               | */
/* |                 |               |               |IVE) and (LI_COMH_PARK_FLT_STAT_ESP!=ESP_STATE_ESP|               | */
/* |                 |               |               |_FLT) and ((LI_COMH_ESP_SYSTEM_STATE!=ESP_SYSTEM_S|               | */
/* |                 |               |               |TATE_ERROR) and (LI_COMH_ESP_SYSTEM_STATE!=ESP_SYS|               | */
/* |                 |               |               |TEM_STATE_DIAGNOSTIC)))]                          |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|EMERGENCY_READY|PPB_PREFILL    |[true==LI_DAPM_EMERGENCY_IMMINENT]/A_PPB_PREFILL_A|               | */
/* |                 |               |               |CTIVE=true;                                       |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|EMERGENCY_READY|PPB_CTRL       |[(true==LI_DAPM_EMERGENCY_REQUIRED)]/A_INITIAL_BRA|               | */
/* |                 |               |               |KE_TORQUE=LI_COMH_GET_BRAKE_PEDAL_TORQUE;A_PPB_CTR|               | */
/* |                 |               |               |L_ACTIVE=true;A_PPB_PARK_REMAINING_DISTANCE_STATE=|               | */
/* |                 |               |               |REM_DIST_STAT_PPB;                                |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|EMERGENCY_READY|PPB_CTRL       |[(true==LI_BRKH_APPL_EMERGENCY_REQUEST and A_APC_C|               | */
/* |                 |               |               |TRL_ACTIVE)]/A_PPB_CTRL_ACTIVE=true;A_PPB_PARK_REM|               | */
/* |                 |               |               |AINING_DISTANCE_STATE=REM_DIST_STAT_NEW_MEASURE;  |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|PPB_PREFILL    |EMERGENCY_READY|[false==LI_DAPM_EMERGENCY_IMMINENT]/A_PPB_PREFILL_|               | */
/* |                 |               |               |ACTIVE=false;                                     |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|               |EMERGENCY_READY|                                                  |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|PPB_PREFILL    |PPB_CTRL       |[(true==LI_DAPM_EMERGENCY_REQUIRED)]/A_INITIAL_BRA|               | */
/* |                 |               |               |KE_TORQUE=LI_COMH_GET_BRAKE_PEDAL_TORQUE;A_PPB_CTR|               | */
/* |                 |               |               |L_ACTIVE=true;A_PPB_PREFILL_ACTIVE=false;         |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|CTRL_CHECK     |EMERGENCY_IDLE |[BRAKING_STATE_PPB_MODE!=LI_COMH_ESP_BRAKE_STATE a|               | */
/* |                 |               |               |nd A_APC_CTRL_ACTIVE==false]/A_PPB_CTRL_ACTIVE=fal|               | */
/* |                 |               |               |se;A_PPB_PARK_REMAINING_DISTANCE=C_MAX_REMAINING_D|               | */
/* |                 |               |               |ISTANCE;                                          |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|PPB_PREFILL    |PPB_CTRL       |[(true==LI_BRKH_APPL_EMERGENCY_REQUEST and A_APC_C|               | */
/* |                 |               |               |TRL_ACTIVE)]/A_PPB_CTRL_ACTIVE=true;A_PPB_PARK_REM|               | */
/* |                 |               |               |AINING_DISTANCE_STATE=REM_DIST_STAT_RPA;A_PPB_PARK|               | */
/* |                 |               |               |_REMAINING_DISTANCE=0;                            |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|               |CTRL_WAIT      |                                                  |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
/* |CTRL_PPB_CHECK_SC|CTRL_WAIT      |CTRL_CHECK     |dly(1000/C_TASK_CYCLIC_TIME)                      |               | */
/* ---------------------------------------------------------------------------------------------------------------------- */
void
cgDo_CTRL_PPB_CHECK_SC(void)
{
  StateInfo_CTRL_PPB_CHECK_SC nextState_brkh_cusCTRL_PPB_CHECK_SC = 0;
  StateInfo_CTRL_PPB_CHECK_SC staySame_brkh_cusCTRL_PPB_CHECK_SC = 0;
  if ((currentState_CTRL_PPB_CHECK_SC == 0) || inState(currentState_CTRL_PPB_CHECK_SC , PPB)) {
    if ((
    ((currentState_CTRL_PPB_CHECK_SC == 0) || inLeafState(currentState_CTRL_PPB_CHECK_SC , DefaultOf_PPB, StateInfo_CTRL_PPB_CHECK_SC)))) {
      /* Transition to State: CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */
        
      nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_EMERGENCY_IDLE/* CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */  ;
    }
    else if (inState(currentState_CTRL_PPB_CHECK_SC , STATE_15st1)) {
      staySame_brkh_cusCTRL_PPB_CHECK_SC = FS_STATE_15st1;
      if ((
      inLeafState(currentState_CTRL_PPB_CHECK_SC , EMERGENCY_READY, StateInfo_CTRL_PPB_CHECK_SC)
       && (( (LI_DAPM_EMERGENCY_REQUIRED))))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:CTRL_WAIT */
         /*@Transition Priority:1*/ 
        A_INITIAL_BRAKE_TORQUE = LI_COMH_GET_BRAKE_PEDAL_TORQUE;
        A_PPB_CTRL_ACTIVE = LOGICAL_CAST(TRUE);
        A_PPB_PARK_REMAINING_DISTANCE_STATE = REM_DIST_STAT_PPB;
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_CTRL_WAIT/* CTRL_PPB_CHECK_SC:CTRL_WAIT */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_PPB_CHECK_SC , EMERGENCY_READY, StateInfo_CTRL_PPB_CHECK_SC)
       && (( (LI_BRKH_APPL_EMERGENCY_REQUEST && A_APC_CTRL_ACTIVE))))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:CTRL_WAIT */
         /*@Transition Priority:2*/ 
        A_PPB_CTRL_ACTIVE = LOGICAL_CAST(TRUE);
        A_PPB_PARK_REMAINING_DISTANCE_STATE = REM_DIST_STAT_NEW_MEASURE;
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_CTRL_WAIT/* CTRL_PPB_CHECK_SC:CTRL_WAIT */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_PPB_CHECK_SC , EMERGENCY_READY, StateInfo_CTRL_PPB_CHECK_SC)
       && ((LI_DAPM_EMERGENCY_IMMINENT)))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:PPB_PREFILL */
         /*@Transition Priority:3*/ 
        lval_A_PPB_PREFILL_ACTIVE = LOGICAL_CAST(TRUE);
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_PPB_PREFILL/* CTRL_PPB_CHECK_SC:PPB_PREFILL */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_PPB_CHECK_SC , PPB_PREFILL, StateInfo_CTRL_PPB_CHECK_SC)
       && (( (LI_DAPM_EMERGENCY_REQUIRED))))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:CTRL_WAIT */
         /*@Transition Priority:1*/ 
        A_INITIAL_BRAKE_TORQUE = LI_COMH_GET_BRAKE_PEDAL_TORQUE;
        A_PPB_CTRL_ACTIVE = LOGICAL_CAST(TRUE);
        lval_A_PPB_PREFILL_ACTIVE = LOGICAL_CAST(FALSE);
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_CTRL_WAIT/* CTRL_PPB_CHECK_SC:CTRL_WAIT */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_PPB_CHECK_SC , PPB_PREFILL, StateInfo_CTRL_PPB_CHECK_SC)
       && (( (LI_BRKH_APPL_EMERGENCY_REQUEST && A_APC_CTRL_ACTIVE))))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:CTRL_WAIT */
         /*@Transition Priority:2*/ 
        A_PPB_CTRL_ACTIVE = LOGICAL_CAST(TRUE);
        A_PPB_PARK_REMAINING_DISTANCE_STATE = REM_DIST_STAT_RPA;
        A_PPB_PARK_REMAINING_DISTANCE = 0;
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_CTRL_WAIT/* CTRL_PPB_CHECK_SC:CTRL_WAIT */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_PPB_CHECK_SC , CTRL_CHECK, StateInfo_CTRL_PPB_CHECK_SC)
       && ((BRAKING_STATE_PPB_MODE != LI_COMH_ESP_BRAKE_STATE && !A_APC_CTRL_ACTIVE)))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */
          
        A_PPB_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
        A_PPB_PARK_REMAINING_DISTANCE = C_MAX_REMAINING_DISTANCE;
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_EMERGENCY_IDLE/* CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_PPB_CHECK_SC , EMERGENCY_IDLE, StateInfo_CTRL_PPB_CHECK_SC)
       && (( (LI_COMH_PPB_ESP_ENABLE_STATE) &&  (LI_COMH_ESP_INTERVENTION_STATE != ESP_INTERVENTION_ACTIVE) 
      &&  (LI_COMH_PARK_FLT_STAT_ESP != ESP_STATE_ESP_FLT) &&  ( (LI_COMH_ESP_SYSTEM_STATE != ESP_SYSTEM_STATE_ERROR) &&  (LI_COMH_ESP_SYSTEM_STATE != ESP_SYSTEM_STATE_DIAGNOSTIC)))))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:EMERGENCY_READY */
          
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_EMERGENCY_READY/* CTRL_PPB_CHECK_SC:EMERGENCY_READY */  ;
      }
      else if ((inState(currentState_CTRL_PPB_CHECK_SC , PPB_CTRL)
       && ( ( (CHANGED(LI_COMH_CAR_GEAR_LEVER_POSITION)) &&  ((!LI_BRKH_APPL_EMERGENCY_REQUEST))) 
      || ( (LI_DAPM_IS_VEHICLE_STANDSTILL) &&  ( ( (LI_COMH_GET_BRAKE_PEDAL_TORQUE >=  (A_INITIAL_BRAKE_TORQUE + C_RELEASE_TORQUE)) 
      &&  (A_PPB_PARK_REMAINING_DISTANCE_STATE == REM_DIST_STAT_PPB)) 
      ||  ( (!LI_BRKH_APPL_EMERGENCY_REQUEST && A_PPB_PARK_REMAINING_DISTANCE_STATE == REM_DIST_STAT_NEW_MEASURE))))))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */
          
        A_PPB_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
        A_PPB_PARKMAN_BRAKE_STATE = BRAKING_STATE_PARKMAN_INACTIV;
        A_PPB_PARK_REMAINING_DISTANCE_STATE = REM_DIST_STAT_NO_STEP;
        A_PPB_PARK_REMAINING_DISTANCE = C_MAX_REMAINING_DISTANCE;
        A_PPB_PARK_GUIDANCE_STATE = PARK_GUIDANCE_STATE_INACTIVE_LONG_ACTIVE;
        A_PPB_PARK_CONTROL_MODE_REQUEST = PARK_CTRL_MODE_RQ_IDLE;
        A_PPB_PARK_PARKTYPE = PARK_PARKTYPE_IDLE;
        A_PPB_VEHICLE_SPEED_REQUEST = C_IDLE_VEHICLE_SPEED_REQUEST;
        A_PPB_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_IDLE;
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_EMERGENCY_IDLE/* CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */  ;
      }
      else if ((
      inLeafState(currentState_CTRL_PPB_CHECK_SC , PPB_PREFILL, StateInfo_CTRL_PPB_CHECK_SC)
       && ((!LI_DAPM_EMERGENCY_IMMINENT)))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:EMERGENCY_READY */
         /*@Transition Priority:3*/ 
        lval_A_PPB_PREFILL_ACTIVE = LOGICAL_CAST(FALSE);
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_EMERGENCY_READY/* CTRL_PPB_CHECK_SC:EMERGENCY_READY */  ;
      }
      else if ((inState(currentState_CTRL_PPB_CHECK_SC , PPB_READY)
       && ((! ( (LI_COMH_PPB_ESP_ENABLE_STATE) &&  (LI_COMH_ESP_INTERVENTION_STATE != ESP_INTERVENTION_ACTIVE) 
      &&  (LI_COMH_PARK_FLT_STAT_ESP != ESP_STATE_ESP_FLT) &&  ( (LI_COMH_ESP_SYSTEM_STATE != ESP_SYSTEM_STATE_ERROR) &&  (LI_COMH_ESP_SYSTEM_STATE != ESP_SYSTEM_STATE_DIAGNOSTIC))))))) {
        /* Transition to State: CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */
          
        nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_EMERGENCY_IDLE/* CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */  ;
      }
      else if (inState(currentState_CTRL_PPB_CHECK_SC , PPB_READY)) {
        staySame_brkh_cusCTRL_PPB_CHECK_SC = FS_PPB_READY;
        if ((
        inLeafState(currentState_CTRL_PPB_CHECK_SC , DefaultOf_PPB_READY, StateInfo_CTRL_PPB_CHECK_SC))) {
          /* Transition to State: CTRL_PPB_CHECK_SC:EMERGENCY_READY */
            
          nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_EMERGENCY_READY/* CTRL_PPB_CHECK_SC:EMERGENCY_READY */  ;
        }
      }
      else if (inState(currentState_CTRL_PPB_CHECK_SC , PPB_CTRL)) {
        staySame_brkh_cusCTRL_PPB_CHECK_SC = FS_PPB_CTRL;
        /* In-state reaction and actions of State: CTRL_PPB_CHECK_SC:PPB_CTRL */
        A_PPB_PARKMAN_BRAKE_STATE = BRAKING_STATE_PPB_MODE;
        A_PPB_PARK_GUIDANCE_STATE = PARK_GUIDANCE_STATE_INACTIVE_LONG_ACTIVE;
        A_PPB_PARK_CONTROL_MODE_REQUEST = PARK_CTRL_MODE_RQ_IDLE;
        A_PPB_PARK_PARKTYPE = PARK_PARKTYPE_IDLE;
        if ( (LI_BRKH_APPL_EMERGENCY_REQUEST && A_APC_CTRL_ACTIVE)) {
          A_PPB_PARK_REMAINING_DISTANCE = C_NULL_REMAINING_DISTANCE;
        }
        else {
          A_PPB_PARK_REMAINING_DISTANCE = LI_DAPM_GET_DISTANCE_TO_COLLISION;
        }
        A_PPB_PARK_DRIVE_POS_REQUEST = GEAR_POSITION_RQ_IDLE;
        A_PPB_VEHICLE_SPEED_REQUEST = LI_COMH_REAL_TIME_VHCL_SPEED;
        if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_D)) {
          A_PPB_PARK_PARKSTAT = PARK_PARK_STAT_FWD_STROKE;
        }
        else {
          if ( (LI_COMH_CAR_GEAR_LEVER_POSITION == GEAR_POSITION_R)) {
            A_PPB_PARK_PARKSTAT = PARK_PARK_STAT_BACK_STROKE;
          }
        }
        if ((
        inLeafState(currentState_CTRL_PPB_CHECK_SC , CTRL_WAIT, StateInfo_CTRL_PPB_CHECK_SC)
         && (tm_999999997_brkh_cus))) {
          /* Transition to State: CTRL_PPB_CHECK_SC:CTRL_CHECK */
            
          nextState_brkh_cusCTRL_PPB_CHECK_SC = FS_CTRL_CHECK/* CTRL_PPB_CHECK_SC:CTRL_CHECK */  ;
        }
        else if (inLeafState(currentState_CTRL_PPB_CHECK_SC , CTRL_WAIT, StateInfo_CTRL_PPB_CHECK_SC)) {
          staySame_brkh_cusCTRL_PPB_CHECK_SC = FS_CTRL_WAIT;
        }
        else {
        }
      }
      else if (inLeafState(currentState_CTRL_PPB_CHECK_SC , EMERGENCY_IDLE, StateInfo_CTRL_PPB_CHECK_SC)) {
        /* In-state reaction and actions of State: CTRL_PPB_CHECK_SC:EMERGENCY_IDLE */
        BRKH_SetEmergencyBrakeStatus(FALSE);
      }
      else {
      }
    }
    else {
    }
  }
  if (nextState_brkh_cusCTRL_PPB_CHECK_SC != 0) {
    cgEnterActions_CTRL_PPB_CHECK_SC(staySame_brkh_cusCTRL_PPB_CHECK_SC, nextState_brkh_cusCTRL_PPB_CHECK_SC);
    currentState_CTRL_PPB_CHECK_SC = nextState_brkh_cusCTRL_PPB_CHECK_SC;
  }
}




/* This function executes the Activity: BRKH_INPUTS_AC:CTRL_PPB_CHECK_SC */
void
cgActivity_CTRL_PPB_CHECK_SC(void)
{
  /* BRKH_INPUTS_AC:CTRL_PPB_CHECK_SC */  
  cgDo_CTRL_PPB_CHECK_SC();
}



/* This function executes the Activity: BRKH_INPUTS_AC:REM_DIST_AC */
void
cgActivity_REM_DIST_AC(void)
{
  /* BRKH_INPUTS_AC:REM_DIST_AC */  
  if ((cgGlobalFlags_brkh_cus & BITAC_REM_DIST_AC) != 0) {
    cgActivity_REM_DIST();
  }
}



/* This function calculates the states that were entered, and executes */
/* the appropriate enter-state reaction in chart: UZL_CONTROL_SC */
void
cgEnterActions_UZL_CONTROL_SC(StateInfo_UZL_CONTROL_SC staySame_brkh_cusUZL_CONTROL_SC, StateInfo_UZL_CONTROL_SC nextState_brkh_cusUZL_CONTROL_SC)
{
  if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , UZL_CONTROL_TERMINATE_TO_FOLLOW_UP)) {
    if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , UZL_CONTROL_TERMINATE_TO_FOLLOW_UP)) {
      /* Enter-state reaction of State: UZL_CONTROL_SC:UZL_CONTROL_TERMINATE_TO_FOLLOW_UP */
      GENERATE_EVENT(EVT_UZL_FOLLOW_UP);
    }
  }
  else if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , UZL_PASSIVE)) {
    if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , UZL_PASSIVE)) {
      currentTick_brkh_cus = MS_COUNTER;
      INSTALL_TIMEOUT(tm_999999991_brkh_cus,T_UZL_ACTIVATION_TOL,MS_COUNTER_INDEX);
    }
  }
  else if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , UZL_ABORT)) {
    if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , UZL_ABORT)) {
      /* Enter-state reaction of State: UZL_CONTROL_SC:UZL_ABORT */
      GENERATE_EVENT(EVT_UZL_FOLLOW_UP);
    }
  }
  else if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , PM_PPB_PREFILLst1)) {
    if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , PM_PPB_PREFILLst1)) {
      /* Enter-state reaction of State: UZL_CONTROL_SC:PM_PPB_PREFILL */
      BRKH_SetParkDangerMdRq(DANGER_MODE_ACTIVE);
    }
  }
  else if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , PM_PPB_CTRL)) {
    if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , PM_PPB_CTRL)) {
      /* Enter-state reaction of State: UZL_CONTROL_SC:PM_PPB_CTRL */
      A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_PPB_CTRL;
      A_LAST_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_PPB_CTRL;
      BRKH_SetParkBrkStat(A_PPB_PARKMAN_BRAKE_STATE);
      if ( (A_PPB_PARK_REMAINING_DISTANCE > C_MAX_REMAINING_DISTANCE)) {
        BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
      }
      else {
        BRKH_SetRemainDist(A_PPB_PARK_REMAINING_DISTANCE);
      }
      BRKH_SetRemainDistStat(A_PPB_PARK_REMAINING_DISTANCE_STATE);
      BRKH_SetVehSpdRq(A_PPB_VEHICLE_SPEED_REQUEST);
      BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
      BRKH_SetParkCtrlMdRq(A_PPB_PARK_CONTROL_MODE_REQUEST);
      BRKH_SetParkParkType(A_PPB_PARK_PARKTYPE);
      BRKH_SetParkParkStat(A_PPB_PARK_PARKSTAT);
      BRKH_SetGearPosRq(A_PPB_PARK_DRIVE_POS_REQUEST);
      BRKH_SetParkBrkMdRq(PARK_BRK_MODE_COMFORT_STOP);
    }
  }
  else if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , APC_CTRL)) {
    if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , APC_CTRL)) {
      /* Enter-state reaction of State: PM_APC_CTRL:APC_CTRL */
      A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_APC_CTRL;
      A_LAST_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_APC_CTRL;
      BRKH_SetParkParkStat(A_APC_PARKMAN_STATUS);
      BRKH_SetParkBrkStat(A_APC_PARKMAN_BRAKE_STATE);
      BRKH_SetParkParkType(A_APC_PARKMAN_PARKTYPE);
      BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_ACTIVE);
      BRKH_SetVehSpdRq(C_APC_PARK_VEHICLE_SPEED_RQ_MAX);
      BRKH_SetGearPosRq(A_APC_PARK_DRIVE_POS_REQUEST);
      BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
      BRKH_SetParkBrkMdRq(PARK_BRK_MODE_COMFORT_STOP);
    }
    if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , PM_PPB_PREFILLst2)) {
      if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , PM_PPB_PREFILLst2)) {
        /* Enter-state reaction of State: PM_APC_CTRL:PM_PPB_PREFILL */
        BRKH_SetParkDangerMdRq(DANGER_MODE_ACTIVE);
      }
    }
    else if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , APC_PPB_CTRL)) {
      if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , APC_PPB_CTRL)) {
        /* Enter-state reaction of State: PM_APC_CTRL:APC_PPB_CTRL */
        if ( (LI_BRKH_APPL_EMERGENCY_REQUEST && A_APC_CTRL_ACTIVE)) {
          BRKH_SetRemainDist(C_NULL_REMAINING_DISTANCE);
          if ( ( (LI_COMH_CAR_CURRENT_GEAR == LI_COMH_CAR_GEAR_LEVER_POSITION) &&  (LI_COMH_CAR_GEAR_LEVER_POSITION != GEAR_POSITION_N))) {
            BRKH_SetRemainDistStat(A_PPB_PARK_REMAINING_DISTANCE_STATE);
          }
          else {
            BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
          }
        }
        else {
          BRKH_SetRemainDistStat(A_PPB_PARK_REMAINING_DISTANCE_STATE);
          if ( (A_PARK_STYLE == PARK_OUT &&  (A_APC_REMAINING_DISTANCE > 2000))) {
            BRKH_SetRemainDist(2000);
          }
          else {
            BRKH_SetRemainDist(A_APC_REMAINING_DISTANCE);
          }
        }
      }
    }
    else if (willBeInState(nextState_brkh_cusUZL_CONTROL_SC , APC_ONLY_CONTROL)) {
      if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , APC_ONLY_CONTROL)) {
        /* Enter-state reaction of State: PM_APC_CTRL:APC_ONLY_CONTROL */
        if ( (A_PARK_STYLE == PARK_OUT &&  (A_APC_REMAINING_DISTANCE > 2000))) {
          BRKH_SetRemainDist(2000);
        }
        else {
          BRKH_SetRemainDist(A_APC_REMAINING_DISTANCE);
        }
        BRKH_SetRemainDistStat(A_APC_REMAINING_DISTANCE_STATE);
        BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
      }
    }
    else {
    }
  }
  else {
  }
}



/* This function calculates the states that are exited, and executes */
/* the appropriate exit-state reaction in chart: UZL_CONTROL_SC */
void
cgExitActions_UZL_CONTROL_SC(StateInfo_UZL_CONTROL_SC staySame_brkh_cusUZL_CONTROL_SC)
{
  if (wasInState(currentState_UZL_CONTROL_SC , PM_PPB_PREFILLst1)) {
    if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , PM_PPB_PREFILLst1)) {
      /* Exit-state reaction of State: UZL_CONTROL_SC:PM_PPB_PREFILL */
      BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
    }
  }
  else if (wasInState(currentState_UZL_CONTROL_SC , PM_PPB_PREFILLst2)) {
    if (isNotStayingInState(staySame_brkh_cusUZL_CONTROL_SC , PM_PPB_PREFILLst2)) {
      /* Exit-state reaction of State: PM_APC_CTRL:PM_PPB_PREFILL */
      BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
    }
  }
  else {
  }
}



/* This function executes UZL_CONTROL_SC (Statechart) */
/* ----------------------------------------------------------------------------------------------------------------------------------------     */
/* |    Chart     |      From       |                To                |                 Transition Label                 |Transition Note| */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |PM_APC_CTRL   |                 |APC_ONLY_CONTROL                  |                                                  |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |PM_APC_CTRL   |APC_PPB_CONTROL  |APC_ONLY_CONTROL                  |[A_PPB_CTRL_ACTIVE==false and A_PPB_PREFILL_ACTIVE|               | */
/* |              |                 |                                  |==false]                                          |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |PM_APC_CTRL   |APC_ONLY_CONTROL |APC_PPB_CONTROL                   |[A_PPB_CTRL_ACTIVE or A_PPB_PREFILL_ACTIVE]       |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |PM_APC_CTRL   |PM_PPB_PREFILL   |APC_PPB_CTRL                      |[A_PPB_CTRL_ACTIVE]                               |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|PM_PPB           |UZL_CONTROL_TERMINATE_TO_FOLLOW_UP|[A_PPB_CTRL_ACTIVE==false]                        |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|UZL_ACTIVE_SWITCH|PM_APC_CTRL                       |[A_APC_CTRL_ACTIVE]                               |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|UZL_ACTIVE_SWITCH|PM_PPB                            |[(A_PPB_CTRL_ACTIVE or A_PPB_PREFILL_ACTIVE)]     |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|                 |UZL_ACTIVE_SWITCH                 |                                                  |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|PM_PPB           |PM_APC_CTRL                       |[A_APC_CTRL_ACTIVE]                               |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|PM_APC_CTRL      |UZL_CONTROL_TERMINATE_TO_FOLLOW_UP|[A_APC_CTRL_ACTIVE==false]                        |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|PM_PPB_PREFILL   |PM_PPB_CTRL                       |[A_PPB_CTRL_ACTIVE]                               |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|UZL_PASSIVE      |UZL_ABORT                         |dly(T_UZL_ACTIVATION_TOL)                         |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|                 |UZL_PASSIVE                       |                                                  |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |PM_APC_CTRL   |                 |PM_PPB_PREFILL                    |[A_PPB_PREFILL_ACTIVE]                            |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |PM_APC_CTRL   |                 |APC_PPB_CTRL                      |[A_PPB_CTRL_ACTIVE and A_PPB_PREFILL_ACTIVE==false|               | */
/* |              |                 |                                  |]                                                 |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|                 |PM_PPB_CTRL                       |[(A_PPB_CTRL_ACTIVE and A_PPB_PREFILL_ACTIVE==fals|               | */
/* |              |                 |                                  |e)]                                               |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
/* |UZL_CONTROL_SC|                 |PM_PPB_PREFILL                    |[A_PPB_PREFILL_ACTIVE==true]                      |               | */
/* ---------------------------------------------------------------------------------------------------------------------------------------- */
void
cgDo_UZL_CONTROL_SC(void)
{
  StateInfo_UZL_CONTROL_SC nextState_brkh_cusUZL_CONTROL_SC = 0;
  StateInfo_UZL_CONTROL_SC staySame_brkh_cusUZL_CONTROL_SC = 0;
  staySame_brkh_cusUZL_CONTROL_SC = FS_Chart_UZL_CONTROL_SC;
  if ((
  ((currentState_UZL_CONTROL_SC == 0) || inLeafState(currentState_UZL_CONTROL_SC , DefaultOf_Chart_UZL_CONTROL_SC, StateInfo_UZL_CONTROL_SC)))) {
    /* Transition to State: UZL_CONTROL_SC:UZL_PASSIVE */
      
    nextState_brkh_cusUZL_CONTROL_SC = FS_UZL_PASSIVE/* UZL_CONTROL_SC:UZL_PASSIVE */  ;
  }
  else if (inState(currentState_UZL_CONTROL_SC , STATE_6)) {
    staySame_brkh_cusUZL_CONTROL_SC = FS_STATE_6;
    if ((inState(currentState_UZL_CONTROL_SC , PM_PPB) && ((A_APC_CTRL_ACTIVE)))) {
      /* Transition to State: PM_APC_CTRL:APC_ONLY_CONTROL */
       /*@Transition Priority:1*/ 
      nextState_brkh_cusUZL_CONTROL_SC = FS_APC_ONLY_CONTROL/* PM_APC_CTRL:APC_ONLY_CONTROL *//* @Traceability :Sys_APC_PTS_Logic_210;Sys_APC_PTS_Logic_211;Sys_APC_PTS_Logic_4096;Sys_APC_PTS_Logic_6471;*/  ;
    }
    else if ((inState(currentState_UZL_CONTROL_SC , PM_PPB) && ((!A_PPB_CTRL_ACTIVE)))) {
      /* Transition to State: UZL_CONTROL_SC:UZL_CONTROL_TERMINATE_TO_FOLLOW_UP */
       /*@Transition Priority:2*/ 
      nextState_brkh_cusUZL_CONTROL_SC = FS_UZL_CONTROL_TERMINATE_TO_FOLLOW_UP/* UZL_CONTROL_SC:UZL_CONTROL_TERMINATE_TO_FOLLOW_UP */  ;
    }
    else if ((
    inState(currentState_UZL_CONTROL_SC , UZL_ACTIVE_SWITCH)
     && ((A_APC_CTRL_ACTIVE)))) {
      /* Transition to State: PM_APC_CTRL:APC_ONLY_CONTROL */
       /*@Transition Priority:1*/ 
      nextState_brkh_cusUZL_CONTROL_SC = FS_APC_ONLY_CONTROL/* PM_APC_CTRL:APC_ONLY_CONTROL *//* @Traceability :Sys_APC_PTS_Logic_210;Sys_APC_PTS_Logic_211;Sys_APC_PTS_Logic_4096;Sys_APC_PTS_Logic_6471;*/  ;
    }
    else if ((inState(currentState_UZL_CONTROL_SC , APC_CTRL) && ((!A_APC_CTRL_ACTIVE)))) {
      /* Transition to State: UZL_CONTROL_SC:UZL_CONTROL_TERMINATE_TO_FOLLOW_UP */
        
      nextState_brkh_cusUZL_CONTROL_SC = FS_UZL_CONTROL_TERMINATE_TO_FOLLOW_UP/* UZL_CONTROL_SC:UZL_CONTROL_TERMINATE_TO_FOLLOW_UP */  ;
    }
    else if ((
    inState(currentState_UZL_CONTROL_SC , UZL_ACTIVE_SWITCH)
     && (( (A_PPB_CTRL_ACTIVE || A_PPB_PREFILL_ACTIVE))))) {
      /* Transition to State: Default Of: UZL_CONTROL_SC:PM_PPB */
       /*@Transition Priority:2*/ 
      nextState_brkh_cusUZL_CONTROL_SC = FS_DefaultOf_PM_PPB/* Default Of: UZL_CONTROL_SC:PM_PPB */  ;
    }
    else if (inLeafState(currentState_UZL_CONTROL_SC , UZL_CONTROL_TERMINATE_TO_FOLLOW_UP, StateInfo_UZL_CONTROL_SC)) {
      staySame_brkh_cusUZL_CONTROL_SC = FS_UZL_CONTROL_TERMINATE_TO_FOLLOW_UP;
    }
    else if (inState(currentState_UZL_CONTROL_SC , UZL_ACTIVE_SWITCH)) {
      staySame_brkh_cusUZL_CONTROL_SC = FS_UZL_ACTIVE_SWITCH;
      if ((
      inLeafState(currentState_UZL_CONTROL_SC , UZL_PASSIVE, StateInfo_UZL_CONTROL_SC)
       && (tm_999999991_brkh_cus))) {
        /* Transition to State: UZL_CONTROL_SC:UZL_ABORT */
          
        nextState_brkh_cusUZL_CONTROL_SC = FS_UZL_ABORT/* Leave UZL control if no function becomes active */  ;
      }
      else if (inLeafState(currentState_UZL_CONTROL_SC , UZL_PASSIVE, StateInfo_UZL_CONTROL_SC)) {
        staySame_brkh_cusUZL_CONTROL_SC = FS_UZL_PASSIVE;
      }
      else if (inLeafState(currentState_UZL_CONTROL_SC , UZL_ABORT, StateInfo_UZL_CONTROL_SC)) {
        staySame_brkh_cusUZL_CONTROL_SC = FS_UZL_ABORT;
      }
      else {
      }
    }
    else if (inState(currentState_UZL_CONTROL_SC , PM_PPB)) {
      staySame_brkh_cusUZL_CONTROL_SC = FS_PM_PPB;
      if ((
      inLeafState(currentState_UZL_CONTROL_SC , DefaultOf_PM_PPB, StateInfo_UZL_CONTROL_SC)
       && (( (A_PPB_CTRL_ACTIVE && !A_PPB_PREFILL_ACTIVE)))) || (
      inLeafState(currentState_UZL_CONTROL_SC , PM_PPB_PREFILLst1, StateInfo_UZL_CONTROL_SC)
       && ((A_PPB_CTRL_ACTIVE)))) {
        /* Transition to State: UZL_CONTROL_SC:PM_PPB_CTRL */
          
          
        nextState_brkh_cusUZL_CONTROL_SC = FS_PM_PPB_CTRL/* UZL_CONTROL_SC:PM_PPB_CTRL *//* @Traceability :Sys_APC_PTS_Logic_222;Sys_APC_PTS_Logic_223;Sys_APC_PTS_Logic_224;Sys_APC_PTS_Logic_225;Sys_APC_PTS_Logic_227*/  ;
      }
      else if ((
      inLeafState(currentState_UZL_CONTROL_SC , DefaultOf_PM_PPB, StateInfo_UZL_CONTROL_SC)
       && ((A_PPB_PREFILL_ACTIVE)))) {
        /* Transition to State: UZL_CONTROL_SC:PM_PPB_PREFILL */
          
        nextState_brkh_cusUZL_CONTROL_SC = FS_PM_PPB_PREFILLst1/* UZL_CONTROL_SC:PM_PPB_PREFILL */  ;
      }
      else if (inLeafState(currentState_UZL_CONTROL_SC , PM_PPB_PREFILLst1, StateInfo_UZL_CONTROL_SC)) {
        staySame_brkh_cusUZL_CONTROL_SC = FS_PM_PPB_PREFILLst1;
      }
      else if (inLeafState(currentState_UZL_CONTROL_SC , PM_PPB_CTRL, StateInfo_UZL_CONTROL_SC)) {
        staySame_brkh_cusUZL_CONTROL_SC = FS_PM_PPB_CTRL;
        /* In-state reaction and actions of State: UZL_CONTROL_SC:PM_PPB_CTRL */
        A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_PPB_CTRL;
        A_LAST_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_PPB_CTRL;
        BRKH_SetParkBrkStat(A_PPB_PARKMAN_BRAKE_STATE);
        if ( (A_PPB_PARK_REMAINING_DISTANCE > C_MAX_REMAINING_DISTANCE)) {
          BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
        }
        else {
          BRKH_SetRemainDist(A_PPB_PARK_REMAINING_DISTANCE);
        }
        BRKH_SetRemainDistStat(A_PPB_PARK_REMAINING_DISTANCE_STATE);
        BRKH_SetVehSpdRq(A_PPB_VEHICLE_SPEED_REQUEST);
        BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
        BRKH_SetParkCtrlMdRq(A_PPB_PARK_CONTROL_MODE_REQUEST);
        BRKH_SetParkParkType(A_PPB_PARK_PARKTYPE);
        BRKH_SetParkParkStat(A_PPB_PARK_PARKSTAT);
        BRKH_SetGearPosRq(A_PPB_PARK_DRIVE_POS_REQUEST);
      }
      else {
      }
    }
    else if (inState(currentState_UZL_CONTROL_SC , APC_CTRL)) {
      staySame_brkh_cusUZL_CONTROL_SC = FS_APC_CTRL;
      /* In-state reaction and actions of State: PM_APC_CTRL:APC_CTRL */
      A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_APC_CTRL;
      A_LAST_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_APC_CTRL;
      BRKH_SetParkParkStat(A_APC_PARKMAN_STATUS);
      BRKH_SetParkBrkStat(A_APC_PARKMAN_BRAKE_STATE);
      BRKH_SetParkParkType(A_APC_PARKMAN_PARKTYPE);
      BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_ACTIVE);
      if ( (!BRKH_LimitMaxSpeed())) {
        BRKH_SetVehSpdRq(C_APC_PARK_VEHICLE_SPEED_RQ_MAX);
      }
      else {
        BRKH_SetVehSpdRq(C_REDUCED_VEHICLE_SPEED_REQUEST);
      }
      BRKH_SetGearPosRq(A_APC_PARK_DRIVE_POS_REQUEST);
      BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
      BRKH_SetParkBrkMdRq(PARK_BRK_MODE_COMFORT_STOP);
      if ((
      inLeafState(currentState_UZL_CONTROL_SC , APC_ONLY_CONTROL, StateInfo_UZL_CONTROL_SC)
       && ((A_PPB_CTRL_ACTIVE || A_PPB_PREFILL_ACTIVE)))) {
        /* Transition to State: Default Of: PM_APC_CTRL:APC_PPB_CONTROL */
         /*@Transition Priority:1*/ 
        nextState_brkh_cusUZL_CONTROL_SC = FS_DefaultOf_APC_PPB_CONTROL/* Default Of: PM_APC_CTRL:APC_PPB_CONTROL */  ;
      }
      else if ((
      inState(currentState_UZL_CONTROL_SC , APC_PPB_CONTROL)
       && ((!A_PPB_CTRL_ACTIVE && !A_PPB_PREFILL_ACTIVE)))) {
        /* Transition to State: PM_APC_CTRL:APC_ONLY_CONTROL */
         /*@Transition Priority:2*/ 
        nextState_brkh_cusUZL_CONTROL_SC = FS_APC_ONLY_CONTROL/* PM_APC_CTRL:APC_ONLY_CONTROL *//* @Traceability :Sys_APC_PTS_Logic_210;Sys_APC_PTS_Logic_211;Sys_APC_PTS_Logic_4096;Sys_APC_PTS_Logic_6471;*/  ;
      }
      else if (inState(currentState_UZL_CONTROL_SC , APC_PPB_CONTROL)) {
        staySame_brkh_cusUZL_CONTROL_SC = FS_APC_PPB_CONTROL;
        if ((
        inLeafState(currentState_UZL_CONTROL_SC , DefaultOf_APC_PPB_CONTROL, StateInfo_UZL_CONTROL_SC)
         && ((A_PPB_CTRL_ACTIVE && !A_PPB_PREFILL_ACTIVE))) || (
        inLeafState(currentState_UZL_CONTROL_SC , PM_PPB_PREFILLst2, StateInfo_UZL_CONTROL_SC)
         && ((A_PPB_CTRL_ACTIVE)))) {
          /* Transition to State: PM_APC_CTRL:APC_PPB_CTRL */
            
            
          nextState_brkh_cusUZL_CONTROL_SC = FS_APC_PPB_CTRL/* PM_APC_CTRL:APC_PPB_CTRL *//* @Traceability :Sys_APC_PTS_Logic_6471*/  ;
        }
        else if ((
        inLeafState(currentState_UZL_CONTROL_SC , DefaultOf_APC_PPB_CONTROL, StateInfo_UZL_CONTROL_SC)
         && ((A_PPB_PREFILL_ACTIVE)))) {
          /* Transition to State: PM_APC_CTRL:PM_PPB_PREFILL */
            
          nextState_brkh_cusUZL_CONTROL_SC = FS_PM_PPB_PREFILLst2/* PM_APC_CTRL:PM_PPB_PREFILL */  ;
        }
        else if (inLeafState(currentState_UZL_CONTROL_SC , PM_PPB_PREFILLst2, StateInfo_UZL_CONTROL_SC)) {
          staySame_brkh_cusUZL_CONTROL_SC = FS_PM_PPB_PREFILLst2;
        }
        else if (inLeafState(currentState_UZL_CONTROL_SC , APC_PPB_CTRL, StateInfo_UZL_CONTROL_SC)) {
          staySame_brkh_cusUZL_CONTROL_SC = FS_APC_PPB_CTRL;
          /* In-state reaction and actions of State: PM_APC_CTRL:APC_PPB_CTRL */
          if ( (LI_BRKH_APPL_EMERGENCY_REQUEST && A_APC_CTRL_ACTIVE)) {
            BRKH_SetRemainDist(C_NULL_REMAINING_DISTANCE);
            if ( ( (LI_COMH_CAR_CURRENT_GEAR == LI_COMH_CAR_GEAR_LEVER_POSITION) &&  (LI_COMH_CAR_GEAR_LEVER_POSITION != GEAR_POSITION_N))) {
              BRKH_SetRemainDistStat(A_PPB_PARK_REMAINING_DISTANCE_STATE);
            }
            else {
              BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
            }
          }
          else {
            BRKH_SetRemainDistStat(A_PPB_PARK_REMAINING_DISTANCE_STATE);
            if ( (A_PARK_STYLE == PARK_OUT &&  (A_APC_REMAINING_DISTANCE > 2000))) {
              BRKH_SetRemainDist(2000);
            }
            else {
              BRKH_SetRemainDist(A_APC_REMAINING_DISTANCE);
            }
          }
        }
        else {
        }
      }
      else if (inLeafState(currentState_UZL_CONTROL_SC , APC_ONLY_CONTROL, StateInfo_UZL_CONTROL_SC)) {
        staySame_brkh_cusUZL_CONTROL_SC = FS_APC_ONLY_CONTROL;
        /* In-state reaction and actions of State: PM_APC_CTRL:APC_ONLY_CONTROL */
        if ( (A_PARK_STYLE == PARK_OUT &&  (A_APC_REMAINING_DISTANCE > 2000))) {
          BRKH_SetRemainDist(2000);
        }
        else {
          BRKH_SetRemainDist(A_APC_REMAINING_DISTANCE);
        }
        BRKH_SetRemainDistStat(A_APC_REMAINING_DISTANCE_STATE);
        BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
      }
      else {
      }
    }
    else {
    }
  }
  else {
  }
  if (nextState_brkh_cusUZL_CONTROL_SC != 0) {
    cgExitActions_UZL_CONTROL_SC(staySame_brkh_cusUZL_CONTROL_SC);
    cgEnterActions_UZL_CONTROL_SC(staySame_brkh_cusUZL_CONTROL_SC, nextState_brkh_cusUZL_CONTROL_SC);
    currentState_UZL_CONTROL_SC = nextState_brkh_cusUZL_CONTROL_SC;
  }
}




/* This function executes the Activity: BRKH_CUS_AC:UZL_CONTROL_SC */
void
cgActivity_UZL_CONTROL_SC(void)
{
  /* BRKH_CUS_AC:UZL_CONTROL_SC */  
  cgDo_UZL_CONTROL_SC();
}



/* This function calculates the states that were entered, and executes */
/* the appropriate enter-state reaction in chart: BRKH_CUS_SC */
void
cgEnterActions_BRKH_CUS_SCcnt1(StateInfo_BRKH_CUS_SCcnt1 staySame_brkh_cusBRKH_CUS_SCcnt1, StateInfo_BRKH_CUS_SCcnt1 nextState_brkh_cusBRKH_CUS_SCcnt1)
{
  if (willBeInState(nextState_brkh_cusBRKH_CUS_SCcnt1 , UZL_CONTROL_ACTIVE)) {
    if (isNotStayingInState(staySame_brkh_cusBRKH_CUS_SCcnt1 , UZL_CONTROL_ACTIVE)) {
      /* Enter-state reaction of State: BRKH_CUS_SC:UZL_CONTROL_ACTIVE */
      start_activity(UZL_CONTROL);
    }
  }
  else if (willBeInState(nextState_brkh_cusBRKH_CUS_SCcnt1 , PM_NA)) {
    if (isNotStayingInState(staySame_brkh_cusBRKH_CUS_SCcnt1 , PM_NA)) {
      /* Enter-state reaction of State: BRKH_CUS_SC:PM_NA */
      A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_NA;
      A_APC_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
      A_PPB_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
      lval_A_PPB_PREFILL_ACTIVE = LOGICAL_CAST(FALSE);
      BRKH_SetParkBrkStat(BRAKING_STATE_PARKMAN_NOT_AVAILABLE);
      BRKH_SetParkParkStat(PARK_PARK_STAT_IDLE);
      BRKH_SetParkParkType(PARK_PARKTYPE_IDLE);
      BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
      BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
      BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
      BRKH_SetParkBrkMdRq(PARK_BRK_MODE_DISABLED);
      BRKH_SetVehSpdRq(C_IDLE_VEHICLE_SPEED_REQUEST);
      BRKH_SetGearPosRq(GEAR_POSITION_RQ_IDLE);
      BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
      BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
    }
  }
  else if (willBeInState(nextState_brkh_cusBRKH_CUS_SCcnt1 , PM_INIT)) {
    if (isNotStayingInState(staySame_brkh_cusBRKH_CUS_SCcnt1 , PM_INIT)) {
      /* Enter-state reaction of State: BRKH_CUS_SC:PM_INIT */
      A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_INIT;
      BRKH_SetParkBrkStat(BRAKING_STATE_PARKMAN_INIT);
      BRKH_SetParkParkType(PARK_PARKTYPE_IDLE);
      BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
      BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
      BRKH_SetGearPosRq(GEAR_POSITION_RQ_IDLE);
      BRKH_SetVehSpdRq(C_IDLE_VEHICLE_SPEED_REQUEST);
      BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
      BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
      BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
      BRKH_SetParkBrkMdRq(PARK_BRK_MODE_COMFORT_STOP);
      BRKH_SetParkParkStat(PARK_PARK_STAT_IDLE);
    }
  }
  else if (willBeInState(nextState_brkh_cusBRKH_CUS_SCcnt1 , PM_INACTIVE)) {
    if (isNotStayingInState(staySame_brkh_cusBRKH_CUS_SCcnt1 , PM_INACTIVE)) {
      /* Enter-state reaction of State: BRKH_CUS_SC:PM_INACTIVE */
      A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_INACTIVE;
      BRKH_SetParkBrkStat(BRAKING_STATE_PARKMAN_INACTIV);
      BRKH_SetParkParkType(A_APC_PARKMAN_PARKTYPE);
      BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
      BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
      BRKH_SetGearPosRq(GEAR_POSITION_RQ_IDLE);
      BRKH_SetVehSpdRq(C_IDLE_VEHICLE_SPEED_REQUEST);
      BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
      BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
      BRKH_SetParkBrkMdRq(PARK_BRK_MODE_COMFORT_STOP);
      BRKH_SetParkParkStat(A_APC_PARKMAN_STATUS);
      BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
    }
  }
  else if (willBeInState(nextState_brkh_cusBRKH_CUS_SCcnt1 , PM_FOLLOW_UP_CONTROL)) {
    if (isNotStayingInState(staySame_brkh_cusBRKH_CUS_SCcnt1 , PM_FOLLOW_UP_CONTROL)) {
      /* Enter-state reaction of State: BRKH_CUS_SC:PM_FOLLOW_UP_CONTROL */
      A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_FOLLOW_UP_CTRL;
      BRKH_SetParkBrkStat(BRAKING_STATE_PARKMAN_INACTIV);
      BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
      BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
      BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
      BRKH_SetParkBrkMdRq(PARK_BRK_MODE_COMFORT_STOP);
      if ( (A_LAST_BRKH_CUS_CTRL_STATE == BRKH_CUS_PM_APC_CTRL)) {
        BRKH_SetRemainDist(A_APC_REMAINING_DISTANCE);
        BRKH_SetParkParkStat(A_APC_PARKMAN_STATUS);
        BRKH_SetParkParkType(A_APC_PARKMAN_PARKTYPE);
        BRKH_SetVehSpdRq(A_APC_VEHICLE_SPEED_REQUEST);
        BRKH_SetGearPosRq(A_APC_PARK_DRIVE_POS_REQUEST);
        BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_ACTIVE);
      }
      else {
        if ( (A_LAST_BRKH_CUS_CTRL_STATE == BRKH_CUS_PM_PPB_CTRL)) {
          BRKH_SetParkParkStat(A_PPB_PARK_PARKSTAT);
          BRKH_SetParkParkType(A_PPB_PARK_PARKTYPE);
          if ( (A_PPB_PARK_REMAINING_DISTANCE > C_MAX_REMAINING_DISTANCE)) {
            BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
          }
          else {
            BRKH_SetRemainDist(A_PPB_PARK_REMAINING_DISTANCE);
          }
          BRKH_SetVehSpdRq(A_PPB_VEHICLE_SPEED_REQUEST);
          BRKH_SetGearPosRq(GEAR_POSITION_RQ_IDLE);
          BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
        }
      }
    }
  }
  else {
  }
}



/* This function calculates the states that are exited, and executes */
/* the appropriate exit-state reaction in chart: BRKH_CUS_SC */
void
cgExitActions_BRKH_CUS_SCcnt1(StateInfo_BRKH_CUS_SCcnt1 staySame_brkh_cusBRKH_CUS_SCcnt1)
{
  if (wasInState(currentState_BRKH_CUS_SCcnt1 , UZL_CONTROL_ACTIVE)) {
    if (isNotStayingInState(staySame_brkh_cusBRKH_CUS_SCcnt1 , UZL_CONTROL_ACTIVE)) {
      /* Exit-state reaction of State: BRKH_CUS_SC:UZL_CONTROL_ACTIVE */
      stop_activity(UZL_CONTROL);
    }
  }
}



/* This function executes BRKH_CUS_SC (Statechart) */
/* --------------------------------------------------------------------------------------------------------------------------     */
/* |   Chart   |        From        |         To         |                 Transition Label                 |Transition Note| */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|                    |PM_NA               |                                                  |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|PM_NA               |PM_INIT             |[LI_COMH_KL15_STATUS==KL15_ON]                    |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|PM_INIT             |PM_NA               |[LI_COMH_KL15_STATUS==KL15_OFF]                   |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|PM_H_INACTIVE       |PM_INIT             |[LI_COMH_PT_READY==ENGINE_NOT_RUNNING]            |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|PM_FOLLOW_UP_CONTROL|PM_INACTIVE         |[LI_COMH_ESP_BRAKE_STATE==BRAKING_STATE_PARKMAN_IN|               | */
/* |           |                    |                    |ACTIV and LI_COMH_ESP_AFTERRUN_CTRL_ACTV==ESP_LDC_|               | */
/* |           |                    |                    |AFTERRUNCTRL_NO_FOLLOW_UP]                        |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|PM_INIT             |PM_H_INACTIVE       |[LI_COMH_PT_READY==ENGINE_RUNNING]                |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|PM_INACTIVE         |UZL_CONTROL_ACTIVE  |[(A_PPB_CTRL_ACTIVE or A_PPB_PREFILL_ACTIVE) or (A|               | */
/* |           |                    |                    |_APC_CTRL_ACTIVE)]                                |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|UZL_CONTROL_ACTIVE  |PM_FOLLOW_UP_CONTROL|EVT_UZL_FOLLOW_UP                                 |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
/* |BRKH_CUS_SC|                    |PM_INACTIVE         |                                                  |               | */
/* -------------------------------------------------------------------------------------------------------------------------- */
void
cgDo_BRKH_CUS_SCcnt1(void)
{
  StateInfo_BRKH_CUS_SCcnt1 nextState_brkh_cusBRKH_CUS_SCcnt1 = 0;
  StateInfo_BRKH_CUS_SCcnt1 staySame_brkh_cusBRKH_CUS_SCcnt1 = 0;
  if ((currentState_BRKH_CUS_SCcnt1 == 0) || inState(currentState_BRKH_CUS_SCcnt1 , BRKH_CUS_SCst2)) {
    staySame_brkh_cusBRKH_CUS_SCcnt1 = FS_BRKH_CUS_SCst2;
    if ((
    ((currentState_BRKH_CUS_SCcnt1 == 0) || inLeafState(currentState_BRKH_CUS_SCcnt1 , DefaultOf_BRKH_CUS_SCst2, StateInfo_BRKH_CUS_SCcnt1)))) {
      /* Transition to State: BRKH_CUS_SC:PM_NA */
      /* @Traceability :Sys_APC_PTS_Logic_3156*/  
      nextState_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_NA/* BRKH_CUS_SC:PM_NA *//* @Traceability :Sys_APC_PTS_Logic_147;Sys_APC_PTS_Logic_6205;Sys_APC_PTS_Logic_6206;Sys_APC_PTS_Logic_6207;Sys_APC_PTS_Logic_6208;Sys_APC_PTS_Logic_6209;Sys_APC_PTS_Logic_6212;Sys_APC_PTS_Logic_6211;Sys_APC_PTS_Logic_6213;Sys_APC_PTS_Logic_6217;Sys_APC_PTS_Logic_6214;Sys_APC_PTS_Logic_6215;Sys_APC_PTS_Logic_5901;Sys_APC_PTS_Logic_7401;Sys_APC_PTS_Logic_7402;Sys_APC_PTS_Logic_7403*/  ;
    }
    else if (inState(currentState_BRKH_CUS_SCcnt1 , STATE_10st2)) {
      staySame_brkh_cusBRKH_CUS_SCcnt1 = FS_STATE_10st2;
      if ((
      inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_INIT, StateInfo_BRKH_CUS_SCcnt1)
       && ((LI_COMH_KL15_STATUS == KL15_OFF)))) {
        /* Transition to State: BRKH_CUS_SC:PM_NA */
        /* @Traceability :Sys_APC_PTS_Logic_288;Sys_APC_PTS_Logic_289;Sys_APC_PTS_Logic_290;Sys_APC_PTS_Logic_291*/ /*@Transition Priority:1*/ 
        nextState_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_NA/* BRKH_CUS_SC:PM_NA *//* @Traceability :Sys_APC_PTS_Logic_147;Sys_APC_PTS_Logic_6205;Sys_APC_PTS_Logic_6206;Sys_APC_PTS_Logic_6207;Sys_APC_PTS_Logic_6208;Sys_APC_PTS_Logic_6209;Sys_APC_PTS_Logic_6212;Sys_APC_PTS_Logic_6211;Sys_APC_PTS_Logic_6213;Sys_APC_PTS_Logic_6217;Sys_APC_PTS_Logic_6214;Sys_APC_PTS_Logic_6215;Sys_APC_PTS_Logic_5901;Sys_APC_PTS_Logic_7401;Sys_APC_PTS_Logic_7402;Sys_APC_PTS_Logic_7403*/  ;
      }
      else if ((
      inState(currentState_BRKH_CUS_SCcnt1 , PM_H_INACTIVE)
       && ((LI_COMH_PT_READY == ENGINE_NOT_RUNNING))) || (
      inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_NA, StateInfo_BRKH_CUS_SCcnt1)
       && ((LI_COMH_KL15_STATUS == KL15_ON)))) {
        /* Transition to State: BRKH_CUS_SC:PM_INIT */
        /* @Traceability :Sys_APC_PTS_Logic_294;Sys_APC_PTS_Logic_295;Sys_APC_PTS_Logic_296;Sys_APC_PTS_Logic_297;Sys_APC_PTS_Logic_6251*/  
        /* @Traceability :Sys_APC_PTS_Logic_282;Sys_APC_PTS_Logic_283;Sys_APC_PTS_Logic_284;Sys_APC_PTS_Logic_285;Sys_APC_PTS_Logic_286*/  
        nextState_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_INIT/* BRKH_CUS_SC:PM_INIT *//* @Traceability :Sys_APC_PTS_Logic_150;Sys_APC_PTS_Logic_151;Sys_APC_PTS_Logic_152;Sys_APC_PTS_Logic_153;Sys_APC_PTS_Logic_154;Sys_APC_PTS_Logic_155;Sys_APC_PTS_Logic_156;Sys_APC_PTS_Logic_157;Sys_APC_PTS_Logic_158;Sys_APC_PTS_Logic_159;Sys_APC_PTS_Logic_160;Sys_APC_PTS_Logic_161*/  ;
      }
      else if ((
      inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_INACTIVE, StateInfo_BRKH_CUS_SCcnt1)
       && (( (A_PPB_CTRL_ACTIVE || A_PPB_PREFILL_ACTIVE) ||  (A_APC_CTRL_ACTIVE))))) {
        /* Transition to State: BRKH_CUS_SC:UZL_CONTROL_ACTIVE */
        /* @Traceability :Sys_APC_PTS_Logic_398;Sys_APC_PTS_Logic_399;Sys_APC_PTS_Logic_400;Sys_APC_PTS_Logic_318;Sys_APC_PTS_Logic_319;Sys_APC_PTS_Logic_320;Sys_APC_PTS_Logic_321;Sys_APC_PTS_Logic_312;Sys_APC_PTS_Logic_313;Sys_APC_PTS_Logic_314;Sys_APC_PTS_Logic_315;Sys_APC_PTS_Logic_324;Sys_APC_PTS_Logic_325;Sys_APC_PTS_Logic_326;Sys_APC_PTS_Logic_327;Sys_APC_PTS_Logic_330;Sys_APC_PTS_Logic_331;Sys_APC_PTS_Logic_332;Sys_APC_PTS_Logic_333;Sys_APC_PTS_Logic_334;Sys_APC_PTS_Logic_337;Sys_APC_PTS_Logic_338;Sys_APC_PTS_Logic_339;Sys_APC_PTS_Logic_340;Sys_APC_PTS_Logic_343;Sys_APC_PTS_Logic_344;Sys_APC_PTS_Logic_345;Sys_APC_PTS_Logic_346;Sys_APC_PTS_Logic_7069;Sys_APC_PTS_Logic_7154*/  
        nextState_brkh_cusBRKH_CUS_SCcnt1 = FS_UZL_CONTROL_ACTIVE/* BRKH_CUS_SC:UZL_CONTROL_ACTIVE *//* @Traceability :Sys_APC_PTS_Logic_130*/  ;
      }
      else if ((
      inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_INIT, StateInfo_BRKH_CUS_SCcnt1)
       && ((LI_COMH_PT_READY == ENGINE_RUNNING)))) {
        /* Transition to State: BRKH_CUS_SC:PM_INACTIVE */
        /* @Traceability :Sys_APC_PTS_Logic_300;Sys_APC_PTS_Logic_301;Sys_APC_PTS_Logic_302;Sys_APC_PTS_Logic_303;Sys_APC_PTS_Logic_6176*/ /*@Transition Priority:2*/ 
        nextState_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_INACTIVE/* BRKH_CUS_SC:PM_INACTIVE *//* @Traceability :Sys_APC_PTS_Logic_164;Sys_APC_PTS_Logic_165;Sys_APC_PTS_Logic_166;Sys_APC_PTS_Logic_167;Sys_APC_PTS_Logic_6246;Sys_APC_PTS_Logic_169;Sys_APC_PTS_Logic_170;Sys_APC_PTS_Logic_171;Sys_APC_PTS_Logic_172;Sys_APC_PTS_Logic_173;Sys_APC_PTS_Logic_174;Sys_APC_PTS_Logic_175*/  ;
      }
      else if ((
      inLeafState(currentState_BRKH_CUS_SCcnt1 , UZL_CONTROL_ACTIVE, StateInfo_BRKH_CUS_SCcnt1)
       && (EVT_UZL_FOLLOW_UP))) {
        /* Transition to State: BRKH_CUS_SC:PM_FOLLOW_UP_CONTROL */
          
        nextState_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_FOLLOW_UP_CONTROL/* BRKH_CUS_SC:PM_FOLLOW_UP_CONTROL *//* @Traceability :Sys_APC_PTS_Logic_178;Sys_APC_PTS_Logic_179;Sys_APC_PTS_Logic_180;Sys_APC_PTS_Logic_181;Sys_APC_PTS_Logic_182;Sys_APC_PTS_Logic_183;Sys_APC_PTS_Logic_184;Sys_APC_PTS_Logic_185;Sys_APC_PTS_Logic_186;Sys_APC_PTS_Logic_187;Sys_APC_PTS_Logic_188;Sys_APC_PTS_Logic_189*/  ;
      }
      else if (inLeafState(currentState_BRKH_CUS_SCcnt1 , UZL_CONTROL_ACTIVE, StateInfo_BRKH_CUS_SCcnt1)) {
        staySame_brkh_cusBRKH_CUS_SCcnt1 = FS_UZL_CONTROL_ACTIVE;
      }
      else if (inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_NA, StateInfo_BRKH_CUS_SCcnt1)) {
        staySame_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_NA;
        /* In-state reaction and actions of State: BRKH_CUS_SC:PM_NA */
        A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_NA;
        A_APC_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
        A_PPB_CTRL_ACTIVE = LOGICAL_CAST(FALSE);
        lval_A_PPB_PREFILL_ACTIVE = LOGICAL_CAST(FALSE);
        BRKH_SetParkBrkStat(BRAKING_STATE_PARKMAN_NOT_AVAILABLE);
        BRKH_SetParkParkStat(PARK_PARK_STAT_IDLE);
        BRKH_SetParkParkType(PARK_PARKTYPE_IDLE);
        BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
        BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
        BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
        BRKH_SetParkBrkMdRq(PARK_BRK_MODE_DISABLED);
        BRKH_SetVehSpdRq(C_IDLE_VEHICLE_SPEED_REQUEST);
        BRKH_SetGearPosRq(GEAR_POSITION_RQ_IDLE);
        BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
        BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
      }
      else if (inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_INIT, StateInfo_BRKH_CUS_SCcnt1)) {
        staySame_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_INIT;
      }
      else if (inState(currentState_BRKH_CUS_SCcnt1 , PM_H_INACTIVE)) {
        staySame_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_H_INACTIVE;
        /* In-state reaction and actions of State: BRKH_CUS_SC:PM_H_INACTIVE */
        if ( (LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_APC_MODE)) {
          BRKH_CusSetBrakeState(TRUE);
        }
        else {
          BRKH_CusSetBrakeState(FALSE);
        }
        if ((
        inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_FOLLOW_UP_CONTROL, StateInfo_BRKH_CUS_SCcnt1)
         && ((LI_COMH_ESP_BRAKE_STATE == BRAKING_STATE_PARKMAN_INACTIV && LI_COMH_ESP_AFTERRUN_CTRL_ACTV == ESP_LDC_AFTERRUNCTRL_NO_FOLLOW_UP)))) {
          /* Transition to State: BRKH_CUS_SC:PM_INACTIVE */
          /* @Traceability :Sys_APC_PTS_Logic_373;Sys_APC_PTS_Logic_374;Sys_APC_PTS_Logic_375;Sys_APC_PTS_Logic_376;Sys_APC_PTS_Logic_377*/  
          nextState_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_INACTIVE/* BRKH_CUS_SC:PM_INACTIVE *//* @Traceability :Sys_APC_PTS_Logic_164;Sys_APC_PTS_Logic_165;Sys_APC_PTS_Logic_166;Sys_APC_PTS_Logic_167;Sys_APC_PTS_Logic_6246;Sys_APC_PTS_Logic_169;Sys_APC_PTS_Logic_170;Sys_APC_PTS_Logic_171;Sys_APC_PTS_Logic_172;Sys_APC_PTS_Logic_173;Sys_APC_PTS_Logic_174;Sys_APC_PTS_Logic_175*/  ;
        }
        else if (inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_INACTIVE, StateInfo_BRKH_CUS_SCcnt1)) {
          staySame_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_INACTIVE;
          /* In-state reaction and actions of State: BRKH_CUS_SC:PM_INACTIVE */
          A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_INACTIVE;
          BRKH_SetParkBrkStat(BRAKING_STATE_PARKMAN_INACTIV);
          BRKH_SetParkParkType(A_APC_PARKMAN_PARKTYPE);
          BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
          BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
          BRKH_SetGearPosRq(GEAR_POSITION_RQ_IDLE);
          BRKH_SetVehSpdRq(C_IDLE_VEHICLE_SPEED_REQUEST);
          BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
          BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
          BRKH_SetParkBrkMdRq(PARK_BRK_MODE_COMFORT_STOP);
          BRKH_SetParkParkStat(A_APC_PARKMAN_STATUS);
          BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
        }
        else if (inLeafState(currentState_BRKH_CUS_SCcnt1 , PM_FOLLOW_UP_CONTROL, StateInfo_BRKH_CUS_SCcnt1)) {
          staySame_brkh_cusBRKH_CUS_SCcnt1 = FS_PM_FOLLOW_UP_CONTROL;
          /* In-state reaction and actions of State: BRKH_CUS_SC:PM_FOLLOW_UP_CONTROL */
          A_BRKH_CUS_CTRL_STATE = BRKH_CUS_PM_FOLLOW_UP_CTRL;
          BRKH_SetParkBrkStat(BRAKING_STATE_PARKMAN_INACTIV);
          BRKH_SetRemainDistStat(REM_DIST_STAT_NO_STEP);
          BRKH_SetParkDangerMdRq(DANGER_MODE_INACTIVE);
          BRKH_SetParkCtrlMdRq(PARK_CTRL_MODE_RQ_IDLE);
          BRKH_SetParkBrkMdRq(PARK_BRK_MODE_COMFORT_STOP);
          if ( (A_LAST_BRKH_CUS_CTRL_STATE == BRKH_CUS_PM_APC_CTRL)) {
            BRKH_SetRemainDist(A_APC_REMAINING_DISTANCE);
            BRKH_SetParkParkStat(A_APC_PARKMAN_STATUS);
            BRKH_SetParkParkType(A_APC_PARKMAN_PARKTYPE);
            BRKH_SetVehSpdRq(A_APC_VEHICLE_SPEED_REQUEST);
            BRKH_SetGearPosRq(A_APC_PARK_DRIVE_POS_REQUEST);
            BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_ACTIVE);
          }
          else {
            if ( (A_LAST_BRKH_CUS_CTRL_STATE == BRKH_CUS_PM_PPB_CTRL)) {
              BRKH_SetParkParkStat(A_PPB_PARK_PARKSTAT);
              BRKH_SetParkParkType(A_PPB_PARK_PARKTYPE);
              if ( (A_PPB_PARK_REMAINING_DISTANCE > C_MAX_REMAINING_DISTANCE)) {
                BRKH_SetRemainDist(C_MAX_REMAINING_DISTANCE);
              }
              else {
                BRKH_SetRemainDist(A_PPB_PARK_REMAINING_DISTANCE);
              }
              BRKH_SetVehSpdRq(A_PPB_VEHICLE_SPEED_REQUEST);
              BRKH_SetGearPosRq(GEAR_POSITION_RQ_IDLE);
              BRKH_SetParkGuidStat(PARK_GUIDANCE_STATE_INACTIVE);
            }
          }
        }
        else {
        }
      }
      else {
      }
    }
    else {
    }
  }
  if (nextState_brkh_cusBRKH_CUS_SCcnt1 != 0) {
    cgExitActions_BRKH_CUS_SCcnt1(staySame_brkh_cusBRKH_CUS_SCcnt1);
    cgEnterActions_BRKH_CUS_SCcnt1(staySame_brkh_cusBRKH_CUS_SCcnt1, nextState_brkh_cusBRKH_CUS_SCcnt1);
    currentState_BRKH_CUS_SCcnt1 = nextState_brkh_cusBRKH_CUS_SCcnt1;
  }
}




/* This function executes the Activity: BRKH_CUS_AC:BRKH_CUS_SC */
void
cgActivity_BRKH_CUS_SCcnt1(void)
{
  /* BRKH_CUS_AC:BRKH_CUS_SC */  
  cgDo_BRKH_CUS_SCcnt1();
}



/* This function executes the Activity: BRKH_INPUTS_AC:CHECK_APC_AC */
void
cgActivity_CHECK_APC_AC(void)
{
  /* BRKH_INPUTS_AC:CHECK_APC_AC */  
  cgActivity_CTRL_APC_CHECK_SC();
  cgActivity_REM_DIST_AC();
  cgActivity_APC_GEAR_AC();
  cgActivity_APC_SPEED_AC();
}



/* This function executes the Activity: BRKH_INPUTS_AC:CHECK_PPB_AC */
void
cgActivity_CHECK_PPB_AC(void)
{
  /* BRKH_INPUTS_AC:CHECK_PPB_AC */  
  cgActivity_CTRL_PPB_CHECK_SC();
}



/* This function executes the Activity: BRKH_CUS_AC:UZL_CONTROL */
void
cgActivity_UZL_CONTROL(void)
{
  /* BRKH_CUS_AC:UZL_CONTROL */  
  if ((cgGlobalFlags_brkh_cus & BITAC_UZL_CONTROL) != 0) {
    cgActivity_UZL_CONTROL_SC();
  }
}



/* This function executes the Activity: BRKH_CUS_AC:BRKH_CUS */
void
cgActivity_BRKH_CUS(void)
{
  /* BRKH_CUS_AC:BRKH_CUS *//* @Traceability :Sys_APC_PTS_Logic_143*/  
  cgActivity_BRKH_CUS_SCcnt1();
  cgActivity_UZL_CONTROL();
}



/* This function executes the Activity: BRKH_INPUTS_AC:BRKH_INPUTS */
void
cgActivity_BRKH_INPUTS(void)
{
  /* BRKH_INPUTS_AC:BRKH_INPUTS */  
  cgActivity_CHECK_APC_AC();
  cgActivity_CHECK_PPB_AC();
}



/* This function executes the Activity: ACTIVATION_LOGIC_TOP_AC:BRKH_CUS_AC */
void
cgActivity_BRKH_CUS_AC(void)
{
  /* ACTIVATION_LOGIC_TOP_AC:BRKH_CUS_AC *//* @Traceability :Sys_APC_PTS_Logic_*/  
  cgActivity_BRKH_CUS();
}



/* This function executes the Activity: ACTIVATION_LOGIC_TOP_AC:BRKH_INPUTS_AC */
void
cgActivity_BRKH_INPUTS_AC(void)
{
  /* ACTIVATION_LOGIC_TOP_AC:BRKH_INPUTS_AC */  
  cgActivity_BRKH_INPUTS();
}



/* This function executes the Activity: ACTIVATION_LOGIC_TOP_AC:TIMER_INCREMENT_AC */
void
cgActivity_TIMER_INCREMENT_AC(void)
{
  /* Increments the timer and handles timer overflow.  *//* @Traceability :Sys_APC_PTS_Logic_*/  
  if (MS_COUNTER == C_MS_COUNTER_MAX) {
    MS_COUNTER = 0;
    BRKH_Overflow();
  }
  MS_COUNTER++;
}



/* This function tests for timeout expiration, and set the appropriate timeout event.*/
/* It might be called from 2 functions: */
/*    Task: BRKH_CUSBRAKESTATEMACHINE body, and the various Counter's overflow Tasks */
void
genTmEvent_BRKH_CUSBRAKESTATEMACHINE(uint16 currentTickVar, cgDoubleBufferType_BRKH_CUSBRAKESTATEMACHINE* buff, uint8 counterIndex)
{
  if((cgTimeoutsMask_brkh_cus & tm_999999998_brkh_cus_TM_MASK) && (currentTickVar >= tm_999999998_brkh_cus_TIME)) {
    GEN_IN_BUFF(tm_999999998_brkh_cus, buff);
    cgTimeoutsMask_brkh_cus &= ~tm_999999998_brkh_cus_TM_MASK;
  }
  if((cgTimeoutsMask_brkh_cus & tm_999999997_brkh_cus_TM_MASK) && (currentTickVar >= tm_999999997_brkh_cus_TIME)) {
    GEN_IN_BUFF(tm_999999997_brkh_cus, buff);
    cgTimeoutsMask_brkh_cus &= ~tm_999999997_brkh_cus_TM_MASK;
  }
  if((cgTimeoutsMask_brkh_cus & tm_999999992_brkh_cus_TM_MASK) && (currentTickVar >= tm_999999992_brkh_cus_TIME)) {
    GEN_IN_BUFF(tm_999999992_brkh_cus, buff);
    cgTimeoutsMask_brkh_cus &= ~tm_999999992_brkh_cus_TM_MASK;
  }
  if((cgTimeoutsMask_brkh_cus & tm_999999991_brkh_cus_TM_MASK) && (currentTickVar >= tm_999999991_brkh_cus_TIME)) {
    GEN_IN_BUFF(tm_999999991_brkh_cus, buff);
    cgTimeoutsMask_brkh_cus &= ~tm_999999991_brkh_cus_TM_MASK;
  }
}



/* This function is the code-body of Task: ACTIVATION_LOGIC_TOP_AC:BRKH_CUSBRAKESTATEMACHINE */
void BRKH_CUSBRAKESTATEMACHINE(void)
{
  /* ACTIVATION_LOGIC_TOP_AC:BRKH_CUSBRAKESTATEMACHINE *//* @Traceability :Sys_APC_PTS_Logic_6183*/  
  if (BRKH_CusIsEmergencyBrakeActive()) {
    LI_BRKH_APPL_EMERGENCY_REQUEST = 1;
  } else {
    LI_BRKH_APPL_EMERGENCY_REQUEST = 0;
  }
  if (BRKH_ParkEnblStatEspAPC()) {
    LI_COMH_APC_ESP_ENABLE_STATE = 1;
  } else {
    LI_COMH_APC_ESP_ENABLE_STATE = 0;
  }
  if (BRKH_ParkEnblStatEspPPB()) {
    LI_COMH_PPB_ESP_ENABLE_STATE = 1;
  } else {
    LI_COMH_PPB_ESP_ENABLE_STATE = 0;
  }
  if (P2DAL_IsEmergencyBrakeImminent()) {
    LI_DAPM_EMERGENCY_IMMINENT = 1;
  } else {
    LI_DAPM_EMERGENCY_IMMINENT = 0;
  }
  if (P2DAL_IsEmergencyBrakeRequired()) {
    LI_DAPM_EMERGENCY_REQUIRED = 1;
  } else {
    LI_DAPM_EMERGENCY_REQUIRED = 0;
  }
  if (P2DAL_IsCtrlSystemActive()) {
    LI_DAPM_IS_CTRL_ACTIVE = 1;
  } else {
    LI_DAPM_IS_CTRL_ACTIVE = 0;
  }
  if (P2DAL_IsVehicleStandstill()) {
    LI_DAPM_IS_VEHICLE_STANDSTILL = 1;
  } else {
    LI_DAPM_IS_VEHICLE_STANDSTILL = 0;
  }
  if (P2DAL_IsSteerActiveInStandstill()) {
    LI_DAPM_STEERING_IN_STANDSTILL = 1;
  } else {
    LI_DAPM_STEERING_IN_STANDSTILL = 0;
  }
  LI_COMH_CAR_CURRENT_GEAR=BRKH_CarCurrentGear();;
  {
    EN_COMH_AUTO_TRANSMISSION_GEAR_STATE_TYPE  tmpLI_COMH_CAR_GEAR_LEVER_POSITION;
    tmpLI_COMH_CAR_GEAR_LEVER_POSITION = BRKH_GearLeverPos();
    if (LI_COMH_CAR_GEAR_LEVER_POSITION != tmpLI_COMH_CAR_GEAR_LEVER_POSITION) {
      LI_COMH_CAR_GEAR_LEVER_POSITION = tmpLI_COMH_CAR_GEAR_LEVER_POSITION;
      GENERATE_EVENT(CHANGED_LI_COMH_CAR_GEAR_LEVER_POSITION);
    }
  }
  LI_COMH_ESP_AFTERRUN_CTRL_ACTV=COMH_GetEspAfterrunControl();
  LI_COMH_ESP_BRAKE_STATE = COMH_GetEspBrakeState();
  LI_COMH_ESP_INTERVENTION_STATE=COMH_GetESPIntervention();
  LI_COMH_ESP_SYSTEM_STATE=COMH_GetEspSystemState();
  LI_COMH_GET_BRAKE_PEDAL_TORQUE=COMH_GetBrakePedalTorque();
  LI_COMH_KL15_STATUS=COMH_GetKl15();
  LI_COMH_PARK_FLT_STAT_ESP=COMH_GetParkFltStatEsp();
  LI_COMH_PT_READY=COMH_GetEngineState();
  LI_COMH_REAL_TIME_VHCL_SPEED=BRKH_GetSpeedKph();
  LI_DAPM_DRIVING_HINT=P2DAL_GetDrivingHint();;
  LI_DAPM_GETSPECIFIC_CTRL_ABORT_REASON=P2DAL_GetSpecificCtrlAbortReason();
  LI_DAPM_GET_DISTANCE_TO_COLLISION=P2DAL_GetDistToCollision();
  LI_DAPM_GET_DISTANCE_TO_HINT=P2DAL_GetDistToHint();
  LI_DAPM_MAX_VELOCITY_RQ=P2DAL_GetDrivableVelInterpMms();
  LI_DAPM_OPERATION_STATE=P2DAL_GetOperationState();
  LI_ODOM_CAR_POSITION=BRKH_GetPosition();
  LO_PARK_PARK_STAT = BRKH_GetParkParkStat();
  if (cgTimeoutsMask_brkh_cus != 0) {
    currentTick_brkh_cus = MS_COUNTER;
    /* Test for expired pending timeouts */
    genTmEvent_BRKH_CUSBRAKESTATEMACHINE(currentTick_brkh_cus, &cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE, MS_COUNTER_INDEX);
  }
  cgDoubleBufferOld_brkh_cusBRKH_CUSBRAKESTATEMACHINE = cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE;
  cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus = 0;
  cgDoubleBufferNew_brkh_cusBRKH_CUSBRAKESTATEMACHINE.cg_Events_brkh_cus1 = 0;
  cgActivity_TIMER_INCREMENT_AC();
  cgActivity_BRKH_INPUTS_AC();
  cgActivity_BRKH_CUS_AC();

}


#if defined __HC12__ || defined __HC08__
#pragma CODE_SEG DEFAULT
#elif defined(COSMIC12)
#pragma section ()
#endif


/* End of code for: brkh_stm.c */

/* End of generated file */
