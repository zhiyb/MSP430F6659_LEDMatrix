/*
 * ledmatrix.h
 *
 *  Created on: 13 Jun 2015
 *      Author: zhiyb
 */

#ifndef LEDMATRIX_H_
#define LEDMATRIX_H_

#include <stdint.h>
#include "ascii.h"

#define LEDMATRIX_W	64
#define LEDMATRIX_H	32
#define LEDMATRIX_FPS	160

#define LEDMATRIX_BUFFER8	((uint8_t (*)[2][LEDMATRIX_H][LEDMATRIX_W / 8])ledMatrix::buffer)
#define LEDMATRIX_COLOUR(f, b)	(((f) & ledMatrix::Foreground) | ((b) & ledMatrix::Background))

//#define LEDMATRIX_POOL1S

namespace ledMatrix
{
	enum Colours {Blank = 0x0000, Red = 0x1010, Green = 0x0101,
		TransparentRed = 0x8080, TransparentGreen = 0x4040, Transparent = TransparentRed | TransparentGreen,
		Foreground = 0xFF00, Background = 0x00FF,
		BufferRed = 0, BufferGreen = 1};

	extern struct data_t {
		uint16_t x, y;
		uint16_t clr;
		const struct font_t *font;
	} data;

	void init();
#ifdef LEDMATRIX_POOL1S
	bool pool1s();
#endif
	void clean();
	void fill(uint16_t clr);
	void testPattern(bool inv);
	void swapBuffer();

	static inline uint16_t x() {return data.x;}
	static inline uint16_t y() {return data.y;}
	static inline void setX(uint16_t x) {data.x = x;}
	static inline void setY(uint16_t y) {data.y = y;}
	static inline void setXY(uint16_t x, uint16_t y) {setX(x); setY(y);}
	static inline uint16_t colour() {return data.clr;}
	static inline void setColour(uint16_t clr) {data.clr = clr;}
	bool setFont(uint8_t w, uint8_t h);
	static inline const struct font_t *font(void) {return data.font;}

	void drawImage_aligned(const uint8_t *ptr, const uint16_t w, const uint16_t h);
	void drawChar(const char c);
	static inline void drawNextChar(const char c) {drawChar(c); setX(x() + font()->width);}
	void drawString(const char *str);

	extern uint16_t (*buffer)[2][LEDMATRIX_H][LEDMATRIX_W / 16];
}

#endif /* LEDMATRIX_H_ */
