/*
 * Author: Yubo Zhi (yz39g13@soton.ac.uk)
 */

#ifndef ASCII_H
#define ASCII_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ASCII_STRIPPED
#define ASCII_SIZE	99
extern const unsigned char ascii_8x6[ASCII_SIZE][8];
#else
extern const unsigned char ascii_8x6[96][6];
#endif

#ifdef __cplusplus
}
#endif

#endif
