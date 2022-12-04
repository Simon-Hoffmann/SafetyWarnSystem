/**
*	@file 	gas_sensor.cpp
*	@author Simon Hoffmann
*	@brief	Gas sensor implementation
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include "gas_sensor.h"
#include "Arduino.h"
#include "led.h"

#ifdef GAS_SENSOR

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

#define BUZZER_PIN  16
#define MQ5_PIN     27
#define MQ135_PIN   26
																														
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

/* ----------------------- F U N C T I O N S  -------------------------- */

void gas_sensor_init(void){
  pinMode(MQ5_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
  //sensor needs time to warmup - turn the alarm for the sensors off
  delay(10000);
}


void gas_sensor_read(void){
  if(analogRead(MQ135_PIN) > 800){
    //TODO
  }
  if(analogRead(MQ5_PIN) > 250){
    //TODO
  }
}

#endif