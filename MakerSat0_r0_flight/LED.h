/*
 * LED.h
 *
 *  Created on: Aug 17, 2016
 *      Author: MKamstra
 */
#include <msp430.h>
#include <stdint.h>

#ifndef LED_H_
#define LED_H_



void LED(uint8_t RED_LED, uint8_t GREEN_LED, uint8_t BLUE_LED, uint8_t delay_seconds, uint8_t blink);


#endif /* LED_H_ */
