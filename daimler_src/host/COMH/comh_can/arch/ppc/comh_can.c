
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
/*   Source:   p2gpaleo_can.c       */
/*   Revision: 1.27.1.3.4       */
/*   Status:   INITIAL       */
/*   Author:   Julian Kreutz, Daniel Schuler, SWD       */
/*   Date:     14-JUL-2015 17:08:31      */
/*                                              */
/******************************************************************************/
/*   COMPONENT: Generic part CAN                                              */
/*   TARGET:    <All/Freescale S12X/...>                                      */
/******************************************************************************/
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/*                Include common and project definition header                */
/******************************************************************************/
#include "dstdbool.h"
#include "dstdint.h"
#include "dstring.h"
#include "dassert.h"
#include "tmp_pdf.h"                             /* Projectdefinitions        */
#include "dapm_tsk.h"
#include "comh.h"
#include "ptpn_apl.h"
/******************************************************************************/
/*                      Include headers of the component                      */
/******************************************************************************/
#include "comh_can.h"
#include "canwr.h"
/******************************************************************************/
/*                            Include other headers                           */
/******************************************************************************/




/******************************************************************************/
/*                        Definition of local constants                       */
/******************************************************************************/

/******************************************************************************/
/*                         Definition of local macros                         */
/******************************************************************************/

/******************************************************************************/
/*                         Definition of local types                          */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/*                       Definition of local variables                        */
/******************************************************************************/

/******************************************************************************/
/*                     Definition of local constant data                      */
/******************************************************************************/

/******************************************************************************/
/*                      Definition of exported variables                      */
/******************************************************************************/

/******************************************************************************/
/*                    Definition of exported constant data                    */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of local function prototypes                  */
/******************************************************************************/

static void Can1Receive (u16 id, const u8* data, u8 dlc);
static void Can1ReceiveExt(u32 id, const u8* data,u8 dlc);


static void Can0Receive (u16 id, const u8* data, u8 dlc);
static void Can0ReceiveExt(u32 id, const u8* data, u8 dlc);

/******************************************************************************/
/*                       Definition of local functions                        */
/******************************************************************************/


static void Can1Receive (u16 id, const u8* data, u8 dlc)
{
	CanReceive(id, data, dlc);
}
static void Can1ReceiveExt(u32 id, const u8* data, u8 dlc)
{
	CanReceiveExt(id, data, dlc);
}
static void Can0Receive (u16 id, const u8* data, u8 dlc)
{
	CanReceive(id, data, dlc);
}
static void Can0ReceiveExt(  u32 id, const u8* data,  u8 dlc)
{
	CanReceiveExt(id, data, dlc);
}
enu_p2gpa_can_trans_state_T P2GPA_GetCanTransState(void)
{
	CanWr_trans_state_E trans_state = CanWr_GetCanTransState();
	enu_p2gpa_can_trans_state_T p2gpa_trans_state;
	switch(trans_state)
	{
	case CANWR_TRANS_OFF_STATE:
		p2gpa_trans_state = P2GPA_CAN_TRANS_OFF_STATE;
		break;
	case CANWR_TRANS_ACTIVE_STATE:
		p2gpa_trans_state =  P2GPA_CAN_TRANS_ACTIVE_STATE;
		break;
	default:
		p2gpa_trans_state =CANWR_TRANS_OFF_STATE;
	}
	return p2gpa_trans_state;
}

/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/

void InitSCan(void)
{

	CanWR_CFG_T canwr_cfg;

		/*define Filters*/
		canwr_cfg.filterd_id_list[0] = 0xAA; /* Brk_Data_ESP */
		canwr_cfg.filterd_id_list[1] = 0xA6; /* Ign_Veh_Stat_ESP */
		canwr_cfg.filterd_id_list[2] = 0x9F; /* Park_Brk_Rs_Data_ESP */
		canwr_cfg.filterd_id_list[3] = 0xAF; /* Veh_Accel_Data_ESP */
		canwr_cfg.filterd_id_list[4] = 0x9E; /* Veh_Speed_Data_ESP */
		canwr_cfg.filterd_id_list[5] = 0xA8; /* Whl_Stat_Left_ESP */
		canwr_cfg.filterd_id_list[6] = 0xA7; /* Whl_Stat_Right_ESP */
		canwr_cfg.filterd_id_list[7] = 0xA2; /* Gr_Current_Gear_CPC */
		canwr_cfg.filterd_id_list[8] = 0xA5; /* PwrTr_Stat_CPC */
		canwr_cfg.filterd_id_list[9] = 0xAE; /* TSL_Target_Pos_CPC */
		canwr_cfg.filterd_id_list[10] = 0xAB; /* Buttons_Data_EIS */
		canwr_cfg.filterd_id_list[11] = 0xA0; /* Electronic_Brk_Eng */
		canwr_cfg.filterd_id_list[12] = 0xA1; /* Park_St_Rs_EPS*/
		canwr_cfg.filterd_id_list[13] = 0xAD;  /* Steering_Data_EPS */
		canwr_cfg.filterd_id_list[14] = 0xAC;  /* Turn_Indicators_Data_EIS */
		canwr_cfg.filterd_id_list[15] = 0x98; /* VehDyn_Stat2_ESP */

		/* Not used for Daimler BR213 */
		canwr_cfg.filterd_id_list[16] = 0x51B; /* CLU_16 */
		canwr_cfg.filterd_id_list[17] = 0x520;	/* CGW3 */
		canwr_cfg.filterd_id_list[18] = 0x541;	/* CGW1 */
		canwr_cfg.filterd_id_list[19] = 0x553;	/* CGW2 */
		canwr_cfg.filterd_id_list[20] = 0x600;	/* PCA */
		canwr_cfg.filterd_id_list[21] = 0x502;//0x644;	/* RSPA_C2 */
		canwr_cfg.filterd_id_list[22] = 0x100;	/* P4U btns sim */
		canwr_cfg.filterd_id_list[23] = XP2GPA_CAN_ID_PARA_RECEIVE;//0x100;//XISTP_RESP_CAN_ID;
		canwr_cfg.filterd_id_list[24] = 0x3A5;
		canwr_cfg.filterd_id_list[25] = 0x766; /*PCA_USS*/

		canwr_cfg.filterd_id_list[26] = 0x6B7; /*PCA_USS*/
		canwr_cfg.num_filtered_id = 27;
		/*Set Tx confirmation Callback*/

		canwr_cfg.call_back_list[0].tx_cmplt_call_back = PTPN_Apl_OnDataSent;
		canwr_cfg.call_back_list[0].msg_id = 0x665;
		canwr_cfg.num_call_back = 1;

		canwr_cfg.channel_id = CanWR_CHANNEL_ID_CAN1;
		canwr_cfg.RecvCbk = Can1Receive;
		canwr_cfg.RecvCbkExt = Can1ReceiveExt ;

		CanWR_Init(&canwr_cfg);
}


u8 P2GPA_CanSendDebugCh (u16 id, const u8 *data, u8 dlc)
{
	return CanWR_Tx(id, data, dlc, CanWR_CHANNEL_ID_CAN1);
}

u8 P2GPA_CanSend (enum P2GPA_CAN_prio_E prio, u16 id, const u8 *p, u8 n)
{
    return CanWR_Tx(id, p, n, CanWR_CHANNEL_ID_CAN1);
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/

