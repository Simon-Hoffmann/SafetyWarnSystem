/**
*	@file 	water_sensor.cpp
*	@author Simon Hoffmann
*	@brief	Water sensor implementation
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include "water_sensor.h"
#include "led.h"

#ifdef WATER_SENSOR

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

#define WATER_SENSOR_PIN 16
//
//PinStatus water_sensor_val;
//static uint32_t alarmOffTime_s = 0;
//
///* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */
//
//bool water_sensor_read_warning(void);
//void water_sensor_send_data(void);
//bool water_sensor_read_warning(void);
//
///* ----------------------- F U N C T I O N S  -------------------------- */
//
///**
//* Initializes water sensor
//*
//*	@param 	none
//*	@return none
//*/
//void water_sensor_init(void){
//  pinMode(WATER_SENSOR_PIN, INPUT);
//}
//
///**
//* Checks if water sensor is submerged, if it is, send data
//*
//*	@param 	none
//*	@return none
//*/
//void water_sensor_check(void){
//  if(alarmOffTime_s){
//    alarmOffTime_s--;
//  }else{
//    if(water_sensor_read_warning()){
//        water_sensor_send_data();
//    }
//  }
//}
//
//void water_sensor_send_data(void){
//  //send data over rfm (push into fifo)
//}
//
///**
//* Gets the value of the water sensors and checks if detects water
//*
//*	@param 	none
//*	@return 1 if values are dangerous, otherwise 0 if all good
//*/
//bool water_sensor_read_warning(void){
//  water_sensor_val = digitalRead(WATER_SENSOR_PIN);
//
//  if (water_sensor_val == LOW) {
//    return true;
//  } else {
//    return false;
//  }
//}

#endif
