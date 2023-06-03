/**
*	@file 	water_sensor.h
*	@author Simon Hoffmann
*	@brief	Water sensor Interface
*/

#ifndef __WATER_SENSOR_H_
#define __WATER_SENSOR_H_

#include "sensor.h"

#ifdef WATER_SENSOR

/*-------------------------- I N C L U D E S ----------------------------*/

/*---------------------------- M A C R O S ------------------------------*/

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
* Initializes water sensor
*
*	@param 	none
*	@return none
*/
void water_sensor_init(void);

/**
* Checks if water sensor is submerged, if it is, send data
*
*	@param 	none
*	@return bool - if data had to be sent
*/
bool water_sensor_check(void);

/**
* Sets the packet content and sends data over rfm
*
*	@param 	none
*	@return none
*/
void water_sensor_send_data(void);

#endif
#endif