/* Code is for testing Hub functionality with radio
 * after a vibe test. The code is supposed to turn on LED(s)
 * while running code, and transmit a single packet to the radio
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "Initialize.h"
#include "LED.h"
#include "UART_Polling.h"

// main.c
int main(void) {
	uint8_t i;				// counter for loop
	uint8_t j = 0;
	uint8_t RX_Data;
	uint8_t collect_Array[39];
	uint8_t test_Array[39] = {0x50, 0x50, 0x50, 0x0C, 'D', 'e', 'a', 'r', ' ', 'E', 'L', 'a', 'N', 'a', ' ', 'X', 'I', 'V', ',',
			' ', 'M', 'a', 'k', 'e', 'r', 'S', 'a', 't', '-', '0', ' ', 'i', 's', ' ', 'r', 'e', 'a', 'd', 'y'};
			// 4 control bytes + "Dear ELaNa XIV, MakerSat-0 is ready"

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

	initialize_Ports();
	initialize_Clocks();
	init_LED();
	init_UART(1, 0);						// Initalize UART at Baud = 38400 with P2.0 - TX, P2.1 - RX, P1.4 - Radio Busy line

	LED_1_On();
	for (i = 0; i < 39; ++i) {				// Writes test_Array to radio
		write_UART(test_Array[i]);
//		flash_LED_1(10, 1);					// Flash LED 1 at 500 Hz (default) once for every byte through TX
	}
	LED_2_On();
	for (j = 0; j < 39; ++j) {				// Writes test_Array to radio
		read_UART();
//		flash_LED_2(0, 1);					// Flash LED 2 at 10Hz once
		collect_Array[j] = RX_Data;			// 0 - 10Hz, 1 - Hz
	}
/*
	while (1) {								//reads what emulator puts in.
		read_UART();
		collect_Array[j] = RX_Data;
		++j;
	}
*/

/*	for (i = 0; i < 39; ++i) {			// Writes test_Array to radio
			write_UART(test_Array[i]);
	//		flash_LED_2(0, 1);				// Flash LED 2 at 10Hz once
//			read_UART();
	//		collect_Array[j] = RX_Data;
		//	++j;
		}									// 0 - 10Hz, 1 - Hz

		while (1) {							//reads what emulator puts in.
			read_UART();
			collect_Array[j] = RX_Data;
			++j;
			LED_2_On();
		}

*/
	while (1) {
	flash_LED_1(1, 1);				// Flash LED 1 & 2 at 1HZ forever after UART TX
	flash_LED_2(1, 1);
	}
}
