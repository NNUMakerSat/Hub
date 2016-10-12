/*
 * init.c
 *
 *  Created on: Aug 17, 2016
 *      Author: MKamstra
 */
#include <msp430.h>
#include "init.h"
#include <stdint.h>


#define NSL_BUSY 0x02


void Init_Clocks(void){
	
    PJSEL0 |= BIT4 | BIT6;	     // LFXT (BIT4) HFXT (BIT6)
    PJSEL1 &= ~(BIT4 + BIT6);      // LFXT (BIT4) HFXT (BIT6)

    CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
    CSCTL2 = SELA__LFXTCLK | SELS__HFXTCLK | SELM__HFXTCLK;	// ACLK-LFXT, SMCLK-HFXT, MCLK-HFXT
	CSCTL3 = DIVA__1 | DIVS__8 | DIVM__8;     // set all dividers

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
// LEDS
void Init_LED(void){
	P9DIR |= RED | GREEN | BLUE;	                            // Set P1.0 to output direction
	P9IN = 0x00;
	P9OUT = 0x00;
}
// TIMERS
// Set up timer A0
void Init_Timer(void){
// Timer A0
	TA0CCR0 = 0x0000;
	TA0R = 0x0000;
	TA0CCTL0 |= CCIE; // no capture | compare mode
	TA0CTL |= (TASSEL_1 + MC_1 + TACLR); // ACLK | /1 | UP mode | clear

// Timer A1
//	TA1CCR0 = 0x0000;
//	TA1R = 0x0000;
//	TA1CCTL0 |= CCIE; // no capture | compare mode
//	TA1CTL |= (TASSEL_1 + MC_1 + TACLR); // ACLK | /1 | UP mode | clear
}
// PORTS
void Init_Ports(void){
	// Inputs
	// Outputs
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
	// Science Board selection
	P4DIR |= SCI_1_SEL | SCI_2_SEL | SCI_3_SEL | SCI_4_SEL;
	P4OUT = 0x00;
}

uint8_t Init_IMU(void){
	init_i2c();
	__delay_cycles(800);
	if (initGyro()){return 1;}
	__delay_cycles(800);
	if (initXM()){return 1;}
	__delay_cycles(800);
return 0;
}

