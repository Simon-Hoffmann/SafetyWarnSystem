/*
 * SPI.h
 *
 *  Created on: 26 Apr 2023
 *      Author: Simon
 */

#ifndef INC_COMMUNICATION_SPI_H_
#define INC_COMMUNICATION_SPI_H_

#include <stdint.h>


void SPICmd8bit(uint8_t WrPara);
uint8_t SPIRead(uint8_t adr);
uint8_t SPIRead8bit(void);
uint8_t SPI_RX_TX(uint8_t WrPara);
void SPIWrite(unsigned int WrPara);
void SPIBurstRead(uint8_t adr, uint8_t *ptr, uint8_t length);
void BurstWrite(uint8_t adr, uint8_t *ptr, uint8_t length);

#endif /* INC_COMMUNICATION_SPI_H_ */
