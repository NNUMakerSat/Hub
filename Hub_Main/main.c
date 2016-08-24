/* This will be the code that will be run when
 * the Hub is in orbit, awakened by the EPS.
 */

// Note: The variables used for commuinication are not finialized

#include <msp430.h> 

// main.c
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    ///////////////////////// INITIALIZE HUB //////////////////////////////
    initialize_Ports();				// Makes all unused ports and output of 0
    initialize_Clocks();			// Sets up all internal clocks
    initialize_WDT();				// Set up the WDT with the EPS (not yet created)
    init_LED();
    init_UART(0, 0);				// Sets up the UART connection with EPS and radio
    init_Circular_Buffers_SBs();	// Set up all Buffers to hold info, including already
    								// having Control Bits set

    ///////////////////////// SCIENCE BOARD1 ///////////////////////
    init_SPI(1, 0);						// Sets up SPI connection with *that* Science Board (before SB is even on
    									// just incase Hub needs to connect to SB instantly
    init_I2C(x, x);						// Sets up SPI connection with *that* Science Board (as needed)

    EPS_To_SB(bool board1, bool pwr);	// Sends request for power on to SB (not yet created, but should be easy)
    									// 0x50, 50, 50, 0B, board1, output1, board2, output2, xxxxxxxx
    									// then check for ACK: AA 05 00, or NACK: AA 05 FF

    while (data_Transmit & Timer != 0) {			// Loop just an example of how this could be implemented
    	read_SPI();						// Reads SPI from Science Board (needs to be upgraded so that it runs on
    									// interrupts (allows to sleep for the 1-10 minutes SB will take)
    									// and to clear its external WDT
    	Circular_Buffer_SB1[i] = RX_Data;	// Saves RX data from Science Board (variable i would have to skip control bits)
    }

    EPS_To_SB(bool board1, bool no pwr);	// Sends request for power off to SB

    ///////////////////////// SCIENCE BOARD2 ///////////////////////
    init_SPI(1, 1);						// Sets up SPI connection with *that* Science Board (before SB is even on
       									// just incase Hub needs to connect to SB instantly
    init_I2C(x, x);						// Sets up SPI connection with *that* Science Board (as needed)

    EPS_To_SB(bool board2, bool pwr);	// Sends request for power on to SB (not yet created, but should be easy)
       									// 0x50, 50, 50, 0B, board1, output1, board2, output2, xxxxxxxx
       									// then check for ACK: AA 05 00, or NACK: AA 05 FF

    while (data_Transmit & Timer != 0) {			// Loop just an example of how this could be implemented
        read_SPI();							// Reads SPI from Science Board (needs to be upgraded so that it runs on
        									// interrupts (allows to sleep for the 1-10 minutes SB will take)
        									// and to clear its external WDT
        Circular_Buffer_SB2[i] = RX_Data;	// Saves RX data from Science Board (variable i would have to skip control bits)
    }

    EPS_To_SB(bool board2, bool no pwr);	// Sends request for power off to SB

    ///////////////////////// RADIO ///////////////////////////////
    while (Circular_Buffer_SB1 & !BUSY) {			// Sends Science Board 1 data to Radio if packets to send and radio not busy
    	Send_To_Radio(Circular_Buffer_SB1[i]);
    	if (ACK) {									// If ACK is recieved next packet, if NACK then repeat
    	++i;
    	} else {
    	}
    }


	while (Circular_Buffer_SB2 & !BUSY) {			// Sends Science Board 1 data to Radio if packets to send and radio not busy
    	Send_To_Radio(Circular_Buffer_SB2[j]);
    	if (ACK) {									// If ACK is recieved next packet, if NACK then repeat
    	++j;
    	} else {
    	}
    }

    ///////////////////////// FIN ///////////////////////////////
    return 0;										// Return value that represents that Hub is ready to be turned off
}
