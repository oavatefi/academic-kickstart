/*
 * E2E_platfrom.h
 *
 *  Created on: Feb 20, 2017
 *      Author: nelmensh
 */

#ifndef E2E_PLATFROM_H_
#define E2E_PLATFROM_H_

#include "platform_types.h"
#include "Std_Types.h"
#include "Compiler.h"

#define E2EP02_COUNTER_MAX 15
#define E2E_E_OK 0U
#define E2EP02_CRC_STARTVALUE 0xFFU
#define SCRC_CRC8H2F_XORVALUE 0xFFU
#define SCRC_CRC8H2F_INITIALVALUE 0xFFU

#ifndef TRUE
/** BOOL value true. */
#    define TRUE        ( (BOOL)1 )
#endif

#ifndef FALSE
/** BOOL value false. */
#    define FALSE       ( (BOOL)0 )
#endif

/*Defines a unique numerical identifier used in combination with
 * the current data element value to create the CRC checksum*/
#define Profile02_DataID_Brk_Rq_PARK_Pr2       {132,162,192,222,252,31,61,91,121,151,181,211,241,20,50,80}
#define Profile02_DataID_ParkCfg_Stat_Pr2      {39,136,233,79,176,22,119,216,62,159,5,102,199,45,142,239}
#define Profile02_DataID_St_Rq_PARK_Pr2        {60,91,122,153,184,215,246,26,57,88,119,150,181,212,243,23}
#define Profile02_DataID_Park_Disp_Rq_AR2      {0xD8U, 0x13U, 0x49U, 0x7FU, 0xB5U, 0xEBU, 0x26U, 0x5CU, 0x92U, 0xC8U, 0x03U, 0x39U, 0x6FU, 0xA5U, 0xDBU, 0x16U}
#define Profile02_DataID_RemPark_Rq_Pr2        {0xD3U, 0x67U, 0xF6U, 0x8AU, 0x1EU, 0xADU, 0x41U, 0xD0U, 0x64U, 0xF3U, 0x87U, 0x1BU, 0xAAU, 0x3EU, 0xCDU, 0x61U}


/* Table of pre-computed values for CRC8H2F */
STATIC CONST(uint8, SCRC_CONST) SCrc_Table8H2F[256] =
{
    0x00U, 0x2FU, 0x5EU, 0x71U, 0xBCU, 0x93U, 0xE2U, 0xCDU, 0x57U, 0x78U, 0x09U,
    0x26U, 0xEBU, 0xC4U, 0xB5U, 0x9AU,
    0xAEU, 0x81U, 0xF0U, 0xDFU, 0x12U, 0x3DU, 0x4CU, 0x63U, 0xF9U, 0xD6U, 0xA7U,
    0x88U, 0x45U, 0x6AU, 0x1BU, 0x34U, 0x73U, 0x5CU, 0x2DU, 0x02U, 0xCFU, 0xE0U,
    0x91U, 0xBEU, 0x24U, 0x0BU, 0x7AU, 0x55U, 0x98U, 0xB7U, 0xC6U, 0xE9U, 0xDDU,
    0xF2U, 0x83U, 0xACU, 0x61U, 0x4EU, 0x3FU, 0x10U, 0x8AU, 0xA5U, 0xD4U, 0xFBU,
    0x36U, 0x19U, 0x68U, 0x47U, 0xE6U, 0xC9U, 0xB8U, 0x97U, 0x5AU, 0x75U, 0x04U,
    0x2BU, 0xB1U, 0x9EU, 0xEFU, 0xC0U, 0x0DU, 0x22U, 0x53U, 0x7CU, 0x48U, 0x67U,
    0x16U, 0x39U, 0xF4U, 0xDBU, 0xAAU, 0x85U, 0x1FU, 0x30U, 0x41U, 0x6EU, 0xA3U,
    0x8CU, 0xFDU, 0xD2U, 0x95U, 0xBAU, 0xCBU, 0xE4U, 0x29U, 0x06U, 0x77U, 0x58U,
    0xC2U, 0xEDU, 0x9CU, 0xB3U, 0x7EU, 0x51U, 0x20U, 0x0FU, 0x3BU, 0x14U, 0x65U,
    0x4AU, 0x87U, 0xA8U, 0xD9U, 0xF6U, 0x6CU, 0x43U, 0x32U, 0x1DU, 0xD0U, 0xFFU,
    0x8EU, 0xA1U, 0xE3U, 0xCCU, 0xBDU, 0x92U, 0x5FU, 0x70U, 0x01U, 0x2EU, 0xB4U,
    0x9BU, 0xEAU, 0xC5U, 0x08U, 0x27U, 0x56U, 0x79U, 0x4DU, 0x62U, 0x13U, 0x3CU,
    0xF1U, 0xDEU, 0xAFU, 0x80U, 0x1AU, 0x35U, 0x44U, 0x6BU, 0xA6U, 0x89U, 0xF8U,
    0xD7U, 0x90U, 0xBFU, 0xCEU, 0xE1U, 0x2CU, 0x03U, 0x72U, 0x5DU, 0xC7U, 0xE8U,
    0x99U, 0xB6U, 0x7BU, 0x54U, 0x25U, 0x0AU, 0x3EU, 0x11U, 0x60U, 0x4FU, 0x82U,
    0xADU, 0xDCU, 0xF3U, 0x69U, 0x46U, 0x37U, 0x18U, 0xD5U, 0xFAU, 0x8BU, 0xA4U,
    0x05U, 0x2AU, 0x5BU, 0x74U, 0xB9U, 0x96U, 0xE7U, 0xC8U, 0x52U, 0x7DU, 0x0CU,
    0x23U, 0xEEU, 0xC1U, 0xB0U, 0x9FU, 0xABU, 0x84U, 0xF5U, 0xDAU, 0x17U, 0x38U,
    0x49U, 0x66U, 0xFCU, 0xD3U, 0xA2U, 0x8DU, 0x40U, 0x6FU, 0x1EU, 0x31U, 0x76U,
    0x59U, 0x28U, 0x07U, 0xCAU, 0xE5U, 0x94U, 0xBBU, 0x21U, 0x0EU, 0x7FU, 0x50U,
    0x9DU, 0xB2U, 0xC3U, 0xECU, 0xD8U, 0xF7U, 0x86U, 0xA9U, 0x64U, 0x4BU, 0x3AU,
    0x15U, 0x8FU, 0xA0U, 0xD1U, 0xFEU, 0x33U, 0x1CU, 0x6DU, 0x42U,
};

/*==================[type definitions]=======================================*/

/**
 * \brief Non-modifiable configuration of the data element sent over an RTE port, for E2E Profile.
 *
 * The position of the counter and CRC is not configurable in Profile 2.
 */

/** \brief Generic function pointer for signal packing */
typedef FUNC(uint8, E2EPW_CODE) (*E2EPW_PackCopyDeFuncPtrType)
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
);


typedef struct {
    E2EPW_PackCopyDeFuncPtrType e2e_serl_func;
   uint16 DataLength;
   /**< Length of Data, in bits. The value shall be a multiplicity of 8. */
   uint8  DataIDList[16];
   /**< An array of appropriately chosen Data IDs for protection against masquerading. */
   uint8  MaxDeltaCounterInit;
   /**< Initial maximum allowed gap between two counter values of two consecutively
    * received valid Data. */
   uint8  MaxNoNewOrRepeatedData;
   /**< Number of consecutively received valid counter values required for validating the
    * consistency of the counter after an unexpected behavior of a received counter was detected */
   uint8  SyncCounterInit;
   /**< Initial maximum allowed gap between two consecutively received valid counter values. */
} E2E_P02ConfigType;

/*Serl Func Proto*/
FUNC(uint8, E2EPW_CODE) E2EPW_PACK_ParkCfgStatPr2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
);

FUNC(uint8, E2EPW_CODE) E2EPW_PACK_BrkRqParkPr2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
);

FUNC(uint8, E2EPW_CODE) E2EPW_PACK_StRqParkPr2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
);

FUNC(uint8, E2EPW_CODE) E2EPW_PACK_Park_Disp_Rq_AR2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
);

static FUNC(uint8, E2EPW_CODE) E2EPW_PACK_RemPark_Rq_Pr2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
);

/* End2End Protection Configuration */
static const E2E_P02ConfigType Cfg_Brk_Rq_PARK_Pr2 = {

           E2EPW_PACK_BrkRqParkPr2,
		   64u                                                 /* uint16 DataLength - length of IPDU in bits */
		  , Profile02_DataID_Brk_Rq_PARK_Pr2                 /* uint8  DataIDList [16] - list of unique IPDU IDs (16) */
		  , 255u                                             /* uint8  MaxDeltaCounterInit - not used for transmission */

		  , 255u                                             /* uint8  MaxNoNewOrRepeatedData - Number of consecutively received
		                                                                                         valid counter values required */
		  , 255u                                             /* uint8  SyncCounterInit - Initial maximum allowed gap between two
		                                                                                  consecutively received valid counter values */

		};

static const E2E_P02ConfigType Cfg_ParkCfg_Stat_Pr2 = {

            E2EPW_PACK_ParkCfgStatPr2,
		   32u                                                 /* uint16 DataLength - length of IPDU in bits */
		  , Profile02_DataID_ParkCfg_Stat_Pr2                 /* uint8  DataIDList [16] - list of unique IPDU IDs (16) */
		  , 255u                                             /* uint8  MaxDeltaCounterInit - not used for transmission */

		  , 255u                                             /* uint8  MaxNoNewOrRepeatedData - Number of consecutively received
		                                                                                         valid counter values required */
		  , 255u                                             /* uint8  SyncCounterInit - Initial maximum allowed gap between two
		                                                                                  consecutively received valid counter values */

		};

static const E2E_P02ConfigType Cfg_St_Rq_PARK_Pr2 = {

           E2EPW_PACK_StRqParkPr2,

		   64u                                              /* uint16 DataLength - length of IPDU in bits */
		  , Profile02_DataID_St_Rq_PARK_Pr2                 /* uint8  DataIDList [16] - list of unique IPDU IDs (16) */
		  , 255u                                             /* uint8  MaxDeltaCounterInit - not used for transmission */

		  , 255u                                             /* uint8  MaxNoNewOrRepeatedData - Number of consecutively received
		                                                                                         valid counter values required */
		  , 255u                                             /* uint8  SyncCounterInit - Initial maximum allowed gap between two
		                                                                                  consecutively received valid counter values */

		};
      
static const E2E_P02ConfigType Cfg_Park_Disp_Rq_AR2 = {

           E2EPW_PACK_Park_Disp_Rq_AR2,

		   64u                                              /* uint16 DataLength - length of IPDU in bits */
		  , Profile02_DataID_Park_Disp_Rq_AR2                 /* uint8  DataIDList [16] - list of unique IPDU IDs (16) */
		  , 255u                                             /* uint8  MaxDeltaCounterInit - not used for transmission */

		  , 255u                                             /* uint8  MaxNoNewOrRepeatedData - Number of consecutively received
		                                                                                         valid counter values required */
		  , 255u                                             /* uint8  SyncCounterInit - Initial maximum allowed gap between two
		                                                                                  consecutively received valid counter values */

		};     

static const E2E_P02ConfigType Cfg_RemPark_Rq_Pr2 = {

           E2EPW_PACK_StRqParkPr2,

		   32u                                              /* uint16 DataLength - length of IPDU in bits */
		  , Profile02_DataID_RemPark_Rq_Pr2                 /* uint8  DataIDList [16] - list of unique IPDU IDs (16) */
		  , 255u                                             /* uint8  MaxDeltaCounterInit - not used for transmission */

		  , 255u                                             /* uint8  MaxNoNewOrRepeatedData - Number of consecutively received
		                                                                                         valid counter values required */
		  , 255u                                             /* uint8  SyncCounterInit - Initial maximum allowed gap between two
		                                                                                  consecutively received valid counter values */

		};      

/**
 * \brief State of the sender for a Data protected with E2E Profile 2.
 */
typedef struct {
   uint8 Counter;
   /**< Counter to be used for protecting the Data. The counter is incremented modulo 16 */
} E2E_P02SenderStateType;

typedef struct
{
  uint8 CRC_Brk_Rq_PARK_Pr2;
  uint8 PARK_Brk_Stat;
  uint8 PARK_BrkMd_Rq;
  uint8 PARK_CtrlMd_Rq;
  uint8 PARK_DangerMd_Rq;
  uint8 PARK_Park_Stat;
  uint8 PARK_ParkType;
  uint16 PARK_RemainDist;
  uint8 PARK_RemainDist_Stat;
  uint8 PARK_TxDrvPosn_Rq;
  uint8 PARK_VehSpd_Rq;
  uint8 Rsrv1_Brk_Rq_PARK_Pr2;
  uint8 Rsrv2_Brk_Rq_PARK_Pr2;
  uint8 Rsrv3_Brk_Rq_PARK_Pr2;
  uint8 SQC_Brk_Rq_PARK_Pr2;
} Brk_Rq_PARK_Pr2;

typedef struct
{
  uint8 CRC_ParkCfg_Stat_Pr2;
  uint8 ParkStyle_V2;
  uint8 Rsrv1_ParkCfg_Stat_Pr2;
  uint8 Rsrv2_ParkCfg_Stat_Pr2;
  uint8 SQC_ParkCfg_Stat_Pr2;
  uint8 ParkGuid_Stat;
}ParkCfg_Stat_Pr2;

typedef struct
{
  uint8 CRC_St_Rq_PARK_Pr2;
  uint8 PARK_Sp_Stat;
  uint8 PARK_Stat_PARK;
  uint16 PPS_AddRf_Rq;
  uint8 PPS_Stat_PARK;
  uint8 SQC_St_Rq_PARK_Pr2;
  uint16 FtWhlAngl_Rq_PARK;
  uint16 Rsrv2_St_Rq_PARK_Pr2;
}St_Rq_PARK_Pr2;

typedef struct
{
  uint8 Park_IconDisp_Rq;
  uint8 Park_Sound_Rq;
  uint8 Park_Warn1_Disp_Rq;
  uint8 CRC_Park_Disp_Rq_Pr2;
  uint8 SQC_Park_Disp_Rq_Pr2;
  uint8 Rsrv1_Park_Disp_Rq_Pr2;
  uint8 Rsrv2_Park_Disp_Rq_Pr2;
  uint8 Rsrv3_Park_Disp_Rq_Pr2;
  uint8 Rsrv4_Park_Disp_Rq_Pr2;
  uint8 PARK_Hitch_Instrct_Disp_Rq;
  uint8 PARK_Hitch_KinkAngl;
  uint8 PARK_Hitch_KinkAngl_Rng;
  uint8 PARK_Pilot_Md;
} Park_Disp_Rq_AR2;

typedef struct
{
  uint8 CRC_RemPark_Rq_Pr2;
  uint8 KG_RemPark_Rq;
  uint8 RemPark_VehLk_Rq;
  uint8 SQC_RemPark_Rq_Pr2;
  uint8 RemPark_Actv;
  uint8 TurnInd_Rq_RemPark;
  uint8 EIS_RemPark_Rq;
  uint8 RemPark_Auth_Rq;
  uint8 Rsrv3_RemPark_Rq_Pr2;
} RemPark_Rq_Pr2;

/**
 * \brief Calculate the Crc value
 *
 * \param[in] Config  Pointer to static configuration.
 * \param[in] Data    Pointer to Data to be protected.
 * \param[in] Counter Rx / Tx Counter for retrieving the counter-specific DataId
 *
 * \pre All input parameters are valid
 *
 * \return Crc value
 */
FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P02Protect
(
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   CONSTP2VAR(E2E_P02SenderStateType, AUTOMATIC, E2E_APPL_DATA) State,
   CONSTP2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) Data
);


/**
 * \brief Calculate the Crc value
 *
 * \param[in] Config  Pointer to static configuration.
 * \param[in] Data    Pointer to Data to be protected.
 * \param[in] Counter Rx / Tx Counter for retrieving the counter-specific DataId
 *
 * \pre All input parameters are valid
 *
 * \return Crc value
 */
STATIC FUNC(uint8, E2E_CODE) E2E_EB_CalculateCrc
(
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   CONSTP2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data,
   const uint8 Counter
);


/** \brief Calculation of CRC8 with the Polynomial 0x2F
 **
 ** This function performs the calculation of a 8-bit CRC value with the Polynomial 0x2F
 ** over the memory block referenced by \p SCrc_DataPtr of byte length \p
 ** SCrc_Length.
 **
 ** \pre SCrc_DataPtr does not equal a Null Pointer
 **
 ** \param[in] SCrc_DataPtr Valid pointer to start address of data block
 ** \param[in] SCrc_Length  Length of data block in bytes
 ** \param[in] SCrc_StartValue8 Initial Value
 ** \param[in] SCrc_IsFirstCall
 **    TRUE: First call in a sequence or individual CRC calculation;
 **          start from initial value, ignore Crc_StartValue8.
 **    FALSE: Subsequent call in a call sequence;
 **           Crc_StartValue8 is interpreted to be the return value
 **           of the previous function call.
 ** \return calculated CRC8 value
 **
 ** \Reentrancy{Reentrant}
 ** \Synchronicity{Synchronous} */
 FUNC(uint8, CRC_CODE) SCrc_CalculateCRC8H2F
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint8                                     SCrc_StartValue8H2F,
    boolean                                   SCrc_IsFirstCall
);

#endif /* E2E_PLATFROM_H_ */
