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

  struct rfm_client{
    uint8_t sensorID = 0;
    uint8_t baseStationID = 0;
    int TRANSMITPERIOD = 200; //transmit a packet to gateway so often (in ms)
    char payload[100];
    char buff[20];
    byte sendSize=0;
    bool requestACK = false;
    bool connected = false;
    swsPacket packet;
  };

  rfm_client clientSensor; 

  RFM69 radio;

  bool rfm_transmit(){
    packet_serializeData(clientSensor.packet, clientSensor.payload, clientSensor.sendSize);
    //send data
    return true;
  }

  swsPacket rfm_receive(){
    return clientSensor.packet;
  }

  public:

  void rfm_init(void){
      radio.initialize(FREQUENCY, clientSensor.sensorID, NETWORKID);
      radio.encrypt(ENCRYPTKEY);
  }

  bool rfm_connect(uint8_t sensorType){
    clientSensor.packet.packetID = packet_getNextPacketID();
    clientSensor.packet.packetType = CONNECT;
    clientSensor.packet.idBase = 0;
    clientSensor.packet.idSensor = 0;
    clientSensor.packet.data[0] = sensorType;
    clientSensor.packet.dataLength = 1;

    bool waitingforACK = true;
    uint8_t connectRetries = 0;
    swsPacket receivedPacket;
    
    while(waitingforACK){
      if(!rfm_transmit()) return false;
      delay(10);

      receivedPacket = rfm_receive();
      if(receivedPacket.packetType == CONNACK){
        connectRetries = 0;
        clientSensor.connected = true;
        clientSensor.sensorID = receivedPacket.data[0];
        clientSensor.baseStationID = receivedPacket.idBase;
        clientSensor.packet.idBase = receivedPacket.idBase;
        clientSensor.packet.idSensor = receivedPacket.data[0];
        waitingforACK = false;
      }
      connectRetries++;
      if(connectRetries == 10){
        connectRetries = 0;
        delay(1000);
      }
    }
    
  }

};



