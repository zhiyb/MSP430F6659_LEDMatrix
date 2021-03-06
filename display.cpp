/*
 * display.cpp
 *
 *  Created on: 5 Jul 2015
 *      Author: zhiyb
 */

#include "display.h"
#include "ledmatrix.h"
#include "cc3000tasks.h"
#include "rtc.h"

namespace ledMatrix
{
	const static uint8_t wifi[][8] = {
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0,	// 1
		0x00, 0x00, 0x00, 0x00, 0xC0, 0x20, 0x10, 0x10,	// 2
		0x00, 0x00, 0xE0, 0x10, 0x08, 0x04, 0x04, 0x04,	// 3
		0xF0, 0x08, 0x04, 0x02, 0x01, 0x01, 0x01, 0x01,	// 4
		0xF0, 0x08, 0xE4, 0x12, 0xC9, 0x25, 0x95, 0xD5,	// Full
		0x00, 0x22, 0x14, 0x08, 0x14, 0x22, 0x80, 0xC0,	// X
	};
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
	static const char *weekdays[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
	const rtc::time_t& t = rtc::getTime();
	const bool colon = !rtc::ps1Hz();
	setFont(6, 8);
	setXY(2, 0);
	setColour(LEDMATRIX_COLOUR(Red, Blank));
	drawBCD(t.i.year);
	drawNextChar('-');
	drawBCD(t.i.mon);
	drawNextChar('-');
	drawBCD(t.i.day);

	setXY(2, 8);
	setColour(LEDMATRIX_COLOUR(Green, Blank));
	drawString(weekdays[t.i.dow]);
#if 0
	drawBCD(t.i.hour);
	drawNextChar(colon ? ':' : ' ');
	drawBCD(t.i.min);
	drawNextChar(colon ? ':' : ' ');
	drawBCD(t.i.sec);
	drawNextChar(' ');
	setColour(LEDMATRIX_COLOUR(Red | Green, Blank));
	drawBCD(t.i.dow, 1);
#endif

	setXY(5, 16);
	setColour(LEDMATRIX_COLOUR(Red | Green, Blank));
	setFont(11, 16);
	drawBCD(t.i.hour);
	setFont(10, 16);
	drawNextChar(colon ? ':' : ' ');
	setFont(11, 16);
	drawBCD(t.i.min);

	setXY(LEDMATRIX_W - 8, 0);
	switch (cc3000.state & ~cc3000_info_t::SocketMask & ~0x000F) {
	case cc3000_info_t::Disconnected:
		setColour(LEDMATRIX_COLOUR(Red, Blank));
		drawImage_aligned(wifi[5], 8, 8);
		break;
	case cc3000_info_t::Connecting:
		setColour(LEDMATRIX_COLOUR(Red, Blank));
		drawImage_aligned(wifi[(rtc::ps() >> 5) & 0x03], 8, 8);
		break;
	case cc3000_info_t::Connected:
		setColour(LEDMATRIX_COLOUR(Red, Blank));
		drawImage_aligned(wifi[4], 8, 8);
		break;
	case cc3000_info_t::Connected | cc3000_info_t::DHCPSuccess:
		if ((cc3000.state & cc3000_info_t::SocketMask) == cc3000_info_t::SocketConnected) {
			setColour(LEDMATRIX_COLOUR(Green, Blank));
			drawImage_aligned(wifi[4], 8, 8);
		} else {
			if ((cc3000.state & cc3000_info_t::SocketMask) == cc3000_info_t::SocketConnecting) {
				setColour(LEDMATRIX_COLOUR(Green, Blank));
				drawImage_aligned(wifi[(rtc::ps() >> 5) & 0x03], 8, 8);
			} else {
				setColour(LEDMATRIX_COLOUR(Red | Green, Blank));
				drawImage_aligned(wifi[4], 8, 8);
			}
		}
		break;
	case cc3000_info_t::Connected | cc3000_info_t::DHCPFailed:
		setColour(LEDMATRIX_COLOUR(Red, Blank));
		drawImage_aligned(wifi[5], 8, 8);
		setColour(LEDMATRIX_COLOUR(Red | Transparent, Transparent));
		drawImage_aligned(wifi[4], 8, 8);
		break;
	}

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
