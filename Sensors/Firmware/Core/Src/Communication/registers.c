/*
 * registers.c
 *
 *  Created on: 26 Apr 2023
 *      Author: Simon
 */

#include "registers.h"

uint8_t  gb_RxData[32];                                         //Receive data buffer
uint8_t  gb_StatusTx=0;                                         //Tx status flag
uint8_t  gb_StatusRx=0;                                         //Rx status flag
uint8_t  gb_ErrorFlag=0;                                        //Error flag
uint16_t gw_TxLedTimer=0;                                       //Tx LED timer
uint8_t  gb_TxLedCount=0;                                       //Tx LED count
uint8_t  gb_SysTimer_1S=0;                                      //System timer for 1S

uint8_t  gb_KeyDownFlag=0;                                      //Key down flag
uint8_t  gb_KeyValue;                                           //Key value
uint16_t gw_KeyDebounceTimer=0;                                 //Debounce timer for key

uint8_t  gb_BeepOnFlag=0;                                       //Buzzer on flag
uint8_t  gb_BeepOnCount;                                        //Buzzer on count
uint16_t gw_ToneBuf[30];                                        //Tone cache
uint8_t  gb_BeepTimerH;                                         //Timer reload value for buzzer
uint8_t  gb_BeepTimerL;
uint16_t gw_BeepTimer;                                          //Buzzer timer

uint16_t gw_LCDFlashTimer=0;                                    //LCD flash display timer

uint8_t  gb_SystemMode=0;                                       //System mode
uint8_t  gb_SysMode_Set;                                        //System mode-Set mode
uint16_t gw_RF_SentInterval;                                    //Send interval
uint16_t gw_TxTimer=0;                                          //Send timer (time base 1S)
uint8_t  gb_ModuleWorkEnableFlag=0;                             //Enable module work in Tx/Rx mode
uint8_t  gb_ModuleWorkMode=0;                                   //Module work mode
uint8_t  gb_RF_RSSI=0;                                          //RF RSSI signal
uint8_t  gb_FirstPowerUpFlag=0;                                 //First power flag
uint8_t  gb_ParameterChangeFlag=0;                              //Parameter change flag

uint8_t  gb_FreqBuf_Addr=2;                                     //RF center frequency cache address
uint8_t  gb_RateBuf_Addr=0;
uint8_t  gb_PowerBuf_Addr=0;
uint8_t  gb_FdevBuf_Addr=0;
uint8_t  gb_BandBuf_Addr=0;
uint8_t  gb_Modem_Addr=0;
uint8_t  gb_Modem_Addr_Backup=0;

//u8  gb_SpreadFactor_Addr=0;

uint16_t gw_SendDataCount=0;                                    //Send data count
uint16_t gw_ReceiveDataCount=0;                                 //Receive data count
