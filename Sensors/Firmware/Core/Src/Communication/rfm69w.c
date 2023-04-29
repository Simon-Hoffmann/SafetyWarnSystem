#include "RFM69W.h"
#include "registers.h"
#include "SPI.h"
#include <string.h>

/************************Description************************
                      ________________
                     /                \
                    /      HOPERF      \
                   /____________________\
                  |       DK_RFM69       |
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
                  
                  

//  RF module:           RFM69
//  Carry Frequency:     315MHz/434MHz/868MHz/915MHz
//  Bit Rate:            1.2Kbps/2.4Kbps/4.8Kbps/9.6Kbps
//  Tx Power Output:     13dbm/10dbm/7dbm/4dbm
//  Frequency Deviation: +/-35KHz
//  Receive Bandwidth:   83KHz
//  Coding:              NRZ
//  Packet Format:       0x5555555555+0xAA2DD4+"HopeRF RFM COBRFM69-S" (total: 29 Bytes)
//  Tx Current:          about 45mA  (RFOP=+13dBm,typ.)
//  Rx Current:          about 16mA  (typ.)                 
**********************************************************/

/**********************************************************
**Parameter table define
**********************************************************/
const uint16_t RFM69FreqTbl[4][3] =
{ 
  {0x074e, 0x08c0, 0x0900}, //315MHz
  {0x076c, 0x0880, 0x0900}, //434MHz
  {0x07d9, 0x0800, 0x0900}, //868MHz
  {0x07e4, 0x08c0, 0x0900}, //915MHz
};

const uint16_t RFM69RateTbl[4][2] =
{
  {0x0368, 0x042B},         //BR=1.2K BW=83.333K
  {0x0334, 0x0415},         //BR=2.4K BW=83.333K  
  {0x031A, 0x040B},         //BR=4.8K BW=83.333K
  {0x030D, 0x0405},         //BR=9.6K BW=83.333K
};

const uint16_t RFM69PowerTbl[4][1] =
{
  {0x119F},                 //13dbm
  {0x119C},                 //10dbm
  {0x1199},                 //7dbm  
  {0x1196},                 //4dbm
};

const uint16_t RFM69ConfigTbl[18] =
{ 
  0x0200,                   //RegDataModul, FSK Packet  
  0x0502,                   //RegFdevMsb, 241*61Hz = 35KHz  
  0x0641,                   //RegFdevLsb
  0x130F,                   //RegOcp, Disable OCP
  0x1952,                   //RegRxBw , RxBW, 83KHz
  
  0x2C00,                   //RegPreambleMsb  
  0x2D05,                   //RegPreambleLsb, 5Byte Preamble
  0x2E90,                   //enable Sync.Word, 2+1=3bytes
  0x2FAA,                   //0xAA, SyncWord = aa2dd4
  0x302D,                   //0x2D
  0x31D4,                   //0xD4  
  0x3700,                   //RegPacketConfig1, Disable CRC£¬NRZ encode
  0x3815,                   //RegPayloadLength, 21bytes for length & Fixed length
  0x3C95,                   //RegFiFoThresh   
  
  0x1888,                   //RegLNA, 200R  
  0x581B,                   //RegTestLna, Normal sensitivity
  //0x582D,                   //RegTestLna, increase sensitivity with LNA (Note: consumption also increase!)
  0x6F30,                   //RegTestDAGC, Improved DAGC
  //0x6F00,                   //RegTestDAGC, Normal DAGC
  0x0104,                   //Enter standby mode    
};

const uint16_t RFM69RxTbl[5] =
{       
  0x2544,                   //DIO Mapping for Rx
  0x1310,                   //OCP enabled
  0x5A55,                   //Normal and Rx
  0x5C70,                   //Normal and Rx   
  0x0110                    //Enter Rx mode
};         

const uint16_t RFM69TxTbl[5] =
{
  0x2504,                   //DIO Mapping for Tx
  0x1310,                   //OCP enabled
  0x5A55,                   //Normal and Rx 
  0x5C70,                   //Normal and Rx 
  0x010C,                   //Enter Tx mode
};

const uint8_t  RFM69Data[] = {"HopeRF RFM COBRFM69-S"};

/**********************************************************
**Variable define
**********************************************************/
uint8_t gb_WaitStableFlag=0;                                    //State stable flag



/**********************************************************
**Name:     RFM69_Config
**Function: Initialize RFM69 & set it entry to standby mode
**Input:    none
**Output:   none
**********************************************************/
void RFM69_Config(void)
{
	uint8_t i;
  for(i=0;i<3;i++)                      
    SPIWrite(RFM69FreqTbl[gb_FreqBuf_Addr][i]);             //setting frequency parameter
  for(i=0;i<2;i++)
    SPIWrite(RFM69RateTbl[gb_RateBuf_Addr][i]);             //setting rf rate parameter
  SPIWrite(RFM69PowerTbl[gb_PowerBuf_Addr][0]);             //Setting output power parameter
  for(i=0;i<18;i++)
    SPIWrite(RFM69ConfigTbl[i]);                            //setting base parameter
}

/**********************************************************
**Name:     RFM69_EntryRx
**Function: Set RFM69 entry Rx_mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM69_EntryRx(void)
{
	uint8_t i;
  RFM69_Config();                                          //Module parameter setting
  
  for(i=0;i<5;i++)                                         //Define to Rx mode  
    SPIWrite(RFM69RxTbl[i]);
  
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusTx=0;                                           //Clear Tx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM69_EntryTx
**Function: Set RFM69 entry Tx_mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM69_EntryTx(void)
{
	uint8_t i;
  RFM69_Config();                                          //Module parameter setting
  
  for(i=0;i<5;i++)                                         //Define to Tx mode  
    SPIWrite(RFM69TxTbl[i]);
    
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusRx=0;                                           //Clear Rx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM69_TxWaitStable
**Function: Determine whether the state of stable Tx
**Input:    none
**Output:   none
**********************************************************/
void RFM69_TxWaitStable(void)
{ 
	uint8_t temp=0;
  if(gb_WaitStableFlag==0x0f) 
  {
    if(gb_SysTimer_1S!=0)
    {
      temp=SPIRead(0x27);
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
**Name:     RFM69_RxWaitStable
**Function: Determine whether the state of stable Rx
**Input:    none
**Output:   none
**********************************************************/
void RFM69_RxWaitStable(void)
{ 
	uint8_t temp=0;
  if(gb_WaitStableFlag==0x0f) 
  {
    if(gb_SysTimer_1S!=0)
    {
      temp=SPIRead(0x27);
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
**Name:     RFM69_ClearFIFO
**Function: Change to RxMode from StandbyMode, can clear FIFO buffer
**Input:    None
**Output:   None
**********************************************************/
void RFM69_ClearFIFO(void)
{
  SPIWrite(0x0104);                                        //Standby
  SPIWrite(0x0110);                                        //entry RxMode
}

/**********************************************************
**Name:     RFM69_Sleep
**Function: Set RFM69 to sleep mode 
**Input:    none
**Output:   none
**********************************************************/
void RFM69_Sleep(void)
{
  SPIWrite(0x0100);                                        //Sleep
}

/**********************************************************
**Name:     RFM69_Standby
**Function: Set RFM69 to Standby mode
**Input:    none
**Output:   none
**********************************************************/
void RFM69_Standby(void)
{
  SPIWrite(0x0104);                                        //Standby
}

/**********************************************************
**Name:     RFM69_ReadRSSI
**Function: Read the RSSI value
**Input:    none
**Output:   temp, RSSI value
**********************************************************/
uint8_t RFM69_ReadRSSI(void)
{
	uint16_t temp=0xff;
  if((SPIRead(0x24)&0x02)==0x02)
  {
    temp=SPIRead(0x24);
    temp=0xff-temp;
    temp>>=1;
    temp&=0xff;
  }
  return (uint8_t)temp;
}

/**********************************************************
**Name:     RFM69_RxPacket
**Function: Check for receive one packet
**Input:    none
**Output:   "!0"-----Receive one packet
**          "0"------Nothing for receive
**********************************************************/
uint8_t RFM69_RxPacket(uint8_t* data, uint8_t length)
{
 
  RFM69_RxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusRx=1;                                         //Rx state stable
  }       
 
  if(gb_StatusRx==1)
  {
    SPIBurstRead(0x00, data, length);
    RFM69_ClearFIFO();
  }
  else
    return(0);
}

/**********************************************************
**Name:     RFM69_TxPacket
**Function: Check RFM69 send over & send next packet
**Input:    none
**Output:   TxFlag=1, Send success
**********************************************************/
uint8_t RFM69_TxPacket(uint8_t* data, uint8_t length)
{
	uint8_t TxFlag=0;
  
  RFM69_TxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    if(gb_StatusTx==0)                                     //First entry Tx mode
    {
      gb_StatusTx=1;                                       //Tx state stable                        
      gw_TxTimer=gw_RF_SentInterval;                       //Send time interval
      BurstWrite(0x00, data, length);               //Send one packet data
      TxFlag=1;
      gb_WaitStableFlag=3;
    }
  }
  if(gb_StatusTx==1)
  {
    if(gb_WaitStableFlag==3)                   //Packet send over
    {
      gb_WaitStableFlag=0;
      RFM69_Standby();                                     //Entry Standby mode
    }   
    if(gw_TxTimer==0)                                      //It's time to Sending
    {
      gw_TxTimer=gw_RF_SentInterval;                       //Send time interval
      gb_SysTimer_1S=3;
      gb_WaitStableFlag=0x0f;                              //Promised to call mode stable decide
      SPIWrite(0x010C);                                    //Entry Tx mode
    }
    if(gb_WaitStableFlag==2)                               //Mode stable
    {
      BurstWrite(0x00, data, length);               //Send data
      TxFlag=1;
      gb_WaitStableFlag=3;
    }
  }
  return TxFlag;
}

/**********************************************************
**Parameter table define in test mode
**********************************************************/
const uint16_t RFM69TestConfigTbl[13] =
{ 
  0x0248,                   //RegDataModul, OOK
  0x0502,                   //RegFdevMsb, 241*61Hz = 35KHz  
  0x0641,                   //RegFdevLsb
  0x1952,                   //RegRxBw , RxBW, 83KHz
    
  0x130F,                   //RegOcp, Disable OCP
  0x1888,                   //RegLNA, 200R  
  0x2C00,                   //RegPreambleMsb  
  0x2D00,                   //RegPreambleLsb, 0Byte Preamble
  0x2E00,                   //Disable Sync
  0x3700,                   //RegPacketConfig1, Disable CRC£¬NRZ encode
  0x581B,                   //RegTestLna, Normal sensitivity
  0x6F30,                   //RegTestDAGC
  0x0104                    //Enter standby mode  
};

/**********************************************************
**Name:     RFM69_TestConfig
**Function: Initialize RFM69 in test mode & set it entry to standby mode
**Input:    none
**Output:   none
**********************************************************/
void RFM69_TestConfig(void)
{
	uint8_t i;
  for(i=0;i<3;i++)                      
    SPIWrite(RFM69FreqTbl[gb_FreqBuf_Addr][i]);             //setting frequency parameter
  for(i=0;i<2;i++)
    SPIWrite(RFM69RateTbl[gb_RateBuf_Addr][i]);             //setting rf rate parameter
  SPIWrite(RFM69PowerTbl[gb_PowerBuf_Addr][0]);             //Setting output power parameter
  
  for(i=0;i<13;i++)
    SPIWrite(RFM69TestConfigTbl[i]);                        //setting base parameter
}

/**********************************************************
**Name:     RFM69_EntryTestRx
**Function: Set RFM69 entry Rx test mode
**Input:    None
**Output:   None
**********************************************************/
void RFM69_EntryTestRx(void)
{
	uint8_t i;
  //Input_RFData();
  RFM69_Config();                                          //Module parameter setting
  for(i=0;i<5;i++)                                         //Define to Rx mode  
    SPIWrite(RFM69RxTbl[i]);
  
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusTx=0;                                           //Clear Tx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM69_EntryTestTx
**Function: Set RFM69 entry Tx test mode
**Input:    None
**Output:   None
**********************************************************/
void RFM69_EntryTestTx(void)
{
	uint8_t i;
  
 // Output_RFData();                                         //DIO2=0
  //RFData=0;
  RFM69_TestConfig();
  for(i=0;i<5;i++)                                         //Define to Tx mode  
    SPIWrite(RFM69TxTbl[i]);
    
  gb_SysTimer_1S=3;                                        //System time = 3S
  gb_StatusRx=0;                                           //Clear Rx status flag 
  gb_WaitStableFlag=0x0f;                                  //State stable flag initial
}

/**********************************************************
**Name:     RFM69_TestRx
**Function: RFM69 Rx test mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM69_TestRx(void)
{
  RFM69_RxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusRx=1;    
  }
}

/**********************************************************
**Name:     RFM69_TestTx
**Function: RFM69 Tx test mode
**Input:    None
**Output:   "0" for Error Status
**********************************************************/
void RFM69_TestTx(void)
{ 
  RFM69_TxWaitStable();
  if(gb_WaitStableFlag==1)
  {
    gb_WaitStableFlag=2;
    gb_StatusTx=1;  
                                        
    //RFData=1;                                              //DIO2=1
  }
}

uint8_t gb_WorkmodeBackup=0;
/**********************************************************
**Name:     RFM69_Running
**Function: RFM69 running function
**Input:    mode, work mode(FS-T,FS-R,T-T,T-R)
            WorkStatus, work status(normal,standby,sleep)
            ParaChangeFlag, Parameter changed when ParaChangeFlag=1
            *TxFlag, Tx success flag affect Tx count
            *RxFlag, Rx success flag affect Rx count
            *RSSI, RSSI value
**Output:   none
**********************************************************/
void RFM69_Running(uint8_t mode,uint8_t WorkStatus,uint8_t ParaChangeFlag,uint8_t *TxFlag,uint8_t *RxFlag,uint8_t *RSSI, DataBuffer* data)
{ 
  uint8_t temp;
  if(WorkStatus==C_ModuleWorkMode_FSK)                  	 //Normal status
  {
    switch(mode)
    {
      case C_SysMode_FSKTx:                                //Normal send mode(Abbreviation:FS-T)
        if(ParaChangeFlag==1)                              //Parament changed
        {                   
          RFM69_EntryTx();
        }         
        temp= RFM69_TxPacket(data->data, data->size);
        if(temp==1){*TxFlag=1;}
        break;
      case C_SysMode_FSKRx:                                //Normal receive mode(Abbreviation:FS-R)      
        if(ParaChangeFlag==1)
        {           
          RFM69_EntryRx();
        }
        temp=RFM69_RxPacket(data->data, data->size);
        if(temp==1)
        {
          *RxFlag=1;
               //Buzzer on When received data success
        }
        
        *RSSI=RFM69_ReadRSSI();                            //Read RSSI
        break;
      case C_SysMode_TestTx:                               //Test send mode(Abbreviation:T-T)   
        if(ParaChangeFlag==1)
        {           
          RFM69_EntryTestTx();
        }
        RFM69_TestTx();
        break;
      case C_SysMode_TestRx:                               //Test receive mode(Abbreviation:T-R)  
        if(ParaChangeFlag==1)
        {       
          RFM69_EntryTestRx();
        }
        RFM69_TestRx();
        
        *RSSI=RFM69_ReadRSSI();                            //Read RSSI
        break;
    } 
  }
  else if(WorkStatus==C_ModuleWorkMode_Standby)            //Standby status
  {
    if(gb_WorkmodeBackup!=C_ModuleWorkMode_Standby)
    	RFM69_Standby();
  }
  else if(WorkStatus==C_ModuleWorkMode_Sleep)              //Sleep status
  {
    if(gb_WorkmodeBackup!=C_ModuleWorkMode_Sleep)
    	RFM69_Sleep();
  }
  gb_WorkmodeBackup=WorkStatus;
}
