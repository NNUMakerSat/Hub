/*
 * UART.c
 *
 *  Created on: Aug 18, 2016
 *      Author: MKamstra
 */
#include "comm.h"
#include <stdint.h>
#include <stdbool.h>
#define MAXBUFFERSIZE = 10;
uint8_t Buffer[MAXBUFFERSIZE];
bool bufferFullFlg;
bool bufferEmptyFlg;
bool volatile isBufferFull;
bool volatile isBufferEmpty;
uint8_t begin;
uint8_t end;
uint8_t top;
uint8_t bottom;
void UART(void);
uint8_t data = 0xFF;
uint8_t buff_full;
uint8_t buff_empty;

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT2_VECTOR
__interrupt void USCI_A0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_A0_VECTOR))) USCI_A0_ISR (void)
#else
#error Compiler not supported!
#endif
{
	switch (__even_in_range(UCA1IV,4)) {
	case 0:
		break;
	 case 2:                                   // Vector 2 - RXIFG
	    while (!(UCA1IFG & UCTXIFG));             // USCI_A0 TX buffer ready?
	    UCA1TXBUF = UCA1RXBUF;                  // TX -> RXed character
	    	break;
	  case 4:
		  break;                             // Vector 4 - TXIFG
	  default:
		  break;
	}
}

void initBuffer(void){
	begin = 0;
	end = MAXBUFFERSIZE - 1;
	top = 0;
	bottom = 0;
}

void writeBuffer(uint8_t data){
	Buffer[bottom] = data;
	bottom++;
	if(bottom > end){
		bottom = begin;
	}
	if(bottom == top){
		bufferFullFlg = true;
	}
	else{
		bufferFullFlg = false;
	}
}

uint8_t readBuffer(void){
	uint8_t volatile data;
	data = Buffer[top];
	top++;
	if(top > end){
		top = begin;
	}
	if(bottom == top){
		bufferEmptyFlg = true;
	}
	else{
		bufferEmptyFlg = false;
	}
	return(data);
}

bool isBufferFull(void){
	return bufferFullFlg;
}

bool isBufferEmpty(void){
	return bufferEmptyFlg;
}

void UART(){	// Needs to include ability to handle different ports
	if(isBufferFull == 0){
		writeBuffer(data);
}
	if(isBufferEmpty == 0){
		data = readBuffer();
}
}