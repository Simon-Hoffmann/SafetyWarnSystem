/**
*	@file 	gas_sensor.cpp
*	@author Simon Hoffmann
*	@brief	Gas sensor implementation
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include "gas_sensor.h"
#include "Arduino.h"

#ifdef GAS_SENSOR

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

/*Pin connections for sensors*/
#define BUZZER_PIN  16
#define MQ5_PIN     27
#define MQ135_PIN   26

const uint8_t BUTTON_PRESS_PIN = 0;


static int mq135_val, mq5_val;
static uint32_t alarmOffTime_s = 30;
static bool buzzer = false;
																														
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

bool gas_sensor_read_warning(void);
void gas_sensor_alarm(void);
void gas_sensor_disable(void);

/* ----------------------- F U N C T I O N S  -------------------------- */

/**
*	Initializes MQ135, MQ5 and Buzzer and sets buzzer off
*
*	@param 	none
*	@return none
*/
void gas_sensor_init(void){
  pinMode(MQ5_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

/**
*	Does nothing if sensor is switched off for the duration
* Otherwise checks if values are in normal range, if not, starts the alarm and notifies the user
*
*	@param 	none
*	@return none
*/
void gas_sensor_check(void){
  if(alarmOffTime_s){
    alarmOffTime_s--;
  }else{
    if(gas_sensor_read_warning()){
      gas_sensor_alarm();
    }
  }
}

/**
* Turns buzzer on until button is pressed or the timeout is reached
* Sends the data over rf
*
*	@param 	none
*	@return none
*/
void gas_sensor_alarm(void){
  uint16_t alarmTime = 0;
  buzzer = true;
  //push data into fifo
  while(buzzer){
    //toggle buzzer sound
    digitalWrite(BUZZER_PIN, HIGH);
    
    //try send data over wifi  here i need to send it directly with short ack timeout so the buzzer still happens even if data cant be sent just call the send func that looks if anything is in fifo
    if((digitalRead(BUTTON_PRESS_PIN) == 0) || (alarmTime > 600)){
      buzzer = false;
      alarmOffTime_s = 600;
    }
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
    alarmTime++;
  }
}

void gas_sensor_send_data(void){
  //send data over rfm (push into fifo)
}

/**
* Gets the value of the gas sensors and checks if they are under the safety threshhold
*
*	@param 	none
*	@return 1 if values are dangerous, otherwise 0 if all good
*/
bool gas_sensor_read_warning(void){
  mq135_val = analogRead(MQ135_PIN);
  mq5_val = analogRead(MQ5_PIN);

  if(mq135_val > 800){
    return true;
  }
  if(mq5_val > 250){
    return true;
  }
  return false;
}


#endif