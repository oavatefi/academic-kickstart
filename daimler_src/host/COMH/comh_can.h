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
/*                                                                            */
/*   Source:         %PM%       */
/*   Revision:       %PR%                                                     */
/*   Status:         %PS%                                                  */
/*   Author:         Bernd Moennicke, Daniel Schuler                                          */
/*   UserID:         SBX684                                                   */
/*   Date            %PRT%                                     */
/*                                                                            */
/******************************************************************************/
/*   COMPONENT: CAN subroutines for the P2GPA                                 */
/*   UNIT:                                                                    */
/*   SCOPE:                                                                   */
/*   TARGET:                                                                  */
/******************************************************************************/
/*                                                                            */
/******************************************************************************/

#ifndef I_P2GPALEO_CAN_H
#define I_P2GPALEO_CAN_H

#include "dstdint.h"
#include "dstdbool.h"

#ifdef TMPL_USE_SCAN
#else
#include "ccan.h"
#endif

#include "capp.h"

/******************************************************************************/
/*                 Definition of exported symbolic constants                  */
/******************************************************************************/
/*** MAP - CAN protocol definitions                      ***/

/* MSG: 0x06FF */
/* messages exclusively related to the flgd application */
#ifndef XCANH_ID_MAP
# ifdef P2GPA_CAN_ID_DEV_31
#  define  XCANH_ID_MAP        P2GPA_CAN_ID_DEV_31
# else
  #define  XCANH_ID_MAP        0x06FF
# endif
#endif

/* muxer settings */
#define  XCANH_MAPMUX_MAP    0x00
#define  XCANH_MAPMUX_OBJ    0x01
#define  XCANH_MAPMUX_ELM    0x02
#define  XCANH_MAPMUX_CMD    0xF0
#define  XCANH_MAPMUX_ERR    0xF1

/* map object types */
#define XCANH_MAPOBJTYPE_UNKNOWN   0
#define XCANH_MAPOBJTYPE_STDOBJ    1
#define XCANH_MAPOBJTYPE_VEHICLE   2
#define XCANH_MAPOBJTYPE_PERIMETER 3

/* command: codes */
#define  XCANH_CMD_CODE_NO_CMD      0
#define  XCANH_CMD_CODE_INIT        1
#define  XCANH_CMD_CODE_REQ_MAP     2
#define  XCANH_CMD_CODE_REINIT_DAPM 3

/* color codes for Display on the VS6 tool */
#define  XCANH_COLOR_CODE_BLACK         0x0000
#define  XCANH_COLOR_CODE_CYAN          0x0001
#define  XCANH_COLOR_CODE_LIGHTBLUE     0x0002
#define  XCANH_COLOR_CODE_WHITE         0x0003
#define  XCANH_COLOR_CODE_PINK          0x0004
#define  XCANH_COLOR_CODE_GREEN         0x0005
#define  XCANH_COLOR_CODE_LIGHTGREEN    0x0006
#define  XCANH_COLOR_CODE_BROWN         0x0007
#define  XCANH_COLOR_CODE_GREYGREEN     0x0008
#define  XCANH_COLOR_CODE_BLUE          0x0009
#define  XCANH_COLOR_CODE_LIGHTBROWN    0x000A
#define  XCANH_COLOR_CODE_VIOLET        0x000B
#define  XCANH_COLOR_CODE_RED           0x000C
#define  XCANH_COLOR_CODE_STONEGREY     0x000D
#define  XCANH_COLOR_CODE_BLUEGREEN     0x000E
#define  XCANH_COLOR_CODE_YELLOW        0x000F

#define  XCANH_COLOR_DEFAULT            XCANH_COLOR_CODE_BLACK
#define  XCANH_COLOR_UNKNOWN            XCANH_COLOR_CODE_BLACK
#define  XCANH_COLOR_STD_OBJ            XCANH_COLOR_CODE_RED
#define  XCANH_COLOR_VEHICLE            XCANH_COLOR_CODE_PINK
#define  XCANH_COLOR_PERIMETER          XCANH_COLOR_CODE_BLUE
#define  XCANH_COLOR_TRACKING_AREA      XCANH_COLOR_CODE_BROWN

#define  CAN_SPI_CS_LOW() 		SIU.GPDO[HWPL_PIN_PDC_DSPI_CS].R = 0
#define  CAN_SPI_CS_HIGH() 		SIU.GPDO[HWPL_PIN_PDC_DSPI_CS].R = 1
/******************************************************************************/
/*                Definition of exported function like macros                 */
/******************************************************************************/
void P2GPA_CanMapReceive(const u8 *p);
/******************************************************************************/
/*         Definition of exported types (typedef, enum, struct, union)        */
/******************************************************************************/
enum P2GPA_CAN_prio_E {
    P2GPA_CAN_prio_high = 0,    /* can data for cycle send in a 20ms cycle    */
    P2GPA_CAN_prio_low,         /* can data, with send in the resttime tasks  */
    P2GPA_CAN_prio_max          /* only for internal                          */
};

enum P2GPA_CAN_type_E {
    P2GPA_CAN_type_bool_T,      /* type: bool_T                               */
    P2GPA_CAN_type_u8,          /* type: u8                                   */
    P2GPA_CAN_type_si8,         /* type: si8                                  */
    P2GPA_CAN_type_u16,         /* type: u16                                  */
    P2GPA_CAN_type_si16,        /* type: si16                                 */
    P2GPA_CAN_type_u32,         /* type: u32                                  */
    P2GPA_CAN_type_si32         /* type: si32                                 */
};

typedef enum
{
	P2GPA_CAN_TRANS_OFF_STATE,
	P2GPA_CAN_TRANS_ACTIVE_STATE,
}enu_p2gpa_can_trans_state_T;

#ifdef TMPL_USE_SCAN
/* Only needed for compatibility with CCAN */
/** SCAN Message buffer type.                                                 */
enum P2GPA_msg_id_type_E
{
    /**< Message not used                                                     */
    ID_TYPE_UNKNOWN = 0,
    /**< Message buffer is standard type                                      */
    ID_TYPE_STANDARD,
    /**< Message buffer is extended type                                      */
    ID_TYPE_EXTENDED
};
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

void P2GPA_CanInit (void);
#ifndef TMPL_USE_FRAY
enu_p2gpa_can_trans_state_T P2GPA_GetCanTransState(void);
void P2GPA_UpdateTranceiverStatus(void);
#endif

u8 P2GPA_RecSendCanData(u16 id, const u8* data, u8 dlc);

#ifdef TMPL_USE_SCAN
void P2GPA_CanReceive (u16 id, const u8* data, u8 dlc );
extern void P2GPA_Can1Receive (u16 id, const u8* data, u8 dlc);
/* Sends CAN messages through the debug channel */
u8   P2GPA_CanSendDebugCh (u16 id, const u8 *data, u8 dlc);
/* TODO-AO: defined with prio for compatibility with CCAN */
u8 P2GPA_CanSend (enum P2GPA_CAN_prio_E prio, u16 id, const u8 *data, u8 dlc);
#else /* #ifdef TMPL_USE_SCAN */
void P2GPA_CanReceive (const struct CCAN_msg_S* p_msg);
void P2GPA_CanSend (enum P2GPA_CAN_prio_E prio, u16 id, const u8 *p, u8 n);
void P2GPA_CanCycle (enum P2GPA_CAN_prio_E prio, u16 count);
void P2GPA_MsgSended(u16 msg_send_idx, u32 msg_timestamp, bool_T with_timestamp);
void P2GPA_CanError(enum CCAN_error_code_E error);

void P2GPA_CanInBuffer
(
    u8                    *pCanBuffer,
    enum P2GPA_CAN_type_E eType,
    bool_T                bBigEndian,
    const void            *pData,
    u8                    ucStartBit,
    u8                    ucBitCount
);

void P2GPA_CanOutBuffer
(
    const u8              *pCanBuffer,
    enum P2GPA_CAN_type_E eType,
    bool_T                bBigEndian,
    void                  *pData,
    u8                    ucStartBit,
    u8                    ucBitCount
);
#endif /* #ifdef TMPL_USE_SCAN */

#ifdef XP2PGA_CAN_BSDDEBUG_SEND
    void P2GPA_CanDsfeDiagSend(u8 num_msgs, u8* msgL, u8* msgR);
#endif

#ifdef P2GPA_CAN_WARNELMS_SEND
void P2GPA_CanWarnElmsSend( void );
#endif /* P2GPA_CAN_WARNELMS_SEND */

#ifdef XP2GPA_CAN_ID_PARKHILFE5_SEND
    void P2GPA_CanParkhilfe5Send (void);
#endif
#ifdef P2GPA_CAN_ID_DEV_SEND
    void P2GPA_CanDevSend (void);
#endif
#ifdef XP2PGA_CAN_BSDALARM_SEND
void P2GPA_CanBSDAlarmSend(enum CAPP_side_E side, bool_T is_alarm);
#endif
#ifdef XP2PGA_CAN_BSDDEBUG_SEND
void P2GPA_CanBSDDebugSend(enum CAPP_side_E update_side,u8* msg1, u8* msg2, u8* msg3, u8* msg4, u8* msg5);
#endif
#ifdef XAPPL_REPLAY_ULSD1_V2
bool_T P2GPA_IsReplayDataAvailableUlsd1V2(void);
#endif
#ifdef XAPPL_REPLAY_ULSD1_V3
bool_T P2GPA_IsReplayDataAvailableUlsd1V3(void);
#endif
#ifdef APPL_ENABLE_SEND_ODOM_INFO
void P2GPA_CanOdomInfoSend(void);
#endif

bool_T P2GPA_IsBufferFull(void);
#ifndef	TMPL_USE_FRAY
void P2GPA_CanSPITransmissionComplete(void);
#endif
/******************************************************************************/
/*                                                                            */
/*  Prototype:                                                                */
/*      u16                                                                   */
/*      MODX_MakeExampleAction(u8 size_of_array)                              */
/*                                                                            */
/*  Description:                                                              */
/*    ...                                                                     */
/*                                                                            */
/*  Input parameters:                                                         */
/*    size_of_array: a size of an array >= 10 and <= 255                      */
/*                                                                            */
/*  Output parameters:                                                        */
/*    return: checksum of the array                                           */
/*                                                                            */
/*  Preconditions:                                                            */
/*    see Input parameters                                                    */
/*                                                                            */
/*  Postconditions:                                                           */
/*    none                                                                    */
/*                                                                            */
/******************************************************************************/

#endif  /* I_P2GPALEO_CAN_H */
