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
/*
 *
 * Source:       comh.c
 * Revision:     1.26.1.6.4
 * last Editor:  BERODRIG
 * Date:         14-JUL-2015 17:10:31
 *
 */
/******************************************************************************/
/*   MODULE: COMH (COMmunication Handler)                                     */
/*   FILE:  Module Header File                                                */
/******************************************************************************/
/* This module contains the communication between the CAN network and the PAM */
/* application logic                                                          */
/******************************************************************************/



/******************************************************************************/
/*              Include of common and projekt definitions header              */
/******************************************************************************/
#include "dstdbool.h"
#include "dstdint.h"
#include "dstring.h"
#include "dassert.h"
#include "tmp_pdf.h"
#include "comh_typ.h"
#include "platform_types.h"
#include "timr.h"
#include "PLATFORM_SharedVar.h"

#ifdef XDAPM_PROD_P4U
#define COMH_PROD_P4U
#endif


/******************************************************************************/
/*                      Include external modul header                         */
/******************************************************************************/
/* DAPM */

#include "conf.h"

#include "p2gpaleo.h"
#include "didh_typ.h"
#include "bda.h"
#include "brkh_cus.h"
#include "stmh_cus.h"

/******************************************************************************/
/*                      Include internal modul header                         */
/******************************************************************************/
#include "comh.h"
#include "E2E_platfrom.h"
#include "FrWR.h"

#define DAIMLER_BR_213_PLATFORM
/******************************************************************************/
/*                   Definition of local module constants                     */
/******************************************************************************/
/*Brake pressed Pressure threshold*/
#define BRAKE_PRESSED_PRESURE_THRESHOLD_PRESSED        40
/*Brake released Pressure threshold*/
#define BRAKE_PRESSED_PRESURE_THRESHOLD_RELEASED       10
#define LONGITUDE_OFFSET -33554432
#define LATITUDE_OFFSET  -16777216
/******************************************************************************/
/*                     Definition of local module macros                      */
/******************************************************************************/
/* Gets the lowbyte of the given variable x. */
#define GetLowByte(x)      ((u8) (((u8)(x)) & 0xff))
/* Gets the highbyte of a 16bit variable x. x has to be a 16bit-variable ! */
#define GetHighByte16(x)   ((u8) (((u16)(x)) >> 8))

#if !defined(APPL_IS_PLA_SW) && !defined(APPL_CAN_DONT_SEND_DUMMY_PLA_STATUS)
#  ifndef APPL_CAN_SEND_DUMMY_PLA_STATUS
#    define APPL_CAN_SEND_DUMMY_PLA_STATUS
#  endif
#endif

#define COMH_WHEEL_IMP_MAX_COUNT              (u16)(APPL_MAX_PATHCNT_VALUE)

/** Number of wheeltick that are allowed after standstill was detected. If the
 *  wheel turns this value or further the standstill-state is deleted. */
#define COMH_MAX_WHEELTICKS_AT_STANDSTILL 3


/* Definitions for Send-Cycle-Handling of PLA_01-Message */
#define COMH_TIMER_PLA_01_LONG_20MS             50 /* 50*20ms = 1000ms */
#define COMH_TIMER_PLA_01_SHORT_LONG            5  /* 5*20ms = 100ms */

#define COMH_BRAKE_MOMENT_MAX_VALUE_NM          (32760) /* Max value 1bit=1Nm */
#define COMH_BRAKE_MOMENT_ERR_RAW_DATA          (0x1FFF) /* error value = 8191dez */
#define COMH_CHECKSUM_ENDVALUE_PLA_01           (u16)(0x0031)

#define COMH_BACK_BUTTON_SECONDS                1
#define COMH_CYCLIC_TASK_MS                     20
#define COMH_BACK_BUTTON_TIMER_VALUE            ((COMH_BACK_BUTTON_SECONDS*1000)/COMH_CYCLIC_TASK_MS)
#define BUTTON_PRESSED_TIME_AFTER_RELEASE       40
#define BUTTON_PRESSED_CYCLES_COUNT                     (BUTTON_PRESSED_TIME_AFTER_RELEASE / COMH_CYCLIC_TASK_MS)
#define BDA_OPTIONS_COUNT 2
#define HZTR_OPTIONS_COUNT          2
#ifdef XECU_VARIANT_SVS
#define VIEWS_OPTIONS_COUNT         5
#else
#define VIEWS_OPTIONS_COUNT         3
#endif
#define P4U_SLOTS_OPTIONS_COUNT     8
#define MANEUVER_OPTIONS_COUNT      2

#define PARK_REMAIN_DIST_TX_SIGNAL_VALUE_MIN          (-300)    /*  -30 [cm] * 10 */
#define PARK_REMAIN_DIST_TX_SIGNAL_VALUE_MAX        (20150u)    /* 2015 [cm] * 10 */
#define PARK_VEHSPED_RQ_TX_SIGNAL_VALUE_MAX          (1000u)    /* 10 [km/h] * 100 */

/* New Defines for Flex */
#define NUM_OF_RX_PDUS 20   /* To be changed if Rx PDU is added !!*/
#define NUM_OF_TX_PDUS 8   /* To be changed if Rx PDU is added !!*/

/******************************************************************************/
/*              Definition of CAN message ID's for Daimler BR213              */
/******************************************************************************/
#ifdef DAIMLER_BR_213_PLATFORM

   #define WHEEL_CIRCUMF_FRONT configurations[0].das_cfg.dapm_cfg.vehicle_cfg.default_wheel_circ_front_mm
   #define WHEEL_CIRCUMF_REAR configurations[0].das_cfg.dapm_cfg.vehicle_cfg.default_wheel_circ_rear_mm

   #ifdef XAPPL_LOT_CNTRL_SUPP
      #define BRK_PEDAL_PRSD_THRSHLD           3000u /* physical value, corresponds to 1000 raw value for brake torque */
   #else
      #define BRK_PEDAL_PRSD_THRSHLD           8000u /* physical value, corresponds to about 2667 raw value for brake torque */
   #endif
   #define BRK_PEDAL_N_PRSD_THRSHLD        1000u /* physical value, corresponds to about 333 raw value for brake torque */
#endif

enum COMH_options_button_modules_E
{
    COMH_UNKNOW_MODULE,
    COMH_HZTR,
    COMH_VIEWS,
    COMH_BDA,
    COMH_SLOT_SELLECTION,
    COMH_MANEUVER_SELLECTION,
    COMH_FUNCBAR
};
/******************************************************************************/
/*                     Definition of local module types                       */
/******************************************************************************/

/* New Typedefs for flex */

typedef void (*encode_signals_p2f) (uint8 *);
typedef void (*tx_pdu_function_p2f) (PduInfoType *);

typedef struct
{
    uint8 length;
    tx_pdu_function_p2f  tx_function;
}tx_pdus_flex_S;

typedef struct
{
    uint8 is_updated;
    uint8 *buffer;
    uint8 *buffer_copy;
    uint8 length;
    encode_signals_p2f  encode_function;
}rx_pdus_flex_S;

typedef struct
{
    uint8 fr_id;
    uint8 array_id;
}rx_id_map_S;


struct lcomh_can_buffer_S
{
    u16 id;
    u8  p[8];
    u8  n;
};

struct lcomh_wheel_impulses_S
{
    /**
     * Timestamp from the moment, when wheel impulses have been received.
     */
    u32 timestamp_2us;
    u32 timestamp_2us_left; /* used for Daimler BR213 */
    u32 timestamp_2us_right; /* used for Daimler BR213 */
    /**
     * Absolute number of impulses from wheel front left.
     */
    u16 wheel_impulses_fl;
    /**
     * Absolute number of impulses from wheel front right.
     */
    u16 wheel_impulses_fr;
    /**
     * Absolute number of impulses from wheel rear left.
     */
    u16 wheel_impulses_rl;
    /**
     * Absolute number of impulses from wheel rear right.
     */
    u16 wheel_impulses_rr;

    u8 msg_count;
    /**
     * TRUE = data is valid
     * FALSE = data is invalid and shouldn't be used
     */
    bool_T valid_fl;
    bool_T valid_fr;
    bool_T valid_rl;
    bool_T valid_rr;
};

struct wheel_speeds_S
{
    u32  timestamp_2us;
    u32 timestamp_2us_left; /* used for Daimler BR213 */
    u32 timestamp_2us_right; /* used for Daimler BR213 */
    si16 wheel_speed_fl; /* RR3_Rad_kmh_VL (15bits startbit 33 0.01km/h */
    si16 wheel_speed_fr; /* RR3_Rad_kmh_VR (15bits startbit 33 0.01km/h */
    si16 wheel_speed_rl; /* RR3_Rad_kmh_HL (15bits startbit 33 0.01km/h */
    si16 wheel_speed_rr; /* RR3_Rad_kmh_HR (15bits startbit 33 0.01km/h */
};


struct lcomh_data_S
{
    ble_msg_T ble_msg;
    bool_T is_seatbelt_put;
    bool_T is_seatbelt_removed;
  /* mBremse_1 */
    u8                              abs_intervention_raw_data;      /* ABS */
    u16                             wheel_speed_raw_data;           /* Used for Daimler BR213, corresponds to VehSpeed_X. Radgeschwindigkeit */
    u8                              asr_request_raw_data;           /* ASR request */
    u8                              msr_request_raw_data;           /* MSR request */
    u8                              eds_intervention_raw_data;      /* EDS intervention */
    u8                              esp_asr_passive_raw_data;       /* ESP ASR passive */
    u8                              esp_intervention_raw_data;      /* ESP Intervention brake intervention for Daimler BR213*/
    u8                              CWA_intervention_raw_data;      /* CWA Intervention brake intervention for Daimler BR213*/
    u8                              EPKB_intervention_raw_data;     /* RPKB brake intervention for Daimler BR213 */
    u8                              AS_intervention_raw_data;       /* AS brake intervention for Daimler BR213 */
    u8                              wheel_speed_backup_value_active;/* Wheel Speed Backup Value */
    u8                              esp_system_status_raw_data;     /* ESP System Status Raw Data */

  /* mBremse_2 */
    u8                              impulsewheel_number_of_teeth;   /* number of teeth of impulsewheel */
    si16                            lateral_acceleration;           /* used for Daimler BR213, corresponds to VehAccel_Lateral */

  /* mBremse_3  */

  /* mBremse_5 */
    u8                              yaw_speed_sign_raw_data;
    u16                             yaw_speed_raw_data;
    u32                             yaw_speed_timestamp_2us;

    si16                            brake_pressure_raw_data;      /* Used in Daimler BR213, corresponds to BrkTrq_V2 */
    si16                            driver_brake_pressure_raw_data;      /* Used in Daimler BR213, corresponds to BrkTrq_V2 */
    bool_T                          brake_is_standstill;            /* holds the info if the brake ecu detects a vehicle standstill */

  /* mBremse_8 */
    si16                            long_acc_phys_data;             /* Used in Daimler BR213, long.-acc. Resolution: 1Bit = 0.03125 m/(s^2) */
    bool_T                          long_acc_qualibit_error;
    bool_T                          long_acc_qualibit;
    bool_T                          long_acc_error;
    u8                              long_acc_qb_err_count;
    u8                              long_acc_qb_count;
    u8                              long_acc_err_count;
    u8                              long_acc_timeout_count;
    u8                              long_acc_offset_phys_data; /* Used in Daimler BR213 */

  /* mBremse_9 */
    u8                              brake_ecu_state;                /* used in Daimler BR213, holds the actual state of ESP (brake) */
    u8                              brake_ecu_abort_reason;         /* holds the abort reason of the brake ecu */


  /* mBremse_10 */
    struct lcomh_wheel_impulses_S   wheel_impulses;                 /* wheel impulses received - used for Daimler BR213*/
    struct wheel_speeds_S           wheel_speeds;
    enum COMH_wheel_info_E          wheel_info_fl_raw_data;         /* Fahrtrichtugnserkennung Front Left  - used for Daimler BR213, indicates wheel direction or standstill*/
    enum COMH_wheel_info_E          wheel_info_fr_raw_data;         /* Fahrtrichtugnserkennung Front Right - used for Daimler BR213, indicates wheel direction or standstill */
    enum COMH_wheel_info_E          wheel_info_rr_raw_data;         /* Fahrtrichtugnserkennung Rear  Right  - used for Daimler BR213, indicates wheel direction or standstill */
    enum COMH_wheel_info_E          wheel_info_rl_raw_data;         /* Fahrtrichtugnserkennung Rear  Left  - used for Daimler BR213, indicates wheel direction or standstill  */
    bool_T                          is_vehicle_standstill;

  /* mBremse_11 */
    bool_T                          brake_is_hydr_hold_active;

  /* mGetriebe_11 */
    u8                              is_gear; /* Used in Daimler BR213, corresponds to TSL_Posn_target, enum values to be handled in getter */

  /* mGate_Komf_1 */
    u8                              trailer_present_raw_data;
    u8                              blinker_left_raw_data;
    u8                              blinker_right_raw_data;
    bool_T                          reverse_gear_raw_data;
    bool_T                          is_door_open_rear_left;
    bool_T                          is_door_open_rear_right;
    bool_T                          is_door_open_front_left;
    bool_T                          is_door_open_front_right;

  /* mLenkhilfe_2 */
    u8                              status_eps_raw_data;    /* Used in Daimler BR213, set to PARK_Stat_EPS raw value */
    u32                             status_eps_timestamp;
    u8                              pla_termination_raw_data;  /* used in Daimler BE213, set to PARK_Cancel_stat raw value */

  /* mLenkhilfe_3 */
    si16                            steering_angle;
    u16                             steering_angle_raw_data; /* Used in Daimler BR213, set to StWhl_Angl raw value */
    u8                              steering_angle_offset_raw_data; /* Used in Daimler BR213 */
    u8                              steering_angle_sign_raw_data; /* Used in Daimler BR213, set according to offset of steering angle raw value */
    u8                              steering_moment_sign_raw_data; /* Used in Daimler BR213, set to StW_Trq_EPS */
    u16                             steering_moment_raw_data; /* Used in Daimler BR213, set according to offset of steering torque raw value */
    u32                             st_wheel_angle_time_2us;

  /* mKombi_2 */
    si8                             temp_filtered_raw_data;         /* outside temperature */
    bool_T                          temperature_received;

  /* other */
    bool_T                          upa_button;
    bool_T                          park4u_button;
    bool_T                          switch_options_button;
    bool_T                          GPS_button;
    bool_T                          confirmation_button;
    bool_T                          view_button;
    bool_T                          p4uhome_training_button;
    bool_T                          abort_button;
    bool_T                          bda_replay_button;
    bool_T                          dead_man_button;
    u16                             wheel_circumference_raw_data;
    enum COMH_gear_lever_position_E automatic_transmission_state;
    bool_T                          gear_change_ready;
    bool_T                          sliding_door_open_left;
    bool_T                          sliding_door_open_right;

    u32                             ext_veh_position_timestamp_2us;

    u16                             ext_veh_angle_raw_data;
    u16                             ext_veh_x_pos_raw_data;
    u16                             ext_veh_y_pos_raw_data;

    u8 status_eps_moment_raw_data;
    si32                            gps_longitude;
    si32                            gps_latitude;

    /* Data added for Daimler BR213 */
    esp_brake_state_E         esp_brake_state_park; /* Used in COMH_GetESPBrakeState */
    u8                        engine_state;
    enu_break_pedal           brake_pedal_stat; /* directly set to APPLIED or RELEASED */
    u8                        ESP_Oprtn_Stat_AS_V2; /* used for GetESPIntervention, 0 & 8 return intervention active */
    esp_system_state_E        esp_system_state;
    bool_T                    epb_engaged;   /* directly set to true or false according to PkBrkSw_Stat */
    gear_position_E           current_gear; /* Set to equivalent enum value of Gr signal in Daimler BR213 */
    u8                        park_enbl_stat_esp; /* Set to value received, to return true/ false in getter */
    park_flt_stat_esp_E       park_flt_stat_esp; /* set enum value according to received input */
    ldc_afterrun_ctrl_E       esp_ldc_afterrun_ctrl; /* set enum values according to recevied input */
    bool_T                    ignition_on; /* set to true if ISw_Stat is 4 (ign on) */

};
struct button_properties_S
{
    bool_T pre_state;
    enum button_state_E *button_state;
    bool_T button_signal;
    bool_T is_toggle_button;
    u8 pressed_cycle_counter;
};
/******************************************************************************/
/*                   Definition of local module variables                     */
/******************************************************************************/

/**
 * buffer for received CAN data
 * this buffer saves data that is received from CAN
 */
static struct lcomh_data_S      st_comh_buffer_data;

/**
 *  flag that indicates, if copy from st_comh_buffer_data to st_comh_buffer_appl_data
 *  is in progress. If yes, all data must be saved in the irpt buffer and should be
 *  copied to can data afterwards
 */
static bool_T                       st_comh_copy_in_can_irpt;

// UNUSED static u8                           st_comh_p2_steering_msg_cnt;   /* counter for the CAN message */

/* possibility to test external warning direction */
volatile enum CAPP_dr_dir_E         st_comh_ext_warning_direction;

#if 0 // NOT FOR DAIMLER
#ifdef XAPPL_BRAKE_OPEN_ROAD
#define COMH_OPEN_ROAD_EM_BR_TONE 3000
/* Counter to give out a long continuous tone if an emergency brake happened */
static u8 open_road_em_br_tone = 0;
#endif
#endif // NOT FOR DAIMLER

static u8                           st_comh_p2_steering_msg_cnt;   /* counter for the CAN message */
static u8                           st_comh_timer_pla_01_long;
static u8                           st_comh_timer_pla_01_short_long;

static bool_T                       send_paravan = FALSE;
static u8                           paravan_count = 0;

static  enum button_state_E   upa_button_state = COMH_BUTTON_RELEASED;
static  enum button_state_E   park4u_button_state = COMH_BUTTON_RELEASED;
static  enum button_state_E   confirmation_button_state = COMH_BUTTON_RELEASED;
static  enum button_state_E   gps_button_state = COMH_BUTTON_RELEASED;
static  enum button_state_E   view_button_state = COMH_BUTTON_RELEASED;
static  enum button_state_E   p4uHomeTraining_button_state = COMH_BUTTON_RELEASED;
static  enum button_state_E   bdaReplay_button_state = COMH_BUTTON_RELEASED;
static  enum button_state_E   abort_button_state = COMH_BUTTON_RELEASED;
static  enum button_state_E   switch_button_state;
static enum COMH_options_button_modules_E module_using_options_button = COMH_UNKNOW_MODULE;
static  u8 switch_options_button_click_count = 0;
static  u8                         switch_back_button_click_count = 0;
static enum button_state_E    funcbar_back_button_state;
static u8                          funcbar_options_count = 0;
static bool_T                      set_flag = FALSE;
/******************************************************************************/
/*                 Definition of local module constant data                   */
/******************************************************************************/
E2E_P02SenderStateType brk_rq_park_Pr2_sender_state = {0} ;
E2E_P02SenderStateType rq_park_pr2_sender_state= {0};
E2E_P02SenderStateType parkcfg_stat_pr2_sender_state = {0};
E2E_P02SenderStateType park_disp_rq_ar2_sender_state = {0};
E2E_P02SenderStateType rempark_rq_pr2_sender_state = {0};

/******************************************************************************/
/*                 Definition of externed configurations struct               */
/******************************************************************************/
extern const struct APPL_codingdata_S configurations[];

/******************************************************************************/
/*                     Definition of exported variables                       */
/******************************************************************************/

/******************************************************************************/
/*                   Definition of exported constant data                     */
/******************************************************************************/

/******************************************************************************/
/*                   Declaration of local module function prototyps           */
/******************************************************************************/

/* New functions for flex */

static void encode_HMI_CHASSIS_R1_AR2_pdu (uint8 *buffer);
static void encode_PtMngmnt_Stat_AR2_pdu (uint8 *buffer);
static void encode_EPKB_Stat_AR2_pdu (uint8 *buffer);
static void encode_CTRL_C_Stat1_AR_pdu (uint8 *buffer);
static void encode_SVS_Rq_HU_AR2_pdu (uint8 *buffer);
static void encode_Config_HU4_AR2_pdu (uint8 *buffer);
static void encode_Ign_Veh_Stat_AR2_pdu (uint8 *buffer);
static void encode_TCM_Data2_pdu (uint8 *buffer);
static void encode_SBW_Stat_pdu (uint8 *buffer);
static void encode_PARK_St_Rs_AR2_pdu (uint8 *buffer);
static void encode_STA_Rs1_AR2_pdu (uint8 *buffer);
static void encode_VehDyn_Stat1_AR2_pdu (uint8 *buffer);
static void encode_Whl_Rt_Stat_AR2_pdu (uint8 *buffer);
static void encode_VehSpd_X_AR2_pdu (uint8 *buffer);
static void encode_EL_TurnInd_Rq_AR2_pdu (uint8 *buffer);
static void encode_BrkSys_Stat_AR2_pdu (uint8 *buffer);
static void encode_PARK_Brk_Rs_AR2_pdu (uint8 *buffer);
static void encode_Whl_Lt_Stat_AR2_pdu (uint8 *buffer);
static void encode_VehDyn_Stat2_AR2_pdu (uint8 *buffer);
static void encode_BC_F_Stat2_AR2_buffer_pdu (uint8 *buffer);


static void encode_updated_rx_pdus(void);
static void transmit_tx_pdus(void);

static void send_ST_Rq_PARK_AR2_pdu(PduInfoType *PduInfoPtr);
static void send_Brk_Rq_PARK_AR2_pdu(PduInfoType *PduInfoPtr);
static void send_Park_stat_AR2_pdu(PduInfoType *PduInfoPtr);
static void send_RVC_SVS_Park_stat_AR2_pdu(PduInfoType *PduInfoPtr);
static void send_Park_Disp_Rq_AR2_pdu(PduInfoType *PduInfoPtr);
static void send_RemPark_Rq_AR2_pdu(PduInfoType *PduInfoPtr);
static void send_Parkwarn_AR2_pdu(PduInfoType *PduInfoPtr);
static void send_Park_set_stat_AR2_pdu(PduInfoType *PduInfoPtr);

static uint8 get_mapped_rx_id (PduIdType id);
static uint8 get_mapped_tx_id (PduIdType id);
static uint8 get_mapped_tx_id_reverse (PduIdType id);

static void MemCpy_Ram2Ram( const uint8 *src, uint8 *dest, uint8 cnt );


static u16 Conv100thDeg2U16f(const si16 angle_100th_deg);

static void COMH_SendParavanInfo(void);

static void CalculateVehicleStandstill(void);

static u8 calc_crc (u8 *buff, u8 start, u8 end);

static u8 ComputeSetPkm(struct COMH_input_S *input);
static u16 GetHaptTorq12bits(struct COMH_input_S *input, u8 pkm_state);

static void GetBrakePressureAutopark(void);
static u16 CalcDistanceToStop(si16 Hint, u16 Collide);
static void EvaluateButtonsState(void);
static void EvaluateUpaButtonState(void);
static void EvaluatePark4uButtonState(void);
static void EvaluateConfirmationButtonState(void);
static void EvaluateGpsButtonState(void);
static void EvaluateP4uHomeTrainingButtonState(void);
static void EvaluateAbortButtonState(void);
static void EvaluateBdaReplayButtonState(void);
static void EvaluateSwitchButtonState(void);
static void EvaluateViewButtonState(void);
static void EvaluateBackButtonState(void);
static void SwitchOptionsButtonCounter(void);
static u8 Calculate_Module_Option(enum COMH_options_button_modules_E current_module,u8 max_options_number);
static void EvaluateButtonState(struct button_properties_S* button_properties);
/******************************************************************************/
/*                   Definition of local module functions                     */
/******************************************************************************/

/* Buffer for every RX PDU to save the content (in interrupt context) */

uint8 HMI_CHASSIS_R1_AR2_buffer[8];
uint8 PtMngmnt_Stat_AR2_buffer[8];
uint8 EPKB_Stat_AR2_buffer[8];
uint8 CTRL_C_Stat1_AR_buffer[8];
uint8 SVS_Rq_HU_AR2_buffer[8];
uint8 Config_HU4_AR2_buffer [8];
uint8 Ign_Veh_Stat_AR2_buffer [8];
uint8 TCM_Data2_buffer [8];
uint8 SBW_Stat_buffer [8];
uint8 PARK_St_Rs_AR2_buffer [8];
uint8 STA_Rs1_AR2_buffer [8];
uint8 VehDyn_Stat1_AR2_buffer [8];
uint8 Whl_Rt_Stat_AR2_buffer [8];
uint8 VehSpd_X_AR2_buffer [8];
uint8 EL_TurnInd_Rq_AR2_buffer [4];
uint8 BrkSys_Stat_AR2_buffer [8];
uint8 PARK_Brk_Rs_AR2_buffer [8];
uint8 Whl_Lt_Stat_AR2_buffer [8];
uint8 VehDyn_Stat2_AR2_buffer[8];
uint8 BC_F_Stat2_AR2_buffer[8];

/* Buffer for every RX PDU which holds a copy of the data to be processed (in TASK context) */

uint8 HMI_CHASSIS_R1_AR2_buffer_copy[8];
uint8 PtMngmnt_Stat_AR2_buffer_copy[8];
uint8 EPKB_Stat_AR2_buffer_copy[8];
uint8 CTRL_C_Stat1_AR_buffer_copy[8];
uint8 SVS_Rq_HU_AR2_buffer_copy[8];
uint8 Config_HU4_AR2_buffer_copy[8];
uint8 Ign_Veh_Stat_AR2_buffer_copy[8];
uint8 TCM_Data2_buffer_copy[8];
uint8 SBW_Stat_buffer_copy[8];
uint8 PARK_St_Rs_AR2_buffer_copy[8];
uint8 STA_Rs1_AR2_buffer_copy[8];
uint8 VehDyn_Stat1_AR2_buffer_copy[8];
uint8 Whl_Rt_Stat_AR2_buffer_copy[8];
uint8 VehSpd_X_AR2_buffer_copy[8];
uint8 EL_TurnInd_Rq_AR2_buffer_copy[4];
uint8 BrkSys_Stat_AR2_buffer_copy[8];
uint8 PARK_Brk_Rs_AR2_buffer_copy[8];
uint8 Whl_Lt_Stat_AR2_buffer_copy[8];
uint8 VehDyn_Stat2_AR2_buffer_copy[8];
uint8 BC_F_Stat2_AR2_buffer_copy[8];

/* Array that holds all the details of the RX Flex PDUs */

rx_pdus_flex_S rx_pdus_array [NUM_OF_RX_PDUS] =
{
    { /* HMI_CHASSIS_R1_AR2 pdu */
        0,  /* is_updated is initially 0 */
        HMI_CHASSIS_R1_AR2_buffer, /* pdu buffer array */
        HMI_CHASSIS_R1_AR2_buffer_copy, /* buffer to copy the content without critical section */
        8, /* Length of the pdu */
        encode_HMI_CHASSIS_R1_AR2_pdu /* pointer to function for encoding the pdu for the signals */
    },
    { /* PtMngmnt_Stat_AR2 pdu */
        0,
        PtMngmnt_Stat_AR2_buffer,
        PtMngmnt_Stat_AR2_buffer_copy,
        8,
        encode_PtMngmnt_Stat_AR2_pdu
    },
    { /* EPKB_Stat_AR2 pdu */
        0,
        EPKB_Stat_AR2_buffer,
        EPKB_Stat_AR2_buffer_copy,
        8,
        encode_EPKB_Stat_AR2_pdu
    },
    { /* CTRL_C_Stat1_AR pdu */
        0,
        CTRL_C_Stat1_AR_buffer,
        CTRL_C_Stat1_AR_buffer_copy,
        8,
        encode_CTRL_C_Stat1_AR_pdu
    },
    { /* SVS_Rq_HU_AR2 pdu */
        0,
        SVS_Rq_HU_AR2_buffer,
        SVS_Rq_HU_AR2_buffer_copy,
        8,
        encode_SVS_Rq_HU_AR2_pdu
    },
    { /* Config_HU4_AR2 pdu */
        0,
        Config_HU4_AR2_buffer,
        Config_HU4_AR2_buffer_copy,
        8,
        encode_Config_HU4_AR2_pdu
    },
    { /* Ign_Veh_Stat_AR2 pdu */
        0,
        Ign_Veh_Stat_AR2_buffer,
        Ign_Veh_Stat_AR2_buffer_copy,
        8,
        encode_Ign_Veh_Stat_AR2_pdu
    },
    { /* TCM_Data2 pdu */
        0,
        TCM_Data2_buffer,
        TCM_Data2_buffer_copy,
        8,
        encode_TCM_Data2_pdu
    },
    { /* SBW_Stat pdu */
        0,
        SBW_Stat_buffer,
        SBW_Stat_buffer_copy,
        8,
        encode_SBW_Stat_pdu
    },
    { /* PARK_St_Rs_AR2 pdu */
        0,
        PARK_St_Rs_AR2_buffer,
        PARK_St_Rs_AR2_buffer_copy,
        8,
        encode_PARK_St_Rs_AR2_pdu
    },
    { /* STA_Rs1_AR2 pdu */
        0,
        STA_Rs1_AR2_buffer,
        STA_Rs1_AR2_buffer_copy,
        8,
        encode_STA_Rs1_AR2_pdu
    },
    { /* VehDyn_Stat1_AR2 pdu */
        0,
        VehDyn_Stat1_AR2_buffer,
        VehDyn_Stat1_AR2_buffer_copy,
        8,
        encode_VehDyn_Stat1_AR2_pdu
    },
    { /* Whl_Rt_Stat_AR2 pdu */
        0,
        Whl_Rt_Stat_AR2_buffer,
        Whl_Rt_Stat_AR2_buffer_copy,
        8,
        encode_Whl_Rt_Stat_AR2_pdu
    },
    { /* VehSpd_X_AR2 pdu */
        0,
        VehSpd_X_AR2_buffer,
        VehSpd_X_AR2_buffer_copy,
        8,
        encode_VehSpd_X_AR2_pdu
    },
    { /* EL_TurnInd_Rq_AR2 pdu */
        0,
        EL_TurnInd_Rq_AR2_buffer,
        EL_TurnInd_Rq_AR2_buffer_copy,
        4,
        encode_EL_TurnInd_Rq_AR2_pdu
    },
    { /* BrkSys_Stat_AR2 pdu */
        0,
        BrkSys_Stat_AR2_buffer,
        BrkSys_Stat_AR2_buffer_copy,
        8,
        encode_BrkSys_Stat_AR2_pdu
    },
    { /* PARK_Brk_Rs_AR2 pdu */
        0,
        PARK_Brk_Rs_AR2_buffer,
        PARK_Brk_Rs_AR2_buffer_copy,
        8,
        encode_PARK_Brk_Rs_AR2_pdu
    },
    { /* Whl_Lt_Stat_AR2 pdu */
        0,
        Whl_Lt_Stat_AR2_buffer,
        Whl_Lt_Stat_AR2_buffer_copy,
        8,
        encode_Whl_Lt_Stat_AR2_pdu
    },
    { /* VehDyn_Stat2_AR2 pdu */
        0,
        VehDyn_Stat2_AR2_buffer,
        VehDyn_Stat2_AR2_buffer_copy,
        8,
        encode_VehDyn_Stat2_AR2_pdu
    },
    /*BC_F_Stat2_AR2*/
    {
        0,
        BC_F_Stat2_AR2_buffer,
        BC_F_Stat2_AR2_buffer_copy,
    	8,
    	encode_BC_F_Stat2_AR2_buffer_pdu

    }
};

/* Array that holds all the details of the TX Flex PDUs */

tx_pdus_flex_S tx_pdus_array[NUM_OF_TX_PDUS] =
{
    { /* Park_set_stat_AR2 pdu */
        8,
        send_Park_set_stat_AR2_pdu
    },
    { /* Park_stat_AR2 pdu */
        8,
        send_Park_stat_AR2_pdu
    },
    { /* Park_Disp_Rq_AR2 pdu */
        8,
        send_Park_Disp_Rq_AR2_pdu
    },
    { /* Brk_Rq_PARK_AR2 pdu */
        8,
        send_Brk_Rq_PARK_AR2_pdu
    },
    { /* Parkwarn_AR2 pdu */
        8,
        send_Parkwarn_AR2_pdu
    },
    { /* RemPark_Rq_AR2 pdu */
        8,
        send_RemPark_Rq_AR2_pdu
    },
    { /* RVC_SVS_Park_stat_AR2 pdu */
        8,
        send_RVC_SVS_Park_stat_AR2_pdu
    },
    { /* ST_Rq_PARK_AR2 pdu */
        8,
        send_ST_Rq_PARK_AR2_pdu
    }
};

/* ID mapping for RX PDU flex IDs to array ID */

rx_id_map_S id_map_array[NUM_OF_RX_PDUS] =
{
    {3,0}, /* HMI_CHASSIS_R1_AR2 */
    {5,1}, /* PtMngmnt_Stat_AR2 */
    {6,2}, /* EPKB_Stat_AR2 */
    {13,3}, /* CTRL_C_Stat1_AR */
    {14,4}, /* SVS_Rq_HU_AR2 */
    {33,5}, /* Config_HU4_AR2 */
    {39,6}, /* Ign_Veh_Stat_AR2 */
    {55,7}, /* TCM_Data2_AR2 */
    {56,8}, /* SBW_Stat_AR2 */
    {62,9}, /* PARK_St_Rs_AR2 */
    {63,10}, /* STA_Rs1_AR2 */
    {64,11}, /* VehDyn_Stat1_AR2 */
    {66,12}, /* Whl_Rt_Stat_AR2 */
    {67,13}, /* VehSpd_X_AR2 */
    {17,14}, /* EL_TurnInd_Rq_AR2 */
    {70,15}, /* BrkSys_Stat_AR2 */
    {71,16}, /* PARK_Brk_Rs_AR2 */
    {72,17}, /* Whl_Lt_Stat_AR2 */
    {65,18}, /* VehDyn_Stat2_AR2 */
    {19,19} /*BC_F_Stat2_AR2*/

};

/* ID mapping for TX PDU flex IDs to array ID */

rx_id_map_S tx_id_map_array[NUM_OF_TX_PDUS] =
{
    {31,0}, /* Park_set_stat_AR2 */
    {14,1}, /* Park_set_stat_AR2 */
    {12,2}, /* Park_Disp_Rq_AR2 */
    {28,3}, /* Brk_Rq_PARK_AR2 */
    {11,4}, /* Parkwarn_AR2 */
    {15,5}, /* RemPark_Rq_AR2 */
    {16,6}, /* RVC_SVS_Park_stat_AR2 */
    {29,7}  /* ST_Rq_PARK_AR2 */
};

/******************************************************************************/
/**
 * converts 1/100 deg to format U16
 * \param[in] angle_100th_deg: range -32768 to 32767
 * \return angle in U16 format
 *
 * This function converts 1/100 degrees into U16 format.
 * Due to restrictions of datasize of input value the input value
 * is restricted to -327.68 to +327.67 degree
 */
static u16 Conv100thDeg2U16f(const si16 angle_100th_deg)
{
    u32 ret_value;

    if (angle_100th_deg < 0)
    {
        /* add 360degree; typecast u32 AFTER addition */
        ret_value = (u32)((si32)angle_100th_deg + (si32)36000);
    }
    else
    {
        ret_value = (u32)(angle_100th_deg);
    }

    ret_value *= (u32)4369;                              /* * 65535 */
    (void)MTLI_DivU32(&ret_value, ret_value, (u32)2400); /* / 36000 */

    return (u16)ret_value;
}


/**
 * void COMH_CanReceive (u16 id, const u8 *p, u8 n)
 * Can Receive Interrupt function
 *
 * \param id CAN message ID
 * \param p  pointer to CAN message
 * \param n  lenght of CAN message
 */
void COMH_CanReceive (u16 id, const u8 *p, u8 n)
{

}


/**
 * static void CalculateVehicleStandstill(void)
 *
 * calculates if the vehicle is in standstill. A standstill is defined as not
 * moving for some time (e.g. 400ms). The standstill stays if the car moves
 * less then 3 wheelticks.\n
 * This function must be called every 20ms.
 *
 * \return nothing */

static void CalculateVehicleStandstill(void)
{
  static u16 old_wheel_impulses_rl = 0;
  static u16 standstill_start_tick_value = 0;
  static u8  standstill_time_counter = 0;

  u16 current_wheel_impulses_rl = COMH_GetWheelImpulsesRearLeft();

  if (current_wheel_impulses_rl > APPL_MAX_PATHCNT_VALUE)
  {
    /* invalid value */
    _ASSERT(FALSE);
    old_wheel_impulses_rl = 0;
    standstill_start_tick_value = 0;
    standstill_time_counter = 0;
    st_comh_buffer_data.is_vehicle_standstill = FALSE;
  }
  else
  {
    /* Standstill recognition */
    /* Geschwindigkeit == 0 (Stillstand) ? */
    if (current_wheel_impulses_rl == old_wheel_impulses_rl)
    {
      if (standstill_time_counter > CONF_GetStandstillTimer20ms())
      {
        /* Zeit ist abgelaufen, Fahrzeug steht */
          st_comh_buffer_data.is_vehicle_standstill = TRUE;
          standstill_start_tick_value = current_wheel_impulses_rl;
      }
      else
      {
        standstill_time_counter++;
      }
    }
    else
    {
       /* check if the wheel turned more than COMH_MAX_WHEELTICKS_AT_STANDSTILL
       * since last recognized standstill. */
      if (standstill_start_tick_value > (APPL_MAX_PATHCNT_VALUE - COMH_MAX_WHEELTICKS_AT_STANDSTILL))
      {
        /* overflow */
        u16 standstill_max_tick_value =
          ((COMH_MAX_WHEELTICKS_AT_STANDSTILL + standstill_start_tick_value) - APPL_MAX_PATHCNT_VALUE) - 1;
        if ( (current_wheel_impulses_rl < standstill_start_tick_value) &&
             (current_wheel_impulses_rl >= standstill_max_tick_value) )
        {
          /* no standstill anymore */
          st_comh_buffer_data.is_vehicle_standstill = FALSE;
        }
      }
      else
      {
        /* no overflow */
        u16 standstill_max_tick_value =
          standstill_start_tick_value + COMH_MAX_WHEELTICKS_AT_STANDSTILL;
        if ( (current_wheel_impulses_rl < standstill_start_tick_value) ||
             (current_wheel_impulses_rl >= standstill_max_tick_value) )
        {
          /* no standstill anymore */
          st_comh_buffer_data.is_vehicle_standstill = FALSE;
        }
      }
      standstill_time_counter = 0;
    }
    old_wheel_impulses_rl = current_wheel_impulses_rl;
  }
}

/* New definitions of functions for flex */

/* function to get the array index corresponding to its flex TX ID */

static uint8 get_mapped_tx_id_reverse (PduIdType id)
{
    uint8 i = 0;
    uint8 mapped_id = 255;

    for (i = 0;i<NUM_OF_TX_PDUS;i++)
    {
        if (tx_id_map_array[i].fr_id == id)
        {
            mapped_id = tx_id_map_array[i].array_id;
            break;
        }
    }

    return mapped_id;
}

/* function to get flex TX ID corresponding to its the array index   */

static uint8 get_mapped_tx_id (PduIdType id)
{
    uint8 i = 0;
    uint8 mapped_id = 255;

    for (i = 0;i<NUM_OF_TX_PDUS;i++)
    {
        if (tx_id_map_array[i].array_id == id)
        {
            mapped_id = tx_id_map_array[i].fr_id;
            break;
        }
    }

    return mapped_id;
}

/* function to get flex RX ID corresponding to its the array index   */

static uint8 get_mapped_rx_id (PduIdType id)
{
    uint8 i = 0;
    uint8 mapped_id = 255;

    for (i = 0;i<NUM_OF_RX_PDUS;i++)
    {
        if (id_map_array[i].fr_id == id)
        {
            mapped_id = id_map_array[i].array_id;
            break;
        }
    }

    return mapped_id;
}

/* Following functions to de-serialize RX PDUs */

static void encode_HMI_CHASSIS_R1_AR2_pdu (uint8 *buffer)
{
    static uint8 tmp_u8_L2 = 0;
    static uint8 tmp_u8_L = 0;
    uint8 prev_temp ;


    prev_temp = tmp_u8_L2;

    /* save confirmation_button state (APC Button in simulation [Park_APC_Sw_Psd_CTRL_L2]) */
    tmp_u8_L2 = (u8)((buffer[4] >> 4) & 0x03);

    if(tmp_u8_L2 != prev_temp )
    {

    	if (tmp_u8_L2 == 0u)
    	{
    		/* button not pressed */
    		st_comh_buffer_data.confirmation_button = FALSE;
    	}
    	else if (tmp_u8_L2 == 1u)
    	{
    		/* button pressed */
    		st_comh_buffer_data.confirmation_button = TRUE;
    	}
    	else
    	{
    		/* do not update confirmation_button status */
    	}
    }

    prev_temp = tmp_u8_L;

	/*if Park_APC_Sw_Psd_CTRL_L2 is SNA Decode from another signal Park_APC_Sw_Psd_CTRL_L*/
    tmp_u8_L = (buffer[2] & 0x30) >> 4 ;

    if(tmp_u8_L != prev_temp )
    {


    	if (tmp_u8_L == 0u)
    	{
    		/* button not pressed */
    		st_comh_buffer_data.confirmation_button = FALSE;
    	}
    	else if (tmp_u8_L == 1u)
    	{
    		/* button pressed */
    		st_comh_buffer_data.confirmation_button = TRUE;
    	}
    	else
    	{
    		/* do not update confirmation_button status */
    	}


    }

}

static void encode_PtMngmnt_Stat_AR2_pdu (uint8 *buffer)
{
    st_comh_buffer_data.engine_state = (u8)((buffer[7] >> 4) & 0x03);
}

static void encode_EPKB_Stat_AR2_pdu (uint8 *buffer)
{
    uint8 tmp_u8;
    /* set epn engaged to true/false */
    tmp_u8 = buffer[2] & 0x07;

    if(tmp_u8 == 3u)
    {
        st_comh_buffer_data.epb_engaged = TRUE;
    }
    else
    {
        st_comh_buffer_data.epb_engaged = FALSE;
    }
}


static void encode_CTRL_C_Stat1_AR_pdu (uint8 *buffer)
{
	static uint8 tmp_u8 = 0;

	uint8 prev_tmp_u8;

	prev_tmp_u8 = tmp_u8;

	/*ParkSw_Psd_CTRL_C => switch button */
	tmp_u8  = buffer[7] & 0x03;

	if(tmp_u8 != prev_tmp_u8)
	{
		if (tmp_u8 == 2u)
		{
			/* button pressed */
			st_comh_buffer_data.switch_options_button = TRUE;
		}
		else if (tmp_u8 == 0u)
		{
			/* button not pressed */
			st_comh_buffer_data.switch_options_button = FALSE;
		}
		else
		{
			/* do not update switch_options_button status */
		}
	}
}

static void encode_SVS_Rq_HU_AR2_pdu (uint8 *buffer)
{

}

static void encode_Config_HU4_AR2_pdu (uint8 *buffer)
{
//    uint8 tmp_u8;
//    /* save switch_option_button state
//     * (parking space selection Button in simulation [Park_ParkSpaceSelSw_Psd_HU]) */
//    tmp_u8 = (buffer[4] >> 6) & 0x03 ;
//
//    if (tmp_u8 == 2u)
//    {
//        /* button pressed */
//        st_comh_buffer_data.switch_options_button = TRUE;
//    }
//    else if (tmp_u8 == 1u)
//    {
//        /* button not pressed */
//        st_comh_buffer_data.switch_options_button = FALSE;
//    }
//    else
//    {
//        /* do not update switch_options_button status */
//    }
}

static void encode_Ign_Veh_Stat_AR2_pdu (uint8 *buffer)
{
    uint8 u8_tmp;
    u8_tmp = (buffer[1] >> 4) & 0x07;

    if ((u8_tmp == 4u) || (u8_tmp == 5u))
    {
        st_comh_buffer_data.ignition_on = TRUE;
    }
    else
    {
        st_comh_buffer_data.ignition_on = FALSE;
    }
}

static void encode_TCM_Data2_pdu (uint8 *buffer)
{
    uint8 u8_tmp;
    u8_tmp = (buffer[1] >> 4) & 0x0F;

    switch(u8_tmp)
    {
       case 0u:
       case 14u:
           st_comh_buffer_data.current_gear = GEAR_POSITION_N;
       break;
          case 1u:
          case 2u:
          case 3u:
          case 4u:
          case 5u:
          case 6u:
          case 7u:
          case 8u:
          case 9u:
           st_comh_buffer_data.current_gear = GEAR_POSITION_D;
          break;
       case 11u:
       case 12u:
           st_comh_buffer_data.current_gear = GEAR_POSITION_R;
          break;
       case 13u:
           st_comh_buffer_data.current_gear = GEAR_POSITION_P;
          break;
       default:
          break;
    }

}

static void encode_SBW_Stat_pdu (uint8 *buffer)
{
    st_comh_buffer_data.is_gear = (uint8)((buffer[2] >> 4) & 0x0F);

    if (st_comh_buffer_data.is_gear == 7u)
    {
        st_comh_buffer_data.reverse_gear_raw_data = TRUE;
    }
    else
    {
        st_comh_buffer_data.reverse_gear_raw_data = FALSE;
    }

}

static void encode_PARK_St_Rs_AR2_pdu (uint8 *buffer)
{
    st_comh_buffer_data.pla_termination_raw_data = (uint8)((buffer[7] >> 4) & 0x0F);
    st_comh_buffer_data.status_eps_raw_data = (uint8)((buffer[5] >> 3) & 0x07);
}

static void encode_STA_Rs1_AR2_pdu (uint8 *buffer)
{
    si16 tmp_si16;
    u16  tmp_u16;
    u8 tmp_u8;
    /* remove offset from data to convert it to a signed value */
    tmp_si16 = ((si16)(((buffer[6] << 8) | buffer[5]) & 0x3FFF) - 8192);

    if(tmp_si16 < 0)
    {
        st_comh_buffer_data.steering_moment_sign_raw_data = 1;
        st_comh_buffer_data.steering_moment_raw_data = (u16)(tmp_si16 * -1);
    }
    else
    {
        st_comh_buffer_data.steering_moment_sign_raw_data = 0;
        st_comh_buffer_data.steering_moment_raw_data = (u16)(tmp_si16);
    }

    /* save steering wheel angle in 0.1 resolution */
    /* remove offset from data to convert it to a signed value, update resolution as done in Parkman */
    st_comh_buffer_data.steering_angle_offset_raw_data = buffer[4];

    /* Input value:     loc_rx_signal_value                 */
    /*                  Unit: [°]                           */
    /*                  Physical range:  [-81.920 ; 80.910] */
    /*                  Resolution: 0.005                   */
    /*                  Offset: -81.920                     */
    /*                  Invalid value: 32767 (SNA)          */

    /* Input value:     signal_offset                       */
    /*                  Unit: [°]                           */
    /*                  Physical range: [-3.200 ; 3.150]    */
    /*                  Resolution: 0.025                   */
    /*                  Offset: -3.200                      */
    /*                  Invalid value: 255 (SNA)            */

    /* Output value:    Unit: [°]                           */
    /*                  Physical range: [-90.000 ; 90.000]  */
    /*                  Resolution: 360/65536               */
    /*                  Offset: 0.000                       */
    /*                  Invalid value: none                 */

    /* save FtWhlAngl */
    tmp_u16 = ((u16)(((buffer[3] << 8) | buffer[2]) & 0x7FFF));
    /* save front wheel angle stat before applying resolution on front wheel angle & saving it */
    tmp_u8 = (u8)((buffer[1] >> 6u) & 0x03);

    /* only update steering angle raw data & sign if state is high or low resolution & steering
       angle value is NOT SNA value (32767) */
    if(  ((tmp_u8 == 0u) || (tmp_u8 == 1u))
      && (tmp_u16 != 32767))
    {
       tmp_si16 = (si16)(( (((si32)((si32)tmp_u16 - 16384))
                         + (si32)(((si32)st_comh_buffer_data.steering_angle_offset_raw_data - 128) * 5)) * 1024) / 1125);
       if(tmp_si16 < 0)
       {
           st_comh_buffer_data.steering_angle_sign_raw_data = 1;
           st_comh_buffer_data.steering_angle_raw_data = (u16)(tmp_si16 * -1);
       }
       else
       {
           st_comh_buffer_data.steering_angle_sign_raw_data = 0;
           st_comh_buffer_data.steering_angle_raw_data = (u16)(tmp_si16);
       }
       st_comh_buffer_data.st_wheel_angle_time_2us = PIT_GetTimer2us();
       st_comh_buffer_data.steering_angle = tmp_si16;
    }
    else
    {
       /* Do not update steering angle raw data & sign */
    }

}

static void encode_VehDyn_Stat1_AR2_pdu (uint8 *buffer)
{
    si16 tmp_si16;
    st_comh_buffer_data.long_acc_offset_phys_data = buffer[5];
    st_comh_buffer_data.lateral_acceleration = (si16)((si16)(((buffer[4] << 8) | buffer[3]) & 0x03FF) - 1024);
    tmp_si16 = (si16)(((buffer[2] << 4) | (buffer[1] >> 4)) & 0x0FFF);
    st_comh_buffer_data.long_acc_phys_data = (si16)( ((si32)((si32)tmp_si16 - 2048u))
                                       + (si32)((si32)st_comh_buffer_data.long_acc_offset_phys_data - 128u)) * (si32)20u;
}

static void encode_Whl_Rt_Stat_AR2_pdu (uint8 *buffer)
{
    uint8 tmp_u8;
    si16 tmp_si16;

    st_comh_buffer_data.wheel_impulses.timestamp_2us_right = XDAPM_InputTimer2us();
    st_comh_buffer_data.wheel_speeds.timestamp_2us_right = XDAPM_InputTimer2us();
    st_comh_buffer_data.wheel_impulses.wheel_impulses_fr = (u16) buffer[4];
    st_comh_buffer_data.wheel_impulses.wheel_impulses_rr = (u16) buffer[7];
    st_comh_buffer_data.wheel_impulses.msg_count = (u8)(buffer[1] & 0x0F); /* add seperate message count for each wheel side? */
    st_comh_buffer_data.wheel_impulses.valid_fr = TRUE; /* wheel impulses validity stubbed to true */
    st_comh_buffer_data.wheel_impulses.valid_rr = TRUE; /* wheel impulses validity stubbed to true */
    tmp_si16 = (si16)(((buffer[3] << 8) | buffer[2]) & 0x3FFF);
    st_comh_buffer_data.wheel_speeds.wheel_speed_fr = (si16)(((si32)tmp_si16 * (si32)WHEEL_CIRCUMF_FRONT) / (si32)1200);
    tmp_si16 = (si16)(((buffer[6] << 8) | buffer[5]) & 0x3FFF);
    st_comh_buffer_data.wheel_speeds.wheel_speed_rr = (si16)(((si32)tmp_si16 * (si32)WHEEL_CIRCUMF_REAR) / (si32)1200);
    tmp_u8 = (u8)((buffer[3] >> 6) & 0x03);
    /* fl wheel direction */
    switch (tmp_u8)
    {
       case 0u:
           st_comh_buffer_data.wheel_info_fr_raw_data = COMH_WHEEL_INFO_STANDSTILL;
          break;
       case 1u:
           st_comh_buffer_data.wheel_info_fr_raw_data = COMH_WHEEL_INFO_FORWARD;
          break;
       case 2u:
           st_comh_buffer_data.wheel_info_fr_raw_data = COMH_WHEEL_INFO_BACKWARD;
          break;
       default:
          /* do not update wheel info */
          break;
    }
    /* RL wheel directoin */
    tmp_u8 = (u8)((buffer[6] >> 6) & 0x03);

    switch (tmp_u8)
    {
       case 0u:
           st_comh_buffer_data.wheel_info_rr_raw_data = COMH_WHEEL_INFO_STANDSTILL;
          break;
       case 1u:
           st_comh_buffer_data.wheel_info_rr_raw_data = COMH_WHEEL_INFO_FORWARD;
          break;
       case 2u:
           st_comh_buffer_data.wheel_info_rr_raw_data = COMH_WHEEL_INFO_BACKWARD;
          break;
       default:
          /* do not update wheel info */
          break;
    }

}

static void encode_VehSpd_X_AR2_pdu (uint8 *buffer)
{
    /* save vehicle speed data with a resolution of 0.01, input signal is of resolution 0.1 */
//    st_comh_buffer_data.wheel_speed_raw_data = (u16)((((u16)(buffer[4] << 4) | (u16)(buffer[3] >> 8)) & 0x0FFF) * 10u);
    st_comh_buffer_data.wheel_speed_raw_data = 0;
    st_comh_buffer_data.wheel_speed_raw_data |= (u16)((u8)((buffer[3] & 0xF0)>>4));
    st_comh_buffer_data.wheel_speed_raw_data |= (u16)((u8)(buffer[4]<<4));
    st_comh_buffer_data.wheel_speed_raw_data *= 10u;
}

static void encode_EL_TurnInd_Rq_AR2_pdu (uint8 *buffer)
{
    if((buffer[3] & 0x07) == 1u)
    {
       /* only left turn indicator is active */
        st_comh_buffer_data.blinker_left_raw_data = 1u;
    }
    else if((buffer[3] & 0x07) == 2u)
    {
       /* only right turn indicator is active */
        st_comh_buffer_data.blinker_right_raw_data = 1u;
    }
    else
    {
       /* either both indicators active or EF is active or invalid combination, switch off both blinkers */
        st_comh_buffer_data.blinker_left_raw_data = 0u;
        st_comh_buffer_data.blinker_right_raw_data = 0u;
    }
}

static void encode_BrkSys_Stat_AR2_pdu (uint8 *buffer)
{
    uint8 tmp_u8;
    /* save brake torque in buffer */
    st_comh_buffer_data.brake_pressure_raw_data = (si16)((((buffer[3] << 8) | buffer[2]) & 0x1FFF) * 3u); /* multiply by a factor of 3 as per FIBEX */
    st_comh_buffer_data.driver_brake_pressure_raw_data = (si16)((((buffer[5] << 8) | buffer[4]) & 0x1FFF) * 3u); /* multiply by a factor of 3 as per FIBEX */

    /* save esp operation status in buffer */
    st_comh_buffer_data.ESP_Oprtn_Stat_AS_V2 = (u8)(buffer[6] & 0x0F);
    /* save brake status in buffer */
    st_comh_buffer_data.brake_ecu_state = (u8)((buffer[1] >> 4) & 0x03);
    /* save brake pedal state in buffer */
    tmp_u8 = (buffer[1] >> 6) & 0x03;
    if(tmp_u8 == 0u)
    {
        st_comh_buffer_data.brake_pedal_stat = RELEASED;
    }
    else if(tmp_u8 == 1u)
    {
        st_comh_buffer_data.brake_pedal_stat = PRESSED;
    }
    else
    {
       /* do not update brake pedal data */
    }

    /* update brake intervention values */
    st_comh_buffer_data.AS_intervention_raw_data = ((buffer[7] >> 1) & 0x01);
    st_comh_buffer_data.CWA_intervention_raw_data = ((buffer[6] >> 7) & 0x01);
    st_comh_buffer_data.EPKB_intervention_raw_data = ((buffer[7] >> 2) & 0x01);
    st_comh_buffer_data.esp_intervention_raw_data = (buffer[7] & 0x01);
    /* Save ESP system state */
    tmp_u8 = ((buffer[6] >> 4) & 0x07);
    switch (tmp_u8 )
    {
       case 0u:
           st_comh_buffer_data.esp_system_state = ESP_SYSTEM_STATE_TEMPORARY_OFF;
          break;
       case 1u:
           st_comh_buffer_data.esp_system_state = ESP_SYSTEM_STATE_NORMAL_OPERATION;
          break;
       case 2u:
           st_comh_buffer_data.esp_system_state = ESP_SYSTEM_STATE_ERROR;
          break;
       case 3u:
           st_comh_buffer_data.esp_system_state = ESP_SYSTEM_STATE_DIAGNOSTIC;
          break;
       case 4u:
           st_comh_buffer_data.esp_system_state = ESP_SYSTEM_STATE_EXHAUST_EMISSION_TEST;
          break;
       case 5u:
           st_comh_buffer_data.esp_system_state = ESP_SYSTEM_STATE_TEST_BENCH_CC_ON;
          break;
       case 6u:
           st_comh_buffer_data.esp_system_state = ESP_SYSTEM_STATE_TEST_BENCH_CC_REGULATION_ACTIVE;
          break;
       default:
       /* do not update esp system state */
          break;
    }
}

static void encode_PARK_Brk_Rs_AR2_pdu (uint8 *buffer)
{
    uint8 tmp_u8;
    /* save park response status from esp to buffer */
    st_comh_buffer_data.park_enbl_stat_esp = (u8)((buffer[5] << 3) | (buffer[4] >> 5));
    tmp_u8 = ((buffer[2] >> 2) & 0x1F);
    switch(tmp_u8)
    {
       case 0u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_IDLE;
          break;
       case 1u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_ESP_FLT;
          break;
       case 2u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_LCP_PSV;
          break;
       case 3u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_PM_FLT;
          break;
       case 4u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_SPCR_FLT;
          break;
       case 5u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_EPB_FLT;
          break;
       case 6u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_LDC_SATURATED;
          break;
       case 7u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_MAX_SPD;
          break;
       case 8u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_REMAIN_DIST;
          break;
       case 9u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_MAX_SLOPE;
          break;
       case 10u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_ESP_CTRL;
          break;
       case 11u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_TCM_FLT;
          break;
       case 12u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_ECM_FLT;
          break;
       case 13u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_ENG_OFF;
          break;
       case 14u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_DRV_ACTV;
          break;
       case 15u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_DRV_ABSENCE;
          break;
       case 16u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_LDC_OVERRULED;
          break;
       case 17u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_RPA_FLT;
          break;
       case 18u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_RPA_MAX_SPD;
          break;
       case 19u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_RPA_UNDO_MAX_SLOPE;
          break;
       case 20u:
          st_comh_buffer_data.park_flt_stat_esp = ESP_STATE_RPA_MAX_SLOPE;
          break;
       default:
          /* do not update park_flt_stat_esp */
          break;
    }
    /* save afterrun ctrl data */
    tmp_u8 = ((buffer[5] >> 5u) & 0x03);
    switch(tmp_u8)
    {
       case 0u:
          st_comh_buffer_data.esp_ldc_afterrun_ctrl = ESP_LDC_AFTERRUNCTRL_NO_FOLLOW_UP;
          break;
       case 1u:
          st_comh_buffer_data.esp_ldc_afterrun_ctrl = ESP_LDC_AFTERRUNCTRL_BRAKE_FOLLOW_UP;
          break;
       case 2u:
          st_comh_buffer_data.esp_ldc_afterrun_ctrl = ESP_LDC_AFTERRUNCTRL_RELEASE_FOLLOW_UP;
          break;
       default:
          /* do not update afterrun ctrl data */
          break;
    }
    /* update park_stat_esp data */
    tmp_u8 = ((buffer[1] >> 4) & 0x0F);
    switch (tmp_u8)
    {
       case 0u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_PARKMAN_NOT_AVAILABLE;
          break;
       case 1u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_PARKMAN_INIT;
          break;
       case 2u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_PARKMAN_CANCEL_DRIVER;
          break;
       case 3u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_PARKMAN_CANCEL_FAULT;
          break;
       case 4u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_PARKMAN_ENABLED;
          break;
       case 7u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_PARKMAN_INACTIV;
          break;
       case 8u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_APC_MODE;
          break;
       case 9u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_RPA_MODE;
          break;
       case 10u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_RPA_UNDO_MODE;
          break;
       case 11u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_DAA_MODE;
          break;
       case 12u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_PPB_MODE;
          break;
       case 13u:
          st_comh_buffer_data.esp_brake_state_park = BRAKING_STATE_CTA_MODE;
          break;
       default:
          /* do not update esp_brake_state_park */
          break;
    }
}

static void encode_Whl_Lt_Stat_AR2_pdu (uint8 *buffer)
{
    uint8 tmp_u8;
    si16 tmp_si16;

    st_comh_buffer_data.wheel_impulses.timestamp_2us_left = XDAPM_InputTimer2us();
    st_comh_buffer_data.wheel_speeds.timestamp_2us_left = XDAPM_InputTimer2us();
    st_comh_buffer_data.wheel_impulses.wheel_impulses_fl = (u16) buffer[4];
    st_comh_buffer_data.wheel_impulses.wheel_impulses_rl = (u16) buffer[7];
    st_comh_buffer_data.wheel_impulses.msg_count = (u8)(buffer[1] & 0x0F); /* add seperate message count for each wheel side? */
    st_comh_buffer_data.wheel_impulses.valid_fl = TRUE; /* wheel impulses validity stubbed to true */
    st_comh_buffer_data.wheel_impulses.valid_rl = TRUE; /* wheel impulses validity stubbed to true */
    tmp_si16 = (si16)(((buffer[3] << 8) | buffer[2]) & 0x3FFF);
    st_comh_buffer_data.wheel_speeds.wheel_speed_fl = (si16)(((si32)tmp_si16 * (si32)WHEEL_CIRCUMF_FRONT) / (si32)1200);
    tmp_si16 = (si16)(((buffer[6] << 8) | buffer[5]) & 0x3FFF);
    st_comh_buffer_data.wheel_speeds.wheel_speed_rl = (si16)(((si32)tmp_si16 * (si32)WHEEL_CIRCUMF_REAR) / (si32)1200);
    tmp_u8 = (u8)((buffer[3] >> 6) & 0x03);
    /* fl wheel direction */
    switch (tmp_u8)
    {
       case 0u:
           st_comh_buffer_data.wheel_info_fl_raw_data = COMH_WHEEL_INFO_STANDSTILL;
          break;
       case 1u:
           st_comh_buffer_data.wheel_info_fl_raw_data = COMH_WHEEL_INFO_FORWARD;
          break;
       case 2u:
           st_comh_buffer_data.wheel_info_fl_raw_data = COMH_WHEEL_INFO_BACKWARD;
          break;
       default:
          /* do not update wheel info */
          break;
    }
    /* RL wheel directoin */
    tmp_u8 = (u8)((buffer[6] >> 6) & 0x03);

    switch (tmp_u8)
    {
       case 0u:
           st_comh_buffer_data.wheel_info_rl_raw_data = COMH_WHEEL_INFO_STANDSTILL;
          break;
       case 1u:
           st_comh_buffer_data.wheel_info_rl_raw_data = COMH_WHEEL_INFO_FORWARD;
          break;
       case 2u:
           st_comh_buffer_data.wheel_info_rl_raw_data = COMH_WHEEL_INFO_BACKWARD;
          break;
       default:
          /* do not update wheel info */
          break;
    }
}

static void encode_VehDyn_Stat2_AR2_pdu (uint8 *buffer)
{
    uint8 tmp_u8;
    uint16 tmp_u16;
    uint32 tmp_u32;
    si32 tmp_s32;

    /* save vehicle yaw data */
        /* Input value:     loc_rx_signal_value                     */
        /*                  Unit: [?s]                             */
        /*                  Physical range:  [-327.68 ; 327.66]     */
        /*                  Resolution: 0.01                        */
        /*                  Offset: -327.68                         */
        /*                  Invalid value: 65535 (SNA)              */

        /* Input value:     signal_offset                           */
        /*                  Unit: [?s]                             */
        /*                  Physical range: [-5.12 ; 5.10]          */
        /*                  Resolution: 0.01                        */
        /*                  Offset: -5.12                           */
        /*                  Invalid value: 1023 (SNA)               */

        /* Output value:    Unit: [?s]                             */
        /*                  Physical range: [-180.0 ; 180.0[        */
        /*                  Resolution: 180/32768                   */
        /*                  Offset: 0.000                           */
        /*                  Invalid value: none                     */
    /* save vehicle yaw quality */
    tmp_u8 = (u8)(buffer[1] >> 4) & 0x03;
    /* save vehicle yaw offset */
    tmp_u16 = (u16)(((buffer[2] << 2u) | (buffer[1] >> 6)) & 0x03FF);
    /* save vehicle yaw raw */
    tmp_u32 = (u32)((buffer[4] << 8u) | (buffer[3]));

    /* apply yaw offset to yaw raw data if qual is ELEC or FUNC, and yaw raw data & yaw offset are not SNA*/
    if ((tmp_u16 != 1023u) && (tmp_u32 != 65535u) && ((tmp_u8 == 1u) || (tmp_u8 == 2u)))
    {
    tmp_s32 = (sint32)( ( ( ((sint32)tmp_u32 - (sint32)32768)
                      + ((sint32)tmp_u16 - (sint32)512))
                     * (sint32)2048) / (sint32)1125);
    if(tmp_s32 < 0)
    {
        st_comh_buffer_data.yaw_speed_raw_data = (u16)(-1 * tmp_s32);
        st_comh_buffer_data.yaw_speed_sign_raw_data = (u8)(1u);
    }
    else
    {
        st_comh_buffer_data.yaw_speed_raw_data = (u16)tmp_s32;
        st_comh_buffer_data.yaw_speed_sign_raw_data = (u8)0u;
    }
    st_comh_buffer_data.yaw_speed_timestamp_2us = XDAPM_InputTimer2us();
    }
    else
    {
    /* do nothing, do not update yaw values */
    }
}

static void encode_BC_F_Stat2_AR2_buffer_pdu (uint8 *buffer)
{


	static uint8 tmp_u8 = 0;

	uint8 prev_tmp_u8;

	prev_tmp_u8 = tmp_u8;

	/*ParkSw_Psd_CTRL_C => switch button */
	tmp_u8  = buffer[2] & 0x03;

	if(tmp_u8 != prev_tmp_u8)
	{
		if (tmp_u8 == 2u)
		{
			/* button pressed */
			st_comh_buffer_data.switch_options_button = TRUE;
		}
		else if (tmp_u8 == 0u)
		{
			/* button not pressed */
			st_comh_buffer_data.switch_options_button = FALSE;
		}
		else
		{
			/* do not update switch_options_button status */
		}
	}

}

static void encode_updated_rx_pdus(void)
{
    uint8 i = 0;

    _DISABLE_INTERRUPTS();
    for (i=0;i<NUM_OF_RX_PDUS;i++)
    {
        if (rx_pdus_array[i].is_updated == 1)
        {
            MemCpy_Ram2Ram(rx_pdus_array[i].buffer,rx_pdus_array[i].buffer_copy,rx_pdus_array[i].length);
//            rx_pdus_array[i].is_updated = 0;
        }
    }
    _ENABLE_INTERRUPTS();

    for (i=0;i<NUM_OF_RX_PDUS;i++)
    {
        if (rx_pdus_array[i].is_updated == 1)
        {
            rx_pdus_array[i].encode_function(rx_pdus_array[i].buffer_copy);
            rx_pdus_array[i].is_updated = 0;
        }
    }

}

static void MemCpy_Ram2Ram( const uint8 *src, uint8 *dest, uint8 cnt )
{
    for( ;cnt > 0u; --cnt )
    {
      dest[cnt-1u] = src[cnt-1u];
    }
}

void COMH_CopyRxBuffer(PduIdType FrIfTxPduId, const PduInfoType *PduInfoPtr)
{
    uint8 local_id = 255;
    local_id = get_mapped_rx_id(FrIfTxPduId);

    if (local_id != 255) /* copy only if the returned ID is VALID. (a valid ID will never be 255) */
    {
        _DISABLE_INTERRUPTS();
        MemCpy_Ram2Ram(PduInfoPtr->SduDataPtr, rx_pdus_array[local_id].buffer, PduInfoPtr->SduLength);
        rx_pdus_array[local_id].is_updated = 1;
        _ENABLE_INTERRUPTS();
    }
}

void COMH_TriggerTransmit(PduIdType FrIfTxPduId, PduInfoType *PduInfoPtr)
{
    uint8 local_id = 0;

    local_id = get_mapped_tx_id_reverse((uint8)FrIfTxPduId);

    if (local_id != 255)
    {
        tx_pdus_array[local_id].tx_function(PduInfoPtr);
    }
}

/* dummy must be used while sending (not immediate TX PDUs) */
static PduInfoType dummy;
/* function to request to transmit TX PDUs */

static void transmit_tx_pdus(void)
{
    uint8 i = 0;
    uint8 local_id = 0;

    for (i=0;i<NUM_OF_TX_PDUS;i++)
    {
      local_id = get_mapped_tx_id(i);
      /* check if id is a VALID id (cannot be 255)*/
      if (local_id != 255)
      {
          FrWR_Appl_Transmit(local_id,&dummy);
      }
    }
}

/* Following functions to send TX PDUs */

static void send_ST_Rq_PARK_AR2_pdu(PduInfoType *PduInfoPtr)
{
    u8      temp_PARK_Stat_PARK;
    si16    temp_FRWheelAngle_Req;
    uint16  temp_FRWheelAngle_Req_to_send;
    uint8   temp_Sp_Stat_PARK = 0; /* Stub PARK_Sp_Stat_PARK to IDLE (0) */
    uint16  temp_PPS_AddRf_Rq = 0x1000; /* stub additional rack force to 0 physical -> 0x1000 hexadecimal*/
    uint8   temp_PPS_Stat_PARK = 1; /* stub PPS stat to disabled */
    u8 i = 0;

    for (i=0;i<8;i++)
    {
        PduInfoPtr->SduDataPtr[i] = 0;
    }

    uint8 SerDataDEData[(64U)/8U];

    St_Rq_PARK_Pr2 rq_park_pr2;
    memset(&rq_park_pr2 , 0 , sizeof(rq_park_pr2));

    temp_PARK_Stat_PARK             = STMH_CusGetAppStateToSteering();
    temp_FRWheelAngle_Req           = P2DAL_GetCmdSteeringOutputFront();

#ifdef E2E
    temp_PARK_EPS_CRC               = COMH_CALC_CRC();
    temp_PARK_EPS_SQC               = COMH_CALC_SQC();
#endif

    if (STMH_CusIsSteeringActive() )
    {
        temp_FRWheelAngle_Req_to_send   = (uint16)(((si32)((si32)temp_FRWheelAngle_Req * (si16)1125) / (si16)1024) + (si32)16384);
    }
    else
    {
        temp_FRWheelAngle_Req_to_send = (uint16)(((si32)((si32)st_comh_buffer_data.steering_angle * (si16)1125) / (si16)1024) + (si32)16384);
    }

    /* Steer_Re_PARK CAN message */

    /* FtWhlAngl_Rq */
    PduInfoPtr->SduDataPtr[4] |= (u8)((temp_FRWheelAngle_Req_to_send & 0x7F) << 1);
    PduInfoPtr->SduDataPtr[5] |= (u8)((u16)(temp_FRWheelAngle_Req_to_send & 0x7F80) >> 7);
    /* Sp_Stat */
    PduInfoPtr->SduDataPtr[6] |= (u8)(temp_Sp_Stat_PARK  &  0x03); /* Stub PARK_Sp_Stat_PARK to IDLE (0) */
    /* Stat_PARK */
    PduInfoPtr->SduDataPtr[1] |= (u8)((temp_PARK_Stat_PARK  &  0x0F) << 4);
    /* PPS_AddRf_Rq */
    PduInfoPtr->SduDataPtr[2] = (u8)(temp_PPS_AddRf_Rq & 0xFF);
    PduInfoPtr->SduDataPtr[3] |= (u8)((temp_PPS_AddRf_Rq >> 8) & 0x1F);
    /* PPS_Stat */
    PduInfoPtr->SduDataPtr[3] |= (u8)((temp_PPS_Stat_PARK & 0x07) << 5);
    PduInfoPtr->SduDataPtr[4] |= (u8)((temp_PPS_Stat_PARK >> 4) & 0x01);

    /*Filling PDU struct to pass it to PDU serl*/
    rq_park_pr2.FtWhlAngl_Rq_PARK = temp_FRWheelAngle_Req_to_send;

    rq_park_pr2.PARK_Stat_PARK = temp_PARK_Stat_PARK;

    rq_park_pr2.PARK_Sp_Stat = (u8)0u; /* Stub PARK_Sp_Stat_PARK to IDLE */
    rq_park_pr2.PPS_AddRf_Rq = (u16)4096u; /* stub additional rack force to 0 physical -> 0x1000 hexadecimal*/
    rq_park_pr2.PPS_Stat_PARK = (u8)1u; /* stub PPS stat to disabled */
    rq_park_pr2.Rsrv2_St_Rq_PARK_Pr2 = (u16)0u;


    /*call serl for this PDU*/
    Cfg_St_Rq_PARK_Pr2.e2e_serl_func(&rq_park_pr2 , SerDataDEData);

    /*Calculate E2E header*/
    E2E_EB_P02Protect(&Cfg_St_Rq_PARK_Pr2,&rq_park_pr2_sender_state , SerDataDEData);

    /*Send CRC & SQC on CAN from serDataEData SerDataDEData[1] - > SQC & SerDataDEData[0] -> CRC*/

    /* CRC */
    PduInfoPtr->SduDataPtr[0] = SerDataDEData[0];
    /* SQC */
    PduInfoPtr->SduDataPtr[1] |= (u8)(SerDataDEData[1] & 0x0F);
}

static void send_Brk_Rq_PARK_AR2_pdu(PduInfoType *PduInfoPtr)
{
    u8      temp_Park_DangerMd_Rq ;
    si16       temp_PARK_Veh_Spd_Re;
   u8      temp_PARK_TXDrvPosn_Rq;
   u8      temp_PARK_Park_Stat;
   u8      temp_PARK_BrkMD_Rq;
   u8      temp_PARK_ParkType;
   u8      temp_PARK_CtrlMD_Rq;
   u8      temp_PARK_RemainDist_Stat;
   si16    temp_PARK_RemainDist;
   u8      temp_PARK_Brk_Stat;
   u8 i = 0;

   for (i=0;i<8;i++)
   {
       PduInfoPtr->SduDataPtr[i] = 0;
   }

   uint8 brk_reqSerDataDEData[(64U)/8U];

   /*PDU of Brk_Rq_Park_Pr2*/
   Brk_Rq_PARK_Pr2 brk_rq_park_pr2 ;


   /*memset Brk Rq PDU with zeros */
   memset(&brk_rq_park_pr2 , 0 , sizeof(brk_rq_park_pr2));

   /* Steer_Re_PARK message data */
   temp_Park_DangerMd_Rq       = (u8) BRKH_GetParkDangerMdRq(); /* no need for switch case, 0 is inactive (false), 1 is active (true)*/
   temp_PARK_Veh_Spd_Re        = (BRKH_GetVehSpdRq() / 10u) /* converting signal back to 0.1 resolution from 0.01 */;
   switch(BRKH_GetGearPosRq())
   {
     case GEAR_POSITION_RQ_IDLE:
        temp_PARK_TXDrvPosn_Rq = 0u;
        break;
     case GEAR_POSITION_RQ_D:
        temp_PARK_TXDrvPosn_Rq = 5u;
        break;
     case GEAR_POSITION_RQ_N:
        temp_PARK_TXDrvPosn_Rq = 6u;
        break;
     case GEAR_POSITION_RQ_R:
        temp_PARK_TXDrvPosn_Rq = 7u;
        break;
     case GEAR_POSITION_RQ_P:
        temp_PARK_TXDrvPosn_Rq = 8u;
        break;
     default:
        temp_PARK_TXDrvPosn_Rq = 15u; /* set invalid value */
        break;
   }

   temp_PARK_Park_Stat         = (u8)BRKH_GetParkParkStat();
   temp_PARK_BrkMD_Rq          = (u8)BRKH_GetParkBrkMdRq();
   temp_PARK_ParkType          = (u8)BRKH_GetParkParkType();
   temp_PARK_CtrlMD_Rq         = (u8)BRKH_GetParkCtrlMdRq();
   temp_PARK_RemainDist_Stat   = (u8)BRKH_GetRemainDistStat();
   temp_PARK_RemainDist        = BRKH_GetRemainDist();

   switch(BRKH_GetParkBrkStat())
   {
     case BRAKING_STATE_PARKMAN_NOT_AVAILABLE:
        temp_PARK_Brk_Stat = 0u;
        break;
     case BRAKING_STATE_PARKMAN_INIT:
        temp_PARK_Brk_Stat = 1u;
        break;
     case BRAKING_STATE_PARKMAN_CANCEL_DRIVER:
        temp_PARK_Brk_Stat = 2u;
        break;
     case BRAKING_STATE_PARKMAN_CANCEL_FAULT:
        temp_PARK_Brk_Stat = 3u;
        break;
     case BRAKING_STATE_PARKMAN_ENABLED:
        temp_PARK_Brk_Stat = 4u;
        break;
     case BRAKING_STATE_PARKMAN_INACTIV:
        temp_PARK_Brk_Stat = 7u;
        break;
     case BRAKING_STATE_APC_MODE:
        temp_PARK_Brk_Stat = 8u;
        break;
     case BRAKING_STATE_RPA_MODE:
        temp_PARK_Brk_Stat = 9u;
        break;
     case BRAKING_STATE_RPA_UNDO_MODE:
        temp_PARK_Brk_Stat = 10u;
        break;
     case BRAKING_STATE_DAA_MODE:
        temp_PARK_Brk_Stat = 11u;
        break;
     case BRAKING_STATE_PPB_MODE:
        temp_PARK_Brk_Stat = 12u;
        break;
     case BRAKING_STATE_CTA_MODE:
        temp_PARK_Brk_Stat = 13u;
        break;
     default:
        temp_PARK_Brk_Stat = 15u; /* set not available value */
        break;
  }

   /* Before Conversion:                                */
   /* temp_PARK_RemainDist value:     Unit: [mm]        */
   /*                  Physical range: [-32768 ; 32767] */
   /*                  Resolution: 1                    */
   /*                  Offset: 0                        */
   /*                  Invalid value: none              */
   /* After Conversion:                                 */
   /* temp_PARK_RemainDist value:    Unit: [cm]         */
   /*                  Physical range: [-30 ; 2015]     */
   /*                  Resolution: 1                    */
   /*                  Offset: -30                      */
   /*                  Invalid value: none              */

   if ((sint16)PARK_REMAIN_DIST_TX_SIGNAL_VALUE_MIN > temp_PARK_RemainDist)
   {
       /* Data element value below the value range of the bus signal */

       temp_PARK_RemainDist = (uint16)(0u); /* -30 [cm] */
   }
   else if ((sint16)PARK_REMAIN_DIST_TX_SIGNAL_VALUE_MAX < temp_PARK_RemainDist)
   {
       /* Data element value above the value range of the bus signal */

       temp_PARK_RemainDist = (uint16)(2045u); /* 2015 [cm] */
   }
   else
   {
       /* Data element value in the range of the bus signal */
       temp_PARK_RemainDist = (uint16)((sint16)(temp_PARK_RemainDist / (10)) + (sint16)(30u));
   }

   /* BRK_Req_PARK CAN message */
   PduInfoPtr->SduDataPtr[2] |= ((u8)(temp_Park_DangerMd_Rq & 0x01)      << 7);
   PduInfoPtr->SduDataPtr[4] |= ((u8)(temp_PARK_Veh_Spd_Re       & 0x07) << 5);
   PduInfoPtr->SduDataPtr[5] |= ((u8)(temp_PARK_Veh_Spd_Re       & 0x78) >> 3);
   PduInfoPtr->SduDataPtr[6] |= ((u8)(temp_PARK_TXDrvPosn_Rq     & 0x0F) << 3);
   PduInfoPtr->SduDataPtr[2] |= ((u8)temp_PARK_Park_Stat         & 0x0F  );
   PduInfoPtr->SduDataPtr[4] |= ((u8)(temp_PARK_BrkMD_Rq         & 0x03) << 3);
   PduInfoPtr->SduDataPtr[2] |= ((u8)(temp_PARK_ParkType         & 0x07) << 4);
   PduInfoPtr->SduDataPtr[6] |= ((u8)temp_PARK_CtrlMD_Rq         & 0x07  );
   PduInfoPtr->SduDataPtr[5] |= ((u8)(temp_PARK_RemainDist_Stat   & 0x07)<< 4);
   PduInfoPtr->SduDataPtr[3] = (u8)(temp_PARK_RemainDist   & 0xff  );
   PduInfoPtr->SduDataPtr[4] |= (u8)((temp_PARK_RemainDist >> 8) & 0x07 );
   PduInfoPtr->SduDataPtr[1] |= ((u8)(temp_PARK_Brk_Stat       & 0x0F )  << 4);
   /*Filling PDU struct to pass it to PDU serl*/
   brk_rq_park_pr2.PARK_DangerMd_Rq = temp_Park_DangerMd_Rq;

   brk_rq_park_pr2.PARK_VehSpd_Rq = temp_PARK_Veh_Spd_Re;

   brk_rq_park_pr2.PARK_TxDrvPosn_Rq = temp_PARK_TXDrvPosn_Rq;

   brk_rq_park_pr2.PARK_Park_Stat  = temp_PARK_Park_Stat;

   brk_rq_park_pr2.PARK_BrkMd_Rq = temp_PARK_BrkMD_Rq ;

   brk_rq_park_pr2.PARK_ParkType = temp_PARK_ParkType;

   brk_rq_park_pr2.PARK_CtrlMd_Rq = temp_PARK_CtrlMD_Rq;

   brk_rq_park_pr2.PARK_RemainDist_Stat = temp_PARK_RemainDist_Stat;

   brk_rq_park_pr2.PARK_RemainDist = temp_PARK_RemainDist;

   brk_rq_park_pr2.PARK_Brk_Stat = temp_PARK_Brk_Stat;

   brk_rq_park_pr2.Rsrv1_Brk_Rq_PARK_Pr2 = (u8)0u;
   brk_rq_park_pr2.Rsrv2_Brk_Rq_PARK_Pr2 = (u8)0u;
   brk_rq_park_pr2.Rsrv3_Brk_Rq_PARK_Pr2 = (u8)0u;


   /*call SERL FUNC for this PDU*/
   Cfg_Brk_Rq_PARK_Pr2.e2e_serl_func(&brk_rq_park_pr2 , brk_reqSerDataDEData);

   /*Calculate E2E header*/
   E2E_EB_P02Protect(&Cfg_Brk_Rq_PARK_Pr2,&brk_rq_park_Pr2_sender_state, brk_reqSerDataDEData);

   /*Fill CRC and SQC from serDataEData SerDataDEData[1] - > SQC & SerDataDEData[0] -> CRC*/
   PduInfoPtr->SduDataPtr[0] = brk_reqSerDataDEData[0];

   PduInfoPtr->SduDataPtr[1] |= brk_reqSerDataDEData[1] & 0x0F;

}

static void send_Park_stat_AR2_pdu(PduInfoType *PduInfoPtr)
{
    static u8 taskCounter = 0u; /* periodicity is 200 ms, so send message every 10 task calls to keep SQC synchronized */
    u8        temp_PARK_ParkGuid_Stat;

    uint8   park_cfg_SerDataDEData[(32U)/8U];
    u8 i = 0;

    for (i=0;i<8;i++)
    {
        PduInfoPtr->SduDataPtr[i] = 0;
    }

    /*PDU of ParkCfg_Stat_Pr2*/
    ParkCfg_Stat_Pr2 park_cfg_stat_pr2;

      /*memset ParkCfg_Stat_Pr2 PDU with zeros */
       memset(&park_cfg_stat_pr2 , 0 , sizeof(park_cfg_stat_pr2));

       /* Park_ParkGuid_Stat_PARK message data */
       temp_PARK_ParkGuid_Stat   = BRKH_GetParkGuidStat();


       /* Park_ParkGuid_Stat_PARK message */
       PduInfoPtr->SduDataPtr[1] |= (u8)(temp_PARK_ParkGuid_Stat & 0x03) << 4;
       //       PduInfoPtr->SduDataPtr[0] = ((u8)temp_PARK_ParkGuid_Stat);
       /* ParkStyle_V2*/
       PduInfoPtr->SduDataPtr[3] = (u8)31u; /* stubbed as in parkman */
       /*Filling PDU struct to pass it to PDU SERL*/
       park_cfg_stat_pr2.ParkGuid_Stat = temp_PARK_ParkGuid_Stat;
       park_cfg_stat_pr2.ParkStyle_V2 = (u8)31u; /* stubbed as in parkman */
       park_cfg_stat_pr2.Rsrv1_ParkCfg_Stat_Pr2 = (u8)0u;
       park_cfg_stat_pr2.Rsrv2_ParkCfg_Stat_Pr2 = (u8)0u;

       /*call SERL FUNC for this PDU*/
       Cfg_ParkCfg_Stat_Pr2.e2e_serl_func(&park_cfg_stat_pr2 , park_cfg_SerDataDEData);

       /*Calculate E2E header*/
       E2E_EB_P02Protect(&Cfg_ParkCfg_Stat_Pr2,&parkcfg_stat_pr2_sender_state, park_cfg_SerDataDEData);

       /* CRC SerDataDEData[0], SQC SerDataDEData[1]*/
       PduInfoPtr->SduDataPtr[0] = ((u8)park_cfg_SerDataDEData[0]);
       PduInfoPtr->SduDataPtr[1] |= ((u8)park_cfg_SerDataDEData[1] & 0x0F);

}

static void send_RVC_SVS_Park_stat_AR2_pdu(PduInfoType *PduInfoPtr)
{
    IPC_PLAY_MODE play_mode;
    u8 i = 0;

    for (i=0;i<8;i++)
    {
        PduInfoPtr->SduDataPtr[i] = 0;
    }

    /* SVS_CPF_R_Rq , SVS_CPF_Ft_Rq, SVS_ExtLmp_Rq, Park_ExplorMd_Avl */
    if (COMH_GetKl15() == KL15_ON)
    {
        PduInfoPtr->SduDataPtr[2] |= (1u) << 4;
        PduInfoPtr->SduDataPtr[2] |= (1u) << 6;
        PduInfoPtr->SduDataPtr[3] |= 1u;
        PduInfoPtr->SduDataPtr[5] |= (1u) << 6;
    }
    else
    {
        PduInfoPtr->SduDataPtr[2] |= (0u) << 4;
        PduInfoPtr->SduDataPtr[2] |= (0u) << 6;
        PduInfoPtr->SduDataPtr[3] |= 0u;
        PduInfoPtr->SduDataPtr[5] |= (0u) << 6;
    }

    /* set PARK_SysMd_Rq_Cam & Park_Row1_MsgDisp_Rq values according to VIDC */
    PLATFORM_ReadIPCPlayMode(&play_mode);
    switch(play_mode)
    {
       case PLAY_MODE_OFF_ABORT:
       case PLAY_MODE_OFF:
          PduInfoPtr->SduDataPtr[2] |= 9u;
          PduInfoPtr->SduDataPtr[3] |= (0x2A << 2); /* set Park_Row1_MsgDisp_Rq text to front */
          break;
       case PLAY_MODE_TOPVIEW_REARVIEW:
          PduInfoPtr->SduDataPtr[2] |= 2u;
          PduInfoPtr->SduDataPtr[3] |= (0x2B << 2); /* set Park_Row1_MsgDisp_Rq text to rear */
          break;
       case PLAY_MODE_REARVIEWFULLSCREEN:
          PduInfoPtr->SduDataPtr[2] |= 3u;
          PduInfoPtr->SduDataPtr[3] |= (0x2F << 2); /* set Park_Row1_MsgDisp_Rq text to rear wide angle */
          break;
       case PLAY_MODE_TOPVIEW_TRAILERCOUPLINGVIEW:
          PduInfoPtr->SduDataPtr[2] |= 2u;
          PduInfoPtr->SduDataPtr[3] |= (0x2B << 2); /* set Park_Row1_MsgDisp_Rq text to rear */
          break;
       case PLAY_MODE_TOPVIEW_SIDEVIEWFORWARD:
          PduInfoPtr->SduDataPtr[2] |= 7u;
          PduInfoPtr->SduDataPtr[3] |= (0x2A << 2); /* set Park_Row1_MsgDisp_Rq text to front */
          break;
       case PLAY_MODE_TOPVIEW_FRONTVIEW:
          PduInfoPtr->SduDataPtr[2] |= 8u;
          PduInfoPtr->SduDataPtr[3] |= (0x2A << 2); /* set Park_Row1_MsgDisp_Rq text to front */
          break;
       case PLAY_MODE_FRONTVIEWFULLSCREEN:
          PduInfoPtr->SduDataPtr[2] |= 9u;
          PduInfoPtr->SduDataPtr[3] |= (0x2E << 2); /* set Park_Row1_MsgDisp_Rq text to front wide angle */
          break;
       case PLAY_MODE_APCVIEW_REARVIEW:
          PduInfoPtr->SduDataPtr[2] |= 11u;
          PduInfoPtr->SduDataPtr[3] |= (0x2B << 2); /* set Park_Row1_MsgDisp_Rq text to rear */
          break;
       case PLAY_MODE_APCVIEW_WALLPAPER:
          PduInfoPtr->SduDataPtr[2] |= 8u; /* set to TV FV */
          PduInfoPtr->SduDataPtr[3] |= (0x2A << 2); /* set Park_Row1_MsgDisp_Rq text to front */
          break;
       case PLAY_MODE_OFF_BY_SPD_FS:
          PduInfoPtr->SduDataPtr[2] |= 1u;
          PduInfoPtr->SduDataPtr[3] |= 0x2A; /* set Park_Row1_MsgDisp_Rq text to front */
          break;
       default:
          PduInfoPtr->SduDataPtr[2] |= 2u;
          PduInfoPtr->SduDataPtr[3] |= 0x2A; /* set Park_Row1_MsgDisp_Rq text to front */
          break;
    }

    PduInfoPtr->SduDataPtr[0] |= (0u) << 2; /* stub DRVM_Warn_Rq to 0 */

}

static void send_Park_Disp_Rq_AR2_pdu(PduInfoType *PduInfoPtr)
{

    Park_Disp_Rq_AR2        park_disp_rq_ar2;
    u8                      park_disp_rq_SerDataDEData[(64u/8u)];
    u8 i = 0;

    for (i=0;i<8;i++)
    {
        PduInfoPtr->SduDataPtr[i] = 0;
    }

    memset(&park_disp_rq_ar2, 0, sizeof(Park_Disp_Rq_AR2));

       /* perform PDU filling for Park_Disp_Rq_AR2 */

      /* save park icon display request according to ESP & EPS states to indicate errors */
 #ifdef XAPPL_LOT_CNTRL_SUPP
      /* set parc icon display to search if there are no ESP faults (if LOT is enabled) & EPS is not
         in disabled or cancel_sys states */
      if(  st_comh_buffer_data.park_flt_stat_esp != ESP_STATE_ESP_FLT
        && st_comh_buffer_data.park_flt_stat_esp != ESP_STATE_PM_FLT
        && st_comh_buffer_data.park_flt_stat_esp != ESP_STATE_SPCR_FLT
        && st_comh_buffer_data.park_flt_stat_esp != ESP_STATE_EPB_FLT
        && st_comh_buffer_data.park_flt_stat_esp != ESP_STATE_TCM_FLT
        && st_comh_buffer_data.park_flt_stat_esp != ESP_STATE_ECM_FLT
        && st_comh_buffer_data.park_flt_stat_esp != ESP_STATE_RPA_FLT
        && st_comh_buffer_data.status_eps_raw_data != 3u
        && st_comh_buffer_data.status_eps_raw_data != 4u)
 #else
      if(st_comh_buffer_data.status_eps_raw_data != 3u
        && st_comh_buffer_data.status_eps_raw_data != 4u)
 #endif
      {
         park_disp_rq_ar2.Park_IconDisp_Rq = 1u;
         PduInfoPtr->SduDataPtr[2] = 1u;
      }
      else
      {
         park_disp_rq_ar2.Park_IconDisp_Rq = 0u;
         PduInfoPtr->SduDataPtr[2] = 0u;
      }

      park_disp_rq_ar2.Park_Sound_Rq = 0u;
      park_disp_rq_ar2.Park_Warn1_Disp_Rq = 0u;
      park_disp_rq_ar2.Rsrv1_Park_Disp_Rq_Pr2 = 0u;
      park_disp_rq_ar2.Rsrv2_Park_Disp_Rq_Pr2 = 0u;
      park_disp_rq_ar2.Rsrv3_Park_Disp_Rq_Pr2 = 0u;
      park_disp_rq_ar2.Rsrv4_Park_Disp_Rq_Pr2 = 0u;
      park_disp_rq_ar2.PARK_Hitch_Instrct_Disp_Rq = 0u;
      park_disp_rq_ar2.PARK_Hitch_KinkAngl = 0u;
      park_disp_rq_ar2.PARK_Hitch_KinkAngl_Rng = 0u;
      park_disp_rq_ar2.PARK_Pilot_Md = 0u;

       /*call SERL FUNC for this PDU*/
        Cfg_Park_Disp_Rq_AR2.e2e_serl_func(&park_disp_rq_ar2, park_disp_rq_SerDataDEData);

       /*Calculate E2E header*/
        E2E_EB_P02Protect(&Cfg_Park_Disp_Rq_AR2,&park_disp_rq_ar2_sender_state, park_disp_rq_SerDataDEData);

       /* fill CAN buffer with CRC & SQC correctly before sending */
       /* CRC & SQC left at bytes 0 & 1 to match FlexRay PDU format */
       /* buff elements other than CRC & SQC are left as 0 */
       PduInfoPtr->SduDataPtr[0] = (u8)park_disp_rq_SerDataDEData[0];
       PduInfoPtr->SduDataPtr[1] |= (u8)(park_disp_rq_SerDataDEData[1] & 0x0F);

}

static void send_RemPark_Rq_AR2_pdu(PduInfoType *PduInfoPtr)
{
    RemPark_Rq_Pr2 rempark_rq_pr2;
    u8             rempark_rq_SerDataDEData[(32u/8u)];
    u8 i = 0;

    for (i=0;i<8;i++)
    {
        PduInfoPtr->SduDataPtr[i] = 0;
    }

    memset(&rempark_rq_pr2, 0, sizeof(RemPark_Rq_Pr2));


    PduInfoPtr->SduDataPtr[2] = (1u) << 5; /* EIS_RemPark_Rq stubbed as per parkman */

      /* perform PDU filling for Park_Disp_Rq_AR2 */
      rempark_rq_pr2.KG_RemPark_Rq = 0u;
      rempark_rq_pr2.RemPark_VehLk_Rq = 0u;
      rempark_rq_pr2.RemPark_Actv = 0u;
      rempark_rq_pr2.TurnInd_Rq_RemPark = 0u;
      rempark_rq_pr2.EIS_RemPark_Rq = 1u; /* stubbed as per parkman */
      rempark_rq_pr2.RemPark_Auth_Rq = 0u;
      rempark_rq_pr2.Rsrv3_RemPark_Rq_Pr2 = 0u;

       /*call SERL FUNC for this PDU*/
        Cfg_RemPark_Rq_Pr2.e2e_serl_func(&rempark_rq_pr2 , rempark_rq_SerDataDEData);

        /*Calculate E2E header*/
        E2E_EB_P02Protect(&Cfg_RemPark_Rq_Pr2,&rempark_rq_pr2_sender_state, rempark_rq_SerDataDEData);

       /* fill CAN buffer with CRC & SQC correctly before sending */
       PduInfoPtr->SduDataPtr[0] = (u8)rempark_rq_SerDataDEData[0];
       PduInfoPtr->SduDataPtr[1] |= (u8)(rempark_rq_SerDataDEData[1] & 0x0F);

}

static void send_Parkwarn_AR2_pdu(PduInfoType *PduInfoPtr)
{
    u8 tmp_u8;
    u8 frontSpeak;
    u8 rearSpeak;
    u8 i = 0;

    for (i=0;i<8;i++)
    {
        PduInfoPtr->SduDataPtr[i] = 0;
    }

      /* read values of front & rear speakers */

      PLATFORM_ReadFrontSpeaker(&frontSpeak);
	  PLATFORM_ReadRearSpeaker(&rearSpeak);

      /* take the more critical speaker ctrl value among front & rear, and route it to front speaker */
       if((frontSpeak) <= (rearSpeak))
       {
         tmp_u8 = frontSpeak;
       }
       else
       {
         tmp_u8 = rearSpeak;
       }

       /* convert speaker ctrl value from
                           255: no sound,
                           0: continues tone
          to:
                           0: off
                           1-14: INT
                           15: continuous */

       tmp_u8 = (u8)(~tmp_u8); /* flip range, 0 is now off, 255 is continuous */

       tmp_u8 = (u8)(tmp_u8 / 15u); /* divide by 15 (max value) to convert range to 0-15 */

       /* clamp speaker ctrl value to 15 */
       if (tmp_u8 > 15u)
       {
         tmp_u8 = 15u;
       }
       else
       {
         /* do nothing, already in range */
       }

       if(tmp_u8 == 15u)
       {
           PduInfoPtr->SduDataPtr[7] |= (1u) << 6; /* acoustic warning park on */
           PduInfoPtr->SduDataPtr[6] |= 1u; /* ParkSwLED_Rq is cont (1u) */
       }
       else if(tmp_u8 != 0u)
       {
           PduInfoPtr->SduDataPtr[7] |= (1u) << 6; /* acoustic warning park on */
           PduInfoPtr->SduDataPtr[6] |= 2u; /* ParkSwLED_Rq is blinking (2u) */
       }
       else
       {
           PduInfoPtr->SduDataPtr[7] |= (0u) << 6; /* acoustic warning park on */
           PduInfoPtr->SduDataPtr[6] |= 0u; /* ParkSwLED_Rq is blinking (2u) */
       }

       /* send F2 frequency, default volume & warning sound front*/
       PduInfoPtr->SduDataPtr[6] |= (u8)((tmp_u8 & 0x0F) << 2); /* warning sound front */
       PduInfoPtr->SduDataPtr[2] |= (u8)(1u); /* volume front */
       PduInfoPtr->SduDataPtr[2] |= (u8)(0u) << 7; /* front freq */

       /* send F2 frequency, default volume & warning sound rear*/
       PduInfoPtr->SduDataPtr[7] |= (u8)(tmp_u8 & 0x0F); /* warning sound rear */
       PduInfoPtr->SduDataPtr[5] |= (u8)(1u); /* volume rear */
       PduInfoPtr->SduDataPtr[5] |= (u8)(0u) << 7; /* rear freq */

}

static void send_Park_set_stat_AR2_pdu(PduInfoType *PduInfoPtr)
{
    u8 tmp_u8;
    u8 frontSpeak;
    u8 rearSpeak;
    u8 i = 0;

    for (i=0;i<8;i++)
    {
        PduInfoPtr->SduDataPtr[i] = 0;
    }

        /* read values of front & rear speakers */

        PLATFORM_ReadFrontSpeaker(&frontSpeak);
    	PLATFORM_ReadRearSpeaker(&rearSpeak);

        /* take the more critical speaker ctrl value among front & rear, and route it to front speaker */
         if((frontSpeak) <= (rearSpeak))
         {
           tmp_u8 = frontSpeak;
         }
         else
         {
           tmp_u8 = rearSpeak;
         }

         /* convert speaker ctrl value from
                             255: no sound,
                             0: continues tone
            to:
                             0: off
                             1-14: INT
                             15: continuous */

         tmp_u8 = (u8)(~tmp_u8); /* flip range, 0 is now off, 255 is continuous */

         tmp_u8 = (u8)(tmp_u8 / 15u); /* divide by 15 (max value) to convert range to 0-15 */

         /* clamp speaker ctrl value to 15 */
         if (tmp_u8 > 15u)
         {
           tmp_u8 = 15u;
         }
         else
         {
           /* do nothing, already in range */
         }

         /* prepare data to be sent on Park_Set_Stat CAN message */

         /* start filling buff according to Park_Set_Stat CAN message structure */
         PduInfoPtr->SduDataPtr[0] |= 0x2; /* front freq = 2 */
         PduInfoPtr->SduDataPtr[1] |= 0x2; /* front volume = 2 */

          /* rear freq = 2 , where is rear freq ??????*/
         PduInfoPtr->SduDataPtr[1] |= (0x2) << 4; /* rear volume = 2 */

         PduInfoPtr->SduDataPtr[2] |= 0x2; /* front sonar = 2 */
         PduInfoPtr->SduDataPtr[2] |= (0x2) << 2; /* rear sonar = 2 */
         PduInfoPtr->SduDataPtr[3] |= 0x2; /* brkmd_stat = 2 */


         if(tmp_u8 != 0u)
         {
             PduInfoPtr->SduDataPtr[4] |= ((1u) << 4); /* attenuat_rq = 1 */
             PduInfoPtr->SduDataPtr[2] |= ((1u) << 6); /* attenuatMd_stat = 1 */
             PduInfoPtr->SduDataPtr[3] |= ((2u) << 6); /* RevGrMd_Stat = 2 */
         }
         else
         {
             PduInfoPtr->SduDataPtr[4] |= ((0u) << 4); /* attenuat_rq = 0 */
             PduInfoPtr->SduDataPtr[2] |= ((1u) << 6); /* attenuatMd_stat = 1 */
             PduInfoPtr->SduDataPtr[3] |= ((2u) << 6); /* RevGrMd_Stat = 2 */
         }

}
/******************************************************************************/
/*                  Definition of exported module functions                   */
/******************************************************************************/

/**
 * void COMH_Init(void)
 *
 * Initialization of COMH, should be initialized with CAN initialization
 *
 */
void COMH_Init(void)
{

    /* initialize can data buffer */
    memset(&st_comh_buffer_data, 0, sizeof(st_comh_buffer_data));


// UNUSED    st_comh_p2_steering_msg_cnt = 0;

    st_comh_ext_warning_direction = CAPP_DD_UNKNOWN;
}



/**
 * void COMH_CanTask(void)
 *
 * Initialization of COMH, should be initialized with CAN initialization
 *
 * CURRENTLY NO CODE IMPLEMENTED IN THIS FUNCTION
 *
 */
/******************************************************************************/
void COMH_CanTask(void)
{
}

/**
 * void COMH_Cyclic20ms(void)
 *
 * cyclic 10ms Task
 *
 */
void COMH_Cyclic10ms(void)
{

}


/**
 * void COMH_Cyclic20ms(void)
 *
 * cyclic 20ms Task
 *
 */
void COMH_Cyclic20ms(const struct COMH_input_S *input)
{
    encode_updated_rx_pdus();

    EvaluateButtonsState();
    CalculateVehicleStandstill();

    transmit_tx_pdus();
}




/**
 * u16 COMH_GetWheelCircumference(void)
 * gets wheel circumference
 *
 * \return Radumfang aus Botschaft mKombi_3 in mm (0..4096mm)
 */
u16 COMH_GetWheelCircumference(void)
{
  return st_comh_buffer_data.wheel_circumference_raw_data;
}


void COMH_Clear_BLE_MSGS(void)
{
    st_comh_buffer_data.ble_msg.BLE_ID_From = 0;
    st_comh_buffer_data.ble_msg.BLE_MsgCounter=0;
    st_comh_buffer_data.ble_msg.SP_CurrentState=0;
    st_comh_buffer_data.ble_msg.SP_DeadmanConclusion=0;
    st_comh_buffer_data.ble_msg.SP_DeadmanX=0;
    st_comh_buffer_data.ble_msg.SP_DeadmanY=0;
    st_comh_buffer_data.ble_msg.SP_NewManeuverRequest=0;
    st_comh_buffer_data.ble_msg.SP_RiskAcknowledgment=0;
    st_comh_buffer_data.ble_msg.SP_SmartphoneConnected=0;
    st_comh_buffer_data.ble_msg.SP_UserAbortRequest=0;
    st_comh_buffer_data.ble_msg.park_MsgType=0;

}

/**
 * u8 COMH_GetImpulseWheelNumberOfTeeth(void)
 *
 * \return Zaehnezahl der Impulsraeder aus Botschaft mBremse_2
 */
u8 COMH_GetImpulseWheelNumberOfTeeth(void)
{
    return st_comh_buffer_data.impulsewheel_number_of_teeth;
}




/**
 * bool_T COMH_IsVehicleStandstill(void)
 *
 * Provides the information if a vehicle standstill is recognised.
 *
 * \return If a vehicle standstill is recognised (TRUE) or not (FALSE)
 */
bool_T COMH_IsVehicleStandstill(void)
{
  return st_comh_buffer_data.is_vehicle_standstill;
}


/******************************************************************************/
/**
 * Provides the vehicle position if EXTERNAL_ODOMETRY is used
 * \param[out] p_ext_pos   pointer to the external position data
 *
 * \return E_OK if returned data is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetExternalPosition(struct DAPM_external_position_S* p_ext_pos)
{
    /* In PQ36 car plattform the information is not available (=> only implemented in testenvironment) */
    si32 temp_si32;
    u16  temp_u16;
    si16 temp_si16;

    p_ext_pos->timestamp_2us = (st_comh_buffer_data.ext_veh_position_timestamp_2us >> 8) & 0x00FFFFFFUL;

    /* raw-data: */
    /*   u16 ext_veh_angle_raw_data:      1bit = 1/100 degree (-327,68 .. 327,67degree) */
    /*   u16 ext_veh_x_pos_raw_data:      1bit = 1 mm (-32768 .. 32767mm) */
    /*   u16 ext_veh_y_pos_raw_data:      1bit = 1 mm (-32768 .. 32767mm) */
    temp_u16 = st_comh_buffer_data.ext_veh_angle_raw_data;
    if (temp_u16 > 32767)
    {
        temp_u16 = (~temp_u16) + 1;
        temp_si16 = (-1) * temp_u16;
    }
    else
    {
        temp_si16 = (si16)temp_u16;
    }
    p_ext_pos->theta_u16f = Conv100thDeg2U16f(temp_si16);

    temp_u16 = st_comh_buffer_data.ext_veh_x_pos_raw_data;
    if (temp_u16 > 32767)
    {
        temp_u16 = (~temp_u16) + 1;
        temp_si32 = (-1) * temp_u16;
    }
    else
    {
        temp_si32 = (si32)temp_u16;
    }
    p_ext_pos->x_mm = temp_si32 * 100;

    temp_u16 = st_comh_buffer_data.ext_veh_y_pos_raw_data;
    if (temp_u16 > 32767)
    {
        temp_u16 = (~temp_u16) + 1;
        temp_si32 = (-1) * temp_u16;
    }
    else
    {
        temp_si32 = (si32)temp_u16;
    }
    p_ext_pos->y_mm = temp_si32 * 100;

    return E_OK;
}

/**
 * u8 COMH_GetStatusEps(void)
 *
 * \return EPS status
 */
u8 COMH_GetStatusEps(void)
{
  return st_comh_buffer_data.status_eps_raw_data;
}



/**
 * void COMH_GetSteeringAngleAndSign(u16 *angle, u8 *sign)
 *
 * \param[in/out] angle: steering angle, 1 Bit = 0.15 degree
 * \param[in/out] sign : 0 positive, 1 negative
 */
void COMH_GetSteeringAngleAndSign(u16 *angle, u8 *sign)
{
  /* Gueltiger Lenkwinkel */
  *angle = st_comh_buffer_data.steering_angle_raw_data;
  *sign  = st_comh_buffer_data.steering_angle_sign_raw_data;
}



/**
 * u8 COMH_GetPlaTerminationOfEps(void)
 *
 * \return
 */
u8 COMH_GetPlaTerminationOfEps(void)
{
  return st_comh_buffer_data.pla_termination_raw_data;
}



/**
 * bool_T COMH_GetReverseGear(void)
 *
 * \return TRUE, if reverse gear is on, FALSE otherwise
 */
bool_T COMH_GetReverseGear(void)
{
    enum CAPP_gear_lever_position_E gear_pos;
    COMH_GetGearLeverPosition(&gear_pos);
    /* Rueckwaertsgang eingelegt ? */
    return (gear_pos == CAPP_GLP_POSITION_R);
}




/**
 * u16 COMH_GetSpeedRawData(void)
 * Get the vehicle speed of mBremse_1 (raw data from CAN)
 * \return speed raw data, resolution is 100th km/h
 */
u16 COMH_GetSpeedRawData(void)
{
  return st_comh_buffer_data.wheel_speed_raw_data;
}



/**
 * u8 COMH_GetSpeedBackupValueActive(void)
 *
 * \return if there is is given back an alternative speed value instead of the
 * real velocity
 */
u8 COMH_GetSpeedBackupValueActive(void)
{
    return st_comh_buffer_data.wheel_speed_backup_value_active;
}



/**
 * u8 COMH_GetAsrRequest(void)
 *
 * \return 0 if there is not ASR request, 1 if there is an ASR request
 */
u8 COMH_GetAsrRequest(void)
{
    return st_comh_buffer_data.asr_request_raw_data;
}



/**
 * u8 COMH_GetMsrRequest(void)
 *
 * \return 0 if there is no MSR request, 1 if there is a MSR request
 */
u8 COMH_GetMsrRequest(void)
{
    return st_comh_buffer_data.msr_request_raw_data;
}



/**
 * u8 COMH_GetEdsIntervention(void)
 *
 * \return 0 if there is no EDS intervention, 1 otherwise
 */
u8 COMH_GetEdsIntervention(void)
{
    return st_comh_buffer_data.eds_intervention_raw_data;
}



/**
 * u8 COMH_GetAbsIntervention(void)
 * returns if there is and ABS intervention
 * \return 1 if there is an intervention, 0 otherwise
 */
u8 COMH_GetAbsIntervention(void)
{
    return st_comh_buffer_data.abs_intervention_raw_data;
}



/**
 * u8 COMH_GetEspIntervention(void)
 *
 * \return 1 if there is an ESP Intervention, 0 if there is no intervention
 */
u8 COMH_GetEspIntervention(void)
{
    return st_comh_buffer_data.esp_intervention_raw_data;

}



/**
 * u8 COMH_GetEspSystemStatusError(void)
 *
 * Returns if the ESP-SystemStatus reports an error
 * \return 0: no error, != 0 ESP Systemstatus reports an error
 */
u8 COMH_GetEspSystemStatusError(void)
{
    return st_comh_buffer_data.esp_system_status_raw_data;
}



/**
 * u8 COMH_GetEspAsrDisabled(void)
 * Returns if the ASR is disabled by driver
 *
 * \return 0 if ASR is enabled, != 0 if ASR is disabled
 */
u8 COMH_GetEspAsrDisabled(void)
{
    return st_comh_buffer_data.esp_asr_passive_raw_data;
}


/**
 * u16 COMH_GetWheelImpulsesFrontLeft(void)
 *
 * \return the wheelimulses of the front left wheel
 */
u16 COMH_GetWheelImpulsesFrontLeft(void)
{
    /* return wheel impulse for front right */
    return st_comh_buffer_data.wheel_impulses.wheel_impulses_fl;
}



/**
 * u16 COMH_GetWheelImpulsesFrontRight(void)
 *
 * \return the wheelimulses of the front right wheel
 *
 * \description
 * Returns the wheelimulses of the front right wheel
 */
u16 COMH_GetWheelImpulsesFrontRight(void)
{
    /* return wheel impulse for front right */
    return st_comh_buffer_data.wheel_impulses.wheel_impulses_fr;
}



/**
 * u16 COMH_GetWheelImpulsesRearLeft(void)
 *
 * \return Wheel Impulses Rear Left
 *
 * \description
 * Returns the wheelimulses of the rear left wheel
 */
u16 COMH_GetWheelImpulsesRearLeft(void)
{
    /* return wheel impulse for rear left */
    return st_comh_buffer_data.wheel_impulses.wheel_impulses_rl;
}




/**
 * u16 COMH_GetWheelImpulsesRearRight(void)
 *
 * \return Returns the wheelimulses of the rear right wheel
 */
u16 COMH_GetWheelImpulsesRearRight(void)
{
    /* return wheel impulse for rear right */
    return st_comh_buffer_data.wheel_impulses.wheel_impulses_rr;

}

u8 COMH_GetWheelImpulseMsgCounter(u8 *msg_counter)
{
    *msg_counter = st_comh_buffer_data.wheel_impulses.msg_count;

    return 0;
}

/**
 * u8 COMH_GetKl15(void)
 * returns if Kl15 is on/off
 *
 * \return 0 - KL15 off, 1 - KL15 on
 */
KL15_state_E COMH_GetKl15(void)
{
    KL15_state_E ret;

#ifdef COMH_DEBUG_DISABLE_DIAG
    /* Fuer Debugging KL15-Bit immer 1 */
    ret = 1
#endif

    if (st_comh_buffer_data.ignition_on == FALSE)
    {
      ret = KL15_OFF;
    }
    else
    {
      ret = KL15_ON;
    }

    return ret;

}

/**
 * u8 COMH_GetStateBrakeEcu(void)
 * Returns the actual state of the brake ecu (handshake with APP ecu for
 * activation of the active braking control)
 *
 * \return the state of the brake ecu (0..15dez)
 */
u8 COMH_GetStateBrakeEcu(void)
{
  return st_comh_buffer_data.brake_ecu_state;
}



/**
 * u8 COMH_GetAppBrakeAbortReason(void)
 * Returns the abort reason if the brake ecu aborts an active braking control
 *
 * \return the abort reason of the brake ecu (0..7dez)
 */
u8 COMH_GetAppBrakeAbortReason(void)
{
  return st_comh_buffer_data.brake_ecu_abort_reason;
}



/**
 * bool_T COMH_IsLongAccValueValid(void)
 *
 * returns, if the value of the can signal "long acceleration" is valid for use
 *
 * \return TRUE  - signal is valid for use, FALSE if signal is not valid
 * (contains the error-value or Qualibit invalid for a specified time)
 */
bool_T COMH_IsLongAccValueValid(void)
{
  bool_T ret;
  //u8     temp_invalid_time_10ms = DRDH_GetLAccInvalidTime10Ms(); //TODO-JB: ??!? thats an parameter to check if signal is valid. But why from DRDH???
#define LACC_INVALID_TIME_10MS_DEF 8  /* default: 80ms */
  u8     temp_invalid_time_10ms = LACC_INVALID_TIME_10MS_DEF;

  if ( (st_comh_buffer_data.long_acc_qb_err_count > temp_invalid_time_10ms)
       || (st_comh_buffer_data.long_acc_qb_count > temp_invalid_time_10ms)
       || (st_comh_buffer_data.long_acc_err_count > temp_invalid_time_10ms) )
  {
    ret = FALSE;
  }
  else
  {
    ret = TRUE;
  }

  return ret;
}

/******************************************************************************/
/*   COMH GEN3 INTERFACE                                                      */
/******************************************************************************/

/*******************************/
/*   COMH_ULS                  */
/*******************************/
/******************************************************************************/
/**
 * Provides the moment the driver applies on the steering wheel.
 * \param[out] steering_wheel_moment  st.-wheel-moment in 0.01Nm / bit
 * \param[out] time_stamp             receive-timestamp of st.-wheel-moment in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetSteeringMoment(si16* steering_wheel_moment, u32* time_stamp)
{
    *steering_wheel_moment = (si16)(st_comh_buffer_data.steering_moment_raw_data);
    /* handle sign */
    if (st_comh_buffer_data.steering_moment_sign_raw_data != 0)
    {
        (*steering_wheel_moment) *= -1;
    }
    return E_OK;
}

#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides if the ESP is disabled by driver. This might justify an abort of an automatic steering.
 * \param[out] is_esp_asr_disabled   TRUE if ESP/... is disabled, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetEspDisabled(bool_T* is_esp_asr_disabled)
{
    *is_esp_asr_disabled = (bool_T)st_comh_buffer_data.esp_asr_passive_raw_data;
    return E_OK;
}
#endif // NOT FOR DAIMLER

Std_ReturnType COMH_IsSideDoorOpen(bool_T* p_is_side_door_open)
{
    bool_T temp_door_open = FALSE;
    *p_is_side_door_open = temp_door_open;
    return E_OK;
}

Std_ReturnType COMH_IsTrailerPresent(bool_T* p_is_trailer_present)
{
    bool_T temp_door_open = FALSE;
    *p_is_trailer_present = temp_door_open;
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the state of the right sliding door.
 * The sliding door - when fully opened - covers the side sensor. The measurements
 * of that sensor can then be evaluated accordingly.
 * \param[out] is_sliding_door_open   TRUE if door is open, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_IsSlidingDoorRightOpen(bool_T* is_sliding_door_open)
{
    bool_T temp_door_open = FALSE;
#ifdef ERROR_CAN_SLIDING_DOOR_NOSIGNAL
    bool_T temp_is_error_active = FALSE;

    if (ERRH_GetErrorStatus(ERROR_CAN_GATE_KOMF_1_NOSIGNAL) == ERRH_STATIC)
    {
        temp_is_error_active = TRUE;
    }
    else
    {
        if (ERRH_GetErrorStatus(ERROR_CAN_SLIDING_DOOR_NOSIGNAL) == ERRH_STATIC)
        {
            temp_is_error_active = TRUE;
        }
    }
    if ( (CONF_IsSlidingDoorInstalled() != FALSE) &&
         (!temp_is_error_active) )
    {
        temp_door_open = st_comh_buffer_data.sliding_door_open_right;
    }
#endif
    *is_sliding_door_open = temp_door_open;

    return E_OK;
}

/******************************************************************************/
/**
 * Provides the state of the left sliding door.
 * The sliding door - when fully opened - covers the side sensor. The measurements
 * of that sensor can then be evaluated accordingly.
 * \param[out] is_sliding_door_open   TRUE if door is open, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_IsSlidingDoorLeftOpen(bool_T* is_sliding_door_open)
{
    bool_T temp_door_open = FALSE;
#ifdef ERROR_CAN_SLIDING_DOOR_NOSIGNAL
    bool_T temp_is_error_active = FALSE;

    if (ERRH_GetErrorStatus(ERROR_CAN_GATE_KOMF_1_NOSIGNAL) == ERRH_STATIC)
    {
        temp_is_error_active = TRUE;
    }
    else
    {
        if (ERRH_GetErrorStatus(ERROR_CAN_SLIDING_DOOR_NOSIGNAL) == ERRH_STATIC)
        {
            temp_is_error_active = TRUE;
        }
    }

    if ( (CONF_IsSlidingDoorInstalled() != FALSE) &&
         (!temp_is_error_active) )
    {
        temp_door_open = st_comh_buffer_data.sliding_door_open_left;
    }
#endif
    *is_sliding_door_open = temp_door_open;

    return E_OK;
}

/******************************************************************************/
/**
 * Returns the info if the brake detects a vehicle standstill
 * \param[out] is_brake_ecu_standstill   TRUE if standstill detected, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_IsBrakeEcuStandstill(bool_T* is_brake_ecu_standstill)
{
    *is_brake_ecu_standstill = (bool_T)st_comh_buffer_data.brake_is_standstill;
    return E_OK;
}


/******************************************************************************/
/**
 * Provides if there is any intervention for the brake system. This can be a
 * combination of signals.
 * This is usually needed to abort the (semi-)automatic parking maneuver.
 * \param[out] is_brake_intervention   TRUE if a intervention of the brake takes place or
 *                                     if the brake system is disabled, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetBrakeIntervention(bool_T*  is_brake_intervention)
{
    bool_T temp_brake_intervention = FALSE;

    if ((st_comh_buffer_data.asr_request_raw_data != 0) ||
        (st_comh_buffer_data.msr_request_raw_data != 0) ||
        (st_comh_buffer_data.eds_intervention_raw_data != 0) ||
        (st_comh_buffer_data.abs_intervention_raw_data != 0) ||
        (st_comh_buffer_data.esp_intervention_raw_data != 0) ||
        (st_comh_buffer_data.esp_asr_passive_raw_data != 0))
    {
        temp_brake_intervention = TRUE;
    }
    *is_brake_intervention = temp_brake_intervention;
    return E_OK;
}


/*******************************/
/*   COMH_COMMON               */
/*******************************/
/******************************************************************************/
/**
 * Provides the wheel impulse of the selected car wheel. These are needed for
 * odometry and standstill detection.
 * \param[out] wheel_impulses  current value of the wheel impulses of the selected wheel
 * \param[out] time_stamp      receive-timestamp of wheel impulses in 2us / bit
 *                             (only one timestamp for all wheel impulses)
 * \param[in]  wheel           selected wheel to get wheel impulses from
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetWheelImpulse(u16* wheel_impulses, u32* time_stamp, enum DAPM_wheel_E wheel)
{
    Std_ReturnType ret_value;
    bool_T         temp_valid;


    switch (wheel)
    {
    case DAPM_WHEEL_FL:
        *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us_left;
        *wheel_impulses = st_comh_buffer_data.wheel_impulses.wheel_impulses_fl;
        temp_valid = st_comh_buffer_data.wheel_impulses.valid_fl;
        break;
    case DAPM_WHEEL_FR:
        *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us_right;
        *wheel_impulses = st_comh_buffer_data.wheel_impulses.wheel_impulses_fr;
        temp_valid = st_comh_buffer_data.wheel_impulses.valid_fr;
        break;
    case DAPM_WHEEL_RL:
        *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us_left;
        *wheel_impulses = st_comh_buffer_data.wheel_impulses.wheel_impulses_rl;
        temp_valid = st_comh_buffer_data.wheel_impulses.valid_rl;
        break;
    case DAPM_WHEEL_RR:
        *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us_right;
        *wheel_impulses = st_comh_buffer_data.wheel_impulses.wheel_impulses_rr;
        temp_valid = st_comh_buffer_data.wheel_impulses.valid_rr;
        break;
    default:
        /* not existing/not allowed */
        _ASSERT(FALSE);
        *wheel_impulses = 0;
        temp_valid = FALSE;
        break;
    }
    /* set validity of signal */
    if (temp_valid == FALSE)
    {
        ret_value = E_NOT_OK;
    }
    else
    {
        ret_value = E_OK;
    }
    return ret_value;
}

/******************************************************************************/
/**
 * Provides the longitudinal acceleration of the vehicle.
 * \param[out] longitudinal_acceleration  long. acc. in 1mm/s^2 / bit
 * \param[out] time_stamp                 receive-timestamp of long. acc. in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetLongAcceleration(si16* longitudinal_acceleration, u32* time_stamp)
{
    si16 temp_long_acc;

    /* phys-data: */
    /*   si16 long_acc_phys_data:  1Bit = 0.03125m/s^2 = 1/32 m/s^2 */
    /*   valid values: 1 .. 1022 */
    /*   data  -511 -> -15.96875 m/s^2 */
    /*   data     0 ->   0       m/s^2 */
    /*   data   510 ->  15.9375  m/s^2 */
    /* raw-data (CAN-Signal) */
    /*   raw-data 1    -> -15.96875 m/s^2 */
    /*   raw-data 512  ->   0       m/s^2 */
    /*   raw-data 1022 ->  15.9375  m/s^2 */

    /* TODO-KI: implement own timestamp */
    *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us;

    temp_long_acc = st_comh_buffer_data.long_acc_phys_data;
    /* convert 1/32m/s^2 into 1mm/s^2 */
    temp_long_acc = (si16)(((si32)temp_long_acc * 1000L) / 32L);

    *longitudinal_acceleration = 0; //TODO-M.Amr Need to know signal on chassis
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the vehicle speed. typically a average value of all 4 wheels provided
 * by the brake ecu.
 * \param[out] speed         vehicle speed in 0.01km/h / bit
 * \param[out] time_stamp    receive-timestamp of vehicle speed in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetSpeed(u16* speed, u32* time_stamp)
{
    *time_stamp = 0; /* TODO-KI: implement timestamp */
    *speed = st_comh_buffer_data.wheel_speed_raw_data;
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the abstracted gear lever position received from CAN. Gear lever
 */
Std_ReturnType COMH_GetGearLeverPosition(enum CAPP_gear_lever_position_E* gear_lever_pos)
{

    /* following values are modified for Daimler BR213 */
    switch (st_comh_buffer_data.is_gear)
    {
    case 5:
        *gear_lever_pos = CAPP_GLP_POSITION_D;
        break;
    case 6:
        *gear_lever_pos = CAPP_GLP_POSITION_N;
        break;
    case 7:
        *gear_lever_pos = CAPP_GLP_POSITION_R;
        break;
    case 8:
        *gear_lever_pos = CAPP_GLP_POSITION_P;
        break;
    default:
        *gear_lever_pos = CAPP_GLP_UNKNOWN;
        break;
    }

    return E_OK;
}
/******************************************************************************/
/**
 * Provides the angle of the steering wheel. For some american cars it may be up
 * to 1000° in both directions.
 * \param[out] steering_wheel_angle  st.-wheel-angle in 0.1deg / bit
 *                                   Positive value: steering left, negative value: steering right.
 * \param[out] time_stamp            receive-timestamp of st.-wheel-angle in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetSteeringWheelAngle(si16* steering_wheel_angle, u32* time_stamp)
{
    *time_stamp = st_comh_buffer_data.st_wheel_angle_time_2us;
    /* raw-data: */
    /*   u16 steering_angle_raw_data:      1 Bit = 0.15 degree */
    /*   u8  steering_angle_sign_raw_data: 0 = positive, 1 = negative */

    *steering_wheel_angle = st_comh_buffer_data.steering_angle;
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the steering angle (wheel angle) of the rear wheels of the vehicle.
 * \note This value may not be available in every car.
 * \param[out] wheel_angle  wheel angle of the rear wheels in u16f / bit
 *                          (u16f = u16-format = 360 deg / 2^16 = roughly 0.005 deg)
 *                          Positive value: steering left, negative value: steering right.
 * \param[out] time_stamp   receive-timestamp of wheel_angle in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetRearWheelSteerAngle(si16* wheel_angle, u32* time_stamp)
{
    /* In PQ36 car plattform the information is not available */
    *time_stamp = 0;
    *wheel_angle = 0;
    return E_NOT_OK;
}

/******************************************************************************/
/**
 * Provides the driving direction for a selected wheel. This is needed for driving
 * direction determination of the car and wheel circumference calculation.
 * \param[out] wheel_driving_dir   driving direction of the selected wheel
 * \param[out] time_stamp          receive-timestamp of wheel-dr.-dir in 2us / bit
 *                                 (only one timestamp for all directions)
 * \param[in]  wheel               selected wheel to get driving-dir from
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetWheelDriveDir(enum CAPP_dr_dir_E* wheel_driving_dir,
                                     u32* time_stamp,
                                     enum DAPM_wheel_E wheel)
{
    enum COMH_wheel_info_E temp_comh_wheel_info;

    /* raw-data: */
    /*   enum COMH_wheel_info_E wheel_info_xx_raw_data */

    switch (wheel)
    {
    case DAPM_WHEEL_FL:
        /* is in the same CAN message as the wheel-impulses => same timestamp as wheel-impulses */
        *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us_left;
        temp_comh_wheel_info = st_comh_buffer_data.wheel_info_fl_raw_data;
        break;
    case DAPM_WHEEL_FR:
        *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us_right;
        temp_comh_wheel_info = st_comh_buffer_data.wheel_info_fr_raw_data;
        break;
    case DAPM_WHEEL_RL:
        *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us_left;
        temp_comh_wheel_info = st_comh_buffer_data.wheel_info_rl_raw_data;
        break;
    case DAPM_WHEEL_RR:
        *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us_right;
        temp_comh_wheel_info = st_comh_buffer_data.wheel_info_rr_raw_data;
        break;
    default:
        /* not existing/not allowed */
        _ASSERT(FALSE);
        temp_comh_wheel_info = COMH_WHEEL_INFO_INVALID;
        break;
    }

     /* convert comh-wheelinfo into DAPM-wheel driving dir */
     switch (temp_comh_wheel_info)
     {
     case COMH_WHEEL_INFO_FORWARD:
         *wheel_driving_dir = CAPP_DD_FORWARD;
         break;
     case COMH_WHEEL_INFO_BACKWARD:
         *wheel_driving_dir = CAPP_DD_BACKWARD;
         break;
     case COMH_WHEEL_INFO_STANDSTILL:
     case COMH_WHEEL_INFO_INVALID:
     default:
         *wheel_driving_dir = CAPP_DD_UNKNOWN;
         break;
     }

    return E_OK;
}

/******************************************************************************/
/**
 * Provides the speed of the selected wheel. This is needed
 * for wheel circumference calculation.
 * \param[out] wheel_speed   wheel speed of the selected wheel in 0.01m/s / bit
 * \param[out] time_stamp    receive-timestamp of wheel speed in 2us / bit
 *                           (only one timestamp for all wheel speeds)
 * \param[in]  wheel         selected wheel to get wheel speed from
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetWheelSpeed(si16* wheel_speed, u32* time_stamp, enum DAPM_wheel_E wheel)
{
    Std_ReturnType ret_val = E_NOT_OK;

    /* raw-data: */
    /*   u16 wheel_speed_xx:      1 Bit = 0.01km/h */
    switch (wheel)
    {
    case DAPM_WHEEL_FL:
        *time_stamp = st_comh_buffer_data.wheel_speeds.timestamp_2us_left;
        *wheel_speed = st_comh_buffer_data.wheel_speeds.wheel_speed_fl;
        ret_val = E_OK;
        break;
    case DAPM_WHEEL_FR:
        *time_stamp = st_comh_buffer_data.wheel_speeds.timestamp_2us_right;
        *wheel_speed = st_comh_buffer_data.wheel_speeds.wheel_speed_fr;
        ret_val = E_OK;
        break;
    case DAPM_WHEEL_RL:
        *time_stamp = st_comh_buffer_data.wheel_speeds.timestamp_2us_left;
        *wheel_speed = st_comh_buffer_data.wheel_speeds.wheel_speed_rl;
        ret_val = E_OK;
        break;
    case DAPM_WHEEL_RR:
        *time_stamp = st_comh_buffer_data.wheel_speeds.timestamp_2us_right;
        *wheel_speed = st_comh_buffer_data.wheel_speeds.wheel_speed_rr;
        ret_val = E_OK;
        break;
    default:
        /* not existing/not allowed */
        _ASSERT(FALSE);
        *wheel_speed = 0;
        break;
    }
    return ret_val;  //TODO-M.Amr Need to know correct signal
}

/******************************************************************************/
/**
 * Provides the yaw speed (Giergeschwindigkeit) of the vehicle
 * \param[out] yaw_speed   yaw speed of the vehicle in u16f/s / bit
 *                         (u16f = u16-format = 360 deg / 2^16 = roughly 0.005 deg)
 * \param[out] time_stamp  receive-timestamp of yaw speed in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetYawSpeed(si16* yaw_speed, u32* time_stamp)
{
    *time_stamp = st_comh_buffer_data.yaw_speed_timestamp_2us;

    #ifdef DAIMLER_BR_213_PLATFORM
    /* already saved with proper resolution */
    *yaw_speed = (si16)(st_comh_buffer_data.yaw_speed_raw_data);

    #else
    /* raw-data: */
    /*   u16 yaw_speed_raw_data:      1bit = 1/100 degree/s */
    /*   u8  yaw_speed_sign_raw_data: 0 - positive (turn left), 1 - negative (turn right) */
   /* convert 1/100 deg/s into u16f/s */
    *yaw_speed = (si16)(((u32)st_comh_buffer_data.yaw_speed_raw_data * (u32)2048) / (u32)1125);
    #endif

    /* handle sign TODO-KI: is this correct for U16-format ? */
    if (st_comh_buffer_data.yaw_speed_sign_raw_data != 0)
    {
        (*yaw_speed) *= -1;
    }
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the lateral acceleration of the vehicle.
 * \param[out] lateral_acceleration   lateral acc. in 1mm/s^2 / bit
 * \param[out] time_stamp             receive-timestamp in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetLateralAcceleration(si16* lateral_acceleration, u32* time_stamp)
{
    /* TODO-KI: implement own timestamp */
    *time_stamp = st_comh_buffer_data.wheel_impulses.timestamp_2us;

    /* raw-data: */
    /*   si16 lateral_acceleration: 1bit = 0.01m/s^2 */
    /*                              positive: turn left, negative: turn right */

    *lateral_acceleration = 0;
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the outside temperature of the vehicle. This temperature is needed for
 * compensation different ultrasonic echo runtimes. The temperature is filtered so
 * that the actual outside temperature is measured and not the heat of the engine.
 * The function returns the last value received. If none has been received yet,
 * it returns a default-value.
 * \param[out] outside_temperature   temperature in 1degree celcius / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetOutsideTemperature(si8* outside_temperature)
{
    Std_ReturnType ret_val = E_NOT_OK;

    *outside_temperature = st_comh_buffer_data.temp_filtered_raw_data;

    if (st_comh_buffer_data.temperature_received == TRUE)
    {
        ret_val = E_OK;
    }

    return ret_val;
}

/******************************************************************************/
/**
 * Provides the if the left blinker is enabled. This information can be used to
 * determine on which side the driver wants to automatically park.
 * \param[out] is_blinker_left_enabled   TRUE if blinker is enabled, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetBlinkerLeft(bool_T* is_blinker_left_enabled)
{
    *is_blinker_left_enabled = st_comh_buffer_data.blinker_left_raw_data;
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the if the right blinker is enabled. This information can be used to
 * determine on which side the driver wants to automatically park.
 * \param[out] is_blinker_right_enabled   TRUE if blinker is enabled, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetBlinkerRight(bool_T* is_blinker_right_enabled)
{
    *is_blinker_right_enabled = st_comh_buffer_data.blinker_right_raw_data;
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the standstill-information (if there are no wheel ticks available)
 * This is only needed and possible for UPA_MID systems
 * \param[out] p_is_standstill   TRUE if vehicle is in standstill, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetVehicleStandstill(bool_T* p_is_standstill)
{
    *p_is_standstill = FALSE;
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the external warning direction.
 * If return value is CAPP_DD_UNKNOWN internal warning direction calculation is used.
 * \param[out] warning_direction   external calculated warning direction
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetExtWarningDirection(enum CAPP_dr_dir_E *warning_direction)
{
    *warning_direction = st_comh_ext_warning_direction;
    return E_OK;
}
void COMH_SendDevIdData(struct DIDH_dev_id_S *dev_id_data)
{
}

void COMH_SendNotification(bool_T start_of_transmission)
{
    /* nothing to do on start or end of transmission of Dev-IDs */
}


bool_T COMH_GetPAM_req_buttonRelease(void)
{
    //TODO: USE NEW INTERFACE ONE HMIM IS UPDATED
  static u8 laststate=0;
  bool_T released = false;
  if(st_comh_buffer_data.switch_options_button!=laststate  && st_comh_buffer_data.switch_options_button == 0 )
  {
      released = true;
  }
  laststate=st_comh_buffer_data.switch_options_button;
  return released ;
}

bool_T COMH_GetP4U_req_buttonRelease(void)
{
  static u8 laststate=0;
  bool_T released = false;
  if(COMH_GetP4USlotsOptionsButtonValue() != laststate)
  {
	  released = true;
  }
  laststate=COMH_GetP4USlotsOptionsButtonValue();
  return released ;
}
Std_ReturnType COMH_ActivateEpb(void){

}


/*****************************************************/
/***          CRC Checksumme calculation           ***/
/*****************************************************/
static u8 calc_crc (u8 *buff, u8 start, u8 end)
{
  u8 crc = 0xff;
  u8 i;
  u8 j;
  for (j= start; j<=end; j++)
  {
    crc ^= buff[j];
    i = 8;
    do
    {
      if(crc & 0x80)
          crc = (crc << 1)^0x1d;
      else
          crc = (crc << 1);
    } while(--i);

  }
  return(crc ^ 0xff);
}


static u8 ComputeSetPkm(struct COMH_input_S *input)
{
    static u8 old_pkm_state = 1;
    u8 set_pkm = 1;

    switch (st_comh_buffer_data.status_eps_moment_raw_data)
    {
    case 0:  /* Default value             */
    case 1:  /* Init State               */
        set_pkm = 2; /* PKM ready */
        break;
    case 2:  /*  */
        if( (COMH_GetSpeedKph() <= 18) ||
            (   (COMH_GetSpeedKph() <= 20) &&
                (3 == old_pkm_state)
            )
        )
        {
            old_pkm_state = 3;
            set_pkm = 3; /* PKM_steer */
        } else
        {
            old_pkm_state = 2;
            set_pkm = 2; /* PKM ready */
        }
        break;
    case 3:  /* Modus 1         */
    case 4:  /* EPS actuates requested and rack force   */
    case 5:  /* Modus 3 */
        if( (COMH_GetSpeedKph() <= 18) ||
            (   (COMH_GetSpeedKph() <= 20) &&
                (3 == old_pkm_state)
            )
        )
        {
            old_pkm_state = 3;
            set_pkm = 3; /* PKM Steer */
        }
        else
        {
            old_pkm_state = 2;
            set_pkm = 2; /* PKM Ready */
        }
        break;
    case 6:  /* safe state         */
    case 7:  /* cancel state       */
        set_pkm = 2; /* PKM ready */
        break;
    case 8:  /* ramping down 1         */
    case 9:  /* ramping down 1        */
        set_pkm = 1; /* Interface OFF/Init */
        break;
    default:
        _ASSERT(FALSE);
        break;
    }

    if (input->parking_active)
    {
        set_pkm = 2; /* PKM Ready */
    }

    return set_pkm;
}

//  NB bits
//  |  Bit Index
//  |  |  bit mask
//  |  |  |                       Value
//  1  0  0000_0000 0000_0001     1 = 2^0
//  2  1  0000_0000 0000_0010     2 = 2^1
//  3  2  0000_0000 0000_0100     4 = 2^2
//  4  3  0000_0000 0000_1000     8 = 2^3
//  5  4  0000_0000 0001_0000    16 = 2^4
//  6  5  0000_0000 0010_0000    32 = 2^5
//  7  6  0000_0000 0100_0000    64 = 2^6
//  8  7  0000_0000 1000_0000   128 = 2^7
//  9  8  0000_0001 0000_0000   256 = 2^8
// 10  9  0000_0010 0000_0000   512 = 2^9
// 11 10  0000_0100 0000_0000  1024 = 2^10
// 12 11  0000_1000 0000_0000  2048 = 2^11
// 13 12  0001_0000 0000_0000  4096 = 2^12
// 14 13  0010_0000 0000_0000  8192 = 2^13
// 15 14  0100_0000 0000_0000 16384 = 2^14
// 16 15  1000_0000 0000_0000 32768 = 2^15


/* Function GetHaptTorq12bits
 * - gets the Hapt torque and
 * - converts it in a 12 bit value to be send on the can
 *   according the DAIMLER Interface
 *
 * The output depends on the pkm_state
 */
static u16 GetHaptTorq12bits(struct COMH_input_S *input, u8 pkm_state)
{
    si16 add_steer_torque_100thnm;
    u16  add_steer_torque_100thnm_offset;
    u16  add_steer_torque_PMW_12bits;

    // Not yet in steering mode, set added torque to zero
    if ((3 != pkm_state) ||
        (   (3 == pkm_state) &&
            (2 == st_comh_buffer_data.status_eps_moment_raw_data)
        )
    )
    {
        add_steer_torque_100thnm = 0;
    }
    else
    {
        /* get torque from DAPM in 100thNm */
        add_steer_torque_100thnm = input->filt_add_torque_100th_nm;

        /* Torque is between -3 Nm and 3Nm */
        _ASSERT(add_steer_torque_100thnm<=300 && add_steer_torque_100thnm>=-300);

        /* optional catch: limit values that are not in range */
        if (add_steer_torque_100thnm < -300)
        {
            add_steer_torque_100thnm = -300;
        }
        else if (add_steer_torque_100thnm > 300)
        {
            add_steer_torque_100thnm = 300;
        }
        else
        {
            /* value is in the correct range */
        }
    }

    /* Convert this torque in scaled data on 12 bits unsigned with offset
     * factor : 2048 <-> 3 Nm
     * Offset : 2048 <-> 3 Nm
     *           0 -> -3.00 Nm
     * 2^11 = 2048 ->  0.00 Nm
     * 2^12 = 4096 ->  3.00 Nm (unreachable) */
#define TORQ_OFFSET_12_BITS 2048
#define TORQ_OFFSET_100THNM 300
#define TORQ_FACTOR_12_BITS 2048
#define TORQ_FACTOR_100THNM 300
#define TORQ_MAX_ALLOWED_12_BITS 4094 //why not 4095 ? used for dignostic = error ?

    add_steer_torque_100thnm_offset = add_steer_torque_100thnm + TORQ_OFFSET_100THNM;

    add_steer_torque_PMW_12bits = MTLI_MulDivSatU16(
        add_steer_torque_100thnm_offset,
        TORQ_FACTOR_12_BITS,
        TORQ_FACTOR_100THNM);

    /* limit the max value */
    if (add_steer_torque_PMW_12bits > TORQ_MAX_ALLOWED_12_BITS)
    {
        add_steer_torque_PMW_12bits = TORQ_MAX_ALLOWED_12_BITS;
    }

    return add_steer_torque_PMW_12bits;
}







/**
 * u8 COMH_GetStatusEpsTimestamp(void)
 *
 * \return EPS status timestamp
 */
u8 COMH_GetStatusEpsTimestamp(void)
{
  return st_comh_buffer_data.status_eps_timestamp;
}

/**
 * u16 COMH_GetSpeedKph(void)
 *
 * gets the vehicle speed of mBremse_1, resoultion is km/h
 *
 * \return speed, res. is km/h
 */
u16 COMH_GetSpeedKph(void)
{
  u16 ret_value = st_comh_buffer_data.wheel_speed_raw_data /100u ;

  return ret_value;
}

Std_ReturnType COMH_GetSpeed_th100kph(u16* vehicle_speed)
{
    * vehicle_speed = st_comh_buffer_data.wheel_speed_raw_data ;

    return E_OK;
}


/******************************************************************************/
/** bool_T COMH_IsReadyForGearChange(void)
 * returns if the Autopark gear change is ready
 *
 * \staticdata   gear_change_ready
 *
 * \description
 * returns if the Autopark gear change is ready
 */
bool_T COMH_IsReadyForGearChange(void)
{
   return st_comh_buffer_data.gear_change_ready;
}


/******************************************************************************/
/**
 * si16 COMH_GetSignedSteeringAngle10thDegree(void)
 *
 * \return calculates steering angle from mLenkhilfe_3 message,
 *         resoultion 1Bit = 1/10 degree
 */
si16 COMH_GetSignedSteeringAngle10thDegree(void)
{
  /* Gueltiger Lenkwinkel */
  u16 temp_angle;
  u8 temp_sign;

  COMH_GetSteeringAngleAndSign(&temp_angle, &temp_sign);
  st_comh_buffer_data.steering_angle = (si16)temp_angle / 3;
  st_comh_buffer_data.steering_angle *= 2;
  st_comh_buffer_data.steering_angle *= temp_sign;

  return st_comh_buffer_data.steering_angle;
}

Std_ReturnType COMH_GetGearLeverPosition_RevGr_Engg(enum CAPP_gear_lever_position_E* gear_lever_pos)
{
    if (st_comh_buffer_data.reverse_gear_raw_data)
    {
        *gear_lever_pos = CAPP_GLP_POSITION_R;
    } else
    {
        *gear_lever_pos = CAPP_GLP_POSITION_D;
    }
    return E_OK;
}

/******************************************************************************/
/*                                                                            */
/*  Description:                                                              */
/*    Calculate a CRC8 according to "A Painless Guide to CRC Error Detection  */
/*    Algorithms".                                                            */
/*                                                                            */
/*    Used polynomial:                                                        */
/*      J1850: 0x11d (x^8 + x^4 + x^3 + x^2 + 1)                              */
/*      "C2":  0x12f (x^8 + x^5 + x^3 + x^2 + x + 1)                          */
/*                                                                            */
/*  Input parameter:                                                          */
/*    id : 16 bit ID for calculation into start or end value                  */
/*             id = CHECKSUM_ENDVALUE for the CAN Message                     */
/*    data_ptr : Pointer to start address of data block                       */
/*    length : Size of data block to be computed                              */
/*                                                                            */
/*  Output parameter:                                                         */
/*    return : The calculated CRC value (8 bit)                               */
/*                                                                            */
/******************************************************************************/

u8 COMH_CalculateCrc8(u16 id, const u8 *data_ptr, u8 length)
{
  u8 crc_value;

  /* Table for CRC-8 calculation with "C2" polynomial */
  static const u8 crc8_table[256] =
    {
      0x00u, 0x2fu, 0x5eu, 0x71u, 0xbcu, 0x93u, 0xe2u, 0xcdu,
      0x57u, 0x78u, 0x09u, 0x26u, 0xebu, 0xc4u, 0xb5u, 0x9au,
      0xaeu, 0x81u, 0xf0u, 0xdfu, 0x12u, 0x3du, 0x4cu, 0x63u,
      0xf9u, 0xd6u, 0xa7u, 0x88u, 0x45u, 0x6au, 0x1bu, 0x34u,
      0x73u, 0x5cu, 0x2du, 0x02u, 0xcfu, 0xe0u, 0x91u, 0xbeu,
      0x24u, 0x0bu, 0x7au, 0x55u, 0x98u, 0xb7u, 0xc6u, 0xe9u,
      0xddu, 0xf2u, 0x83u, 0xacu, 0x61u, 0x4eu, 0x3fu, 0x10u,
      0x8au, 0xa5u, 0xd4u, 0xfbu, 0x36u, 0x19u, 0x68u, 0x47u,
      0xe6u, 0xc9u, 0xb8u, 0x97u, 0x5au, 0x75u, 0x04u, 0x2bu,
      0xb1u, 0x9eu, 0xefu, 0xc0u, 0x0du, 0x22u, 0x53u, 0x7cu,
      0x48u, 0x67u, 0x16u, 0x39u, 0xf4u, 0xdbu, 0xaau, 0x85u,
      0x1fu, 0x30u, 0x41u, 0x6eu, 0xa3u, 0x8cu, 0xfdu, 0xd2u,
      0x95u, 0xbau, 0xcbu, 0xe4u, 0x29u, 0x06u, 0x77u, 0x58u,
      0xc2u, 0xedu, 0x9cu, 0xb3u, 0x7eu, 0x51u, 0x20u, 0x0fu,
      0x3bu, 0x14u, 0x65u, 0x4au, 0x87u, 0xa8u, 0xd9u, 0xf6u,
      0x6cu, 0x43u, 0x32u, 0x1du, 0xd0u, 0xffu, 0x8eu, 0xa1u,
      0xe3u, 0xccu, 0xbdu, 0x92u, 0x5fu, 0x70u, 0x01u, 0x2eu,
      0xb4u, 0x9bu, 0xeau, 0xc5u, 0x08u, 0x27u, 0x56u, 0x79u,
      0x4du, 0x62u, 0x13u, 0x3cu, 0xf1u, 0xdeu, 0xafu, 0x80u,
      0x1au, 0x35u, 0x44u, 0x6bu, 0xa6u, 0x89u, 0xf8u, 0xd7u,
      0x90u, 0xbfu, 0xceu, 0xe1u, 0x2cu, 0x03u, 0x72u, 0x5du,
      0xc7u, 0xe8u, 0x99u, 0xb6u, 0x7bu, 0x54u, 0x25u, 0x0au,
      0x3eu, 0x11u, 0x60u, 0x4fu, 0x82u, 0xadu, 0xdcu, 0xf3u,
      0x69u, 0x46u, 0x37u, 0x18u, 0xd5u, 0xfau, 0x8bu, 0xa4u,
      0x05u, 0x2au, 0x5bu, 0x74u, 0xb9u, 0x96u, 0xe7u, 0xc8u,
      0x52u, 0x7du, 0x0cu, 0x23u, 0xeeu, 0xc1u, 0xb0u, 0x9fu,
      0xabu, 0x84u, 0xf5u, 0xdau, 0x17u, 0x38u, 0x49u, 0x66u,
      0xfcu, 0xd3u, 0xa2u, 0x8du, 0x40u, 0x6fu, 0x1eu, 0x31u,
      0x76u, 0x59u, 0x28u, 0x07u, 0xcau, 0xe5u, 0x94u, 0xbbu,
      0x21u, 0x0eu, 0x7fu, 0x50u, 0x9du, 0xb2u, 0xc3u, 0xecu,
      0xd8u, 0xf7u, 0x86u, 0xa9u, 0x64u, 0x4bu, 0x3au, 0x15u,
      0x8fu, 0xa0u, 0xd1u, 0xfeu, 0x33u, 0x1cu, 0x6du, 0x42u
    };

#if (CRC8_STARTVALUE_FROM_ID == CRC8_ON)
  crc_value = crc8_table[GetLowByte(id)];
  crc_value = crc8_table[crc_value ^ GetHighByte16(id)];
#else
  crc_value = 0xffu;
#endif

  /* PRQA S 3440 2 */ /* << MISRA Rule 12.13: [1] Communication is time critical. \
     Needed for performance reason. >> */
  while (length-- > 0)
    {
      /* PRQA S 489 1 */ /* << MISRA Rule 17.4: see [1] >> */
      crc_value = crc8_table[crc_value ^ *(data_ptr++)];
    }

#if (CRC8_STARTVALUE_FROM_ID == CRC8_OFF)
  crc_value = crc8_table[crc_value ^ (GetHighByte16(id) ^ GetLowByte(id))];
#endif

#if (CRC8_COMPLEMENT_RESULT == CRC8_ON)
  crc_value = (u8) (~crc_value);

#endif
  return (crc_value);
}

/* Fills the output parameter with the front left speed in 100mps resolution*/
Std_ReturnType COMH_GetWheelSpeedfl_th100mps(si16* wheel_speed_fl)
{

    *wheel_speed_fl = st_comh_buffer_data.wheel_speeds.wheel_speed_fl;
    return E_OK;
}

Std_ReturnType COMH_GetWheelSpeedrl_th100mps(si16* wheel_speed_rl)
{

   *wheel_speed_rl = st_comh_buffer_data.wheel_speeds.wheel_speed_rl;
   return E_OK;
}

Std_ReturnType COMH_GetWheelSpeedfr_th100mps(si16* wheel_speed_fr)
{

    *wheel_speed_fr = st_comh_buffer_data.wheel_speeds.wheel_speed_fr;
    return E_OK;
}

Std_ReturnType COMH_GetWheelSpeedrr_th100mps(si16* wheel_speed_rr)
{

   *wheel_speed_rr = st_comh_buffer_data.wheel_speeds.wheel_speed_rr;
   return E_OK;
}
enum button_state_E  COMH_GetUpaButtonState(void)
{
  return upa_button_state;
}

/*Returns the value of the actual pressed brake torque value*/
void
COMH_GetBrakePressure(si16 * data)
{
    *data = (st_comh_buffer_data.brake_pressure_raw_data );
}

enu_break_pedal
COMH_GetBreakPedal(void)
{
    static enu_break_pedal BrakePedalState = false;
    if(st_comh_buffer_data.driver_brake_pressure_raw_data >= BRK_PEDAL_PRSD_THRSHLD)
    {
        BrakePedalState = PRESSED;
    }
    else if(st_comh_buffer_data.driver_brake_pressure_raw_data <= BRK_PEDAL_N_PRSD_THRSHLD)
    {
        BrakePedalState = RELEASED;
    }
    else
    {
        /* do not update BrakePedalPressed */
    }
    return BrakePedalState;
}

Std_ReturnType COMH_GetGPSdata(si32* longitude , si32* latitude)

{
    /* Adding offset to the value received to compensate for the sign that is not handled by the can converter*/
    *latitude  = st_comh_buffer_data.gps_latitude  + LATITUDE_OFFSET;
    *longitude = st_comh_buffer_data.gps_longitude + LONGITUDE_OFFSET;

    return E_OK;
}

/**
 * bool_T COMH_GetPark4uButtonState(void)
 *
 * \return the Park4U Button state, 0: not pressed, != 0: pressed
 */
enum button_state_E  COMH_GetPark4uButtonState(void)
{
    return park4u_button_state;
}


enum button_state_E  COMH_GetConfirmationButtonState(void)
{
    return confirmation_button_state;
}

enum button_state_E  COMH_GetFuncBarBackButtonState(void)
{
    return funcbar_back_button_state;
}

u8 COMH_GetRequestedView(void)
{
    return (COMH_GetViewsOptionsButtonValue() + 1);
}

u8 COMH_GetHZTROptionsButtonValue(void)
{
    return Calculate_Module_Option(COMH_HZTR,HZTR_OPTIONS_COUNT);
}

u8 COMH_GetBDAOptionsButtonValue(void)
{
    return Calculate_Module_Option(COMH_BDA,BDA_OPTIONS_COUNT);
}
u8 COMH_GetViewsOptionsButtonValue(void)
{
    return Calculate_Module_Option(COMH_VIEWS,VIEWS_OPTIONS_COUNT);
}
u8 COMH_GetP4USlotsOptionsButtonValue(void)
{
    return Calculate_Module_Option(COMH_SLOT_SELLECTION,P4U_SLOTS_OPTIONS_COUNT);
}

u8 COMH_ChooseManeuverButtonValue(void)
{
     return Calculate_Module_Option(COMH_MANEUVER_SELLECTION,MANEUVER_OPTIONS_COUNT);
}
u8 COMH_GetFuncBarOptionsButtonValue(void)
{
    return Calculate_Module_Option(COMH_FUNCBAR,funcbar_options_count);
}
void COMH_SetFuncBarOptionsButtonValue(u8 funcbar_views)
{
    set_flag = TRUE;
    funcbar_options_count = funcbar_views;
}
u8 Calculate_Module_Option(enum COMH_options_button_modules_E current_module,u8 max_options_number)
{
    static u8 switch_options_init_value = 0;

    if((module_using_options_button != current_module) || (set_flag == TRUE) )
    {
        set_flag = FALSE;
        module_using_options_button = current_module;
        switch_options_init_value = (u8) switch_options_button_click_count;

        if(switch_options_button_click_count < switch_options_init_value)
        {
            switch_options_button_click_count = switch_options_init_value;
        }
    }
    return ((switch_options_button_click_count - switch_options_init_value) % max_options_number);
}

enum button_state_E  COMH_GetGpsButtonState(void)
{
    return gps_button_state;
}

enum button_state_E  COMH_GetViewButtonState(void)
{
    return view_button_state;
}

enum button_state_E  COMH_GetP4uHomeTrainingButtonState(void)
{
    return p4uHomeTraining_button_state;
}

enum button_state_E  COMH_GetAbortButtonState(void)
{
    return abort_button_state;
}

enum button_state_E  COMH_GetBdaReplayButtonState(void)
{
    return bdaReplay_button_state;
}

enum button_state_E  COMH_GetDeadManButtonState(void)
{
    return ((st_comh_buffer_data.dead_man_button == 1 ) ? COMH_BUTTON_PRESSED : COMH_BUTTON_RELEASED);
}

enum button_state_E  COMH_GetSwitchOptionsButtonState(void)
{
    return switch_button_state;
}

static void EvaluateButtonsState(void)
{
    EvaluateUpaButtonState();
    EvaluatePark4uButtonState();
    EvaluateConfirmationButtonState();
    EvaluateGpsButtonState();
    EvaluateP4uHomeTrainingButtonState();
    EvaluateBdaReplayButtonState();
    EvaluateSwitchButtonState();
    EvaluateAbortButtonState();
    EvaluateViewButtonState();
    EvaluateBackButtonState();
    SwitchOptionsButtonCounter();
}

static void EvaluateUpaButtonState(void)
{
    static struct button_properties_S upa_button = {COMH_BUTTON_RELEASED,&upa_button_state,0,FALSE,0};
    upa_button.button_signal = st_comh_buffer_data.upa_button;
    EvaluateButtonState(&upa_button);
}
static void EvaluatePark4uButtonState(void)
{
    static struct button_properties_S p4ubutton = {COMH_BUTTON_RELEASED,&park4u_button_state,0,FALSE,0};
    p4ubutton.button_signal = st_comh_buffer_data.park4u_button;
    EvaluateButtonState(&p4ubutton);
}
static void EvaluateConfirmationButtonState(void)
{
    static struct button_properties_S confirmation_button = {COMH_BUTTON_RELEASED,&confirmation_button_state,0,FALSE,0};
    confirmation_button.button_signal = st_comh_buffer_data.confirmation_button;
    EvaluateButtonState(&confirmation_button);
}
static void EvaluateGpsButtonState(void)
{
    static struct button_properties_S GPS_button = {COMH_BUTTON_RELEASED,&gps_button_state,0,FALSE,0};
    GPS_button.button_signal = st_comh_buffer_data.GPS_button;
    EvaluateButtonState(&GPS_button);
}

static void EvaluateP4uHomeTrainingButtonState(void)
{
    static struct button_properties_S p4uhome_training_button = {COMH_BUTTON_RELEASED,&p4uHomeTraining_button_state,0,FALSE,0};
    p4uhome_training_button.button_signal = st_comh_buffer_data.p4uhome_training_button;
    EvaluateButtonState(&p4uhome_training_button);
}

static void EvaluateAbortButtonState(void)
{
    static struct button_properties_S abort_button = {COMH_BUTTON_RELEASED,&abort_button_state,0,FALSE,0};
    abort_button.button_signal = st_comh_buffer_data.abort_button;
    EvaluateButtonState(&abort_button);
}

static void EvaluateViewButtonState(void)
{
    static struct button_properties_S view_button = {COMH_BUTTON_RELEASED,&view_button_state,0,TRUE,0};
    view_button.button_signal = st_comh_buffer_data.view_button;
    EvaluateButtonState(&view_button);
}
static void EvaluateBdaReplayButtonState(void)
{
    static struct button_properties_S bda_replay_button = {COMH_BUTTON_RELEASED,&bdaReplay_button_state,0,FALSE,0};
    bda_replay_button.button_signal = st_comh_buffer_data.bda_replay_button;
    EvaluateButtonState(&bda_replay_button);
}

static void EvaluateSwitchButtonState(void)
{
    static struct button_properties_S switch_button = {COMH_BUTTON_RELEASED,&switch_button_state,0,FALSE,0};
    switch_button.button_signal = st_comh_buffer_data.switch_options_button;
    EvaluateButtonState(&switch_button);
}

static void EvaluateButtonState(struct button_properties_S* button_properties)
{

    if((button_properties->is_toggle_button)) /*if the button toggle it's state with each click*/
    {
        /* On a negative edge toggle the button */
        if( (button_properties->button_signal == FALSE) && (button_properties->pre_state == TRUE ))
        {
            /*toggle the button*/
            *(button_properties->button_state) = ((*(button_properties->button_state) == COMH_BUTTON_PRESSED) ? COMH_BUTTON_RELEASED : COMH_BUTTON_PRESSED);
        }
    }
    else
    {
        /* pressed_cycle_counter is incremented as long as the button_signal is true */
        if(button_properties->button_signal == TRUE)
        {
            button_properties->pressed_cycle_counter++;
        }

        /* if the button is pressed for more than one second, the button state will be set to
         *  COMH_BUTTON_LONG_PRESSED and will be released once the signal become low
         *  50 * 20ms = 1Sec */
        if(button_properties->pressed_cycle_counter >= (1000 / COMH_CYCLIC_TASK_MS))
        {
            *(button_properties->button_state) = COMH_BUTTON_LONG_PRESSED;
        }


        /*if there is a negative edge (current state = low , previous state = high) and the button is not LONG_PRESSED */
        if((button_properties->button_signal == FALSE) && (button_properties->pre_state == TRUE )
                && (*(button_properties->button_state) != COMH_BUTTON_LONG_PRESSED))
        {
            /* In this case: not Toggle and not Long_pressed so the button should be COMH_BUTTON_PRESSED, and will be released after 2 cycles */
            *(button_properties->button_state) = COMH_BUTTON_PRESSED;

            button_properties->pressed_cycle_counter = 0;

        }

        /*if the button is COMH_BUTTON_LONG_PRESSED and the signal become low, release the
         *  button_state to COMH_BUTTON_RELEASED */
        if((button_properties->button_signal == FALSE) && (*(button_properties->button_state) == COMH_BUTTON_LONG_PRESSED) )
        {
            *(button_properties->button_state) = COMH_BUTTON_RELEASED;

            button_properties->pressed_cycle_counter = 0;
        }

        /* Release the button if it is Pressed after 2 cycles */
        if(  *(button_properties->button_state) == COMH_BUTTON_PRESSED )
        {
            /*start cycles count*/
            button_properties->pressed_cycle_counter++;

            if(button_properties->pressed_cycle_counter > BUTTON_PRESSED_CYCLES_COUNT )
            {
                button_properties->pressed_cycle_counter = 0;

                *(button_properties->button_state) = COMH_BUTTON_RELEASED;
            }
        }
    }

    button_properties->pre_state = button_properties->button_signal ;
}

static void EvaluateBackButtonState(void)
{
    static bool_T pre_state = FALSE;

    if((st_comh_buffer_data.switch_options_button == TRUE))
    {
        switch_back_button_click_count++;
        if(switch_back_button_click_count>COMH_BACK_BUTTON_TIMER_VALUE)
        {
            funcbar_back_button_state = COMH_BUTTON_PRESSED;
        }
    }

    if(st_comh_buffer_data.switch_options_button == FALSE)
    {
        funcbar_back_button_state = COMH_BUTTON_RELEASED;
        switch_back_button_click_count = 0;
    }


    pre_state = st_comh_buffer_data.switch_options_button;
}
static void SwitchOptionsButtonCounter(void)
{
    static bool_T pre_state = FALSE;
    if((st_comh_buffer_data.switch_options_button == TRUE) && (pre_state == COMH_BUTTON_RELEASED ))
    {
        switch_options_button_click_count++;
    }
    pre_state = st_comh_buffer_data.switch_options_button;
}

/**
 * u8 COMH_GetBLEMessage(void)
 *
 * \return 0 if there is no MSR request, 1 if there is a MSR request
 */
void COMH_GetBLEMessage(ble_msg_T* p)
{
    *p = st_comh_buffer_data.ble_msg;
}


/**
 * u8 COMH_IsSeatBeltRemoved(void)
 *
 */
bool COMH_IsSeatBeltRemoved(void)
{
    return FALSE;
}
/**
 * u8 COMH_IsDoorOpened(void)
 *
 */
bool COMH_IsDoorOpened(void)
{
    return FALSE;
}

/******************************************************************************/
/*              Function definitions for Daimler BR213 prototype              */
/******************************************************************************/

/**
 * esp_brake_state_E COMH_GetEspBrakeState(void)
 *
 * \return st_comh_buffer_data.esp_brake_state_park
 */
 esp_brake_state_E COMH_GetEspBrakeState(void)
 {
   return st_comh_buffer_data.esp_brake_state_park;
 }

/**
 * esp_intervention_E   COMH_GetESPIntervention
 *
 * \return ESP_INTERVENTION_ACTIVE if
 *         st_comh_buffer_data.ESP_Oprtn_Stat_AS_V2 is not 0 nor 8
 */
 esp_intervention_E   COMH_GetESPIntervention(void)
 {
   esp_intervention_E temp;
   if (  st_comh_buffer_data.ESP_Oprtn_Stat_AS_V2 != 0u
      && st_comh_buffer_data.ESP_Oprtn_Stat_AS_V2 != 8u)
   {
      temp = ESP_INTERVENTION_ACTIVE;
   }
   else
   {
      temp = ESP_INTERVENTION_INACTIVE;
   }
   return temp;
 }

 /**
 * engine_running_state_E  COMH_GetEngineState
 *
 * \return ENGINE_RUNNING if
 *         st_comh_buffer_data.engine_state is not 1
 */
 engine_running_state_E  COMH_GetEngineState(void)
 {
   engine_running_state_E temp;
   if (st_comh_buffer_data.engine_state == 1u)
   {
      temp = ENGINE_RUNNING;
   }
   else
   {
      temp = ENGINE_NOT_RUNNING;
   }
   return temp;
 }

 /**
 * ldc_afterrun_ctrl_E     COMH_GetEspAfterrunControl
 *
 * \return st_comh_buffer_data.esp_ldc_afterrun_ctrl
 */
 ldc_afterrun_ctrl_E     COMH_GetEspAfterrunControl(void)
 {
   return st_comh_buffer_data.esp_ldc_afterrun_ctrl;
 }


 /**
 * Std_ReturnType     COMH_GetEpbStatus(void)
 *
 * \return 1 if epb is engaged, 0 otherwise
 */
 Std_ReturnType COMH_GetEpbStatus(epb_status_T* epb_status)
 {
     Std_ReturnType ret = E_OK;

     if(st_comh_buffer_data.epb_engaged)
     {
         * epb_status = EPB_IS_Tightened;
     }
     else
     {
         * epb_status = EPB_IS_Released;
     }

   return ret;
 }

 /**
 * gear_request_E          COMH_GetCarCurrentGear(void)
 *
 * \return           returns the currently engaged gear
 */
 gear_position_E          COMH_GetCarCurrentGear(void)
 {
   return st_comh_buffer_data.current_gear;
 }


 /**
 * bool_T                  COMH_GetParkEnblStatEsp(void)
 *
 * \return           returns 1 if park is enabled
 */
 u8                    COMH_GetParkEnblStatEsp(void)
 {

   return st_comh_buffer_data.park_enbl_stat_esp;

 }

 /**
 * park_flt_stat_esp_E     COMH_GetParkFltStatEsp(void)
 *
 * \return           st_comh_buffer_data.park_flt_stat_esp;
 */
 park_flt_stat_esp_E     COMH_GetParkFltStatEsp(void)
 {
   return st_comh_buffer_data.park_flt_stat_esp;
 }

 /**
 * esp_system_state_E      COMH_GetEspSystemState(void)
 *
 * \return           st_comh_buffer_data.esp_system_state;
 */
 esp_system_state_E      COMH_GetEspSystemState(void)
 {
   return st_comh_buffer_data.esp_system_state;
 }

 si16 COMH_GetBrakePedalTorque(void)
 {
     return st_comh_buffer_data.driver_brake_pressure_raw_data;
 }

 /******************************************************************************/
 /**
  * Provides the speed of the selected wheel in rpm.
  * \param[out] wheel_speed   wheel speed of the selected wheel in 1 rpm
  * \param[out] time_stamp    receive-timestamp of wheel speed in 2us / bit
  *                           (only one timestamp for all wheel speeds)
  * \param[in]  wheel         selected wheel to get wheel speed from
  *
  * \return E_OK if value is valid, E_NOT_OK otherwise.
  */
 Std_ReturnType COMH_GetWheelSpeedRPM(si16* wheel_speed, u32* time_stamp, enum DAPM_wheel_E wheel)
 {
     si16 localSpeed;
     Std_ReturnType ret_val = E_NOT_OK;

     *time_stamp = st_comh_buffer_data.wheel_speeds.timestamp_2us;

     /* raw-data: */
     /*   u16 wheel_speed_xx:      1 Bit = 0.01km/h */
     switch (wheel)
     {
     case DAPM_WHEEL_FL:
         /* conversion from mm/s to rpm                   */
         /* Conversion provided for rpm -> mm/s :         */
         /* mm/s = (rpm * wheel_circumference(mm)) / 60.  */
         /* rpm = (mm/s / wheel_circumference(mm)) * 60   */

         localSpeed = (si16)((si32)((si32)st_comh_buffer_data.wheel_speeds.wheel_speed_fl / WHEEL_CIRCUMF_FRONT) * 60);
         *wheel_speed = localSpeed;
         ret_val = E_OK;
         break;
     case DAPM_WHEEL_FR:
         /* conversion from mm/s to rpm                   */
         /* Conversion provided for rpm -> mm/s :         */
         /* mm/s = (rpm * wheel_circumference(mm)) / 60.  */
         /* rpm = (mm/s / wheel_circumference(mm)) * 60   */

         localSpeed = (si16)((si32)((si32)st_comh_buffer_data.wheel_speeds.wheel_speed_fr / WHEEL_CIRCUMF_FRONT) * 60);
         *wheel_speed = localSpeed;
         ret_val = E_OK;
         break;
     case DAPM_WHEEL_RL:
         /* conversion from mm/s to rpm                   */
         /* Conversion provided for rpm -> mm/s :         */
         /* mm/s = (rpm * wheel_circumference(mm)) / 60.  */
         /* rpm = (mm/s / wheel_circumference(mm)) * 60   */

         localSpeed = (si16)((si32)((si32)st_comh_buffer_data.wheel_speeds.wheel_speed_rl / WHEEL_CIRCUMF_REAR) * 60);
         *wheel_speed = localSpeed;
         ret_val = E_OK;
         break;
     case DAPM_WHEEL_RR:
         /* conversion from mm/s to rpm                   */
         /* Conversion provided for rpm -> mm/s :         */
         /* mm/s = (rpm * wheel_circumference(mm)) / 60.  */
         /* rpm = (mm/s / wheel_circumference(mm)) * 60   */

         localSpeed = (si16)((si32)((si32)st_comh_buffer_data.wheel_speeds.wheel_speed_rr / WHEEL_CIRCUMF_REAR) * 60);
         *wheel_speed = localSpeed;
         ret_val = E_OK;
         break;
     default:
         /* not existing/not allowed */
         _ASSERT(FALSE);
         *wheel_speed = 0;
         break;
     }
     return ret_val;  //TODO-M.Amr Need to know correct signal
 }

 /**
  * bool_T COMH_IsVehicleDoorsClosed(void)
  *
  * Provides the information if all Important car doors are closed.
  *
  * \return If all Important car doors are closed (TRUE) or not (FALSE)
  */
 bool_T COMH_IsVehicleDoorsClosed(void)
 {
 	// TODO stubbed to TRUE, to be mapped correctly
 	return TRUE;
 }

 Std_ReturnType COMH_TxClouParkMsgCb(IPC_msg_tx_id_T msg_id, IPC_tx_msg_status_T tx_status) /* PRQA S 0850 *//* MD_MSR_19.8 */
 {
    return E_OK;
 }

/******************************************************************************
 *
 * END  Source:         COMH.c
 *
 ******************************************************************************/
