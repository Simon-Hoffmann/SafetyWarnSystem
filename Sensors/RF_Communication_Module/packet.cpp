#include "packet.h"


bool serializeData(swsPacket packet, char (&payload)[100], uint8_t &sendSize){
  payload[0] = packet.idSensor;
  payload[1] = packet.idBase;
  payload[2] = packet.packetID;
  payload[3] = packet.packetType;

  switch(packet.packetType){
    //no data
    case PING:
    case ACK:
      payload[4] = '~';
      sendSize = 5;
      break;
    case CONNECT:
    case CONNACK:
    case SEND:
      sendSize = 4;
      for(unsigned int i = 0; i < packet.dataLength; i++){
        payload[i + 4] = packet.data[i];
        sendSize++;
      }
      payload[sendSize] = '~';
      break;
    default:
      return false;
  }
  return true;
}

bool deserializeData(swsPacket &packet, char (&payload)[100], uint8_t receiveSize){
  packet.idSensor = payload[0];
  packet.idBase = payload[1];
  packet.packetID = payload[2];
  packet.packetType = (PacketType) payload[3];

  switch(packet.packetType){
    //no data
    case PING:
    case ACK:
      break;
    case CONNECT:
    case CONNACK:
    case SEND:
      for(unsigned int i = 0; i < receiveSize - 4; i++){
        packet.data[i] = payload[i + 4];
      }
      break;
    default:
      return false;
  }
  return true;
}

uint8_t getNextPacketID(){
  static uint8_t packetID = 255;
  return packetID++;
}

