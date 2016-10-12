/*
 * Initialize.c
 *
 *  Created on: Aug 0xFFFF0xFFFF, 200xFFFF6
 *      Author: aaronewing
 */
// contains all functions for Initializing MSP430

#include <msp430fr6989.h>
#include "Initialize.h"

void initialize_Clocks(void) {								// Sets all clocks to standard position

    PJSEL0 |= BIT4 | BIT6;	     // LFXT (BIT4) HFXT (BIT6)
    PJSEL1 &= ~(BIT4 + BIT6);      // LFXT (BIT4) HFXT (BIT6)

    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL2 = SELA__LFXTCLK | SELS__HFXTCLK | SELM__HFXTCLK;	// ACLK-LFXT, SMCLK-HFXT, MCLK-HFXT
	CSCTL3 = DIVA_0 | DIVS__8 | DIVM__8;     // set all dividers

    CSCTL4 = HFFREQ_1;
    CSCTL4 &= ~(HFXTOFF + LFXTOFF);

	do
	{
		CSCTL5 &= ~(LFXTOFFG + HFXTOFFG);                    // Clear LFXT fault flag
		SFRIFG1 &= ~OFIFG;
	}
	while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
		  CSCTL0_H = 0;                             // Lock CS registers

    CSCTL0_H = 0;
}

void initialize_Ports(void){			// sets all pins on all ports as an output (except Port 10)
	PM5CTL0 &= ~LOCKLPM5;

	P1DIR |= 0xFFFF;
	P2DIR |= 0xFFFF;
	P3DIR |= 0xFFFF;
//	P4DIR |= 0xFFFF;
	P5DIR |= 0xFFFF;
	P6DIR |= 0xFFFF;
	P7DIR |= 0xFFFF;
	P8DIR |= 0xFFFF;
	P9DIR |= 0xFFFF;
	P10DIR |= 0xFFFF;					// Pins 0, 1, and 2 are the only ones to exist on Port 10

	P1OUT = 0x000;						// sets all pins on all ports to a low output (redundant)
	P2OUT = 0x000;
	P3OUT = 0x000;
//	P4OUT = 0x000;
	P5OUT = 0x000;
	P6OUT = 0x000;
	P7OUT = 0x000;
	P8OUT = 0x000;
	P9OUT = 0x000;
	P10OUT = 0x000;
}
