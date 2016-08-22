/*
 * LED.c
 *
 *  Created on: Aug 20, 2016
 *      Author: aaronewing
 */
#include <msp430.h>
#include <stdint.h>
#include "LED.h"

uint32_t g_timer1;						// init timer for LED 1 (so won't have to destroy variable)
uint32_t g_timer2;						// init timer for LED 2 (so won't have to destroy variable)

void init_LED(void) {
	P1DIR |= BIT0;						// Set LED1 as an output
	P9DIR |= BIT7;						// Set LED2 as an output
}

void LED_1_On(void) {
	P1OUT |= BIT0;						// Turn on LED1
}

void LED_2_On(void) {
	P9OUT |= BIT7;						// Turn on LED2
}

void LED_1_Off(void) {
	P1OUT &= ~BIT0;						// Turn off LED1
}

void LED_2_Off(void) {
	P9OUT &= ~BIT7;					// Turn off LED2
}

void flash_LED_1(uint8_t clk_Delay1, uint32_t duration1) {
	for (g_timer1 = 0; g_timer1 < duration1; ++g_timer1) {	// Flash LED however long you want at whatever frequency you want
		P1OUT |= BIT0;
		switch (clk_Delay1){
		case 0:
			__delay_cycles(100000);							// Delay to 160Hz/10Hz (16MHz clk vs. 1MHz clk)
			break;
		case 1:
			__delay_cycles(1000000);						// Delay to 16Hz/1Hz (16MHz clk vs. 1MHz clk)
			break;
		case 2:
			__delay_cycles(10000000);						// Delay to 1.6Hz/0.1Hz (16MHz clk vs. 1MHz clk)
			break;
		case 3:
			__delay_cycles(100000000);						// Delay to 0.16Hz/0.01Hz (16MHz clk vs. 1MHz clk)
			break;
		case 4:
			__delay_cycles(1600000);						// Delay to 10Hz/0.625Hz (16MHz clk vs. 1MHz clk)
			break;
		case 5:
			__delay_cycles(16000000);						// Delay to 1Hz/0.0625Hz (16MHz clk vs. 1MHz clk)
			break;
		case 6:
			__delay_cycles(160000000);						// Delay to 0.1Hz/0.00625Hz (16MHz clk vs. 1MHz clk)
			break;
		case 7:
			__delay_cycles(1600000000);						// Delay to 0.01Hz/0.000625Hz (16MHz clk vs. 1MHz clk)
			break;
		}
		P1OUT &= ~BIT0;
	}
}

void flash_LED_2(uint8_t clk_Delay2, uint32_t duration2) {
	for (g_timer2 = 0; g_timer2 < duration2; ++g_timer2) {	// Flash LED however long you want at whatever frequency you want
		P9OUT |= BIT7;
		switch (clk_Delay2){
		case 0:
			__delay_cycles(100000);							// Delay to 160Hz/10Hz (16MHz clk vs. 1MHz clk)
			break;
		case 1:
			__delay_cycles(1000000);						// Delay to 16Hz/1Hz (16MHz clk vs. 1MHz clk)
			break;
		case 2:
			__delay_cycles(10000000);						// Delay to 1.6Hz/0.1Hz (16MHz clk vs. 1MHz clk)
			break;
		case 3:
			__delay_cycles(100000000);						// Delay to 0.16Hz/0.01Hz (16MHz clk vs. 1MHz clk)
			break;
		case 4:
			__delay_cycles(1600000);						// Delay to 10Hz/0.625Hz (16MHz clk vs. 1MHz clk)
			break;
		case 5:
			__delay_cycles(16000000);						// Delay to 1Hz/0.0625Hz (16MHz clk vs. 1MHz clk)
			break;
		case 6:
			__delay_cycles(160000000);						// Delay to 0.1Hz/0.00625Hz (16MHz clk vs. 1MHz clk)
			break;
		case 7:
			__delay_cycles(1600000000);						// Delay to 0.01Hz/0.000625Hz (16MHz clk vs. 1MHz clk)
			break;
		}
		P9OUT &= ~BIT7;
	}
}
