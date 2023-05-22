/*
 * RFM69W.h
 *
 *  Created on: 26 Apr 2023
 *      Author: Simon
 */

#include <stdint.h>
#include <stdbool.h>
#include "packet.h"

#ifndef __RFM_h__
#define __RFM_h__

#define RF69_MAX_DATA_LEN       61 // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)
#define RF69_SPI_CS             SS // SS is the SPI slave select pin, for instance D10 on ATmega328


#define RF69_IRQ_PIN          2
#define RF69_IRQ_NUM          0


#define CSMA_LIMIT              -90 // upper RX signal sensitivity threshold in dBm for carrier sense access
#define RF69_MODE_SLEEP         0 // XTAL OFF
#define RF69_MODE_STANDBY       1 // XTAL ON
#define RF69_MODE_SYNTH         2 // PLL ON
#define RF69_MODE_RX            3 // RX MODE
#define RF69_MODE_TX            4 // TX MODE

// available frequency bands
#define RF69_315MHZ            31 // non trivial values to avoid misconfiguration
#define RF69_433MHZ            43
#define RF69_868MHZ            86
#define RF69_915MHZ            91

#define null                  0
#define COURSE_TEMP_COEF    -90 // puts the temperature reading in the ballpark, user can fine tune the returned value
#define RF69_BROADCAST_ADDR 255
#define RF69_CSMA_LIMIT_MS 1000
#define RF69_TX_LIMIT_MS   1000
#define RF69_FSTEP  61.03515625 // == FXOSC / 2^19 = 32MHz / 2^19 (p13 in datasheet)

// TWS: define CTLbyte bits
#define RFM69_CTL_SENDACK   0x80
#define RFM69_CTL_REQACK    0x40

//
#define ISRFM69HW  1

// module interface, platform specific
extern void noInterrupts();                // function to disable interrupts
extern void interrupts();                  // function to enable interrupts
extern void RFM69_SetCSPin(bool);          // function to control the GPIO tied to RFM69 chip select (parameter HIGH or LOW)
extern bool RFM69_ReadDIO0Pin(void);       // function to read GPIO connected to RFM69 DIO0 (RFM69 interrupt signalling)
extern uint8_t SPI_transfer8(uint8_t);     // function to transfer 1byte on SPI with readback
extern void Serialprint(char*);            // function to print to serial port a string
extern bool Timeout_IsTimeout1(void);      // function for timeout handling, checks if previously set timeout expired
extern void Timeout_SetTimeout1(uint16_t); // function for timeout handling, sets a timeout, parameter is in milliseconds (ms)

bool RFM69_initialize(uint8_t freqBand, uint8_t nodeID, uint16_t networkID);
bool RFM69_canSend();
void RFM69_send(uint8_t toAddress, const void* buffer, uint8_t bufferSize, bool requestACK);
bool RFM69_receive(uint8_t* buffer, uint8_t bufferSize);
void RFM69_setMode(uint8_t newMode);



#endif
