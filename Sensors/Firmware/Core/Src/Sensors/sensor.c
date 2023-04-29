/**
*	@file 	sensor.cpp
*	@author Simon Hoffmann
*	@brief	sensor implementation
*
* Contains all standard functionalities between all sensors
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include <stdint.h>
#include "sensor.h"
#include "led.h"
#include "gas_sensor.h"
#include "water_sensor.h"

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

const uint8_t BUTTON_PRESS_PIN = 0;

/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

void sensor_led_startup_indication(void);
void sensor_reset(void);
void sensor_button_pressed(void);

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
*	Initialises all hardware for the sensors
*
* @details Depending on the defined sensor, the relevant hardware is initialised
*	@param 	none
*	@return none
*/
//void sensor_init(void){
//  //Initialize Button Interrupt
//  pinMode(BUTTON_PRESS_PIN, INPUT_PULLUP);
//
//  #ifdef GAS_SENSOR
//    gas_sensor_init();
//  #endif
//
//  #ifdef WATER_SENSOR
//    water_sensor_init();
//  #endif
//
//  //Initialise LED
//  led_init();
//}
//
///**
//*	Main sensor loop
//*
//*	@param 	none
//*	@return none
//*/
//void sensor_task(void){
//  static uint32_t lastDataSent_s = 0;
//
//  /*---- Check sensor values for warnings ----*/
//
//  #ifdef GAS_SENSOR
//  gas_sensor_check();
//  #endif
//
//  #ifdef WATER_SENSOR
//  water_sensor_check();
//  #endif
//
//  /*---- Button press check ----*/
//  sensor_button_pressed();
//
//    /*---- Send data every 10min ----*/
//  if(lastDataSent_s >= 600){
//    #ifdef GAS_SENSOR
//    gas_sensor_send_data();
//    #endif
//
//    #ifdef WATER_SENSOR
//    water_sensor_send_data();
//    #endif
//  }
//  //send data if there is any to send
//  lastDataSent_s++;
//}
//
//
///**
//*	Checks how long the button is pressed:
//*
//*	-	>5s The sensor is reset
//*	.
//*	@param 	none
//*	@return none
//*/
//void sensor_button_pressed(void){
//    uint8_t count = 0;
//    if(digitalRead(BUTTON_PRESS_PIN) == 0){
//      while(digitalRead(BUTTON_PRESS_PIN) == 0){
//        count++;
//        delay(500);
//        if(count >= 9){
//          sensor_reset();
//          break;
//        }
//      }
//    }
//}

/**
*	Resets the variables of sensor and base station ID in flash and restarts the microcontroller
*
*	@param 	none
*	@return none
*/
void sensor_reset(void){
  //set flash vars back
  //restart microcontroller --- TODO
}

