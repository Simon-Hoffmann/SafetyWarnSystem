/**
*	@file 	water_sensor.cpp
*	@author Simon Hoffmann
*	@brief	Water sensor implementation
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include "water_sensor.h"
#include "Arduino.h"
#include "led.h"
#include "debug.h"
#include "packet.h"
#include "rfm.h"
#include "sensor.h"

#ifdef WATER_SENSOR

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

#define WATER_SENSOR_PIN 12

uint8_t water_sensor;
static struct swsPacket packet;
													
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

bool water_sensor_read_warning(void);
void water_sensor_send_data(void);

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
* Initializes water sensor
*
*	@param 	none
*	@return none
*/
void water_sensor_init(void){
  pinMode(WATER_SENSOR_PIN, INPUT);
}

/**
* Checks if water sensor is submerged, if it is, send data
*
*	@param 	none
*	@return none
*/
bool water_sensor_check(void){
  if(water_sensor_read_warning()){
        #ifdef DEBUG
        Serial.println("water_sensor_read_warning");
      #endif
      water_sensor_send_data();
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
void water_sensor_send_data(void){
  packet.idSensor = sensorID;
  packet.packetType = SEND;
  packet.idBase = baseID;
  packet.dataLength = 2;
  packet.data[0] = 1;
  packet.data[1] = water_sensor;

  #ifndef HARDWARE_DEBUG
  if(!rfm_send(&packet)){
    sensor_reset();
  }
  #endif
}

/**
* Gets the value of the water sensors and checks if detects water
*
*	@param 	none
*	@return 1 if values are dangerous, otherwise 0 if all good
*/
bool water_sensor_read_warning(void){
  PinStatus water_sensor_val = digitalRead(WATER_SENSOR_PIN);
  water_sensor_val == LOW ? water_sensor = 1 : water_sensor = 0;
  return water_sensor;
}

#endif
