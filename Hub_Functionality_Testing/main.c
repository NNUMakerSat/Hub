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
	uint8_t test_Array[39] = {0x50, 0x50, 0x50, 0x0C, 'D', 'e', 'a', 'r', ' ', 'E', 'L', 'a', 'N', 'a', ' ', 'X', 'I', 'V', ',',
			' ', 'M', 'a', 'k', 'e', 'r', 'S', 'a', 't', '-', '0', ' ', 'i', 's', ' ', 'r', 'e', 'a', 'd', 'y'};
			// 4 control bytes + "Dear ELaNa XIV, MakerSat-0 is ready"

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

	initialize_Ports();
	initialize_Clocks();
	init_LED();

	LED_1_On();								// Turn LED 1 on, indicating power

	init_UART(0, 0);						// Initalize UART at Baud = 9600 with P2.0 - TX, P2.1 - RX, P1.4 - Radio Busy line

	for (i = 0; i < 39; ++i) {			// Writes test_Array to radio
		write_UART(test_Array[i]);
//		flash_LED_2(0, 1);				// Flash LED 2 at 10Hz once
	}									// 0 - 10Hz, 1 - Hz

	while (1) {
	flash_LED_2(1, 1);				// Flash LED 2 at 1HZ forever after UART TX
	}
}
