/*
 * i2c.c
 *
 *  Created on: Sep 24, 2016
 *      Author: bgrim
 */
#include <msp430fr6989.h>
#include <stdint.h>
#include "i2c.h"
//#include "LSM9DS0.h"

char single_byte_read_i2c(uint8_t slaveAddress, uint8_t registerAddress) {
	UCB1I2CSA = slaveAddress;
	uint8_t data;
	data = 0;

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
	UCB1CTL1 |= UCTR + UCTXSTT;               	// I2C TX with start condition
	while ((UCB1CTL1 & UCTXSTT) || !(UCB1IFG & UCTXIFG0));
	UCB1TXBUF = registerAddress;
	while (!(UCB1IFG & UCTXIFG0));
	UCB1CTL1 &= ~UCTR;                     			// Clear I2C TX flag
	UCB1CTL1 |= UCTXSTT;
	while (UCB1CTL1 & UCTXSTT);                     			// I2C start condition
	UCB1CTL1 |= UCTXSTP;
	while (!(UCB1IFG & UCRXIFG0));

	data = UCB1RXBUF;

	return data;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void single_byte_write_i2c(uint8_t slaveAddress, uint8_t registerAddress, uint8_t setBits) {
	UCB1I2CSA = slaveAddress;

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
	UCB1CTL1 |= UCTR + UCTXSTT;               	// I2C TX with start condition
	while ((UCB1CTL1 & UCTXSTT) || !(UCB1IFG & UCTXIFG0));
	UCB1TXBUF = registerAddress;
	//while ((UCB1CTL1 & UCTXSTT) || !(UCB1IFG & UCTXIFG0));
	while((UCB1IFG & UCTXIFG0) == 0);
	UCB1TXBUF = setBits;                      			// Clear I2C TX flag
	while((UCB1IFG & UCTXIFG0) == 0);
	UCB1CTL1 |= UCTXSTP;
	UCB1IFG &= ~UCTXIFG0;
}

void init_i2c(void) {
	P3SEL0 |= BIT1 + BIT2;     							// Assign I2C pins to USCI_B1
	P3SEL0 &= ~(BIT0 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7);
	P3SEL1 |= BIT0 + BIT3 + BIT4 + BIT5 + BIT6 + BIT7;
	P3SEL1 &= ~(BIT1 + BIT2);     						// Assign I2C pins to USCI_B1
	UCB1CTLW0 |= UCSWRST;                      			// Enable SW reset
	UCB1CTLW0 |= UCMST + UCMODE_3 + UCSYNC + UCSSEL_2; // I2C Master, synchronous mode
	UCB1BRW = 80;                             		// fSCL = 1Mhz/100 = ~100kHz
	UCB1CTLW0 &= ~UCSWRST;                 	// **Initialize USCI state machine**
}
