/*
 * UART_Polling.h
 *
 *  Created on: Aug 18, 2016
 *      Author: aaronewing
 */
#ifndef UART_POLLING_H_
#define UART_POLLING_H_

void init_UART (bool baud_Rate, bool pin_Setting);	// initalizes UART clk rate and which pins are being used
void write_UART (uint8_t TX_Data);					// writes 8 bits with SPI
uint8_t read_UART (void);

#endif /* UART_POLLING_H_ */
