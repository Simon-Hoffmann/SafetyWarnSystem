/**
*	@file 	rfm.h
*	@author Simon Hoffmann
*	@brief	RFM69HWC Interface
*/

#ifndef __RFM_SENSOR_H_
#define __RFM_SENSOR_H_

#include "packet.h"

/*-------------------------- I N C L U D E S ----------------------------*/

/*---------------------------- M A C R O S ------------------------------*/

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
*	Initializes the RFM69HWC module on the adafruit
*
*	@param 	none
*	@return none
*/
void rfm_init(void);

/**
*	Sends data over rfm module and handles any replies
*
*	@param 	packet - the data to send
*	@return bool - if a ACK was received
*/
bool rfm_send(swsPacket* packet);

#endif

