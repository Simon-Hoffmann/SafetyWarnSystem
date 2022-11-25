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
  uint8_t data[100];
  size_t dataLength;
};

uint8_t getNextPacketID();
bool serializeData(swsPacket packet, char (&payload)[100], uint8_t &sendSize);
#endif