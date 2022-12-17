/**
*	@file 	gas_sensor.h
*	@author Simon Hoffmann
*	@brief	Gas sensor Interface
*/

#ifndef __GAS_SENSOR_H_
#define __GAS_SENSOR_H_

#include "sensor.h"

#ifdef GAS_SENSOR

/*-------------------------- I N C L U D E S ----------------------------*/


/*---------------------------- M A C R O S ------------------------------*/

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

/* ----------------------- F U N C T I O N S  -------------------------- */

void gas_sensor_init(void);
void gas_sensor_check(void);
void gas_sensor_send_data(void);

#endif
#endif