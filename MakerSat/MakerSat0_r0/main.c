/*
 * main.c
 */
#include <msp430.h>
#include <stdint.h>
#include <stdio.h>
#include "init.h"
#include "LED.h"
#include "poop.h"
//#include "radio_chk.h"
#include "Comm.h"

enum SCI_States { SCI_SMStart, SCI_Init, SCI_PowerUp, SCI_UART, SCI_PowerDown} SCI_State;

void SCI_Loop()
{
	switch(SCI_State){
		case SCI_SMStart:
			SCI_State = SCI_Init;
			break;
			
		case SCI_Init:
			// Initialize???
			SCI_State = SCI_PowerUp;
			break;
		
		case SCI_PowerUp:
			// Initialize UART comms with NSL
			// Send power up command for SCI board X
			SCI_State = SCI_UART;
			break;
			
		case SCI_UART:
			

			
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
	Init_Port();
	Init_Comm();	// Should I wait on UART init???
	Init_ADC();
	Init_Var();

	
/*
*	Determine Mode of Satellite
*		Check if in Diagnostics mode - Set flag
*			Might be disabled for this flight
*		Check if Radio responds
*			Re-initializes UART and attempt again?
*			Blink light
*
*/
//	SCI_State = SCI_SMStart;
//	__enable_interrupt();
	while(1) {
		// wait for interrupt


		//		LED();	// Just to check functionality
//		__low_power_mode_3();

	}
	return 0;
}
