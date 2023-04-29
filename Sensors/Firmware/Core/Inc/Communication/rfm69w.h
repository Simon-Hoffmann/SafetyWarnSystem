/*
 * RFM69W.h
 *
 *  Created on: 26 Apr 2023
 *      Author: Simon
 */

#include <stdint.h>
#include "packet.h"

#ifndef __RFM_h__
#define __RFM_h__



void RFM69_Running(uint8_t mode,uint8_t WorkStatus,uint8_t ParaChangeFlag,uint8_t *TxFlag,uint8_t *RxFlag,uint8_t *RSSI, DataBuffer* data);

#endif
