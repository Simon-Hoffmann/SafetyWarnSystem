/*
 * SPI.c
 *
 *  Created on: 26 Apr 2023
 *      Author: Simon
 */


#include "SPI.h"
#include "stm32f3xx_hal.h"
#include "main.h"

/**********************************************************
**Name:     SPICmd8bit
**Function: SPI Write one byte
**Input:    WrPara
**Output:   none
**note:     use for burst mode
**********************************************************/
void SPICmd8bit(uint8_t WrPara)
{
	HAL_SPI_Transmit(&hspi2, &WrPara, 1, HAL_MAX_DELAY);
}

/**********************************************************
**Name:     SPIRead8bit
**Function: SPI Read one byte
**Input:    None
**Output:   result byte
**Note:     use for burst mode
**********************************************************/
uint8_t SPIRead8bit(void)
{
	uint8_t RdPara = 0;

	HAL_SPI_Receive(&hspi2, &RdPara, 1, HAL_MAX_DELAY);
	return(RdPara);
}

/**********************************************************
**Name:     SPIRead
**Function: SPI Read CMD
**Input:    adr -> address for read
**Output:   None
**********************************************************/
uint8_t SPIRead(uint8_t adr)
{
	uint8_t tmp;

	HAL_SPI_Receive(&hspi2, &tmp, 1, HAL_MAX_DELAY);
  return(tmp);
}

/**********************************************************
**Name:     SPIWrite
**Function: SPI Write CMD
**Input:    WrPara -> address & data
**Output:   None
**********************************************************/
void SPIWrite(unsigned int WrPara)
{
	HAL_SPI_Transmit(&hspi2, &WrPara, 4, HAL_MAX_DELAY);
}

/**********************************************************
**Name:     SPIBurstRead
**Function: SPI burst read mode
**Input:    adr-----address for read
**          ptr-----data buffer point for read
**          length--how many bytes for read
**Output:   None
**********************************************************/
void SPIBurstRead(uint8_t adr, uint8_t *ptr, uint8_t length)
{
	HAL_SPI_Receive(&hspi2, ptr, length, 10);
}

/**********************************************************
**Name:     SPIBurstWrite
**Function: SPI burst write mode
**Input:    adr-----address for write
**          ptr-----data buffer point for write
**          length--how many bytes for write
**Output:   none
**********************************************************/
void BurstWrite(uint8_t adr, uint8_t *ptr, uint8_t length)
{
	HAL_SPI_Transmit(&hspi2, ptr, length, HAL_MAX_DELAY);
}
