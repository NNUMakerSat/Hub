/*
 * radio_chk.c
 *
 *  Created on: Aug 17, 2016
 *      Author: MKamstra
 */
#include <msp430.h>
#include "Comm.h"

#define NSL_BUSY 0x02;

int Radio_Chk(void){
	if ((P2IN & NSL_BUSY) == NSL_BUSY)
		return 1;
	else
		return 0;

		// Check if radio is able to take packet of data from HUB
}
