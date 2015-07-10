/*
 * ledmatrix.cpp
 *
 *  Created on: 13 Jun 2015
 *      Author: zhiyb
 */

#include <stdint.h>
#include <msp430.h>
#include "connection.h"
#include "clocks.h"
#include "ledmatrix.h"
#include "ascii.h"

__attribute__((section(".TI.noinit"))) static uint8_t row;

namespace ledMatrix
{
	__attribute__((section(".TI.noinit"))) uint16_t (*buffer)[2][LEDMATRIX_H][LEDMATRIX_W / 16];
	__attribute__((section(".TI.noinit"))) uint16_t dispBuffer[2][2][LEDMATRIX_H][LEDMATRIX_W / 16];
#ifdef LEDMATRIX_POOL1S
	static volatile bool sec = false;
#endif
	__attribute__((section(".TI.noinit"))) data_t data;
	static void drawSegment(uint16_t *buf[], const uint16_t disp, const uint16_t mask);
}

void ledMatrix::swapBuffer()
{
	if (!data.dbufEnable)
		return;
	buffer = buffer == &dispBuffer[0] ? &dispBuffer[1] : &dispBuffer[0];
}

void ledMatrix::drawString(const char *str)
{
	while (*str)
		drawNextChar(*str++);
}

bool ledMatrix::setFont(uint8_t w, uint8_t h)
{
	if (font()->width == w && font()->height == h)
		return true;
	const struct font_t *ptr = &fonts;
	while (ptr != 0)
		if (ptr->width == w && ptr->height == h) {
			data.font = ptr;
			return true;
		} else
			ptr = ptr->next;
	return false;
}

void ledMatrix::drawChar(const char c)
{
	drawImage_aligned(font()->ptr + font()->size * (c - font()->offset), font()->width, font()->height);
}

static void ledMatrix::drawSegment(uint16_t *buf[], uint16_t disp, const uint16_t mask)
{
	disp &= mask;
	uint16_t invDisp = ~disp & mask;
	uint16_t b[2];
	b[BufferRed] = (data.clr & Foreground & Red ? disp : 0) | (data.clr & Background & Red ? invDisp : 0);
	b[BufferGreen] = (data.clr & Foreground & Green ? disp : 0) | (data.clr & Background & Green ? invDisp : 0);
	*buf[BufferRed] = (*buf[BufferRed] & ~((data.clr & TransparentRed & Foreground ? disp : mask) & (data.clr & TransparentRed & Background ? invDisp : mask))) | b[BufferRed];
	*buf[BufferGreen] = (*buf[BufferGreen] & ~((data.clr & TransparentGreen & Foreground ? disp : mask) & (data.clr & TransparentGreen & Background ? invDisp : mask))) | b[BufferGreen];
}

void ledMatrix::drawImage_aligned(const uint8_t *ptr, const uint16_t w, const uint16_t h)
{
	uint16_t x = data.x / 16;
	uint16_t *buf[2];
	buf[BufferRed] = &(*buffer)[BufferRed][data.y][x];
	buf[BufferGreen] = &(*buffer)[BufferGreen][data.y][x];
	x = data.x % 16;
	bool aligned = x % 8 == 0;
	uint16_t segments = (x + w - 1) / 16; // Extra segments
	uint16_t mask = 0xFFFF >> x, shift;
	if (segments == 0)
		mask &= 0xFFFF << (16 - x - w);
	else
		shift = x == 0 ? 0 : 16 - x;
	for (uint16_t y = h; y != 0; y--) {
		// Draw first segment
		uint16_t disp = *ptr << 8;
		if (w > 8 && x < 8)
			disp |= *++ptr;
		disp = disp >> x;
		drawSegment(buf, disp, mask);

		if (segments != 0) {
			// Complete segments
			for (uint16_t s = segments - 1; s != 0; s--) {
				// IGNORE FOR NOW
				fill(Red | Green);
			}

			// Last segment
			if (aligned)	// Alignment
				ptr++;
			disp = *ptr;
			uint16_t rem = (x + w) % 16;
			if (8 - shift % 8 < rem)
				disp = (disp << 8) | *++ptr;
			else if (w <= 8)
				disp <<= 8;
			uint16_t mask = 0xFFFF << (16 - rem);
			disp <<= shift;
			buf[BufferRed]++;
			buf[BufferGreen]++;
			drawSegment(buf, disp, mask);
		}

		buf[BufferRed] += LEDMATRIX_W / 16 - segments;
		buf[BufferGreen] += LEDMATRIX_W / 16 - segments;
		ptr++;
	}
}

void ledMatrix::init()
{
	// Data port initialisation
	PXSEL(LED_DATA_PORT) = 0x00;
	PXIE(LED_DATA_PORT) = 0x00;
	PXDIR(LED_DATA_PORT) = 0xFF;
	PXDS(LED_DATA_PORT) = 0xFF;
	PXOUT(LED_DATA_PORT) = 0x00;

	// Control port initialisation
	PXSEL(LED_CTRL_PORT) &= ~(LED_CTRL_STB | LED_CTRL_CLK | LED_CTRL_EN);
	//PXIE(LED_CTRL_PORT) &= ~(LED_CTRL_STB | LED_CTRL_CLK | LED_CTRL_EN);
	PXDIR(LED_CTRL_PORT) |= LED_CTRL_STB | LED_CTRL_CLK | LED_CTRL_EN;
	PXDS(LED_CTRL_PORT) |= LED_CTRL_STB | LED_CTRL_CLK | LED_CTRL_EN;
	PXOUT(LED_CTRL_PORT) &= ~(LED_CTRL_STB | LED_CTRL_CLK | LED_CTRL_EN);

	// Variable initialisation
	row = 0;
	buffer = &dispBuffer[0];
	data.font = &fonts;
	data.dbufEnable = false;
	clean();
	swapBuffer();
	clean();
	setXY(0, 0);
	setColour(Blank);

	// Timer initialisation
	TA0CTL = TASSEL__SMCLK | ID__1 | MC__UP | TACLR | TAIE;
	TA0EX0 = TAIDEX_0;
	TA0CCR0 = SMCLK / (LEDMATRIX_H / 2) / LEDMATRIX_FPS;
	TA0CCTL0 = OUTMOD_0 | CCIE;
}

void ledMatrix::clean()
{
	fill(Blank);
}

void ledMatrix::fill(uint16_t clr)
{
	uint16_t f = clr & Red ? 0xFFFF : 0x0000;
	uint16_t *p = (*buffer)[BufferRed][0];
	for (uint16_t i = LEDMATRIX_H * (LEDMATRIX_W / 16); i != 0; i--)
		*p++ = f;
	f = clr & Green ? 0xFFFF : 0x0000;
	for (uint16_t i = LEDMATRIX_H * (LEDMATRIX_W / 16); i != 0; i--)
		*p++ = f;
}

void ledMatrix::testPattern(bool inv)
{
	uint16_t pattern = inv ? 0x5555 : 0xAAAA;
	uint16_t *p = (*buffer)[BufferRed][0];
	for (uint16_t c = 2; c != 0; c--) {
		for (uint16_t h = LEDMATRIX_H; h != 0; h--) {
			for (uint16_t w = LEDMATRIX_W / 16; w != 0; w--)
				*p++ = pattern;
			pattern ^= 0xFFFF;
		}
		pattern ^= 0xFFFF;
	}
}

#ifdef LEDMATRIX_POOL1S
bool ledMatrix::pool1s()
{
	if (!sec)
		return false;
	sec = false;
	return true;
}
#endif

__attribute__((interrupt(TIMER0_A0_VECTOR)))
void TIMER0_A0_ISR()
{
	using namespace ledMatrix;
	uint16_t (*buff)[2][LEDMATRIX_H][LEDMATRIX_W / 16];
	buff = data.dbufEnable ? buffer == &dispBuffer[0] ? &dispBuffer[1] : &dispBuffer[0] : buffer;

	TA0CCTL0 &= ~CCIFG;	// Clear interrupt flag
	uint8_t prevRow = row;
	row = row == 0 ? (LEDMATRIX_H / 2 - 1) << LED_DATA_ROW : row - (1 << LED_DATA_ROW);

#ifdef LEDMATRIX_POOL1S
	static uint16_t counter = 0;
	if (counter == 0)	// 1 second timer
		sec = true;//P9OUT ^= 0xFF;
	counter = counter == 0 ? LEDMATRIX_H / 2 * LEDMATRIX_FPS - 1 : counter - 1;
#endif

	uint16_t idx = (row >> LED_DATA_ROW) & 0x0F;
	uint16_t *buf[4] = {
		(*buff)[BufferRed][idx], (*buff)[BufferRed][idx + LEDMATRIX_H / 2],
		(*buff)[BufferGreen][idx], (*buff)[BufferGreen][idx + LEDMATRIX_H / 2]
	};
	for (uint16_t w = LEDMATRIX_W / 16; w != 0; w--) {
		uint16_t b[4] = {*buf[0]++, *buf[1]++, *buf[2]++, *buf[3]++};
		for (uint16_t i = 16; i != 0; i--) {
			uint8_t out = prevRow;
			out |= b[0] & 0x8000 ? 0 : LED_DATA_R1;
			out |= b[1] & 0x8000 ? 0 : LED_DATA_R2;
			out |= b[2] & 0x8000 ? 0 : LED_DATA_G1;
			out |= b[3] & 0x8000 ? 0 : LED_DATA_G2;
			PXOUT(LED_DATA_PORT) = out;
			for (uint16_t i = 4; i != 0;)
				b[--i] <<= 1;
			PXOUT(LED_CTRL_PORT) |= LED_CTRL_CLK;
			PXOUT(LED_CTRL_PORT) &= ~LED_CTRL_CLK;
		}
	}

	// Update current row
	PXOUT(LED_CTRL_PORT) |= LED_CTRL_EN;
	PXOUT(LED_CTRL_PORT) |= LED_CTRL_STB;
	PXOUT(LED_DATA_PORT) = row;
	PXOUT(LED_CTRL_PORT) &= ~(LED_CTRL_STB | LED_CTRL_EN);

#if 0
	if (TA0CCTL0 & CCIFG)	// ISR is about to overleap
		for (;;) {
			P9OUT ^= 0xFF;
			__delay_cycles(100 * (MCLK / 1000));
		}
#endif
	TA0CTL &= ~TAIFG;
}
