/*
 * Author: Yubo Zhi (normanzyb@gmail.com)
 */

#include "ascii.h"

#ifndef ASCII_STRIPPED
const unsigned char ascii_8x6[99][8] = {
	{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, },
	{0x22, 0x20, 0x20, 0x20, 0x00, 0x00, 0x20, 0x01, },
	{0x52, 0x50, 0x50, 0x00, 0x00, 0x00, 0x00, 0x01, },
	{0x52, 0x50, 0xF8, 0x50, 0xF8, 0x50, 0x50, 0x01, },
	{0x22, 0x78, 0xA0, 0x70, 0x28, 0xF0, 0x20, 0x01, },
	{0xC2, 0xC8, 0x10, 0x20, 0x40, 0x98, 0x18, 0x01, },
	{0x62, 0x90, 0xA0, 0x40, 0xA8, 0x90, 0x68, 0x01, },
	{0x62, 0x20, 0x40, 0x00, 0x00, 0x00, 0x00, 0x01, },
	{0x12, 0x20, 0x40, 0x40, 0x40, 0x20, 0x10, 0x01, },
	{0x42, 0x20, 0x10, 0x10, 0x10, 0x20, 0x40, 0x01, },
	{0x02, 0x20, 0xA8, 0x70, 0xA8, 0x20, 0x00, 0x01, },
	{0x02, 0x20, 0x20, 0xF8, 0x20, 0x20, 0x00, 0x01, },
	{0x02, 0x00, 0x00, 0x00, 0x00, 0x60, 0x20, 0x41, },
	{0x02, 0x00, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x01, },
	{0x02, 0x00, 0x00, 0x00, 0x00, 0x60, 0x60, 0x01, },
	{0x02, 0x08, 0x10, 0x20, 0x40, 0x80, 0x00, 0x01, },
	{0x72, 0x88, 0x98, 0xA8, 0xC8, 0x88, 0x70, 0x01, },
	{0x22, 0x60, 0x20, 0x20, 0x20, 0x20, 0x70, 0x01, },
	{0x72, 0x88, 0x08, 0x10, 0x20, 0x40, 0xF8, 0x01, },
	{0xFA, 0x10, 0x20, 0x10, 0x08, 0x88, 0x70, 0x01, },
	{0x12, 0x30, 0x50, 0x90, 0xF8, 0x10, 0x10, 0x01, },
	{0xFA, 0x80, 0xF0, 0x08, 0x08, 0x88, 0x70, 0x01, },
	{0x32, 0x40, 0x80, 0xF0, 0x88, 0x88, 0x70, 0x01, },
	{0xFA, 0x08, 0x10, 0x20, 0x40, 0x40, 0x40, 0x01, },
	{0x72, 0x88, 0x88, 0x70, 0x88, 0x88, 0x70, 0x01, },
	{0x72, 0x88, 0x88, 0x78, 0x08, 0x10, 0x60, 0x01, },
	{0x02, 0x60, 0x60, 0x00, 0x60, 0x60, 0x00, 0x01, },
	{0x02, 0x60, 0x60, 0x00, 0x60, 0x20, 0x40, 0x01, },
	{0x12, 0x20, 0x40, 0x80, 0x40, 0x20, 0x10, 0x01, },
	{0x02, 0x00, 0xF8, 0x00, 0xF8, 0x00, 0x00, 0x01, },
	{0x42, 0x20, 0x10, 0x08, 0x10, 0x20, 0x40, 0x01, },
	{0x72, 0x88, 0x08, 0x10, 0x20, 0x00, 0x20, 0x01, },
	{0x72, 0x88, 0x08, 0x68, 0xA8, 0xA8, 0x70, 0x01, },
	{0x72, 0x88, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x01, },
	{0xF2, 0x88, 0x88, 0xF0, 0x88, 0x88, 0xF0, 0x01, },
	{0x72, 0x88, 0x80, 0x80, 0x80, 0x88, 0x70, 0x01, },
	{0xE2, 0x90, 0x88, 0x88, 0x88, 0x90, 0xE0, 0x01, },
	{0xFA, 0x80, 0x80, 0xF0, 0x80, 0x80, 0xF8, 0x01, },
	{0xFA, 0x80, 0x80, 0xF0, 0x80, 0x80, 0x80, 0x01, },
	{0x72, 0x88, 0x80, 0xB8, 0x88, 0x88, 0x78, 0x01, },
	{0x8A, 0x88, 0x88, 0xF8, 0x88, 0x88, 0x88, 0x01, },
	{0x72, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x01, },
	{0x3A, 0x10, 0x10, 0x10, 0x10, 0x90, 0x60, 0x01, },
	{0x8A, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 0x88, 0x01, },
	{0x82, 0x80, 0x80, 0x80, 0x80, 0x80, 0xF8, 0x01, },
	{0x8A, 0xD8, 0xA8, 0xA8, 0x88, 0x88, 0x88, 0x01, },
	{0x8A, 0x88, 0xC8, 0xA8, 0x98, 0x88, 0x88, 0x01, },
	{0x72, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x01, },
	{0xF2, 0x88, 0x88, 0xF0, 0x80, 0x80, 0x80, 0x01, },
	{0x72, 0x88, 0x88, 0x88, 0xA8, 0x90, 0x68, 0x01, },
	{0xF2, 0x88, 0x88, 0xF0, 0xA0, 0x90, 0x88, 0x01, },
	{0x7A, 0x80, 0x80, 0x70, 0x08, 0x08, 0xF0, 0x01, },
	{0xFA, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, },
	{0x8A, 0x88, 0x88, 0x88, 0x88, 0x88, 0x70, 0x01, },
	{0x8A, 0x88, 0x88, 0x88, 0x88, 0x50, 0x20, 0x01, },
	{0x8A, 0x88, 0x88, 0xA8, 0xA8, 0xA8, 0x50, 0x01, },
	{0x8A, 0x88, 0x50, 0x20, 0x50, 0x88, 0x88, 0x01, },
	{0x8A, 0x88, 0x88, 0x50, 0x20, 0x20, 0x20, 0x01, },
	{0xFA, 0x08, 0x10, 0x20, 0x40, 0x80, 0xF8, 0x01, },
	{0x72, 0x40, 0x40, 0x40, 0x40, 0x40, 0x70, 0x01, },
	{0x02, 0x80, 0x40, 0x20, 0x10, 0x08, 0x00, 0x01, },
	{0x72, 0x10, 0x10, 0x10, 0x10, 0x10, 0x70, 0x01, },
	{0x22, 0x50, 0x88, 0x00, 0x00, 0x00, 0x00, 0x01, },
	{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x01, },
	{0x42, 0x20, 0x10, 0x00, 0x00, 0x00, 0x00, 0x01, },
	{0x02, 0x00, 0x70, 0x08, 0x78, 0x88, 0x78, 0x01, },
	{0x82, 0x80, 0xB0, 0xC8, 0x88, 0x88, 0xF0, 0x01, },
	{0x02, 0x00, 0x70, 0x80, 0x80, 0x88, 0x70, 0x01, },
	{0x0A, 0x08, 0x68, 0x98, 0x88, 0x88, 0x78, 0x01, },
	{0x02, 0x00, 0x70, 0x88, 0xF8, 0x80, 0x70, 0x01, },
	{0x32, 0x48, 0x40, 0xE0, 0x40, 0x40, 0x40, 0x01, },
	{0x02, 0x78, 0x88, 0x88, 0x78, 0x08, 0x70, 0x01, },
	{0x82, 0x80, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x01, },
	{0x22, 0x00, 0x60, 0x20, 0x20, 0x20, 0x70, 0x01, },
	{0x12, 0x00, 0x30, 0x10, 0x10, 0x90, 0x60, 0x01, },
	{0x82, 0x80, 0x90, 0xA0, 0xC0, 0xA0, 0x90, 0x01, },
	{0x62, 0x20, 0x20, 0x20, 0x20, 0x20, 0x70, 0x01, },
	{0x02, 0x00, 0xD0, 0xA8, 0xA8, 0xA8, 0xA8, 0x01, },
	{0x02, 0x00, 0xB0, 0xC8, 0x88, 0x88, 0x88, 0x01, },
	{0x02, 0x00, 0x70, 0x88, 0x88, 0x88, 0x70, 0x01, },
	{0x02, 0x00, 0xF0, 0x88, 0xF0, 0x80, 0x80, 0x01, },
	{0x02, 0x00, 0x68, 0x98, 0x78, 0x08, 0x08, 0x01, },
	{0x02, 0x00, 0xB0, 0xC8, 0x80, 0x80, 0x80, 0x01, },
	{0x02, 0x00, 0x70, 0x80, 0x70, 0x08, 0xF0, 0x01, },
	{0x42, 0x40, 0xE0, 0x40, 0x40, 0x48, 0x30, 0x01, },
	{0x02, 0x00, 0x88, 0x88, 0x88, 0x98, 0x68, 0x01, },
	{0x02, 0x00, 0x88, 0x88, 0x88, 0x50, 0x20, 0x01, },
	{0x02, 0x00, 0x88, 0x88, 0xA8, 0xA8, 0x50, 0x01, },
	{0x02, 0x00, 0x88, 0x50, 0x20, 0x50, 0x88, 0x01, },
	{0x02, 0x00, 0x88, 0x88, 0x78, 0x08, 0x70, 0x01, },
	{0x02, 0x00, 0xF8, 0x10, 0x20, 0x40, 0xF8, 0x01, },
	{0x12, 0x20, 0x20, 0x40, 0x20, 0x20, 0x10, 0x01, },
	{0x22, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, },
	{0x42, 0x20, 0x20, 0x10, 0x20, 0x20, 0x40, 0x01, },
	{0x02, 0x00, 0x68, 0x90, 0x00, 0x00, 0x00, 0x01, },
	{0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, },
	{0x00, 0x50, 0x50, 0x00, 0x00, 0x88, 0x70, 0x00, },	// Smile, 97/128
	{0x00, 0x50, 0x50, 0x00, 0x00, 0x70, 0x88, 0x00, },	// Sad, 98/129
	{0x00, 0x50, 0x50, 0x00, 0x88, 0xA8, 0x50, 0x00,},	// Cat face, 99/130
};
#else
const unsigned char ascii_8x6[96][6] = {
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
	{0x20, 0x82, 0x08, 0x00, 0x02, 0x00, },
	{0x51, 0x45, 0x00, 0x00, 0x00, 0x00, },
	{0x51, 0x4F, 0x94, 0xF9, 0x45, 0x00, },
	{0x21, 0xEA, 0x1C, 0x2B, 0xC2, 0x00, },
	{0xC3, 0x21, 0x08, 0x42, 0x61, 0x80, },
	{0x62, 0x4A, 0x10, 0xAA, 0x46, 0x80, },
	{0x60, 0x84, 0x00, 0x00, 0x00, 0x00, },
	{0x10, 0x84, 0x10, 0x40, 0x81, 0x00, },
	{0x40, 0x81, 0x04, 0x10, 0x84, 0x00, },
	{0x00, 0x8A, 0x9C, 0xA8, 0x80, 0x00, },
	{0x00, 0x82, 0x3E, 0x20, 0x80, 0x00, },
	{0x00, 0x00, 0x00, 0x01, 0x82, 0x10, },
	{0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, },
	{0x00, 0x00, 0x00, 0x01, 0x86, 0x00, },
	{0x00, 0x21, 0x08, 0x42, 0x00, 0x00, },
	{0x72, 0x29, 0xAA, 0xCA, 0x27, 0x00, },
	{0x21, 0x82, 0x08, 0x20, 0x87, 0x00, },
	{0x72, 0x20, 0x84, 0x21, 0x0F, 0x80, },
	{0xF8, 0x42, 0x04, 0x0A, 0x27, 0x00, },
	{0x10, 0xC5, 0x24, 0xF8, 0x41, 0x00, },
	{0xFA, 0x0F, 0x02, 0x0A, 0x27, 0x00, },
	{0x31, 0x08, 0x3C, 0x8A, 0x27, 0x00, },
	{0xF8, 0x21, 0x08, 0x41, 0x04, 0x00, },
	{0x72, 0x28, 0x9C, 0x8A, 0x27, 0x00, },
	{0x72, 0x28, 0x9E, 0x08, 0x46, 0x00, },
	{0x01, 0x86, 0x00, 0x61, 0x80, 0x00, },
	{0x01, 0x86, 0x00, 0x60, 0x84, 0x00, },
	{0x10, 0x84, 0x20, 0x40, 0x81, 0x00, },
	{0x00, 0x0F, 0x80, 0xF8, 0x00, 0x00, },
	{0x40, 0x81, 0x02, 0x10, 0x84, 0x00, },
	{0x72, 0x20, 0x84, 0x20, 0x02, 0x00, },
	{0x72, 0x20, 0x9A, 0xAA, 0xA7, 0x00, },
	{0x72, 0x28, 0xA2, 0xFA, 0x28, 0x80, },
	{0xF2, 0x28, 0xBC, 0x8A, 0x2F, 0x00, },
	{0x72, 0x28, 0x20, 0x82, 0x27, 0x00, },
	{0xE2, 0x48, 0xA2, 0x8A, 0x4E, 0x00, },
	{0xFA, 0x08, 0x3C, 0x82, 0x0F, 0x80, },
	{0xFA, 0x08, 0x3C, 0x82, 0x08, 0x00, },
	{0x72, 0x28, 0x2E, 0x8A, 0x27, 0x80, },
	{0x8A, 0x28, 0xBE, 0x8A, 0x28, 0x80, },
	{0x70, 0x82, 0x08, 0x20, 0x87, 0x00, },
	{0x38, 0x41, 0x04, 0x12, 0x46, 0x00, },
	{0x8A, 0x4A, 0x30, 0xA2, 0x48, 0x80, },
	{0x82, 0x08, 0x20, 0x82, 0x0F, 0x80, },
	{0x8B, 0x6A, 0xAA, 0x8A, 0x28, 0x80, },
	{0x8A, 0x2C, 0xAA, 0x9A, 0x28, 0x80, },
	{0x72, 0x28, 0xA2, 0x8A, 0x27, 0x00, },
	{0xF2, 0x28, 0xBC, 0x82, 0x08, 0x00, },
	{0x72, 0x28, 0xA2, 0xAA, 0x46, 0x80, },
	{0xF2, 0x28, 0xBC, 0xA2, 0x48, 0x80, },
	{0x7A, 0x08, 0x1C, 0x08, 0x2F, 0x00, },
	{0xF8, 0x82, 0x08, 0x20, 0x82, 0x00, },
	{0x8A, 0x28, 0xA2, 0x8A, 0x27, 0x00, },
	{0x8A, 0x28, 0xA2, 0x89, 0x42, 0x00, },
	{0x8A, 0x28, 0xAA, 0xAA, 0xA5, 0x00, },
	{0x8A, 0x25, 0x08, 0x52, 0x28, 0x80, },
	{0x8A, 0x28, 0x94, 0x20, 0x82, 0x00, },
	{0xF8, 0x21, 0x08, 0x42, 0x0F, 0x80, },
	{0x71, 0x04, 0x10, 0x41, 0x07, 0x00, },
	{0x02, 0x04, 0x08, 0x10, 0x20, 0x00, },
	{0x70, 0x41, 0x04, 0x10, 0x47, 0x00, },
	{0x21, 0x48, 0x80, 0x00, 0x00, 0x00, },
	{0x00, 0x00, 0x00, 0x00, 0x0F, 0x80, },
	{0x40, 0x81, 0x00, 0x00, 0x00, 0x00, },
	{0x00, 0x07, 0x02, 0x7A, 0x27, 0x80, },
	{0x82, 0x0B, 0x32, 0x8A, 0x2F, 0x00, },
	{0x00, 0x07, 0x20, 0x82, 0x27, 0x00, },
	{0x08, 0x26, 0xA6, 0x8A, 0x27, 0x80, },
	{0x00, 0x07, 0x22, 0xFA, 0x07, 0x00, },
	{0x31, 0x24, 0x38, 0x41, 0x04, 0x00, },
	{0x01, 0xE8, 0xA2, 0x78, 0x27, 0x00, },
	{0x82, 0x0B, 0x32, 0x8A, 0x28, 0x80, },
	{0x20, 0x06, 0x08, 0x20, 0x87, 0x00, },
	{0x10, 0x03, 0x04, 0x12, 0x46, 0x00, },
	{0x82, 0x09, 0x28, 0xC2, 0x89, 0x00, },
	{0x60, 0x82, 0x08, 0x20, 0x87, 0x00, },
	{0x00, 0x0D, 0x2A, 0xAA, 0xAA, 0x80, },
	{0x00, 0x0B, 0x32, 0x8A, 0x28, 0x80, },
	{0x00, 0x07, 0x22, 0x8A, 0x27, 0x00, },
	{0x00, 0x0F, 0x22, 0xF2, 0x08, 0x00, },
	{0x00, 0x06, 0xA6, 0x78, 0x20, 0x80, },
	{0x00, 0x0B, 0x32, 0x82, 0x08, 0x00, },
	{0x00, 0x07, 0x20, 0x70, 0x2F, 0x00, },
	{0x41, 0x0E, 0x10, 0x41, 0x23, 0x00, },
	{0x00, 0x08, 0xA2, 0x8A, 0x66, 0x80, },
	{0x00, 0x08, 0xA2, 0x89, 0x42, 0x00, },
	{0x00, 0x08, 0xA2, 0xAA, 0xA5, 0x00, },
	{0x00, 0x08, 0x94, 0x21, 0x48, 0x80, },
	{0x00, 0x08, 0xA2, 0x78, 0x27, 0x00, },
	{0x00, 0x0F, 0x84, 0x21, 0x0F, 0x80, },
	{0x10, 0x82, 0x10, 0x20, 0x81, 0x00, },
	{0x20, 0x82, 0x08, 0x20, 0x82, 0x00, },
	{0x40, 0x82, 0x04, 0x20, 0x84, 0x00, },
	{0x00, 0x06, 0xA4, 0x00, 0x00, 0x00, },
	{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, },
};
#endif
