/*
 * LED.c
 *
 *  Created on: Aug 17, 2016
 *      Author: MKamstra
 */
#include <msp430.h>
#include "LED.h"

void LED(void){
	volatile unsigned int i;	// volatile to prevent optimization

	P1OUT ^= 0x01;				// Toggle P1.0 using exclusive-OR

	i = 10000;					// SW Delay
	do i--;
	while(i != 0);
}



