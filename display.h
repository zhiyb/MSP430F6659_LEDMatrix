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

#include <data_types.h>
#include <socket.h>

namespace display
{
	static inline void drawBCD(const uint8_t v);
	static inline void drawBCD(const uint16_t v, uint8_t cnt);
	void timeFS();
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
