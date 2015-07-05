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
#define LEDMATRIX_FPS	160

#define LEDMATRIX_BUFFER8	((uint8_t (*)[2][LEDMATRIX_H][LEDMATRIX_W / 8])ledMatrix::buffer)
#define LEDMATRIX_COLOUR(f, b)	(((f) & ledMatrix::Foreground) | ((b) & ledMatrix::Background))

//#define LEDMATRIX_POOL1S

namespace ledMatrix
{
	enum Colours {Blank = 0x0000, Red = 0x00FF, Green = 0xFF00,
		Foreground = 0xF0F0, Background = 0x0F0F,
		BufferRed = 0, BufferGreen = 1};

	extern struct data_t {
		uint16_t x, y;
		uint16_t clr;
	} data;

	void init();
#ifdef LEDMATRIX_POOL1S
	bool pool1s();
#endif
	void clean();
	void fill(uint16_t clr);
	void testPattern(bool inv);

	static inline uint16_t x() {return data.x;}
	static inline uint16_t y() {return data.y;}
	static inline void setX(uint16_t x) {data.x = x;}
	static inline void setY(uint16_t y) {data.y = y;}
	static inline void setXY(uint16_t x, uint16_t y) {setX(x); setY(y);}
	static inline uint16_t colour() {return data.clr;}
	static inline void setColour(uint16_t clr) {data.clr = clr;}

	void drawChar(const char c);
	void drawString(const char *str);
	void swapBuffer();

	extern uint16_t (*buffer)[2][LEDMATRIX_H][LEDMATRIX_W / 16];
}

#endif /* LEDMATRIX_H_ */
