/*
 * registers.h
 *
 *  Created on: 26 Apr 2023
 *      Author: Simon
 */
#include <stdint.h>

#ifndef INC_COMMUNICATION_REGISTERS_H_
#define INC_COMMUNICATION_REGISTERS_H_

/**********************************************************
**Constant define
**********************************************************/
//Define system mode
#define C_SysMode_ModuleSel 0x01                           //Module select mode
#define C_SysMode_EntrySet  0x02                           //Entry set mode
#define C_SysMode_ProduceTest  0x03                        //Produce test mode
#define C_SysMode_Modem     0x04                           //Set Modem mode(FSK /OOK /LoRa )
#define C_SysMode_FSK       0x10
#define C_SysMode_OOK       0x11
#define C_SysMode_LoRa      0x12
#define C_SysMode_TestTx    0x21                           //Work mode
#define C_SysMode_TestRx    0x22
//#define C_SysMode_Standby   0x22
#define C_SysMode_Sleep     0x23
#define C_SysMode_Set       0x24
#define C_SysMode_FSKTx     0x25
#define C_SysMode_FSKRx     0x26
#define C_SysMode_FSKTRx    0x27
#define C_SysMode_OOKTx     0x28
#define C_SysMode_OOKRx     0x29
#define C_SysMode_OOKTRx    0x2A
#define C_SysMode_LoRaTx    0x2B
#define C_SysMode_LoRaRx    0x2C
#define C_SysMode_LoRaTRx   0x2D

//Define set mode
#define C_SysMode_Set_Freq  0x51                           //Set mode
#define C_SysMode_Set_Power 0x52
#define C_SysMode_Set_Rate  0x53
#define C_SysMode_Set_Fdev  0x54
#define C_SysMode_Set_Band  0x55

//Define sent time interval
#define C_RF_SentInterval   500                            //0.5S

//Define module work mode
#define C_ModuleWorkMode_FSK     0
#define C_ModuleWorkMode_OOK     1
#define C_ModuleWorkMode_LoRa    2
#define C_ModuleWorkMode_Standby 3
#define C_ModuleWorkMode_Sleep   4

//Key value table
#define C_KeyNone           0x00
#define C_KeyUp             0x01
#define C_KeyDown           0x02
#define C_KeyLeft           0x04
#define C_KeyRight          0x08


/**********************************************************
**Extern variable define
**********************************************************/
extern uint8_t  gb_RxData[32];                                  //Receive data buffer
extern uint8_t  gb_StatusTx;                                    //Tx status flag
extern uint8_t  gb_StatusRx;                                    //Rx status flag
extern uint8_t  gb_ErrorFlag;                                   //Error flag
extern uint16_t gw_TxLedTimer;                                  //Tx LED timer
extern uint8_t  gb_TxLedCount;                                  //Tx LED count
extern uint8_t  gb_SysTimer_1S;                                 //System timer for 1S

extern uint8_t  gb_KeyValue;                                    //Key value
extern uint16_t gw_KeyDebounceTimer;                            //Debounce timer for key

extern uint8_t  gb_BeepOnFlag;                                  //Buzzer on flag
extern uint8_t  gb_BeepOnCount;                                 //Buzzer on count
extern uint16_t gw_ToneBuf[30];                                 //Tone cache
extern uint8_t  gb_BeepTimerH;                                  //Timer reload value for buzzer
extern uint8_t  gb_BeepTimerL;
extern uint16_t gw_BeepTimer;                                   //Buzzer timer

extern uint16_t gw_LCDFlashTimer;                               //LCD flash display timer

extern uint8_t  gb_SystemMode;                                  //System mode
extern uint8_t  gb_SysMode_Set;                                 //System mode-Set mode
extern uint16_t gw_RF_SentInterval;                             //Send interval
extern uint16_t gw_TxTimer;                                     //Send timer (time base 1ms)
extern uint8_t  gb_ModuleWorkEnableFlag;                        //Enable module work in Tx/Rx mode
extern uint8_t  gb_ModuleWorkMode;                              //Module work mode
extern uint8_t  gb_RF_Module;                                   //RF module
extern uint8_t  gb_RF_RSSI;                                     //RF RSSI signal
extern uint8_t  gb_FirstPowerUpFlag;                                 //First power flag
extern uint8_t  gb_ParameterChangeFlag;                         //Parameter change flag

extern uint8_t  gb_FreqBuf_Addr;                                //RF center frequency cache address
extern uint8_t  gb_RateBuf_Addr;
extern uint8_t  gb_PowerBuf_Addr;
extern uint8_t  gb_FdevBuf_Addr;
extern uint8_t  gb_BandBuf_Addr;
extern uint8_t  gb_Modem_Addr;
extern uint8_t  gb_Modem_Addr_Backup;
//extern u8  gb_SpreadFactor_Addr;

extern uint16_t gw_SendDataCount;                               //Send data count
extern uint16_t gw_ReceiveDataCount;                            //Receive data count


#endif /* INC_COMMUNICATION_REGISTERS_H_ */
