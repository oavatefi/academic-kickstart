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

/******************************************************************************/
/*                      Include headers of the component                      */
/******************************************************************************/
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
/*                            Include other headers                           */
/******************************************************************************/

#ifdef TMPL_USE_SCAN
#include "scan.h"
#include "hwpl.h"
#else
#include "fcan.h"
#include "ccan.h"
#endif

#include "hmih.h"
#ifndef	TMPL_USE_FRAY
#include "tmp_pdf.h"
#include "gspi.h"
#endif


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
#define P2GPA_CANILOG_NR_ERRORS 10

#define P2GPA_TIMESTAMP_SGW_OFFSET_2US 14

#define RAM_DAPM_PARA CONF_coding_dataset_ram.das_cfg.dapm_cfg

#ifndef	TMPL_USE_FRAY
#define MC_ADDR                 	 0x01
#define MAIN_STATUS_ADDR  			 0x03
#define TRANSCEIVER_STATUS_ADDR 	 0x22
#define TRANSCEIVER_CTRL_ADDR     	 0x20
#define WRITE_OPERATION              0x00
#define READ_OPERATION               0x01
#define TRANSCEIVER_NORMAL_MODE      0x07
#define CTS_MASK       				 0x80
static u8 recv_buf[XCAN_BUFFER_SIZE] = {0};
static u8 trans_buf[XCAN_BUFFER_SIZE] = {0};
static enu_p2gpa_can_trans_state_T P2GPA_Can_Trans_State = P2GPA_CAN_TRANS_OFF_STATE ;
#endif
#ifdef TMPL_USE_SCAN
/* TODO-AO: only defined for compatibility to switch between CCAN or SCAN */
struct P2GPA_CAN_msg_S
{
    /**< Message buffer's data array.\n                                       */
    u8* data;
    /**< Message buffer's ID.
     *   In case of Standard frame: id_l used
     */
    u16 id_l;
    /**< Channel bus nummber.                                                 */
    //u8 channel;
    /**< Message buffer's data length.\n                                      */
    u8 dlc;
    /**< In case of Extended frame: id_h and id_l used.                       */
    //u16 id_h;
    /**< Message type ID enumuration.                                         */
    //enum CCAN_msg_id_type_E id_type;
    /**< Timestamp value for the requested message.                           */
    //u32 timestamp_512us;
};
#else /* #ifdef TMPL_USE_SCAN */
/******************************************************************************/
/*                         Definition of local types                          */
/******************************************************************************/
struct P2GPA_CAN_entry_S {
    u16 uiID;                                  /* CAN ID                      */
    u8  pBuffer[8];                            /* max. 8 data bytes           */
    u8  ucCount;                               /* data count [0...8]          */
};
/******************************************************************************/

struct P2GPA_CAN_queue_S {
    /* FIFO buffers                */
    struct P2GPA_CAN_entry_S pEntry[XP2GPA_CAN_QUEUE_SIZE];
    u16 uiPosFirst;                            /* first data in the FIFO      */
    u16 uiPosNext;                             /* last data in the FIFO       */
    u16 uiCount;                               /* fill stand                  */
    u8  bOverflow;                             /* buffer overflow is market   */
};
/******************************************************************************/
static struct CCAN_msg_S g_can_msg_send;       /* structure for CCAN_Send */

/******************************************************************************/
/*                       Definition of local variables                        */
/******************************************************************************/
static struct P2GPA_CAN_queue_S  P2GPA_CanQueue[P2GPA_CAN_prio_max];
#endif /* #ifdef TMPL_USE_SCAN */

#if (F_DAS_ULSD_SENS_SIMUL_MODE_1 == F_DAS_ON)

static u16 P2GPA_sgws_dists_mm[P2GPA_NUM_SIGNALWAYS_TOTAL][3];
static u8  P2GPA_sgws_recv_echos_mask[P2GPA_NUM_SIGNALWAYS_TOTAL];

#endif

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
#ifndef	TMPL_USE_FRAY
void P2GPA_CanSPITransmissionComplete(void)
{
	CAN_SPI_CS_HIGH();
	if(recv_buf[0] == ((TRANSCEIVER_STATUS_ADDR << 1 ) | READ_OPERATION))
	{
		if( (recv_buf[1] & CTS_MASK) != 0 )
		{
			P2GPA_Can_Trans_State = P2GPA_CAN_TRANS_ACTIVE_STATE;
		}
		else
		{
			P2GPA_Can_Trans_State = P2GPA_CAN_TRANS_OFF_STATE ;
		}
	}
}
#endif
/* static void ClearArray(u8 *p, u8 length); */

/* prototypes for the callback functions                                      */
void XP2GPA_CAN_RECEIVE (u16 id, const u8 *p, u8 n);


#ifdef TMPL_USE_SCAN
void P2GPA_InitSCan(void);
#else
void P2GPA_InitCCan(void);
#endif

#if (F_DAS_ULSD_SENS_SIMUL_MODE_1 == F_DAS_ON)
static void SetSDBGDists(u8 sgw, u16* p_dists_mm);
#endif

#ifndef	TMPL_USE_FRAY
static u8 CanSPIInit(void);
static void ActivateCanTransceiver(void);
#endif
/******************************************************************************/
/*                       Definition of local functions                        */
/******************************************************************************/
#ifndef	TMPL_USE_FRAY
static u8 CanSPIInit(void)
{
    static GSPI_Config_Type_S transportlayer_cfg;
    u8     return_status = E_NOT_OK;
    bool_T error_status  = FALSE;

    transportlayer_cfg.TransmissionCompleteFunction = &P2GPA_CanSPITransmissionComplete;
    transportlayer_cfg.system_clock                 = XCAN_SPI_SYSTEM_CLOCK;
    transportlayer_cfg.baudrate                     = XCAN_SPI_BAUDRATE;
    transportlayer_cfg.spi_channel                  = XCAN_SPI_CHANNEL;
    transportlayer_cfg.dma_channel_transmit         = XCAN_DMA_CHANNEL_TRANSMIT;
    transportlayer_cfg.dma_channel_receive          = XCAN_DMA_CHANNEL_RECEIVE;
    transportlayer_cfg.dma_channel_tx_prep          = XCAN_DMA_CHANNEL_TX_PREPARE;
    transportlayer_cfg.data_width                   = XCAN_SPI_DATA_WIDTH;
    transportlayer_cfg.phase                        = XCAN_SPI_CLOCK_PHASE;
    transportlayer_cfg.polarity                     = XCAN_SPI_CLOCK_POLARITY;
    transportlayer_cfg.mlsbf                        = XCAN_SPI_BIT_ORDER;
    transportlayer_cfg.default_transmission_value   = XCAN_SPI_DEFAULT_TRANSMISSION_VALUE;
    transportlayer_cfg.No_DMA_Used = TRUE;
    if ((trans_buf != NULL) && (recv_buf != NULL))
    {
        if(GSPI_SetupEB( trans_buf, recv_buf, XCAN_BUFFER_SIZE, XCAN_SPI_CHANNEL) == GSPI_RETURN_NOT_OK)
        {
            error_status = TRUE;
        }
        if(GSPI_Init(&transportlayer_cfg) == GSPI_RETURN_NOT_OK)
        {
            error_status = TRUE;
        }
        if(error_status == FALSE)
        {
            return_status = E_OK;
        }
        else
        {
        }
    }
    else
    {
        error_status = TRUE;
    }
    return return_status;
}
static void ActivateCanTransceiver(void)
{
 	 CAN_SPI_CS_LOW();
	trans_buf[0] = (MC_ADDR << 1 ) | WRITE_OPERATION ;
	trans_buf[1] = TRANSCEIVER_NORMAL_MODE ;
	GSPI_syncTransmit(2, XCAN_SPI_CHANNEL);
}
#endif
/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/
#ifdef TMPL_USE_SCAN
#	ifndef	TMPL_USE_FRAY
void P2GPA_UpdateTranceiverStatus(void)
{
	 CAN_SPI_CS_LOW();
	trans_buf[0] = (TRANSCEIVER_STATUS_ADDR << 1 ) | READ_OPERATION ;
	trans_buf[1] = 0 ;
	GSPI_syncTransmit(2, XCAN_SPI_CHANNEL);
}
enu_p2gpa_can_trans_state_T P2GPA_GetCanTransState(void)
{
	return P2GPA_Can_Trans_State;
}
#	endif  /*!TMPL_USE_FRAY*/
#else /* #ifdef TMPL_USE_SCAN */
/******************************************************************************/
/*                              queue functions                               */
/******************************************************************************/
void P2GPA_CanQueueInit (void)
{
    struct P2GPA_CAN_queue_S  * pQ;
    u16 i;

    for (i = 0, pQ = P2GPA_CanQueue; i < P2GPA_CAN_prio_max; i++, pQ++)
    {
        /* queue is empty */
        pQ->uiPosFirst = 0;
        pQ->uiPosNext  = 0;
        pQ->bOverflow  = FALSE;
        pQ->uiCount    = 0;
    }
}
/******************************************************************************/
struct P2GPA_CAN_queue_S  * P2GPA_CanQueueGet (enum P2GPA_CAN_prio_E prio)
{
    struct P2GPA_CAN_queue_S  * ret = NULL;

    _ASSERT (prio < P2GPA_CAN_prio_max);

    if (prio < P2GPA_CAN_prio_max)
    {
        ret = &P2GPA_CanQueue[prio];
    }

    return ret;
}
/******************************************************************************/
void P2GPA_CanQueueIncPos (u16  * pos)
{
    if (*pos < (XP2GPA_CAN_QUEUE_SIZE -1))
    {
        *pos += 1;
    }
    else
    {
        *pos = 0;
    }
}
/******************************************************************************/
u16 P2GPA_CanQueueFree (enum P2GPA_CAN_prio_E prio)
{
    u16 free = 0;
    struct P2GPA_CAN_queue_S  * pQ;

    pQ = P2GPA_CanQueueGet (prio);

    if (pQ != NULL)
    {
        free = (XP2GPA_CAN_QUEUE_SIZE -1) - pQ->uiCount;
    }

    return free;
}
/******************************************************************************/
struct P2GPA_CAN_entry_S  * P2GPA_CanQueueGetFirst (struct P2GPA_CAN_queue_S  *pQ)
{
    struct P2GPA_CAN_entry_S  * ret = NULL;

    _ASSERT (pQ != NULL);

    if (pQ != NULL)
    {
        /* if anythink in the FIFO */
        if (pQ->uiCount > 0)
        {
            ret = &pQ->pEntry[pQ->uiPosFirst];
        }
    }

    return ret;
}
/******************************************************************************/
void P2GPA_CanQueueFirstFree (struct P2GPA_CAN_queue_S  * pQ)
{
    _ASSERT (pQ != NULL);

    if (pQ != NULL)
    {
        /* if anythink in the FIFO */
        if (pQ->uiCount > 0)
        {
            P2GPA_CanQueueIncPos (&pQ->uiPosFirst);
            pQ->uiCount--;
            pQ->bOverflow = FALSE;
        }
    }
}
/******************************************************************************/
struct P2GPA_CAN_entry_S  * P2GPA_CanQueueGetNextFree (struct P2GPA_CAN_queue_S  * pQ)
{
    struct P2GPA_CAN_entry_S  * ret = NULL;

    _ASSERT (pQ != NULL);

    if (pQ != NULL)
    {
        /* if free space in the FIFO */
        if (pQ->uiCount < (XP2GPA_CAN_QUEUE_SIZE -1))
        {
            ret = &pQ->pEntry[pQ->uiPosNext];
            P2GPA_CanQueueIncPos (&pQ->uiPosNext);
            pQ->uiCount++;
        }
        else
        {
            if (!pQ->bOverflow)
            {
                pQ->bOverflow = TRUE;
                /* here insert the CAN message for overflow */
            }
        }
    }
    return ret;
}
#endif /* #ifdef TMPL_USE_SCAN */
/******************************************************************************/
/*                                                                            */
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
    P2GPA_InitSCan();
#else /*#ifdef TMPL_USE_SCAN*/
    P2GPA_InitCCan();
#endif /*#ifdef TMPL_USE_SCAN*/


#ifdef APPL_VS6_ECHOES_CAN
    VS6_CanInit();
#endif
}

#ifdef TMPL_USE_SCAN
void P2GPA_InitSCan(void)
{
    struct SCAN_cfg_S cfg_scan;

    _SetBit(XFCAN_PIN_ENABLE);
    _SetBit(XFCAN_PIN_NSTB);

    memset(&cfg_scan, 0, sizeof(cfg_scan));
    cfg_scan.baud_rate = SCAN_BAUD_500K;
    /* The definition of the filters is only relevant if XTMPL_CAN0_USE_RX_FIFO is not defined */
    /* XTMPL_CAN0_USE_RX_FIFO
     * ==> if defined ==> SCAN has a reception FIFO and no filtering is performed */
    /* ==> if NOT defined ==> the hw filters are enabled and shall be defined to receive the IDs */
#  if !defined(XTMPL_CAN0_USE_RX_FIFO)
    /* FCAN message buffer configuration */
    cfg_scan.hw_filter_id[0] = 0xAA; /* Brk_Data_ESP */
    cfg_scan.hw_filter_id[1] = 0xA6; /* Ign_Veh_Stat_ESP */
    cfg_scan.hw_filter_id[2] = 0x9F; /* Park_Brk_Rs_Data_ESP */
    cfg_scan.hw_filter_id[3] = 0xAF; /* Veh_Accel_Data_ESP */
    cfg_scan.hw_filter_id[4] = 0x9E; /* Veh_Speed_Data_ESP */
    cfg_scan.hw_filter_id[5] = 0xA8; /* Whl_Stat_Left_ESP */
    cfg_scan.hw_filter_id[6] = 0xA7; /* Whl_Stat_Right_ESP */
    cfg_scan.hw_filter_id[7] = 0xA2; /* Gr_Current_Gear_CPC */
    cfg_scan.hw_filter_id[8] = 0xA5; /* PwrTr_Stat_CPC */
    cfg_scan.hw_filter_id[9] = 0xAE; /* TSL_Target_Pos_CPC */
    cfg_scan.hw_filter_id[10] = 0xAB; /* Buttons_Data_EIS */
    cfg_scan.hw_filter_id[11] = 0xA0; /* Electronic_Brk_Eng */
    cfg_scan.hw_filter_id[12] = 0xA1; /* Park_St_Rs_EPS*/
    cfg_scan.hw_filter_id[13] = 0xAD;  /* Steering_Data_EPS */
    cfg_scan.hw_filter_id[14] = 0xAC;  /* Turn_Indicators_Data_EIS */
    cfg_scan.hw_filter_id[15] = 0x98; /* VehDyn_Stat2_ESP */
    
    /* Not used for Daimler BR213 */
    cfg_scan.hw_filter_id[16] = 0x51B; /* CLU_16 */
    cfg_scan.hw_filter_id[17] = 0x520;	/* CGW3 */
    cfg_scan.hw_filter_id[18] = 0x541;	/* CGW1 */
    cfg_scan.hw_filter_id[19] = 0x553;	/* CGW2 */
    cfg_scan.hw_filter_id[20] = 0x600;	/* PCA */
    cfg_scan.hw_filter_id[21] = 0x502;//0x644;	/* RSPA_C2 */
    cfg_scan.hw_filter_id[22] = 0x100;	/* P4U btns sim */
    cfg_scan.hw_filter_id[23] = XP2GPA_CAN_ID_PARA_RECEIVE;//0x100;//XISTP_RESP_CAN_ID;
    cfg_scan.hw_filter_id[24] = 0x3A5;
    cfg_scan.hw_filter_id[25] = 0x766; /*PCA_USS*/

    cfg_scan.hw_filter_id[26] = 0x6B7; /*PCA_USS*/


    cfg_scan.num_hw_filters = 28;

#else
    cfg_scan.num_hw_filters = 0;
#endif /* #if !defined(XTMPL_CAN0_USE_RX_FIFO) && !defined(XTMPL_CAN1_USE_RX_FIFO) */

#  ifdef XTMPL_CAN0_ENABLE
    SCAN_Can0Init(&cfg_scan);
#  endif

#  ifdef XTMPL_CAN1_ENABLE
    SCAN_Can1SetTxCallbackID(0x665);
    SCAN_Can1Init(&cfg_scan);
    ActivateCanTransceiver();
#  endif
}
#else /* #ifdef TMPL_USE_SCAN */
void P2GPA_InitCCan(void)
{
    /* configuration structure of FCAN (only necessary if FCAN used) */
    struct FCAN_config_S cfg_fcan;

    struct CCAN_cfg_filter_S cfg_ccan;
    cfg_ccan.filter[0].start_id_low  = 0x000;
    cfg_ccan.filter[0].start_id_high = 0x0;
    cfg_ccan.filter[0].end_id_low    = 0xFFF;
    cfg_ccan.filter[0].end_id_high   = 0x0;

    /* FCAN parameter Channel 0 */
    /* set baud rate */
#if defined (XP2GPA_CAN0_BAUD_5K)
    cfg_fcan.baud_rate = FCAN_BAUD_5K;
#elif defined (XP2GPA_CAN0_BAUD_10K)
    cfg_fcan.baud_rate = FCAN_BAUD_10K;
#elif defined (XP2GPA_CAN0_BAUD_20K)
    cfg_fcan.baud_rate = FCAN_BAUD_20K;
#elif defined (XP2GPA_CAN0_BAUD_50K)
    cfg_fcan.baud_rate = FCAN_BAUD_50K;
#elif defined (XP2GPA_CAN0_BAUD_100K)
    cfg_fcan.baud_rate = FCAN_BAUD_100K;
#elif defined (XP2GPA_CAN0_BAUD_125K)
    cfg_fcan.baud_rate = FCAN_BAUD_125K;
#elif defined (XP2GPA_CAN0_BAUD_250K)
    cfg_fcan.baud_rate = FCAN_BAUD_250K;
#elif defined (XP2GPA_CAN0_BAUD_500K)
    cfg_fcan.baud_rate = FCAN_BAUD_500K;
#elif defined (XP2GPA_CAN0_BAUD_1M)
    cfg_fcan.baud_rate = FCAN_BAUD_1M;
#else
#   error "No CAN baud rate set"
#endif

    /* set osc */
#if defined(XMCU_USE_8MHZ_CRYSTAL)
    cfg_fcan.xosc_value = FCAN_USED_8MHZ;
#elif defined(XMCU_USE_16MHZ_CRYSTAL)
    cfg_fcan.xosc_value = FCAN_USED_16MHZ;
#elif defined (XMCU_USE_40MHZ_CRYSTAL)
    cfg_fcan.xosc_value = FCAN_USED_40MHZ;
#else
#error "No crystal frequence set"
#endif

    /* FCAN message buffer configuration */
    cfg_fcan.rx_filter[0] = 0x153; /* TCS11 */
    cfg_fcan.rx_filter[0].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[1] = 0x220; /* ESP12 */
    cfg_fcan.rx_filter[1].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[2] = 0x260; /* EMS16 */
    cfg_fcan.rx_filter[2].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[3] = 0x2B0; /* SAS11 */
    cfg_fcan.rx_filter[3].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[4] = 0x366; /* CGW_PC4 */
    cfg_fcan.rx_filter[4].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[5] = 0x367; /* CGW_PC5 */
    cfg_fcan.rx_filter[5].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[6] = 0x381; /* MDPS11 */
    cfg_fcan.rx_filter[6].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[7] = 0x386; /* WHL_SPD11 */
    cfg_fcan.rx_filter[7].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[8] = 0x387; /* WHL_PUL11 */
    cfg_fcan.rx_filter[8].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[9] = 0x38A; /* ABS11 */
    cfg_fcan.rx_filter[9].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[10] = 0x453; /* CGW_PC1 */
    cfg_fcan.rx_filter[10].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[11] = 0x470; /* P_STS */
    cfg_fcan.rx_filter[11].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[12] = 0x4A1; /* FCS1*/
    cfg_fcan.rx_filter[12].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[13] = 0x4F1;  /* CLU11 */
    cfg_fcan.rx_filter[13].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[14] = 0x507;  /* TCS15 */
    cfg_fcan.rx_filter[14].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[15] = 0x515; /* CLU_14 */
    cfg_fcan.rx_filter[15].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[16] = 0x51B; /* CLU_16 */
    cfg_fcan.rx_filter[16].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[17] = 0x520;	/* CGW3 */
    cfg_fcan.rx_filter[17].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[18] = 0x541;	/* CGW1 */
    cfg_fcan.rx_filter[18].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[19] = 0x553;	/* CGW2 */
    cfg_fcan.rx_filter[19].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[20] = 0x600;	/* PCA */
    cfg_fcan.rx_filter[20].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[21] = 0x644;	/* RSPA_C2 */
    cfg_fcan.rx_filter[21].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[22] = 0x100;	/* P4U btns sim */
    cfg_fcan.rx_filter[22].frame_type = FCAN_STANDARD_FRAME;

    cfg_fcan.rx_filter[23] = 0x6B7; /* P4U btns sim */
    cfg_fcan.rx_filter[23].frame_type = FCAN_STANDARD_FRAME;

#if (F_DAS_ULSD_SENS_SIMUL_MODE_1 == F_DAS_ON)
    cfg_fcan.rx_filter[15].id_of_message_buffer_l = 0x770; /* CAN_ID_DRDV_01    */
    cfg_fcan.rx_filter[15].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[16].id_of_message_buffer_l = 0x771; /* CAN_ID_DRDV_02    */
    cfg_fcan.rx_filter[16].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[17].id_of_message_buffer_l = 0x772; /* CAN_ID_DRDV_03    */
    cfg_fcan.rx_filter[17].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[18].id_of_message_buffer_l = 0x773; /* CAN_ID_DRDV_04    */
    cfg_fcan.rx_filter[18].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[19].id_of_message_buffer_l = 0x774; /* CAN_ID_DRDV_05    */
    cfg_fcan.rx_filter[19].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[20].id_of_message_buffer_l = 0x775; /* CAN_ID_DRDV_06    */
    cfg_fcan.rx_filter[20].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[21].id_of_message_buffer_l = 0x776; /* CAN_ID_DRDV_07    */
    cfg_fcan.rx_filter[21].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[22].id_of_message_buffer_l = 0x777; /* CAN_ID_DRDV_08    */
    cfg_fcan.rx_filter[22].frame_type = FCAN_STANDARD_FRAME;

#elif defined( XAPPL_REPLAY_ULSD1_V2 )
    /* Receive NEW ROHDAT protocol data */
    cfg_fcan.rx_filter[15].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID1;
    cfg_fcan.rx_filter[15].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[16].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID2;
    cfg_fcan.rx_filter[16].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[17].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID3;
    cfg_fcan.rx_filter[17].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[18].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID4;
    cfg_fcan.rx_filter[18].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[19].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID5;
    cfg_fcan.rx_filter[19].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[20].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID6;
    cfg_fcan.rx_filter[20].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[21].id_of_message_buffer_l = 0x000;
    cfg_fcan.rx_filter[21].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[22].id_of_message_buffer_l = 0x000;
    cfg_fcan.rx_filter[22].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[24].id_of_message_buffer_l = XCOMH_P2_SYNC_TASK_CAN_ID;  /* REPLAY timestamp for ODOM and Sync*/
    cfg_fcan.rx_filter[24].frame_type = FCAN_STANDARD_FRAME;
#elif defined( XAPPL_REPLAY_ULSD1_V3 )
    cfg_fcan.rx_filter[15].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID1;
    cfg_fcan.rx_filter[15].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[16].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID2;
    cfg_fcan.rx_filter[16].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[17].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID3;
    cfg_fcan.rx_filter[17].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[18].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID4;
    cfg_fcan.rx_filter[18].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[19].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID5;
    cfg_fcan.rx_filter[19].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[20].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID6;
    cfg_fcan.rx_filter[20].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[21].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID7;
    cfg_fcan.rx_filter[21].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[22].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID8;
    cfg_fcan.rx_filter[22].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[23].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID9;
    cfg_fcan.rx_filter[23].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[24].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID10;
    cfg_fcan.rx_filter[24].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[26].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID11;
    cfg_fcan.rx_filter[26].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[27].id_of_message_buffer_l = P2GPA_CAN_RAWDATA_ID12;
    cfg_fcan.rx_filter[27].frame_type = FCAN_STANDARD_FRAME;
# err "The Ids for ULSD1_V3 have to be added to the filter (or the filter removed)"
#else
    cfg_fcan.rx_filter[22].id_of_message_buffer_l = 0x000;
    cfg_fcan.rx_filter[22].frame_type = FCAN_STANDARD_FRAME;
#endif

    cfg_fcan.rx_filter[25].id_of_message_buffer_l = XCANH_ID_MAP;
    cfg_fcan.rx_filter[25].frame_type = FCAN_STANDARD_FRAME;

#ifndef XAPPL_REPLAY_ULSD1_V3
    cfg_fcan.rx_filter[23].id_of_message_buffer_l = 0x118; /* VCTM testmode: XP2GPA_VCTM_CAN_SWITCH_TESTMODE */
    cfg_fcan.rx_filter[23].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[24].id_of_message_buffer_l = 0;
    cfg_fcan.rx_filter[24].frame_type = FCAN_STANDARD_FRAME;
# ifdef XP2GPA_ENABLE_EITP
    cfg_fcan.rx_filter[26].id_of_message_buffer_l = XEITP_PC_HEADER_ID;
    cfg_fcan.rx_filter[26].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[27].id_of_message_buffer_l = XEITP_PC_BODY_ID;
    cfg_fcan.rx_filter[27].frame_type = FCAN_STANDARD_FRAME;
# else
    cfg_fcan.rx_filter[26].id_of_message_buffer_l = 0x11A; /* VCTM testmode: XP2GPA_CALIBRATE_PACO */
    cfg_fcan.rx_filter[26].frame_type = FCAN_STANDARD_FRAME;
    cfg_fcan.rx_filter[27].id_of_message_buffer_l = 0x11B;
    cfg_fcan.rx_filter[27].frame_type = FCAN_STANDARD_FRAME;
# endif
#endif /* # ifndef XAPPL_REPLAY_ULSD1_V3 */

    /* Initialization of main CAN-structure for sending in P2GPA_CanCycle */
    g_can_msg_send.channel = FCAN_CHANNEL0;
    g_can_msg_send.dlc = 0;
    g_can_msg_send.data[0] = 0;
    g_can_msg_send.data[1] = 0;
    g_can_msg_send.data[2] = 0;
    g_can_msg_send.data[3] = 0;
    g_can_msg_send.data[4] = 0;
    g_can_msg_send.data[5] = 0;
    g_can_msg_send.data[6] = 0;
    g_can_msg_send.data[7] = 0;
    g_can_msg_send.id_h = 0;
    g_can_msg_send.id_l = 0;
    g_can_msg_send.id_type = ID_TYPE_STANDARD;
    g_can_msg_send.timestamp_512us = 0;

    /* Setting of message buffer parameters */
    FCAN_SetModuleParameters(&cfg_fcan, FCAN_CHANNEL0);

    P2GPA_CanQueueInit ();

    CCAN_InitAccFilter(&cfg_ccan,FCAN_CHANNEL0);
    CCAN_Init(FCAN_CHANNEL0);


}
#endif /*#ifdef TMPL_USE_SCAN*/

/******************************************************************************/
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*                                                                            */
/******************************************************************************/


/******************************************************************************/
/*                                                                            */
/******************************************************************************/
 //   P2GPA_CanSendDebugCh(0x600, data, sizeof(data));

#ifdef TMPL_USE_SCAN
void XSCAN_Can0RxExt(
    u16 id_a,
    u32 id_b,
    const u8* data,
    u8 dlc)
{
}

/**
 * \param[IN] id_a
 * \param[IN] id_b
 * \param[IN] data
 * \param[IN] dlc
 */
void XSCAN_Can1RxExt(
    u16 id_a,
    u32 id_b,
    const u8* data,
    u8 dlc)
{
}

void XSCAN_Can0Sent(void)
{
}

void P2GPA_Can1Receive (u16 id, const u8* data, u8 dlc)
{

}

void P2GPA_CanReceive (u16 id, const u8* data, u8 dlc )
{
#else /* #ifdef TMPL_USE_SCAN */
void P2GPA_CanReceive (const struct CCAN_msg_S* p_msg)
{
    const u8* data;
    u16 id = p_msg->id_l;
    u8 dlc = p_msg->dlc;

    data = p_msg->data;
#endif /* #ifdef TMPL_USE_SCAN */

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
/*                                                                            */
/******************************************************************************/
bool_T  P2GPA_IsBufferFull()
{
    return FALSE;
}
/******************************************************************************/
/*                                                                            */
/******************************************************************************/

#ifdef TMPL_USE_SCAN
u8 P2GPA_CanSendDebugCh (u16 id, const u8 *data, u8 dlc)
{
#ifdef XAPPL_SEND_DEV_IDS
#    ifdef TMPL_USE_FRAY
		FRMAPTX_TransmitDebug(id, data,dlc);
#  else
    return SCAN_Can1Tx(id, data, dlc);
#  endif
#endif
}

u8 P2GPA_CanSend (enum P2GPA_CAN_prio_E prio, u16 id, const u8 *data, u8 dlc)
{
#ifdef TMPL_USE_FRAY
	FRMAPTX_Tansmit(id, data,dlc) ;
    return 0;
#else
     return SCAN_Can1Tx(id, data, dlc);
#endif
}

#else /*#ifdef TMPL_USE_SCAN*/
void P2GPA_CanSend (enum P2GPA_CAN_prio_E prio, u16 id, const u8 *p, u8 n)
{
    struct P2GPA_CAN_queue_S  * pQ;

    _ASSERT (p != NULL);
    _ASSERT ((n > 0) && (n < 9));

    pQ = P2GPA_CanQueueGet (prio);

    _ASSERT (pQ != NULL);

    if(P2GPA_send_can_enabled == TRUE)
    {
        if (pQ != NULL)
        {
            struct P2GPA_CAN_entry_S  * pE = P2GPA_CanQueueGetNextFree (pQ);

            if (pE != NULL)
            {
                u16 count;

                pE->ucCount = n;
                pE->uiID    = id;

                for (count = 0; count < n; count++)
                {
                    pE->pBuffer[count] = p[count];
                }
            }
            else
            {
                _ASSERT(FALSE);
                /* no free buffer, FIFO full */
            }
        }
    }
}
/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void P2GPA_CanCycle (enum P2GPA_CAN_prio_E prio, u16 count)
{
    struct P2GPA_CAN_queue_S  * pQ = NULL;
    enum CCAN_result_E result;
    _ASSERT (count > 0);

    pQ = P2GPA_CanQueueGet (prio);

    _ASSERT (pQ != NULL);

    if ((pQ != NULL) && (pQ->uiCount > 0))
    {
        for (; count > 0; count--)
        {
            struct P2GPA_CAN_entry_S  * pE = P2GPA_CanQueueGetFirst (pQ);

            if (pE != NULL)
            {
                /* ECAN cannot work with GPAGE pointers, so we need a local copy of the entry. */
                struct P2GPA_CAN_entry_S entry_loc_copy;
                entry_loc_copy = *pE;

                g_can_msg_send.dlc = entry_loc_copy.ucCount;
                g_can_msg_send.data[0] = entry_loc_copy.pBuffer[0];
                g_can_msg_send.data[1] = entry_loc_copy.pBuffer[1];
                g_can_msg_send.data[2] = entry_loc_copy.pBuffer[2];
                g_can_msg_send.data[3] = entry_loc_copy.pBuffer[3];
                g_can_msg_send.data[4] = entry_loc_copy.pBuffer[4];
                g_can_msg_send.data[5] = entry_loc_copy.pBuffer[5];
                g_can_msg_send.data[6] = entry_loc_copy.pBuffer[6];
                g_can_msg_send.data[7] = entry_loc_copy.pBuffer[7];
                g_can_msg_send.id_l = entry_loc_copy.uiID;

                result = CCAN_Send (&g_can_msg_send, NULL);
                /*                _ASSERT(result == FCAN_TX_BUFFERS_FULL); */

                if (result == CCAN_OK)
                {
                    P2GPA_CanQueueFirstFree (pQ);
                }

            }
            else
            {
                /* no more data in the FIFO */
                break;
            }
        }
    }
}

/******************************************************************************/
/*                                                                            */
/******************************************************************************/
void P2GPA_CanInBuffer
(
    u8                    *pCanBuffer,
    enum P2GPA_CAN_type_E eType,
    bool_T                bBigEndian,
    const void            *pData,
    u8                    ucStartBit,
    u8                    ucBitCount
)
{
    const u8  masks8[]  = { 0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f };
    const u32 masks32[] = {
        0xffffffff, 0xfffffffe, 0xfffffffc, 0xfffffff8,
        0xfffffff0, 0xffffffe0, 0xffffffc0, 0xffffff80
    };

    u16    count       = 0;
    si16   countBuffer = 0;
    u32    tempValue   = 0x00000000;
    u32    tempMask    = 0x00000000;
    u32    tempSigMask = 0x00000000;
    u8     tempByteV   = 0x00;
    u8     tempByteM   = 0x00;
    bool_T bNeg        = FALSE;


    _ASSERT (pCanBuffer != NULL);
    _ASSERT (pData != NULL);
    _ASSERT (ucStartBit < 64);
    _ASSERT (ucBitCount < (64 -ucStartBit));
    _ASSERT (ucBitCount < 33);
    _ASSERT (ucBitCount > 0);

    /* generates the bit mask for the sign information*/
    if ((eType == P2GPA_CAN_type_si8)  ||
        (eType == P2GPA_CAN_type_si16) ||
        (eType == P2GPA_CAN_type_si32) )
    {
        count = 1;
        tempSigMask = 0x0000001 << ucBitCount;
    }

    /* generates the bit mask for the value */
    for (; count < (ucBitCount -4); count += 4)
    {
        tempMask <<= 4;
        tempMask  |= 0x0000000f;
    }

    for (; count < ucBitCount; count++)
    {
        tempMask <<= 1;
        tempMask  |= 0x00000001;
    }

    /* read the value and set the 32bit variable with this          */
    /* check if the value negative or not, for setting the sign bit */
    switch (eType)
    {
    case P2GPA_CAN_type_bool_T:      /* type: bool_T */
        if (*(bool_T*)pData)
        {
            tempValue = tempMask;
        }
        break;
    case P2GPA_CAN_type_si8:         /* type: si8    */
        if (*(si8*)pData < 0)
        {
            bNeg = TRUE;
        }
    case P2GPA_CAN_type_u8:          /* type: u8     */
        tempValue = *(u8*)pData;
        break;
    case P2GPA_CAN_type_si16:        /* type: si16   */
        if (*(si16*)pData < 0)
        {
            bNeg = TRUE;
        }
    case P2GPA_CAN_type_u16:         /* type: u16    */
        tempValue = (u32)*(u16*)pData;
        break;
    case P2GPA_CAN_type_si32:        /* type: si32   */
        if (*(si32*)pData < 0)
        {
            bNeg = TRUE;
        }
    case P2GPA_CAN_type_u32:         /* type: u32    */
        tempValue = (u32)*(u32*)pData;
        break;
    default:
        _ASSERT (FALSE);
    }
    tempValue  &= tempMask;              /* only bit count bit's           */
    tempMask   |= tempSigMask;           /* add to the mask the sign bit   */
    if (bNeg)
    {
        tempValue  |= tempSigMask;       /* add leading sign               */
    }

    /* how many bits are moved in one byte */
    count = ucStartBit % 8;
    /* shift the value left, for the right position */
    /* the lsb is in the last byte                  */
    if (count > 0)
    {
        tempByteV   = (tempValue >> (32 - count)) & masks8[count];
        tempByteM   = (tempMask  >> (32 - count)) & masks8[count];
        tempValue <<= count;
        tempMask  <<= count;
        tempValue  &= masks32[count];
        tempMask   &= masks32[count];
    }

    /* start byte in the CAN buffer */
    countBuffer = ucStartBit / 8;

    /* insert the bytes in the CAN buffer */
    for (count = 0; (count < 4) && (countBuffer >= 0) && (countBuffer < 8) && (tempMask != 0x00000000); count++)
    {
        /* remove all used bits */
        pCanBuffer[countBuffer] &= (~tempMask  & 0xff);
        /* set all used bits    */
        pCanBuffer[countBuffer] |= (tempValue & 0xff);
        /* remove the used bits from the temp varibale */
        tempMask  >>= 8;
        tempValue >>= 8;
        tempMask   &= 0x00ffffff;
        tempValue  &= 0x00ffffff;

        if (bBigEndian)
        {
            countBuffer--;
        }
        else
        {
            countBuffer++;
        }
    }
    if ((tempByteM != 0x00) && (countBuffer >= 0) && (countBuffer < 8))
    {
        /* remove all used bits */
        pCanBuffer[countBuffer] &= ~tempByteM;
        /* set all used bits    */
        pCanBuffer[countBuffer] |= tempByteV;
    }
}
#endif/*#ifdef TMPL_USE_SCAN*/
/******************************************************************************/

/******************************************************************************/



/******************************************************************************/
/* Callback-.function for ULSD2.1: Connect to XSECO_RecSendCanData            */
/* via PDF-Header                                                             */
/******************************************************************************/
u8 P2GPA_RecSendCanData(u16 id, const u8* data, u8 dlc)
{
    return P2GPA_CanSend(P2GPA_CAN_prio_high, id, data, dlc);
}


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







#ifdef TMPL_USE_SCAN
#else /*#ifdef TMPL_USE_SCAN*/
/******************************************************************************/
/* Stubs for CCAN                                                             */
/******************************************************************************/
void P2GPA_MsgSended(u16 msg_send_idx, u32 msg_timestamp, bool_T with_timestamp)
{
    /* stub for CCAN - function not needed at the moment */
}

void P2GPA_CanError(enum CCAN_error_code_E error)
{
    /* stub for CCAN - function not needed at the moment */
}

void P2GPA_CanRxInterrupt(const struct CCAN_msg_S *p_msg)
{
    /* stub for CCAN - function not needed at the moment */
}
/******************************************************************************/
/******************************************************************************/
#endif /*#ifdef TMPL_USE_SCAN*/










/****************************************************/
// CODE TO REMOVE
/****************************************************/










/******************************************************************************/
/*  CAN-Communication of the EXIT test mode                                   */
/*  Use the canalyzer data base:                                              */
/******************************************************************************/


#if defined APPL_ENABLE_FAS3_BOARD && defined APPL_ENABLE_DSP_FEATURES
void P2GPA_CanTshapesSend( void )
{
    static u16 tshape_index = 0;
    u8 buff[8];
    u16 tmp;
    static struct DAPM_dapm_cfg_S* p_dapm_cfg = NULL;

    struct GL2D_frame_S tshapes[5];
    u8 num_tshapes;

    CREM_IPC_read_tshapes(tshapes, &num_tshapes);

    /* Only 5 tshapes are sent */
    if (num_tshapes>5) num_tshapes=5;

    /* Send reset/finish command */
    (void)memset(buff, XP2GPA_CANWARNELEMS_END_MSG, 8);
    P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_CAN_WARNELMS_ID, buff, 8);

    for (tshape_index=0; tshape_index<num_tshapes; tshape_index++)
    {
        (void)memset(buff, XP2GPA_CANWARNELEMS_END_MSG, 8);  //reset buffer
        tmp = (u16)MTLI_LimSi16(tshapes[tshape_index].p.x/20, -1024, 1023);
        buff[0] |= (u8)((tmp << 0) & 0xFF);
        buff[1] |= (u8)((tmp >> 8) & 0x07);

        tmp = (u16)MTLI_LimSi16(tshapes[tshape_index].p.y/20, -1025, 1024);
        buff[1] |= (u8)((tmp << 3) & 0xF8);
        buff[2] |= (u8)((tmp >> 5) & 0x3F);
        buff[2] |= (u8)((2 << 6) & 0xE0);  //confidence_trend
        buff[3] = (u8)20;   //spatial_uncertainty_mm
        buff[4] |= (u8)(2 & 0x03); //type
        buff[4] |= ((1 << 2) & _0001_1100b);  //trend
        buff[4] |= (u8)((5 << 5) & _1110_0000b);   //CAPP_H_HIGH
        buff[5] = (u8)(0 & 0x00FF);   //rel_velocity_mm_s
        buff[6] |= (u8)((0 >> 8) & 0x0003);   //rel_velocity_mm_s
        buff[6] |= (u8)((100 << 2) & _1111_1100b);   //confidence
        buff[7] |= (u8)((100 >> 6) & _0000_0001b);   //confidence

        P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_CAN_WARNELMS_ID, buff, 8);
    }

    /* Send reset/finish command */
    (void)memset(buff, XP2GPA_CANWARNELEMS_END_MSG, 8);
    P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_CAN_WARNELMS_ID, buff, 8);
}
#endif

