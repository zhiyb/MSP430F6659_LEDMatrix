/*
 * macros.h
 *
 *  Created on: 13 Jun 2015
 *      Author: zhiyb
 */

#ifndef MACROS_H_
#define MACROS_H_

#define CONCAT(a,b)	a ## b
#define CONCAT_E(a,b)	CONCAT(a, b)

#define P_IN(p)		p ## IN
#define P_OUT(p)	p ## OUT
#define P_DIR(p)	p ## DIR
#define P_REN(p)	p ## REN
#define P_DS(p)		p ## DS
#define P_SEL(p)	p ## SEL
#define P_IES(p)	p ## IES
#define P_IE(p)		p ## IE
#define P_IFG(p)	p ## IFG

#define PXIN(p)		P_IN(p)
#define PXOUT(p)	P_OUT(p)
#define PXDIR(p)	P_DIR(p)
#define PXREN(p)	P_REN(p)
#define PXDS(p)		P_DS(p)
#define PXSEL(p)	P_SEL(p)
#define PXIES(p)	P_IES(p)
#define PXIE(p)		P_IE(p)
#define PXIFG(p)	P_IFG(p)

#endif /* MACROS_H_ */
