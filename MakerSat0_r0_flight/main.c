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
///////////////////////
#include "LSM9DS0.h"///
#include "i2c.h"///////
uint8_t IMU_test_Array[80] = {0xFF, 0xFF, 0x00, 0x00, 0x11, 0x11, 0x22, 0x22, 0x33, 0x33, 0x44, 0x44, 0x55, 0x55, 0x66, 0x66, 0x77, 0x77, 0x88, 0x88,0xFF, 0xFF, 0x00, 0x00, 0x11, 0x11, 0x22, 0x22, 0x33, 0x33, 0x44, 0x44, 0x55, 0x55, 0x66, 0x66, 0x77, 0x77, 0x88, 0x88,0xFF, 0xFF, 0x00, 0x00, 0x11, 0x11, 0x22, 0x22, 0x33, 0x33, 0x44, 0x44, 0x55, 0x55, 0x66, 0x66, 0x77, 0x77, 0x88, 0x88,0xFF, 0xFF, 0x00, 0x00, 0x11, 0x11, 0x22, 0x22, 0x33, 0x33, 0x44, 0x44, 0x55, 0x55, 0x66, 0x66, 0x77, 0x77, 0x88, 0x88};
uint8_t jj;
///////////////////////
#define hand_Shake_Rec 0xCC;
#define hand_Shake_Send 0xBB;


uint8_t g_RXData;
uint16_t source_ID = 1;
bool IMU_mode = false;
bool sci_1_mode = false;
bool sci_2_mode = false;
bool IMU_done = false;
bool sci_1_done = false;
bool sci_2_done = false;
uint8_t location = 0;
uint8_t pin_Setting = 0;				// selects the pins used for 6989
uint8_t device_CS = 0;					// selects the SYNC/SS pin (5k POT)

bool radio_busy = false;


void IMU_Loop();
void SCI_1_Loop();
void SCI_2_Loop();


#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
	timer++;
	if ((timer >= 90) && (IMU_mode || sci_2_mode)) {	// 1 minute For IMU and science board 2 (Caldwell HS)
		timeout = true;
		timer = 0;
		LED(1,0,1,1,3); // BLUE RED flash 3 times
		TA0CCR0 = 0x0000;	// clear timer
		TA0R = 0x0000;
	}
	if ((timer >= 600) && sci_1_mode) {	// 5 minutes for science board 1 (poly)
		timeout = true;
		timer = 0;
		LED(1,0,1,1,3); // BLUE RED flash 3 times
		TA0CCR0 = 0x0000;	// clear timer
		TA0R = 0x0000;
}
}

enum IMU_States { IMU_SMStart, IMU_Init, IMU_Read, IMU_Send, IMU_FAIL, IMU_Done} IMU_State;

void IMU_Loop()
{
	switch(IMU_State){
		case IMU_SMStart:
			IMU_mode = true;
			Start_Timer();
			IMU_State = IMU_Init;
			break;

		case IMU_Init:
			if(Init_IMU()){timeout = false;location = 0x01;IMU_State = IMU_FAIL;break;}	// In case I2C fails and gets stuck
			IMU_State = IMU_Read;
			break;
		case IMU_Read:
			if(run_IMU()){timeout = false;location = 0x02;IMU_State = IMU_FAIL;break;}	// In case I2C fails and gets stuck
//			for (jj = 0; jj <= 80; jj++){
//			write_Buffer(IMU_test_Array[jj]);
//			}
			IMU_State = IMU_Send;
			break;

		case IMU_Send:
			radio_Send(0);	// Send whats in circular buffer
			IMU_State = IMU_Done;
			break;

		case IMU_FAIL:
			clear_CircularBuffer();
			write_Buffer(0xFA); // Fail code
			write_Buffer(location); // Fail location
			radio_Send(0);//	// Send whats in circular buffer (fail: 0xFA, location)
			timeout = false;
			IMU_State = IMU_Done;
			location = 0x00;
			break;

		case IMU_Done:
			clear_CircularBuffer();
			LED(1,0,0,5,1); // BLUE
			IMU_mode = false;
			IMU_done = true;
			break;
	}
}


enum SCI_1_States { SCI_1_SMStart, SCI_1_PowerUp, SCI_1_HandShake, SCI_1_Read, SCI_1_PowerDown, SCI_1_Send, SCI_1_FAIL, SCI_1_Done} SCI_1_State;

void SCI_1_Loop()
{
	switch(SCI_1_State){
		case SCI_1_SMStart:
			sci_1_mode = true;
			Start_Timer();
			SCI_1_State = SCI_1_PowerUp;
			break;

		case SCI_1_PowerUp:
			if(power_Up(1)){timeout = false;location = 0x01;SCI_1_State = SCI_1_FAIL;break;}	// Power up science board 1
			SCI_1_State = SCI_1_HandShake;
			break;
			
		case SCI_1_HandShake:
			if(master_HandShake(1)){timeout = false;location = 0x02;power_Down(1);SCI_1_State = SCI_1_FAIL;break;}
//////////////////////////////////////
			UCB0CTLW0 &= ~UCMST;	// Move to slave mode so that RAD can drive SPI in master
////////////////////////////////////// Return to master mode after SCI_Ready line goes high
			if (get_state_x_PowerUp() && !timeout){power_Down(1);SCI_1_State = SCI_1_PowerUp;break;}
			else if(get_state_x_Send()){power_Down(1);SCI_1_State = SCI_1_Send;break;}
			LED(0,1,1,5,1);	// handshake 1 pass (GREEN RED)
			SCI_1_State = SCI_1_Read;
			break;
			
		case SCI_1_Read:
			while (!Sci_Ready() && !timeout){}
///////////////////////////////////
			UCB0CTLW0 |= UCMST;	 // Move to master mode so that RAD can drive SPI in master
///////////////////////////////////
			if (timeout){timeout = false;location = 0x03;power_Down(1);SCI_1_State = SCI_1_FAIL;break;}	// Deal with timeout during wait for science GPIO
			if(SCI_Read()){timeout = false;power_Down(1);SCI_1_State = SCI_1_FAIL;break;}
			SCI_1_State = SCI_1_PowerDown;
			break;

		case SCI_1_PowerDown:
			if(power_Down(1)){timeout = false;location = 0x04;SCI_1_State = SCI_1_FAIL;break;}	// Power up science board 1
			SCI_1_State = SCI_1_Send;
			break;
			
		case SCI_1_Send:
			radio_Send(1);	// Send whats in circular buffer
			SCI_1_State = SCI_1_Done;
			break;

		case SCI_1_FAIL:
			clear_CircularBuffer();
			write_Buffer(0xFA); // Fail code
			write_Buffer(location); // Fail location
			radio_Send(1);// Send whats in circular buffer (fail: 0xFA, location)
			timeout = false;
			SCI_1_State = SCI_1_Done;
			location = 0x00;
			break;

		case SCI_1_Done:
			clear_CircularBuffer();
			LED(0,1,0,5,1);	// GREEN
			sci_1_mode = false;
			sci_1_done = true;
			break;
	}
}

enum SCI_2_States { SCI_2_SMStart, SCI_2_PowerUp, SCI_2_HandShake, SCI_2_Read, SCI_2_PowerDown, SCI_2_Send, SCI_2_FAIL, SCI_2_Done} SCI_2_State;

void SCI_2_Loop()
{
	switch(SCI_2_State){
		case SCI_2_SMStart:
			sci_2_mode = true;
			Start_Timer();
			SCI_2_State = SCI_2_PowerUp;
			break;

		case SCI_2_PowerUp:
			if(power_Up(2)){timeout = false;location = 0x01;SCI_2_State = SCI_2_FAIL;break;};	// Power up science board 2
			SCI_2_State = SCI_2_HandShake;
			break;
			
		case SCI_2_HandShake:
			if(master_HandShake(2)){timeout = false;location = 0x02;power_Down(2);SCI_2_State = SCI_2_FAIL;break;}
			if (get_state_x_PowerUp() && !timeout){power_Down(2);SCI_2_State = SCI_2_PowerUp;break;}
			else if(get_state_x_Send()){power_Down(2);SCI_2_State = SCI_2_Send;break;}
			LED(1,1,0,5,1);	// handshake pass (BLUE GREEN)
			SCI_2_State = SCI_2_Read;
			break;
			
		case SCI_2_Read:
			while (!Sci_Ready() && !timeout){}
			if (timeout){timeout = false;location = 0x03;power_Down(2);SCI_2_State = SCI_2_FAIL;break;}
			if(SCI_Read()){timeout = false;power_Down(2);SCI_2_State = SCI_2_Done;break;}
			SCI_2_State = SCI_2_PowerDown;
			break;
			
		case SCI_2_PowerDown:
			if(power_Down(2)){timeout = false;location = 0x04;SCI_2_State = SCI_2_FAIL;break;}	// Power up science board 1
			SCI_2_State = SCI_2_Send;
			break;

		case SCI_2_Send:
			radio_Send(2);	// Send what is in circular buffer
			SCI_2_State = SCI_2_Done;
			break;

		case SCI_2_FAIL:
			clear_CircularBuffer();
			write_Buffer(0xFA); // Fail code
			write_Buffer(location); // Fail location
			radio_Send(2);	// Send whats in circular buffer (fail: 0xFA, location)
			timeout = false;
			SCI_2_State = SCI_2_Done;
			location = 0x00;
			break;

		case SCI_2_Done:
			clear_CircularBuffer();
			LED(0,0,1,5,1);	// RED
			sci_2_mode = false;
			sci_2_done = true;
			break;
	}
}

void main(void) {
	WDTCTL = WDTPW | WDTHOLD;					// Stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;
// Initializations
	Init_Clocks();					// Sets up timers (takes care of FRAM issue)
	__delay_cycles(500);
	Init_Ports();
	Init_LED();
	Init_Timer();
	
	init_SPI (pin_Setting);
	init_UART (1, 0);

	IMU_State = IMU_SMStart;
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
}
