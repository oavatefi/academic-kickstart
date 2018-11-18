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
/*      Source:         stmh_cus.c                                                  */
/*      Revision:       1.1.1.3                                                 */
/*      Author:         kistner_andreas                                       */
/*      UserID:         BRODRIG2                                                  */
/*      Date            14-JUL-2015 17:13:33                                                 */
/*                                              */
/******************************************************************************/
/*   MODULE: STMH_CUS (customer specific code (steering communication) of the */
/*                     STMH-module)                                           */
/*   FILE: STMH_CUS.C                                                         */
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
#include <stdio.h>
/******************************************************************************/
/*                      Include external modul header                         */
/******************************************************************************/

/******************************************************************************/
/*                      Include internal modul header                         */
/******************************************************************************/
#include "stmh_cus.h"
#include "comh.h"
#include "p2dal.h"
/******************************************************************************/
/*                   Definition of local module constants                     */
/******************************************************************************/

/******************************************************************************/
/*                     Definition of local module macros                      */
/******************************************************************************/

#define STMH_STATE_SEND_TIME_MS  270
#define XVCTM_CYCLE_TIME_MS 20

/******************************************************************************/
/*                     Definition of local module types                       */
/******************************************************************************/



/******************************************************************************/
/*                 Definition of local module constant data                   */
/******************************************************************************/


#define STMH_EPS_STATUS_TIMEOUT 100

/******************************************************************************/
/*                   Definition of local module variables                     */
/******************************************************************************/
/** contains the steering state for the STMA module */
static bool_T      stmh_steering_state;
/** contains the APP-State to be sent from APP to Steering ECU */
static u8 stmh_app_state_to_send;
static u8 stmh_steer_abort_reason;
static bool_T                         stmh_is_steering_ready;
static enum stmh_app_states_E         stmh_app_main_state;
static enum stmh_app_states_E         stmh_last_app_main_state;
static enum stmh_app_sub_states_E     stmh_app_sub_state;

static u8                             stmh_state_send_timer;
static bool_T                         stmh_activation_request = FALSE;
/******************************************************************************/
/*                      Definition of extern variables                        */
/******************************************************************************/

/******************************************************************************/
/*                     Definition of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of exported constant data                     */
/******************************************************************************/

/***** Received steer ECU states *******/
#define STEER_ECU_INIT_STATE            0   //INIT  System is initializing
#define STEER_ECU_ENBL_STATE            1   //ENBL  System is enabled
#define STEER_ECU_CTRL_STATE            2   //CTRL  System is controlling
#define STEER_ECU_DSABL_STATE           3   //DSABL System is disabled
#define STEER_ECU_CANCEL_SYS_STATE      4   //CANCEL_SYS    Cancel by system
#define STEER_ECU_CANCEL_DRV_STATE      5   //CANCEL_DRV    Cancel by driver

/*******Request EPS commands*************/
#define STEER_ECU_INIT_REQUEST          0   //INIT  System is initializing
#define STEER_ECU_ENBL_REQUEST          1   //System is enabled
#define STEER_ECU_CTRL_REQUEST          2   //System is controlling
#define STEER_ECU_DSABL_REQUEST         3   //System is disabled
#define STEER_ECU_CANCEL_SYS_REQUEST    4   //CANCEL_SYS    Cancel by system
#define STEER_ECU_CANCEL_DRV_REQUEST    5   //CANCEL_DRV    Cancel by driver

/******Abort Reason ********************/
#define STEER_ECU_ABORT_NONE            0

/******************************************************************************/
/*                   Declaration of local module function prototyps           */
/******************************************************************************/
static void   InitStateSendTimer(void);
static void   IncrementStateSendTimer(void);
static void   StartStateSendTimer(void);
static bool_T StateSendTimerExpired(void);
static bool_T isEngineReady(void);

/****************************************************************
 *       Steer ECU Function deceleration
 ***************************************************************/
/***** Received ECU states *******/
static inline bool_T isSteerEcuEnabled (uint8 steer_ecu_state);
static inline bool_T isSteerEcuUnAvailable (uint8 steer_ecu_state);
static inline bool_T isSteerActive (uint8 steer_ecu_state);
static inline bool_T isSteerEcuDisabled (uint8 steer_ecu_state);
static inline bool_T isSteerEcuCancel (uint8 steer_ecu_state);
static inline bool_T isNoPendingCancelReason(uint8 steer_abort_reason, uint8 last_steer_abort_reason);
static inline bool_T isSteerEcuInit (uint8 steer_ecu_state);

/****** Requested ECU state ********/
static inline uint8 getSteerActRequestValue(void);
static inline uint8 getSteerRdyRequestValue(void);
static inline uint8 getSteerInitRequestValue(void);
static inline uint8 getSteerDsablRequestValue(void);
static inline uint8 getSteerCancelSysRequestValue(void);

/*******   abort reason      *********/
static inline uint8 getSteerNonAbortReasonValue(void);



/******************************************************************************/
/* BEGIN Source: */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of local module functions                     */
/******************************************************************************/

static void InitStateSendTimer(void)
{
  stmh_state_send_timer = 255; /* inactive */
}

static void IncrementStateSendTimer(void)
{
  /* increment the state send timer */
  if (stmh_state_send_timer <= (STMH_STATE_SEND_TIME_MS / XVCTM_CYCLE_TIME_MS))
  {
    stmh_state_send_timer++;
  }
}

static void StartStateSendTimer(void)
{
  /* only (re-) start if timer is in init-state (inactive) */
  if (stmh_state_send_timer == 255)
  {
    stmh_state_send_timer = 0;
  }
}

static bool_T StateSendTimerExpired(void)
{
  bool_T ret_value = FALSE;

  if (stmh_state_send_timer > (STMH_STATE_SEND_TIME_MS / XVCTM_CYCLE_TIME_MS))
  {
    ret_value = TRUE;
  }
  return ret_value;
}

/******************************************************************************/
/*                  Definition of exported module functions                   */
/******************************************************************************/

/******************************************************************************/
/**
 * inits the STMH_CUS module
 * It is called by the STMH module.
 *
 * \return nothing
 */
/******************************************************************************/
void STMH_CusInit(void)
{
  stmh_app_state_to_send  = getSteerInitRequestValue();
  stmh_is_steering_ready  = FALSE;
  stmh_steering_state     = FALSE;
  stmh_app_main_state = STMH_APP_INIT;
  stmh_last_app_main_state = STMH_APP_INIT;
  stmh_app_sub_state  = STMH_APP_SUB_REQ;

  InitStateSendTimer();

  STMH_CusResetAbortReason();
}

/******************************************************************************/
/**
 * Statemachine for handshake with steering ecu
 * It is called by a cyclic STMH function
 *
 * \return nothing
 */
/******************************************************************************/

void STMH_CusSteeringStateMachine(void)
{
#if !defined(XAPPL_ENABLE_UPA_BRAKE_PROTOTYP)
  /* State of Steering ECU received by APP ECU */
  u8	temp_steering_state_received;
  u8	temp_steer_abort_reason;
  u16	ext_reason;
  static volatile u8 last_temp_steer_abort_reason= 0;


  /* Get inputs from steering ECU */

  temp_steering_state_received = COMH_GetStatusEps();
  temp_steer_abort_reason      = COMH_GetPlaTerminationOfEps();

  /* Get inputs from STMA module */

  if (P2DAL_IsLatCtrlRequired())

  {
      STMH_CusActivateSteer();

  }
  else
  {
      STMH_CusDeactivateSteer();
  }
  /* init of static members */
  stmh_app_state_to_send       = getSteerInitRequestValue();                             /* Park4U-ECU-State:  0 */

  switch (stmh_app_main_state)
  {
    case STMH_APP_INIT:
		/* IF Ignition is on we send Enable value to EPS else we send INIT value */
	  if (isEngineReady())
	  {
		stmh_app_state_to_send = getSteerRdyRequestValue();                               /* Park4U-ECU-State:  1 */
	  }
	  else
	  {
		stmh_app_state_to_send = getSteerInitRequestValue();                               /* Park4U-ECU-State:  0 */
	  }
      stmh_steering_state    = FALSE;

      switch (stmh_app_sub_state)
      {
        case STMH_APP_SUB_REQ:
          StartStateSendTimer();
          if ( (StateSendTimerExpired() != FALSE) &&
        	   ((isSteerEcuInit(temp_steering_state_received)) ||
        	   (isSteerEcuEnabled(temp_steering_state_received))) )              /* SteerECU-State:  0 or 1 */
          {
            stmh_app_sub_state = STMH_APP_SUB_ACKN;
          }
          break;
        case STMH_APP_SUB_ACKN:
          /* set flag that steering ECU is initialized */
          stmh_is_steering_ready  = FALSE;
          if (	( ((isSteerEcuInit(temp_steering_state_received)) ||
        		   (isSteerEcuEnabled(temp_steering_state_received))))
        	 	&& (isEngineReady()) ) /*check if we need to Handle Condition for engine running since more than 15 sec */
          {
        	  stmh_app_main_state = STMH_APP_READY;
          }
          break;
        /* PRQA S 2018 4 */
        /* << This statement is only reachable if there is a new sub state added \
              and not handled yet. >> */
        default:
          _ASSERT(FALSE);
          break;
      }
      /* Actions on exit of the state */
      if (stmh_app_main_state != STMH_APP_INIT)
      {
        stmh_app_sub_state  = STMH_APP_SUB_REQ;
        stmh_last_app_main_state = STMH_APP_INIT;
        InitStateSendTimer();
      }
      break;

    case STMH_APP_READY:
      stmh_app_state_to_send = getSteerRdyRequestValue();                               /* Park4U-ECU-State:  1 */
      stmh_steering_state    = FALSE;

      switch (stmh_app_sub_state)
      {
        case STMH_APP_SUB_REQ:
          StartStateSendTimer();
          if ( (StateSendTimerExpired() != FALSE) &&
               (isSteerEcuEnabled(temp_steering_state_received)))              /* SteerECU-State:  1 */
          {
            stmh_app_sub_state = STMH_APP_SUB_ACKN;
          }
          break;
        case STMH_APP_SUB_ACKN:
          /* set flag that steering ECU is ready (initialized) */
          stmh_is_steering_ready  = TRUE;
          if (isSteerEcuEnabled(temp_steering_state_received) && isEngineReady())
          {
        	  if (stmh_activation_request == TRUE)
        	  {
        		  stmh_app_main_state = STMH_APP_STEERING_ACTIVE;
        	  }
          }
          break;
        /* PRQA S 2018 4 */
        /* << This statement is only reachable if there is a new sub state added \
              and not handled yet. >> */
        default:
          _ASSERT(FALSE);
          break;
      }
      /* Check for Internal EPS failure */
      if ( 	(isSteerEcuUnAvailable(temp_steering_state_received)) ||
    		(isNoPendingCancelReason(temp_steer_abort_reason,last_temp_steer_abort_reason) != TRUE) ||
    		( (isSteerEcuInit(temp_steering_state_received)) && (stmh_last_app_main_state != STMH_APP_INIT )))
    	  /* Check for Init is separated to avoid heading direct transition to ABORT at EPS Initialization phase*/
      {
    	  stmh_is_steering_ready  	= FALSE;
    	  stmh_app_main_state 		= STMH_APP_STEER_CANCEL;
      }
      /* If Engine is Off Go back and wait in INIT */
      else if (isEngineReady() == FALSE)
      {
    	  stmh_is_steering_ready  	= FALSE;
    	  stmh_app_main_state 		= STMH_APP_INIT;
      }
      /* Actions on exit of the state */
      if (stmh_app_main_state != STMH_APP_READY)
      {
        stmh_app_sub_state  = STMH_APP_SUB_REQ;
        stmh_last_app_main_state = STMH_APP_READY;
        InitStateSendTimer();
      }
      break;

    case STMH_APP_STEERING_ACTIVE:
      stmh_app_state_to_send = getSteerActRequestValue();                     /* Park4U-ECU-State:  2 */
      if ( stmh_activation_request == FALSE )
      {
    	  if (P2DAL_GetSpecificCtrlAbortReason() == DAPM_OAR_NONE)
    	  {
    		  stmh_app_main_state = STMH_APP_READY;
    	  }
    	  else
    	  {
    		  stmh_app_main_state = STMH_APP_ABORT;
    	  }
      }else{

      }

      switch (stmh_app_sub_state)
      {
        case STMH_APP_SUB_REQ:
          StartStateSendTimer();
          if ( (StateSendTimerExpired() != FALSE) &&
        	   (isSteerActive(temp_steering_state_received)) )      /* SteerECU-State:  2 */
            {
              stmh_app_sub_state = STMH_APP_SUB_ACKN;
            }
          break;
        case STMH_APP_SUB_ACKN:
          stmh_steering_state = TRUE;
          if (isSteerEcuEnabled(temp_steering_state_received))
          {
            stmh_app_main_state = STMH_APP_STEER_CANCEL;
          }
          break;
        /* PRQA S 2018 4 */
        /* << This statement is only reachable if there is a new sub state added \
              and not handled yet. >> */
        default:
          _ASSERT(FALSE);
          break;
      }
      /* Check for Internal EPS failure */
      if ( 	(isSteerEcuUnAvailable(temp_steering_state_received)) ||
    		(isNoPendingCancelReason(temp_steer_abort_reason,last_temp_steer_abort_reason) != TRUE) ||
    		(isSteerEcuInit(temp_steering_state_received)) )
      {
    	  stmh_is_steering_ready  	= FALSE;
    	  stmh_app_main_state 		= STMH_APP_STEER_CANCEL;
      }
      /* If Engine is Off Go back and wait in INIT */
      else if (isEngineReady() == FALSE)
      {
    	  stmh_is_steering_ready  	= FALSE;
    	  stmh_app_main_state 		= STMH_APP_INIT;
      }
      /* Actions on exit of the state */
      if (stmh_app_main_state != STMH_APP_STEERING_ACTIVE)
      {
        stmh_app_sub_state  = STMH_APP_SUB_REQ;
        stmh_last_app_main_state = STMH_APP_STEERING_ACTIVE;
        STMH_CusDeactivateSteer();
        InitStateSendTimer();
      }
      break;

    case STMH_APP_ABORT:
      stmh_app_state_to_send = getSteerDsablRequestValue();                               /* Park4U-ECU-State:  3 */
      stmh_steering_state    = FALSE;
      switch (stmh_app_sub_state)
      {
        case STMH_APP_SUB_REQ:
          if (((isSteerEcuDisabled(temp_steering_state_received)) ||
                (isSteerEcuCancel(temp_steering_state_received)) ))              /* SteerECU-State: 3 or 4 or 5 */

          {
            stmh_app_sub_state = STMH_APP_SUB_ACKN;
          }
          break;
        case STMH_APP_SUB_ACKN:
          /* Check for Cancel Reason */
          if (isNoPendingCancelReason(temp_steer_abort_reason,last_temp_steer_abort_reason))
          {
        	stmh_app_state_to_send = getSteerRdyRequestValue();
        	if (isSteerEcuEnabled(temp_steering_state_received))
        	{
            stmh_app_main_state = STMH_APP_READY;
        	}
          }
          break;
        /* PRQA S 2018 4 */
        /* << This statement is only reachable if there is a new sub state added \
              and not handled yet. >> */
        default:
          _ASSERT(FALSE);
          break;
      }
      /* If Engine is Off Go back and wait in INIT */
      if (isEngineReady() == FALSE)
      {
    	  stmh_is_steering_ready  	= FALSE;
    	  stmh_app_main_state 		= STMH_APP_INIT;
      }
      /* Actions on exit of the state */
      if (stmh_app_main_state != STMH_APP_ABORT)
      {
        stmh_app_sub_state  = STMH_APP_SUB_REQ;
        stmh_last_app_main_state = STMH_APP_ABORT;
        InitStateSendTimer();
      }
      break;


    case STMH_APP_STEER_CANCEL:
      stmh_app_state_to_send  = getSteerCancelSysRequestValue();                              /* Park4U-ECU-State:  4 */
      stmh_steer_abort_reason = temp_steer_abort_reason;
      stmh_steering_state     = FALSE;

      switch (stmh_app_sub_state)
      {
        case STMH_APP_SUB_REQ:
          if (  (isSteerEcuDisabled(temp_steering_state_received))
             || (isSteerEcuCancel(temp_steering_state_received)))              /* SteerECU-State: 3 or 4 or 5 */

          {
            stmh_app_sub_state = STMH_APP_SUB_ACKN;
          }
          break;
        case STMH_APP_SUB_ACKN:
          /* Check for Cancel Reason */
          if (isNoPendingCancelReason(temp_steer_abort_reason,last_temp_steer_abort_reason))
          {
        	stmh_app_state_to_send = getSteerRdyRequestValue();
        	if (isSteerEcuEnabled(temp_steering_state_received))
        	{
            stmh_app_main_state = STMH_APP_READY;
        	}
          }
          break;
        /* PRQA S 2018 4 */
        /* << This statement is only reachable if there is a new sub state added \
              and not handled yet. >> */
        default:
          _ASSERT(FALSE);
          break;
      }

      /* If Engine is Off Go back and wait in INIT */
      if (isEngineReady() == FALSE)
      {
    	  stmh_is_steering_ready  	= FALSE;
    	  stmh_app_main_state 		= STMH_APP_INIT;
      }
      /* Actions on exit of the state */
      if (stmh_app_main_state != STMH_APP_STEER_CANCEL)
      {
        stmh_app_sub_state  = STMH_APP_SUB_REQ;
        stmh_last_app_main_state = STMH_APP_STEER_CANCEL;
        InitStateSendTimer();
      }
      break;

    /* PRQA S 2018 4 */
    /* << This statement is only reachable if there is a new main state added \
          and not handled yet. >> */
    default:
      _ASSERT(FALSE);
      break;
  }

  /* Update last steering abort reason */
  last_temp_steer_abort_reason = temp_steer_abort_reason;
  /* increment the state send timer */
  IncrementStateSendTimer();
  /* PRQA S 5560,5830 7 */
  /* << STMH_CusSteeringStateMachine() is tested and verified with VW Park4U GEN2 project so \
        the function will not be changed because of the too high cyclomatic complexity (STCYC) \
        and the too large number of lines of effective code (STXLN). \
        Modification only if there is a request to change the functionality. >> */
#endif
}

/******************************************************************************/
/**
 * returns the actual steering state of the statemachine (needed as input
 * for the STMA/STMH-module)
 *
 * \return the actual steering state of the statemachine
 */
/******************************************************************************/
u8 STMH_CusGetStateEpsSys(bool_T* p_lat_ext_sys_state)
{
    u8 ret_val = 0;
#ifdef XAPPL_ENABLE_UPA_BRAKE_PROTOTYP
    enum VCTC_ctrl_state_E temp_stma_state = P2DAL_GetLotCtrlState();

    if ( (temp_stma_state == VCTC_STATE_DEACTIVATING) ||
         (temp_stma_state == VCTC_STATE_STANDBY) )
    {
        *p_lat_ext_sys_state = FALSE;
    }
    else
    {
        /* STMA_STATE_ACTIVATING || STMA_STATE_ACTIVE */
        *p_lat_ext_sys_state = TRUE;
    }

    return ret_value;
#else
    *p_lat_ext_sys_state = stmh_steering_state;
#endif
    return ret_val;
}

/******************************************************************************/
/**
 * returns the state of the app system which has to be send to the steering ecu
 * (handshake with steering ecu)
 *
 * \return the actual state of the app system (0..15)
 */
/******************************************************************************/
u8 STMH_CusGetAppStateToSteering(void)
{
  return (u8)stmh_app_state_to_send;
}

/******************************************************************************/
/**
 * returns the True if the request to be sent is activation request
 *
/******************************************************************************/
bool_T STMH_CusIsActivatingRequest(void)
{
  return (stmh_app_state_to_send == getSteerActRequestValue());
}

/******************************************************************************/
/**
 * returns the abort reason of the steering system for debug purpose
 *
 * \return the abort reason of the steering system
 */
/******************************************************************************/
u8 STMH_CusGetSteeringAbortReason(void)
{
  return (u8)stmh_steer_abort_reason;
}

/******************************************************************************/
/**
 * resets the abort reason of the steering system to the init value
 *
 * \return nothing
 */
/******************************************************************************/
void STMH_CusResetAbortReason(void)
{
  stmh_steer_abort_reason = getSteerNonAbortReasonValue();
}

/******************************************************************************/
/**
 * returns if the steering system is active (activated steering)
 *
 * \return if the steering system is active (TRUE) or not (FALSE)
 */
/******************************************************************************/
bool_T STMH_CusIsSteeringActive(void)
{
  return (bool_T)(stmh_steering_state);
}

/******************************************************************************/
/**
 * returns if the steering system is ready for activating the steering access
 * (steering system is initialized)
 *
 * \return if the steering system is initialized (TRUE) or not (FALSE)
 */
/******************************************************************************/
bool_T STMH_CusIsSteeringReady(void)
{
  return stmh_is_steering_ready;
}

/******************************************************************************
 * Call this function to request the activation of the steering ECU
 * \return if TRUE if the activation request is accepted else returns FALSE
******************************************************************************/
bool_T STMH_CusActivateSteer(void)
{
    bool_T ret = FALSE;
    if( (stmh_activation_request == FALSE) && (stmh_app_main_state == STMH_APP_READY))
    {
        stmh_activation_request = TRUE;
        ret = TRUE;
    }
    return ret;
}

/******************************************************************************
 * Call this function to request the de-activation of the steering ECU
******************************************************************************/
void STMH_CusDeactivateSteer(void)
{
    stmh_activation_request = FALSE;
    // Req_Steering = 0; // KABD
}

u8 STMH_GetMainState(void)
{
    return stmh_app_main_state;
}

u8 STMH_GetActiveSubState(void)
{
	return stmh_app_sub_state;
}

static bool_T isEngineReady (void)
{
    if( COMH_GetEngineState() == ENGINE_RUNNING)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }

}

/****************************************************************
 *       Steering ECU Function definition
 ***************************************************************/
/***           Received ECU states    **/
static inline bool_T isSteerEcuInit (uint8 steer_ecu_state)
{
    bool_T ret_val = FALSE;
    if(steer_ecu_state == STEER_ECU_INIT_STATE)
    {
        ret_val = TRUE;
    }

    return ret_val;
}

static inline bool_T isSteerEcuEnabled (uint8 steer_ecu_state)
{
    bool_T ret_val = FALSE;
    if(steer_ecu_state == STEER_ECU_ENBL_STATE)
    {
        ret_val = TRUE;
    }

    return ret_val;
}

static inline bool_T isSteerEcuUnAvailable (uint8 steer_ecu_state)
{
    bool_T ret_val = FALSE;
    if( (steer_ecu_state == STEER_ECU_DSABL_STATE) || (steer_ecu_state == STEER_ECU_CANCEL_SYS_STATE) || (steer_ecu_state == STEER_ECU_CANCEL_DRV_STATE) )
    {
        ret_val = TRUE;
    }

    return ret_val;
}

static inline bool_T isSteerEcuDisabled (uint8 steer_ecu_state)
{
    bool_T ret_val = FALSE;
    if(steer_ecu_state == STEER_ECU_DSABL_STATE)
    {
        ret_val = TRUE;
    }

    return ret_val;
}

static inline bool_T isSteerEcuCancel (uint8 steer_ecu_state)
{
    bool_T ret_val = FALSE;
    if(steer_ecu_state == STEER_ECU_CANCEL_SYS_STATE || steer_ecu_state == STEER_ECU_CANCEL_DRV_STATE)
    {
        ret_val = TRUE;
    }

    return ret_val;
}

static inline bool_T isSteerActive (uint8 steer_ecu_state)
{
    bool_T ret_val = FALSE;
    if(steer_ecu_state == STEER_ECU_CTRL_STATE)
    {
        ret_val = TRUE;
    }

    return ret_val;
}

static inline bool_T isNoPendingCancelReason(uint8 steer_abort_reason, uint8 last_steer_abort_reason)
{
    bool_T ret_val = FALSE;
    if(steer_abort_reason == STEER_ECU_ABORT_NONE)
    {
        ret_val = TRUE;
    }
    /* Steering Cancel reason was sent for at least two cycles */
    else if(steer_abort_reason == last_steer_abort_reason)
    {
        ret_val = TRUE;
    }

    return ret_val;
}

/***** state request to be send *******/
static inline uint8 getSteerInitRequestValue(void)
{
     return STEER_ECU_INIT_REQUEST;
}
static inline uint8 getSteerRdyRequestValue(void)
{
     return STEER_ECU_ENBL_REQUEST;
}
static inline uint8 getSteerActRequestValue(void)
{
    return STEER_ECU_CTRL_REQUEST;
}
static inline uint8 getSteerDsablRequestValue(void)
{
    return STEER_ECU_DSABL_REQUEST;
}
static inline uint8 getSteerCancelSysRequestValue(void)
{
    return STEER_ECU_CANCEL_SYS_REQUEST;
}


/******** Abort Reason ***************/
static inline uint8 getSteerNonAbortReasonValue(void)
{
    return STEER_ECU_ABORT_NONE;
}

/******************************************************************************/
/*                                            */
/* END  Source:         stmh_cus.c                                                  */
/*                                              */
/******************************************************************************/

