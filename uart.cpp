/*
 * uart.cpp
 *
 *  Created on: 16 Jul 2015
 *      Author: zhiyb
 */

#include <msp430.h>
#include <string.h>
#include <stdio.h>
#include "clocks.h"
#include "macros.h"
#include "uart.h"

#define UART_PORT	P9
#define UART_CLK	BIT1
#define UART_TX		BIT2
#define UART_RX		BIT3

void uart::init()
{
	UCA2CTL1 = UCSWRST;
	UCA2CTL0 = UCMODE_0;
	UCA2CTL1 = UCSSEL__SMCLK | UCSWRST;
	UCA2BRW = SMCLK / BAUD - 1;
	UCA2MCTL = 0;
	UCA2STAT = 0;
	UCA2IRTCTL = 0;
	UCA2IRRCTL = 0;
	UCA2ABCTL = 0;
	UCA2IE = 0;
	//UCA2IFG = 0;
	UCA2CTL1 &= ~UCSWRST;

	PXSEL(UART_PORT) |= UART_RX | UART_TX;
	PXDIR(UART_PORT) |= UART_TX;
	PXDS(UART_PORT) |= UART_TX;
	PXDIR(UART_PORT) &= ~UART_RX;
	PXREN(UART_PORT) |= UART_RX;
	PXOUT(UART_PORT) |= UART_RX;
}

void uart::puts(const char *str)
{
	while (*str)
		putc(*str++);
}

// Output to stdout
extern "C" {
	int puts(const char *_ptr)
	{
		uart::puts(_ptr);
		uart::putc('\r');
		uart::putc('\n');
		return strlen(_ptr);
	}

	int putc(int _c, register FILE *_fp)
	{
		uart::putc(_c);
		return _c;
	}

	int fputs(const char *_ptr, register FILE *_fp)
	{
		uart::puts(_ptr);
		return strlen(_ptr);
	}

	int fputc(int _c, register FILE *_fp)
	{
		uart::putc(_c);
		return _c;
	}
}
