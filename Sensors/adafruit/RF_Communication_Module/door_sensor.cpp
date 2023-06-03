/**
*	@file 	water_sensor.cpp
*	@author Simon Hoffmann
*	@brief	Water sensor implementation
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include "door_sensor.h"
#include "Arduino.h"
#include "led.h"
#include "debug.h"
#include "packet.h"
#include "rfm.h"
#include "sensor.h"

#ifdef DOOR_SENSOR

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

#define DOOR_SENSOR_PIN 12
PinStatus door_sensor_val;
static struct swsPacket packet;
													
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

void door_sensor_send_data(void);
bool door_sensor_read_warning(void);

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
* Initializes door sensor
*
*	@param 	none
*	@return none
*/
void door_sensor_init(void){
  pinMode(DOOR_SENSOR_PIN, INPUT);
}

/**
* Checks if door is open
*
*	@param 	none
*	@return none
*/
bool door_sensor_check(void){
    if(door_sensor_read_warning()){
        door_sensor_send_data();
        return true;
    }
    return false;
}

/**
* Sets the packet content and sends data over rfm
*
*	@param 	none
*	@return none
*/
void door_sensor_send_data(void){
  packet.idSensor = sensorID;
  packet.packetType = SEND;
  packet.idBase = baseID;
  packet.dataLength = 2;
  packet.data[0] = 3;
  packet.data[1] = door_sensor_val;

  if(!rfm_send(&packet)){
    sensor_reset();
  }
}

/**
* Gets the value of the water sensors and checks if detects water
*
*	@param 	none
*	@return 1 if values are dangerous, otherwise 0 if all good
*/
bool door_sensor_read_warning(void){
  door_sensor_val = digitalRead(DOOR_SENSOR_PIN);
  return door_sensor_val == LOW;
}

#endif
