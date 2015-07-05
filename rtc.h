/*
 * rtc.h
 *
 *  Created on: 5 Jul 2015
 *      Author: zhiyb
 */

#ifndef RTC_H_
#define RTC_H_

#include <stdint.h>
#include <msp430.h>

namespace rtc
{
	union time_t {
		struct info_t {
			uint8_t sec;	// Second
			uint8_t min;	// Minute
			uint8_t hour;	// Hour
			uint8_t dow;	// Day of week
			uint8_t day;	// Day of month
			uint8_t mon;	// Month
			uint16_t year;	// Year
		} i;
		uint16_t d[4];
	};

	void init();
	time_t time();
	static inline uint8_t ps() {return RT1PS;}
	static inline bool ps1Hz() {return ps() & BIT6;}
}

#endif /* RTC_H_ */
