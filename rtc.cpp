/*
 * rtc.cpp
 *
 *  Created on: 5 Jul 2015
 *      Author: zhiyb
 */

#include <msp430.h>
#include <time.h>
#include "rtc.h"

rtc::time_t rtc::tm;

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

rtc::time_t& rtc::getTime()
{
	// Waiting for RTC module ready for access
	while (!(RTCCTL01 & RTCRDY));
	tm.d[0] = RTCTIM0;
	tm.d[1] = RTCTIM1;
	tm.d[2] = RTCDATE;
	tm.d[3] = RTCYEAR;
	return tm;
}

void rtc::setTime(const time_t *t)
{
	// Waiting for RTC module ready for access
	while (!(RTCCTL01 & RTCRDY));
	RTCTIM0 = t->d[0];
	RTCTIM1 = t->d[1];
	RTCDATE = t->d[2];
	RTCYEAR = t->d[3];
}

void rtc::setTimeFromBin(uint8_t *data)
{
	// Waiting for RTC module ready for access
	while (!(RTCCTL01 & RTCRDY));
	RTCYEAR = bin2bcd((data[0] << 8) | data[1]);
	RTCMON = bin2bcd(data[2]);
	RTCDAY = bin2bcd(data[3]);
	RTCDOW = bin2bcd(data[4]);
	RTCHOUR = bin2bcd(data[5]);
	RTCMIN = bin2bcd(data[6]);
	RTCSEC = bin2bcd(data[7]);
}

void rtc::setTimeFromSecond(uint32_t timeData)
{
	struct tm *time;
	time = localtime(&timeData);
	rtc::time_t t;
	t.i.sec = rtc::bin2bcd(time->tm_sec);
	t.i.min = rtc::bin2bcd(time->tm_min);
	t.i.hour = rtc::bin2bcd(time->tm_hour);
	t.i.dow = rtc::bin2bcd(time->tm_wday);
	t.i.day = rtc::bin2bcd(time->tm_mday);
	t.i.mon = rtc::bin2bcd(time->tm_mon + 1);
	t.i.year = rtc::bin2bcd(time->tm_year + 1900UL);
	setTime(&t);
}
