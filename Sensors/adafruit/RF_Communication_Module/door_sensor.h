/**
*	@file 	door_sensor.h
*	@author Simon Hoffmann
*	@brief	Door sensor Interface
*/

#ifndef __DOOR_SENSOR_H_
#define __DOOR_SENSOR_H_

#include "sensor.h"

#ifdef DOOR_SENSOR

/*-------------------------- I N C L U D E S ----------------------------*/

/*---------------------------- M A C R O S ------------------------------*/

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
* Initializes door sensor
*
*	@param 	none
*	@return none
*/
void door_sensor_init(void);

/**
* Checks if door open
*
*	@param 	none
*	@return bool - if data had to be sent
*/
bool door_sensor_check(void);

/**
* Sets the packet content and sends data over rfm
*
*	@param 	none
*	@return none
*/
void door_sensor_send_data(void);

#endif
#endif