#include <msp430.h> 
#include "Circular_Buffer.h"
#include "Packetizer.h"

// main.c
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    uint16_t source_ID = 0x0001;
    uint8_t i = 0;
    uint8_t exp_Counter = 4;

    uint8_t PolyExample[30] = {0x22, 0x33, 0x44, 0x55, '$', 'T', '0', 'p', 0x22, 0x33, '$', 'T', '0', 'p',
    		0x55, 0x22, 0x33, 0x44, 0x55, '$', 'T', '0', 'p', 0x11, 0xFF, 0x34 '$', 'T', '0', 'p'};

    uint8_t PolyIMU[11] = {0x22, 0x33, 0x44, 0x55, 0x22, 0x33, 0x55, 0x22, 0x33, 0x44, 0x68};

    uint8_t PolyRAD[4] = {0x22, 0x33, 0x44, 0x55};


//////////////////////////////////////////////////////////////////////////
//					 init Hub/SPI section								//
//////////////////////////////////////////////////////////////////////////
//	init_Hub();
//	init_Buffer();
//	uint8_t source_ID (returned) EPS(Hub&RAD);

    while (P4OUT |= BIT1) { 			// while SB is transmitting information
    	write_Buffer(SPI_read());
    	++exp_Counter;					// Counts times SPI read was used
    }

    Packetizer(source_ID, exp_Counter);

//////////////////////////////////////////////////////////////////////////
//					 write to Radio section								//
//////////////////////////////////////////////////////////////////////////

	return 0;
}
