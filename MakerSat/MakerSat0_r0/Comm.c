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

//#define hand_Shake_Rec 0xCC;
//#define hand_Shake_Send 0xBB;

uint8_t hand_Shake_Rec = 0xCC;
uint8_t hand_Shake_Send = 0xBB;

bool debounce0 = false;
bool debounce1 = false;
bool state_1_PowerUp = false;
bool state_1_Done = false;

uint8_t hand_Shake_FAIL[3];
uint32_t i = 0;
uint8_t k = 0;
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



void powerUp(uint8_t sci_x_board){		
	P4OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7); // Turn off all boards on HUB
	if (sci_x_board == 1){P4OUT |= BIT4;}
	if (sci_x_board == 2){P4OUT |= BIT5;}
	if (sci_x_board == 3){P4OUT |= BIT6;}
	if (sci_x_board == 4){P4OUT |= BIT7;}
	__delay_cycles(500); // Turn on Science board 1 on HUB
	uint8_t n = 0;
	while (n < 39)
			{
				if (sci_x_board == 1){write_UART (sci_1_pUp[n]);}		// Request power up from NSL
				if (sci_x_board == 2){write_UART (sci_2_pUp[n]);}		// Request power up from NSL
				if (sci_x_board == 3){write_UART (sci_3_pUp[n]);}		// Request power up from NSL
				if (sci_x_board == 4){write_UART (sci_4_pUp[n]);}		// Request power up from NSL
				n++;
			}
			n = 0;
}

void powerDown(uint8_t sci_x_board){
	uint8_t n = 0;
	while (n < 39)
			{
				write_UART (sci_x_pDown[n]);		// Request power up from NSL
				n++;
			}
			n = 0;
	__delay_cycles(500);
	P4OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);	// Turn off all boards on HUB
}

int Sci_Ready(void){
	if (P4IN & BIT1){debounce0 = true;}
	__delay_cycles(50);
	if (P4IN & BIT1){debounce1 = true;}
	if (debounce0 && debounce1){
		debounce0 = false;
		debounce1 = false;
		return 1;
	}
	else return 0;
	}
	
void Start_Timer(void){
	TA0R = 0X0000;
	TA0CCTL0 &= ~CCIFG; // Clears the interrupt flag
	TA0CCR0 = 0xFFFF; 	// wait 1 second
	timer = 0;
	timeout = false; // Clears the flag in the main function
}

uint8_t getTimer(void) {
	return timer;
}

uint8_t hand_Shake(void){
	state_1_PowerUp = false;
	state_1_Done = false;
	read_SPI (hand_Shake_Send);
	if (g_RXData != hand_Shake_Rec)
	{
		hand_Shake_FAIL[m] = g_RXData;
		state_1_PowerUp = true;
		m++;
		//		break;
	}

	if (m > 2)	// Try three times and move on to science board 2 if no handshake
	{
		state_1_Done = true;
		while(o < 3)
		{
			write_UART(hand_Shake_FAIL[o]);
			o++;
		}
	m = 0;
	}
	return 0;
}
bool get_state_1_PowerUp(void) {
	return state_1_PowerUp;
}
bool get_state_1_Done(void) {
	return state_1_Done;
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

void radio_Send(void){
	while (j < 39)
	{
		write_UART (get_Data(j,1));
		j++;
	}
	j = 0;
}

void SCI_Read(void){
	while(g_RXData != 0xFF){
		while (!(Sci_Ready())) {}							// Waits for GPIO to go high
		read_SPI (0xAA);
		write_Buffer(g_RXData);
		i++;
	}
	g_RXData = 0;	// Clear variable for next science board.
	i = 0;
}
