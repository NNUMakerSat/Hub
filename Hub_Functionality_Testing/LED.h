/*
 * LED.h
 *
 *  Created on: Aug 20, 2016
 *      Author: aaronewing
 */

#ifndef LED_H_
#define LED_H_

void init_LED(void);

void LED_1_On(void);
void LED_2_On(void);

void LED_1_Off(void);
void LED_2_Off(void);

void flash_LED_1(uint8_t clk_Delay1, uint32_t duration1);
void flash_LED_2(uint8_t clk_Delay2, uint32_t duration2);

#endif /* LED_H_ */
