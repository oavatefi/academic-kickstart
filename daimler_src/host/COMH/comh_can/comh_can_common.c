/*
 * comh_can_common.c
 *
 *  Created on: May 24, 2018
 *      Author: ysameh
 */

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
#include "capp.h"
#include "gl2d.h"
#include "mtli.h"
#include "p2gpaleo.h"
#include "actl.h"
#include "comh_can.h"
#include "ptpn_apl.h"
#include "comh.h"
#include "timr.h"
#include "odom.h"

#include "ptpn.h"

#ifdef APPL_VS6_ECHOES_CAN
#include "ulsm.h"
#include "vs6_can.h"
#endif

#include "conf.h"
#include "istp.h"
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
#ifdef APPL_ENABLE_SEND_ODOM_INFO
struct CAPP_pos_S odom_pos = {{0,0},{0,0,0,0},0, 0, 0} ;
#endif
static enum CFG_send_warnelems_S warnelems_send_cfg = CFG_WE_DISABLED;
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

#if (F_DAS_ULSD_SENS_SIMUL_MODE_1 == F_DAS_ON)

static u16 P2GPA_sgws_dists_mm[P2GPA_NUM_SIGNALWAYS_TOTAL][3];
static u8  P2GPA_sgws_recv_echos_mask[P2GPA_NUM_SIGNALWAYS_TOTAL];

#endif
#if (F_DAS_ULSD_SENS_SIMUL_MODE_1 == F_DAS_ON)
static void SetSDBGDists(u8 sgw, u16* p_dists_mm);
#endif


/******************************************************************************/
/*                       Definition of local functions                        */
/******************************************************************************/



#if (F_DAS_ULSD_SENS_SIMUL_MODE_1 == F_DAS_ON)
static void SetSDBGDists(u8 sgw, u16* p_dists_mm)
{
    u8 sender;
    u8 receiver;
    u8 mod3_sgw;
    u8 sgw_tmp;
    u8 num_echos = 3;

    /* signal ways front */
    if(sgw < 16)
    {
        mod3_sgw = sgw%3;
        if(0 == mod3_sgw)
        {
            sender = sgw;
            receiver = sgw;
        }
        else if(1 == mod3_sgw)
        {
            sender = sgw-1;
            receiver = sgw+2;

        }
        else /* mod3_sgw == 2 */
        {
            sender = sgw+1;
            receiver = sgw-2;
        }

        sender /= 3;
        receiver /= 3;
    }
    else
    {
        /* signal ways rear */
        /* temporary decrement sgw */
        sgw_tmp = sgw-1;

        mod3_sgw = sgw_tmp%3;
        if(0 == mod3_sgw)
        {
            sender = sgw;
            receiver = sgw;
        }
        else if(1 == mod3_sgw)
        {
            sender = sgw-1;
            receiver = sgw+2;

        }
        else /* mod3_sgw == 2 */
        {
            sender = sgw+1;
            receiver = sgw-2;
        }

        sender /= 3;
        sender += 1;
        receiver /= 3;
        receiver += 1;
    }

    if(p_dists_mm[2] == 0)
    {
        num_echos = 2;
    }
    if(p_dists_mm[1] == 0)
    {
        num_echos = 1;
    }
    if(p_dists_mm[0] == 0)
    {
        num_echos = 0;
    }

    if(ULSM_MODE_ACTIVE == ULSM_GetMode())
    {
        SDBG_InpUpacSetDistance(sender, receiver, p_dists_mm, num_echos);
    }
}
#endif

/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/

void P2GPA_CanInit (void)
{

#if (F_DAS_ULSD_SENS_SIMUL_MODE_1 == F_DAS_ON)
    u8 temp_i;
    for(temp_i=0; temp_i<P2GPA_NUM_SIGNALWAYS_TOTAL; ++temp_i)
    {
        P2GPA_sgws_dists_mm[temp_i][0] = U16_MAX;
        P2GPA_sgws_dists_mm[temp_i][1] = U16_MAX;
        P2GPA_sgws_dists_mm[temp_i][2] = U16_MAX;
        P2GPA_sgws_recv_echos_mask[temp_i] = 0;
    }
#endif
#ifdef TMPL_USE_SCAN
#  ifndef TMPL_USE_FRAY
    	CanSPIInit();
#  endif
	InitSCan();
#endif

#ifdef USE_LINUX_CAN
	InitLinuxCan();
#endif
}
void CanReceiveExt ( u16 id_a, u32 id_b, const u8* data,  u8 dlc)
{
	/*Insert Logic */
	//XP2GPA_CAN_RECEIVE_EXT(id_a, id_b, data, dlc);
}
void CanReceive (u16 id, const u8* data, u8 dlc )
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
#ifdef APPL_ENABLE_SEND_ODOM_INFO
void P2GPA_CanOdomInfoSend(void)
{
    u8 buff[8];
    si16 temp_si16;
    u16 temp_u16;

    _CLR_ARRAY(buff);

    if (P2DAL_GetState!=DAPM_STATE_ERROR)
    {
        ODOM_UpdatePosition(&odom_pos);

        temp_si16 = odom_pos.coord_mm.x;


        buff[0] = _GET_LOW_BYTE(odom_pos.coord_mm.x);
        buff[1] = _GET_HIGH_BYTE(odom_pos.coord_mm.x);

        buff[2] = _GET_LOW_BYTE(odom_pos.coord_mm.y);
        buff[3] = _GET_HIGH_BYTE(odom_pos.coord_mm.y);

        buff[4] = _GET_LOW_BYTE(odom_pos.theta_u16f);
        buff[5] = _GET_HIGH_BYTE(odom_pos.theta_u16f);

        temp_u16 = (u16) (odom_pos.timestamp_512us & 0xFFFF);

        buff[6] = _GET_LOW_BYTE(temp_u16);
        buff[7] = _GET_HIGH_BYTE(temp_u16);

        P2GPA_CanSend(P2GPA_CAN_prio_high, XP2GPA_ODOM_INFO_TO_CAN_ID, buff, sizeof(buff));
    }

}
#endif




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
