/*
 * Packetizer.c
 *
 *  Created on: Sep 1, 2016
 *      Author: AaronEwing
 */
#include <stdint.h>
#include <stdbool.h>
#include "Circular_Buffer.h"

void init_Circular_Buffer(void) {
										// init buffer just does what is natural for the circular buffer
	 init_Buffer();						// (reason for the same thing is so that it is all in one place)
										// please note that max is 1000 bytes
	 void write_Buffer(uint8_t data)
}






