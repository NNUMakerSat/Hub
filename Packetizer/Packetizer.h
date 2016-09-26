/*
 * Packetizer.h
 *
 *  Created on: Sep 1, 2016
 *      Author: AaronEwing
 */

#ifndef PACKETIZER_H_
#define PACKETIZER_H_

void init_Buffers(void);

void Packetizer(uint16_t source_ID, uint8_t bytes_Read);

void write_To_Packetizer(uint8_t packet_Data, uint8_t SB_Select);

#endif /* PACKETIZER_H_ */
