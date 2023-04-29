/*
 * packet.h
 *
 *  Created on: Feb 7, 2023
 *      Author: Simon
 */

#ifndef INC_COMMUNICATION_PACKET_H_
#define INC_COMMUNICATION_PACKET_H_

#include <stdint.h>


enum PacketType{
	CONNECT = 1,
	CONNACK = 2,

	ACK = 4,
	SEND = 5,
	PING = 6,
	DISCONNECT = 7
};

enum SensorType{
	WaterSensor = 1,
	SmokeSensor = 2,
	DoorSensor = 3
};

typedef struct DataBuffer{
	uint8_t data[25];
	uint8_t size;
}DataBuffer;

typedef struct SWS_Packet{
	uint8_t idSensor;
	uint8_t idBase;
	uint8_t packetID;
	uint8_t packetType;
	uint8_t data[20];
}SWS_Packet;


typedef struct ConnackData{
	uint8_t SensorID;
}ConnackData;

typedef struct ConnectData{
	uint8_t SensorType;
}ConnectData;

typedef struct WaterSensorData{
	uint8_t SensorType;
	uint8_t Submerged;
	uint16_t Temperature;
}WaterSensorData;

typedef struct SmokeSensorData{
	uint8_t SensorType;
	uint8_t GasDetectionTimeout_Minutes;
	uint16_t SmokeSensor;
	uint16_t GasSensor;
	uint16_t Temperature;
}SmokeSensorData;

void packet_serializeData(SWS_Packet* packet, DataBuffer* data);
void packet_deserializeData(SWS_Packet* packet, DataBuffer* data);


#endif /* INC_COMMUNICATION_PACKET_H_ */
