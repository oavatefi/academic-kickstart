/*
 * comh_ble.h
 *
 *  Created on: May 23, 2016
 *      Author: mmetwall
 */

#ifndef COMH_BLE_H_
#define COMH_BLE_H_
enum ble_active_system
{
	BLE_UNDEFINED,
	BLE_HZTR_ACTIVE,
	BLE_P4U_ACTIVE
};

void COMH_BLE_Cylic40ms(void);
void COMH_BLE_Cylic40msShift5ms(void);
void COMH_BLE_Activate(void);
void COMH_BLE_Deactivate(void);
ble_state_T COMH_BLE_GetState(void);
void COMH_BLE_SetManeuverTwoMsg(ble_maneuver2_T* p, enum ble_active_system active_system);
void COMH_BLE_SetManeuverOneMsg(ble_maneuver1_T* p, enum ble_active_system active_system);
void COMH_BLE_Reset(void);
void COMH_BLE_Set_HZTR_Active(void);
void COMH_BLE_Set_P4u_Active(void);
void COMH_BLE_Reset_HZTR_Active(void);
void COMH_BLE_Reset_P4U_Active(void);
#endif /* COMH_BLE_H_ */
