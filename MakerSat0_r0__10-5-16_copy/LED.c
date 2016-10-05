/*
 * LED.c
 *
 *  Created on: Aug 17, 2016
 *      Author: MKamstra
 */
#include <msp430.h>
#include "LED.h"
#include "init.h"
#include <stdint.h>

void LED(uint8_t BLUE_LED, uint8_t GREEN_LED, uint8_t RED_LED, uint8_t delay_seconds, uint8_t blink){

	for (blink; blink > 0; blink--){
		if (BLUE_LED){P9OUT |= BLUE;}
		if (GREEN_LED){P9OUT |= GREEN;}
		if (RED_LED){P9OUT |= RED;}

		if (delay_seconds == 1){__delay_cycles(1000000);}
		if (delay_seconds == 2){__delay_cycles(2000000);}
		if (delay_seconds == 3){__delay_cycles(3000000);}
		if (delay_seconds == 4){__delay_cycles(4000000);}
		if (delay_seconds == 5){__delay_cycles(5000000);}

		P9OUT = 0x00;
		__delay_cycles(1000000);
	}
}



