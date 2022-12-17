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

const uint8_t BUTTON_PRESS_PIN = 0;

static int mq135_val, mq5_val;
static uint32_t alarmOffTime_s = 30;
static bool buzzer = false;
																														
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

bool gas_sensor_read_warning(void);
void gas_sensor_alarm(void);

/* ----------------------- F U N C T I O N S  -------------------------- */

void gas_sensor_init(void){
  pinMode(MQ5_PIN, INPUT);
  pinMode(MQ135_PIN, INPUT);
}

void gas_sensor_check(void){
  if(alarmOffTime_s){
    alarmOffTime_s--;
  }else{
    if(gas_sensor_read_warning()){
      gas_sensor_alarm();
    }
  }
}

void gas_sensor_alarm(void){
  uint16_t alarmTime = 0;
  buzzer = true;
  //push data into fifo
  while(buzzer){
    //toggle buzzer sound
    //try send data over wifi  here i need to send it directly with short ack timeout so the buzzer still happens even if data cant be sent just call the send func that looks if anything is in fifo
    if(digitalRead(BUTTON_PRESS_PIN) == 0){
      buzzer = false;
    }
    if(alarmTime > 1800)
    delay(300);
    alarmTime++;
  }
}

void gas_sensor_send_data(void){
  //send data over rfm (push into fifo)
}

void gas_sensor_disable(void){
  //check if buzzer on
  //turn buzzer off
  alarmOffTime_s = 600;
}


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