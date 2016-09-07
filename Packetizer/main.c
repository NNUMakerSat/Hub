#include <msp430.h> 
#include <stdint.h>
#include "Circular_Buffer.h"
#include "Packetizer.h"

// main.c
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
/*
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

    Packetizer(source_ID, exp_Counter); */

//////////////////////////////////////////////////////////////////////////
//					 write to Radio section								//
//////////////////////////////////////////////////////////////////////////

   // uint8_t  var = 20;   /* actual variable declaration */
    uint8_t  *ip;        /* pointer variable declaration */
    uint8_t array[3] = {10, 20, 30};
    uint8_t array2[6] = {0, 0, 0, 0, 0, 0};

    ip = &array[0];  /* store address of var in pointer variable*/

//   array[0] = &var;
  // array[1] = ip;
   //array[2] = *ip;

    array2[0] = *ip;
    array2[1] = ++ip;
    array2[2] = *ip;
 //   array2[3] = *++ip;
   array2[4] = ++ip;
    array2[5] = *ip;

    /* address stored in pointer variable */

    /* access the value using the pointer */

	return 0;
}
