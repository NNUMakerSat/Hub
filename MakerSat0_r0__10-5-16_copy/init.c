/*
 * init.c
 *
 *  Created on: Aug 17, 2016
 *      Author: MKamstra
 */
#include <msp430.h>
#include "init.h"


#define NSL_BUSY 0x02


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
void Init_Port(void){
	// ISR
	// Inputs
	// Outputs
	// Science Board selection
	P4DIR |= SCI_1_SEL | SCI_2_SEL | SCI_3_SEL | SCI_4_SEL;
	P4OUT = 0x00;
}
void Init_IMU(void){
	init_i2c();
	__delay_cycles(800);
	initGyro();
	__delay_cycles(800);
	initXM();
	__delay_cycles(800);
}

