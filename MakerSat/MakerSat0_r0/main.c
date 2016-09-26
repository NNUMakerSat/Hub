/*
 * main.c
 */
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "init.h"
#include "LED.h"
#include "radio_chk.h"
#include "SPI_Polling.h"
#include "UART_Polling.h"
#include "Packetizer.h"
#include "Circular_Buffer.h"
#include "Comm.h"

#define hand_Shake_Rec 0xCC;
#define hand_Shake_Send 0xBB;

bool break_flg = 0;
uint8_t g_RXData;
uint16_t source_ID = 1;

bool IMU_done = 0;
bool sci_1_done = 0;
bool sci_2_done = 0;

uint8_t pin_Setting = 0;				// selects the pins used for 6989
uint8_t device_CS = 0;					// selects the SYNC/SS pin (5k POT)

bool radio_busy = 0;


void IMU_Loop();
void SCI_1_Loop();
void SCI_2_Loop();


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
	timer++;
	if (timer >= 4){timeout = 0x01;timer = 0;}	// wait 5 seconds
}


enum IMU_States { IMU_SMStart, IMU_Init, IMU_Read, IMU_Send, IMU_Done} IMU_State;

void IMU_Loop()
{
	switch(IMU_State){
		case IMU_SMStart:
			IMU_State = IMU_Init;
			break;

		case IMU_Init:
			// Initialize IMU through I2C
			IMU_State = IMU_Read;
			break;
		case IMU_Read:
			// Read IMU
			// Use circular buffer
			IMU_State = IMU_Send;
			break;

		case IMU_Send:
			// Radio data
			IMU_State = IMU_Done;
			break;

		case IMU_Done:
			clear_CircularBuffer();
			P9OUT = BIT5;
			__delay_cycles(2000000);
			P9OUT = 0x00;
			IMU_done = 1;
			break;
	}
}


enum SCI_1_States { SCI_1_SMStart, SCI_1_PowerUp, SCI_1_HandShake, SCI_1_Read, SCI_1_PowerDown, SCI_1_Send, SCI_1_Done} SCI_1_State;

void SCI_1_Loop()
{
	switch(SCI_1_State){
		case SCI_1_SMStart:
			SCI_1_State = SCI_1_PowerUp;
			break;

		case SCI_1_PowerUp:
			powerUp(1);	// Power up science board 1
			SCI_1_State = SCI_1_HandShake;
			break;
			
		case SCI_1_HandShake:
			Start_Timer();

			while(!timeout && !Sci_Ready()){}
// Alive check
			if(timeout){
				powerDown(1);
				SCI_1_State = SCI_1_PowerUp;
				break_flg = 1;
			}
			if (break_flg){break_flg = 0;break;}
// Handshake
//			hand_Shake();
//			if (get_state_1_PowerUp()){g_RXData = 0x00;SCI_1_State = SCI_1_PowerUp;break;}
//			else if(get_state_1_Done()){g_RXData = 0x00;SCI_1_State = SCI_1_Done;break;}

			SCI_1_State = SCI_1_Read;
			break;
			
		case SCI_1_Read:
			SCI_Read();
			Packetizer(1, 0);
			SCI_1_State = SCI_1_PowerDown;
			break;

		case SCI_1_PowerDown:
			powerDown(1);	// Power up science board 1
			SCI_1_State = SCI_1_Send;
			break;
			
		case SCI_1_Send:
			radio_Send();	// Send whats in circular buffer
			SCI_1_State = SCI_1_Done;
			break;

		case SCI_1_Done:
			clear_CircularBuffer();
			P9OUT = BIT6;
			__delay_cycles(2000000);
			P9OUT = 0x00;
			sci_1_done = 1;
			break;
	}
}

enum SCI_2_States { SCI_2_SMStart, SCI_2_PowerUp, SCI_2_HandShake, SCI_2_Read, SCI_2_PowerDown, SCI_2_Send, SCI_2_Done} SCI_2_State;

void SCI_2_Loop()
{
	switch(SCI_2_State){
		case SCI_2_SMStart:
			SCI_2_State = SCI_2_PowerUp;
			break;

		case SCI_2_PowerUp:
			powerUp(2);	// Power up science board 2
			SCI_2_State = SCI_2_HandShake;
			break;
			
		case SCI_2_HandShake:
			Start_Timer();

			while(!timeout && !Sci_Ready()){}
// Alive check
			if(timeout){
				powerDown(2);
				SCI_2_State = SCI_2_PowerUp;
				break_flg = 1;
			}
			if (break_flg){break_flg = 0;break;}
// Handshake
//			hand_Shake();
//			if (get_state_1_PowerUp()){g_RXData = 0x00;SCI_1_State = SCI_1_PowerUp;break;}
//			else if(get_state_1_Done()){g_RXData = 0x00;SCI_1_State = SCI_1_Done;break;}

			break_flg = 0;
			SCI_2_State = SCI_2_Read;
			break;
			
		case SCI_2_Read:
			while(g_RXData != 0xFF){
					while (!(Sci_Ready())) {}							// Waits for GPIO to go high
					read_SPI (0xAA);
					write_Buffer(g_RXData);
			}
			Packetizer(1, 0);
			SCI_2_State = SCI_2_PowerDown;
			break;
			
		case SCI_2_PowerDown:
			powerDown(2);	// Power up science board 1
			SCI_2_State = SCI_2_Send;
			break;

		case SCI_2_Send:
			radio_Send();	// Send what is in circular buffer
			SCI_2_State = SCI_2_Done;
			break;

		case SCI_2_Done:
			clear_CircularBuffer();
			P9OUT = BIT7;
			__delay_cycles(2000000);
			P9OUT = 0x00;
			sci_2_done = 1;
			break;
	}
}

int main(void) {
	WDTCTL = WDTPW | WDTHOLD;					// Stop watchdog timer

// Initializations
	initialize_Ports();						// Init all non used ports
	initialize_Clocks();					// Sets up timers (takes care of FRAM issue)
	__delay_cycles(500);
	Init_LED();
	Init_Timer();
	Init_Comm();
	Init_ADC();
	Init_Var();

	init_SPI (pin_Setting);
	init_UART (1, 0);

	SCI_1_State = SCI_1_SMStart;
	SCI_2_State = SCI_2_SMStart;
	__enable_interrupt();
	while(1) {

		if (!IMU_done)
			{
				IMU_Loop();
			}
		if (IMU_done & !sci_1_done)
			{
			SCI_1_Loop();
			}
		if (IMU_done & sci_1_done & !sci_2_done)
		{
			SCI_2_Loop();
		}
	}
	return 0;
}
