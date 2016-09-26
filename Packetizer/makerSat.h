/*
 * makerSat.h
 *
 *  Created on: Sep 3, 2016
 *      Author: AaronEwing
 */

#ifndef MAKERSAT_H_
#define MAKERSAT_H_

#define BOARD_NAME_POLY
#define BOARD_NAME_RAD
#define BOARD_NAME_HUB

#ifdef BOARD_NAME_POLY
#ifdef BOARD_NAME_RAD
#include "msp430fr5969.h"
#endif

#ifdef BOARD_NAME_HUB
#include "msp430fr6989.h""
#endif

#endif /* MAKERSAT_H_ */
