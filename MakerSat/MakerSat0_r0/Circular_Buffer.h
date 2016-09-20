/*
 * Circular_Buffer.h
 *
 *  Created on: Aug 10, 2016
 *      Author: Dr. Griffin
 */
#include <msp430fr6989.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

void init_Buffer(void);
void write_Buffer(uint8_t data);
void write_Buffer_Element(uint8_t data, uint8_t ele_Num);
uint8_t read_Buffer(void);
uint8_t read_Buffer_Element(uint8_t ele_Num);
bool is_Buffer_Full(void);
bool is_Buffer_Empty(void);
uint8_t bytes_In_Buffer(void);


#endif /* CIRCULAR_BUFFER_H_ */
