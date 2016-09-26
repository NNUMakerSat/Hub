/*
 * LSM9DS0.c
 *
 *  Created on: Sep 24, 2016
 *      Author: bgrim
 */
#include <msp430fr6989.h>
#include <stdint.h>
#include "LSM9DS0.h"
#include "i2c.h"

void initGyro(void) {
	single_byte_write_i2c(gyro_Address, CTRL_REG1_G, 0x0F);
	single_byte_write_i2c(gyro_Address, CTRL_REG4_G, 0x30);
}

void initXM(void) {
	single_byte_write_i2c(XMAddress, CTRL_REG1_XM, 0x67);
	single_byte_write_i2c(XMAddress, CTRL_REG2_XM, 0x20);
	single_byte_write_i2c(XMAddress, CTRL_REG5_XM, 0xF0);
	single_byte_write_i2c(XMAddress, CTRL_REG6_XM, 0x60);
	single_byte_write_i2c(XMAddress, CTRL_REG7_XM, 0x00);
}


