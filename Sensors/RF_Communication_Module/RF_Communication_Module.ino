/**
*	@file 	RF_Communication_Module.ino
*	@author Simon Hoffmann
*	@brief	Main Program start and loop - Select the type os sensor in sensor.h
* @date   10.11.2022
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include <FreeRTOS.h>
#include "sensor.h"


/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

#define VERSION  "1.0.0"
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
*	Startup for initialisation
*
*	@param 	none
*	@return none
*/
void setup(void) {
  sensor_init();
}

/**
*	Main program loop not used because of FreeRTOS
*
*	@param 	none
*	@return none
*/
void loop(void) {}








