#include <msp430.h>
#include <stdint.h>
#include "Circular_Buffer.h"
#include "Packetizer.h"

// main.c
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    uint16_t source_ID = 0x0001;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t exp_Counter = 0; //4

    uint8_t array[20];

    uint8_t example_POLY[30] = {0x22, 0x33, 0x44, 0x55, 0x24, 0x54, 0x00, 0x70};  //150 bytes expected? //memcpy.c??

//    uint8_t example_POLY[30] = {0x22, 0x33, 0x44, 0x55, 0x24, 0x54, 0, 0x70, 0x22, 0x33, 0x24, 0x54, 0, 0x70, 0x55, 0x22, 0x33, 0x44, 0x55, 0x24, 0x54, 0, 0x70, 0x11, 0xFF, 0x34 0x24, 0x54, 0, 0x70};

    uint8_t example_IMU[11] = {0x22, 0x33, 0x44, 0x55, 0x22, 0x33, 0x55, 0x22, 0x33, 0x44, 0x68};

    uint8_t example_RAD[4] = {0x19, 0x73, 0x22, 0x85};

    uint8_t *p_RAD;
    uint8_t *p_IMU;        					// pointers for Science Board Arrays
    uint8_t *p_POLY;


//////////////////////////////////////////////////////////////////////////
//					 init Hub/SPI section								//
//////////////////////////////////////////////////////////////////////////
//	init_Hub();
//	init_Buffer();
//	uint8_t source_ID (returned) EPS(Hub&RAD);

    for (j = 0; j < 4; ++j) {
    	write_Buffer(example_RAD[j]);
    	++exp_Counter;
    }

/*    while (P4OUT |= BIT1) { 			// while SB is transmitting information
 //   	write_Buffer(SPI_read());
    	++exp_Counter;					// Counts times SPI read was used
    } */

 /*   Packetizer(source_ID, exp_Counter);

      for (i = 0; i < 20; ++i) {
      	switch (source_ID) {
      	case 0:							// IMU
    //  		write_UART(*p_IMU);
     // 		array[i] = *p_IMU++;
      	case 1:							// RAD
    //  		write_UART(*p_RAD);
      		array[i] = *p_RAD++;		// works great




      	case 2:							// POLY
     // 		write_UART(*p_POLY);
      		++p_POLY;
      	}
      } */
/////////////////////////////////////////////////////////////////////////////// EXPERMENTATION CODE
    Packetizer(source_ID, exp_Counter);

    for (i = 0; i < 20; ++i) {
    	switch (source_ID) {
    	case 0:							// IMU
  //  		write_UART(*p_IMU);
  //  		array[i] = *p_IMU++;
    		++j;
    		break;
    	case 1:							// RAD
    //		write_UART(*p_RAD);
    		array[i] = *p_RAD++;		// works with EXTREME caution
    		break;						// alternates FFh and 3Fh when




    	case 2:							// POLY
   // 		write_UART(*p_POLY);
    //		array[i] = *p_POLY++;
    	//
    	//	break;



    	}
    }
      //////////////////////////////////////////////////////////////////////////////////////
	while (1) {}
}

//////////////////////////////////////////////////////////////////////////
//					 write to Radio section								//
//////////////////////////////////////////////////////////////////////////
/*
   // uint8_t  var = 20;   // actual variable declaration
    uint8_t  *ip;        // pointer variable declaration
    uint8_t array[3] = {10, 20, 30};
    uint8_t array2[6] = {0, 0, 0, 0, 0, 0};

    ip = &array[0];  // store address of var in pointer variable

//   array[0] = &var;
  // array[1] = ip;
   //array[2] = *ip;

    array2[0] = *ip;
    array2[1] = ++ip;
    array2[2] = *ip;
 //   array2[3] = *++ip;
   array2[4] = ++ip;
    array2[5] = *ip;
    */

    /* address stored in pointer variable */

    /* access the value using the pointer

	while (1) {}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////    THIS WORKS     /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
#include <msp430.h>
#include <stdint.h>
#include "Circular_Buffer.h"
#include "Packetizer.h"

// main.c
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    uint16_t source_ID = 0x0001;
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t exp_Counter = 0; //4

    uint8_t array[10];

    uint8_t example_POLY[30] = {0x22, 0x33, 0x44, 0x55, 0x24, 0x54, 0x00, 0x70};  //150 bytes expected?

//    uint8_t example_POLY[30] = {0x22, 0x33, 0x44, 0x55, 0x24, 0x54, 0, 0x70, 0x22, 0x33, 0x24, 0x54, 0, 0x70, 0x55, 0x22, 0x33, 0x44, 0x55, 0x24, 0x54, 0, 0x70, 0x11, 0xFF, 0x34 0x24, 0x54, 0, 0x70};

    uint8_t example_IMU[11] = {0x22, 0x33, 0x44, 0x55, 0x22, 0x33, 0x55, 0x22, 0x33, 0x44, 0x68};

    uint8_t example_RAD[4] = {0x22, 0x33, 0x44, 0x55};

    uint8_t *p_RAD;
    uint8_t *p_IMU;        					// pointers for Science Board Arrays
    uint8_t *p_POLY;


//////////////////////////////////////////////////////////////////////////
//					 init Hub/SPI section								//
//////////////////////////////////////////////////////////////////////////
//	init_Hub();
//	init_Buffer();
//	uint8_t source_ID (returned) EPS(Hub&RAD);

    for (j = 0; j < 4; ++j) {
    	write_Buffer(example_RAD[j]);
    	++exp_Counter;
    }

/*    while (P4OUT |= BIT1) { 			// while SB is transmitting information
 //   	write_Buffer(SPI_read());
    	++exp_Counter;					// Counts times SPI read was used
    } */
/*
    Packetizer(source_ID, exp_Counter);

    for (i = 0; i < 10; ++i) {
    	switch (source_ID) {
    	case 0:							// IMU
  //  		write_UART(*p_IMU);
   // 		array[i] = *p_IMU++;
    	case 1:							// RAD
  //  		write_UART(*p_RAD);
    		array[i] = *p_RAD++;		// works great




    	case 2:							// POLY
   // 		write_UART(*p_POLY);
    		++p_POLY;
    	}
    }

//////////////////////////////////////////////////////////////////////////
//					 write to Radio section								//
//////////////////////////////////////////////////////////////////////////
/*
   // uint8_t  var = 20;   // actual variable declaration
    uint8_t  *ip;        // pointer variable declaration
    uint8_t array[3] = {10, 20, 30};
    uint8_t array2[6] = {0, 0, 0, 0, 0, 0};

    ip = &array[0];  // store address of var in pointer variable

//   array[0] = &var;
  // array[1] = ip;
   //array[2] = *ip;

    array2[0] = *ip;
    array2[1] = ++ip;
    array2[2] = *ip;
 //   array2[3] = *++ip;
   array2[4] = ++ip;
    array2[5] = *ip;
    */

    /* address stored in pointer variable */

    /* access the value using the pointer */

//	return 0;
//}

