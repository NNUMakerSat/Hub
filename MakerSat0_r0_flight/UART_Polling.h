/*
 * UART_Polling.h
 *
 *  Created on: Aug 18, 2016
 *      Author: aaronewing
 */
#ifndef UART_POLLING_H_
#define UART_POLLING_H_

void init_UART (bool baud_Rate, bool pin_Setting);	// initalizes UART clk rate and which pins are being used
// baud_Rate = 0 - 9600, 1 - 38400 (default), pin_Setting (0 - 2.0 TX, 2.1 RX, 2.2 BUSY (default), 1 - testing)
void write_UART (uint8_t TX_Data);					// writes 8 bits with SPI
uint8_t read_UART (void);
uint8_t TX_Data;

#endif /* UART_POLLING_H_ */
