/*
 * display.cpp
 *
 *  Created on: 5 Jul 2015
 *      Author: zhiyb
 */

#include "display.h"
#include "ledmatrix.h"
#include "rtc.h"

using namespace ledMatrix;

void display::drawBCD(const uint8_t v)
{
	drawNextChar('0' + (v >> 4));
	drawNextChar('0' + (v & 0x0F));
}

void display::drawBCD(const uint16_t v, uint8_t cnt)
{
	while (cnt--)
		drawNextChar('0' + ((v >> (cnt * 4)) & 0x0F));
}

void display::drawNextChar(const char c)
{
	ledMatrix::drawChar(c);
	setX(x() + 6);
}

void display::timeFS()
{
	const rtc::time_t t = rtc::time();
	const bool colon = !rtc::ps1Hz();
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
}
