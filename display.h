/*
 * display.h
 *
 *  Created on: 5 Jul 2015
 *      Author: zhiyb
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <stdint.h>

namespace display
{
	void drawBCD(const uint8_t v);
	void drawBCD(const uint16_t v, uint8_t cnt);
	void drawNextChar(const char c);
	void timeFS();
}

#endif /* DISPLAY_H_ */
