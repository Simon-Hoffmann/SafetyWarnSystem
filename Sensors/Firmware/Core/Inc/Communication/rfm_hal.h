/*
 * rfm_hal.h
 *
 *  Created on: May 2, 2023
 *      Author: Simon
 */

#ifndef INC_COMMUNICATION_RFM_HAL_H_
#define INC_COMMUNICATION_RFM_HAL_H_

#include <stdbool.h>
#include <stdint.h>

void noInterrupts(void);                // function to disable interrupts
void interrupts(void);                  // function to enable interrupts
void RFM69_SetCSPin(bool state);          // function to control the GPIO tied to RFM69 chip select (parameter HIGH or LOW)
bool RFM69_ReadDIO0Pin(void);       // function to read GPIO connected to RFM69 DIO0 (RFM69 interrupt signalling)
uint8_t SPI_transfer8(uint8_t data);     // function to transfer 1byte on SPI with readback
void Serialprint(char* string);            // function to print to serial port a string
bool Timeout_IsTimeout1(void);      // function for timeout handling, checks if previously set timeout expired
void Timeout_SetTimeout1(uint16_t ms); // function for timeout handling, sets a timeout, parameter is in milliseconds (ms)

#endif /* INC_COMMUNICATION_RFM_HAL_H_ */
