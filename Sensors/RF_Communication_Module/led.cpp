/**
*	@file 	led.cpp
*	@author Simon Hoffmann
*	@brief	LED implementation
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include "led.h"
#include <stdint.h>
#include "Arduino.h"

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

static volatile uint8_t LED_STATE = 0;
																														
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

void led_startup_indication(void);

/* ----------------------- F U N C T I O N S  -------------------------- */

void led_init(void){
    //Initialise LED
  pinMode(LED_BUILTIN, OUTPUT);
  led_startup_indication();
}

/**
*	Led blinks on startup or reset for user indication
*
*	@param 	none
*	@return none
*/
void led_startup_indication(void){
  digitalWrite(LED_BUILTIN, 0);
  delay(200);
  digitalWrite(LED_BUILTIN, 1);
  delay(200);
  digitalWrite(LED_BUILTIN, 0);
  delay(200);
  digitalWrite(LED_BUILTIN, 1);
  delay(200);
  digitalWrite(LED_BUILTIN, 0);
  delay(200);
  digitalWrite(LED_BUILTIN, 1);
  delay(200);
  digitalWrite(LED_BUILTIN, 0);
  delay(200);
  digitalWrite(LED_BUILTIN, 1);
}

void led_gas_indication(void){
   digitalWrite(LED_BUILTIN, 0);
  delay(800);
  digitalWrite(LED_BUILTIN, 1);
  delay(800);
  digitalWrite(LED_BUILTIN, 0);
  delay(800);
  digitalWrite(LED_BUILTIN, 1);
  delay(800);
}

