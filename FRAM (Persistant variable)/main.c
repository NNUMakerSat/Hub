#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;   // Disable the GPIO power-on default high-impedance mode ABSOLUTELY REQUIRED

    int i = 0;
while (1) {
    if (i > 0 || i > 9) {
    	i = 1;
    } else {
    	i++;
    }
}
	return 0;
}
