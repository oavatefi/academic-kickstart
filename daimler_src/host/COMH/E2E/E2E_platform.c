
#include "E2E_platfrom.h"
#include "E2EPW_Int.h"
/** \brief Macro for packing a Data Element member into a serialized data memory.
 *
 * \param[in] appData The Data Element which shall be serialized
 * \param[in] memptr Pointer to the memory which shall contain the serialized Data Element
 * \param[in] structmember Member of the Data Element which shall be serialized
 * \param[in] bitpos Bit position of the Data Element member in the serialized memory
 * \param[in] bitlength Bit length of the Data Element member in the serialized memory
 * \param[in] sigtype Signal type of the Data Element member in the serialized memory.
 *            Shall be one of: E2EPW_[LE|BE]_[BL|U8|U16|U32|S8|S16|S32] or E2EPW_OP_U8N
 * \param[in] nbytes Number of occupied bytes of the Data Element member in the serialized memory
 */
#define E2EPW_PACKSIG(appData,memptr,structmember,bitpos,bitlength,sigtype,nbytes) \
   do {\
      E2EPW_CHECKRANGE_ ## sigtype(bitlength,(appData)->structmember); \
      E2EPW_PACKSIG_ ## sigtype((bitlength),(bitpos),(memptr),(appData)->structmember,nbytes); \
   } while (0)

FUNC(Std_ReturnType, E2E_CODE) E2E_EB_P02Protect
(
   P2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   P2VAR(E2E_P02SenderStateType, AUTOMATIC, E2E_APPL_DATA) State,
   P2VAR(uint8, AUTOMATIC, E2E_APPL_DATA) Data
)
{
   /* update sequence counter modulo 16 */
   if (E2EP02_COUNTER_MAX > State->Counter)
   {
      State->Counter++;
   }
   else
   {
      State->Counter = 0U;
   }

   /* write sequence counter to first nibble of byte 1 */
   Data[1] &= 0xF0U;
   Data[1] |= State->Counter & 0x0FU;

   /* calculate CRC and write CRC to byte 0 */
   Data[0] = E2E_EB_CalculateCrc(Config, Data, State->Counter);

   return E2E_E_OK;
}


STATIC FUNC(uint8, E2E_CODE) E2E_EB_CalculateCrc
(
   CONSTP2CONST(E2E_P02ConfigType, AUTOMATIC, E2E_APPL_CONST) Config,
   CONSTP2CONST(uint8, AUTOMATIC, E2E_APPL_DATA) Data,
   const uint8 Counter
)
{
   const uint8 ArrayLength = (uint8)(Config->DataLength / 8U);
   P2CONST(uint8, AUTOMATIC, E2E_APPL_CONST) DataId =
         &(Config->DataIDList[Counter]);
   uint8 Crc;

   Crc = SCrc_CalculateCRC8H2F(&Data[1],
                              (uint32)ArrayLength - 1U,
                              E2EP02_CRC_STARTVALUE,
                              TRUE);
   Crc = SCrc_CalculateCRC8H2F(DataId,
                              1U,
                              Crc,
                              FALSE);

   return Crc;
}


FUNC(uint8, SCRC_CODE) SCrc_CalculateCRC8H2F
(
    P2CONST(uint8, AUTOMATIC, SCRC_APPL_DATA) SCrc_DataPtr,
    uint32                                    SCrc_Length,
    uint8                                     SCrc_StartValue8H2F,
    boolean                                   SCrc_IsFirstCall
)
{
    uint32 i;

    /* Check if this is the first call in a sequence or individual CRC calculation.
     * If so, then use the defined initial value. Otherwise, the start value
     * is interpreted as the return value of the previous function call.
     */
    if (FALSE == SCrc_IsFirstCall)
    {
        SCrc_StartValue8H2F = (uint8)(SCrc_StartValue8H2F ^ SCRC_CRC8H2F_XORVALUE);
    } else {
        SCrc_StartValue8H2F = SCRC_CRC8H2F_INITIALVALUE;
    }

    /* Process all data (byte wise) */
    for (i=0U; i<SCrc_Length; ++i)
    {
        SCrc_StartValue8H2F = SCrc_Table8H2F[(uint8)(SCrc_StartValue8H2F ^ SCrc_DataPtr[i])];
    }

    /* Apply xor-value of 0xFF as specified in ASR R4.0 CRC SWS. */
    return (uint8)(SCrc_StartValue8H2F ^ SCRC_CRC8H2F_XORVALUE);
}

FUNC(uint8, E2EPW_CODE) E2EPW_PACK_BrkRqParkPr2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
)
{
   CONSTP2CONST(Brk_Rq_PARK_Pr2, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DataElement =
      (P2CONST(Brk_Rq_PARK_Pr2, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_SrcPtr;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_MemPtr =
      (P2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_DstPtr;
   uint8 E2EPW_RangeCheckRetVal = E2EPW_RANGECHK_VALID;
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, CRC_Brk_Rq_PARK_Pr2, 0U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, SQC_Brk_Rq_PARK_Pr2, 8U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_Brk_Stat, 12U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_Park_Stat, 16U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_ParkType, 20U, 3U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_DangerMd_Rq, 23U, 1U, E2EPW_LE_BL, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_RemainDist, 24U, 11U, E2EPW_LE_U16, 2);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_BrkMd_Rq, 35U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_VehSpd_Rq, 37U, 7U, E2EPW_LE_U8, 2);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_RemainDist_Stat, 44U, 3U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv1_Brk_Rq_PARK_Pr2, 47U, 1U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_CtrlMd_Rq, 48U, 3U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_TxDrvPosn_Rq, 51U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv2_Brk_Rq_PARK_Pr2, 55U, 1U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv3_Brk_Rq_PARK_Pr2, 56U, 8U, E2EPW_LE_U8, 1);
   return E2EPW_RangeCheckRetVal;
}

FUNC(uint8, E2EPW_CODE) E2EPW_PACK_ParkCfgStatPr2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
)
{
   CONSTP2CONST(ParkCfg_Stat_Pr2, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DataElement =
      (P2CONST(ParkCfg_Stat_Pr2, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_SrcPtr;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_MemPtr =
      (P2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_DstPtr;
   uint8 E2EPW_RangeCheckRetVal = E2EPW_RANGECHK_VALID;
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, CRC_ParkCfg_Stat_Pr2, 0U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, SQC_ParkCfg_Stat_Pr2, 8U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, ParkGuid_Stat, 12U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv1_ParkCfg_Stat_Pr2, 14U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv2_ParkCfg_Stat_Pr2, 16U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, ParkStyle_V2, 24U, 8U, E2EPW_LE_U8, 1);
   return E2EPW_RangeCheckRetVal;
}

FUNC(uint8, E2EPW_CODE) E2EPW_PACK_StRqParkPr2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
)
{
   CONSTP2CONST(St_Rq_PARK_Pr2, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DataElement =
      (P2CONST(St_Rq_PARK_Pr2, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_SrcPtr;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_MemPtr =
      (P2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_DstPtr;
   uint8 E2EPW_RangeCheckRetVal = E2EPW_RANGECHK_VALID;
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, CRC_St_Rq_PARK_Pr2, 0U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, SQC_St_Rq_PARK_Pr2, 8U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_Stat_PARK, 12U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PPS_AddRf_Rq, 16U, 13U, E2EPW_LE_U16, 2);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PPS_Stat_PARK, 29U, 4U, E2EPW_LE_U8, 2);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, FtWhlAngl_Rq_PARK, 33U, 15U, E2EPW_LE_U16, 2);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_Sp_Stat, 48U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv2_St_Rq_PARK_Pr2, 50U, 14U, E2EPW_LE_U16, 2);
   return E2EPW_RangeCheckRetVal;
}

FUNC(uint8, E2EPW_CODE) E2EPW_PACK_Park_Disp_Rq_AR2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
)
{
   CONSTP2CONST(Park_Disp_Rq_AR2, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DataElement =
      (P2CONST(Park_Disp_Rq_AR2, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_SrcPtr;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_MemPtr =
      (P2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_DstPtr;
   uint8 E2EPW_RangeCheckRetVal = E2EPW_RANGECHK_VALID;
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, CRC_Park_Disp_Rq_Pr2, 0U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, SQC_Park_Disp_Rq_Pr2, 8U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_Pilot_Md, 12U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Park_Sound_Rq, 14U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Park_IconDisp_Rq, 16U, 5U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_Hitch_Instrct_Disp_Rq, 21U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv1_Park_Disp_Rq_Pr2, 23U, 1U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv2_Park_Disp_Rq_Pr2, 24U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Park_Warn1_Disp_Rq, 32U, 5U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv3_Park_Disp_Rq_Pr2, 37U, 3U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv4_Park_Disp_Rq_Pr2, 40U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_Hitch_KinkAngl, 48U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, PARK_Hitch_KinkAngl_Rng, 56U, 8U, E2EPW_LE_U8, 1);
   return E2EPW_RangeCheckRetVal;
}

static FUNC(uint8, E2EPW_CODE) E2EPW_PACK_RemPark_Rq_Pr2
(
   CONSTP2CONST(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_SrcPtr,
   CONSTP2VAR(void, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DstPtr
)
{
   CONSTP2CONST(RemPark_Rq_Pr2, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_DataElement =
      (P2CONST(RemPark_Rq_Pr2, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_SrcPtr;
   CONSTP2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA) E2EPW_MemPtr =
      (P2VAR(uint8, AUTOMATIC, E2EPW_APPL_DATA)) E2EPW_DstPtr;
   uint8 E2EPW_RangeCheckRetVal = E2EPW_RANGECHK_VALID;
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, CRC_RemPark_Rq_Pr2, 0U, 8U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, SQC_RemPark_Rq_Pr2, 8U, 4U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, KG_RemPark_Rq, 12U, 3U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, RemPark_Actv, 15U, 1U, E2EPW_LE_BL, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, RemPark_VehLk_Rq, 16U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, TurnInd_Rq_RemPark, 18U, 3U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, EIS_RemPark_Rq, 21U, 3U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, RemPark_Auth_Rq, 24U, 2U, E2EPW_LE_U8, 1);
   E2EPW_PACKSIG(E2EPW_DataElement, E2EPW_MemPtr, Rsrv3_RemPark_Rq_Pr2, 26U, 6U, E2EPW_LE_U8, 1);
   return E2EPW_RangeCheckRetVal;
}