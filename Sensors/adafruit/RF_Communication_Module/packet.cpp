#include "packet.h"


bool packet_serializeData(swsPacket *packet, char *payload, uint8_t *sendSize){
  payload[0] = packet->idSensor;
  payload[1] = packet->idBase;
  payload[2] = packet->packetID;
  payload[3] = packet->packetType;

  switch(packet->packetType){
    //no data
    case PING:
    case ACK:
      *sendSize = 4;
      break;
    case CONNECT:
    case CONNACK:
      payload[4] = packet->data[0];
      *sendSize = 5;
      break;
    case SEND:
      payload[4] = packet->dataLength;
      *sendSize = 5;
      for(unsigned int i = 0; i < packet->dataLength; i++){
        payload[i + 5] = packet->data[i];
        (*sendSize)++;
      }
      break;
    default:
      return false;
  }
  return true;
}

bool packet_deserializeData(swsPacket *packet, uint8_t *payload, uint8_t receiveSize){
  packet->idSensor = payload[0];
  packet->idBase = payload[1];
  packet->packetID = payload[2];
  packet->packetType = (PacketType) payload[3];

  switch(packet->packetType){
    //no data
    case PING:
    case ACK:
    case DISCONNECT:
      break;
    case CONNECT:
    case CONNACK:
      packet->data[0] = payload[4];
      break;
    case SEND:
      packet->dataLength =  payload[4];
      for(unsigned int i = 0; i < receiveSize - 5; i++){
        packet->data[i] = payload[i + 4];
      }
      break;
    default:
      return false;
  }
  return true;
}

uint8_t packet_getNextPacketID(){
  static uint8_t packetID = 255;
  return packetID++;
}

