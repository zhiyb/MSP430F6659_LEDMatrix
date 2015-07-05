/*
 * connection.h
 *
 *  Created on: 13 Jun 2015
 *      Author: zhiyb
 */

#ifndef CONNECTION_H_
#define CONNECTION_H_

#include "macros.h"

#define LED_DATA_PORT	P1
// Data lines (R1, R2, G1, G2) from
#define LED_DATA_DATA	0
#define LED_DATA_R1	BIT2
#define LED_DATA_R2	BIT3
#define LED_DATA_G1	BIT0
#define LED_DATA_G2	BIT1
// Row selection from
#define LED_DATA_ROW	4

#define LED_CTRL_PORT	P5
#define LED_CTRL_STB	BIT5
#define LED_CTRL_CLK	BIT4
#define LED_CTRL_EN	BIT3

#endif /* CONNECTION_H_ */
