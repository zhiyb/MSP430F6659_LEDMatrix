/*
 * Author: Yubo Zhi (normanzyb@gmail.com)
 */

#ifndef ASCII_H
#define ASCII_H

#ifdef __cplusplus
extern "C" {
#endif

struct font_t {
	const unsigned char width, height, size, offset;
	const unsigned char *ptr;
	const struct font_t *next;
};

extern const struct font_t fonts;

#ifdef __cplusplus
}
#endif

#endif
