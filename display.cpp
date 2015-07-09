/*
 * display.cpp
 *
 *  Created on: 5 Jul 2015
 *      Author: zhiyb
 */

#include "display.h"
#include "ledmatrix.h"
#include "rtc.h"

namespace ledMatrix
{
#if 0
	const static uint8_t texture[] = {		// Debug texture
		0xAA, 0xAA, 0x4C, 0xCD, 0x80, 0x02, 0x00, 0x03, 0xC1, 0x80, 0x43, 0xC1, 0x87, 0xE2, 0x0D, 0xB3,
		0xC9, 0x90, 0x41, 0x81, 0x81, 0x82, 0x19, 0x83, 0xD8, 0x00, 0x40, 0x01, 0xB3, 0x32, 0x55, 0x55,
	};
#endif
}

using namespace ledMatrix;

void display::timeFS()
{
	const rtc::time_t t = rtc::time();
	const bool colon = !rtc::ps1Hz();
	setFont(6, 8);
	setXY(2, 0);
	setColour(LEDMATRIX_COLOUR(Red, Blank));
	drawBCD(t.i.year, 4);
	drawNextChar('-');
	drawBCD(t.i.mon);
	drawNextChar('-');
	drawBCD(t.i.day);

	setXY(2, 8);
	setColour(LEDMATRIX_COLOUR(Green, Blank));
	drawBCD(t.i.hour);
	drawNextChar(colon ? ':' : ' ');
	drawBCD(t.i.min);
	drawNextChar(colon ? ':' : ' ');
	drawBCD(t.i.sec);
	drawNextChar(' ');
	setColour(LEDMATRIX_COLOUR(Red | Green, Blank));
	drawBCD(t.i.dow, 1);

	setXY(t.i.sec & 0x0F, 16);
	setColour(LEDMATRIX_COLOUR(Red | Green, Blank));
	setFont(11, 16);
	drawBCD(t.i.hour);
	setFont(10, 16);
	drawNextChar(colon ? ':' : ' ');
	setFont(11, 16);
	drawBCD(t.i.min);

#if 0
	setXY(t.i.sec & 0x0F, 16);
	setColour(LEDMATRIX_COLOUR(Green | TransparentRed, TransparentRed));
	drawImage_aligned(texture, 16, 16);
	setXY(x() + 16 + 2, 16);
	drawImage_aligned(texture, 16, 16);
	setXY(x() + 16 + 2, 16);
	drawImage_aligned(texture, 16, 16);
#endif
}
