/**
*	@file 	sensor.cpp
*	@author Simon Hoffmann
*	@brief	sensor implementation
*
* Contains all standard functionalities between all sensors
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include <stdint.h>
#include <Arduino.h>
#include "sensor.h"
#include "led.h"
#include "gas_sensor.h"
#include "water_sensor.h"
#include "debug.h"
#include "door_sensor.h"
#include "rfm.h"
#include "packet.h"

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

const uint8_t BUTTON_PRESS_PIN = 13;
volatile uint8_t sensorID = 0;
volatile uint8_t baseID = 0;
																														
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

void sensor_led_startup_indication(void);
void sensor_reset(void);
void sensor_button_pressed(void);
void sensor_connectBase(void);

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
*	Initialises all hardware for the sensors
*
* @details Depending on the defined sensor, the relevant hardware is initialised
*	@param 	none
*	@return none
*/
void sensor_init(void){
  //Initialize Button Interrupt
  pinMode(BUTTON_PRESS_PIN, INPUT_PULLUP);

  #ifdef GAS_SENSOR
    gas_sensor_init();
  #endif

  #ifdef WATER_SENSOR
    water_sensor_init();
  #endif

  #ifdef DOOR_SENSOR
    door_sensor_init();
  #endif

  //Initialise LED
  led_init();

  #ifdef DEBUG
  Serial.println("Sensor initialization success");
  #endif

  #ifndef HARDWARE_DEBUG
  sensor_connectBase();
  #endif
}

/**
*	Handles initial connection with base station
*
* @details Retires every minute to connect to a base station
*	@param 	none
*	@return none
*/
void sensor_connectBase(void){
  struct swsPacket packet;
  packet.idSensor = 0;
  packet.idBase = 0;
  packet.packetType = CONNECT;
  #ifdef GAS_SENSOR
  packet.data[0] = 2;
  #endif
  #ifdef WATER_SENSOR
  packet.data[0] = 1;
  #endif
  #ifdef DOOR_SENSOR
  packet.data[0] = 3;
  #endif
  bool connected = false;

  while(!connected){
    if(rfm_send(&packet)){
      connected = true;
      sensorID = packet.idSensor;
      baseID = packet.idBase;
    }
    #ifdef DEBUG
    Serial.println("Connection to base station could not be established, trying again in 1min");
    #endif
    #ifdef GAS_SENSOR
    //The gas sensor should still be able to have its normal function of detection even when not connected with the base station
      int timer = 0;
      while(timer < 6000){
          gas_sensor_check_no_connection();
          delay(10);
          timer++;
      }
      
    #else
      delay(60000);
    #endif
  }

}

/**
*	Main sensor loop
*	
*	@param 	none
*	@return none
*/
void sensor_task(void){
  static uint32_t lastDataSent_s = 0;

  /*---- Check sensor values for warnings ----*/

  #ifdef GAS_SENSOR
  if(gas_sensor_check()){
    lastDataSent_s = 0;
  }
  #endif

  #ifdef WATER_SENSOR
  if(water_sensor_check()){
    lastDataSent_s = 0;
  }
  #endif

  #ifdef DOOR_SENSOR
  if(door_sensor_check()){
    lastDataSent_s = 0;
  }
  #endif

  /*---- Button press check ----*/
  sensor_button_pressed();

    /*---- Send data every 10min ----*/
  if(lastDataSent_s >= 600){
    #ifdef GAS_SENSOR
    gas_sensor_send_data();
    #endif

    #ifdef WATER_SENSOR
    water_sensor_send_data();
    #endif

    #ifdef DOOR_SENSOR
    door_sensor_send_data();
    #endif
  }
  //send data if there is any to send
  lastDataSent_s++;
}


/**
*	Checks how long the button is pressed:
*
*	-	>5s The sensor is reset
*	.
*	@param 	none
*	@return none
*/
void sensor_button_pressed(void){
    uint8_t count = 0;
    if(digitalRead(BUTTON_PRESS_PIN) == 0){
      while(digitalRead(BUTTON_PRESS_PIN) == 0){
        count++;
        delay(500);
        if(count >= 9){
          sensor_reset();
          break;
        }
      }
    }
}

void(*resetFunc)(void) = 0;

/**
*	Resets the variables of sensor and base station ID in flash and restarts the microcontroller
*
*	@param 	none
*	@return none
*/
void sensor_reset(void){
  resetFunc();
}

