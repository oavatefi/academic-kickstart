/*
 * comh_ble.h
 *
 *  Created on: May 23, 2016
 *      Author: mmetwall
 */

#ifndef COMH_BLE_H_
#define COMH_BLE_H_


void COMH_BLE_Cylic40ms(void);
void COMH_BLE_Cylic40msShift5ms(void);
void COMH_BLE_Activate(void);
void COMH_BLE_Deactivate(void);
ble_state_T COMH_BLE_GetState(void);
void COMH_BLE_SetManeuverTwoMsg(ble_maneuver2_T* p);
void COMH_BLE_SetManeuverOneMsg(ble_maneuver1_T* p);
void COMH_BLE_Reset(void);
#endif /* COMH_BLE_H_ */
