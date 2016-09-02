/*
 * Comm.h
 *
 *  Created on: Aug 18, 2016
 *      Author: MKamstra
 */
#include <stdint.h>
#ifndef COMM_H_
#define COMM_H_

int I2C(uint16_t foo1, uint16_t foo2);	// Destiniation info as perameters, defined in HUB_perif.h

// UART
//int UART(uint16_t foo1, uint16_t foo2); // Destiniation info as perameters, defined in HUB_perif.h
uint8_t readBuffer(void);
void writeBuffer(uint8_t data);


int SPI(uint16_t foo1, uint16_t foo2);  // Destiniation info as perameters, defined in HUB_perif.h



int Radio_Chk(void);

#endif /* COMM_H_ */
