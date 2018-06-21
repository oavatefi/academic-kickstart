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

#ifdef XDAPM_PROD_P4U
#define COMH_PROD_P4U
#endif


/******************************************************************************/
/*                      Include external modul header                         */
/******************************************************************************/
/* DAPM */

#include "conf.h"

#include "p2gpaleo.h"
#include "comh_can.h"
#include "didh_typ.h"
#include "bda.h"
#include "brkh_cus.h"
#include "stmh_cus.h"
#ifdef XP4UHOME_FEATURE
#include "hztr.h"
#endif
#include "actl.h"
#include "p4u.h"
#include "hmih.h"
#include "PLATFORM_SharedVar.h"
#include "car_variants.h"

/******************************************************************************/
/*                      Include internal modul header                         */
/******************************************************************************/
#include "comh.h"
#include "E2E_platfrom.h"

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
#define GPS_OFFSET   2147483648
/* PEIKER ECU */
#define XP2GPA_CAN_ID_ATM_NM_510                       0x510
#define XP2GPA_CAN_ID_ATM_NM_12F                       0x12F
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

/**
 * macro that configures the can buffer size
 */
#define COMH_CAN_IRT_BUFFER_SIZE 5

#define COMH_P2_CAN_ID_DISPLAY               0x5d6

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

/******************************************************************************/
/*              Definition of CAN message ID's for Daimler BR213              */
/******************************************************************************/
#ifdef DAIMLER_BR_213_PLATFORM
   #define CAN_ID_VEH_SPEED_DATA_ESP       0x9E
   #define CAN_ID_PARK_BRK_RS_DATA_ESP     0x9F
   #define CAN_ID_ELECTRONIC_BRK_ENG       0xA0
   #define CAN_ID_PARK_ST_RS_EPS           0xA1
   #define CAN_ID_GR_CURRENT_GEAR_CPC      0xA2
   #define CAN_ID_STEER_REQ_PARK           0xA3
   #define CAN_ID_BRK_REQ_PARK             0xA4
   #define CAN_ID_PWRTR_STAT_CPC           0xA5
   #define CAN_ID_IGN_VEH_STAT_ESP         0xA6
   #define CAN_ID_WHL_STAT_RIGHT_ESP       0xA7
   #define CAN_ID_WHL_STAT_LEFT_ESP        0xA8
   #define CAN_ID_BRK_DATA_ESP             0xAA
   #define CAN_ID_BUTTONS_DATA_EIS         0xAB
   #define CAN_ID_TURN_INDICATORS_DATA_EIS 0xAC
   #define CAN_ID_STEERING_DATA_EPS        0xAD
   #define CAN_ID_TSL_TARGET_POS_CPC       0xAE
   #define CAN_ID_VEH_ACCEL_DATA_ESP       0xAF
   #define CAN_ID_PARK_PARKGUID_STAT_PARK  0x9D
   #define CAN_ID_PARK_SVS_CPF_Rq          0x9C
   #define CAN_ID_PARKWARN_PARK            0x9B
   #define CAN_ID_REMPARK_RQ_PARK          0x9A
   #define CAN_ID_PARK_DISP_RQ_PARK        0x99
   #define CAN_ID_VEHDYN_STAT2_ESP         0x98
   #define CAN_ID_PARK_SET_STAT_PARK       0x97
	#define CAN_ID_PLATFORM_BLE				 0x3A5

   #define WHEEL_CIRCUMF_FRONT all_car_variants[PLATFORM_CAR_VARIANT]->das_cfg.dapm_cfg.vehicle_cfg.default_wheel_circ_front_mm
   #define WHEEL_CIRCUMF_REAR all_car_variants[PLATFORM_CAR_VARIANT]->das_cfg.dapm_cfg.vehicle_cfg.default_wheel_circ_rear_mm

   #ifdef XAPPL_LOT_CNTRL_SUPP
      #define BRK_PEDAL_PRSD_THRSHLD		   3000u /* physical value, corresponds to 1000 raw value for brake torque */
   #else
      #define BRK_PEDAL_PRSD_THRSHLD		   8000u /* physical value, corresponds to about 2667 raw value for brake torque */
   #endif
   #define BRK_PEDAL_N_PRSD_THRSHLD		   1000u /* physical value, corresponds to about 333 raw value for brake torque */
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
    si16 wheel_speed_fl_raw;  /* raw value */
    si16 wheel_speed_fr_raw;  /* raw value */
    si16 wheel_speed_rl_raw;  /* raw value */
    si16 wheel_speed_rr_raw;  /* raw value */
};


struct lcomh_can_data_S
{
    ble_msg_T ble_msg;
    bool_T is_seatbelt_put;
    bool_T is_seatbelt_removed;
  /* mBremse_1 */
    u8                              abs_intervention_raw_data;      /* ABS */
    u16                             wheel_speed_raw_data;           /* Used for Daimler BR213, corresponds to VehSpeed_X. Radgeschwindigkeit */
    u32                             wheel_speed_timestamp_2us;
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
    si32                            yaw_speed_phys_data_100th;
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
    u8								is_gear; /* Used in Daimler BR213, corresponds to TSL_Posn_target, enum values to be handled in getter */

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
    si32                            steering_angle_phy_1000th;  /*pure physical value*/
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
    bool_T							bda_replay_button;
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
    si32 							gps_longitude;
    si32 							gps_latitude;

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
    float                     yaw_physical_deg_p_s;

    /* PEIKER ECU */
    si32                            gps_pos_longitude;
    si32                            gps_pos_latitude;
    uint16 gps_date_year;
    uint8  gps_date_mon;
    uint8  gps_date_day;
    uint8  gps_time_sec;
    uint8  gps_time_min;
    uint8  gps_time_hour;
    uint8  gps_accuracy_horizontal;
    uint8  gps_error_latitude_position;
    uint8  gps_error_longitude_position;
    uint16 gps_speed_horizontal;
    uint8  gps_quantity_satellite_usage;
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
static struct lcomh_can_data_S      st_comh_buffer_can_data;

/**
 *  buffer for application data
 *  this buffer includes the data, that is provided to the application
 *  the data for this buffer is copied from can_data.
 */
static struct lcomh_can_data_S      st_comh_buffer_appl_data;

/**
 *  flag that indicates, if copy from st_comh_buffer_can_data to st_comh_buffer_appl_data
 *  is in progress. If yes, all data must be saved in the irpt buffer and should be
 *  copied to can data afterwards
 */
static bool_T                       st_comh_copy_in_can_irpt;

/**
 *  buffer for can received CAN data, while data is copied (interrupt buffer)
 *  this buffer is necessary to save data, if an interrupt occurs during copying
 *  data from buffer_can_data to buffer_appl_data
 */
static struct lcomh_can_buffer_S    st_comh_buffer_can_irpt_data[COMH_CAN_IRT_BUFFER_SIZE];

/**
 * counter that indicates, with how much CAN data, the buffer is filled
 */
static u8                           st_comh_buffer_counter;

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

static bool_T						send_paravan = FALSE;
static u8							paravan_count = 0;

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
static u16 Conv100thDeg2U16f(const si16 angle_100th_deg);


static void CanSendDevId1(u8 *send_buffer);
static void CanSendDevId2(u8 *send_buffer);
static void CanSendDevId3(u8 *send_buffer);
static void CanSendDevId4(u8 *send_buffer);
static void CanSendDevId5(u8 *send_buffer);
static void CanSendDevId6(u8 *send_buffer);
static void CanSendDevId7(u8 *send_buffer);
static void CanSendDevId8(u8 *send_buffer);

static void CanSendDevId9(u8 *send_buffer);
static void CanSendDevId10(u8 *send_buffer);
static void CanSendDevId11(u8 *send_buffer);
static void CanSendDevId12(u8 *send_buffer);
static void CanSendDevId13(u8 *send_buffer);
static void CanSendDevId14(u8 *send_buffer);

static void CanSendDevId15(u8 *send_buffer);
static void CanSendDevId16(u8 *send_buffer);
static void CanSendDevId17(u8 *send_buffer);
static void CanSendDevId18(u8 *send_buffer);
static void CanSendDevId19(u8 *send_buffer);
static void CanSendDevId20(u8 *send_buffer);
static void CanSendDevId21(u8 *send_buffer);
static void CanSendDevId22(u8 *send_buffer);
static void CanSendDevId23(u8 *send_buffer);
static void CanSendDevId24(u8 *send_buffer);
static void CanSendDevId25(u8 *send_buffer);
static void CanSendDevId26(u8 *send_buffer);
static void CanSendDevId27(u8 *send_buffer);
static void CanSendDevId28(u8 *send_buffer);
static void CanSendDevId29(u8 *send_buffer);
static void CanSendDevId30(u8 *send_buffer);
static void CanSendDevId31(u8 *send_buffer);

static void COMH_SendParavanInfo(void);

#ifdef COMH_PROD_P4U
static void CanSendPlaStatus(void);
#endif /* COMH_PROD_P4U */

static void CanSendParkhilfe5(void);

static void CalculateVehicleStandstill(void);

static void CanSendLedTong(void);

static void SaveCanDataInIrptBuffer(u16 id, const u8 *p, u8 n, u8 counter, struct lcomh_can_buffer_S* buffer);
static void SaveCanDataInBuffer(u16 id, const u8 *p, u8 n, struct lcomh_can_data_S* buffer);
static void CopyIrptBufferInCanBuffer(u8 counter,
                                      const struct lcomh_can_buffer_S* buffer_source,
                                      struct lcomh_can_data_S* buffer_dest);
static void CopyCanBufferInApplBuffer(void);

#ifdef APPL_SECT_CRITICALITY_TO_CAN
static void CanSendSectorCriticality(void);
#endif

static u8 calc_crc (u8 *buff, u8 start, u8 end);

static u8 ComputeSetPkm(void);
static u16 GetHaptTorq12bits(u8 pkm_state);

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
static void CanSendbrakingMsg(void);
static void CanSendSteeringMsg(void);
static void CanSendSVSCPFRqMsg(void);
static void CanSendParkStat(void);
static void CanSendParkDispRq(void);
static void CanSendRemParkRq(void);

/* PEIKER ECU */
static void Send_ATM_NM(void);

/******************************************************************************/
/*                   Definition of local module functions                     */
/******************************************************************************/


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
 * static void CopyIrptBufferInCanBuffer(u8 counter,
 *                                       const struct lcomh_can_buffer_S* buffer_source,
 *                                       struct lcomh_can_data_S* buffer_dest)
 *
 * copies interrupt buffer in the normal CAN Buffer (if there is sth. to copy)
 *
 * @param counter           counter value = number of messages to copy
 * @param buffer_source     can buffer queue
 * @param buffer_dest       buffer destination
 */
static void CopyIrptBufferInCanBuffer(u8 counter,
                                      const struct lcomh_can_buffer_S* buffer_source,
                                      struct lcomh_can_data_S* buffer_dest)
{
    u8 temp_i;

    for(temp_i = 0; temp_i < counter; temp_i++)
    {
        SaveCanDataInBuffer(buffer_source[temp_i].id,
                            buffer_source[temp_i].p,
                            buffer_source[temp_i].n,
                            buffer_dest);
    }
}





/**
 * static SaveCanDataDuringIrpt(u16 id, const u8 *p, u8 n, u8 counter, struct lcomh_can_buffer_S buffer)
 * Saves the given CAN data with id, *p, n in the given buffer in slot counter
 *
 * \param id        CAN id
 * \param p         pointer to CAN data that should be copied
 * \param n         length of message
 * \param counter   counter, where the CAN id should be saved in
 * \param buffer    buffer in which the data should be saved in
 */
static void SaveCanDataInIrptBuffer(u16 id, const u8 *p, u8 n, u8 counter, struct lcomh_can_buffer_S* buffer)
{

    u8 temp_i;

    buffer[counter].id = id;
    buffer[counter].n = n;

    for(temp_i = 0; temp_i < n; temp_i++)
    {
        buffer[counter].p[temp_i] = p[temp_i];
    }

}


/**
 * static SaveCanDataInBuffer(u16 id, const u8 *p, u8 n, struct lcomh_can_data_S* buffer)
 *
 * \description: saves can data in the given buffer pointer
 *
 * \param id: CAN Id
 * \param p : pointer to CAN message
 * \param n : length of CAN message
 * \param buffer: buffer, where the can message should be saved
 *
 */
static void SaveCanDataInBuffer(u16 id, const u8 *p, u8 n, struct lcomh_can_data_S* buffer)
{
    u8   tmp_u8;
    u16  tmp_u16;
    si16 tmp_si16;
    u32  tmp_u32;
    sint32  tmp_s32;
    bool_T bFrAxeError = FALSE;
    bool_T bReAxeError = FALSE;

    {
    switch (id)
    {
    #ifdef DAIMLER_BR_213_PLATFORM
    case 0x40F:/*Cloud Parking GPS Time - PEIKER ECU */

        buffer->gps_time_sec = p[0];
        buffer->gps_time_min = p[1];
        buffer->gps_time_hour = p[2];
        break;

    case 0x40E:/*Cloud Parking GPS Date - PEIKER ECU */

        buffer->gps_date_day = p[0];
        buffer->gps_date_mon = p[1];
        buffer->gps_date_year = (u16) p[2];
        buffer->gps_date_year |= (u16)(p[3] << 8);
        break;
    case 0x412:/*Cloud Parking GPS Location - PEIKER ECU */

        buffer->gps_pos_longitude = (si32)p[0];
        buffer->gps_pos_longitude |= (si32)(p[1] << 8);
        buffer->gps_pos_longitude |= (si32)(p[2] << 16);
        buffer->gps_pos_longitude |= (si32)(p[3] << 24);
        buffer->gps_pos_longitude = buffer->gps_pos_longitude + GPS_OFFSET;
        buffer->gps_pos_latitude = (si32)p[4];
        buffer->gps_pos_latitude |= (si32)(p[5] << 8);
        buffer->gps_pos_latitude |= (si32)(p[6] << 16);
        buffer->gps_pos_latitude |= (si32)(p[7] << 24);
        buffer->gps_pos_latitude = buffer->gps_pos_latitude + GPS_OFFSET;

        break;
    case 0x38D:/*Cloud Parking GPS Settings - PEIKER ECU */

        buffer->gps_accuracy_horizontal = p[5] / 10; // factor is 0.1
        buffer->gps_error_latitude_position = p[2] / 10 ; // factor is 0.1
        buffer->gps_error_longitude_position = p[3] / 10; // factor is 0.1
        buffer->gps_speed_horizontal = (u16)p[0];
        buffer->gps_speed_horizontal |= (u16)(p[1] << 8);
        buffer->gps_speed_horizontal = buffer->gps_speed_horizontal / 200; // factor is 0.005
        buffer->gps_quantity_satellite_usage = (p[4] & 0xF0) >> 4;

        break;

    case CAN_ID_BRK_DATA_ESP:
         /* save brake torque in buffer */
         buffer->brake_pressure_raw_data = (si16)((((p[3] << 8) | p[2]) & 0x1FFF) * 3u); /* multiply by a factor of 3 as per FIBEX */
         buffer->driver_brake_pressure_raw_data = (si16)((((p[5] << 8) | p[4]) & 0x1FFF) * 3u); /* multiply by a factor of 3 as per FIBEX */

         /* save esp operation status in buffer */
         buffer->ESP_Oprtn_Stat_AS_V2 = (u8)(p[0] & 0x0F);
         /* save brake status in buffer */
         buffer->brake_ecu_state = (u8)((p[0] >> 4) & 0x03);
         /* save brake pedal state in buffer */
         tmp_u8 = (p[0] >> 6) & 0x03;
         if(tmp_u8 == 0u)
         {
            buffer->brake_pedal_stat = RELEASED;
         }
         else if(tmp_u8 == 1u)
         {
            buffer->brake_pedal_stat = PRESSED;
         }
         else
         {
            /* do not update brake pedal data */
         }

         /* update brake intervention values */
         buffer->AS_intervention_raw_data = (p[1] & 0x01);
         buffer->CWA_intervention_raw_data = ((p[1] >> 1) & 0x01);
         buffer->EPKB_intervention_raw_data = ((p[1] >> 2) & 0x01);
         buffer->esp_intervention_raw_data = ((p[1] >> 3) & 0x01);
         /* Save ESP system state */
         tmp_u8 = ((p[1] >> 4) & 0x07);
         switch (tmp_u8 )
         {
            case 0u:
               buffer->esp_system_state = ESP_SYSTEM_STATE_TEMPORARY_OFF;
               break;
            case 1u:
               buffer->esp_system_state = ESP_SYSTEM_STATE_NORMAL_OPERATION;
               break;
            case 2u:
               buffer->esp_system_state = ESP_SYSTEM_STATE_ERROR;
               break;
            case 3u:
               buffer->esp_system_state = ESP_SYSTEM_STATE_DIAGNOSTIC;
               break;
            case 4u:
               buffer->esp_system_state = ESP_SYSTEM_STATE_EXHAUST_EMISSION_TEST;
               break;
            case 5u:
               buffer->esp_system_state = ESP_SYSTEM_STATE_TEST_BENCH_CC_ON;
               break;
            case 6u:
               buffer->esp_system_state = ESP_SYSTEM_STATE_TEST_BENCH_CC_REGULATION_ACTIVE;
               break;
            default:
            /* do not update esp system state */
               break;
         }

         break;

   case CAN_ID_BUTTONS_DATA_EIS:
      tmp_u8 = (u8)((p[0] >> 4) & 0x03);
      if (tmp_u8 == 0u)
      {
         /* button not pressed */
         buffer->switch_options_button = FALSE;
      }
      else if (tmp_u8 == 2u)
      {
         /* button pressed */
         buffer->switch_options_button = TRUE;
      }
      else
      {
         /* do not update switch_options_button status */
      }

      /* save confirmation_button state */
      tmp_u8 = (u8)(p[0] & 0x03);
      if (tmp_u8 == 0u)
      {
         /* button not pressed */
         buffer->confirmation_button = FALSE;
      }
      else if (tmp_u8 == 1u)
      {
         /* button pressed */
         buffer->confirmation_button = TRUE;
      }
      else
      {
         /* do not update confirmation_button status */
      }
      break;

   /* save turn indicators data in CAN buffer */
   case CAN_ID_TURN_INDICATORS_DATA_EIS:
      if((p[0] & 0x07) == 2u)
      {
         /* only left turn indicator is active */
         buffer->blinker_left_raw_data = 1u;
      }
      else if((p[0] & 0x07) == 4u)
      {
         /* only right turn indicator is active */
         buffer->blinker_right_raw_data = 1u;
      }
      else
      {
         /* either both indicators active or EF is active or invalid combination, switch off both blinkers */
         buffer->blinker_left_raw_data = 0u;
         buffer->blinker_right_raw_data = 0u;
      }
      break;

   case CAN_ID_ELECTRONIC_BRK_ENG:
      /* set epn engaged to true/false */
      tmp_u8 = p[0] & 0x07;

      if(tmp_u8 == 3u)
      {
         buffer->epb_engaged = TRUE;
      }
      else
      {
         buffer->epb_engaged = FALSE;
      }
      break;

      /* set current gear (P, R, N, D) */
   case CAN_ID_GR_CURRENT_GEAR_CPC:
      tmp_u8 = (p[0] & 0x0F);
      switch(tmp_u8)
      {
         case 0u:
            buffer->current_gear = GEAR_POSITION_N;
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
            buffer->current_gear = GEAR_POSITION_D;
            break;
         case 11u:
         case 12u:
            buffer->current_gear = GEAR_POSITION_R;
            break;
         case 13u:
            buffer->current_gear = GEAR_POSITION_P;
            break;
         default:
            break;
      }
      break;

   case CAN_ID_IGN_VEH_STAT_ESP:
      /* save ignition on */
      tmp_u8 = (p[0] & 0x07);
      if ((tmp_u8 == 4u) || (tmp_u8 == 5u))
      {
        buffer->ignition_on  = TRUE;
      }
      else
      {
         buffer->ignition_on = FALSE;
      }
      break;

   case CAN_ID_PARK_BRK_RS_DATA_ESP:
      /* save park response status from esp to buffer */
      buffer->park_enbl_stat_esp = (u8)p[0];
      tmp_u8 = (p[1] & 0x1F);
      switch(tmp_u8)
      {
         case 0u:
            buffer->park_flt_stat_esp = ESP_STATE_IDLE;
            break;
         case 1u:
            buffer->park_flt_stat_esp = ESP_STATE_ESP_FLT;
            break;
         case 2u:
            buffer->park_flt_stat_esp = ESP_STATE_LCP_PSV;
            break;
         case 3u:
            buffer->park_flt_stat_esp = ESP_STATE_PM_FLT;
            break;
         case 4u:
            buffer->park_flt_stat_esp = ESP_STATE_SPCR_FLT;
            break;
         case 5u:
            buffer->park_flt_stat_esp = ESP_STATE_EPB_FLT;
            break;
         case 6u:
            buffer->park_flt_stat_esp = ESP_STATE_LDC_SATURATED;
            break;
         case 7u:
            buffer->park_flt_stat_esp = ESP_STATE_MAX_SPD;
            break;
         case 8u:
            buffer->park_flt_stat_esp = ESP_STATE_REMAIN_DIST;
            break;
         case 9u:
            buffer->park_flt_stat_esp = ESP_STATE_MAX_SLOPE;
            break;
         case 10u:
            buffer->park_flt_stat_esp = ESP_STATE_ESP_CTRL;
            break;
         case 11u:
            buffer->park_flt_stat_esp = ESP_STATE_TCM_FLT;
            break;
         case 12u:
            buffer->park_flt_stat_esp = ESP_STATE_ECM_FLT;
            break;
         case 13u:
            buffer->park_flt_stat_esp = ESP_STATE_ENG_OFF;
            break;
         case 14u:
            buffer->park_flt_stat_esp = ESP_STATE_DRV_ACTV;
            break;
         case 15u:
            buffer->park_flt_stat_esp = ESP_STATE_DRV_ABSENCE;
            break;
         case 16u:
            buffer->park_flt_stat_esp = ESP_STATE_LDC_OVERRULED;
            break;
         case 17u:
            buffer->park_flt_stat_esp = ESP_STATE_RPA_FLT;
            break;
         case 18u:
            buffer->park_flt_stat_esp = ESP_STATE_RPA_MAX_SPD;
            break;
         case 19u:
            buffer->park_flt_stat_esp = ESP_STATE_RPA_UNDO_MAX_SLOPE;
            break;
         case 20u:
            buffer->park_flt_stat_esp = ESP_STATE_RPA_MAX_SLOPE;
            break;
         default:
            /* do not update park_flt_stat_esp */
            break;
      }
      /* save afterrun ctrl data */
      tmp_u8 = ((p[1] >> 5u) & 0x03);
      switch(tmp_u8)
      {
         case 0u:
            buffer->esp_ldc_afterrun_ctrl = ESP_LDC_AFTERRUNCTRL_NO_FOLLOW_UP;
            break;
         case 1u:
            buffer->esp_ldc_afterrun_ctrl = ESP_LDC_AFTERRUNCTRL_BRAKE_FOLLOW_UP;
            break;
         case 2u:
            buffer->esp_ldc_afterrun_ctrl = ESP_LDC_AFTERRUNCTRL_RELEASE_FOLLOW_UP;
            break;
         default:
            /* do not update afterrun ctrl data */
            break;
      }
      /* update park_stat_esp data */
      tmp_u8 = (p[2] & 0x0F);
      switch (tmp_u8)
      {
         case 0u:
            buffer->esp_brake_state_park = BRAKING_STATE_PARKMAN_NOT_AVAILABLE;
            break;
         case 1u:
            buffer->esp_brake_state_park = BRAKING_STATE_PARKMAN_INIT;
            break;
         case 2u:
            buffer->esp_brake_state_park = BRAKING_STATE_PARKMAN_CANCEL_DRIVER;
            break;
         case 3u:
            buffer->esp_brake_state_park = BRAKING_STATE_PARKMAN_CANCEL_FAULT;
            break;
         case 4u:
            buffer->esp_brake_state_park = BRAKING_STATE_PARKMAN_ENABLED;
            break;
         case 7u:
            buffer->esp_brake_state_park = BRAKING_STATE_PARKMAN_INACTIV;
            break;
         case 8u:
            buffer->esp_brake_state_park = BRAKING_STATE_APC_MODE;
            break;
         case 9u:
            buffer->esp_brake_state_park = BRAKING_STATE_RPA_MODE;
            break;
         case 10u:
            buffer->esp_brake_state_park = BRAKING_STATE_RPA_UNDO_MODE;
            break;
         case 11u:
            buffer->esp_brake_state_park = BRAKING_STATE_DAA_MODE;
            break;
         case 12u:
            buffer->esp_brake_state_park = BRAKING_STATE_PPB_MODE;
            break;
         case 13u:
            buffer->esp_brake_state_park = BRAKING_STATE_CTA_MODE;
            break;
         default:
            /* do not update esp_brake_state_park */
            break;
      }
      break;

   /* save park status data from EPS to buffer */
   case CAN_ID_PARK_ST_RS_EPS:
         buffer->pla_termination_raw_data = (u8)(p[0] & 0x0F);
         buffer->status_eps_raw_data = (u8)((p[0] >> 4u) & 0x07);
         break;
   /* save engine status */
   case CAN_ID_PWRTR_STAT_CPC:
      buffer->engine_state = (u8)(p[0] & 0x03);
      break;

   /* save steering torque to buffer, stored in resolution  0.01 */
   case CAN_ID_STEERING_DATA_EPS:
      /* remove offset from data to convert it to a signed value */
      tmp_si16 = ((u16)(((p[1] << 8) | p[0]) & 0x3FFF) - 8192);

      if(tmp_si16 < 0)
      {
         buffer->steering_moment_sign_raw_data = 1;
         buffer->steering_moment_raw_data = (u16)(tmp_si16 * -1);
      }
      else
      {
         buffer->steering_moment_sign_raw_data = 0;
         buffer->steering_moment_raw_data = (u16)(tmp_si16);
      }

      /* save steering wheel angle in 0.1 resolution */
      /* remove offset from data to convert it to a signed value, update resolution as done in Parkman */
      buffer->steering_angle_offset_raw_data = p[4];

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

      tmp_u16 = (u16)(((p[3] << 8) | p[2]) & 0x7FFF);
      /* save front wheel angle stat before applying resolution on front wheel angle & saving it */
      tmp_u8 = (u8)((p[1] >> 6u) & 0x03);

      /* only update steering angle raw data & sign if state is high or low resolution & steering
         angle value is NOT SNA value (32767) */
      if(  ((tmp_u8 == 0u) || (tmp_u8 == 1u))
        && (tmp_u16 != 32767))
      {
         tmp_si16 = (si16)(( (((si32)((si32)tmp_u16 - 16384))
                           + (si32)(((si32)buffer->steering_angle_offset_raw_data - 128) * 5)) * 1024) / 1125);
         if(tmp_si16 < 0)
         {
            buffer->steering_angle_sign_raw_data = 1;
            buffer->steering_angle_raw_data = (u16)(tmp_si16 * -1);
         }
         else
         {
            buffer->steering_angle_sign_raw_data = 0;
            buffer->steering_angle_raw_data = (u16)(tmp_si16);
         }
      buffer->st_wheel_angle_time_2us = PIT_GetTimer2us();
         buffer->steering_angle = tmp_si16;

         tmp_s32 = (si32)( (((si32)((si32)tmp_u16 - 16384))
                           + (si32)(((si32)buffer->steering_angle_offset_raw_data - 128) * 5)) * 5);
         buffer->steering_angle_phy_1000th = tmp_s32;
      }
      else
      {
         /* Do not update steering angle raw data & sign */
      }
      break;

    /* save TSL target position in buffer */
    case CAN_ID_TSL_TARGET_POS_CPC:
      buffer->is_gear = (u8)(p[0] & 0x07);

      if(buffer->is_gear == 7u)
      {
         buffer->reverse_gear_raw_data = true;
      }
      else
      {
         buffer->reverse_gear_raw_data = false;
      }

      break;

    /* save vehicle acceleration data with a resolution of 0.02 */
    case CAN_ID_VEH_ACCEL_DATA_ESP:
      buffer->long_acc_offset_phys_data = p[4];
      buffer->lateral_acceleration = (si16)((si16)(((p[3] << 8) | p[2]) & 0x03FF) - 1024);
      tmp_si16 = (si16)(((p[1] << 8) | p[0]) & 0x0FFF);
      buffer->long_acc_phys_data = (si16)( ((si32)((si32)tmp_si16 - 2048u))
                                         + (si32)((si32)buffer->long_acc_offset_phys_data - 128u)) * (si32)20u;
      break;

   /* save vehicle speed data with a resolution of 0.01, input signal is of resolution 0.1 */
   case CAN_ID_VEH_SPEED_DATA_ESP:
      buffer->wheel_speed_raw_data = (u16)((((p[1] << 8) | p[0]) & 0x0FFF) * 10u);
      buffer->wheel_speed_timestamp_2us = XDAPM_InputTimer2us();
   break;

   /* save left wheel data */
   case CAN_ID_WHL_STAT_LEFT_ESP:
      buffer->wheel_impulses.timestamp_2us_left = XDAPM_InputTimer2us();
      buffer->wheel_speeds.timestamp_2us_left = XDAPM_InputTimer2us();
      buffer->wheel_impulses.wheel_impulses_fl = (u16) p[1];
      buffer->wheel_impulses.wheel_impulses_rl = (u16) p[2];
      buffer->wheel_impulses.msg_count = (u8)(p[0] & 0x0F); /* add seperate message count for each wheel side? */
      buffer->wheel_impulses.valid_fl = TRUE; /* wheel impulses validity stubbed to true */
      buffer->wheel_impulses.valid_rl = TRUE; /* wheel impulses validity stubbed to true */
      tmp_si16 = (si16)(((p[4] << 8) | p[3]) & 0x3FFF);
      buffer->wheel_speeds.wheel_speed_fl_raw = tmp_si16;
      buffer->wheel_speeds.wheel_speed_fl = (si16)(((si32)tmp_si16 * (si32)WHEEL_CIRCUMF_FRONT) / (si32)1200);
      tmp_si16 = (si16)(((p[6] << 8) | p[5]) & 0x3FFF);
      buffer->wheel_speeds.wheel_speed_rl_raw = tmp_si16;
      buffer->wheel_speeds.wheel_speed_rl = (si16)(((si32)tmp_si16 * (si32)WHEEL_CIRCUMF_REAR) / (si32)1200);
      tmp_u8 = (u8)((p[0] >> 4) & 0x03);
      /* fl wheel direction */
      switch (tmp_u8)
      {
         case 0u:
            buffer->wheel_info_fl_raw_data = COMH_WHEEL_INFO_STANDSTILL;
            break;
         case 1u:
            buffer->wheel_info_fl_raw_data = COMH_WHEEL_INFO_FORWARD;
            break;
         case 2u:
            buffer->wheel_info_fl_raw_data = COMH_WHEEL_INFO_BACKWARD;
            break;
         default:
            /* do not update wheel info */
            break;
      }
      /* RL wheel directoin */
      tmp_u8 = (u8)((p[0] >> 6) & 0x03);

      switch (tmp_u8)
      {
         case 0u:
            buffer->wheel_info_rl_raw_data = COMH_WHEEL_INFO_STANDSTILL;
            break;
         case 1u:
            buffer->wheel_info_rl_raw_data = COMH_WHEEL_INFO_FORWARD;
            break;
         case 2u:
            buffer->wheel_info_rl_raw_data = COMH_WHEEL_INFO_BACKWARD;
            break;
         default:
            /* do not update wheel info */
            break;
      }
      break;
   /* save left wheel data */
   case CAN_ID_WHL_STAT_RIGHT_ESP:
      buffer->wheel_impulses.timestamp_2us_right = XDAPM_InputTimer2us();
      buffer->wheel_speeds.timestamp_2us_right = XDAPM_InputTimer2us();
      buffer->wheel_impulses.wheel_impulses_fr = (u16) p[1];
      buffer->wheel_impulses.wheel_impulses_rr = (u16) p[2];
      buffer->wheel_impulses.msg_count = (u8)(p[0] & 0x0F); /* add seperate message count for each wheel side? */
      buffer->wheel_impulses.valid_fr = TRUE; /* wheel impulses validity stubbed to true */
      buffer->wheel_impulses.valid_rr = TRUE; /* wheel impulses validity stubbed to true */
      tmp_si16 = (si16)(((p[4] << 8) | p[3]) & 0x3FFF);
      buffer->wheel_speeds.wheel_speed_fr_raw = tmp_si16;
      buffer->wheel_speeds.wheel_speed_fr = (si16)(((si32)tmp_si16 * (si32)WHEEL_CIRCUMF_FRONT) / (si32)1200);
      tmp_si16 = (si16)(((p[6] << 8) | p[5]) & 0x3FFF);
      buffer->wheel_speeds.wheel_speed_rr_raw = tmp_si16;
      buffer->wheel_speeds.wheel_speed_rr = (si16)(((si32)tmp_si16 * (si32)WHEEL_CIRCUMF_REAR) / (si32)1200);
      tmp_u8 = (u8)((p[0] >> 4) & 0x03);
      /* fl wheel direction */
      switch (tmp_u8)
      {
         case 0u:
            buffer->wheel_info_fr_raw_data = COMH_WHEEL_INFO_STANDSTILL;
            break;
         case 1u:
            buffer->wheel_info_fr_raw_data = COMH_WHEEL_INFO_FORWARD;
            break;
         case 2u:
            buffer->wheel_info_fr_raw_data = COMH_WHEEL_INFO_BACKWARD;
            break;
         default:
            /* do not update wheel info */
            break;
      }
      /* RL wheel directoin */
      tmp_u8 = (u8)((p[0] >> 6) & 0x03);

      switch (tmp_u8)
      {
         case 0u:
            buffer->wheel_info_rr_raw_data = COMH_WHEEL_INFO_STANDSTILL;
            break;
         case 1u:
            buffer->wheel_info_rr_raw_data = COMH_WHEEL_INFO_FORWARD;
            break;
         case 2u:
            buffer->wheel_info_rr_raw_data = COMH_WHEEL_INFO_BACKWARD;
            break;
         default:
            /* do not update wheel info */
            break;
      }
      break;

      /* save vehicle yaw data */
      case CAN_ID_VEHDYN_STAT2_ESP:
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
         tmp_u8 = (u8)p[4];
         /* save vehicle yaw offset */
         tmp_u16 = (u16)(((p[3] << 8u) | p[2]) & 0x03FF);

         tmp_u32 = (u32)((p[1] << 8u) | (p[0]));

         /* apply yaw offset to yaw raw data if qual is ELEC or FUNC, and yaw raw data & yaw offset are not SNA*/
         if ((tmp_u16 != 1023u) && (tmp_u32 != 65535u) && ((tmp_u8 == 1u) || (tmp_u8 == 2u)))
         {
            tmp_s32 = (sint32)( ( ( ((sint32)tmp_u32 - (sint32)32768)
                              + ((sint32)tmp_u16 - (sint32)512))
                             * (sint32)2048) / (sint32)1125);
            if(tmp_s32 < 0)
            {
               buffer->yaw_speed_raw_data = (u16)(-1 * tmp_s32);
               buffer->yaw_speed_sign_raw_data = (u8)(1u);
            }
            else
            {
               buffer->yaw_speed_raw_data = (u16)tmp_s32;
               buffer->yaw_speed_sign_raw_data = (u8)0u;
            }
            buffer->yaw_speed_timestamp_2us = XDAPM_InputTimer2us();
            float yaw_offset_phys;
            float yaw_phys;
            yaw_offset_phys = (float)((tmp_u16 * 0.01f ) - 5.12f);
            yaw_phys =(float) ((tmp_u32 * 0.01f) - 327.68f);

            buffer->yaw_physical_deg_p_s = yaw_phys ;

            tmp_s32 = (sint32)  ( ((sint32)tmp_u32 - (sint32)32768)
                              + ((sint32)tmp_u16 - (sint32)512));
            buffer->yaw_speed_phys_data_100th = tmp_s32;
         }
         else
         {
            /* do nothing, do not update yaw values */
         }
         break;

      case 0x6B7: /* Kombi_2                                               */

          buffer->temp_filtered_raw_data    = ((p[0]*0.5f)-40);
          buffer->temperature_received      = TRUE;

          break;

   	case CAN_ID_PLATFORM_BLE:

   			buffer->ble_msg.BLE_ID_From = p[0] & 0x0F;
            buffer->ble_msg.BLE_MsgCounter = p[1];
            buffer->ble_msg.park_MsgType = (p[0] & 0xF0) >> 4;
            buffer->ble_msg.SP_CurrentState = p[2] & 0x3F;
            buffer->ble_msg.SP_DeadmanConclusion = (p[6] & 0x20) >> 5;
            buffer->ble_msg.SP_DeadmanX = ((u16)p[3]) | ((p[4] & 0x0F) << 8);
            buffer->ble_msg.SP_DeadmanY = ((u16)p[4] >> 4) | (((u16)p[5]) << 4);
            buffer->ble_msg.SP_SmartphoneConnected = (p[2] & 0x40) >> 6;
            buffer->ble_msg.SP_UserAbortRequest = (p[2] & 0x80) >> 7;
            buffer->ble_msg.SP_NewManeuverRequest = p[6] & 0x1F;
            buffer->ble_msg.SP_RiskAcknowledgment = p[7] & 0x0F;

   	break;

    #else
    case 0xFD: /* ESP_21 */
            if (n == 8)
            {
                /* ASR_Anf  */
                if ((p[7] & 0x02) == 0x00)
                {
                    buffer->asr_request_raw_data = (u8)FALSE;
                }
                else
                {
                    buffer->asr_request_raw_data = (u8)TRUE;
                }
                /* ABS_Bremsung - ABS intervention */
                if ((p[7] & 0x01) == 0x00)
                {
                    buffer->abs_intervention_raw_data = (u8)FALSE;
                }
                else
                {
                    buffer->abs_intervention_raw_data = (u8)TRUE;
                }
                /* ESP_Eingriff - ESP intervention */
                if ((p[7] & 0x20) == 0x00)
                {
                    buffer->esp_intervention_raw_data = (u8)FALSE;
                }
                else
                {
                    buffer->esp_intervention_raw_data = (u8)TRUE;
                }
                /* ESP_v_signal 16bits start bit32 factor 0,01 km/h)           */
                buffer->wheel_speed_raw_data = (p[4] + (p[5] << 8));
                /* ESPASR_passiv - ESP/ASR switched off */
                if ((p[6] & 0x03) == 0x00)
                {
                    buffer->esp_asr_passive_raw_data = (u8)FALSE;
                }
                else
                {
                    buffer->esp_asr_passive_raw_data = (u8)TRUE;
                }
            }
            break;


    case 0x6B7: /* Kombi_2                                               */
        if (n == 8)
        {
            buffer->temp_filtered_raw_data    = (p[7] >> 1) - 50;
            buffer->temperature_received      = TRUE;
        }
        else
        {
        }
        break;

    case 0x101: /* ESP_02                                             */
#ifdef APPL_IS_PLA_SW
        if (n == 8)
        {
          buffer->long_acc_phys_data = (si16)(p[3] + ((p[4] & 0x03) << 8));

          buffer->long_acc_qualibit = (p[1] & 0x20);


          buffer->yaw_speed_raw_data = (p[5] + (si16)((p[6] & 0x3f) * 256));
          if ((p[6] & 0x40) == 0x00)
          {
            buffer->yaw_speed_sign_raw_data = 0;
          }
          else
          {
            buffer->yaw_speed_sign_raw_data = 1;
          }

          if (p[1] & 0x80)
          {
            buffer->brake_is_standstill = TRUE;
          }
          else
          {
            buffer->brake_is_standstill = FALSE;
          }

//            /* ESP_Querb_Zahn (bit 8 0:Querbeschleunigung  */
//            if ((p[1] & 0x01) == 0)
//            {
          /* BR2_Querbeschl (8bits startbit 0 0.01m/m^2 */
          buffer->lateral_acceleration = (si16)p[2];
//            }

          buffer->brake_ecu_state = (p[7] & 0xF0) >> 4;

        }
#endif
    break;

    case 0x3DB: /* Gateway_72                                           */
        if (n == 8)
        {
            /* BCM_Rueckfahrlicht_Anf (bit 38: 0 off 1 on)              */
            if (p[4] & 0x40)
            {
                buffer->reverse_gear_raw_data = TRUE;
            }
            else
            {
                buffer->reverse_gear_raw_data = FALSE;
            }
            /* AAG_Anhaenger_erkannt (bit 28: 0 not attached, 1 attached) */
            if (p[3] & 0x10)
            {
                buffer->trailer_present_raw_data = TRUE;
            }
            else
            {
                buffer->trailer_present_raw_data = FALSE;
            }
#ifdef APPL_IS_PLA_SW
            /* Blinker_li (bit 50: 0 off 1 on)                        */
            /* Blinker_re (bit 51: 0 off 1 on)                        */
            switch (p[6] & 0x0c)
            {
            case 0x04:                            /* li on and re off */
                buffer->blinker_left_raw_data  = TRUE;
                buffer->blinker_right_raw_data = FALSE;
                break;
            case 0x08:                            /* re on and li off */
                buffer->blinker_left_raw_data  = FALSE;
                buffer->blinker_right_raw_data = TRUE;
                break;
            default:                              /* booth on or off  */
                buffer->blinker_left_raw_data  = FALSE;
                buffer->blinker_right_raw_data = FALSE;
            }
            /* ZV_HFS_offen (bit 20: 0 closed, 1 opened) */
            if (p[2] & 0x10)
            {
                buffer->is_door_open_rear_left = TRUE;
            }
            else
            {
                buffer->is_door_open_rear_left = FALSE;
            }
            /* ZV_HBFS_offen (bit 21: 0 closed, 1 opened) */
            if (p[2] & 0x20)
            {
                buffer->is_door_open_rear_right = TRUE;
            }
            else
            {
                buffer->is_door_open_rear_right = FALSE;
            }
            /* ZV_FT_offen (bit 26: 0 closed, 1 opened) */
            if (p[3] & 0x04)
            {
                buffer->is_door_open_front_left = TRUE;
            }
            else
            {
                buffer->is_door_open_front_left = FALSE;
            }
            /* ZV_BT_offen (bit 24: 0 closed, 1 opened) */
            if (p[3] & 0x01)
            {
                buffer->is_door_open_front_right = TRUE;
            }
            else
            {
                buffer->is_door_open_front_right = FALSE;
            }
#endif
        }
        break;

	case 0x3A5:

			buffer->ble_msg.BLE_ID_From = p[0] & 0x0F;
            buffer->ble_msg.BLE_MsgCounter = p[1];
            buffer->ble_msg.park_MsgType = (p[0] & 0xF0) >> 4;
            buffer->ble_msg.SP_CurrentState = p[2] & 0x3F;
            buffer->ble_msg.SP_DeadmanConclusion = (p[6] & 0x20) >> 5;
            buffer->ble_msg.SP_DeadmanX = ((u16)p[3]) | ((p[4] & 0x0F) << 8);
            buffer->ble_msg.SP_DeadmanY = ((u16)p[4] >> 4) | (((u16)p[5]) << 4);
            buffer->ble_msg.SP_SmartphoneConnected = (p[2] & 0x40) >> 6;
            buffer->ble_msg.SP_UserAbortRequest = (p[2] & 0x80) >> 7;
            buffer->ble_msg.SP_NewManeuverRequest = p[6] & 0x1F;
            buffer->ble_msg.SP_RiskAcknowledgment = p[7] & 0x0F;

	break;

    case 0x100:/* Switch Button UPA and P4U */
#ifdef  XMENU_BAR
        buffer->confirmation_button            = (p[0] & 0x01);
        buffer->switch_options_button          = (p[1] & 0x01);
#else
        buffer->park4u_button            = (p[0] & 0x01);
        buffer->upa_button               = (p[1] & 0x01);
#endif
		buffer->bda_replay_button        = (p[2] & 0x01);

	break;

    case 0x6AE:
        //buffer->view_button =
        buffer->GPS_button              = (p[2]&0x10); //mirror rotate left
        buffer->p4uhome_training_button = (p[2]&0x20); //mirror rotate right
        buffer->abort_button            = ((p[0]&0x01) || (p[0]&0x10) || (p[2]&0x01)); //mirror up
        buffer->dead_man_button         = ((p[0]&0x02) || (p[0]&0x20) || (p[2]&0x02)); //mirror down
#ifdef  XMENU_BAR
        buffer->upa_button   = ((p[0]&0x04) || (p[0]&0x40) || (p[2]&0x04)); //mirror left
        buffer->park4u_button  = ((p[0]&0x08) || (p[0]&0x80) || (p[2]&0x08)); //mirror right
#else
        buffer->switch_options_button   = ((p[0]&0x04) || (p[0]&0x40) || (p[2]&0x04)); //mirror left
        buffer->confirmation_button     = ((p[0]&0x08) || (p[0]&0x80) || (p[2]&0x08)); //mirror right
#endif
        break;
    case 0x116: /* ESP_10                                             */
        if (n == 8)
        {
#ifdef XAPPL_REPLAY_ULSD1_V3
            /* Copy the last wheel ticks in the CAN Wheel Ticks buffer */
            CopyCanBufferInCanWTBuffer();
#endif
            buffer->wheel_impulses.timestamp_2us = XDAPM_InputTimer2us();
            buffer->wheel_impulses.wheel_impulses_fl = p[2] + ((p[3] & 0x03) << 8);
            buffer->wheel_impulses.wheel_impulses_fr = ((p[3] & 0xfc) >> 2) + ((p[4] & 0x0f) << 6);
            buffer->wheel_impulses.wheel_impulses_rr = ((p[5] & 0xc0) >> 6) + (p[6] << 2);
            buffer->wheel_impulses.wheel_impulses_rl = ((p[4] & 0xf0) >> 4) + ((p[5] & 0x3f) << 4);

            /* get signal valid info */
            if ((p[1] & 0x10) == 0)
            {
                buffer->wheel_impulses.valid_fl = TRUE;
            }
            else
            {
                buffer->wheel_impulses.valid_fl = FALSE;
            }
            if ((p[1] & 0x20) == 0)
            {
                buffer->wheel_impulses.valid_fr = TRUE;
            }
            else
            {
                buffer->wheel_impulses.valid_fr = FALSE;
            }
            if ((p[1] & 0x40) == 0)
            {
                buffer->wheel_impulses.valid_rl = TRUE;
            }
            else
            {
                buffer->wheel_impulses.valid_rl = FALSE;
            }
            if ((p[1] & 0x80) == 0)
            {
                buffer->wheel_impulses.valid_rr = TRUE;
            }
            else
            {
                buffer->wheel_impulses.valid_rr = FALSE;
            }



            tmp_u8 = ( (p[7] & 0xC0) >> 6 );
            if      ( tmp_u8 == 0 ) { buffer->wheel_info_rr_raw_data = COMH_WHEEL_INFO_FORWARD;  }
            else if ( tmp_u8 == 1 ) { buffer->wheel_info_rr_raw_data = COMH_WHEEL_INFO_BACKWARD; }
            else                    { buffer->wheel_info_rr_raw_data = COMH_WHEEL_INFO_INVALID;  }
            tmp_u8 = ( (p[7] & 0x30) >> 4 );
            if      ( tmp_u8 == 0 ) { buffer->wheel_info_rl_raw_data = COMH_WHEEL_INFO_FORWARD;  }
            else if ( tmp_u8 == 1 ) { buffer->wheel_info_rl_raw_data = COMH_WHEEL_INFO_BACKWARD; }
            else                    { buffer->wheel_info_rl_raw_data = COMH_WHEEL_INFO_INVALID;  }
            tmp_u8 = ( (p[7] & 0x0C) >> 2 );
            if      ( tmp_u8 == 0 ) { buffer->wheel_info_fr_raw_data = COMH_WHEEL_INFO_FORWARD;  }
            else if ( tmp_u8 == 1 ) { buffer->wheel_info_fr_raw_data = COMH_WHEEL_INFO_BACKWARD; }
            else                    { buffer->wheel_info_fr_raw_data = COMH_WHEEL_INFO_INVALID;  }
            tmp_u8 = ( p[7] & 0x03 );
            if      ( tmp_u8 == 0 ) { buffer->wheel_info_fl_raw_data = COMH_WHEEL_INFO_FORWARD;  }
            else if ( tmp_u8 == 1 ) { buffer->wheel_info_fl_raw_data = COMH_WHEEL_INFO_BACKWARD; }
            else                    { buffer->wheel_info_fl_raw_data = COMH_WHEEL_INFO_INVALID;  }

            buffer->wheel_impulses.msg_count = (p[1] & 0x0F);
            //DAPM_ReceiveSignals();
        }
        break;

    case 0xB2: /* ESP_19                                             */
#ifdef APPL_IS_PLA_SW
        if (n == 8)
        {
            /* RR3_Rad_kmh_HL (16bits startbit 0  0.0075km/h                */
            buffer->wheel_speeds.wheel_speed_rl = (((p[0] + (p[1] << 8))*75) / 360);
            /* RR3_Rad_kmh_HR (16bits startbit 16 0.0075km/h                */
            buffer->wheel_speeds.wheel_speed_rr = (((p[2] + (p[3] << 8))*75) / 360);
            /* RR3_Rad_kmh_VL (16bits startbit 32 0.0075km/h                */
            buffer->wheel_speeds.wheel_speed_fl = (((p[4] + (p[5] << 8))*75) / 360);
            /* RR3_Rad_kmh_VR (16bits startbit 48 0.0075km/h                */
            buffer->wheel_speeds.wheel_speed_fr = (((p[6] + (p[7] << 8))*75) / 360);
            }
#endif
        break;


    case 0x9F: /* LH_EPS_03                                           */
#ifdef APPL_IS_PLA_SW
        if (n == 8)
        {
            /* Lenkmoment (10bits startbit 40 0.01Nm                */
            buffer->steering_moment_raw_data = p[5] + ((p[6] & 0x03) << 8);
            /* Lenkmoment Sign (1Bit, startbit 26)*/
            buffer->steering_moment_sign_raw_data = (p[6] & 0x80);
        }
#endif
        break;


    case 0x32A: /* LH_EPS_01                                           */
#ifdef APPL_IS_PLA_SW
        if (n == 8)
        {
            /* StatEPS_PLA (2bit startbit 32)                         */
            buffer->status_eps_raw_data = ((p[3] & 0xc0) >> 6) + ((p[4] & 0x03) << 2);
        }
#endif
        break;

    case 0x86: /* LWI_01                                              */
#ifdef APPL_IS_PLA_SW
        if (n == 8)
        {
            buffer->st_wheel_angle_time_2us = XDAPM_InputTimer2us();
            /* LWI_Lenkradwinkel (13bits startbit 16 0.1degrees)                  */
            buffer->steering_angle_raw_data = p[2] + ((p[3] & 0x1f) << 8);
            /* LWI_VZ_Lenkradwinkel (bit29 0 positive 1 negative)                 */
            buffer->steering_angle_sign_raw_data = (p[3] & 0x20);
        }
#endif
        break;

    case 0xAD: /* Getriebe_11 */
      if (n == 8)
      {
        buffer->is_gear = (p[5] & 0x3C) >> 2;
      }
      break;
    case 0x106: /* ESP_05                                             */
        if(n == 8)
        {
            buffer->brake_pressure_raw_data = (((p[2]) | ((p[3] & 0x03) << 8))* 0.3)- 30UL;
        }
        break;


#ifdef XP2GPA_EXT_ODOM_REAR_WA
    case XP2GPA_EXT_ODOM_REAR_WA_CAN_ID: /* TestData (ext. Odom. / RearWheelAngle)   */
        buffer->ext_veh_position_timestamp_2us = XDAPM_InputTimer2us();
        buffer->ext_veh_x_pos_raw_data = p[0] + (p[1] << 8);
        buffer->ext_veh_y_pos_raw_data = p[2] + (p[3] << 8);
        buffer->ext_veh_angle_raw_data = p[4] + (p[5] << 8);
        buffer->wheel_angle_rear_raw_data = p[6] + ((p[7] & 0x03) << 8);
        buffer->wheel_angle_rear_sign_raw_data = (p[7] & 0x04) >> 2;
        break;
#endif

#ifdef XCOMH_ENABLE_REPLAY_INPUT
    case XCOMH_P2_SYNC_TASK_CAN_ID:
# if defined(XAPPL_REPLAY_ULSD1_V2)
        /* get timestamp for sync time */
        st_comh_recvd_sync_timstmp_2us = 0;
        st_comh_recvd_sync_timstmp_2us = (u32)p[0];
        st_comh_recvd_sync_timstmp_2us |= (u32)(p[1] << 8);
        st_comh_recvd_sync_timstmp_2us |= (u32)(p[2] << 16);
        st_comh_recvd_sync_timstmp_2us |= (u32)(p[3] << 24);

        /* get timestamp for wheel pulses */
        buffer->wheel_impulses.timestamp_2us = 0;
        buffer->wheel_impulses.timestamp_2us = (u32)p[4];
        buffer->wheel_impulses.timestamp_2us |= (u32)(p[5] << 8);
        buffer->wheel_impulses.timestamp_2us |= (u32)(p[6] << 16);
        buffer->wheel_impulses.timestamp_2us |= (u32)(p[7] << 24);
        /* convert from 512us into 2 us (left shift 8) */
        buffer->wheel_impulses.timestamp_2us <<= 8;

        P2GPA_RunCycle(TRUE);
        /* Synchronize Can Data */
        CopyCanBufferInSyncBuffer();
# elif defined(XAPPL_REPLAY_ULSD1_V3)
        /* get timestamp for wheel pulses */
        buffer->wheel_impulses.timestamp_2us = 0;
        buffer->wheel_impulses.timestamp_2us = (u32)p[0];
        buffer->wheel_impulses.timestamp_2us |= (u32)(p[1] << 8);
        buffer->wheel_impulses.timestamp_2us |= (u32)(p[2] << 16);
        /* convert from 512us into 2 us (left shift 8) */
        buffer->wheel_impulses.timestamp_2us <<= 8;
        st_comh_recvd_sync_timstmp_2us = buffer->wheel_impulses.timestamp_2us;
        st_comh_sync_cnt = p[3];

        if (buffer->wheel_impulses.cnt == st_comh_sync_cnt)
        {
          /* can_buffer cnt == sync cnt */
          /* the last wheel ticks were used processed with the Sync Frame */
          CopyCanBufferInSyncBuffer();
        }
        else if (st_comh_sync_cnt == st_comh_buffer_can_wt_data.wheel_impulses.cnt)
        {
          /* wt_buffer cnt == sync cnt */
          CopyCanWTBufferInSyncBuffer();
          st_comh_buffer_sync_data.wheel_impulses.timestamp_2us = st_comh_recvd_sync_timstmp_2us;
          _ASSERT(st_comh_sync_cnt == st_comh_buffer_sync_data.wheel_impulses.cnt);
        }
        else if (st_comh_sync_cnt == st_comh_buffer_sync_data.wheel_impulses.cnt)
        {
          /* sync_buffer cnt == sync cnt ==> No change in the wheel ticks cnt since last Sync Frame */
          st_comh_recvd_sync_timstmp_2us = st_comh_buffer_sync_data.wheel_impulses.timestamp_2us;
          _ASSERT((st_comh_sync_cnt == st_comh_buffer_sync_data.wheel_impulses.cnt) || (st_comh_buffer_sync_data.wheel_impulses.cnt == 0xFF));
        }
        else
        {
            /* We shall receive wheel ticks before next COMH */
        }
# endif
        break;
#endif

#endif

    }
    }
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

    /* if flag st_comh_copy_in_can_irpt is set, save the data in
     * the can interrupt buffer */

    if(st_comh_copy_in_can_irpt)
    {
        /* only save data in can_irpt_buffer */
        SaveCanDataInIrptBuffer(id, p, n, st_comh_buffer_counter, st_comh_buffer_can_irpt_data);
        /* increase can data buffer counter */
        st_comh_buffer_counter++;
    }
    else
    {
        /* save data in buffer_can_data and in can_irpt_data buffer */
        SaveCanDataInBuffer(id, p, n, &st_comh_buffer_can_data);
    }
}
/**
 * void SynchronizeTask(void)
 *
 * gets current TIMR time
 */
void SynchronizeTask(void)
{
    /* copy Can Buffer to Application buffer */
    CopyCanBufferInApplBuffer();
}


/**
 * static void CopyCanBufferInApplBuffer(void)
 *
 * Copy for RECORD mode, copies the data, saved in the Can Buffer to
 * Application Buffer.
 */
static void CopyCanBufferInApplBuffer(void)
{
    st_comh_buffer_counter = 0;
    /* activate can interrupt buffer */
    st_comh_copy_in_can_irpt = TRUE;

    /* copy can data buffer to application data buffer */
    memcpy(&st_comh_buffer_appl_data, &st_comh_buffer_can_data, sizeof(st_comh_buffer_appl_data));

    /* deactivate can interrupt buffer */
    st_comh_copy_in_can_irpt = FALSE;

    /* save can Irpt Buffer in can data */
    CopyIrptBufferInCanBuffer(st_comh_buffer_counter,
        st_comh_buffer_can_irpt_data,
        &st_comh_buffer_can_data);
}

/**
 * static void DIDH_LENGTH_OF_SENDBUFFER(void)
 *
 * Function to send the mDevPLA_1 - CAN message (development identifier)
 *
 */
static void CanSendDevId1(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_1, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_1, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 * static void CanSendDevId2(void)
 *
 * Function to send the mDevPLA_2 - CAN message (development identifier)
 */
static void CanSendDevId2(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_2, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_2, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}


/**
 * static void CanSendDevId3(void)
 *
 * Function to send the mDevPLA_3 - CAN message (development identifier)
 */
static void CanSendDevId3(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_3, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_3, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 * static void CanSendDevId4(void)
 *
 * Function to send the mDevPLA_4 - CAN message (development identifier)
 */
static void CanSendDevId4(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_4, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    if(TRUE == BRKH_CusIsEmergencyBrakeActive())
    {
        send_buffer[1] = 0;
        send_buffer[0] = send_buffer[0] & 0x0F ;
    }
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_4, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}


/**
 * static void CanSendDevId5(void)
 *
 * Function to send the mDevPLA_5 - CAN message (development identifier)
 */
static void CanSendDevId5(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_5, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_5, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 * static void CanSendDevId6(void)
 *
 * Function to send the mDevPLA_6 - CAN message (development identifier)
 */
static void CanSendDevId6(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_6, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_6, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 * static void CanSendDevId7(void)
 * Function to send the mDevPLA_7 - CAN message (development identifier)
 *
 * \return nothing
 */
static void CanSendDevId7(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_7, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_7, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 *  static void CanSendDevId8(void)
 *
 *  function to send the mDevPLA_8 - CAN message (development identifier)
 *
 *  \return nothing
 */
static void CanSendDevId8(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_8, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_8, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 *  static void CanSendDevId9(void)
 *
 *  function to send the mDevPLA_9 - CAN message (development identifier)
 *
 *  \return nothing
 */
static void CanSendDevId9(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_9, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_9, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 *  static void CanSendDevId10(void)
 *  function to send the mDevPLA_10 - CAN message (development identifier)
 *
 *  \return nothing
 */
static void CanSendDevId10(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_10, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_10, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 *  static void CanSendDevId11(void)
 *
 *  function to send the mDevPLA_11 - CAN message (development identifier)
 *
 *  \return nothing
 */
static void CanSendDevId11(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_11, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_11, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}




/**
 * static void CanSendDevId12(void)
 *
 * function to send the mDevPLA_12 - CAN message (development identifier)
 *
 * \return nothing
 */
static void CanSendDevId12(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_12, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_12, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}



/**
 *  static void CanSendDevId13(void)
 *
 *  function to send the mDevPLA_13 - CAN message (development identifier)
 *
 *  \return nothing
 */
static void CanSendDevId13(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_13, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_13, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}




/**
 *  static void CanSendDevId14(void)
 *
 *  function to send the mDevPLA_14 - CAN message (development identifier)
 *
 *  \return nothing
 */
static void CanSendDevId14(u8 *send_buffer)
{
#ifdef XP2GPA_STD_DEV_IDS_OUT_CAN1
    P2GPA_CanSend (P2GPA_CAN_prio_high, P2GPA_CAN1_ID_DEV_14, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
#if defined(XP2GPA_STD_DEV_IDS_OUT_CAN2) && !defined(XP2GPA_CAN2_DEV_IDS_ON_CAN1)
    P2GPA_CanSendDebugCh (P2GPA_CAN2_ID_DEV_14, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif
}

static void CanSendDevId15(u8 *send_buffer)
{
    /* Send RawDat from DID2 */
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_15, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_15, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId16(u8 *send_buffer)
{
    /* Send PosTim */
#ifdef XP2GPA_POSTIM_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
            P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_16, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
            P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_16, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif
}

static void CanSendDevId17(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_17, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_17, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId18(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_18, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_18, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId19(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_19, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_19, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId20(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_20, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_20, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId21(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_21, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_21, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId22(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_22, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_22, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId23(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_23, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_23, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId24(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_24, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_24, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId25(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_25, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_25, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId26(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_26, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_26, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId27(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_27, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_27, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId28(u8 *send_buffer)
{
#ifdef XP2GPA_RAWDAT_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_28, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_28, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_RAWDAT_OUT_CAN2 */
}

static void CanSendDevId29(u8 *send_buffer)
{
#ifdef XP2GPA_ECHOSIGNALS_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_29, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_29, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_ECHOSIGNALS_OUT_CAN2 */
}

static void CanSendDevId30(u8 *send_buffer)
{
#ifdef XP2GPA_ECHOSIGNALS_OUT_CAN2
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_30, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_30, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef XP2GPA_ECHOSIGNALS_OUT_CAN2 */
}

static void CanSendDevId31(u8 *send_buffer)
{
#ifdef P2GPA_CAN_WARNELMS_SEND
#  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1
    P2GPA_CanSend(P2GPA_CAN_prio_high, P2GPA_CAN_ID_DEV_31, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  else
    P2GPA_CanSendDebugCh(P2GPA_CAN_ID_DEV_31, send_buffer, DIDH_LENGTH_OF_SENDBUFFER);
#  endif /* #  ifdef XP2GPA_CAN2_DEV_IDS_ON_CAN1 */
#endif /* #ifdef P2GPA_CAN_WARNELMS_SEND */
}



static void CanSendPlaStatus(void)
{
    u8 hmi_content[8];
    u8 selectable_hmi_content[8];

    HMIH_GetMessage(hmi_content);
    ACTL_GetSelectableDisplayContent(selectable_hmi_content);

    P2GPA_CanSend (P2GPA_CAN_prio_high, COMH_P2_CAN_ID_DISPLAY, hmi_content, 8);
    P2GPA_CanSend (P2GPA_CAN_prio_high, 0x779, selectable_hmi_content, 8);
}



/**
 * static void CanSendLedTong(void)
 * Function to send the LED and Sound command to other ECU - CAN message
 *
 */
static void CanSendLedTong(void)
{
    u8 buff[8];
    u8 frontSpeak;
    u8 rearSpeak;

    #ifdef DAIMLER_BR_213_PLATFORM

    u8 tmp_u8;
    u8 dlc = 3u;
    static u8 taskCounter = 0u; /* periodicity is 200 ms, so send message every 10 task calls to keep SQC synchronized */

   if(taskCounter == 10u)
   {
      /* read values of front & rear speakers */

      PLATFORM_ReadFrontSpeaker(&frontSpeak) ;
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
         buff[0] = 0x03; /* acoustic warning park on & ParkSwLED_Rq is cont (1u)*/
       }
       else if(tmp_u8 != 0u)
       {
         buff[0] = 0x05; /* acoustic warning park on & ParkSwLED_Rq is blinking (2u) */
       }
       else
       {
         buff[0] = 0u; /* acoustic warning park off since ctrl value is 0 & ParkSwLED_Rq OFF */
       }
       buff[1] = (u8)(0x20 | (tmp_u8 & 0x0F)); /* send F2 frequency, default volume & warning sound front*/
       buff[2] = (u8)(0x20 | (tmp_u8 & 0x0F)); /* send F2 frequency, default volume & warning sound rear*/


       /* Send ParkWarn_PARK CAN message */
       P2GPA_CanSend (P2GPA_CAN_prio_high, CAN_ID_PARKWARN_PARK, buff, dlc);

       /* prepare data to be sent on Park_Set_Stat CAN message */
       /* set dlc to DLC value of Park_set_Stat CAN message */
       dlc = 4u;

       /* start filling buff according to Park_Set_Stat CAN message structure */
       buff[0] = 0x22; /* front freq & volume are 2 */
       buff[1] = 0x22; /* rear freq & volume are 2 */
       buff[2] = 0x2A; /* front & rear sonar modes are on (2u), brkmd_stat is on (2u) */

       if(tmp_u8 != 0u)
       {
         buff[3] = 0x25; /* no acoustics required so attenuat_rq is active (1u), attenuatMd_stat is 1, RevGrMd_Stat is on (2u) */
       }
       else
       {
         buff[3] = 0x24; /* no acoustics required so attenuat_rq is idle (0u), attenuatMd_stat is 1, RevGrMd_Stat is on (2u) */
       }

       taskCounter = 0u; /* reset task counter to 0 */

       /* Send Park_set_Stat CAN message */
       P2GPA_CanSend (P2GPA_CAN_prio_high, CAN_ID_PARK_SET_STAT_PARK, buff, dlc);
   }
   else
   {
      taskCounter++;
   }

    #else

   /* read values of front & rear speakers */
    PLATFORM_ReadFrontSpeaker(&frontSpeak) ;
    PLATFORM_ReadRearSpeaker(&rearSpeak);

    buff[0] = (bool_T)p4u_get_led_state();
    buff[1] = (bool_T)upa_get_led_state();
    buff[2] = 0xff;
    buff[3] = 0xff;
    buff[4] = frontSpeak;
    buff[5] = rearSpeak;
    buff[6] = (u8)0;
    buff[7] = (u8)0;

#ifdef XAPPL_BRAKE_OPEN_ROAD
    if ((IEBM_IsEmergencyBrakeRequired() != FALSE) ||
    (open_road_em_br_tone != 0))
    {
    if (IEBM_IsEmergencyBrakeRequired() != FALSE)
    {
      /* initialize the long continuous tone */
      open_road_em_br_tone = COMH_OPEN_ROAD_EM_BR_TONE/20;
    }

    /* decrease the counter until 0 */
    if (open_road_em_br_tone > 0)
    {
      open_road_em_br_tone = open_road_em_br_tone - 1;
    }
    /* perform a continuos tone if emergency brake or timer not 0 */
        buff[4] = 0;
        buff[5] = 0;
    }
#endif /*XAPPL_BRAKE_OPEN_ROAD*/

    P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_CAN_ID_LED_SOUND, buff, sizeof(buff));
    #endif
}



/**
 * static void CanSendParkhilfe5(void)
 *
 * function to send the mParkhilfe_5 CAN message
 *
 */
static void CanSendParkhilfe5(void)
{
    u8 i;
    u8 buff[8];
    u8 sector_distances_cm[4][COMH_NUM_SECTORS_MAX];

    for (i = 0; i < COMH_NUM_SECTORS_MAX; i++)
    {
        sector_distances_cm[COMH_SPA_FRONT][i] = (u8) P2DAL_GetSectorDistance(DAPM_SPA_FRONT, i,DAPM_DR_CM);
        sector_distances_cm[COMH_SPA_REAR][i]  = (u8) P2DAL_GetSectorDistance(DAPM_SPA_REAR, i,DAPM_DR_CM);
    }

    buff[0] = sector_distances_cm[COMH_SPA_FRONT][0];
    buff[1] = sector_distances_cm[COMH_SPA_FRONT][3];
    buff[2] = sector_distances_cm[COMH_SPA_REAR][0];
    buff[3] = sector_distances_cm[COMH_SPA_REAR][3];
    buff[4] = sector_distances_cm[COMH_SPA_FRONT][1];
    buff[5] = sector_distances_cm[COMH_SPA_FRONT][2];
    buff[6] = sector_distances_cm[COMH_SPA_REAR][1];
    buff[7] = sector_distances_cm[COMH_SPA_REAR][2];

    P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_CAN_ID_PARKHILFE5, buff, sizeof(buff));
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
    st_comh_buffer_can_data.is_vehicle_standstill = FALSE;
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
          st_comh_buffer_can_data.is_vehicle_standstill = TRUE;
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
          st_comh_buffer_can_data.is_vehicle_standstill = FALSE;
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
          st_comh_buffer_can_data.is_vehicle_standstill = FALSE;
        }
      }
      standstill_time_counter = 0;
    }
    old_wheel_impulses_rl = current_wheel_impulses_rl;
  }
}


//void COMH_SendAutoparkDebug(void)
//{
//    u8 buff[7] = {0,0,0,0,0,0,0};
//    u8 dlc = 7;
//
//    buff[0] = P2GPA_autopark_requested_gear;
//    buff[0] += (P2GPA_maneuver_ended << 7) & _1000_0000b; // bit 7
//
//    buff[1] = Enable_gear_request ;
//    buff[1] += (paravan_brake << 7) & _1000_0000b;
//
//    buff[2] = P2GPA_autopark_status;
//    buff[2] += (P2GPA_purpose_N << 4) & _0011_0000b;
//    buff[2] += (P2GPA_autopark_main_status << 6) & _1100_0000b;
//
//    buff[3] += (P2GPA_Autopark_p4ubtn_start << 1) & _0000_0010b; // bit 25
//    buff[3] += (P2GPA_Autopark_p4ubtn_stop << 2) & _0000_0100b; // bit 26
//    buff[3] += (P2GPA_autopark_enabled << 3) & _0000_1000b; // bit 27
//    buff[3] += (autopark_p4ubtn_start_aux << 7) & _1000_0000b; // bit 31
//
//    buff[4] = (u8)(brake_pressure_autopark & 0x00ff);   // brake pressure
//    buff[5] = (u8)(brake_pressure_autopark >> 8);   // brake pressure
//    buff[6] = st_comh_buffer_can_data.status_eps_raw_data;
//
//    P2GPA_CanSend (P2GPA_CAN_prio_high, 0x610, buff, dlc);
//}


//void COMH_SendParavanInfo(void)
//{
//    u8 buff[2] = {0,0};
//    u8 dlc = 2;
//
//    /* tx message: control-position Gas/brake (big endian) */
//    if (TRUE == paravan_brake)
//    {
//      if (TRUE == autopark_p4ubtn_start_aux)
//      {
//        if (paravan_count < 60)  // paravan brake active during x seconds
//        {
//          paravan_count += 1;
//        }
//        else
//        {
//          paravan_count = 0;
//          paravan_brake = FALSE;
//        }
//      }
//
//      buff[0] = 0x20;
//      buff[1] = 0x20;
//    }
//
//    else
//    {
//      buff[0] = 0x40;
//      buff[1] = 0x00;
//    }



//    P2GPA_CanSend (P2GPA_CAN_prio_high, 0x21, buff, dlc);
//}

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
    memset(&st_comh_buffer_can_data, 0, sizeof(st_comh_buffer_can_data));

    /* initialize application data buffer */
    memset(&st_comh_buffer_appl_data, 0, sizeof(st_comh_buffer_appl_data));
    /* initialize irpt data buffer */
    memset(&st_comh_buffer_can_irpt_data, 0, sizeof(st_comh_buffer_can_irpt_data));

    st_comh_copy_in_can_irpt = FALSE;

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
 * void COMH_Cyclic10ms(void)
 *
 * cyclic 10ms Task
 *
 */
void COMH_Cyclic10ms(void)
{
    #if 0
    /* send the steering message  */
    CanSendSteeringMsg();
	 /*send brake MSG*/
    CanSendbrakingMsg();
    #endif
}


/**
 * void COMH_Cyclic20ms(void)
 *
 * cyclic 20ms Task
 *
 */
void COMH_Cyclic20ms(void)
{
    SynchronizeTask();

    EvaluateButtonsState();
    CalculateVehicleStandstill();

#ifdef DAIMLER_BR_213_PLATFORM
    /* send the CAN data to gateway for Daimler BR213 gateway */
    /* send the steering message  */
    CanSendSteeringMsg();
	 /*send brake MSG*/
    CanSendbrakingMsg();
    /* Send parkguid & park style message */
    CanSendParkStat();
    /* send RVC CPF message */
    CanSendSVSCPFRqMsg();
    /* Send warn sounds */
    CanSendLedTong();
    /* Send park display requst */
    CanSendParkDispRq();
    /* Send remote park request */
    CanSendRemParkRq();

    /* Handling of mPLA_Status message */
    CanSendPlaStatus();

    Send_Debug_Msg();

    /* Send alive message for Peiker ECU */
    Send_ATM_NM();


#else

    /* Handling of mPLA_Status message */
    CanSendPlaStatus();
    CanSendParkhilfe5();
    CanSendLedTong();

#ifdef APPL_ENABLE_SEND_ODOM_INFO
    P2GPA_CanOdomInfoSend();
#endif

#ifdef APPL_SECT_CRITICALITY_TO_CAN
    CanSendSectorCriticality();
#endif

    Send_Debug_Msg();
#endif

}




/**
 * u16 COMH_GetWheelCircumference(void)
 * gets wheel circumference
 *
 * \return Radumfang aus Botschaft mKombi_3 in mm (0..4096mm)
 */
u16 COMH_GetWheelCircumference(void)
{
  return st_comh_buffer_appl_data.wheel_circumference_raw_data;
}


void COMH_Clear_BLE_MSGS(void)
{
    st_comh_buffer_can_data.ble_msg.BLE_ID_From = 0;
    st_comh_buffer_can_data.ble_msg.BLE_MsgCounter=0;
    st_comh_buffer_can_data.ble_msg.SP_CurrentState=0;
    st_comh_buffer_can_data.ble_msg.SP_DeadmanConclusion=0;
    st_comh_buffer_can_data.ble_msg.SP_DeadmanX=0;
    st_comh_buffer_can_data.ble_msg.SP_DeadmanY=0;
    st_comh_buffer_can_data.ble_msg.SP_NewManeuverRequest=0;
    st_comh_buffer_can_data.ble_msg.SP_RiskAcknowledgment=0;
    st_comh_buffer_can_data.ble_msg.SP_SmartphoneConnected=0;
    st_comh_buffer_can_data.ble_msg.SP_UserAbortRequest=0;
    st_comh_buffer_can_data.ble_msg.park_MsgType=0;

}

/**
 * u8 COMH_GetImpulseWheelNumberOfTeeth(void)
 *
 * \return Zaehnezahl der Impulsraeder aus Botschaft mBremse_2
 */
u8 COMH_GetImpulseWheelNumberOfTeeth(void)
{
    return st_comh_buffer_appl_data.impulsewheel_number_of_teeth;
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
	// TODO stubbed in Daimler to be Implemented in Gate way
	return TRUE;
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
  return st_comh_buffer_appl_data.is_vehicle_standstill;
}


/******************************************************************************/
/**
 * Provides the vehicle position if EXTERNAL_ODOMETRY is used
 * \param[out] p_ext_pos   pointer to the external position data
 *
 * \return E_OK if returned data is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetExternalPosition(struct ODOC_ext_veh_pos_S* p_ext_pos)
{
    /* In PQ36 car plattform the information is not available (=> only implemented in testenvironment) */
    si32 temp_si32;
    u16  temp_u16;
    si16 temp_si16;

    p_ext_pos->timestamp_2us = (st_comh_buffer_appl_data.ext_veh_position_timestamp_2us >> 8) & 0x00FFFFFFUL;

    /* raw-data: */
    /*   u16 ext_veh_angle_raw_data:      1bit = 1/100 degree (-327,68 .. 327,67degree) */
    /*   u16 ext_veh_x_pos_raw_data:      1bit = 1 mm (-32768 .. 32767mm) */
    /*   u16 ext_veh_y_pos_raw_data:      1bit = 1 mm (-32768 .. 32767mm) */
    temp_u16 = st_comh_buffer_appl_data.ext_veh_angle_raw_data;
    if (temp_u16 > 32767)
    {
        temp_u16 = (~temp_u16) + 1;
        temp_si16 = (-1) * temp_u16;
    }
    else
    {
        temp_si16 = (si16)temp_u16;
    }
    p_ext_pos->angle_u16f = Conv100thDeg2U16f(temp_si16);

    temp_u16 = st_comh_buffer_appl_data.ext_veh_x_pos_raw_data;
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

    temp_u16 = st_comh_buffer_appl_data.ext_veh_y_pos_raw_data;
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


bool_T send_steer_completion = TRUE;
/**
 * void COMH_CanSendSteeringMsg(void)
 *
 * Function to send the mPLA_1 - CAN message
 *
 */
static void CanSendSteeringMsg(void)
{

    u8   	temp_PARK_Stat_PARK;
    si16 	temp_FRWheelAngle_Req;
    uint16 	temp_FRWheelAngle_Req_to_send;
    u8 buff[8] = {0,0,0,0,0,0,0,0};

    uint8 SerDataDEData[(64U)/8U];

    u8 dlc = 8;

    St_Rq_PARK_Pr2 rq_park_pr2;
    memset(&rq_park_pr2 , 0 , sizeof(rq_park_pr2));

    temp_PARK_Stat_PARK 			= STMH_CusGetAppStateToSteering();
    temp_FRWheelAngle_Req 			= P2DAL_GetCmdSteeringOutputFront();

#ifdef E2E
    temp_PARK_EPS_CRC 				= COMH_CALC_CRC();
    temp_PARK_EPS_SQC 				= COMH_CALC_SQC();
#endif

    if (STMH_CusIsSteeringActive() )
    {
    	temp_FRWheelAngle_Req_to_send 	= (uint16)(((si32)((si32)temp_FRWheelAngle_Req * (si16)1125) / (si16)1024) + (si32)16384);
    }
    else
    {
    	temp_FRWheelAngle_Req_to_send = (uint16)(((si32)((si32)st_comh_buffer_appl_data.steering_angle * (si16)1125) / (si16)1024) + (si32)16384);
    }



    /* Steer_Re_PARK CAN message */

    buff[0]  = (u8)(temp_FRWheelAngle_Req_to_send 			& 0xff	);
    buff[1]  = (u8)((temp_FRWheelAngle_Req_to_send >> 8) 	& 0x7f	);
    buff[2]  = (u8)(temp_PARK_Stat_PARK 					<< 2	);
    buff[2] &= (u8)(	0x3C	); 												/* Stub PARK_Sp_Stat_PARK to IDLE */
    buff[3] = (u8)0u; /* stub additional rack force to 0 physical -> 0x1000 hexadecimal*/
    buff[4] = (u8)0x10; /* stub additional rack force to 0 physical -> 0x1000 hexadecimal*/
    buff[5] = (u8)1u; /* stub PPS stat to disabled */

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

    /*Senc CRC & SQC on CAN from serDataEData SerDataDEData[1] - > SQC & SerDataDEData[0] -> CRC*/
    buff[6] = SerDataDEData[0];

    buff[7] = SerDataDEData[1] & 0x0F;

    P2GPA_CanSend (P2GPA_CAN_prio_high, CAN_ID_STEER_REQ_PARK, buff, dlc);

}
/**
 * void COMH_CanSendbrakingMsg(void)
 *
 * Function to send the BRK_Re_PARK - CAN message
 *
 */
static void CanSendbrakingMsg(void)
{
	 u8      temp_Park_DangerMd_Rq;
	 si16   	temp_PARK_Veh_Spd_Re;
    u8 		temp_PARK_TXDrvPosn_Rq;
    u8 		temp_PARK_Park_Stat;
    u8 		temp_PARK_BrkMD_Rq;
    u8	 	temp_PARK_ParkType;
    u8 		temp_PARK_CtrlMD_Rq;
    u8 		temp_PARK_RemainDist_Stat;
    si16   	temp_PARK_RemainDist;
    u8 		temp_PARK_Brk_Stat;

    u8 buff1[8] = {0,0,0,0,0,0,0,0};
    memset(buff1, 0u, 8u);

    u8 dlc = 8;

    uint8 brk_reqSerDataDEData[(64U)/8U];

    /*PDU of Brk_Rq_Park_Pr2*/
    Brk_Rq_PARK_Pr2 brk_rq_park_pr2 ;


    /*memset Brk Rq PDU with zeros */
    memset(&brk_rq_park_pr2 , 0 , sizeof(brk_rq_park_pr2));

    /* Steer_Re_PARK message data */
    temp_Park_DangerMd_Rq 		= (u8) BRKH_GetParkDangerMdRq(); /* no need for switch case, 0 is inactive (false), 1 is active (true)*/
    temp_PARK_Veh_Spd_Re 		= (BRKH_GetVehSpdRq() / 10u) /* converting signal back to 0.1 resolution from 0.01 */;
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
    temp_PARK_Park_Stat			= (u8)BRKH_GetParkParkStat();
    temp_PARK_BrkMD_Rq 			= (u8)BRKH_GetParkBrkMdRq();
    temp_PARK_ParkType 			= (u8)BRKH_GetParkParkType();
    temp_PARK_CtrlMD_Rq 		= (u8)BRKH_GetParkCtrlMdRq();
    temp_PARK_RemainDist_Stat 	= (u8)BRKH_GetRemainDistStat();
    temp_PARK_RemainDist 		= BRKH_GetRemainDist();

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
    buff1[0]  = ((u8)temp_Park_DangerMd_Rq 		<< 7 	);
    buff1[0] |= ((u8)temp_PARK_Veh_Spd_Re 		& 0x7F);
    buff1[1]  = ((u8)temp_PARK_TXDrvPosn_Rq 		<< 4 	);
    buff1[1] |= ((u8)temp_PARK_Park_Stat 		& 0x0F 	);
    buff1[2]  = ((u8)temp_PARK_BrkMD_Rq 			<< 6 	);
    buff1[2] |= ((u8)temp_PARK_ParkType 			<< 3 	);
    buff1[2] |= ((u8)temp_PARK_CtrlMD_Rq 		& 0x07	);
    buff1[3]  = ((u8)temp_PARK_RemainDist_Stat 		 	);
    buff1[4]  = (u8)(temp_PARK_RemainDist 	& 0xff	);
    buff1[5]  = (u8)((temp_PARK_RemainDist >> 8) & 0x07	);
    buff1[5] |= ((u8)temp_PARK_Brk_Stat 			<< 3	);

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
    buff1[6] = brk_reqSerDataDEData[0];

    buff1[7] = brk_reqSerDataDEData[1] & 0x0F;


    /*Send BRK_Req_PARK CAN message */
    P2GPA_CanSend (P2GPA_CAN_prio_high, CAN_ID_BRK_REQ_PARK, buff1, dlc);

}


/**
 * void CanSendSVSCPFRqMsg(void)
 *
 * Function to send the RVC CFP - CAN message
 *
 */
static void CanSendSVSCPFRqMsg(void)
{
   u8 buff[8] = {0,0,0,0,0,0,0,0};
   u8 dlc;
   enum IPC_PLAY_MODE play_mode;

   dlc = 4u;
   if (COMH_GetKl15() == KL15_ON)
   {
      buff[0] = 0x15; /* stub CPF to be always open, exploration mode to 1, ext lmp rq to 1, if KL15 is ON*/
   }
   else
   {
      buff[0] = 0u; /* close CPF, exploration mode & ext lmp rq if KL15 is off */
   }

   /* set PARK_SysMd_Rq_Cam & Park_Row1_MsgDisp_Rq values according to VIDC */
   PLATFORM_ReadIPCPlayMode(&play_mode);
   switch(play_mode)
   {
      case PLAY_MODE_OFF_ABORT:
      case PLAY_MODE_OFF:
         buff[1] = 9u;
         buff[2] = 0x2A; /* set Park_Row1_MsgDisp_Rq text to front */
         break;
      case PLAY_MODE_TOPVIEW_REARVIEW:
         buff[1] = 2u;
         buff[2] = 0x2B; /* set Park_Row1_MsgDisp_Rq text to rear */
         break;
      case PLAY_MODE_REARVIEWFULLSCREEN:
         buff[1] = 3u;
         buff[2] = 0x2F; /* set Park_Row1_MsgDisp_Rq text to rear wide angle */
         break;
      case PLAY_MODE_TOPVIEW_TRAILERCOUPLINGVIEW:
         buff[1] = 2u;
         buff[2] = 0x2B; /* set Park_Row1_MsgDisp_Rq text to rear */
         break;
      case PLAY_MODE_TOPVIEW_SIDEVIEWFORWARD:
         buff[1] = 7u;
         buff[2] = 0x2A; /* set Park_Row1_MsgDisp_Rq text to front */
         break;
      case PLAY_MODE_TOPVIEW_FRONTVIEW:
         buff[1] = 8u;
         buff[2] = 0x2A; /* set Park_Row1_MsgDisp_Rq text to front */
         break;
      case PLAY_MODE_FRONTVIEWFULLSCREEN:
         buff[1] = 9u;
         buff[2] = 0x2E; /* set Park_Row1_MsgDisp_Rq text to front wide angle */
         break;
      case PLAY_MODE_APCVIEW_REARVIEW:
         buff[1] = 11u;
         buff[2] = 0x2B; /* set Park_Row1_MsgDisp_Rq text to rear */
         break;
      case PLAY_MODE_APCVIEW_WALLPAPER:
         buff[1] = 8u; /* set to TV FV */
         buff[2] = 0x2A; /* set Park_Row1_MsgDisp_Rq text to front */
         break;
      case PLAY_MODE_OFF_BY_SPD_FS:
         buff[1] = 1u;
         buff[2] = 0x2A; /* set Park_Row1_MsgDisp_Rq text to front */
         break;
      default:
         buff[1] = 2u;
         buff[2] = 0x2A; /* set Park_Row1_MsgDisp_Rq text to front */
         break;
   }

   buff[3] = 0u; /* stub DRVM_Warn_Rq to 0 */

   /* Send RVC_CPF_Request_Msg_PARK message */
   P2GPA_CanSend (P2GPA_CAN_prio_high, CAN_ID_PARK_SVS_CPF_Rq, buff, dlc);
}

/**
 * void CanSendParkStat(void)
 *
 * Function to send the parkguid stat & park style- CAN message
 ****************************************************
 *********Park_ParkGuid_Stat_PARK Message ************
 ****************************************************
 *
 */
 static void CanSendParkStat(void)
 {
    static u8 taskCounter = 0u; /* periodicity is 200 ms, so send message every 10 task calls to keep SQC synchronized */
    u8        buff[8] = {0,0,0,0,0,0,0,0};
    u8        dlc = 4;
    u8 	  	  temp_PARK_ParkGuid_Stat;

    uint8   park_cfg_SerDataDEData[(32U)/8U];

    /*PDU of ParkCfg_Stat_Pr2*/
    ParkCfg_Stat_Pr2 park_cfg_stat_pr2;
   if(taskCounter == 10u)
   {

      /*memset ParkCfg_Stat_Pr2 PDU with zeros */
       memset(&park_cfg_stat_pr2 , 0 , sizeof(park_cfg_stat_pr2));

       /* Park_ParkGuid_Stat_PARK message data */
       temp_PARK_ParkGuid_Stat   = BRKH_GetParkGuidStat();


       /* Park_ParkGuid_Stat_PARK message */
       buff[0] = ((u8)temp_PARK_ParkGuid_Stat);
       buff[1] = (u8)31u; /* stubbed as in parkman */
       /*Filling PDU struct to pass it to PDU SERL*/
       park_cfg_stat_pr2.ParkGuid_Stat = temp_PARK_ParkGuid_Stat;
       park_cfg_stat_pr2.ParkStyle_V2 = (u8)31u; /* stubbed as in parkman */
       park_cfg_stat_pr2.Rsrv1_ParkCfg_Stat_Pr2 = (u8)0u;
       park_cfg_stat_pr2.Rsrv2_ParkCfg_Stat_Pr2 = (u8)0u;

       /*call SERL FUNC for this PDU*/
       Cfg_ParkCfg_Stat_Pr2.e2e_serl_func(&park_cfg_stat_pr2 , park_cfg_SerDataDEData);

       /*Calculate E2E header*/
       E2E_EB_P02Protect(&Cfg_ParkCfg_Stat_Pr2,&parkcfg_stat_pr2_sender_state, park_cfg_SerDataDEData);

       buff[2] = ((u8)park_cfg_SerDataDEData[0]);
       buff[3] = ((u8)park_cfg_SerDataDEData[1] & 0x0F);

      taskCounter = 0u;

       /* Send Park_ParkGuid_Stat_PARK message */
       P2GPA_CanSend (P2GPA_CAN_prio_high, CAN_ID_PARK_PARKGUID_STAT_PARK, buff, dlc);
   }
   else
   {
      taskCounter++;
   }
 }


 /**
 * void CanSendParkDispRq(void)
 *
 * Function to send the park display request CAN message
 ****************************************************
 *************PARK_DISP_RQ_PARK Message *************
 ****************************************************
 *
 */
 static void CanSendParkDispRq(void)
 {
   static u8               taskCounter = 0u; /* periodicity is 200 ms, so send message every 10 task calls to keep SQC synchronized */
   u8                      buff[8] = {0,0,0,0,0,0,0,0};
   u8                      dlc = 8u;
   Park_Disp_Rq_AR2        park_disp_rq_ar2;
   u8                      park_disp_rq_SerDataDEData[(64u/8u)];

   memset(&park_disp_rq_ar2, 0, sizeof(Park_Disp_Rq_AR2));

   if(taskCounter == 10u)
   {

      /* perform PDU filling for Park_Disp_Rq_AR2 */

     /* save park icon display request according to ESP & EPS states to indicate errors */
#ifdef XAPPL_LOT_CNTRL_SUPP
     /* set parc icon display to search if there are no ESP faults (if LOT is enabled) & EPS is not
        in disabled or cancel_sys states */
     if(  st_comh_buffer_appl_data.park_flt_stat_esp != ESP_STATE_ESP_FLT
       && st_comh_buffer_appl_data.park_flt_stat_esp != ESP_STATE_PM_FLT
       && st_comh_buffer_appl_data.park_flt_stat_esp != ESP_STATE_SPCR_FLT
       && st_comh_buffer_appl_data.park_flt_stat_esp != ESP_STATE_EPB_FLT
       && st_comh_buffer_appl_data.park_flt_stat_esp != ESP_STATE_TCM_FLT
       && st_comh_buffer_appl_data.park_flt_stat_esp != ESP_STATE_ECM_FLT
       && st_comh_buffer_appl_data.park_flt_stat_esp != ESP_STATE_RPA_FLT
       && (st_comh_buffer_appl_data.status_eps_raw_data == 1u
           || st_comh_buffer_appl_data.status_eps_raw_data == 2u)
       && (0 == COMH_GetPlaTerminationOfEps())
       && (ESP_INTERVENTION_INACTIVE == COMH_GetESPIntervention())
       && ((BRAKING_STATE_PARKMAN_INACTIV == COMH_GetEspBrakeState())
           || (BRAKING_STATE_APC_MODE == COMH_GetEspBrakeState()))
       && (1 == ((COMH_GetParkEnblStatEsp()) & 0x01))    )
       {
             park_disp_rq_ar2.Park_IconDisp_Rq = 1u;
             buff[2] = 1u;
       }
#else
     if(st_comh_buffer_appl_data.status_eps_raw_data == 1u
         || st_comh_buffer_appl_data.status_eps_raw_data == 2u)
     {
        park_disp_rq_ar2.Park_IconDisp_Rq = 1u;
        buff[2] = 1u;
     }
#endif
     else
     {
        park_disp_rq_ar2.Park_IconDisp_Rq = 0u;
        buff[2] = 0u;
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
      buff[0] = (u8)park_disp_rq_SerDataDEData[0];
      buff[1] = (u8)(park_disp_rq_SerDataDEData[1] & 0x0F);

      taskCounter = 0u;

      /* Send PARK_DISP_RQ_PARK message */
      P2GPA_CanSend (P2GPA_CAN_prio_high, CAN_ID_PARK_DISP_RQ_PARK, buff, dlc);
   }
   else
   {
      taskCounter++;
   }
 }

 /**
 * void CanSendRemParkRq(void)
 *
 * Function to send the remote park request CAN message
 ****************************************************
 ************** RemPark_Rq_PARK Message *************
 ****************************************************
 *
 */
 static void CanSendRemParkRq(void)
 {
   static u8      taskCounter = 0u; /* periodicity is 200 ms, so send message every 10 task calls to keep SQC synchronized */
   RemPark_Rq_Pr2 rempark_rq_pr2;
   u8             buff[8] = {0,0,0,0,0,0,0,0};
   u8             dlc = 3u;         /* dlc is kept as 2 as only CRC & SQC are sent */
   u8             rempark_rq_SerDataDEData[(32u/8u)];

   memset(&rempark_rq_pr2, 0, sizeof(RemPark_Rq_Pr2));


   if(taskCounter == 10u)
   {

     buff[0] = 1u; /* EIS_RemPark_Rq stubbed as per parkman */

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
      buff[1] = (u8)rempark_rq_SerDataDEData[0];
      buff[2] = (u8)(rempark_rq_SerDataDEData[1] & 0x0F);

      taskCounter = 0u;
      /* Send RemPark_Rq_PARK message */
      P2GPA_CanSend (P2GPA_CAN_prio_high, CAN_ID_REMPARK_RQ_PARK, buff, dlc);
   }
   else
   {
      taskCounter++;
   }
 }

#if 0 // NOT FOR DAIMLER
/**
 * void COMH_GetSteeringMomentAndSign(u16 *moment, u8 *sign)
 *
 * \param[in/out] moment: 1Bit = 15/1024Nm
 * \param[in/out] sign  : 0 = positive, 1 = negative
 */
void COMH_GetSteeringMomentAndSign(u16 *moment, u8 *sign)
{
  /* Gueltiges Lenkmoment */
  *moment = st_comh_buffer_appl_data.steering_moment_raw_data;
  *sign   = st_comh_buffer_appl_data.steering_moment_sign_raw_data;
}
#endif



/**
 * u8 COMH_GetStatusEps(void)
 *
 * \return EPS status
 */
u8 COMH_GetStatusEps(void)
{
  return st_comh_buffer_appl_data.status_eps_raw_data;
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
  *angle = st_comh_buffer_appl_data.steering_angle_raw_data;
  *sign  = st_comh_buffer_appl_data.steering_angle_sign_raw_data;
}



/**
 * u8 COMH_GetPlaTerminationOfEps(void)
 *
 * \return
 */
u8 COMH_GetPlaTerminationOfEps(void)
{
  return st_comh_buffer_appl_data.pla_termination_raw_data;
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
  return st_comh_buffer_appl_data.wheel_speed_raw_data;
}



/**
 * u8 COMH_GetSpeedBackupValueActive(void)
 *
 * \return if there is is given back an alternative speed value instead of the
 * real velocity
 */
u8 COMH_GetSpeedBackupValueActive(void)
{
    return st_comh_buffer_appl_data.wheel_speed_backup_value_active;
}



/**
 * u8 COMH_GetAsrRequest(void)
 *
 * \return 0 if there is not ASR request, 1 if there is an ASR request
 */
u8 COMH_GetAsrRequest(void)
{
    return st_comh_buffer_appl_data.asr_request_raw_data;
}



/**
 * u8 COMH_GetMsrRequest(void)
 *
 * \return 0 if there is no MSR request, 1 if there is a MSR request
 */
u8 COMH_GetMsrRequest(void)
{
    return st_comh_buffer_appl_data.msr_request_raw_data;
}



/**
 * u8 COMH_GetEdsIntervention(void)
 *
 * \return 0 if there is no EDS intervention, 1 otherwise
 */
u8 COMH_GetEdsIntervention(void)
{
    return st_comh_buffer_appl_data.eds_intervention_raw_data;
}



/**
 * u8 COMH_GetAbsIntervention(void)
 * returns if there is and ABS intervention
 * \return 1 if there is an intervention, 0 otherwise
 */
u8 COMH_GetAbsIntervention(void)
{
    return st_comh_buffer_appl_data.abs_intervention_raw_data;
}



/**
 * u8 COMH_GetEspIntervention(void)
 *
 * \return 1 if there is an ESP Intervention, 0 if there is no intervention
 */
u8 COMH_GetEspIntervention(void)
{
    return st_comh_buffer_appl_data.esp_intervention_raw_data;

}



/**
 * u8 COMH_GetEspSystemStatusError(void)
 *
 * Returns if the ESP-SystemStatus reports an error
 * \return 0: no error, != 0 ESP Systemstatus reports an error
 */
u8 COMH_GetEspSystemStatusError(void)
{
    return st_comh_buffer_appl_data.esp_system_status_raw_data;
}



/**
 * u8 COMH_GetEspAsrDisabled(void)
 * Returns if the ASR is disabled by driver
 *
 * \return 0 if ASR is enabled, != 0 if ASR is disabled
 */
u8 COMH_GetEspAsrDisabled(void)
{
    return st_comh_buffer_appl_data.esp_asr_passive_raw_data;
}



#if 0 // NOT FOR DAIMLER
// TODO SEEMS TO BE DOUBLE OF COMH_GetLongAcceleration
/**
 * si16 COMH_GetLongitudinalAcceleration(void)
 *
 * \return longitudinal acc, res. 1Bit = 1mm/s^2
 */
si16 COMH_GetLongitudinalAcceleration(void)
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

    temp_long_acc = st_comh_buffer_appl_data.long_acc_phys_data;
    /* convert 1/32m/s^2 into 1mm/s^2 */
    temp_long_acc = (si16)(((si32)temp_long_acc * 1000L) / 32L);

    return temp_long_acc;
}
#endif // NOT FOR DAIMLER



/**
 * u16 COMH_GetWheelImpulsesFrontLeft(void)
 *
 * \return the wheelimulses of the front left wheel
 */
u16 COMH_GetWheelImpulsesFrontLeft(void)
{
    /* return wheel impulse for front right */
    return st_comh_buffer_appl_data.wheel_impulses.wheel_impulses_fl;
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
    return st_comh_buffer_appl_data.wheel_impulses.wheel_impulses_fr;
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
    return st_comh_buffer_appl_data.wheel_impulses.wheel_impulses_rl;
}




/**
 * u16 COMH_GetWheelImpulsesRearRight(void)
 *
 * \return Returns the wheelimulses of the rear right wheel
 */
u16 COMH_GetWheelImpulsesRearRight(void)
{
    /* return wheel impulse for rear right */
    return st_comh_buffer_appl_data.wheel_impulses.wheel_impulses_rr;

}

u8 COMH_GetWheelImpulseMsgCounter(u8 *msg_counter)
{
    *msg_counter = st_comh_buffer_can_data.wheel_impulses.msg_count;

    return 0;
}


#if 0 // NOT FOR DAIMLER
/**
 * bool_T COMH_IsWheelImpValid(void)
 *
 * Returns if all wheel-impulses (of all 4 wheels) are once valid received after
 * initialisation (Reset/Re-Init).
 * This is for example used for correct start-up behaviour of the
 * odometrie module (POFU).
 *
 * \return if all wheel-impulses are valid received (TRUE) or not (FALSE)
 */
bool_T COMH_IsWheelImpValid(void)
{
    bool_T ret_value = FALSE;

    if ( (st_comh_buffer_appl_data.wheel_impulses.valid_fl) &&
         (st_comh_buffer_appl_data.wheel_impulses.valid_fr) &&
         (st_comh_buffer_appl_data.wheel_impulses.valid_rl) &&
         (st_comh_buffer_appl_data.wheel_impulses.valid_rr) )
    {
        ret_value = TRUE;
    }
    return ret_value;
}
#endif // NOT FOR DAIMLER




#if 0 // NOT FOR DAIMLER
/**
 *
 * enum COMH_wheel_info_E COMH_GetWheelInfoRearLeft(void)
 *
 * returns all available informations about the rear left wheel
 * (standstill / roll-direction forward or backward / invalid)
 * Actually only forward and backward supported
 *
 * \return infos about the rear left wheel (enum COMH_wheel_info_E).
 */
enum COMH_wheel_info_E COMH_GetWheelInfoRearLeft(void)
{
    /* return wheel info for rear left side */
    return st_comh_buffer_appl_data.wheel_info_rl_raw_data;
}
#endif // NOT FOR DAIMLER





#if 0 // NOT FOR DAIMLER
/**
 * enum COMH_wheel_info_E COMH_GetWheelInfoRearRight(void)
 *
 * Returns all available informations about the rear right wheel
 * (standstill / roll-direction forward or backward / invalid)
 * Actually only forward and backward supported
 *
 * \return infos about the rear right wheel (enum COMH_wheel_info_E).
 */
enum COMH_wheel_info_E COMH_GetWheelInfoRearRight(void)
{
    /* return wheel info for rear right side */
    return st_comh_buffer_appl_data.wheel_info_rr_raw_data;
}
#endif // NOT FOR DAIMLER



#if 0 // NOT FOR DAIMLER
/**
 * enum COMH_wheel_info_E COMH_GetWheelInfoFrontLeft(void)
 *
 *  Returns all available informations about the front left wheel
 *  (standstill / roll-direction forward or backward / invalid)
 *  Actually only forward and backward supported
 *
 *  \return infos about the front left wheel (enum COMH_wheel_info_E).
 */
enum COMH_wheel_info_E COMH_GetWheelInfoFrontLeft(void)
{
    /* return wheel info for front left side */
    return st_comh_buffer_appl_data.wheel_info_fl_raw_data;
}
#endif // NOT FOR DAIMLER



#if 0 // NOT FOR DAIMLER
/**
 * enum COMH_wheel_info_E COMH_GetWheelInfoFrontRight(void)
 *
 * \return infos about the front right wheel (enum COMH_wheel_info_E)
 *
 * \description
 * Returns all available informations about the front right wheel
 * (standstill / roll-direction forward or backward / invalid)
 * Actually only forward and backward supported
 *
 */
enum COMH_wheel_info_E COMH_GetWheelInfoFrontRight(void)
{
    /* return wheel info for rear left side */
    return st_comh_buffer_appl_data.wheel_info_fr_raw_data;
}
#endif // NOT FOR DAIMLER



#if 0 // NOT FOR DAIMLER
/**
 * u32 COMH_GetStWheelAngleTime512us(void)
 * Returns the TCNT counter value (right shift 8) set in the mLenkhilfe_3 indication function
 *
 * \return the TCNT counter value
 */
u32 COMH_GetStWheelAngleTime512us(void)
{
  return ((st_comh_buffer_appl_data.st_wheel_angle_time_2us >> 8) & 0x00FFFFFFUL);
}
#endif // NOT FOR DAIMLER



#if 0 // NOT FOR DAIMLER
/**
 * COMH_GetWheelImpTime512us
 *
 * \return wheel impulse time, resolution 512us
 *
 * \description
 * Returns the TCNT counter value (right shift 8) set in the mBremse_10 indication function
 */
u32 COMH_GetWheelImpTime512us(void)
{
  return ((st_comh_buffer_appl_data.wheel_impulses.timestamp_2us >> 8) & 0x00FFFFFFUL);
}
#endif // NOT FOR DAIMLER


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

    if (st_comh_buffer_appl_data.ignition_on == FALSE)
    {
      ret = KL15_OFF;
    }
    else
    {
      ret = KL15_ON;
    }

    return ret;

}


#if 0 // NOT FOR DAIMLER
/**
 * void COMH_GetYawSpeedAndSign(u16 *yaw_speed, u8 *sign)
 * Returns the yaw speed (Giergeschwindigkeit) and the sign
 * resolution: 1bit = 1/100 degree/s
 *
 * \param yaw_speed, resolution 1bit = 1/100 degree/s
 * \param sign  0 - positive (turn left) / 1 - negative (turn right)
 */
void COMH_GetYawSpeedAndSign(u16 *yaw_speed, u8 *sign)
{
  /* Gueltige Giergeschwindigkeit */
  *yaw_speed = st_comh_buffer_appl_data.yaw_speed_raw_data;
  *sign      = st_comh_buffer_appl_data.yaw_speed_sign_raw_data;
}
#endif // NOT FOR DAIMLER



/**
 * u8 COMH_GetStateBrakeEcu(void)
 * Returns the actual state of the brake ecu (handshake with APP ecu for
 * activation of the active braking control)
 *
 * \return the state of the brake ecu (0..15dez)
 */
u8 COMH_GetStateBrakeEcu(void)
{
  return st_comh_buffer_appl_data.brake_ecu_state;
}



/**
 * u8 COMH_GetAppBrakeAbortReason(void)
 * Returns the abort reason if the brake ecu aborts an active braking control
 *
 * \return the abort reason of the brake ecu (0..7dez)
 */
u8 COMH_GetAppBrakeAbortReason(void)
{
  return st_comh_buffer_appl_data.brake_ecu_abort_reason;
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

  if ( (st_comh_buffer_appl_data.long_acc_qb_err_count > temp_invalid_time_10ms)
       || (st_comh_buffer_appl_data.long_acc_qb_count > temp_invalid_time_10ms)
       || (st_comh_buffer_appl_data.long_acc_err_count > temp_invalid_time_10ms) )
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
    *steering_wheel_moment = (si16)(st_comh_buffer_appl_data.steering_moment_raw_data);
    /* handle sign */
    if (st_comh_buffer_appl_data.steering_moment_sign_raw_data != 0)
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
    *is_esp_asr_disabled = (bool_T)st_comh_buffer_appl_data.esp_asr_passive_raw_data;
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
        temp_door_open = st_comh_buffer_appl_data.sliding_door_open_right;
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
        temp_door_open = st_comh_buffer_appl_data.sliding_door_open_left;
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
    *is_brake_ecu_standstill = (bool_T)st_comh_buffer_appl_data.brake_is_standstill;
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

    if ((st_comh_buffer_appl_data.asr_request_raw_data != 0) ||
        (st_comh_buffer_appl_data.msr_request_raw_data != 0) ||
        (st_comh_buffer_appl_data.eds_intervention_raw_data != 0) ||
        (st_comh_buffer_appl_data.abs_intervention_raw_data != 0) ||
        (st_comh_buffer_appl_data.esp_intervention_raw_data != 0) ||
        (st_comh_buffer_appl_data.esp_asr_passive_raw_data != 0))
    {
        temp_brake_intervention = TRUE;
    }
    *is_brake_intervention = temp_brake_intervention;
    return E_OK;
}

#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides the state of the Acceleration Interface. Needed for synchronization with Park4U control logic.
 * \param[out] acceleration_abort  ...
 * \param[out] acceleration_state  ...
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetStateAccelerationSys(enum DAPM_ext_sys_abort_E* acceleration_abort,
                                            enum DAPM_ext_sys_state_E* acceleration_state)
{
    /* TODO-KI: currently not used */
    return E_NOT_OK;
}
#endif // NOT FOR DAIMLER




#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides the state of the electrical parking brake (EPB) system. Needed for
 * synchronization with Park4U control logic.
 * \param[out] epb_abort  ...
 * \param[out] epb_state  ...
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetStateEpbSys(enum DAPM_ext_sys_abort_E* epb_abort, enum DAPM_ext_sys_state_E* epb_state)
{
    /* TODO-KI: currently not used */
    return E_NOT_OK;
}
#endif // NOT FOR DAIMLER

#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides the state of the electrical power steering (EPS) system. Needed for
 * synchronization with Park4U control logic.
 * \param[out] eps_abort  ...
 * \param[out] eps_state  ...
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetStateEpsSys(enum DAPM_ext_sys_abort_E* eps_abort, enum DAPM_ext_sys_state_E* eps_state)
{
    /* TODO-KI: currently not used */
    *eps_abort = DAPM_ES_ABORT_NONE;
    *eps_state = DAPM_ES_STATE_NOT_AVAILABLE;
    return E_NOT_OK;
}
#endif // NOT FOR DAIMLER

#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides the state of the transmission system. Needed for synchronization with Park4U control logic.
 * \param[out] transmission_abort  ...
 * \param[out] transmission_state  ...
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetStateTransmissionSys(enum DAPM_ext_sys_abort_E* transmission_abort,
                                            enum DAPM_ext_sys_state_E* transmission_state)
{
    /* TODO-KI: currently not used */
    return E_NOT_OK;
}
#endif // NOT FOR DAIMLER

#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides if the Park4U button is currently pressed.
 * The function is intended for BUTH, to filter the information for the application.
 * If no button is installed or the button is connected discrete, then this function
 * returns always E_NOT_OK.
 * \param[out] is_park4u_button_pressed   TRUE if button is pressed, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_IsPark4UButtonPressed(bool_T* is_park4u_button_pressed)
{
    *is_park4u_button_pressed = st_comh_buffer_appl_data.park4u_button;
    return E_OK;
}
#endif // NOT FOR DAIMLER



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
Std_ReturnType COMH_GetWheelImpulse(u16* wheel_impulses, u32* time_stamp, enum ODOC_wheels_E wheel)
{
    Std_ReturnType ret_value;
    bool_T         temp_valid;


    switch (wheel)
    {
    case ODOC_WHEEL_FL:
        *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us_left;
        *wheel_impulses = st_comh_buffer_appl_data.wheel_impulses.wheel_impulses_fl;
        temp_valid = st_comh_buffer_appl_data.wheel_impulses.valid_fl;
        break;
    case ODOC_WHEEL_FR:
        *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us_right;
        *wheel_impulses = st_comh_buffer_appl_data.wheel_impulses.wheel_impulses_fr;
        temp_valid = st_comh_buffer_appl_data.wheel_impulses.valid_fr;
        break;
    case ODOC_WHEEL_RL:
        *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us_left;
        *wheel_impulses = st_comh_buffer_appl_data.wheel_impulses.wheel_impulses_rl;
        temp_valid = st_comh_buffer_appl_data.wheel_impulses.valid_rl;
        break;
    case ODOC_WHEEL_RR:
        *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us_right;
        *wheel_impulses = st_comh_buffer_appl_data.wheel_impulses.wheel_impulses_rr;
        temp_valid = st_comh_buffer_appl_data.wheel_impulses.valid_rr;
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
    *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us;

    temp_long_acc = st_comh_buffer_appl_data.long_acc_phys_data;
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
    *time_stamp = st_comh_buffer_appl_data.wheel_speed_timestamp_2us; /* TODO-KI: implement timestamp */
    *speed = st_comh_buffer_appl_data.wheel_speed_raw_data;
    return E_OK;
}

/******************************************************************************/
/**
 * Provides the abstracted gear lever position received from CAN. Gear lever
 */
Std_ReturnType COMH_GetGearLeverPosition(enum CAPP_gear_lever_position_E* gear_lever_pos)
{

    /* following values are modified for Daimler BR213 */
    switch (st_comh_buffer_appl_data.is_gear)
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
    *time_stamp = st_comh_buffer_appl_data.st_wheel_angle_time_2us;
    /* raw-data: */
    /*   u16 steering_angle_raw_data:      1 Bit = 0.15 degree */
    /*   u8  steering_angle_sign_raw_data: 0 = positive, 1 = negative */

    *steering_wheel_angle = st_comh_buffer_appl_data.steering_angle;
    return E_OK;
}

/**
 * Provides the  front SteeringWheelAngle.
 * by the brake ecu.
 * \param[out] steering_wheel_angle         SteeringWheelAngle in 0.001 degree / bit
 * \param[out] time_stamp    receive-timestamp of vehicle speed in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetSteeringWheelAnglePhys(si32* steering_wheel_angle, u32* time_stamp)
{
    *time_stamp = st_comh_buffer_appl_data.st_wheel_angle_time_2us;
    /* raw-data: */
    /*   u16 steering_angle_raw_data:      1 Bit = 0.15 degree */
    /*   u8  steering_angle_sign_raw_data: 0 = positive, 1 = negative */

    *steering_wheel_angle = st_comh_buffer_appl_data.steering_angle_phy_1000th;
    return E_OK;
}

#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides the steering angle (wheel angle) of the front wheels of the vehicle.
 * \note This value may not be available in every car. Use COMH_GetSteeringWheelAngle()
 *       in that case. Though a transformation with the steering table might be needed.
 * \param[out] wheel_angle  wheel angle of the front wheels in u16f / bit
 *                          (u16f = u16-format = 360 deg / 2^16 = roughly 0.005 deg)
 *                          Positive value: steering left, negative value: steering right.
 * \param[out] time_stamp   receive-timestamp of wheel_angle in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetFrontWheelSteerAngle(si16* wheel_angle, u32* time_stamp)
{
    /* TODO-KI: currently not used */
    return E_NOT_OK;
}
#endif // NOT FOR DAIMLER

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

#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides the position of the steering rack gear.
 * It is needed for Park4U odometry and driving tube lines. It is interchangeable
 * with angle of steering wheel and steering angle of front wheels.
 * The steering rack gear is the gear between the steering wheel and the steering
 * linkage of the wheels.
 * \param[out] steering_rack_gear_pos   steering rack pos. in 0.01mm / bit
 *                                      Positive value: steering left, negative value: steering right.
 * \param[out] time_stamp               receive-timestamp of steering rack pos. in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetSteeringRackGearPos(si16* steering_rack_gear_pos, u32* time_stamp)
{
    /* TODO-KI: currently not used */
    return E_NOT_OK;
}
#endif // NOT FOR DAIMLER

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
                                     enum ODOC_wheels_E wheel)
{
    enum COMH_wheel_info_E temp_comh_wheel_info;

    /* raw-data: */
    /*   enum COMH_wheel_info_E wheel_info_xx_raw_data */

    switch (wheel)
    {
    case ODOC_WHEEL_FL:
        /* is in the same CAN message as the wheel-impulses => same timestamp as wheel-impulses */
        *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us_left;
        temp_comh_wheel_info = st_comh_buffer_appl_data.wheel_info_fl_raw_data;
        break;
    case ODOC_WHEEL_FR:
        *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us_right;
        temp_comh_wheel_info = st_comh_buffer_appl_data.wheel_info_fr_raw_data;
        break;
    case ODOC_WHEEL_RL:
        *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us_left;
        temp_comh_wheel_info = st_comh_buffer_appl_data.wheel_info_rl_raw_data;
        break;
    case ODOC_WHEEL_RR:
        *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us_right;
        temp_comh_wheel_info = st_comh_buffer_appl_data.wheel_info_rr_raw_data;
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
Std_ReturnType COMH_GetWheelSpeed(si16* wheel_speed, u32* time_stamp, enum ODOC_wheels_E wheel)
{
    Std_ReturnType ret_val = E_NOT_OK;

    /* raw-data: */
    /*   u16 wheel_speed_xx:      1 Bit = 0.01km/h */
    switch (wheel)
    {
    case ODOC_WHEEL_FL:
        *time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us_left;
        *wheel_speed = st_comh_buffer_appl_data.wheel_speeds.wheel_speed_fl;
        ret_val = E_OK;
        break;
    case ODOC_WHEEL_FR:
        *time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us_right;
        *wheel_speed = st_comh_buffer_appl_data.wheel_speeds.wheel_speed_fr;
        ret_val = E_OK;
        break;
    case ODOC_WHEEL_RL:
        *time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us_left;
        *wheel_speed = st_comh_buffer_appl_data.wheel_speeds.wheel_speed_rl;
        ret_val = E_OK;
        break;
    case ODOC_WHEEL_RR:
        *time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us_right;
        *wheel_speed = st_comh_buffer_appl_data.wheel_speeds.wheel_speed_rr;
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
 * Provides the speed of the selected wheel in rpm.
 * \param[out] wheel_speed   wheel speed of the selected wheel in 1 rpm
 * \param[out] time_stamp    receive-timestamp of wheel speed in 2us / bit
 *                           (only one timestamp for all wheel speeds)
 * \param[in]  wheel         selected wheel to get wheel speed from
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetWheelSpeedRPM(float* wheel_speed, u32* time_stamp, enum ODOC_wheels_E wheel)
{
	float localSpeed;
	Std_ReturnType ret_val = E_NOT_OK;

//	*time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us;

	/* raw-data: */

	switch (wheel)
	{
	case ODOC_WHEEL_FL:
		localSpeed =((float)st_comh_buffer_appl_data.wheel_speeds.wheel_speed_fl_raw / (float)2);
		*time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us_left;
		*wheel_speed = localSpeed;
		ret_val = E_OK;
		break;

	case ODOC_WHEEL_FR:
		localSpeed = ((float)st_comh_buffer_appl_data.wheel_speeds.wheel_speed_fr_raw / (float)2);
		*time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us_right;
		*wheel_speed = localSpeed;
		ret_val = E_OK;
		break;

	case ODOC_WHEEL_RL:
		localSpeed = ((float)st_comh_buffer_appl_data.wheel_speeds.wheel_speed_rl_raw / (float)2);
		*time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us_left;
		*wheel_speed = localSpeed;
		ret_val = E_OK;
		break;

	case ODOC_WHEEL_RR:
		localSpeed = ((float)st_comh_buffer_appl_data.wheel_speeds.wheel_speed_rr_raw / (float)2);
		*time_stamp = st_comh_buffer_appl_data.wheel_speeds.timestamp_2us_right;
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
    *time_stamp = st_comh_buffer_appl_data.yaw_speed_timestamp_2us;

    #ifdef DAIMLER_BR_213_PLATFORM
    /* already saved with proper resolution */
    *yaw_speed = (si16)(st_comh_buffer_appl_data.yaw_speed_raw_data);

    #else
    /* raw-data: */
    /*   u16 yaw_speed_raw_data:      1bit = 1/100 degree/s */
    /*   u8  yaw_speed_sign_raw_data: 0 - positive (turn left), 1 - negative (turn right) */
   /* convert 1/100 deg/s into u16f/s */
    *yaw_speed = (si16)(((u32)st_comh_buffer_appl_data.yaw_speed_raw_data * (u32)2048) / (u32)1125);
    #endif

    /* handle sign TODO-KI: is this correct for U16-format ? */
    if (st_comh_buffer_appl_data.yaw_speed_sign_raw_data != 0)
    {
        (*yaw_speed) *= -1;
    }
    return E_OK;
}

/**
 * Provides the  Yaw rate.
 * by the brake ecu.
 * \param[out] yaw_speed         Yaw rate in 0.01 degree_per_S / bit
 * \param[out] time_stamp    receive-timestamp of vehicle speed in 2us / bit
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_GetYawRatePhys(float* yaw_speed, u32* time_stamp)
{
    *time_stamp = st_comh_buffer_appl_data.yaw_speed_timestamp_2us;


    /* already saved with proper resolution */
    *yaw_speed = st_comh_buffer_appl_data.yaw_physical_deg_p_s;

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
    *time_stamp = st_comh_buffer_appl_data.wheel_impulses.timestamp_2us;

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

    *outside_temperature = st_comh_buffer_appl_data.temp_filtered_raw_data;

    if (st_comh_buffer_appl_data.temperature_received == TRUE)
    {
        ret_val = E_OK;
    }

    return ret_val;
}


#if 0 // NOT FOR DAIMLER
/******************************************************************************/
/**
 * Provides if the PDC button is currently pressed.
 * This is needed for pure AreaView-variant for synchronization with a PDC-ECU.
 * The signal is then accessed by SYNC to make it transparent for the AreaView-composite
 * if it is integrated in IPA or just AreaView.
 * \param[out] is_pdc_button_pressed   TRUE if button is pressed, otherwise FALSE
 *
 * \return E_OK if value is valid, E_NOT_OK otherwise.
 */
Std_ReturnType COMH_IsPdcButtonPressed(bool_T* is_pdc_button_pressed)
{
    *is_pdc_button_pressed = st_comh_buffer_appl_data.upa_button;
    return E_OK;
}
#endif // NOT FOR DAIMLER


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
    *is_blinker_left_enabled = st_comh_buffer_appl_data.blinker_left_raw_data;
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
    *is_blinker_right_enabled = st_comh_buffer_appl_data.blinker_right_raw_data;
    return E_OK;
}

#if 0 //TODO TO BE REMOVED
/******************************************************************************/
/**
 * Gets the environment data received via CAN. Usually this function is called
 * by the error memory, when an error has been detected. The environment data is
 * meant to help in reproducing the error in the workshop.
 *
 * \return Nothing
 */
//Std_ReturnType COMH_GetEnvironmentData(struct APPL_environment_S* env_data)
//{
//}
#endif

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
{switch(dev_id_data->dev_id)
{
case DIDH_ODO_P4U_ID_1:
case DIDH_ULS_PDC_ID_1:
    CanSendDevId1(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_P4U_ID_2:
case DIDH_ULS_PDC_ID_2:
    CanSendDevId2(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_P4U_ID_3:
case DIDH_ULS_PDC_ID_3:
    CanSendDevId3(dev_id_data->dev_id_data);
    break;
case DIDH_FUN_P4U_ID_4:
case DIDH_ULS_PDC_ID_4:
    CanSendDevId4(dev_id_data->dev_id_data);
    break;
case DIDH_FUN_P4U_ID_5:
case DIDH_ULS_PDC_ID_5:
    CanSendDevId5(dev_id_data->dev_id_data);
    break;
case DIDH_FUN_P4U_ID_6:
case DIDH_ULS_PDC_ID_6:
    CanSendDevId6(dev_id_data->dev_id_data);
    break;
case DIDH_VCT_P4U_ID_7:
case DIDH_ULS_PDC_ID_7:
    CanSendDevId7(dev_id_data->dev_id_data);
    break;
case DIDH_VCT_P4U_ID_8:
case DIDH_ULS_PDC_ID_8:
    CanSendDevId8(dev_id_data->dev_id_data);
    break;
case DIDH_FUN_OPS_ID_1:
case DIDH_FUN_COL_MIT_ID_1:
case DIDH_ULS_BSD_ID_1:
    CanSendDevId9(dev_id_data->dev_id_data);
    break;
case DIDH_FUN_OPS_ID_2:
case DIDH_FUN_COL_MIT_ID_2:
case DIDH_ULS_BSD_ID_2:
    CanSendDevId10(dev_id_data->dev_id_data);
    break;
case DIDH_FUN_OPS_ID_3:
case DIDH_FUN_COL_MIT_ID_3:
case DIDH_ULS_BSD_ID_3:
    CanSendDevId11(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_P4U_ID_9:
case DIDH_FUN_COL_MIT_ID_4:
case DIDH_ULS_BSD_ID_4:
    CanSendDevId12(dev_id_data->dev_id_data);
    break;
case DIDH_FUN_P4U_ID_10:
case DIDH_FUN_COL_MIT_ID_5:
case DIDH_ULS_BSD_ID_5:
    CanSendDevId13(dev_id_data->dev_id_data);
    break;
case DIDH_VCT_P4U_ID_11:
case DIDH_FUN_COL_MIT_ID_6:
case DIDH_ULS_BSD_ID_6:
    CanSendDevId14(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_SYNCFRAME_ID:
    CanSendDevId15(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_TIMESTAMP_ID:
    CanSendDevId16(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_1:
    CanSendDevId17(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_2:
    CanSendDevId18(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_3:
    CanSendDevId19(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_4:
    CanSendDevId20(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_5:
    CanSendDevId21(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_6:
    CanSendDevId22(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_7:
    CanSendDevId23(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_8:
    CanSendDevId24(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_9:
    CanSendDevId25(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_10:
    CanSendDevId26(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_11:
    CanSendDevId27(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_12:
    CanSendDevId28(dev_id_data->dev_id_data);
    break;
case DIDH_FUN_MAP_ID:
    CanSendDevId31(dev_id_data->dev_id_data);
    break;
#ifdef DIDH_ENABLE_EXTENDED_RAW_DATA
case DIDH_ULS_RAW_DATA_ID_13:
    CanSendDevId32(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_14:
    CanSendDevId33(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_15:
    CanSendDevId34(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_16:
    CanSendDevId35(dev_id_data->dev_id_data);
    break;
#  ifdef DIDH_USE_HP_SENSORS
case DIDH_ULS_RAW_DATA_ID_17:
    CanSendDevId36(dev_id_data->dev_id_data);
    break;
case DIDH_ULS_RAW_DATA_ID_18:
    CanSendDevId37(dev_id_data->dev_id_data);
    break;
#  endif
#endif
default:
    break;
}}

void COMH_SendNotification(bool_T start_of_transmission)
{
    /* nothing to do on start or end of transmission of Dev-IDs */
}


bool_T COMH_GetPAM_req_buttonRelease(void)
{
    //TODO: USE NEW INTERFACE ONE HMIM IS UPDATED
  static u8 laststate=0;
  bool_T released = false;
  if(st_comh_buffer_appl_data.switch_options_button!=laststate  && st_comh_buffer_appl_data.switch_options_button == 0 )
  {
	  released = true;
  }
  laststate=st_comh_buffer_appl_data.switch_options_button;
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

/**
 * static void CanSendSectorCriticality(void)
 *
 * function to send the sector criticality message over CAN
 *
 */
#ifdef APPL_SECT_CRITICALITY_TO_CAN
static void CanSendSectorCriticality(void)
{
    u8 buff[2]={0};

    buff[0] |= SECT_IsSectorCriticalSector(CAPP_SPA_FRONT,0);       //Front Outer Left
    buff[0] |= (SECT_IsSectorCriticalSector(CAPP_SPA_FRONT,1)<<1);  //Front Middle Left
    buff[0] |= (SECT_IsSectorCriticalSector(CAPP_SPA_FRONT,2)<<2);  //Front Middle Right
    buff[0] |= (SECT_IsSectorCriticalSector(CAPP_SPA_FRONT,3)<<3);  //Front Outer Right

    buff[0] |= (SECT_IsSectorCriticalSector(CAPP_SPA_REAR,0)<<4);  //Rear Outer Left
    buff[0] |= (SECT_IsSectorCriticalSector(CAPP_SPA_REAR,1)<<5);  //Rear Middle Left
    buff[0] |= (SECT_IsSectorCriticalSector(CAPP_SPA_REAR,2)<<6);  //Rear Middle Right
    buff[0] |= (SECT_IsSectorCriticalSector(CAPP_SPA_REAR,3)<<7);  //Rear Outer Right

    buff[1] |= SECT_IsSectorCriticalSector(CAPP_SPA_LEFT,0);       //Left Side Front
    buff[1] |= (SECT_IsSectorCriticalSector(CAPP_SPA_LEFT,1)<<1);  //Left Side Middle Front
    buff[1] |= (SECT_IsSectorCriticalSector(CAPP_SPA_LEFT,2)<<2);  //Left Side Middle Rear
    buff[1] |= (SECT_IsSectorCriticalSector(CAPP_SPA_LEFT,3)<<3);  //Left Side Rear

    buff[1] |= (SECT_IsSectorCriticalSector(CAPP_SPA_RIGHT,0)<<4);  //Right Side Front
    buff[1] |= (SECT_IsSectorCriticalSector(CAPP_SPA_RIGHT,1)<<5);  //Right Side Middle Front
    buff[1] |= (SECT_IsSectorCriticalSector(CAPP_SPA_RIGHT,2)<<6);  //Right Side Middle Rear
    buff[1] |= (SECT_IsSectorCriticalSector(CAPP_SPA_RIGHT,3)<<7);  //Right Side Rear

    P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_SECT_CRITICALITY_TO_CAN_ID, buff, sizeof(buff));
}
#endif


static u8 ComputeSetPkm(void)
{
    static u8 old_pkm_state = 1;
    u8 set_pkm = 1;

    switch (st_comh_buffer_appl_data.status_eps_moment_raw_data)
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

    if (P4U_IsParkingActive())
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
static u16 GetHaptTorq12bits(u8 pkm_state)
{
    si16 add_steer_torque_100thnm;
    u16  add_steer_torque_100thnm_offset;
    u16  add_steer_torque_PMW_12bits;

    // Not yet in steering mode, set added torque to zero
    if ((3 != pkm_state) ||
        (   (3 == pkm_state) &&
            (2 == st_comh_buffer_appl_data.status_eps_moment_raw_data)
        )
    )
    {
        add_steer_torque_100thnm = 0;
    }
    else
    {
        /* get torque from DAPM in 100thNm */
        add_steer_torque_100thnm = P2DAL_GetFiltAddTorque100thnm();

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
  return st_comh_buffer_appl_data.status_eps_timestamp;
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
  u16 ret_value = st_comh_buffer_appl_data.wheel_speed_raw_data /100u ;

  return ret_value;
}

Std_ReturnType COMH_GetSpeed_th100kph(u16* vehicle_speed)
{
    * vehicle_speed = st_comh_buffer_appl_data.wheel_speed_raw_data ;

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
   return st_comh_buffer_appl_data.gear_change_ready;
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
  st_comh_buffer_appl_data.steering_angle = (si16)temp_angle / 3;
  st_comh_buffer_appl_data.steering_angle *= 2;
  st_comh_buffer_appl_data.steering_angle *= temp_sign;

  return st_comh_buffer_appl_data.steering_angle;
}

Std_ReturnType COMH_GetGearLeverPosition_RevGr_Engg(enum CAPP_gear_lever_position_E* gear_lever_pos)
{
    if (st_comh_buffer_appl_data.reverse_gear_raw_data)
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

    *wheel_speed_fl = st_comh_buffer_appl_data.wheel_speeds.wheel_speed_fl;
    return E_OK;
}

Std_ReturnType COMH_GetWheelSpeedrl_th100mps(si16* wheel_speed_rl)
{

   *wheel_speed_rl = st_comh_buffer_appl_data.wheel_speeds.wheel_speed_rl;
   return E_OK;
}

Std_ReturnType COMH_GetWheelSpeedfr_th100mps(si16* wheel_speed_fr)
{

    *wheel_speed_fr = st_comh_buffer_appl_data.wheel_speeds.wheel_speed_fr;
    return E_OK;
}

Std_ReturnType COMH_GetWheelSpeedrr_th100mps(si16* wheel_speed_rr)
{

   *wheel_speed_rr = st_comh_buffer_appl_data.wheel_speeds.wheel_speed_rr;
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
    *data = (st_comh_buffer_appl_data.brake_pressure_raw_data );
}

enu_break_pedal
COMH_GetBreakPedal(void)
{
    static enu_break_pedal BrakePedalState = false;
    if(st_comh_buffer_appl_data.driver_brake_pressure_raw_data >= BRK_PEDAL_PRSD_THRSHLD)
    {
    	BrakePedalState = PRESSED;
    }
    else if(st_comh_buffer_appl_data.driver_brake_pressure_raw_data <= BRK_PEDAL_N_PRSD_THRSHLD)
    {
    	BrakePedalState = RELEASED;
    }
    else
    {
    	/* do not update BrakePedalPressed */
    }
	return BrakePedalState;
}

si16 COMH_GetBrakePedalTorque(void)
{
    return st_comh_buffer_appl_data.driver_brake_pressure_raw_data;
}

Std_ReturnType COMH_GetGPSdata(si32* longitude , si32* latitude)

{
	/* Adding offset to the value received to compensate for the sign that is not handled by the can converter*/
	*latitude  = st_comh_buffer_appl_data.gps_latitude  + LATITUDE_OFFSET;
	*longitude = st_comh_buffer_appl_data.gps_longitude + LONGITUDE_OFFSET;

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
    return ((st_comh_buffer_appl_data.dead_man_button == 1 ) ? COMH_BUTTON_PRESSED : COMH_BUTTON_RELEASED);
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
    upa_button.button_signal = st_comh_buffer_appl_data.upa_button;
    EvaluateButtonState(&upa_button);
}
static void EvaluatePark4uButtonState(void)
{
    static struct button_properties_S p4ubutton = {COMH_BUTTON_RELEASED,&park4u_button_state,0,FALSE,0};
    p4ubutton.button_signal = st_comh_buffer_appl_data.park4u_button;
    EvaluateButtonState(&p4ubutton);
}
static void EvaluateConfirmationButtonState(void)
{
    static struct button_properties_S confirmation_button = {COMH_BUTTON_RELEASED,&confirmation_button_state,0,FALSE,0};
    confirmation_button.button_signal = st_comh_buffer_appl_data.confirmation_button;
    EvaluateButtonState(&confirmation_button);
}
static void EvaluateGpsButtonState(void)
{
    static struct button_properties_S GPS_button = {COMH_BUTTON_RELEASED,&gps_button_state,0,FALSE,0};
    GPS_button.button_signal = st_comh_buffer_appl_data.GPS_button;
    EvaluateButtonState(&GPS_button);
}

static void EvaluateP4uHomeTrainingButtonState(void)
{
    static struct button_properties_S p4uhome_training_button = {COMH_BUTTON_RELEASED,&p4uHomeTraining_button_state,0,FALSE,0};
    p4uhome_training_button.button_signal = st_comh_buffer_appl_data.p4uhome_training_button;
    EvaluateButtonState(&p4uhome_training_button);
}

static void EvaluateAbortButtonState(void)
{
    static struct button_properties_S abort_button = {COMH_BUTTON_RELEASED,&abort_button_state,0,FALSE,0};
    abort_button.button_signal = st_comh_buffer_appl_data.abort_button;
    EvaluateButtonState(&abort_button);
}

static void EvaluateViewButtonState(void)
{
    static struct button_properties_S view_button = {COMH_BUTTON_RELEASED,&view_button_state,0,TRUE,0};
    view_button.button_signal = st_comh_buffer_appl_data.view_button;
    EvaluateButtonState(&view_button);
}
static void EvaluateBdaReplayButtonState(void)
{
    static struct button_properties_S bda_replay_button = {COMH_BUTTON_RELEASED,&bdaReplay_button_state,0,FALSE,0};
    bda_replay_button.button_signal = st_comh_buffer_appl_data.bda_replay_button;
    EvaluateButtonState(&bda_replay_button);
}

static void EvaluateSwitchButtonState(void)
{
    static struct button_properties_S switch_button = {COMH_BUTTON_RELEASED,&switch_button_state,0,FALSE,0};
    switch_button.button_signal = st_comh_buffer_appl_data.switch_options_button;
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

    if((st_comh_buffer_appl_data.switch_options_button == TRUE))
    {
    	switch_back_button_click_count++;
        if(switch_back_button_click_count>COMH_BACK_BUTTON_TIMER_VALUE)
        {
        	funcbar_back_button_state = COMH_BUTTON_PRESSED;
        }
    }

    if(st_comh_buffer_appl_data.switch_options_button == FALSE)
    {
    	funcbar_back_button_state = COMH_BUTTON_RELEASED;
    	switch_back_button_click_count = 0;
    }


    pre_state = st_comh_buffer_appl_data.switch_options_button;
}
static void SwitchOptionsButtonCounter(void)
{
    static bool_T pre_state = FALSE;
    if((st_comh_buffer_appl_data.switch_options_button == TRUE) && (pre_state == COMH_BUTTON_RELEASED ))
    {
        switch_options_button_click_count++;
    }
    pre_state = st_comh_buffer_appl_data.switch_options_button;
}

/**
 * u8 COMH_GetBLEMessage(void)
 *
 * \return 0 if there is no MSR request, 1 if there is a MSR request
 */
void COMH_GetBLEMessage(ble_msg_T* p)
{
    *p = st_comh_buffer_appl_data.ble_msg;
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

/**
 * void COMH_GetGPSDate(u8* day, u8* month, u16* year)
 *
 * fill GPS date data for PEIKER ECU, day - month - year
 * \return
 */
void COMH_GetGPSDate(u8* day, u8* month, u16* year)
{
    *day = st_comh_buffer_appl_data.gps_date_day;
    *month = st_comh_buffer_appl_data.gps_date_mon;
    *year = st_comh_buffer_appl_data.gps_date_year;
}

/**
 * void COMH_GetGPSTime(u8* seconds, u8* minutes, u16* hours)
 *
 * fill GPS time data for PEIKER ECU, seconds - minutes - hours
 * \return
 */
void COMH_GetGPSTime(u8* seconds, u8* minutes, u8* hours)
{
    *seconds = st_comh_buffer_appl_data.gps_time_sec;
    *minutes = st_comh_buffer_appl_data.gps_time_min;
    *hours = st_comh_buffer_appl_data.gps_time_hour;
}

/**
 * void COMH_GetGPSProperties(u16* accuracy_horizontal, u16* error_latitude, u16* error_longitude, u8* quantity_satellite)
 *
 * fill GPS Properties data for PEIKER ECU, accuracy_horizontal - error_latitude - error_longitude - quantity_satellite
 * \return
 */
void COMH_GetGPSProperties(u16* accuracy_horizontal, u16* error_latitude, u16* error_longitude, u8* quantity_satellite)
{
    *accuracy_horizontal = st_comh_buffer_appl_data.gps_accuracy_horizontal;
    *error_latitude = st_comh_buffer_appl_data.gps_error_latitude_position;
    *error_longitude = st_comh_buffer_appl_data.gps_error_longitude_position;
    *quantity_satellite = st_comh_buffer_appl_data.gps_quantity_satellite_usage;
}

/**
 * void COMH_GetGPSSpeedHorizontal(u16* speed_horizontal)
 *
 * fill GPS Properties data for PEIKER ECU, accuracy_horizontal - error_latitude - error_longitude - quantity_satellite
 * \return
 */
void COMH_GetGPSHorizontalSpeed(u16* speed_horizontal)
{
    *speed_horizontal = st_comh_buffer_appl_data.gps_speed_horizontal;
}

/**
 * void COMH_GetGPSPosition(u32* longitude , u32* latitude)
 *
 * fill GPS Properties data for PEIKER ECU, accuracy_horizontal - error_latitude - error_longitude - quantity_satellite
 * \return
 */
void COMH_GetGPSPosition(u32* longitude , u32* latitude)
{
    *latitude  = (u32) st_comh_buffer_appl_data.gps_pos_latitude;
    *longitude = (u32) st_comh_buffer_appl_data.gps_pos_longitude;
}

/* Keep ATM alive and keep sending messages - PEIKER ECU */
static void Send_ATM_NM(void)
{
    u8 msg_buf_length = 8;
    u8 msg_buf[msg_buf_length];
    static u8 cycle_count = 0;
    static u8 delay = 0;

    if(delay >= 50u){
        if(0u == (cycle_count % 5u))//Cycle time 100 ms
        {
            P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_CAN_ID_ATM_NM_12F, msg_buf, msg_buf_length);
        }

        if(32 == cycle_count )//Cycle time 640 ms
        {
            P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_CAN_ID_ATM_NM_510, msg_buf, msg_buf_length);
            cycle_count = 0;
        }

        cycle_count++;
        delay = 50;
    }

    else{
        delay++;
    }
}


static void Send_Debug_Msg(void)
{
    const char debug_msg_length = 8;
    u8 debug_msg[debug_msg_length];
    memset(debug_msg,0,debug_msg_length);
    debug_msg[0] |= ((u8)BRKH_CusIsEmergencyBrakeActive()) << 2;
    debug_msg[0] |= ((u8)P2DAL_IsLotCtrlRequired()) << 3;
    debug_msg[0] |= ((u8)P2DAL_IsLatCtrlRequired()) << 4;
    debug_msg[3] |= (u8)P4U_Get_p4u_state();
    debug_msg[4] |= (u8)BRKH_GetMainState();
    debug_msg[4] |= (((u8)BRKH_GetActiveSubState()) << 4);
    debug_msg[6] |= (u8)STMH_GetMainState();
    debug_msg[6] |= (((u8)STMH_GetActiveSubState()) << 4 );
#ifdef XP4UHOME_FEATURE
    debug_msg[1] = HZTR_GetReplayState();
    debug_msg[2] = HZTR_GetTrainingState();
#endif
#ifdef XBDA_ENABLE
    debug_msg[7] |= BDA_GetReplayState();
    debug_msg[7] |= (BDA_GetTrainingState() << 4 );
#endif
    P2GPA_CanSend (P2GPA_CAN_prio_high, XP2GPA_CAN_ID_DEBUG_DATA, debug_msg, debug_msg_length);
}



#ifdef DAIMLER_BR_213_PLATFORM
/******************************************************************************/
/*              Function definitions for Daimler BR213 prototype              */
/******************************************************************************/

/**
 * esp_brake_state_E COMH_GetEspBrakeState(void)
 *
 * \return st_comh_buffer_can_data.esp_brake_state_park
 */
 esp_brake_state_E COMH_GetEspBrakeState(void)
 {
   return st_comh_buffer_can_data.esp_brake_state_park;
 }

/**
 * esp_intervention_E   COMH_GetESPIntervention
 *
 * \return ESP_INTERVENTION_ACTIVE if
 *         st_comh_buffer_can_data.ESP_Oprtn_Stat_AS_V2 is not 0 nor 8
 */
 esp_intervention_E   COMH_GetESPIntervention(void)
 {
   esp_intervention_E temp;
   if (  st_comh_buffer_can_data.ESP_Oprtn_Stat_AS_V2 != 0u
      && st_comh_buffer_can_data.ESP_Oprtn_Stat_AS_V2 != 8u)
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
 *         st_comh_buffer_can_data.engine_state is not 1
 */
 engine_running_state_E  COMH_GetEngineState(void)
 {
   engine_running_state_E temp;
   if (st_comh_buffer_can_data.engine_state == 1u)
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
 * \return st_comh_buffer_can_data.esp_ldc_afterrun_ctrl
 */
 ldc_afterrun_ctrl_E     COMH_GetEspAfterrunControl(void)
 {
   return st_comh_buffer_can_data.esp_ldc_afterrun_ctrl;
 }


 /**
 * Std_ReturnType     COMH_GetEpbStatus(void)
 *
 * \return 1 if epb is engaged, 0 otherwise
 */
 Std_ReturnType COMH_GetEpbStatus(epb_status_T* epb_status)
 {
     Std_ReturnType ret = E_OK;

     if(st_comh_buffer_can_data.epb_engaged)
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
   return st_comh_buffer_can_data.current_gear;
 }


 /**
 * bool_T                  COMH_GetParkEnblStatEsp(void)
 *
 * \return           returns 1 if park is enabled
 */
 u8                    COMH_GetParkEnblStatEsp(void)
 {

   return st_comh_buffer_can_data.park_enbl_stat_esp;

 }

 /**
 * park_flt_stat_esp_E     COMH_GetParkFltStatEsp(void)
 *
 * \return           st_comh_buffer_can_data.park_flt_stat_esp;
 */
 park_flt_stat_esp_E     COMH_GetParkFltStatEsp(void)
 {
   return st_comh_buffer_can_data.park_flt_stat_esp;
 }

 /**
 * esp_system_state_E      COMH_GetEspSystemState(void)
 *
 * \return           st_comh_buffer_can_data.esp_system_state;
 */
 esp_system_state_E      COMH_GetEspSystemState(void)
 {
   return st_comh_buffer_can_data.esp_system_state;
 }

 #endif
/******************************************************************************
 *
 * END  Source:         COMH.c
 *
 ******************************************************************************/


