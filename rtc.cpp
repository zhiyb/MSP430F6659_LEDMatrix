/*
 * rtc.cpp
 *
 *  Created on: 5 Jul 2015
 *      Author: zhiyb
 */

#include <msp430.h>
#include "rtc.h"

void rtc::init()
{
	// Initialise 32.768 kHz XTAL1
	UCSCTL6_L = (UCSCTL6_L & SMCLKOFF_L) | XT1DRIVE_3 | XCAP_1;

	// Initialise RTC module
	while (!(RTCCTL01 & RTCRDY));
	RTCCTL01 = RTCBCD | RTCHOLD | RTCTEV__MIN;
	RTCCTL23 = RTCCALF_3;
	RTCTIM0 = 0;
	RTCTIM1 = 0;
	RTCDATE = 0;
	RTCYEAR = 0;
	RTCAMINHR = 0;
	RTCADOWDAY = 0;
	RTCPS0CTL = RT0IP__256;
	RTCPS1CTL = RT1IP__64;

	// RTC clock output port
	P5SEL |= 1 << 7;
	P5DIR |= 1 << 7;
	P5DS |= 1 << 7;

	// Start RTC
	RTCCTL01 &= ~RTCHOLD;

	// Erase LOCKBAK bit for crystal + RTC settings to take effect
	while(BAKCTL & LOCKBAK)
		BAKCTL &= ~LOCKBAK;
}

rtc::time_t rtc::time()
{
	time_t t;
	// Waiting for RTC module ready for access
	while (!(RTCCTL01 & RTCRDY));
	t.d[0] = RTCTIM0;
	t.d[1] = RTCTIM1;
	t.d[2] = RTCDATE;
	t.d[3] = RTCYEAR;
	return t;
}

void rtc::setTimeFromBin(uint8_t *data)
{
	// Waiting for RTC module ready for access
	while (!(RTCCTL01 & RTCRDY));
	// Suspend RTC module
	//RTCCTL01 |= RTCHOLD;

	RTCYEAR = bin2bcd((data[0] << 8) | data[1]);
	RTCMON = bin2bcd(data[2]);
	RTCDAY = bin2bcd(data[3]);
	RTCDOW = bin2bcd(data[4]);
	RTCHOUR = bin2bcd(data[5]);
	RTCMIN = bin2bcd(data[6]);
	RTCSEC = bin2bcd(data[7]);

	// Resume RTC
	//RTCCTL01 &= ~RTCHOLD;
}
