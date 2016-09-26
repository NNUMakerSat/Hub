//  Description: This demo connects a MSP430FR6989 launchpad to an LSM9DS0 via
//  USCI_B1 I2C module. The launchpad acts as a master and the LSM9DS0 acts as a
//  slave. Master reads 6 byte consecutively and send it to the vitual COM
//  Port on Usb with USCI_A0 Uart module. DCO = 1MHz
//
//                  LSM9DS0          			 MSP430FR6989
//                   slave            			    master
//             -----------------  			   -----------------
//            |              SDA|<------+---->|P3.1/UCB1SDA     |
//            |              CLK|<--          |                 |
//            |              GND|<--          |                 |
//            |              SCL|<-+--------->|P3.2/UCB1SCL     |
//            |              VCC|<--.         |                 |
//            |              VIO|<--'3.3V     |                 |
//            |              INT|             |                 |

//  Original code modified by Braden Grim
//******************************************************************************
#include <msp430fr6989.h>
#include <stdio.h>
#include <stdint.h>
#include "LSM9DS0.h"
#include "i2c.h"

#define G_GAIN	0.07		//[deg/s/LSB]
#define A_GAIN	0.732		//[mg/LSB]
#define thousand	1000		//[mg/LSB]

char xRate_L;  // Variable to hold X_Low register value
char xRate_H;  // Variable to hold X_High register value
char yRate_L;  // Variable to hold Y_Low register value
char yRate_H;  // Variable to hold Y_High register value
char zRate_L;  // Variable to hold Z_Low register value
char zRate_H;  // Variable to hold Z_High register value
char xaccel_L;
char xaccel_H;
char yaccel_L;
char yaccel_H;
char zaccel_L;
char zaccel_H;

int32_t gyrRawx;
int32_t gyrRawy;
int32_t gyrRawz;
float gyroXangle;
float gyroYangle;
float gyroZangle;

int32_t accRawx;
int32_t accRawy;
int32_t accRawz;
float g_x;
float g_y;
float g_z;

float rate_gyr_x;
float rate_gyr_y;
float rate_gyr_z;

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 			// Stop WDT
	PM5CTL0 &= ~LOCKLPM5;

//Set clock speed
	CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
	CSCTL1 = DCOFSEL_6;                       // Set DCO to 8MHz
	CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK; // Set SMCLK = MCLK = DCO
	// ACLK = VLOCLK
	CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // Set all dividers to 1
	CSCTL0_H = 0;                             // Lock CS registers
//Set clock speed

	rate_gyr_x = 0;
	rate_gyr_y = 0;
	rate_gyr_z = 0;

	init_i2c();
	initGyro();
	initXM();

	while (1) {
		////////////READ GYROSCOPE//////////////
		xRate_L = single_byte_read_i2c(gyro_Address, OUT_X_L_G);
		xRate_H = single_byte_read_i2c(gyro_Address, OUT_X_H_G);
		yRate_L = single_byte_read_i2c(gyro_Address, OUT_Y_L_G);
		yRate_H = single_byte_read_i2c(gyro_Address, OUT_Y_H_G);
		zRate_L = single_byte_read_i2c(gyro_Address, OUT_Z_L_G);
		zRate_H = single_byte_read_i2c(gyro_Address, OUT_Z_H_G);

		/////////////READ ACCELEROMETER/////////////////
		xaccel_L = single_byte_read_i2c(XMAddress, OUT_X_L_A);
		xaccel_H = single_byte_read_i2c(XMAddress, OUT_X_H_A);
		yaccel_L = single_byte_read_i2c(XMAddress, OUT_Y_L_A);
		yaccel_H = single_byte_read_i2c(XMAddress, OUT_Y_H_A);
		zaccel_L = single_byte_read_i2c(XMAddress, OUT_Z_L_A);
		zaccel_H = single_byte_read_i2c(XMAddress, OUT_Z_H_A);

		////////////////CONVERT TO MEANINGFUL DATA//////////////////////
		gyrRawx = (xRate_L | xRate_H << 8);
		gyrRawy = (yRate_L | yRate_H << 8);
		gyrRawz = (zRate_L | zRate_H << 8);

		rate_gyr_x = gyrRawx * G_GAIN;
		rate_gyr_y = gyrRawy * G_GAIN;
		rate_gyr_z = gyrRawz * G_GAIN;

		accRawx = (xaccel_L | xaccel_H << 8);
		accRawy = (yaccel_L | yaccel_H << 8);
		accRawz = (zaccel_L | zaccel_H << 8);

		g_x = (accRawx * A_GAIN) / thousand;
		g_y = (accRawy * A_GAIN) / thousand;
		g_z = (accRawz * A_GAIN) / thousand;
	}
}
