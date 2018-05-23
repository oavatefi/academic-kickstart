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
#include "canwr.h"
#include "comh.h"
#include "ptpn_apl.h"
/******************************************************************************/
/*                      Include headers of the component                      */
/******************************************************************************/
#include "comh_can.h"
/******************************************************************************/
/*                            Include other headers                           */
/******************************************************************************/




/******************************************************************************/
/*                        Definition of local constants                       */
/******************************************************************************/
#ifdef P2GPA_CAN_WARNELMS_SEND
# ifndef P2GPA_CAN_ID_DEV_31
# error P2GPA_CAN_ID_DEV_31 has to be defined
# endif /* XP2GPA_CAN_WARNELEMS_ID*/
#endif

enum CFG_send_warnelems_S
{
    /* no warn elements sent */
    CFG_WE_DISABLED = 0,
    /* warn elements from USFE sent */
    CFG_WE_SEND_GMAP,
    /* warn elements from DOXY sent */
    CFG_WE_SEND_DOXY,
    /* warn elements from PSEG sent */
    CFG_WE_SEND_PSEG,
    /* warn elements from PCLU(lines) sent */
    CFG_WE_SEND_LINE,
    CFG_WE_CNT
};

#define P2GPA_NUM_SIGNALWAYS            16
#define P2GPA_NUM_SIGNALWAYS_TOTAL      2*P2GPA_NUM_SIGNALWAYS
#define P2GPA_NUM_DISTANCES_PSM_SENS    2

#define P2GPA_RECV_E1                   1
#define P2GPA_RECV_E2                   2
#define P2GPA_RECV_E3                   4
#define P2GPA_ALL_ECHOS_RECV            7

/* definition of all signal ways */
#define P2GPA_SGW_FSL       0
#define P2GPA_SGW_FOL_SL    1
#define P2GPA_SGW_FSL_OL    2
#define P2GPA_SGW_FOL       3
#define P2GPA_SGW_FOL_ML    4
#define P2GPA_SGW_FML_OL    5
#define P2GPA_SGW_FML       6
#define P2GPA_SGW_FML_MR    7
#define P2GPA_SGW_FMR_ML    8
#define P2GPA_SGW_FMR       9
#define P2GPA_SGW_FMR_OR    10
#define P2GPA_SGW_FOR_MR    11
#define P2GPA_SGW_FOR       12
#define P2GPA_SGW_FOR_SR    13
#define P2GPA_SGW_FSR_OR    14
#define P2GPA_SGW_FSR       15
#define P2GPA_SGW_RSR       16
#define P2GPA_SGW_ROR_SR    17
#define P2GPA_SGW_RSR_OR    18
#define P2GPA_SGW_ROR       19
#define P2GPA_SGW_RMR_OR    20
#define P2GPA_SGW_ROR_MR    21
#define P2GPA_SGW_RMR       22
#define P2GPA_SGW_RML_MR    23
#define P2GPA_SGW_RMR_ML    24
#define P2GPA_SGW_RML       25
#define P2GPA_SGW_ROL_ML    26
#define P2GPA_SGW_RML_OL    27
#define P2GPA_SGW_ROL       28
#define P2GPA_SGW_RSL_OL    29
#define P2GPA_SGW_ROL_SL    30
#define P2GPA_SGW_RSL       31

#define P2GPA_MAX_NB_WARN_ELMT_MSG 5

/******************************************************************************/
/*                         Definition of local macros                         */
/******************************************************************************/
#ifndef XP2GPA_CAN_RECEIVE
#define XP2GPA_CAN_RECEIVE                  COMH_CanReceive
#endif
#ifndef XP2GPA_CAN_RECEIVE_EXT
#define XP2GPA_CAN_RECEIVE_EXT				/*tbd*///COMH_CanReceiveExt
#endif
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
static enum CFG_send_warnelems_S warnelems_send_cfg = CFG_WE_DISABLED;

/******************************************************************************/
/*                      Definition of exported variables                      */
/******************************************************************************/

/******************************************************************************/
/*                    Definition of exported constant data                    */
/******************************************************************************/

/******************************************************************************/
/*                  Declaration of local function prototypes                  */
/******************************************************************************/
static void CanReceive (u16 id, const u8* data, u8 dlc );
static void CanReceiveExt (  u16 id_a,u32 id_b, const u8* data,u8 dlc);;

static void Can1Receive (u16 id, const u8* data, u8 dlc);
static void Can1ReceiveExt(  u16 id_a,u32 id_b, const u8* data,u8 dlc);
static void Can0Receive (u16 id, const u8* data, u8 dlc);
static void Can0ReceiveExt(u16 id_a, u32 id_b, const u8* data, u8 dlc);
static void InitLinuxCan(void);
/******************************************************************************/
/*                       Definition of local functions                        */
/******************************************************************************/
static void InitLinuxCan(void)
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
	canwr_cfg.channel_id = CanWR_CHANNEL_ID_CAN0;
	canwr_cfg.RecvCbk = Can0Receive ;
	canwr_cfg.RecvCbkExt = Can0ReceiveExt ;
	CanWR_Init(&canwr_cfg);
}


static void Can1Receive (u16 id, const u8* data, u8 dlc)
{
	CanReceive(id, data, dlc);
}
static void Can1ReceiveExt(u16 id_a, u32 id_b, const u8* data, u8 dlc)
{
	CanReceiveExt(id_a, id_b, data, dlc);
}
static void Can0Receive (u16 id, const u8* data, u8 dlc)
{
	CanReceive(id, data, dlc);
}
static void Can0ReceiveExt( u16 id_a, u32 id_b, const u8* data,  u8 dlc)
{
	CanReceiveExt(id_a, id_b, data, dlc);
}
static void CanReceiveExt ( u16 id_a, u32 id_b, const u8* data,  u8 dlc)
{
	/*Insert Logic */
	//XP2GPA_CAN_RECEIVE_EXT(id_a, id_b, data, dlc);
}
static void CanReceive (u16 id, const u8* data, u8 dlc )
{

#ifdef XAPPL_VS6_ECHO_SIM
	u8 echo_number;
	u8 echo_number_mask;
	u8 multiplexor;
	u16 tmp_u16=0;
	bool_T echo_distance_received = FALSE;
	u8 recv_sgws[4];
#endif

	/* only messages with 8 bytes                                             */
	if (dlc == 8)
	{
		switch (id)
		{
#ifdef XP2GPA_REC_THRESHOLDS
		case 0x400: /* Thresholds from VS6 */
			for (i=0; i<8; i++)
			{
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_side_psm_upa_t1[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_side_psm_upa_t2[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_side_psm_upa_t3[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_side_psm_upa_t4[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_side_psm_upa_t5[i] = data[i];
			}
			break;
		case 0x401: /* Thresholds from VS6 */
			for (i=0; i<8; i++)
			{
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_outer_upa_t1[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_outer_upa_t2[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_outer_upa_t3[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_outer_upa_t4[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_outer_upa_t5[i] = data[i];
			}
			break;
		case 0x402: /* Thresholds from VS6 */
			for (i=0; i<8; i++)
			{
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_inner_upa_t1[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_inner_upa_t2[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_inner_upa_t3[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_inner_upa_t4[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.front_inner_upa_t5[i] = data[i];
			}
			break;
		case 0x403: /* Thresholds from VS6 */
			for (i=0; i<8; i++)
			{
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_side_psm_upa_t1[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_side_psm_upa_t2[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_side_psm_upa_t3[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_side_psm_upa_t4[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_side_psm_upa_t5[i] = data[i];
			}
			break;
		case 0x404: /* Thresholds from VS6 */
			for (i=0; i<8; i++)
			{
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_outer_upa_t1[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_outer_upa_t2[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_outer_upa_t3[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_outer_upa_t4[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_outer_upa_t5[i] = data[i];
			}
			break;
		case 0x405: /* Thresholds from VS6 */
			for (i=0; i<8; i++)
			{
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_inner_upa_t1[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_inner_upa_t2[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_inner_upa_t3[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_inner_upa_t4[i] = data[i];
				CONF_coding_dataset_ram.das_cfg.ulsm_cfg.thresholds_cfg.rear_inner_upa_t5[i] = data[i];
			}
			break;
		case 0x406:
			if (data[0] == 0x1)
			{
				P2GPA_ActSetULSActiveState(PDC_RE_INIT);
			}
			if (data[0] == 0x2)
			{
				P2GPA_ActSetULSActiveState(PDC_ACTIVE);
				P2GPA_ActSetULSWarnState(PDC_ACTIVE);
			}
			break;
#endif
		case XCANH_ID_MAP: /* Map from VS6*/
		P2GPA_CanMapReceive(data);
		break;

#ifdef XAPPL_VS6_ECHO_SIM
		case XP2GPA_CAN_ID_SIGNALS00:
			echo_distance_received = TRUE;
			recv_sgws[0] = P2GPA_SGW_FSL;
			recv_sgws[1] = P2GPA_SGW_FSL_OL;
			recv_sgws[2] = P2GPA_SGW_FOL_SL;
			recv_sgws[3] = P2GPA_SGW_FOL;

			/* read multiplexor */
			multiplexor = data[0] & 0b000011;

			if(0 == multiplexor)
			{
				echo_number = 0;
				echo_number_mask = P2GPA_RECV_E1;
			}
			else if(1 == multiplexor)
			{
				echo_number = 1;
				echo_number_mask = P2GPA_RECV_E2;
			}
			else
			{
				echo_number = 2;
				echo_number_mask = P2GPA_RECV_E3;
			}
			break;

		case XP2GPA_CAN_ID_SIGNALS01:
			echo_distance_received = TRUE;
			recv_sgws[0] = P2GPA_SGW_FOL_ML;
			recv_sgws[1] = P2GPA_SGW_FML_OL;
			recv_sgws[2] = P2GPA_SGW_FML;
			recv_sgws[3] = P2GPA_SGW_FML_MR;

			/* read multiplexor */
			multiplexor = data[0] & 0b000011;

			if(0 == multiplexor)
			{
				echo_number = 0;
				echo_number_mask = P2GPA_RECV_E1;
			}
			else if(1 == multiplexor)
			{
				echo_number = 1;
				echo_number_mask = P2GPA_RECV_E2;
			}
			else
			{
				echo_number = 2;
				echo_number_mask = P2GPA_RECV_E3;
			}
			break;

		case XP2GPA_CAN_ID_SIGNALS02:
			echo_distance_received = TRUE;
			recv_sgws[0] = P2GPA_SGW_FMR_ML;
			recv_sgws[1] = P2GPA_SGW_FMR;
			recv_sgws[2] = P2GPA_SGW_FMR_OR;
			recv_sgws[3] = P2GPA_SGW_FOR_MR;

			/* read multiplexor */
			multiplexor = data[0] & 0b000011;

			if(0 == multiplexor)
			{
				echo_number = 0;
				echo_number_mask = P2GPA_RECV_E1;
			}
			else if(1 == multiplexor)
			{
				echo_number = 1;
				echo_number_mask = P2GPA_RECV_E2;
			}
			else
			{
				echo_number = 2;
				echo_number_mask = P2GPA_RECV_E3;
			}
			break;

		case XP2GPA_CAN_ID_SIGNALS03:
			echo_distance_received = TRUE;
			recv_sgws[0] = P2GPA_SGW_FOR;
			recv_sgws[1] = P2GPA_SGW_FOR_SR;
			recv_sgws[2] = P2GPA_SGW_FSR_OR;
			recv_sgws[3] = P2GPA_SGW_FSR;

			/* read multiplexor */
			multiplexor = data[0] & 0b000011;

			if(0 == multiplexor)
			{
				echo_number = 0;
				echo_number_mask = P2GPA_RECV_E1;
			}
			else if(1 == multiplexor)
			{
				echo_number = 1;
				echo_number_mask = P2GPA_RECV_E2;
			}
			else
			{
				echo_number = 2;
				echo_number_mask = P2GPA_RECV_E3;
			}
			break;

		case XP2GPA_CAN_ID_SIGNALS04:
			echo_distance_received = TRUE;
			recv_sgws[0] = P2GPA_SGW_RSL;
			recv_sgws[1] = P2GPA_SGW_RSL_OL;
			recv_sgws[2] = P2GPA_SGW_ROL_SL;
			recv_sgws[3] = P2GPA_SGW_ROL;

			/* read multiplexor */
			multiplexor = data[0] & 0b000011;

			if(0 == multiplexor)
			{
				echo_number = 0;
				echo_number_mask = P2GPA_RECV_E1;
			}
			else if(1 == multiplexor)
			{
				echo_number = 1;
				echo_number_mask = P2GPA_RECV_E2;
			}
			else
			{
				echo_number = 2;
				echo_number_mask = P2GPA_RECV_E3;
			}
			break;

		case XP2GPA_CAN_ID_SIGNALS05:
			echo_distance_received = TRUE;
			recv_sgws[0] = P2GPA_SGW_ROL_ML;
			recv_sgws[1] = P2GPA_SGW_RML_OL;
			recv_sgws[2] = P2GPA_SGW_RML;
			recv_sgws[3] = P2GPA_SGW_RML_MR;

			/* read multiplexor */
			multiplexor = data[0] & 0b000011;

			if(0 == multiplexor)
			{
				echo_number = 0;
				echo_number_mask = P2GPA_RECV_E1;
			}
			else if(1 == multiplexor)
			{
				echo_number = 1;
				echo_number_mask = P2GPA_RECV_E2;
			}
			else
			{
				echo_number = 2;
				echo_number_mask = P2GPA_RECV_E3;
			}
			break;

		case XP2GPA_CAN_ID_SIGNALS06:
			echo_distance_received = TRUE;
			recv_sgws[0] = P2GPA_SGW_RMR_ML;
			recv_sgws[1] = P2GPA_SGW_RMR;
			recv_sgws[2] = P2GPA_SGW_RMR_OR;
			recv_sgws[3] = P2GPA_SGW_ROR_MR;

			/* read multiplexor */
			multiplexor = data[0] & 0b000011;

			if(0 == multiplexor)
			{
				echo_number = 0;
				echo_number_mask = P2GPA_RECV_E1;
			}
			else if(1 == multiplexor)
			{
				echo_number = 1;
				echo_number_mask = P2GPA_RECV_E2;
			}
			else
			{
				echo_number = 2;
				echo_number_mask = P2GPA_RECV_E3;
			}
			break;

		case XP2GPA_CAN_ID_SIGNALS07:
			echo_distance_received = TRUE;
			recv_sgws[0] = P2GPA_SGW_ROR;
			recv_sgws[1] = P2GPA_SGW_ROR_SR;
			recv_sgws[2] = P2GPA_SGW_RSR_OR;
			recv_sgws[3] = P2GPA_SGW_RSR;

			/* read multiplexor */
			multiplexor = data[0] & 0b000011;

			if(0 == multiplexor)
			{
				echo_number = 0;
				echo_number_mask = P2GPA_RECV_E1;
			}
			else if(1 == multiplexor)
			{
				echo_number = 1;
				echo_number_mask = P2GPA_RECV_E2;
			}
			else
			{
				echo_number = 2;
				echo_number_mask = P2GPA_RECV_E3;
			}
			break;
#endif
		}

	}

#ifdef APPL_VS6_ECHOES_CAN
	VS6_EchoesReceive(id, data, dlc);
#endif

#if (F_DAS_ULSD_SENS_SIMUL_MODE_1 == F_DAS_ON)
	if(echo_distance_received)
	{
		u8 i;
		/* set current received echo massage */

		echo_distance_received = FALSE;

		tmp_u16 = data[0]>>2;
		tmp_u16 |= ((u16)data[1])<<6;
		P2GPA_sgws_dists_mm[recv_sgws[0]][echo_number]    = tmp_u16;

		tmp_u16 = data[2];
		tmp_u16 |= ((u16)(data[3] & 0b00111111))<<8;
		P2GPA_sgws_dists_mm[recv_sgws[1]][echo_number]    = tmp_u16;

		tmp_u16 = data[3]>>6;
		tmp_u16 |= ((u16)data[4])<<2;
		tmp_u16 |= ((u16)(data[5] & 0b00001111))<<10;
		P2GPA_sgws_dists_mm[recv_sgws[2]][echo_number]    = tmp_u16;

		tmp_u16 = data[5]>>4;
		tmp_u16 |= ((u16)data[6])<<4;
		tmp_u16 |= ((u16)data[7])<<12;
		P2GPA_sgws_dists_mm[recv_sgws[3]][echo_number]    = tmp_u16;

		/* update recv mask */
		for(i=0; i<4; ++i)
		{
			P2GPA_sgws_recv_echos_mask[recv_sgws[i]] |= echo_number_mask;

			if(P2GPA_sgws_recv_echos_mask[recv_sgws[i]] == P2GPA_ALL_ECHOS_RECV)
			{
				SetSDBGDists(recv_sgws[i], &P2GPA_sgws_dists_mm[recv_sgws[i]][0]);

				/* reset mask */
				P2GPA_sgws_recv_echos_mask[recv_sgws[i]] = 0;
			}
		}
	}
#endif

	if (id == 0x778)
	{
		/*DAPM_SetSimulatedSlot(NULL, data[0]); NOT YET IMPLEMENTED IN DAPM*/
	}

	if (id == 0x780)
	{
		ACTL_ReceiveChosenSlotData(data);
	}

	if (id == XP2GPA_CAN_ID_PARA_RECEIVE)
	{
		ISTP_CanReceive(data, dlc);
	}
	PTPN_Apl_OnDataReceived(id, data, dlc);
	XP2GPA_CAN_RECEIVE (id, data, dlc);
}
/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/

/******************************************************************************/
/*                              queue functions                               */
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/******************************************************************************/


u8 P2GPA_CanSendDebugCh (u16 id, const u8 *data, u8 dlc)
{
	CanWR_Tx(id, data, dlc);
}

void P2GPA_CanSend (enum P2GPA_CAN_prio_E prio, u16 id, const u8 *p, u8 n)
{
	CanWR_Tx(id, p, n);
}


void P2GPA_CanInit (void)
{
	InitLinuxCan();
}
#ifndef TMPL_USE_FRAY
enu_p2gpa_can_trans_state_T P2GPA_GetCanTransState(void)
{

}
void P2GPA_UpdateTranceiverStatus(void)
{

}
#endif

u8 P2GPA_RecSendCanData(u16 id, const u8* data, u8 dlc)
{

}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/



/******************************************************************************/
/* Callback-.function for ULSD2.1: Connect to XSECO_RecSendCanData            */
/* via PDF-Header                                                             */
/******************************************************************************/

/******************************************************************************/

/******************************************************************************/
/* MAP protokoll functions                                                    */
/******************************************************************************/
void P2GPA_CanMapReceive(const u8 *p)
{
    /* in Gen3.0 used to get the warn elements from different points in the system */
    u16 tmp;

    tmp = (u16)p[0];
    switch (tmp)
    {
    case XCANH_MAPMUX_CMD:
        /* read command code */
        tmp = (p[2] + (p[3] * 256));
        switch (tmp)
        {
        case XCANH_CMD_CODE_NO_CMD:
            break;
        case XCANH_CMD_CODE_INIT:
            warnelems_send_cfg = CFG_WE_DISABLED;
#ifdef DAPM_OBJD_ENABLED
            /* clear the map */
            P2DAL_ClearMap();
#endif
            break;
        case XCANH_CMD_CODE_REQ_MAP:
            warnelems_send_cfg = (enum CFG_send_warnelems_S) (((u8)warnelems_send_cfg) + 1);
            if (warnelems_send_cfg == CFG_WE_CNT)
            {
                warnelems_send_cfg = CFG_WE_DISABLED;
            }
            break;
        case XCANH_CMD_CODE_REINIT_DAPM:

            /*DAPM_ReInit();*/
            /*the below line are the replacements lines for the above Re-init function interface*/
            P2DAL_Shutdown();

            /*TODO: not use get interface from conf but ask conf to re-config DAS*/
			 if(CONF_IsConfigEepromMirrorValid() == TRUE )
        	{
            	P2DAL_SetDasCfg(CONF_GetCongifStruct());
        	}

            break;
        }
        break;
    }
}

/******************************************************************************/

/******************************************************************************/
/* Stubs for CCAN                                                             */
/******************************************************************************/

/******************************************************************************/
/******************************************************************************/
/****************************************************/
// CODE TO REMOVE
/****************************************************/

/******************************************************************************/
/*  CAN-Communication of the EXIT test mode                                   */
/*  Use the canalyzer data base:                                              */
/******************************************************************************/
