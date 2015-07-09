/*
 * display.h
 *
 *  Created on: 5 Jul 2015
 *      Author: zhiyb
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>
#include <msp430.h>
#include "ledmatrix.h"

extern struct cc3000_info_t {
	volatile enum {Disconnected = 0, Connecting = 0x1000, Connected = 0x8000, DHCPSuccess = 0x8800, DHCPFailed = 0x8100} state;
	volatile bool newEvent;
	uint8_t ip[4];
} cc3000;

namespace display
{
	static inline uint8_t bcd2bin(const uint8_t v);
	static inline uint8_t bin2bcd(const uint8_t v);
	static inline void drawBCD(const uint8_t v);
	static inline void drawBCD(const uint16_t v, uint8_t cnt);
	void timeFS();
}

static inline uint8_t display::bcd2bin(const uint8_t v)
{
	BCD2BIN = v;
	return BCD2BIN;
}

static inline uint8_t display::bin2bcd(const uint8_t v)
{
	BIN2BCD = v;
	return BIN2BCD;
}

static inline void display::drawBCD(const uint8_t v)
{
	using namespace ledMatrix;
	drawNextChar('0' + (v >> 4));
	drawNextChar('0' + (v & 0x0F));
}

static inline void display::drawBCD(const uint16_t v, uint8_t cnt)
{
	using namespace ledMatrix;
	while (cnt--)
		drawNextChar('0' + ((v >> (cnt * 4)) & 0x0F));
}

#endif /* DISPLAY_H_ */
