/*
 *	 ***************************************************************
 *	  Licensed Materials - Property of IBM
 *	  (C)Copyright IBM Corporation 1989, 2010. All Rights Reserved.
 *
 *	  Module Name: cgstuff.h
 *	 ***************************************************************
 */


#ifndef __CGSTUFF_H_
#define __CGSTUFF_H_

/* The following is used internally by the generated code */
/* They should be defined appropriately for each architecture */

#ifndef INT8_T
#define INT8_T
typedef unsigned char      int8;
#endif
#ifndef INT16_T
#define INT16_T
typedef unsigned short int int16;
#endif
#ifndef INT32_T
#define INT32_T
typedef unsigned long int  int32;
#endif
#ifndef SINT8_T
#define SINT8_T
typedef char      sint8;
#endif
#ifndef SINT16_T
#define SINT16_T
typedef short int sint16;
#endif
#ifndef SINT32_T
#define SINT32_T
typedef long int  sint32;
#endif
#ifndef UINT8_T
#define UINT8_T
typedef unsigned char      uint8;
#endif
#ifndef UINT16_T
#define UINT16_T
typedef unsigned short int uint16;
#endif
#ifndef UINT32_T
#define UINT32_T
typedef unsigned long int  uint32;
#endif

#ifndef DOUBLE_T
#define DOUBLE_T
typedef double         real;
#endif

#define MSBIT_FIRST_0_to_7 \
  unsigned int b7:1;\
  unsigned int b6:1;\
  unsigned int b5:1;\
  unsigned int b4:1;\
  unsigned int b3:1;\
  unsigned int b2:1;\
  unsigned int b1:1;\
  unsigned int b0:1;

#define MSBIT_FIRST_0_to_7_UCHAR \
  unsigned char b7:1;\
  unsigned char b6:1;\
  unsigned char b5:1;\
  unsigned char b4:1;\
  unsigned char b3:1;\
  unsigned char b2:1;\
  unsigned char b1:1;\
  unsigned char b0:1;

#define MSBIT_FIRST_8_to_15 \
  unsigned int b15:1;\
  unsigned int b14:1;\
  unsigned int b13:1;\
  unsigned int b12:1;\
  unsigned int b11:1;\
  unsigned int b10:1;\
  unsigned int b9:1; \
  unsigned int b8:1;

#define MSBIT_FIRST_8_to_15_UCHAR \
  unsigned char b15:1;\
  unsigned char b14:1;\
  unsigned char b13:1;\
  unsigned char b12:1;\
  unsigned char b11:1;\
  unsigned char b10:1;\
  unsigned char b9:1; \
  unsigned char b8:1;

#define MSBIT_FIRST_16_to_23 \
  unsigned int b23:1;\
  unsigned int b22:1;\
  unsigned int b21:1;\
  unsigned int b20:1;\
  unsigned int b19:1;\
  unsigned int b18:1;\
  unsigned int b17:1;\
  unsigned int b16:1;

#define MSBIT_FIRST_16_to_23_UCHAR \
  unsigned char b23:1;\
  unsigned char b22:1;\
  unsigned char b21:1;\
  unsigned char b20:1;\
  unsigned char b19:1;\
  unsigned char b18:1;\
  unsigned char b17:1;\
  unsigned char b16:1;

#define MSBIT_FIRST_24_to_31 \
  unsigned int b31:1;\
  unsigned int b30:1;\
  unsigned int b29:1;\
  unsigned int b28:1;\
  unsigned int b27:1;\
  unsigned int b26:1;\
  unsigned int b25:1;\
  unsigned int b24:1;

#define MSBIT_FIRST_24_to_31_UCHAR \
  unsigned char b31:1;\
  unsigned char b30:1;\
  unsigned char b29:1;\
  unsigned char b28:1;\
  unsigned char b27:1;\
  unsigned char b26:1;\
  unsigned char b25:1;\
  unsigned char b24:1;

#define LSBIT_FIRST_0_to_7 \
  unsigned int b0:1;\
  unsigned int b1:1;\
  unsigned int b2:1;\
  unsigned int b3:1;\
  unsigned int b4:1;\
  unsigned int b5:1;\
  unsigned int b6:1;\
  unsigned int b7:1;

#define LSBIT_FIRST_0_to_7_UCHAR \
  unsigned char b0:1;\
  unsigned char b1:1;\
  unsigned char b2:1;\
  unsigned char b3:1;\
  unsigned char b4:1;\
  unsigned char b5:1;\
  unsigned char b6:1;\
  unsigned char b7:1;

#define LSBIT_FIRST_8_to_15 \
  unsigned int b8:1; \
  unsigned int b9:1; \
  unsigned int b10:1;\
  unsigned int b11:1;\
  unsigned int b12:1;\
  unsigned int b13:1;\
  unsigned int b14:1;\
  unsigned int b15:1;

#define LSBIT_FIRST_8_to_15_UCHAR \
  unsigned char b8:1; \
  unsigned char b9:1; \
  unsigned char b10:1;\
  unsigned char b11:1;\
  unsigned char b12:1;\
  unsigned char b13:1;\
  unsigned char b14:1;\
  unsigned char b15:1;

#define LSBIT_FIRST_16_to_23 \
  unsigned int b16:1;\
  unsigned int b17:1;\
  unsigned int b18:1;\
  unsigned int b19:1;\
  unsigned int b20:1;\
  unsigned int b21:1;\
  unsigned int b22:1;\
  unsigned int b23:1;

#define LSBIT_FIRST_16_to_23_UCHAR \
  unsigned char b16:1;\
  unsigned char b17:1;\
  unsigned char b18:1;\
  unsigned char b19:1;\
  unsigned char b20:1;\
  unsigned char b21:1;\
  unsigned char b22:1;\
  unsigned char b23:1;

#define LSBIT_FIRST_24_to_31 \
  unsigned int b24:1;\
  unsigned int b25:1;\
  unsigned int b26:1;\
  unsigned int b27:1;\
  unsigned int b28:1;\
  unsigned int b29:1;\
  unsigned int b30:1;\
  unsigned int b31:1;

#define LSBIT_FIRST_24_to_31_UCHAR \
  unsigned char b24:1;\
  unsigned char b25:1;\
  unsigned char b26:1;\
  unsigned char b27:1;\
  unsigned char b28:1;\
  unsigned char b29:1;\
  unsigned char b30:1;\
  unsigned char b31:1;


typedef struct AMCBitsStruct8 {
#ifdef MSBIT_FIRST
#ifdef USE_UCHAR_BITFIELD
	MSBIT_FIRST_0_to_7_UCHAR
#else
	MSBIT_FIRST_0_to_7
#endif
#else
#ifdef USE_UCHAR_BITFIELD
	LSBIT_FIRST_0_to_7_UCHAR
#else
	LSBIT_FIRST_0_to_7
#endif
#endif
}AMCBitsStruct8;




typedef struct AMCBitsStruct16 {
#ifdef LSBYTE_FIRST
#ifdef MSBIT_FIRST
#ifdef USE_UCHAR_BITFIELD
	MSBIT_FIRST_0_to_7_UCHAR
	MSBIT_FIRST_8_to_15_UCHAR
#else
	MSBIT_FIRST_0_to_7
	MSBIT_FIRST_8_to_15
#endif
#else
#ifdef USE_UCHAR_BITFIELD
	LSBIT_FIRST_0_to_7_UCHAR
	LSBIT_FIRST_8_to_15_UCHAR
#else
	LSBIT_FIRST_0_to_7
	LSBIT_FIRST_8_to_15
#endif
#endif
#else
#ifdef MSBIT_FIRST
#ifdef USE_UCHAR_BITFIELD
	MSBIT_FIRST_8_to_15_UCHAR
	MSBIT_FIRST_0_to_7_UCHAR
#else
	MSBIT_FIRST_8_to_15
	MSBIT_FIRST_0_to_7
#endif
#else
#ifdef USE_UCHAR_BITFIELD
	LSBIT_FIRST_8_to_15_UCHAR
	LSBIT_FIRST_0_to_7_UCHAR
#else
	LSBIT_FIRST_8_to_15
	LSBIT_FIRST_0_to_7
#endif
#endif
#endif
}AMCBitsStruct16;



typedef struct AMCBitsStruct32 {
#ifdef LSBYTE_FIRST
#ifdef MSBIT_FIRST
#ifdef USE_UCHAR_BITFIELD
	MSBIT_FIRST_0_to_7_UCHAR
	MSBIT_FIRST_8_to_15_UCHAR
	MSBIT_FIRST_16_to_23_UCHAR
	MSBIT_FIRST_24_to_31_UCHAR
#else
	MSBIT_FIRST_0_to_7
	MSBIT_FIRST_8_to_15
	MSBIT_FIRST_16_to_23
	MSBIT_FIRST_24_to_31
#endif
#else
#ifdef USE_UCHAR_BITFIELD
	LSBIT_FIRST_0_to_7_UCHAR
	LSBIT_FIRST_8_to_15_UCHAR
	LSBIT_FIRST_16_to_23_UCHAR
	LSBIT_FIRST_24_to_31_UCHAR
#else
	LSBIT_FIRST_0_to_7
	LSBIT_FIRST_8_to_15
	LSBIT_FIRST_16_to_23
	LSBIT_FIRST_24_to_31
#endif
#endif
#else
#ifdef MSBIT_FIRST
#ifdef USE_UCHAR_BITFIELD
	MSBIT_FIRST_8_to_15_UCHAR
	MSBIT_FIRST_0_to_7_UCHAR
	MSBIT_FIRST_24_to_31_UCHAR
	MSBIT_FIRST_16_to_23_UCHAR
#else
	MSBIT_FIRST_8_to_15
	MSBIT_FIRST_0_to_7
	MSBIT_FIRST_24_to_31
	MSBIT_FIRST_16_to_23
#endif
#else
#ifdef USE_UCHAR_BITFIELD
	LSBIT_FIRST_8_to_15_UCHAR
	LSBIT_FIRST_0_to_7_UCHAR
	LSBIT_FIRST_24_to_31_UCHAR
	LSBIT_FIRST_16_to_23_UCHAR
#else
	LSBIT_FIRST_8_to_15
	LSBIT_FIRST_0_to_7
	LSBIT_FIRST_24_to_31
	LSBIT_FIRST_16_to_23
#endif
#endif
#endif
}AMCBitsStruct32;


/* Constants Definitions */
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef M_PI
#ifdef USE_FXP
#define M_PI 0x6487ED51
#define M_PI_FXP_SHIFT 29
#else
#define M_PI 3.14159265358979323846
#endif
#endif
#ifndef M_E
#ifdef USE_FXP
#define	M_E 0x15BF0A8B
#define	M_E_FXP_SHIFT 27
#else
#define	M_E  2.7182818284590452354
#endif
#endif


/* Fix Point Support Macros */
#ifndef FXP2INT
#define FXP2INT(FPvalue, FPshift) ((FPvalue) >> (FPshift))
#endif
#ifndef LS_FXP2FXP8
#define LS_FXP2FXP8(FPvalue, fromFPshift, toFPshift) (((sint8)(FPvalue)) << ((toFPshift) - (fromFPshift)))
#endif
#ifndef RS_FXP2FXP8
#define RS_FXP2FXP8(FPvalue, fromFPshift, toFPshift) (((sint8)(FPvalue)) >> ((fromFPshift) - (toFPshift)))
#endif
#ifndef LS_FXP2FXP16
#define LS_FXP2FXP16(FPvalue, fromFPshift, toFPshift) (((sint16)(FPvalue)) << ((toFPshift) - (fromFPshift)))
#endif
#ifndef RS_FXP2FXP16
#define RS_FXP2FXP16(FPvalue, fromFPshift, toFPshift) (((sint16)(FPvalue)) >> ((fromFPshift) - (toFPshift)))
#endif
#ifndef LS_FXP2FXP32
#define LS_FXP2FXP32(FPvalue, fromFPshift, toFPshift) (((sint32)(FPvalue)) << ((toFPshift) - (fromFPshift)))
#endif
#ifndef RS_FXP2FXP32
#define RS_FXP2FXP32(FPvalue, fromFPshift, toFPshift) (((sint32)(FPvalue)) >> ((fromFPshift) - (toFPshift)))
#endif
#ifndef FXP2DOUBLE
#define FXP2DOUBLE(FPvalue, FPshift) (((double)(FPvalue)) / (1 << (FPshift)))
#endif
#ifndef DOUBLE2FXP
#define DOUBLE2FXP(Dvalue, FPshift) ((unsigned long int)((Dvalue) * (1 << (FPshift))))
#endif

/* Predefined Mathematical Functions */
#ifndef ABS
#define ABS(x)              (((x)>0)?(x):-(x))
#endif
#ifndef MAX
#define MAX(x,y)            (((x)>(y))?(x):(y))
#endif
#ifndef MIN
#define MIN(x,y)            (((x)<(y))?(x):(y))
#endif
#ifndef MOD
#define MOD(x,y)            ((x) % (y))
#endif
#ifndef ROUND
#ifdef USE_FXP
#define ROUND_INT(f)            (f)
#define ROUND_FXP(f, FPshift)   (FXP2INT((((f)>0) ? (f + (1 << ((FPshift)-1))) : (f - (1 << ((FPshift)-1)))), FPshift))
#else
#define ROUND(f)            (((f)>0) ? (int)((f) + 0.5) : (int)((f) - 0.5))
#endif
#endif
#ifndef TRUNC
#ifdef USE_FXP
#define TRUNC(f, FPshift)   (FXP2INT(f, FPshift))
#else
#define TRUNC(f)            ((int)(f))
#endif
#endif
#ifndef LOG
#define LOG(x)              log(x)
#endif

#ifdef NO_LOG2
#ifndef LOG2
#define LOG2(x)             (log(x)/log(2.0))
#endif
#else
#ifndef LOG2
#define LOG2(x)             log2(x)
#endif
#endif

#ifndef LOG10
#define LOG10(x)      log10(x)
#endif
#ifndef EXP
#define EXP(x)        exp(x)
#endif
#ifndef SQRT
#define SQRT(x)       sqrt(x)
#endif
#ifndef POWER
#define POWER(a,b)     pow((a),(b))
#endif
#ifndef SIN
#define SIN(x)        sin(x)
#endif
#ifndef COS
#define COS(x)        cos(x)
#endif
#ifndef TAN
#define TAN(x)        tan(x)
#endif
#ifndef SINH
#define SINH(x)       sinh(x)
#endif
#ifndef COSH
#define COSH(x)       cosh(x)
#endif
#ifndef TANH
#define TANH(x)       tanh(x)
#endif
#ifndef SINE
#define SINE(x)       sin(x)
#endif
#ifndef COSINE
#define COSINE(x)     cos(x)
#endif
#ifndef TANGENT
#define TANGENT(x)    tan(x)
#endif
#ifndef ARCTAN
#define ARCTAN(x)     atan(x)
#endif
#ifndef ARCSIN
#define ARCSIN(x)     asin(x)
#endif
#ifndef ARCCOS
#define ARCCOS(x)     acos(x)
#endif
#ifndef ATAN
#define ATAN(x)       atan(x)
#endif
#ifndef ATAN2
#define ATAN2(x,y)    atan2((x),(y))
#endif
#ifndef ASIN
#define ASIN(x)       asin(x)
#endif
#ifndef ACOS
#define ACOS(x)       acos(x)
#endif
#ifndef SIND
#define SIND(x)       sin((x)*M_PI/180.0)
#endif
#ifndef COSD
#define COSD(x)       cos((x)*M_PI/180.0)
#endif
#ifndef TAND
#define TAND(x)       tan((x)*M_PI/180.0)
#endif
#ifndef ATAND
#define ATAND(x)      (180.0*atan(x)/M_PI)
#endif
#ifndef ATAN2D
#define ATAN2D(x,y)   (180.0*atan2(x,y)/M_PI)
#endif
#ifndef ASIND
#define ASIND(x)      (180.0*asin(x)/M_PI)
#endif
#ifndef ACOSD
#define ACOSD(x)      (180.0*acos(x)/M_PI)
#endif
#ifndef STRING_EXTRACT
#define STRING_EXTRACT(S, I1, I2) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef STRING_INDEX
#define STRING_INDEX(S1, I, S2) ( ((int) strstr((S1+(I)),(S2)) ) != 0? ( (int)  ( strstr((S1+(I)),(S2)) - (S1)) ) : -1)
#endif
#ifndef STRING_CONCAT
#define STRING_CONCAT(S1, S2) strcat(S1, S2)
#endif
#ifndef STRING_LENGTH
#define STRING_LENGTH(S) strlen(S)
#endif
#ifndef CHAR_TO_ASCII
#define CHAR_TO_ASCII(S) (int)S[0]
#endif
#ifndef ASCII_TO_CHAR
#define ASCII_TO_CHAR(I) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef INT_TO_STRING
#define INT_TO_STRING(I) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef STRING_TO_INT
#define STRING_TO_INT(S) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef RIMC_MALLOC
#define RIMC_MALLOC(Sz) malloc(Sz)
#endif
#ifndef RIMC_REALLOC
#define RIMC_REALLOC(Blk, Sz) realloc(Blk, Sz)
#endif
#ifndef SAVE_COPY_STR
#define SAVE_COPY_STR(D, S) if(D==(char*)0){D = (char*)RIMC_MALLOC(STRING_LENGTH(S)+1);} else {D = (char*)RIMC_REALLOC(D, STRING_LENGTH(S)+1);} strcpy(D, S);
#endif
#ifndef RAND_UNIFORM
#define RAND_UNIFORM(F1,F2) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef RAND_EXPONENTIAL
#define RAND_EXPONENTIAL(F1) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef RAND_BINOMIAL
#define RAND_BINOMIAL(L1,F1) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef RAND_POISSON
#define RAND_POISSON(F1) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef RAND_NORMAL
#define RAND_NORMAL(L1,L2) ** NOT IMPLEMENTED FUNCTION **
#endif
#ifndef ANY
#define ANY(EVARR) ANY_##EVARR
#endif
#ifndef ALL
#define ALL(EVARR) ALL_##EVARR
#endif
#ifndef LENGTH_OF
#define LENGTH_OF(ARR) LENGTH_OF_##ARR
#endif
#ifndef RINDEX
#define RINDEX(ARR) RINDEX_##ARR
#endif
#ifndef LINDEX
#define LINDEX(ARR) LINDEX_##ARR
#endif
#ifndef MEM_SET
#ifndef AVOID_MEMSET
#define MEM_SET(X, V) memset((void*)&(X), 0, sizeof(X))
#else
#define USE_RIMC_MEM_SET
#define MEM_SET(X, V) rimc_mem_set((unsigned char*)&(X), sizeof(X), 0)
#endif
#endif
#ifndef MEM_CPY
#ifndef AVOID_MEMCPY
#define MEM_CPY(D, S) memcpy((void*)(D), (const void *)(S), sizeof(D))
#else
#define USE_RIMC_MEM_CPY
#define MEM_CPY(D, S) rimc_mem_cpy((unsigned char*)(D), (const unsigned char*)(S), sizeof(D))
#endif
#endif


/** CG Macros **/
#ifdef TRACE_TASK
#ifndef TRACE_TASK_STARTED
extern void traceTask();
#define TRACE_TASK_STARTED(t) traceTask((t),'S')
#endif
#ifndef TRACE_TASK_TERMINATED
extern void traceTask();
#define TRACE_TASK_TERMINATED(t) traceTask((t),'T')
#endif
#ifndef TRACE_TASK_ENTER_WAITEVENT
extern void traceTask();
#define TRACE_TASK_ENTER_WAITEVENT(t) traceTask((t),'W')
#endif
#ifndef TRACE_TASK_EXIT_WAITEVENT
extern void traceTask();
#define TRACE_TASK_EXIT_WAITEVENT(t) traceTask((t),'E')
#endif
#else
#ifndef TRACE_TASK_STARTED
#define TRACE_TASK_STARTED(t)
#endif
#ifndef TRACE_TASK_TERMINATED
#define TRACE_TASK_TERMINATED(t)
#endif
#ifndef TRACE_TASK_ENTER_WAITEVENT
#define TRACE_TASK_ENTER_WAITEVENT(t)
#endif
#ifndef TRACE_TASK_EXIT_WAITEVENT
#define TRACE_TASK_EXIT_WAITEVENT(t)
#endif
#endif

#ifndef UNSIGNED_MASK_TYPE
#define UNSIGNED_MASK_TYPE unsigned int
#endif

#ifdef TRACE_ISR
#ifndef TRACE_ISR_ENTER
extern void traceIsr();
#define TRACE_ISR_ENTER(i) traceIsr((i), 'N')
#endif
#ifndef TRACE_ISR_LEAVE
extern void traceIsr();
#define TRACE_ISR_LEAVE(i) traceIsr((i), 'L')
#endif
#else
#ifndef TRACE_ISR_ENTER
#define TRACE_ISR_ENTER(i)
#endif
#ifndef TRACE_ISR_LEAVE
#define TRACE_ISR_LEAVE(i)
#endif
#endif

#ifdef CG_DEBUG_LEVEL1
#ifndef PRINT_STATES_LEVEL1
#define PRINT_STATES_LEVEL1(X) cgReportStates_##X()
#endif
#else
#ifndef PRINT_STATES_LEVEL1
#define PRINT_STATES_LEVEL1(X)
#endif
#endif

#ifdef CG_DEBUG_LEVEL2
#ifndef PRINT_STATES_LEVEL2
#define PRINT_STATES_LEVEL2(X) cgReportStates_##X()
#endif
#else
#ifndef PRINT_STATES_LEVEL2
#define PRINT_STATES_LEVEL2(X)
#endif
#endif

#ifdef USE_TEST_DRIVER
#ifndef CALL_TEST_DRIVER
extern void call_test_driver();
#define CALL_TEST_DRIVER(X1, X2) call_test_driver((X1), (X2))
#endif
#endif

#ifdef GBA_STATES
#ifndef GBA
#define GBA
#endif
#ifndef GBA_COLOR_STATE
#define GBA_COLOR_STATE(x) (x)
#endif
#ifndef GBA_PAINT_STATE
#define GBA_PAINT_STATE(state, mask, entID, insNum) {(state) |= (mask); gba_curr_entity_id=entID; gba_curr_inst_num=insNum; GBA_PAINT();}
#endif
#ifndef GBA_SET_STATE_OFF
#define GBA_SET_STATE_OFF(state, mask) (state) &= ~(mask)
#endif
#ifndef GBA_SET_BIT_OFF
#define GBA_SET_BIT_OFF(cond, state, mask) if ((cond)) (state) &= ~(mask)
#endif
#ifndef GBA_PAINT_BOX_ON
#define GBA_PAINT_BOX_ON(state, value, entID, insNum) {(state) |= (value); gba_curr_entity_id=entID; gba_curr_inst_num=insNum; gba_paint();}
#endif
#ifndef GBA_PAINT_BOX_OFF
#define GBA_PAINT_BOX_OFF(state, value) (state) &= ~(value)
#endif
#else
#ifndef GBA_COLOR_STATE
#define GBA_COLOR_STATE(x)
#endif
#ifndef GBA_PAINT_STATE
#define GBA_PAINT_STATE(state, mask, entID, insNum)
#endif
#ifndef GBA_SET_STATE_OFF
#define GBA_SET_STATE_OFF(state, mask)
#endif
#ifndef GBA_SET_BIT_OFF
#define GBA_SET_BIT_OFF(cond, state, mask)
#endif
#ifndef GBA_PAINT_BOX_ON
#define GBA_PAINT_BOX_ON(state, value, entID, insNum)
#endif
#ifndef GBA_PAINT_BOX_OFF
#define GBA_PAINT_BOX_OFF(state, value)
#endif
#endif

#ifdef GBA_ACTIVITIES
#ifndef GBA
#define GBA
#endif
#ifndef GBA_PAINT_TASK_READY
#define GBA_PAINT_TASK_READY(act, low, high, entID, insNum) \
  {(act) &= ~(low); if(!((act) & (high))) { gba_curr_entity_id=entID; gba_curr_inst_num=insNum; gba_paint();}}
#endif
#ifndef GBA_PAINT_TASK_SUSPENDED
#define GBA_PAINT_TASK_SUSPENDED(act, low, high, entID, insNum) \
  {(act) |= (low); gba_curr_entity_id=entID; gba_curr_inst_num=insNum; gba_paint();}
#endif
#ifndef GBA_PAINT_TASK_RUNNING
#define GBA_PAINT_TASK_RUNNING(act, low, high, entID, insNum) \
  {(act) &= ~(high); if (!((act) & (low))) { gba_curr_entity_id=entID; gba_curr_inst_num=insNum; gba_paint();}}
#endif
#ifndef GBA_PAINT_TASK_OUT
#define GBA_PAINT_TASK_OUT(act, low, high, entID, insNum) \
  { (act) |= ((low) | (high)); gba_curr_entity_id=entID; gba_curr_inst_num=insNum; gba_paint();}
#endif
#ifndef GBA_PAINT_ACTIVITY_ON
#define GBA_PAINT_ACTIVITY_ON(act, low, high, entID, insNum) \
  {(act) = ((act) & ~(low)) | (high); gba_curr_entity_id=entID; gba_curr_inst_num=insNum; gba_paint();}
#endif
#ifndef GBA_PAINT_ACTIVITY_OFF
#define GBA_PAINT_ACTIVITY_OFF(act, low, high, entID, insNum) \
  {(act) |= ((low) | (high)); gba_curr_entity_id=entID; gba_curr_inst_num=insNum; gba_paint();}
#endif
#else
#ifndef GBA_PAINT_TASK_READY
#define GBA_PAINT_TASK_READY(act, low, high, entID, insNum)
#endif
#ifndef GBA_PAINT_TASK_SUSPENDED
#define GBA_PAINT_TASK_SUSPENDED(act, low, high, entID, insNum)
#endif
#ifndef GBA_PAINT_TASK_RUNNING
#define GBA_PAINT_TASK_RUNNING(act, low, high, entID, insNum)
#endif
#ifndef GBA_PAINT_TASK_OUT
#define GBA_PAINT_TASK_OUT(act, low, high, entID, insNum)
#endif
#ifndef GBA_PAINT_ACTIVITY_ON
#define GBA_PAINT_ACTIVITY_ON(act, low, high, entID, insNum)
#endif
#ifndef GBA_PAINT_ACTIVITY_OFF
#define GBA_PAINT_ACTIVITY_OFF(act, low, high, entID, insNum)
#endif
#endif

#ifdef GBA
extern void gba_paint();
extern void gba_initialize();
extern void send_cmd_buffer();

#ifndef GBA_PAINT
#define GBA_PAINT() gba_paint()
#endif

#endif

#ifndef DEFAULT_QUEUE_SIZE
#define DEFAULT_QUEUE_SIZE 512
#endif

#define inState(cS, X)                 (((cS) & FM_##X) == FS_##X)
#define inLeafState(cS, X, stInfoType) ((cS) == (stInfoType)FS_##X)
#define isInState(sI, X)               (((sI) & FM_##X) == FS_##X)
#define isNotInState(sI, X)            (((sI) & FM_##X) != FS_##X)
#define isNotStayingInState(sS, X)     isNotInState((sS), X)
#define willBeInState(eOrEBits, X)     isInState((eOrEBits), X)
#define willBeInSibState(eOrEBits, X)  ((eOrEBits) & FM2_##X)
#define wasInState(eOrEBits, X)        isInState((eOrEBits), X)
#define wasInSibState(eOrEBits, X)     ((eOrEBits) & FM2_##X)

#define GET_COND_IN_ARR(T, B, I, Ws)  (((B)[(I)/(Ws)] & (T)(1 << ((I)%(Ws)))) != 0)
#define SET_COND_IN_ARR(T, B, I, Ws)  ((B)[(I)/(Ws)] |= (T)(1 << ((I)%(Ws))))
#define RESET_COND_IN_ARR(T, B, I, Ws)  ((B)[(I)/(Ws)] &= ~(T)(1 << ((I)%(Ws))))

#define GET_BIT_IN_BITARRAY(T, B, I)    (((B) &   (T)(1 << (I))) != 0)
#define SET_BIT_IN_BITARRAY(T, B, I)    ((B) |=  (T)(1 << (I)))
#define RESET_BIT_IN_BITARRAY(T, B, I)  ((B) &= ~(T)(1 << (I)))

#define GET_BUFF_OF_BIT(T, Buff, I, Ws)  ((T)&(((Buff)[(I)/(Ws)])))

#define MAKE_TRUE(X)              MAKE_TRUE_##X
#define MAKE_FALSE(X)             MAKE_FALSE_##X
#define SET_BA_BIT(X)             SET_BA_BIT_##X
#define RESET_BA_BIT(X)           RESET_BA_BIT_##X
#define LOGICAL_CAST(C)           ((C) ? 1 :0)
#define IS_TRUE(X)                IS_TRUE_##X
#define IS_FALSE(X)               IS_FALSE_##X
#define GENERATE_EVENT(X)         GENERATE_EVENT_##X
#define GEN_IN_CURRENT(X)         GEN_IN_CURRENT_##X
#define GEN_IN_BUFF(X, BUFF)      GEN_IN_BUFF_##X( (BUFF) )
#define INSTALL_TIMEOUT(X, D, C)  INSTALL_TM_##X((D), (C))
#define INSTALL_ALARM(A, X)       INSTALL_ALARM_##A( (X) )
#define start_activity(X)         start_activity_##X
#define resume_activity(X)        resume_activity_##X
#define stop_activity(X)          stop_activity_##X
#define abort_activity(X)         abort_activity_##X
#define suspend_activity(X)       suspend_activity_##X
#define is_active(X)              is_active_##X
#define read_data(X)              read_data_##X
#define write_data(X)             write_data_##X
#define HISTORY_CLEAR(X)          HISTORY_CLEAR_##X
#define RESET_HISTORY(X)          RESET_HISTORY_##X
#define DEEP_CLEAR(X)             DEEP_CLEAR_##X
#define RESET_DEEP_HISTORY(X)     RESET_DEEP_HISTORY_##X
#define ENTERED(X)		          ENTERED_##X
#define EXITED(X)		          EXITED_##X
#define STARTED(X)		          STARTED_##X
#define STOPPED(X)		          STOPPED_##X
#define CHANGED(X)		          CHANGED_##X
#define BECAME_TRUE(X)	          BECAME_TRUE_##X
#define BECAME_FALSE(X)	          BECAME_FALSE_##X
#define READ(X)			          READ_##X
#define WRITTEN(X)		          WRITTEN_##X
#define MUX(W1, W2, B)	          ((B)==0?(W1):(W2))
#endif

