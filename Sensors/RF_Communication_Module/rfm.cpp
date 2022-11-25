#include <RFM69.h>         
#include <RFM69_ATC.h>
#include <stdint.h>
#include "packet.h"

#define NETWORKID     100  // keep IDENTICAL on all nodes that talk to each other
#define GATEWAYID     1    // "central" node

#define FREQUENCY   RF69_868MHZ
#define ENCRYPTKEY    "sampleEncryptKey"    //exactly the same 16 characters/bytes on all nodes!

#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
#define ATC_RSSI      -80

class rfm{
  private:
  uint8_t sensorID = 0;
  uint8_t baseStationID = 0;

  int TRANSMITPERIOD = 200; //transmit a packet to gateway so often (in ms)
  char payload[100];
  char buff[20];
  byte sendSize=0;
  boolean requestACK = false;

  swsPacket packet;

  RFM69 radio;

  bool rfm_transmit(swsPacket sendPacket, bool waitforACK){
    if(serializeData(sendPacket, payload, sendSize)){
      //send data
      if(waitforACK){
        //receiveData with timeout
      }
    }else{
      return false;
    }
    return true;
  }

  public:

  void rfm_init(void){
      radio.initialize(FREQUENCY, sensorID, NETWORKID);
      radio.encrypt(ENCRYPTKEY);
  }

  bool rfm_connect(uint8_t sensorType){
    packet.packetID = getNextPacketID();
    packet.packetType = CONNECT;
    packet.idBase = 0;
    packet.idSensor = 0;
    packet.data[0] = sensorType;
    packet.dataLength = 1;

    if(rfm_transmit(packet, true)){

    }else{
      return false;
    }
    return true;
  }

};



