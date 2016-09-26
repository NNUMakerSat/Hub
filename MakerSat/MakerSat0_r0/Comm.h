/*
 * Comm.h
 *
 *  Created on: Aug 18, 2016
 *      Author: MKamstra
 */
#include <stdint.h>
#ifndef COMM_H_
#define COMM_H_



void powerUp(uint8_t sci_x_board);
void powerDown(uint8_t sci_x_board);
int Sci_Ready(void);
bool get_state_1_PowerUp(void);
bool get_state_1_Done(void);
void clear_CircularBuffer(void);
void radio_Send(void);

extern uint8_t sci_1_pUp[39];   // Science board 1 ON
extern uint8_t sci_2_pUp[39];   // Science board 2 ON
extern uint8_t sci_3_pUp[39];   // Science board 3 ON
extern uint8_t sci_4_pUp[39];   // Science board 4 ON
extern uint8_t sci_x_pDown[39]; // Science board X OFF

void Start_Timer(void);
bool timeout;
uint8_t timer;



#endif /* COMM_H_ */
