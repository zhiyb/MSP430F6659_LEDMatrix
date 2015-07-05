/*
 * ledmatrix.h
 *
 *  Created on: 13 Jun 2015
 *      Author: zhiyb
 */

#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_

#include <stdint.h>

#define LEDMATRIX_W	64
#define LEDMATRIX_H	32
#define LEDMATRIX_FPS	120

namespace ledMatrix
{
	enum Colours {Blank = 0x0000, Red = 0x00FF, Green = 0xFF00,
		Foreground = 0xF0F0, Background = 0x0F0F,
		BufferRed = 0, BufferGreen = 1};

	void init();
	bool poolOneSecond();
	void clean();
	void fill(uint16_t clr);
	void testPattern(bool inv);

	void setX(uint16_t x);
	void setY(uint16_t y);
	void setXY(uint16_t x, uint16_t y);
	void setColour(uint16_t clr);

	void drawChar(const char c);
	void drawString(const char *str);
	void swapBuffer();

	extern uint16_t (*buffer)[2][LEDMATRIX_H][LEDMATRIX_W / 16];
}

#endif /* LEDMATRIX_H_ */
