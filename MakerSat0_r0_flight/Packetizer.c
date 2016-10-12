/*
 * Packetizer.c
 *
 *  Created on: Sep 1, 2016
 *      Author: AaronEwing
 */
#include <stdint.h>
#include <stdbool.h>
#include "Circular_Buffer.h"

uint16_t g_POLY_Exp_Count = 0;
uint16_t g_RAD_Exp_Count = 0;
uint16_t g_IMU_Exp_Count = 0;
uint16_t g_source_ID_Exp_Count = 0x0000;

uint8_t POLY_Bytes[39] = { 0x50, 0x50, 0x50, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t IMU_Bytes[39] = { 0x50, 0x50, 0x50, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
uint8_t RAD_Bytes[39] = { 0x50, 0x50, 0x50, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

void Packetizer(uint16_t source_ID, uint8_t bytes_Read) {
	uint8_t ii = 6;
	switch (source_ID) {
	case 0:														// IMU
	default:
		source_ID <<= 14; 										// shift ID 14 bits to the left & fill the 14 bits with 1's

		g_source_ID_Exp_Count = source_ID | g_IMU_Exp_Count; 	// give source_ID_Exp_Count appropriate value

		if (g_IMU_Exp_Count >= 0x3FFF) {						// increment counter
			g_IMU_Exp_Count = 0x0000;
		} else {
			++g_IMU_Exp_Count;
		}

		IMU_Bytes[4] = g_source_ID_Exp_Count >> 8;				// put values into array
		IMU_Bytes[5] = g_source_ID_Exp_Count;

		while (!is_Buffer_Empty() && (ii < 39)) {							// reads from Circular Buffer into packetized buffer
			IMU_Bytes[ii] = read_Buffer();
			ii++;
		}

		while (ii < 39) {
			IMU_Bytes[ii] = 0x00;								// fill the rest of the packetzied buffer with zeroes
			ii++;
		}
		ii = 0;
		break;

	case 1:														// RAD
		source_ID <<= 14; 										// shift ID 14 bits to the left & fill the 14 bits with 1's

		g_source_ID_Exp_Count = source_ID | g_RAD_Exp_Count; 	// give source_ID_Exp_Count appropriate value

		if (g_RAD_Exp_Count >= 0x3FFF) {						// increment counter
			g_RAD_Exp_Count = 0x0000;
		} else {
			++g_RAD_Exp_Count;
		}

		RAD_Bytes[4] = g_source_ID_Exp_Count >> 8;				// put values into array
		RAD_Bytes[5] = g_source_ID_Exp_Count;

		while (!is_Buffer_Empty() && (ii < 39)) {							// reads from Circular Buffer into packetized buffer
			RAD_Bytes[ii] = read_Buffer();
			ii++;
		}

		while (ii < 39) {
			RAD_Bytes[ii] = 0x00;								// fill the rest of the packetzied buffer with zeroes
			ii++;
		}
		ii = 0;
		break;

	case 2:														// POLY
		source_ID <<= 14; 										// shift ID 14 bits to the left & fill the 14 bits with 1's

		g_source_ID_Exp_Count = source_ID | g_POLY_Exp_Count; 	// give source_ID_Exp_Count appropriate value

		if (g_POLY_Exp_Count >= 0x3FFF) {						// increment counter
			g_POLY_Exp_Count = 0x0000;
		} else {
			++g_POLY_Exp_Count;
		}

		POLY_Bytes[4] = g_source_ID_Exp_Count >> 8;				// put values into array
		POLY_Bytes[5] = g_source_ID_Exp_Count;

		while (!is_Buffer_Empty() && (ii < 39)) {							// reads from Circular Buffer into packetized buffer
			POLY_Bytes[ii] = read_Buffer();
			ii++;
		}

		while (ii < 39) {
			POLY_Bytes[ii] = 0x00;								// fill the rest of the packetzied buffer with zeroes
			ii++;
		}
		break;
	}
}

uint8_t get_IMU_Data(uint8_t index) {
	return IMU_Bytes[index];
}

uint8_t get_RAD_Data(uint8_t index) {
	return RAD_Bytes[index];
}

uint8_t get_POLY_Data(uint8_t index) {
	return POLY_Bytes[index];
}

uint8_t get_Data(uint8_t index, uint8_t source_ID) {
	switch (source_ID) {
	case 0:
	default:
		return IMU_Bytes[index];
	case 1:
		return RAD_Bytes[index];
	case 2:
		return POLY_Bytes[index];
	}
}

// Pass pointer to start of all arrays
// work out getting rid of $T0p bytes

/* Aaron's Notes: SPI read all data initually into the circular buffer
 * read the size with a counter and then read in that number of bytes
 * from the buffer >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> not packetizer
 * (ie if 4 is put in, read four out. They should all be
 * in correct order. Now what will happen is you will make a couple of arrays
 * that will output the entire packet to the proper area (multiple arrays
 * because don't wanan do stop bytes much). >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> done
 * This means that you will have all the control bytes (nope),
 * the experment and source ID numbers, and the data with a pointer to
 * the start of it being passed out of the function. >>>>>>>>>>>>>>>>>>>>>doententent
 * The issue is with the polymer board. I will have to check and delete stop
 * bytes ($T0p) and then put it into arrays. It might be multiple packets long
 * with multiple (nononon single) points (struct? or multiple calls? nonono
 * just one array.) UART will then have to be able to stop reading after 35 bytes
 * and keep going after that until every bit is sent. YAy!
 */

/*
 uint8_t *Packetizer(uint16_t source_ID, uint8_t bytes_Read) {
 uint8_t *p_RAD = 0;
 uint8_t *p_IMU = 0;
 uint8_t *p_POLY = 0;

 switch (source_ID) {
 case 0:													// IMU
 *p_IMU = &IMU_Bytes[0];					// stores 1st element of array address in pointer (don't move out of swi!ch!)
 source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
 source_ID &= 0x3FFFF;  //check/

 source_ID_Exp_Count = source_ID | g_IMU_Exp_Count;	// give source_ID_Exp_Count appropriate value

 if (g_IMU_Exp_Count >= 0x3FFF) {					// increment counter
 g_IMU_Exp_Count = 0x0000;
 } else {
 ++g_IMU_Exp_Count;
 }
 IMU_Bytes[0] = IMU_Bytes[1] = IMU_Bytes[2] = 0x50;
 IMU_Bytes[3] = 0x0C;
 IMU_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
 IMU_Bytes[5] = source_ID_Exp_Count;

 for (i = 6; i < (11+6); ++i) {
 IMU_Bytes[i] = read_Buffer();
 }

 return p_IMU;

 case 1:													// RAD
 *p_RAD = &RAD_Bytes[0];					// stores 1st element of array address in poin (don't move out of switch!)h!)
 source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
 source_ID &= 0x3FFFF;

 source_ID_Exp_Count = source_ID | g_RAD_Exp_Count;	// give source_ID_Exp_Count appropriate value

 if (g_RAD_Exp_Count >= 0x3FFF) {					// increment counter
 g_RAD_Exp_Count = 0x0000;
 } else {
 ++g_RAD_Exp_Count;
 }

 RAD_Bytes[0] = RAD_Bytes[1] = RAD_Bytes[2] = 0x50;
 RAD_Bytes[3] = 0x0C;
 RAD_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
 RAD_Bytes[5] = source_ID_Exp_Count;

 for (i = 6; i < (4+6); ++i) {					// works up to this point
 RAD_Bytes[i] = read_Buffer();
 }

 return p_RAD;

 case 2:													// POLY
 *p_POLY = &POLY_Bytes[0];				// stores 1st element of array address in point(don't move out of switch!)h!)
 source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
 source_ID &= 0x3FFFF;

 source_ID_Exp_Count = source_ID | g_IMU_Exp_Count;	// give source_ID_Exp_Count appropriate value

 if (g_IMU_Exp_Count >= 0x3FFF) {					// increment counter
 g_IMU_Exp_Count = 0x0000;
 } else {
 ++g_IMU_Exp_Count;
 }

 POLY_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
 POLY_Bytes[5] = source_ID_Exp_Count;

 return p_POLY;

 default:												// ERROR, so no packet
 break;
 }
 }


 void init_Buffers(void) {


 //	uint8_t control_Bits[6] = {0x50, 0x50, 0x50, 0x0C};		// Creates control  its

 // init buffer just does what is natural for the circular buffer
 init_Buffer();						// (reason for the same thing is so that it is all in one place)
 // please note that max is 1000 bytes
 }

 //void write_To_Packetizer(uint8_t packet_Data, uint8_t SB_Select) {

 //}

 /*
 uint8_t init_Counter1;
 uint8_t init_Counter_Control = 0;
 uint8_t init_Counter2;

 for (init_Counter1 = 0; init_Counter1 < 23; ++init_Counter1) {		// writes 23 packets worth of control bits
 switch (init_Counter_Control) {
 case 0:
 case 1:
 case 2:
 write_Buffer(0x50);			// writes 0x50 3 times to the pic, to let it know it is going to be used
 ++init_Counter_Control;
 break;
 case 3:
 write_Buffer(0x0C);			// writes 0x0C to say it will be using the radio
 init_Counter_Control = 0;	// resets counter
 }
 for (init_Counter2 = 4; init_Counter2 < 39; ++init_Counter2) {	// fill elements other than control with 0
 write_Buffer(0);
 }
 } */

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////    THIS WORKS     /////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * Packetizer.c
 *
 *  Created on: Sep 1, 2016
 *      Author: AaronEwing
 *
 #include <stdint.h>
 #include <stdbool.h>
 #include "Circular_Buffer.h"

 uint16_t g_POLY_Exp_Count = 0;
 uint16_t g_RAD_Exp_Count = 0;
 uint16_t g_IMU_Exp_Count = 0x3FFF;
 uint16_t source_ID_Exp_Count = 0x0000;

 uint8_t i;

 uint8_t POLY_Bytes[100];
 uint8_t IMU_Bytes[17] = {0x50, 0x50, 0x50, 0x0C, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
 uint8_t RAD_Bytes[10] = {0x51, 0x50, 0x50, 0x0C, 0, 0, 0, 0, 0, 0};


 //uint16_t *p_RAD = &RAD_Bytes[0];
 //uint8_t *p_IMU = &IMU_Bytes[0];        					// pointers for Science Board Arrays
 //uint8_t *p_POLY = &POLY_Bytes[0];

 //uint8_t* p_RAD = &RAD_Bytes[0];
 //uint8_t* p_IMU = &IMU_Bytes[0];					// stores 1st element of array into pointer
 //uint8_t* p_POLY = &POLY_Bytes[0];					// does not compile




 // Pass pointer to start of all arrays
 // work out getting rid of $T0p bytes

 /* Aaron's Notes: SPI read all data initually into the circular buffer
 * read the size with a counter and then read in that number of bytes
 * from the buffer >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> not packetizer
 * (ie if 4 is put in, read four out. They should all be
 * in correct order. Now what will happen is you will make a couple of arrays
 * that will output the entire packet to the proper area (multiple arrays
 * because don't wanan do stop bytes much). >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> done
 * This means that you will have all the control bytes (nope),
 * the experment and source ID numbers, and the data with a pointer to
 * the start of it being passed out of the function. >>>>>>>>>>>>>>>>>>>>>>>>>>>>> current
 * The issue is with the polymer board. I will have to check and delete stop
 * bytes ($T0p) and then put it into arrays. It might be multiple packets long
 * with multiple (nononon single) points (struct? or multiple calls? nonono
 * just one array.) UART will then have to be able to stop reading after 35 bytes
 * and keep going after that until every bit is sent. YAy!
 *

 uint8_t *Packetizer(uint16_t source_ID, uint8_t bytes_Read) {
 uint8_t *p_RAD;
 //	uint8_t *p_IMU;
 //	uint8_t *p_POLY;
 *p_RAD = &RAD_Bytes[0];
 //	*p_IMU = &IMU_Bytes[0];					// stores 1st element of array address in pointer
 //*p_POLY = &POLY_Bytes[0];

 switch (source_ID) {
 case 0:													// IMU
 source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
 source_ID &= 0x3FFFF;

 source_ID_Exp_Count = source_ID | g_IMU_Exp_Count;	// give source_ID_Exp_Count appropriate value

 if (g_IMU_Exp_Count >= 0x3FFF) {					// increment counter
 g_IMU_Exp_Count = 0x0000;
 } else {
 ++g_IMU_Exp_Count;
 }
 IMU_Bytes[0] = IMU_Bytes[1] = IMU_Bytes[2] = 0x50;
 IMU_Bytes[3] = 0x0C;
 IMU_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
 IMU_Bytes[5] = source_ID_Exp_Count;

 for (i = 6; i < (11+6); ++i) {
 IMU_Bytes[i] = read_Buffer();
 }

 //		return p_IMU;

 case 1:													// RAD
 source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
 source_ID &= 0x3FFFF;

 source_ID_Exp_Count = source_ID | g_RAD_Exp_Count;	// give source_ID_Exp_Count appropriate value

 if (g_RAD_Exp_Count >= 0x3FFF) {					// increment counter
 g_RAD_Exp_Count = 0x0000;
 } else {
 ++g_RAD_Exp_Count;
 }

 RAD_Bytes[0] = RAD_Bytes[1] = RAD_Bytes[2] = 0x50;
 RAD_Bytes[3] = 0x0C;
 RAD_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
 RAD_Bytes[5] = source_ID_Exp_Count;

 for (i = 6; i < (4+6); ++i) {					// works up to this point
 RAD_Bytes[i] = read_Buffer();
 }

 return p_RAD;

 case 2:													// POLY
 source_ID <<= 14;						// shift ID 14 bits to the left & fill the 14 bits with 1's
 source_ID &= 0x3FFFF;

 source_ID_Exp_Count = source_ID | g_IMU_Exp_Count;	// give source_ID_Exp_Count appropriate value

 if (g_IMU_Exp_Count >= 0x3FFF) {					// increment counter
 g_IMU_Exp_Count = 0x0000;
 } else {
 ++g_IMU_Exp_Count;
 }

 POLY_Bytes[4] = source_ID_Exp_Count >> 8;		// put values into array
 POLY_Bytes[5] = source_ID_Exp_Count;

 //		return p_POLY;

 default:												// ERROR, so no packet
 break;
 }

 return 1;

 }


 void init_Buffers(void) {


 //	uint8_t control_Bits[6] = {0x50, 0x50, 0x50, 0x0C};		// Creates control  its

 // init buffer just does what is natural for the circular buffer
 init_Buffer();						// (reason for the same thing is so that it is all in one place)
 // please note that max is 1000 bytes
 }

 //void write_To_Packetizer(uint8_t packet_Data, uint8_t SB_Select) {

 //}

 /*
 uint8_t init_Counter1;
 uint8_t init_Counter_Control = 0;
 uint8_t init_Counter2;

 for (init_Counter1 = 0; init_Counter1 < 23; ++init_Counter1) {		// writes 23 packets worth of control bits
 switch (init_Counter_Control) {
 case 0:
 case 1:
 case 2:
 write_Buffer(0x50);			// writes 0x50 3 times to the pic, to let it know it is going to be used
 ++init_Counter_Control;
 break;
 case 3:
 write_Buffer(0x0C);			// writes 0x0C to say it will be using the radio
 init_Counter_Control = 0;	// resets counter
 }
 for (init_Counter2 = 4; init_Counter2 < 39; ++init_Counter2) {	// fill elements other than control with 0
 write_Buffer(0);
 }
 } */

