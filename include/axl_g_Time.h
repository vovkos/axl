// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_TIME_H

#include "axl_rtl_String.h"

namespace axl {
namespace g {

//.............................................................................

// timestamp is Windows FILETIME: number of 100-nanosecond intervals since January 1, 1601 (UTC)
// note that Windows FILETIME epoch is different from POSIX time epoch which is January 1, 1970

uint64_t
getTimestamp ();

uint64_t
getPreciseTimestamp ();

void
sleep (uint32_t msCount);

//.............................................................................

// time zone is given in ISO 8601 format e.g.: -930, +4, +800, +845

inline
int
getTimeZoneOffsetInMinutes (int timeZone)
{
	return timeZone <= -100 || timeZone >= 100 ?
		(timeZone / 100) * 60 + timeZone % 100 :
		timeZone * 60;
}

//.............................................................................

struct Time
{
	uint16_t m_year;
	uint16_t m_month;
	uint16_t m_monthDay;
	uint16_t m_dayOfWeek;
	uint16_t m_hour;
	uint16_t m_minute;
	uint16_t m_second;
	uint16_t m_milliSecond;
	uint16_t m_microSecond;
	uint16_t m_nanoSecond;

public:
	Time ()
	{
		memset (this, 0, sizeof (Time));
	}

	Time (uint64_t timestamp)
	{
		setTimestampImpl (timestamp, true, 0);
	}

	Time (
		uint64_t timestamp,
		int timeZone
		)
	{
		setTimestampImpl (timestamp, false, timeZone);
	}

	uint64_t
	getTimestamp () const
	{
		return getTimestampImpl (true, 0);
	}

	uint64_t
	getTimestamp (int timeZone) const
	{
		return getTimestampImpl (false, timeZone);
	}

	void
	setTimestamp (uint64_t timestamp)
	{
		setTimestampImpl (timestamp, true, 0);
	}

	void
	setTimestamp (
		uint64_t timestamp,
		int timeZone
		)
	{
		setTimestampImpl (timestamp, false, timeZone);
	}

	size_t
	format (
		rtl::String* string,
		const char* formatString = "%W %D %N %Y %h:%m:%s"
		) const;

	rtl::String
	format (const char* formatString = "%W %D %N %Y %h:%m:%s") const
	{
		rtl::String string;
		format (&string, formatString);
		return string;
	}

protected:
	uint64_t
	getTimestampImpl (
		bool isLocal,
		int timeZone
		) const;

	void
	setTimestampImpl (
		uint64_t timestamp,
		bool isLocal,
		int timeZone
		);
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// format specifiers differ slightly from strftime ()
//
//   %h -- hour in 24-hour format (00..24)
//   %H -- hour in 12-hour format (1..12)
//   %m -- minutes (00..59)
//   %s -- seconds (00..59)
//   %l -- milliseconds (0..999)
//   %c -- microseconds (0..999)
//   %p -- AM / PM designition lower case (a / p)
//   %p -- AM / PM designition upper case (A / P)
//   %y -- year, last 2-digit format (0..99)
//   %Y -- year, all digits
//   %D -- day of month, leading zero (01..31)
//   %d -- day of month, no leading zero (1..31)
//   %M -- month index, leading zero (01..12)
//   %o -- month index, no leading zero (1..12)
//   %n -- month short name (Jan..Dec)
//   %N -- month full name (January..December)
//   %w -- week day short name (Sun..Sat)
//   %W -- week day full name (Sunday..Saturday)

//.............................................................................

#if (_AXL_ENV == AXL_ENV_POSIX)

inline
void
getTimespecFromTimeout (
	uint_t timeout,
	timespec* tspec
	)
{
	tspec->tv_sec = timeout / 1000;
	tspec->tv_nsec = (timeout % 1000) * 1000000;
}

inline
void
getAbsTimespecFromTimeout (
	uint_t timeout,
	timespec* tspec
	)
{
	int result = clock_gettime (CLOCK_REALTIME, tspec);
	ASSERT (result == 0);

	tspec->tv_sec += timeout / 1000;
	tspec->tv_nsec += (timeout % 1000) * 1000000;
}

#endif

//.............................................................................

} // namespace g
} // namespace axl
