/*
 * i2c.h
 *
 *  Created on: Sep 24, 2016
 *      Author: bgrim
 */
#include <stdint.h>
#ifndef I2C_H_
#define I2C_H_

void init_i2c(void);

uint8_t single_byte_write_i2c(uint8_t slaveAddress, uint8_t registerAddress,
		uint8_t setBits);

uint8_t single_byte_read_i2c(uint8_t slaveAddress, uint8_t registerAddress);

#endif /* I2C_H_ */
