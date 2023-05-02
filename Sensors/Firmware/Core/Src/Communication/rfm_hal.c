/*
 * registers.c
 *
 *  Created on: 26 Apr 2023
 *      Author: Simon
 */

#include "rfm_hal.h"
#include "stm32f3xx_hal.h"
#include "spi.h"

static volatile uint32_t timeoutSet;

void noInterrupts(void){

}

void interrupts(void){

}

void RFM69_SetCSPin(bool state){
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

bool RFM69_ReadDIO0Pin(void){
	return HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_6);
}       // function to read GPIO connected to RFM69 DIO0 (RFM69 interrupt signalling)

uint8_t SPI_transfer8(uint8_t data){
	return SPI_RX_TX(data);
}     // function to transfer 1byte on SPI with return

void Serialprint(char* c){

}            // function to print to serial port a string

bool Timeout_IsTimeout1(void){
	if(HAL_GetTick() > timeoutSet){
		return true;
	}
	return false;
}      // function for timeout handling, checks if previously set timeout expired


void Timeout_SetTimeout1(uint16_t a){
	timeoutSet = HAL_GetTick() + a;
} // function for timeout handling, sets a timeout, parameter is in milliseconds (ms)
