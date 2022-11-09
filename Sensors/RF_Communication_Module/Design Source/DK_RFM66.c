// ========================================================
/// @file       DK_RFM66.c
/// @brief      RFM66 basic application
/// @version    V2.1
/// @date       2013/12/25
/// @company    HOPE MICROELECTRONICS Co., Ltd.
/// @website    http://www.hoperf.com
/// @author     Geman Deng
/// @mobile     +86-013244720618
/// @tel        0755-82973805 Ext:846
// ========================================================
#include "DK_RFM.h"

/************************Description************************
                      ________________
                     /                \
                    /      HOPERF      \
                   /____________________\
                  |       DK_RFM66       |
    (MCU)DIO1 --- | DIO4             GND | --- GND
    (MCU)DIO0 --- | DIO3              A7 | --- 
  (MCU)RFData --- | DIO2              A6 | ---  
   (MCU)nIRQ1 --- | DIO1              A5 | --- 
   (MCU)nIRQ0 --- | DIO0              A4 | --- 
    (MCU)MISO --- | MISO              A3 | --- 
    (MCU)MOSI --- | MOSI              A2 | --- 
     (MCU)SCK --- | SCK               A1 | --- 
     (MCU)nCS --- | NSS               A0 | --- 
  (MCU)PORChk --- | RESET            VCC | --- VCC             
                  |______________________|
                  
                  

//  RF module:           RFM66
//  Carry Frequency:     315MHz/434MHz/868MHz/915MHz
//  Bit Rate:            1.2Kbps/2.4Kbps/4.8Kbps/9.6Kbps
//  Tx Power Output:     20dbm/17dbm/14dbm/11dbm
//  Frequency Deviation: +/-35KHz
//  Receive Bandwidth:   83KHz
//  Coding:              NRZ
//  Packet Format:       0x5555555555+0xAA2DD4+"HopeRF RFM COBRFM66S" (total: 29 Bytes)
//  Tx Current:          about 130mA  (RFOP=+20dBm,typ.)
//  Rx Current:          about 16mA  (typ.)                 
**********************************************************/

/**********************************************************
**Parameter table define
**********************************************************/
const u16 RFM66FreqTbl[2][3] = 
{ 
  {0x06d9, 0x0700, 0x0800}, //868MHz
  {0x06e4, 0x07c0, 0x0800}, //915MHz
};

const u16 RFM66RateTbl[4][2] = 
{
  {0x0268, 0x032B},         //BR=1.2Kbps
  {0x0234, 0x0315},         //BR=2.4Kbps
  {0x021A, 0x030B},         //BR=4.8Kbps
  {0x020D, 0x0305},         //BR=9.6Kbps
};

const u16 RFM66PowerTbl[4] = 
{
  0x09FF,                   //20dbm  
  0x09FC,                   //17dbm
  0x09F9,                   //14dbm
  0x09F6,                   //11dbm 
};

const u16 RFM66ConfigTbl[17] = 
{ 
  0x0402,                   //RegFdevMsb    35KHz 
  0x053D,                   //RegFdevLsb
  0x0B0B,                   //RegOcp      关闭Ocp
  //0x0C20,                 //RegLNA            High & LNA Disable
  0x0C23,                   //RegLNA            High & LNA Enable
  0x1212,                   //RegRxBw     RxBW  83KHz
  0x1FA0,                   //RegPreambleDet    Enable 2Byte 
  //0x1F20,                 //RegPreambleDet    Disable 
  0x2500,                   //RegPreambleMsb  
  0x2606,                   //RegPreambleLsb  6Byte Preamble
  0x2792,                   //RegSyncConfig     开Sync 2+1=3字节
  0x28AA,              			//SyncWord = aa2dd4
  0x292D,              			//
  0x2AD4,              			//
  0x3000,                   //RegPacketConfig1  Disable CRC，NRZ数据包
  0x3140,                   //RegPacketConfig2  Packet Mode
  0x3215,                   //RegPayloadLength  接收21字节 Fixed
  0x3595,                   //RegFiFoThresh   21字节，不空就发射  
  0x0101                    //Standby 
};

const u16 RFM66RxTbl[5] = 
{   
  0x090F,                   //RFIO Pin
  0x400C,                   //DIO0 Mapping for IRQ / DIO2 for RxData
  0x4100,                   //
  0x5A84,                   //Normal and Rx 
  0x0105                    //Enter Rx mode  
};         
    
const u16 RFM66TxTbl[4] = 
{
  0x4000,                   //DIO0 Mapping for IRQ / DIO2 for RxData
  0x4100,                   //          
  0x5A87,                   //20dBm Tx
  0x0103,                   //Enter Tx mode
};

const u8  RFM66Data[] = {"HopeRF RFM COBRFM66-S"};

/**********************************************************
**Variable define
**********************************************************/
u8 gb_WaitStableFlag=0;                                    //State stable flag



/**********************************************************
**Name:     RFM66_Config
**Function: Initialize RFM66 & set it entry to standby mode
**Input:    none
**Output:   none
**********************************************************/
void RFM66_Config(void)
{
  u8 i;
  
  for(i=0;i<3;i++)                      
    SPIWrite(RFM66FreqTbl[gb_FreqBuf_Addr][i]);           //setting frequency parameter
  for(i=0;i<2;i++)
    SPIWrite(RFM66RateTbl[gb_RateBuf_Addr][i]);           //setting rf rate parameter
  SPIWrite(RFM66PowerTbl[gb_PowerBuf_Addr]);              //Setting output power parameter
    
  for(i=0;i<17;i++)
    SPIWrite(RFM66ConfigTbl[i]);                          //setting base parameter
}

/**********************************************************
**Name:     RFM66_EntryRx
**Function: Set RFM66 entry Rx_mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM66_EntryRx(void)
{
  u8 i;
  
  Input_RFData();
    
  RFM66_Config();  
  for(i=0;i<5;i++)                                         //Define to Rx mode  
    SPIWrite(RFM66RxTbl[i]);
      
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusTx=0;                                           //Clear Tx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM66_EntryTx
**Function: Set RFM66 entry Tx_mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM66_EntryTx(void)
{
  u8 i;
  
  RFM66_Config();
  for(i=0;i<4;i++)                                         //Define to Tx mode  
    SPIWrite(RFM66TxTbl[i]);
  
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusRx=0;                                           //Clear Rx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial 
}

/**********************************************************
**Name:     RFM66_TxWaitStable
**Function: Determine whether the state of stable Tx
**Input:    none
**Output:   none
**********************************************************/
void RFM66_TxWaitStable(void)
{ 
  u8 temp=0;
  if(gb_WaitStableFlag==0x0f) 
  {
    if(gb_SysTimer_1S!=0)
    {
      temp=SPIRead(0x3E);
      if((temp&0xA0)==0xA0 && temp!=0xff)
      {
        gb_WaitStableFlag=1; 
      }
    }
    else
    {
      gb_ErrorFlag=1;
      gb_WaitStableFlag=0;
    }
  }
}

/**********************************************************
**Name:     RFM66_RxWaitStable
**Function: Determine whether the state of stable Rx
**Input:    none
**Output:   none
**********************************************************/
void RFM66_RxWaitStable(void)
{ 
  u8 temp=0;
  if(gb_WaitStableFlag==0x0f) 
  {
    if(gb_SysTimer_1S!=0)
    {
      temp=SPIRead(0x3E);
      if((temp&0xC0)==0xC0 && temp!=0xff)
      {
        gb_WaitStableFlag=1;        
      }
    }
    else
    {
      gb_ErrorFlag=1;
      gb_WaitStableFlag=0;
    }
  }
}

/**********************************************************
**Name:     RFM66_ClearFIFO
**Function: Change to RxMode from StandbyMode, can clear FIFO buffer
**Input:    None
**Output:   None
**********************************************************/
void RFM66_ClearFIFO(void)
{
  SPIWrite(0x0101);                                        //Standby
  SPIWrite(0x0105);                                        //entry RxMode
}

/**********************************************************
**Name:     RFM66_Sleep
**Function: Set RFM66 to sleep mode 
**Input:    none
**Output:   none
**********************************************************/
void RFM66_Sleep(void)
{
  SPIWrite(0x0100);                                        //Sleep
}

/**********************************************************
**Name:     RFM66_Standby
**Function: Set RFM66 to Standby mode
**Input:    none
**Output:   none
**********************************************************/
void RFM66_Standby(void)
{
  SPIWrite(0x0101);                                        //Standby
}

/**********************************************************
**Name:     RFM66_ReadRSSI
**Function: Read the RSSI value
**Input:    none
**Output:   temp, RSSI value
**********************************************************/
u8 RFM66_ReadRSSI(void)
{
  u8 temp=0xff;
  temp=SPIRead(0x11);
  temp>>=1;
  temp=127-temp;                                           //127:Max RSSI
  return temp;
}

/**********************************************************
**Name:     RFM66_RxPacket
**Function: Check for receive one packet
**Input:    none
**Output:   "!0"-----Receive one packet
**          "0"------Nothing for receive
**********************************************************/
u8 RFM66_RxPacket(void)
{
  u8 i; 
  
  RFM66_RxWaitStable();  
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusRx=1;                                         //Rx state stable
  }       
 
  if((nIRQ0) && (gb_StatusRx==1))
  { 
    for(i=0;i<21;i++) 
      gb_RxData[i] = 0x00;  
    
    SPIBurstRead(0x00, gb_RxData, 21);  
    RFM66_ClearFIFO();
    for(i=0;i<14;i++)
    {
      if(gb_RxData[i]!=RFM66Data[i])
        break;  
    }
    if(i>=14)                                              //比较成功
    {
      return(1);
    }
    else
      return(0);
  }
  else
    return(0);
}

/**********************************************************
**Name:     RFM66_TxPacket
**Function: Check RFM66 send over & send next packet
**Input:    none
**Output:   TxFlag=1, Send success
**********************************************************/
u8 RFM66_TxPacket(void)
{
  u8 TxFlag=0;
  
  RFM66_TxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    if(gb_StatusTx==0)                                     //First entry Tx mode
    {   
      gb_StatusTx=1;                                       //Rx state stable                        
      gw_TxTimer=gw_RF_SentInterval;                       //Send time interval
      BurstWrite(0x00, (u8 *)RFM66Data, 21);               //Send one packet data
      TxFlag=1;
      gb_WaitStableFlag=3;
    }
  }
  if(gb_StatusTx==1)
  {
    if(gb_WaitStableFlag==3 && nIRQ0==1)                   //Packet send over
    {
      gb_WaitStableFlag=0;
      RFM66_Standby();                                     //Entry Standby mode
    }   
    if(gw_TxTimer==0)                                      //It's time to Sending
    {
      gw_TxTimer=gw_RF_SentInterval;                       //Send time interval
      gb_SysTimer_1S=3;
      gb_WaitStableFlag=0x0f;                              //Promised to call mode stable decide
      SPIWrite(0x0103);                                    //Entry Tx mode
    }
    if(gb_WaitStableFlag==2)                               //Mode stable
    {
      BurstWrite(0x00, (u8 *)RFM66Data, 21);               //Send data
      TxFlag=1;
      gb_WaitStableFlag=3;
    }
  }
  return TxFlag;
}

/**********************************************************
**Parameter table define in test mode
**********************************************************/
const u16 RFM66TestConfigTbl[10] = 
{ 
  0x0402,                   //RegFdevMsb    35KHz 
  0x053D,                   //RegFdevLsb
  0x0B0B,                   //RegOcp      关闭Ocp
  0x0C20,                   //RegLNA            High & LNA Disable
  //0x0C23,                 //RegLNA            High & LNA Enable
  0x1212,                   //RegRxBw     RxBW  83KHz ??
  0x1FA0,                   //RegPreambleDet    Enable 2Byte 
  //0x1F20,                 //RegPreambleDet    Disable 
  0x2500,                   //RegPreambleMsb  
  0x2600,                   //RegPreambleLsb  0Byte Preamble
  0x3100,                   //RegPacketConfig2  Continuous Mode 
  0x0101                    //Standby 
};

/**********************************************************
**Name:     RFM66_TestConfig
**Function: Initialize RFM66 in test mode & set it entry to standby mode
**Input:    none
**Output:   none
**********************************************************/
void RFM66_TestConfig(void)
{
  u8 i;
  
  for(i=0;i<3;i++)                      
    SPIWrite(RFM66FreqTbl[gb_FreqBuf_Addr][i]);           //setting frequency parameter
  for(i=0;i<2;i++)
    SPIWrite(RFM66RateTbl[gb_RateBuf_Addr][i]);           //setting rf rate parameter
  SPIWrite(RFM66PowerTbl[gb_PowerBuf_Addr]);              //Setting output power parameter
    
  for(i=0;i<10;i++)
    SPIWrite(RFM66TestConfigTbl[i]);                      //setting base parameter
}

/**********************************************************
**Name:     RFM66_EntryTestRx
**Function: Set RFM66 entry Rx test mode
**Input:    None
**Output:   None
**********************************************************/
void RFM66_EntryTestRx(void)
{
  u8 i;
  Input_RFData(); 
  RFM66_TestConfig();                                         //Module parameter setting
  for(i=0;i<5;i++)                                         //Define to Rx mode  
    SPIWrite(RFM66RxTbl[i]);
  
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusTx=0;                                           //Clear Tx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM66_EntryTestTx
**Function: Set RFM66 entry Tx test mode
**Input:    None
**Output:   None
**********************************************************/
void RFM66_EntryTestTx(void)
{
  u8 i;
  
  Output_RFData();                                         //DIO2=0                                 
  RFData=0;
  RFM66_TestConfig();
  for(i=0;i<4;i++)                                         //配置发送参数 
    SPIWrite(RFM66TxTbl[i]);  
  SPIWrite(0x0100+0x20+0x03);															 //OOK Tx mode
    
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusRx=0;                                           //Clear Rx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM66_TestRx
**Function: RFM66 Rx test mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM66_TestRx(void)
{
  RFM66_RxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusRx=1;    
  }
}

/**********************************************************
**Name:     RFM66_TestTx
**Function: RFM66 Tx test mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM66_TestTx(void)
{ 
  RFM66_TxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusTx=1;  
                                        
    RFData=1;                                              //DIO2=1
  }
}

u8 gb_WorkmodeBackup=0;
/**********************************************************
**Name:     RFM66_Running
**Function: RFM66 running function
**Input:    mode, work mode(FS-T,FS-R,T-T,T-R)
            WorkStatus, work status(normal,standby,sleep)
            ParaChangeFlag, Parameter changed when ParaChangeFlag=1
            *TxFlag, Tx success flag affect Tx count
            *RxFlag, Rx success flag affect Rx count
            *RSSI, RSSI value
**Output:   none
**********************************************************/
void RFM66_Running(u8 mode,u8 WorkStatus,u8 ParaChangeFlag,u8 *TxFlag,u8 *RxFlag,u8 *RSSI)
{ 
  u8 temp;  
  if(WorkStatus==C_ModuleWorkMode_FSK)                  //Normal status
  {
    switch(mode)
    {
      case C_SysMode_FSKTx:                                //Normal send mode(Abbreviation:FS-T)
        if(ParaChangeFlag==1)                              //Parament changed
        {                   
          RFM66_EntryTx();
        }         
        temp=RFM66_TxPacket(); 
        if(temp==1){*TxFlag=1;}
        break;
      case C_SysMode_FSKRx:                                //Normal receive mode(Abbreviation:FS-R)      
        if(ParaChangeFlag==1)
        {           
          RFM66_EntryRx();
        }
        temp=RFM66_RxPacket(); 
        if(temp==1)
        {
          *RxFlag=1;
          BeepOn((u16*)MusicTab_RxSuccess);                //Buzzer on When received data success
        }
        
        *RSSI=RFM66_ReadRSSI();                           //Read RSSI
        break;
      case C_SysMode_TestTx:                               //Test send mode(Abbreviation:T-T)   
        if(ParaChangeFlag==1)
        {           
          RFM66_EntryTestTx();
        }
        RFM66_TestTx();
        break;
      case C_SysMode_TestRx:                               //Test receive mode(Abbreviation:T-R)  
        if(ParaChangeFlag==1)
        {       
          RFM66_EntryTestRx();
        }
        RFM66_TestRx();
        
        *RSSI=RFM66_ReadRSSI();                           //Read RSSI
        break;
    } 
  }
  else if(WorkStatus==C_ModuleWorkMode_Standby)            //Standby status
  {
    if(gb_WorkmodeBackup!=C_ModuleWorkMode_Standby)
    	RFM66_Standby();
  }
  else if(WorkStatus==C_ModuleWorkMode_Sleep)              //Sleep status
  {
    if(gb_WorkmodeBackup!=C_ModuleWorkMode_Sleep)
    	RFM66_Sleep();
  }
  gb_WorkmodeBackup=WorkStatus;
}