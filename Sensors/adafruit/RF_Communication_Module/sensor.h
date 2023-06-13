/**
*	@file 	sensor.h
*	@author Simon Hoffmann
*	@brief	Sensor Interface
*/

#ifndef __SENSOR_H_
#define __SENSOR_H_

/*Sensor types: GAS_SENSOR  WATER_SENSOR  DOOR_SENSOR*/

#define WATER_SENSOR

/*-------------------------- I N C L U D E S ----------------------------*/

#include <stdint.h>

/*---------------------------- M A C R O S ------------------------------*/

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

extern volatile uint8_t sensorID;
extern volatile uint8_t baseID;

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
*	Initialises all hardware for the sensors
*
* @details Depending on the defined sensor, the relevant hardware is initialised
*	@param 	none
*	@return none
*/
void sensor_init(void);


/**
*	Main sensor loop
*	
*	@param 	none
*	@return none
*/
void sensor_task(void);

/**
*	Resets the variables of sensor and base station ID in flash and restarts the microcontroller
*
*	@param 	none
*	@return none
*/
void sensor_reset(void);

#endif