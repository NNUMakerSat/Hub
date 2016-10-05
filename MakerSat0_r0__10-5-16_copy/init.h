/*
 * init.h
 *
 *  Created on: Aug 17, 2016
 *      Author: MKamstra
 */

#ifndef INIT_H_
#define INIT_H_

#define RED BIT5
#define GREEN BIT6
#define BLUE BIT7

#define SCI_1_SEL BIT4
#define SCI_2_SEL BIT5
#define SCI_3_SEL BIT6
#define SCI_4_SEL BIT7

void Init_LED(void);
void Init_Timer(void);
void Init_Port(void);
void Init_Comm(void);
void Init_ADC(void);
void Init_Var(void);



#endif /* INIT_H_ */
