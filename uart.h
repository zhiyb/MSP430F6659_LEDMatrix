/*
 * uart.h
 *
 *  Created on: 16 Jul 2015
 *      Author: zhiyb
 */

#ifndef UART_H_
#define UART_H_

#include <msp430.h>

namespace uart
{
	extern void init();
	static inline char rx() {while (!(UCA2IFG & UCRXIFG)); return UCA2RXBUF;}
	static inline void tx(const char c) {while (!(UCA2IFG & UCTXIFG)); UCA2TXBUF = c;}
	static inline void putchar(const char c) {tx(c);}
	void puts(const char *str);
}

#endif /* UART_H_ */
