#include <stdint.h>
#include <SPI.h>
#include <RH_RF69.h>
#include "packet.h"
#include "debug.h"
#include "sensor.h"

#define RF69_FREQ 868.0

#define RFM69_CS    8
#define RFM69_INT   3
#define RFM69_RST   4

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

/**
*	Initializes the RFM69HWC module on the adafruit
*
*	@param 	none
*	@return none
*/
void rfm_init(void){
  pinMode(RFM69_RST, OUTPUT);
  digitalWrite(RFM69_RST, LOW);

  // manual reset
  digitalWrite(RFM69_RST, HIGH);
  delay(10);
  digitalWrite(RFM69_RST, LOW);
  delay(10);

  if (!rf69.init()) {
    #ifdef DEBUG
    Serial.println("RFM69 radio init failed");
    #endif
    while (1);
  }

  if(!rf69.setFrequency(RF69_FREQ)){
    #ifdef DEBUG
    Serial.println("setFrequency failed");
    #endif
  }

  #ifdef DEBUG
  Serial.println("RFM69 radio init OK!");
  #endif
  rf69.setTxPower(20, true);

  // The encryption key has to be the same as the one in the server
  /*
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
  rf69.setEncryptionKey(key);
  */
}

/**
*	Sends data over rfm module and handles any replies
*
*	@param 	packet - the data to send
*	@return bool - if a ACK was received
*/
bool rfm_send(swsPacket* packet){
  static uint8_t packetnumber = 0;
  //Send
  char payload[100];
  uint8_t sendLength;
  //Receive
  struct swsPacket receiveData;
  uint8_t buf[100];
  uint8_t len = sizeof(buf);


  uint8_t count = 0;
  packet->packetID = packetnumber++;
  packet_serializeData(packet,payload, &sendLength);

  while(true){
    count++;

    #ifdef DEBUG
    Serial.print("Sending "); Serial.println(payload);
    #endif

    rf69.send((uint8_t *)payload, sendLength);
    rf69.waitPacketSent();

    if (rf69.waitAvailableTimeout(10)) {
      // Should be a reply message for us now
      if (rf69.recv(buf, &len)) {
        packet_deserializeData(&receiveData, buf, len);

        #ifdef DEBUG
        Serial.print("Got a reply: ");
        Serial.println((char*)buf);
        #endif

        switch(packet->packetType){
          case ACK:
          case CONNACK:
            *packet = receiveData;
            break;
          case DISCONNECT: 
            packet->packetType = ACK;
            char payload2[100];
            packet_serializeData(packet,payload2, &sendLength);
            rf69.send((uint8_t *)payload2, sendLength);
            rf69.waitPacketSent();
            sensor_reset();
            return true;
            break;
          default:
            break;
        }
      } 
      if(count >= 10){
        #ifdef DEBUG
        Serial.print("No reply received");
        #endif

        return false;
      }
    } 
  }
}


