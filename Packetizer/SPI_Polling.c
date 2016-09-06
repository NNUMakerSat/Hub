/*
 * SPI_Pulling.c
 *
 *  Created on: Aug 16, 2016
 *      Author: aaronewing
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t g_RXData;



////////////////////// SPI INIT /////////////////////////////////////
void init_SPI (uint8_t pin_Setting) {
	switch (pin_Setting) {
	case 0:								// Hub
	default:
		// Configure Primary Function Pins
		P1SEL0 |= BIT4 | BIT6 | BIT7;       		// P1.4 - CLK, P1.6 - SIMO, P1.7 - SOMI
		P1SEL1 &= ~(BIT4 & BIT6 & BIT7);

		// configure as GPIO used to enable SPI write from Hub

		P4SEL0 &= ~(BIT1 + BIT4 + BIT5 + BIT6 + BIT7);
		P4SEL1 &= ~(BIT1 + BIT4 + BIT5 + BIT6 + BIT7);							// P4.1 slave data ready line
		P4DIR &= ~BIT1;
		P4DIR |= BIT4 + BIT5 + BIT6 + BIT7;
		P4OUT |= BIT6;
		P4OUT &= ~(BIT4 + BIT7 + BIT5);






		break;

	case 1:								// MSP430FR5969
		// Configure Primary Function Pins
		P1SEL0 |= BIT6 | BIT7;              // P1.6 - SIMO, P1.7 - SOMI
		P2SEL0 |= BIT2;              		// P2.2 - CLK

		// configure as GPIO used to enable SPI write to Hub
		P4SEL0 &= ~BIT1;
		P4SEL1 &= ~BIT1;					// P4.1 - SYNC/Slave Select
		P4DIR |= BIT1;
		P4OUT |= BIT1;
		break;

	case 2:								// Polymer degradation board - Pot
		// Configure Primary Function Pins
		P1SEL0 |= BIT6 | BIT7;              // P1.6 - SIMO, P1.7 - SOMI
		P2SEL0 |= BIT2;              		// P2.2 - CLK

		// configure as GPIO used to enable SPI write to Hub
		P1SEL0 &= ~BIT1;
		P1SEL1 &= ~BIT1;					// P1.1 - SYNC/Slave Select
		P1DIR |= BIT1;
		P1OUT |= BIT1;
		break;

	case 3:								// Polymer degradation board - other
		// Configure Primary Function Pins
		P1SEL0 |= BIT6 | BIT7;              // P1.6 - SIMO, P1.7 - SOMI
		P2SEL0 |= BIT2;              		// P2.2 - CLK

		// configure as GPIO used to enable SPI write to Hub
		P1SEL0 &= ~BIT2;
		P1SEL1 &= ~BIT2;					// P1.2 - SYNC/Slave Select
		P1DIR |= BIT2;
		P1OUT |= BIT2;
		break;
	}

	 // Configure USCI_B0 for SPI operation
	 UCB0CTLW0 |= UCSWRST;                      		// **Put state machine in reset**

	 UCB0CTLW0 |= UCMST | UCSYNC | UCMSB | UCCKPL;		// 3-pin, 8-bit SPI master
	                                          	  	    // Clock polarity high, MSB
	 UCB0CTLW0 |= UCSSEL__SMCLK;              	 		// SMCLK

	 UCB0BRW = 0x0008;							// Divides SMCLK module by 8 (8MHz/8 = 1MHz)

	 UCA0MCTLW = 0;                           	 	// No modulation
	 UCB0CTLW0 &= ~UCSWRST;                   	 	// **Initialize USCI state machine**


}

////////////////////// SPI WRITE 8 BIT /////////////////////////////////////
void write_uint8_SPI (uint8_t tx_Data_8, uint8_t device_CS) {
	while (!(UCB0IFG & UCTXIFG)){};							// If able to TX

	switch (device_CS) {					// Hub
		case 0:
		default:
//			P4OUT &= ~BIT1;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};				// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};					// While not busy
//			P4OUT |= BIT1;
			break;

		case 1:
			P2OUT &= ~BIT8;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};				// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P2OUT |= BIT6;
			break;

		case 2:
			P3OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P3OUT |= BIT6;
			break;

		case 3:
			P4OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P4OUT |= BIT6;
			break;
	}
}

////////////////////// SPI WRITE 16 BIT /////////////////////////////////////
void write_uint16_SPI (uint16_t tx_Data_16, uint8_t device_CS) {
	while (!(UCB0IFG & UCTXIFG)){};							// If able to TX

	switch (device_CS) {
		case 0:									// Hub
		default:
			P4OUT &= ~BIT1;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P4OUT |= BIT1;
			break;

		case 1:
			P2OUT &= ~BIT8;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P2OUT |= BIT6;
			break;

		case 2:
			P3OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P3OUT |= BIT6;
			break;

		case 3:
			P4OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P4OUT |= BIT6;
			break;
	}
}

////////////////////// SPI READ POLLING //////////////////////////////////
uint16_t read_SPI (void) {

//		P4OUT &= ~BIT1;									// Pulls SYNC low
		while (!(UCB0IFG & UCTXIFG)) {};				// While TXing
		UCB0TXBUF = 0xAA;								// Transmits read_byte
		while (UCB0STATW & UCBUSY) {};					// While not busy
//		P4OUT |= BIT1;
	while (!(UCB0IFG & UCRXIFG)) {};    				// While RX flag is high
	g_RXData = UCB0RXBUF;								// First 8 bits transmitted (Control bits and data)
	return g_RXData;

}
