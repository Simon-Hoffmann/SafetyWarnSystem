/**
*	@file 	gas_sensor.cpp
*	@author Simon Hoffmann
*	@brief	Gas sensor implementation
*/

/*-------------------------- I N C L U D E S ----------------------------*/

#include "gas_sensor.h"
#include "Arduino.h"
#include "debug.h"
#include "rfm.h"
#include "packet.h"
#include "sensor.h"

#ifdef GAS_SENSOR

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

/*Pin connections for sensors*/
#define BUZZER_PIN  10
#define MQ5_PIN     12
#define MQ135_PIN   11

const uint8_t BUTTON_PRESS_PIN = 13;


static int mq135_val, mq5_val;
static uint32_t alarmOffTime_s = 30;
static bool buzzer = false;
static bool dataReceived = false;

static struct swsPacket packet;
																														
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
bool gas_sensor_check(void){
  if(alarmOffTime_s){
    alarmOffTime_s--;
  }else{
    if(gas_sensor_read_warning()){
      #ifdef DEBUG
        Serial.println("gas_sensor_read_warning");
      #endif
      dataReceived = false;
      gas_sensor_alarm();
      return true;
    }
  }
  return false;
}

/**
*	Does nothing if sensor is switched off for the duration
* Otherwise checks if values are in normal range, if not, starts the alarm 
*
*	@param 	none
*	@return none
*/
bool gas_sensor_check_no_connection(void){
  if(alarmOffTime_s){
    alarmOffTime_s--;
  }else{
    if(gas_sensor_read_warning()){
      #ifdef DEBUG
        Serial.println("gas_sensor_read_warning");
      #endif
      dataReceived = true;
      gas_sensor_alarm();
      return true;
    }
  }
  return false;
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

  while(buzzer){
    //toggle buzzer sound
    digitalWrite(BUZZER_PIN, HIGH);
    
    if(!dataReceived){
      gas_sensor_send_data();
    }


    if((digitalRead(BUTTON_PRESS_PIN) == 0) || (alarmTime > 600)){
      buzzer = false;
      alarmOffTime_s = 600;
      #ifdef DEBUG
        Serial.println("Gas sensor alarm turned off");
      #endif
    }
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
    delay(500);
    alarmTime++;
  }
  if(!dataReceived){
    sensor_reset();
  }
}

/**
* Sets the packet content and sends data over rfm
*
*	@param 	none
*	@return none
*/
void gas_sensor_send_data(void){
  packet.idSensor = sensorID;
  packet.packetType = SEND;
  packet.idBase = baseID;
  packet.dataLength = 6;
  packet.data[0] = 2;
  packet.data[1] = alarmOffTime_s;
  packet.data[2] = mq5_val >> 8;
  packet.data[3] = (uint8_t) mq5_val;
  packet.data[4] = mq135_val >> 8;
  packet.data[5] = (uint8_t) mq135_val;

  #ifndef HARDWARE_DEBUG
  if(rfm_send(&packet)){
    dataReceived = true;
  }
  #endif
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
  #ifdef DEBUG
    Serial.print("MQ135: "); Serial.println(mq135_val);
    Serial.print("MQ5: "); Serial.println(mq5_val);
  #endif
      
  if((mq135_val > 800) || (mq5_val > 800)){
    return true;
  }
  return false;
}


#endif