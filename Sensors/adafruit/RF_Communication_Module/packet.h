#include <stdint.h>
#include <stdio.h>

#ifndef PACKET_H_
#define PACKET_H_

enum PacketType : uint8_t{
  CONNECT = 0,
  CONNACK,
  UNDEFINED,
  ACK,
  SEND,
  PING,
  DISCONNECT
};

struct swsPacket{
  uint8_t idSensor;
  uint8_t idBase;
  uint8_t packetID;
  PacketType packetType;
  uint8_t dataLength;
  uint8_t data[100];
};

uint8_t packet_getNextPacketID();
bool packet_serializeData(swsPacket *packet, char *payload, uint8_t *sendSize);
bool packet_deserializeData(swsPacket *packet, uint8_t *payload, uint8_t receiveSize);
#endif