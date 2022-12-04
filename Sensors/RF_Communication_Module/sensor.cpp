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
#include <FreeRTOS.h>
#include <event_groups.h>
#include "sensor.h"
#include "led.h"
#include "gas_sensor.h"

/* ----------- V A R I A B L E S   &  C O N S T A N T S  --------------- */

/*Event flags*/
#define BUTTON_PRESSED_FLAG       (1UL << 0UL)
#define SENSOR_TASK_FLAG          (1UL << 1UL)
#define GAS_SENSOR_READY_FLAG     (1UL << 2UL)
#define BUZZER_ACTIVE_FLAG        (1UL << 3UL)

/* Event grounp handler variable */
EventGroupHandle_t  SensorEventGroup;

/* A constant to define 500ms delay*/
const TickType_t xDelay500ms =  pdMS_TO_TICKS(500);

const uint8_t BUTTON_INTERRUPT_PIN = 0;


																														
/* ------------- F u n c t i o n  P r o t o t y p e s  ----------------- */

void sensor_led_startup_indication(void);
void sensor_reset(void);
void IRQ_Button_Pressed(void);


/* ----------------------- F U N C T I O N S  -------------------------- */

void sensor_init(void){
  SensorEventGroup  =  xEventGroupCreate();

  //Initialize Button Interrupt
  pinMode(BUTTON_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BUTTON_INTERRUPT_PIN), IRQ_Button_Pressed, FALLING);

  #ifdef GAS_SENSOR
    gas_sensor_init();
  #endif

  //Initialise LED
  led_init();
}

void sensor_Task(void *pvParameters){
  EventBits_t EventGroupValue;
    while(1){
      EventGroupValue  = xEventGroupWaitBits(SensorEventGroup, SENSOR_TASK_FLAG ,pdTRUE ,pdTRUE , portMAX_DELAY );

      #ifdef GAS_SENSOR
      /*If gas sensor is active, read values and handle*/
      EventGroupValue  = xEventGroupGetBits(SensorEventGroup);
      if((EventGroupValue & GAS_SENSOR_READY_FLAG)){
          //TODO

      }
      #endif
    }
}

void sensor_timer_Task(void *pvParameters){
  while(1){
    vTaskDelay(xDelay500ms);
    xEventGroupSetBits(SensorEventGroup, SENSOR_TASK_FLAG);

    #ifdef GAS_SENSOR
      /*If the gas sensor is deactivated, then wait 10min, then activate it*/
      EventBits_t EventGroupValue  = xEventGroupGetBits(SensorEventGroup);
      if(!(EventGroupValue & GAS_SENSOR_READY_FLAG)){
        static int count = 0;
        count++;
        if(count >= 1200){
          xEventGroupSetBits(SensorEventGroup, GAS_SENSOR_READY_FLAG);
        }
      }
    #endif
  }
}

/**
*	Checks how long the button is pressed:
*
*	-	<5s If sensor is of type Gas sensor, the buzzer is turned off if active
*	-	>5s The sensor is reset
*	.
*	@param 	none
*	@return none
*/
void sensor_button_Task(void *pvParameters){
  while(1){
    xEventGroupWaitBits(SensorEventGroup, BUTTON_PRESSED_FLAG ,pdTRUE ,pdTRUE , portMAX_DELAY );
    //Incase a debounce occurs
    uint8_t count = 0;
    vTaskDelay(xDelay500ms);
    if(digitalRead(BUTTON_INTERRUPT_PIN) == 0){
      while(digitalRead(BUTTON_INTERRUPT_PIN) == 0){
        count++;
        vTaskDelay(xDelay500ms);
        if(count >= 9){
          sensor_reset();
          break;
        }
      }
      #ifdef GAS_SENSOR
      //turn buzzer off if it is active---------TODO
      #endif
    }
  }
}

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


/**
*	Interrupt handler for button press event
*
*	@param 	none
*	@return none
*/
void IRQ_Button_Pressed(void){
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();

  // If interrupts come faster than 200ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 200) 
  {
    xEventGroupSetBits(SensorEventGroup, BUTTON_PRESSED_FLAG);
  }
  last_interrupt_time = interrupt_time;
}


