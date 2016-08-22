/*
 * SPI_Pulling.c
 *
 *  Created on: Aug 16, 2016
 *      Author: aaronewing
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>

#define UART_RADIO_BUSY 0x10       	// P1.4

uint8_t RX_Data;					// basicly the radio ACK

////////////////////// SPI INIT /////////////////////////////////////
void init_UART (bool baud_Rate, bool pin_Setting) {
	switch (pin_Setting) {
	case 0:
		// Configure Secondary Function Pins
		P2SEL0 |= BIT0 | BIT1;					// P2.0 - TX, P2.1 - RX
		P2SEL1 &= ~(BIT0 | BIT1);

		P1SEL0 &= ~UART_RADIO_BUSY;
		P1SEL1 &= ~UART_RADIO_BUSY;				// P1.4 - Radio Busy line
		P1DIR &= ~UART_RADIO_BUSY;
		P1IN &= ~UART_RADIO_BUSY;
		break;

	case 1:
		// Configure Secondary Function Pins
		P3SEL0 |= BIT4 | BIT5;                    // P3.4 - TX, P3.5 - RX
		P4SEL1 &= ~(BIT4 | BIT5);

		P1SEL0 &= ~UART_RADIO_BUSY;
		P1SEL1 &= ~UART_RADIO_BUSY;				// P1.4 - Radio Busy line
		P1DIR &= ~UART_RADIO_BUSY;
		P1IN &= ~UART_RADIO_BUSY;
		break;

	default:
		// Configure Secondary Function Pins
		P2SEL0 |= BIT0 | BIT1;                    // P2.0 - TX, P2.1 - RX
		P2SEL1 &= ~(BIT0 | BIT1);

		P1SEL0 &= ~UART_RADIO_BUSY;
		P1SEL1 &= ~UART_RADIO_BUSY;				// P1.4 - Radio Busy line
		P1DIR &= ~UART_RADIO_BUSY;
		P1IN &= ~UART_RADIO_BUSY;
		break;
}

	 // XT1 Setup
	CSCTL0_H = CSKEY >> 8;                		// Unlock CS registers
	CSCTL1 = DCOFSEL_0;							// Set DCO to 1MHz
	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	CSCTL0_H = 0;                             	// Lock CS registers

	// Configure USCI_A0 for SPI operation
	UCA0CTL1 |= UCSWRST;                      		// **Put state machine in reset**

	switch (baud_Rate) {
	case 0:
				// Configure Timer for 9600 Baud
		UCA0CTL1 = UCSSEL__ACLK;                  // Set ACLK = 32768 as UCBRCLK
		UCA0BR0 = 3;                              // 9600 baud
		UCA0MCTLW |= 0x5300;                      // 32768/9600 - INT(32768/9600)=0.41
		                                          // UCBRSx value = 0x53 (See UG)
		UCA0BR1 = 0;
	 	break;

	case 1:
				// Configure Timer for 38400 Baud
		UCA0CTL1 = UCSSEL__SMCLK;                 	// Set SMCLK = 1000000 as UCBRCLK
		UCA0BR0 = 0x1A;								// 9600 baud
		UCA0MCTLW |= 0x0100;                 // 1000000/38400 - INT(1000000/38400)=0.04
		                                          	// UCBRSx value = 0x01 (See UG)
											// N = 0.0529, effectively 38,383.4 Baud
		UCA0BR1 = 0;
	 	break;

	default:
					// Configure Timer for 9600 Baud
		UCA0CTL1 = UCSSEL__ACLK;                  // Set ACLK = 32768 as UCBRCLK
		UCA0BR0 = 3;                              // 9600 baud
		UCA0MCTLW |= 0x5300;                      // 32768/9600 - INT(32768/9600)=0.41
		                                          // UCBRSx value = 0x53 (See UG)
		UCA0BR1 = 0;
		break;
	}
	UCA0CTL1 &= ~UCSWRST;                    	 // release from reset                   	 	// **Initialize USCI state machine**
}

////////////////////// UART WRITE POLLING /////////////////////////////////////
void write_UART (uint8_t TX_Data) {
	while (!(UCA0IFG & UCTXIFG)){};					// If able to TX
	while (P1IN == UART_RADIO_BUSY) {};				// If Radio is not busy
	UCA0TXBUF = TX_Data;							// 8 bits transmitted
}

////////////////////// UART READ POLLING //////////////////////////////////
uint8_t read_UART (void) {
	while (!(UCA0IFG & UCRXIFG)) {};    			// While RX flag is high
	RX_Data = UCA0RXBUF;							// Recieve Radio ACK
	return RX_Data;
}

////////////////////// LEGAL /////////////////////////////////////
/* XT1 config section:
 *
 *
 * --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
