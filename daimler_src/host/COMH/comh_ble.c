/*
 * comh_ble.c
 *
 *  Created on: May 23, 2016
 *      Author: mmetwall
 */

#include"tmp_pdf.h"
#include"platform_types.h"
#include"comh_can.h"
#include"comh.h"
#include"comh_ble.h"
#include "PLATFORM_SharedVar.h"
/******************************************************************************/
/*                  Declaration of local function prototypes                  */
/******************************************************************************/
static void bleStateMachine();
static void ble_no_request_handler(void);
static void ble_initialization_handler(void);
static void ble_waiting_response_handler(void);
static void ble_connected_handler(void);
static void ble_error_handler(void);
static void SendConnectMsg(ble_connect_T* msg);
static void SendManeuverOneMsg(ble_maneuver1_T* msg);
static void SendManeuverTwoMsg(ble_maneuver2_T* msg);
static void ble_acknowledgement_check(void);
/******************************************************************************/
/*          Definition of local types (typedef, enum, struct, union)          */
/******************************************************************************/

/******************************************************************************/
/*                       Definition of local variables                        */
/******************************************************************************/

static ble_maneuver2_T ble_maneuver2;
static ble_maneuver1_T ble_maneuver1;
static ble_state_T ble_state = CONNECTED;
static bool_T out_side_active = false;
static uint8 Park_MsgCounter = 0;
/******************************************************************************/
/*                      Definition of exported functions                      */
/******************************************************************************/
void COMH_BLE_Cylic40ms(void)
{
    bleStateMachine();

}

void COMH_BLE_Cylic40msShift5ms(void)
{
	uint8 frontspeaker;
	uint8 rearspeaker;
	PLATFORM_ReadFrontSpeaker(&frontspeaker);
	PLATFORM_ReadRearSpeaker(&rearspeaker);
	ble_maneuver2.Park_FrontSpeakerValue=frontspeaker;
	ble_maneuver2.Park_RearSpeakerValue=rearspeaker;
    if ( (ble_state == INITIALIZATION) || (ble_state == CONNECTED))
    {
        SendManeuverTwoMsg(&ble_maneuver2);
    }
}

void COMH_BLE_Activate(void)
{
    out_side_active = true;
}

void COMH_BLE_Deactivate(void)
{
    out_side_active = false;
}

ble_state_T COMH_BLE_GetState(void)
{

    return ble_state;
}

void COMH_BLE_SetManeuverTwoMsg(ble_maneuver2_T* p)
{
    ble_maneuver2 = *p;
}

void COMH_BLE_SetManeuverOneMsg(ble_maneuver1_T* p)
{
    ble_maneuver1 = *p;
}

void COMH_BLE_Reset(void)
{
    ble_state = NO_REQUEST;
    COMH_Clear_BLE_MSGS();
}
/******************************************************************************/
/*                      Definition of local functions                      */
/******************************************************************************/
static void bleStateMachine()
{
    switch (ble_state)
    {
    case NO_REQUEST:
        ble_no_request_handler();
        break;

    case INITIALIZATION:
         ble_initialization_handler();
        ble_waiting_response_handler();
        break;

    case CONNECTED:
        ble_connected_handler();
        ble_acknowledgement_check();
        break;

    case BLE_ERROR:
        ble_error_handler();
        break;

    }
}

static void ble_no_request_handler(void)
{
    if (out_side_active)
    {
        ble_state = INITIALIZATION;
		ble_maneuver1.Park_VehicleReadyForManeuver = FALSE;
    }
}

static void ble_initialization_handler(void)
{
/*    ble_connect_T temp_msg;
    temp_msg.BLE_ID_To = 0x01;
    temp_msg.Park_MsgType = 0x01;
    temp_msg.Park_Trx_Type = 0x02;
    temp_msg.Park_Trx_Version = 0x00;
    temp_msg.Park_TrxSw_Vmajor = 0x00;
    temp_msg.Park_TrxSw_Vminor = 0x00;
    SendConnectMsg(&temp_msg);*/

    SendManeuverOneMsg(&ble_maneuver1);
      if (!out_side_active)
      {
          ble_state = NO_REQUEST;
      }
}

static void ble_waiting_response_handler(void)
{
    static uint16 save_trials;
    ble_msg_T ble_tmp;
    COMH_GetBLEMessage(&ble_tmp);
    bool_T event =
        (ble_tmp.park_MsgType >= 0) && (ble_tmp.SP_SmartphoneConnected == 1 )/*&& ble_tmp.SP_SmartphoneConnected*/;
    if (event)
    {
        ble_state = CONNECTED;
    }



    if (!out_side_active)
    {
        ble_state = NO_REQUEST;
    }

}

static void ble_connected_handler(void)
{
	ble_maneuver1.Park_VehicleReadyForManeuver = TRUE;

    SendManeuverOneMsg(&ble_maneuver1);
    if (!out_side_active)
    {
        ble_state = NO_REQUEST;
    }
}

static void ble_error_handler(void)
{

}
static void SendConnectMsg(ble_connect_T* msg)
{
    Park_MsgCounter++;
    uint8 buff[8];
    memset(buff, 0, 8);
    buff[0] |= 1;
    buff[0] |= (msg->Park_MsgType) << 4;
    buff[1] |= Park_MsgCounter;
    buff[2] |= (msg->Park_Trx_Type);
    buff[3] |= (msg->Park_VehInfo_Line);
    buff[3] |= (msg->Park_VehInfo_Body) << 4;
    buff[4] |= (msg->Park_TrxSw_Vmajor);
    buff[4] |= ((msg->Park_TrxSw_Vminor) << 7) & 0x7f;
    buff[5] |= (msg->Park_TrxSw_Vmajor) >> 1;
    buff[5] |= (msg->Park_VehInfo_ColourR) << 4;
    buff[6] |= (msg->Park_VehInfo_ColourG);
    buff[6] |= (msg->Park_VehInfo_ColourB) << 4;
    P2GPA_CanSend(P2GPA_CAN_prio_high, 0x3A6, buff, 8);
}

static void SendManeuverOneMsg(ble_maneuver1_T* msg)
{
    Park_MsgCounter++;
    uint8 buff[8];
    memset(buff, 0, 8);
    buff[0] |= 1;
    buff[0] |= (1) << 4;
    buff[1] |= Park_MsgCounter;
    buff[2] |= (msg->ParkRemainingDistance);
    buff[3] |= (msg->Park_CurrentState);
    buff[3] |= (msg->ParkVehicleMovingDirection) << 6;
    buff[4] |= (msg->ParkGearEngaged);
    buff[4] |= (msg->Park_AbortReason) << 4;
    buff[5] |= (msg->Park_ObstaclePosition);
    buff[5] |= (msg->Park_VehicleReadyForManeuver) << 2;
    P2GPA_CanSend(P2GPA_CAN_prio_high, 0x3A6, buff, 8);

}

static void SendManeuverTwoMsg(ble_maneuver2_T* msg)
{
   // Park_MsgCounter++;
    uint8 buff[8];
    memset(buff, 0, 8);
    buff[0] |= 1;
    buff[0] |= (2) << 4;
    buff[1] |= Park_MsgCounter;
    buff[2] |= (msg->Park_Sector0);
    buff[2] |= (msg->Park_Sector1) << 2;
    buff[2] |= (msg->Park_Sector2) << 4;
    buff[2] |= (msg->Park_Sector3) << 6;
    buff[3] |= (msg->Park_Sector4);
    buff[3] |= (msg->Park_Sector5) << 2;
    buff[3] |= (msg->Park_Sector6) << 4;
    buff[3] |= (msg->Park_Sector7) << 6;
    buff[4] |= (msg->Park_Sector8);
    buff[4] |= (msg->Park_Sector9) << 2;
    buff[4] |= (msg->Park_Sector10) << 4;
    buff[4] |= (msg->Park_Sector11) << 6;
    buff[5] |= (msg->Park_Sector12);
    buff[5] |= (msg->Park_Sector13) << 2;
    buff[5] |= (msg->Park_Sector14) << 4;
    buff[5] |= (msg->Park_Sector15) << 6;
    buff[6] |= (msg->Park_FrontSpeakerValue);
    buff[7] |= (msg->Park_RearSpeakerValue);
    P2GPA_CanSend(P2GPA_CAN_prio_high, 0x3A6, buff, 8);

}
static void ble_acknowledgement_check(void)
{
    static u8 counter = 0;
    static u8 last_msg_counter = 0;
    ble_msg_T ble_tmp;
    COMH_GetBLEMessage(&ble_tmp);
    if (counter > 10)
    {

        if (counter > 10)
        {
            if (ble_tmp.BLE_MsgCounter == last_msg_counter)
            {

               // ble_state = BLE_ERROR;
            }
            counter = 0;
        }
        last_msg_counter = ble_tmp.BLE_MsgCounter;
    }
    else
    {
        counter++;

    }

}
