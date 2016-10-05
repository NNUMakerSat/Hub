/*
 * Comm.c
 *
 *  Created on: Sep 16, 2016
 *      Author: mkamstra
 */
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "Comm.h"
#include "SPI_Polling.h"
#include "init.h"
#include "LED.h"

uint8_t radio_Busy = 0x00;//BIT2;
uint8_t hand_Shake_Rec = 0xCC;
uint8_t hand_Shake_Send = 0xBB;


bool hs_Fail = false;
bool debounce1 = false;
bool state_x_PowerUp = false;
bool state_x_Send = false;

uint8_t MOCK0_write_UART[150];
uint8_t MOCK1_write_UART[100];
uint8_t MOCK2_write_UART[100];
uint8_t hand_Shake_FAIL[3];
uint32_t i = 0;
uint16_t k = 0;
uint8_t j = 0;
uint16_t m = 0;
uint8_t n = 0;
uint8_t o = 0;
uint8_t g_RXData;
uint16_t g_bottom;
uint16_t g_top;
bool g_bufferEmpty;



uint8_t sci_1_pUp[39] = { 0x50, 0x50, 0x50, 0x0B,   // Science board 1 ON
						  0x01, 0xFF, 0x02, 0x00,
						  0x03, 0x00, 0x04, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00 };

uint8_t sci_2_pUp[39] = { 0x50, 0x50, 0x50, 0x0B,   // Science board 2 ON
		 	 	 	 	  0x01, 0x00, 0x02, 0xFF,
		 	 	 	 	  0x03, 0x00, 0x04, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00 };

uint8_t sci_3_pUp[39] = { 0x50, 0x50, 0x50, 0x0B,   // Science board 3 ON
						  0x01, 0x00, 0x02, 0x00,
						  0x03, 0xFF, 0x04, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00 };

uint8_t sci_4_pUp[39] = { 0x50, 0x50, 0x50, 0x0B,   // Science board 4 ON
						  0x01, 0x00, 0x02, 0x00,
						  0x03, 0x00, 0x04, 0xFF,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00 };

uint8_t sci_x_pDown[39] = { 0x50, 0x50, 0x50, 0x0B,   // Science board X OFF
		  	  	  	  	  0x01, 0x00, 0x02, 0x00,
						  0x03, 0x00, 0x04, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00, 0x00,
						  0x00, 0x00, 0x00 };


uint8_t hand_Shake_Fail[39] = { 0x50, 0x50, 0x50, 0x0C,   // Science board X OFF
						     0x99, 0x99, 0x99, 0x88,
						     0x00, 0x00, 0x00, 0x00,
							 0x00, 0x00, 0x00, 0x00,
							 0x00, 0x00, 0x00, 0x00,
							 0x00, 0x00, 0x00, 0x00,
							 0x00, 0x00, 0x00, 0x00,
							 0x00, 0x00, 0x00, 0x00,
							 0x00, 0x00, 0x00, 0x00,
							 0x00, 0x00, 0x00 };
void power_Up(uint8_t sci_x_board){
	P4OUT &= ~(SCI_1_SEL + SCI_2_SEL + SCI_3_SEL + SCI_4_SEL); // Turn off all boards on HUB
	uint8_t n = 0;
	while (P2IN & radio_Busy){}
//	__delay_cycles(1000000);	// Wait 1 seconds to let settle
	while (n < 39)
			{
				if (sci_x_board == 1){write_UART (sci_1_pUp[n]);}		// Request power up from NSL
				if (sci_x_board == 2){write_UART (sci_2_pUp[n]);}		// Request power up from NSL
				if (sci_x_board == 3){write_UART (sci_3_pUp[n]);}		// Request power up from NSL
				if (sci_x_board == 4){write_UART (sci_4_pUp[n]);}		// Request power up from NSL
				n++;
			}
			n = 0;
			__delay_cycles(1000000); // Turn on Science board 1 on HUB
			if (sci_x_board == 1){P4OUT |= SCI_1_SEL;}
			if (sci_x_board == 2){P4OUT |= SCI_2_SEL;}
			if (sci_x_board == 3){P4OUT |= SCI_3_SEL;}
			if (sci_x_board == 4){P4OUT |= SCI_4_SEL;}
}

void power_Down(uint8_t sci_x_board){
	P4OUT &= ~(SCI_1_SEL + SCI_2_SEL + SCI_3_SEL + SCI_4_SEL);	// Turn off all boards on HUB
	__delay_cycles(1000000);
	uint8_t n = 0;
	while (P2IN & radio_Busy){}
	while (n < 39)
	{
		write_UART (sci_x_pDown[n]);		// Request power up from NSL
		n++;
	}
	g_RXData = 0;
	n = 0;
}

int Sci_Ready(void){
	if (P4IN & BIT1){
		return 1;
	}
	else{
		return 0;
	}
}
	
void Start_Timer(void){
	TA0CCR0 = 0x8000; 	// Count up to 1 second.
	TA0R = 0x0000;
	TA0CCTL0 &= ~CCIFG; // Clears the interrupt flag
	timer = 0;
	timeout = false; // Clears the flag in the main function

}
uint8_t getTimer(void) {
	return timer;
}

uint8_t master_HandShake(uint8_t sci_x){
	state_x_Send = false;
	state_x_PowerUp = false;
	while (!timeout && !Sci_Ready()){}
	if (!timeout){
		read_SPI (hand_Shake_Send);
		if (g_RXData != hand_Shake_Rec){	// handshake fail
			LED(1,1,1,5,1);	// Handshake fail (BLUE GREEN RED)
			hand_Shake_FAIL[(m + 9)] = g_RXData;
			m++;
			state_x_PowerUp = true;
		}

	}
	if (m > 20)	// Try three times and move on to science board 2 if no handshake
	{
		state_x_Send = true;
		for (o = 9; o < 29; o++){
		write_Buffer(hand_Shake_FAIL[o]);
	}
}
	return 0;
}
bool get_state_x_PowerUp(void) {
	return state_x_PowerUp;
}
bool get_state_x_Send(void) {
	return state_x_Send;
}

void clear_CircularBuffer(void){
	for (k = 500; k > 0; k--)
	{
		write_Buffer(0x00);		// Clear circular buffer
	}
	g_bottom = 0;				// Set circular buffer back to zero
	g_top = 0;					// Set circular buffer back to zero
	g_bufferEmpty = false;		// Clear the g_bufferEmpty flag so that the next science board can store data
}

void radio_Send(uint8_t source_ID){
	uint8_t l = 0;
	while (!g_bufferEmpty && !timeout){
		Packetizer(source_ID, 0);
		while ((P2IN & radio_Busy) && !timeout){}
		while ((j < 39) && !timeout)
		{
			write_UART (get_Data(j,source_ID));
			if (source_ID == 0){MOCK0_write_UART[j + (39 * l)] = get_Data(j,source_ID);}
			if (source_ID == 1){MOCK1_write_UART[j + (39 * l)] = get_Data(j,source_ID);}
			if (source_ID == 2){MOCK2_write_UART[j + (39 * l)] = get_Data(j,source_ID);}
			j++;
		}
		j = 0;
		l++;
	}
	l = 0;
}

void SCI_Read(void){

	while(Sci_Ready()){							// Waits for GPIO to go high
		read_SPI (0xAA);
		write_Buffer(g_RXData);
		i++;
	}
	g_RXData = 0;	// Clear variable for next science board.
	i = 0;
}
