/*
 * Packetizer.c
 *
 *  Created on: Sep 1, 2016
 *      Author: AaronEwing
 */
#include <stdint.h>
#include <stdbool.h>
#include "Circular_Buffer.h"

uint16_t g_POLY_Exp_Count = 0;
uint16_t g_RAD_Exp_Count = 0;
uint16_t g_IMU_Exp_Count = 0x3FFF;
uint16_t source_ID_Exp_Count = 0x0000;



uint8_t Control_Bytes[6] = {0x50, 0x50, 0x50, 0x50, 0, 0};


void Packetizer(uint16_t source_ID) {
	switch (source_ID) {
	case 0:													// IMU
		source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
		source_ID &= 0x3FFFF;

		source_ID_Exp_Count = source_ID | g_IMU_Exp_Count;	// give source_ID_Exp_Count appropriate value

		if (g_IMU_Exp_Count >= 0x3FFF) {					// increment counter
			g_IMU_Exp_Count = 0x0000;
		} else {
			++g_IMU_Exp_Count;
		}

		Control_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
		Control_Bytes[5] = source_ID_Exp_Count;
		break;

	case 1:													// RAD
		source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
		source_ID &= 0x3FFFF;

		source_ID_Exp_Count = source_ID | g_RAD_Exp_Count;	// give source_ID_Exp_Count appropriate value

		if (g_RAD_Exp_Count >= 0x3FFF) {					// increment counter
			g_RAD_Exp_Count = 0x0000;
		} else {
			++g_RAD_Exp_Count;
		}

		Control_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
		Control_Bytes[5] = source_ID_Exp_Count;
		break;

	case 2:													// POLY
		source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
		source_ID &= 0x3FFFF;

		source_ID_Exp_Count = source_ID | g_IMU_Exp_Count;	// give source_ID_Exp_Count appropriate value

		if (g_IMU_Exp_Count >= 0x3FFF) {					// increment counter
			g_IMU_Exp_Count = 0x0000;
		} else {
			++g_IMU_Exp_Count;
		}

		Control_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
		Control_Bytes[5] = source_ID_Exp_Count;
		break;

	default:												// ERROR, so packet
		break;
	}
}


void init_Buffers(void) {


//	uint8_t control_Bits[6] = {0x50, 0x50, 0x50, 0x0C};		// Creates control  its

										// init buffer just does what is natural for the circular buffer
	init_Buffer();						// (reason for the same thing is so that it is all in one place)
										// please note that max is 1000 bytes
}

void write_To_Packetizer(uint8_t packet_Data, uint8_t SB_Select) {

}

/*
	uint8_t init_Counter1;
	uint8_t init_Counter_Control = 0;
	uint8_t init_Counter2;

	for (init_Counter1 = 0; init_Counter1 < 23; ++init_Counter1) {		// writes 23 packets worth of control bits
		switch (init_Counter_Control) {
		case 0:
		case 1:
		case 2:
			write_Buffer(0x50);			// writes 0x50 3 times to the pic, to let it know it is going to be used
			++init_Counter_Control;
			break;
		case 3:
			write_Buffer(0x0C);			// writes 0x0C to say it will be using the radio
			init_Counter_Control = 0;	// resets counter
		}
		for (init_Counter2 = 4; init_Counter2 < 39; ++init_Counter2) {	// fill elements other than control with 0
			write_Buffer(0);
		}
	} */




