/*
 * init.c
 *
 *  Created on: Aug 17, 2016
 *      Author: MKamstra
 */
#include <msp430.h>
#include "init.h"

#define LED_0 0x01;
#define LED_1 0x02;
#define LED_2 0x04;
#define NSL_BUSY 0x02;
#define SCI_1_SEL 0x10;
#define SCI_2_SEL 0x20;
#define SCI_3_SEL 0x40;
#define SCI_4_SEL 0x80;


// LEDS
void Init_LED(void){
	P1DIR |= LED_0 + LED_1 + LED_2;	                            // Set P1.0 to output direction
	P1IN = 0x00;
	P1OUT = 0x00;
}
// TIMERS
// Set up timer A0
void Init_Timer(void){
	TA0CCR0 = 0x0000;
	TA0R = 0X0000;
	TA0CCTL0 |= CCIE; // no capture | compare mode
	TA0CTL |= (TASSEL_1 + MC_1 + TACLR); // ACLK | /1 | UP mode | clear

}
// PORTS
void Init_Port(void){
	// ISR
	// Inputs
	// Outputs
	// Science Board selection
	P4DIR |= SCI_1_SEL + SCI_2_SEL + SCI_3_SEL + SCI_4_SEL;
	P4OUT = 0x00;
}

// COMMUNICATION
void Init_Comm(void){
// Radio
	P2DIR |= NSL_BUSY;	// Set P2.2
	P2IN = 0x00;
	P2OUT = 0x00;
/*	// I2C
	// UART
	UCA1CTL1 = 0x41; // Includes software enable
    UCA1CTL0 = 0x00;
    UCA1BR0 = 0x5D; // set to decimal 109 for 9600 Baud Rate
    UCA1BR1 = 0x5D;
    UCA1MCTL = 0x00;
    UCA1STAT = 0x80; // set loop back mode
    UCA1RXBUF = 0x00; // initialize the read buffer
	UCA1TXBUF = 0x00; // initialize the transmit buffer
	//UCA1IRCTL maybe later?
	UCA1ABCTL = 0x00; // auto baud rate not being used
	UCA1CTL1 = 0x40; // Includes software disable
	UCA1IE = 0x03; // enable interrupt for both TX and RX
	UCA1IV = 0x04; // highest priority
	// SPI
}
// ADC
 */
}
void Init_ADC(void){
	// 12 Bit ADC
}
// VARIABLES
void Init_Var(void){
	// Random Variables
}



