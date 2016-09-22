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

#define HandShake 0xBB

bool break_flg = 0;
uint32_t i = 0;
uint8_t j = 0;
uint8_t l = 0;
uint16_t k = 0;
uint8_t g_RXData;
uint16_t source_ID = 1;
uint16_t g_bottom;
uint16_t g_top;
bool g_bufferEmpty;
int IMU_done = 0;
int sci_1_done = 0;
int sci_2_done = 0;
int HandShake_FAIL[3];

int radio_busy = 0;

void IMU_Loop();
void SCI_1_Loop();
void SCI_2_Loop();


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
			for (k = 500; k > 0; k--)
			{
				write_Buffer(0x00);		// Clear circular buffer
			}
			j = 0;
			g_bottom = 0;				// Set circular buffer back to zero
			g_top = 0;					// Set circular buffer back to zero
			g_bufferEmpty = false;		// Clear the g_bufferEmpty flag so that the next science board can store data
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
			// Radio powerup science board information
			powerUp(1);	// Power up science board 1
			P4OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
			P4OUT |= BIT4;
			SCI_1_State = SCI_1_HandShake;
			break;
			
		case SCI_1_HandShake:
/*
			init();
			start_timer();

			while(!timeout && !GPIO){}

			if(timeout) {
				// what you do if timeout occurred
			}
			else {
				//what you do if GPIO went high before timeout
			}
*/
			// Send Science board handshake byte
			break_flg = 0;
			while (!(Sci_Ready()) && !break_flg) // Waits for GPIO to go high
			{
				i++;
				if (i > 100000)	// Wait for 5 seconds to recieve a high otherwise restart science board 1
				{
					powerDown(1);
					SCI_1_State = SCI_1_PowerUp;
					break_flg = 1;
				}
			}
			i = 0;
			if (break_flg){break;}
/*			read_SPI ();
			if (g_RXData != HandShake)
				{
					HandShake_FAIL[k] = g_RXData;
					SCI_1_State = SCI_1_PowerUp;
					break;
				}
				k++;
				if (k > 2)	// Try three times and move on to science board 2 if no handshake
				{
					SCI_1_State = SCI_1_Done;
					while(l < 3)
					{
						write_UART(HandShake_FAIL[l]);
						l++;
					}
					break;
					k = 0;
				}
*/
			g_RXData = 0x00;
			break_flg = 0;
			k = 0;
			SCI_1_State = SCI_1_Read;
			break;
			
		case SCI_1_Read:
			while(g_RXData != 0xFF){
					while (!(Sci_Ready())) {}							// Waits for GPIO to go high
					read_SPI ();
					write_Buffer(g_RXData);
					i++;
			}
			Packetizer(1, 0);
			g_RXData = 0;	// Clear variable for next science board.
			i = 0;
			SCI_1_State = SCI_1_PowerDown;
			break;

		case SCI_1_PowerDown:
			// Radio powerdown science board information
			powerDown(1);	// Power up science board 1
			P4OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
			SCI_1_State = SCI_1_Send;
			break;
			
		case SCI_1_Send:
			// Radio data
			while (j < 39)
			{
				write_UART (get_Data(j,1));
				j++;
			}
			SCI_1_State = SCI_1_Done;
			break;

		case SCI_1_Done:
			for (k = 500; k > 0; k--)
			{
				write_Buffer(0x00);		// Clear circular buffer
			}
			j = 0;
			g_bottom = 0;				// Set circular buffer back to zero
			g_top = 0;					// Set circular buffer back to zero
			g_bufferEmpty = false;		// Clear the g_bufferEmpty flag so that the next science board can store data
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
			// Radio powerup science board information
			powerUp(2);	// Power up science board 2
			P4OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
			P4OUT |= BIT5;
			SCI_2_State = SCI_2_HandShake;
			break;
			
		case SCI_2_HandShake:
			// Send Science board handshake byte
/*
			init();
			start_timer();

			while(!timeout && !GPIO){}

			if(timeout) {
				// what you do if timeout occurred
			}
			else {
				//what you do if GPIO went high before timeout
			}
*/
			// Send Science board handshake byte
			break_flg = 0;
			while (!(Sci_Ready()) && !break_flg) // Waits for GPIO to go high
			{
				i++;
				if (i > 100000)	// Wait for 5 seconds to recieve a high otherwise restart science board 1
				{
					powerDown(2);
					SCI_2_State = SCI_2_PowerUp;
					break_flg = 1;
				}
			}
			i = 0;
			if (break_flg){break;}
/*			read_SPI ();
			if (g_RXData != HandShake)
				{
					HandShake_FAIL[k] = g_RXData;
					SCI_2_State = SCI_2_PowerUp;
					break;
				}
				k++;
				if (k > 2)	// Try three times and move on to science board 2 if no handshake
				{
					SCI_2_State = SCI_2_Done;
					while(l < 3)
					{
						write_UART(HandShake_FAIL[l]);
						l++;
					}
					break;
					k = 0;
				}
*/
			g_RXData = 0x00;
			break_flg = 0;
			k = 0;
			SCI_2_State = SCI_2_Read;
			break;
			
		case SCI_2_Read:
			while(g_RXData != 0xFF){
					while (!(Sci_Ready())) {}							// Waits for GPIO to go high
					read_SPI ();
					write_Buffer(g_RXData);
					i++;
			}
			Packetizer(1, 0);
			g_RXData = 0;	// Clear variable for next science board.
			i = 0;
			// Request power down from NSL
			SCI_2_State = SCI_2_PowerDown;
			break;
			
		case SCI_2_PowerDown:
			// Radio powerdown science board information
			powerDown(2);	// Power up science board 1
			P4OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
			SCI_2_State = SCI_2_Send;
			break;

		case SCI_2_Send:
			// Radio data
			while (j < 39)
			{
				write_UART (get_Data(j,1));
				j++;
			}
			SCI_2_State = SCI_2_Done;
			break;

		case SCI_2_Done:
			for (k = 500; k > 0; k--)
			{
				write_Buffer(0x00);		// Clear circular buffer
			}
			j = 0;
			g_bottom = 0;				// Set circular buffer back to zero
			g_top = 0;					// Set circular buffer back to zero
			g_bufferEmpty = false;		// Clear the g_bufferEmpty flag so that the next science board can store data
			P9OUT = BIT7;
			__delay_cycles(2000000);
			P9OUT = 0x00;
			sci_2_done = 1;
			break;
	}
}



// Interrupt for Timer 1
// used to set global_Flg_2 which runs func_1 when the ISR is exited
/*
#pragma vector=TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void)

{
//	global_Flg_1 = 0x01;
	__low_power_mode_off_on_exit();
	TA1CTL &= ~CCIFG;
	TA1CCTL1 &= ~CCIFG;

}
*/


int main(void) {
	WDTCTL = WDTPW | WDTHOLD;					// Stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;						// Disable the GPIO power-on default high-impedance mode to actibate previoulsy configured port settings

// Initializations
	Init_LED();
	Init_Timer();
//	Init_Port();
	Init_Comm();	// Should I wait on UART init???
	Init_ADC();
	Init_Var();


	initialize_Ports();						// Init all non used ports
	initialize_Clocks();					// Sets up timers (takes care of FRAM issue)

	uint8_t pin_Setting = 0;				// selects the pins used for 6989
	uint8_t device_CS = 0;					// selects the SYNC/SS pin (5k POT)

//P2DIR &= ~BIT1;
	init_SPI (pin_Setting);
	init_UART (1, 0);
/*
*	Determine Mode of Satellite
*		Check if in Diagnostics mode - Set flag
*			Might be disabled for this flight
*		Check if Radio responds
*			Re-initializes UART and attempt again?
*			Blink light
*
*/
	SCI_1_State = SCI_1_SMStart;
	SCI_2_State = SCI_2_SMStart;
	//	__enable_interrupt();
	while(1) {
		// wait for interrupt
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

//		__low_power_mode_3();

	}
	return 0;
}
