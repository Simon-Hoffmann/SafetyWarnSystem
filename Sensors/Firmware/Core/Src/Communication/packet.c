/*
 * packet.c
 *
 *  Created on: Feb 7, 2023
 *      Author: Simon
 */

#include "packet.h"



void packet_serializeData(SWS_Packet* packet, DataBuffer* data){
	data->data[0] = packet->idSensor;
	data->data[1] = packet->idBase;
	data->data[2] = packet->packetID;
	data->data[3] = packet->packetType;

	switch(packet->packetType){
	case CONNECT:
	case CONNACK:
		data->data[4] = packet->data[0];
		data->data[5] = '~';
		data->size = 6;
		break;
	case SEND:
		switch(*(packet->data)){
			case WaterSensor:
				data->data[4] = packet->data[0];
				data->data[5] = packet->data[1];
				data->data[6] = packet->data[2];
				data->data[7] = packet->data[3];
				data->data[8] = '~';
				data->size = 9;
				break;
			case SmokeSensor:
				data->data[4] = packet->data[0];
				data->data[5] = packet->data[1];
				data->data[6] = packet->data[2];
				data->data[7] = packet->data[3];

				data->data[8] = packet->data[4];
				data->data[9] = packet->data[5];
				data->data[10] = packet->data[6];
				data->data[11] = packet->data[7];
				data->data[12] = '~';
				data->size = 13;
				break;
			case DoorSensor:
				break;
			default:
				break;
		}

		break;
	default:
		data->data[4] = '~';
		data->size = 5;
		break;
	}
}

void packet_deserializeData(SWS_Packet* packet, DataBuffer* data){
	packet->idSensor = data->data[0];
	packet->idBase = data->data[1];
	packet->packetID = data->data[2];
	packet->packetType = data->data[3];

	switch(packet->packetType){
	case CONNECT:
	case CONNACK:
		packet->data[0] = data->data[4];
		break;
	case SEND:
		switch(*(packet->data)){
			case WaterSensor:
				packet->data[0] = data->data[4];
				packet->data[1] = data->data[5];
				packet->data[2] = data->data[5];
				packet->data[3] = data->data[7];
				break;
			case SmokeSensor:
				packet->data[0] = data->data[4];
				packet->data[1] = data->data[5];
				packet->data[2] = data->data[6];
				packet->data[3] = data->data[7];

				packet->data[4] = data->data[8];
				packet->data[5] = data->data[9];
				packet->data[6] = data->data[10];
				packet->data[7] = data->data[11];
				break;
			case DoorSensor:
				break;
			default:
				break;
		}

		break;
	default:
		break;
	}
}

