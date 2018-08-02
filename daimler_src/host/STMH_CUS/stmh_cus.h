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
/*      Source:         stmh_cus.h                                                  */
/*      Revision:       1.1.1.4                                                  */
/*      Author:         kistner_andreas                                       */
/*      UserID:         BRODRIG2                                                 */
/*      Date            14-JUL-2015 17:13:15                                                 */
/*                                              */
/******************************************************************************/
/*   MODULE: STMH_CUS (customer specific code (steering communication) of the */
/*                     STMH-module)                                           */
/*   FILE: STMH_CUS.H                                                         */
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
#ifndef I_STMH_CUS_H
#define I_STMH_CUS_H

#include "dstdbool.h"
#include "dstdint.h"
#include "platform_types.h"


/******************************************************************************/
/*                 Definition of exported symbolic constants                  */
/******************************************************************************/

/******************************************************************************/
/*                Definition of exported function like macros                 */
/******************************************************************************/

/******************************************************************************/
/*          Definition of exported types (typdefs, enums, struct, union)      */
/******************************************************************************/
struct STMH_cus_input_S
{
    u8 state_eps;
    u8 eps_timestamp;
    u8 pla_termination_off_eps;
};
enum stmh_app_states_E
{
	STMH_APP_INIT = 0,
	STMH_APP_READY,
	STMH_APP_STEERING_ACTIVE,
	STMH_APP_ABORT,
	STMH_APP_STEER_CANCEL
};

enum stmh_app_sub_states_E
{
  STMH_APP_SUB_REQ = 0,
  STMH_APP_SUB_ACKN
};


/******************************************************************************/
/*                    Declaration of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of exported constant data                     */
/******************************************************************************/

/******************************************************************************/
/*               Declaration of exported function prototypes                  */
/******************************************************************************/

void                       STMH_CusInit(void);
void                       STMH_CusSteeringStateMachine(void);

u8                         STMH_CusGetStateEpsSys(bool_T * p_lat_ext_sys_state);

u8                         STMH_CusGetAppStateToSteering(void);
u8                         STMH_CusGetSteeringAbortReason(void);
enum STMH_abort_reason_E   STMH_CusGetAbortReason(void);
void                       STMH_CusResetAbortReason(void);

bool_T                     STMH_CusIsSteeringActive(void);
bool_T                     STMH_CusIsSteeringReady(void);
bool_T                     STMH_CusActivateSteer(void);
void                       STMH_CusDeactivateSteer(void);
u8 STMH_GetActiveSubState(void);
u8 STMH_GetMainState(void);

#endif /* I_STMH_CUS_H */
/******************************************************************************/
/*                                            */
/* END  Source:         stmh_cus.h                                                  */
/*                                              */
/******************************************************************************/
