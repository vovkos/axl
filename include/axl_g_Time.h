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
GetTimestamp ();

void
Sleep (uint32_t MsCount);

//.............................................................................

// time zone is given in ISO 8601 format e.g.: -930, +4, +800, +845

inline
int
GetTimeZoneOffsetInMinutes (int TimeZone)
{
	return TimeZone <= -100 || TimeZone >= 100 ?
		(TimeZone / 100) * 60 + TimeZone % 100 :
		TimeZone * 60;
}

//.............................................................................

struct TTime
{
	uint16_t m_Year;
	uint16_t m_Month;
	uint16_t m_MonthDay;
	uint16_t m_DayOfWeek;
	uint16_t m_Hour;
	uint16_t m_Minute;
	uint16_t m_Second;
	uint16_t m_MilliSecond;
	uint16_t m_MicroSecond;
	uint16_t m_NanoSecond;

public:
	TTime ()
	{
		memset (this, 0, sizeof (TTime));
	}

	TTime (uint64_t Timestamp)
	{
		SetTimestampImpl (Timestamp, true, 0);
	}

	TTime (
		uint64_t Timestamp,
		int TimeZone
		)
	{
		SetTimestampImpl (Timestamp, false, TimeZone);
	}

	uint64_t
	GetTimestamp () const
	{
		return GetTimestampImpl (true, 0);
	}

	uint64_t
	GetTimestamp (int TimeZone) const
	{
		return GetTimestampImpl (false, TimeZone);
	}

	void
	SetTimestamp (uint64_t Timestamp)
	{
		SetTimestampImpl (Timestamp, true, 0);
	}

	void
	SetTimestamp (
		uint64_t Timestamp,
		int TimeZone
		)
	{
		SetTimestampImpl (Timestamp, false, TimeZone);
	}

	size_t
	Format (
		rtl::CString* pString,
		const char* pFormat = "%W %D %N %Y %h:%m:%s"
		) const;

	rtl::CString
	Format (const char* pFormat = "%W %D %N %Y %h:%m:%s") const
	{
		rtl::CString String;
		Format (&String, pFormat);
		return String;
	}

protected:
	uint64_t
	GetTimestampImpl (
		bool IsLocal,
		int TimeZone
		) const;

	void
	SetTimestampImpl (
		uint64_t Timestamp,
		bool IsLocal,
		int TimeZone
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
GetTimespecFromTimeout (
	uint_t Timeout,
	timespec* pTimespec
	)
{
	pTimespec->tv_sec = Timeout / 1000;
	pTimespec->tv_nsec = (Timeout % 1000) * 1000000;
}

inline
void
GetAbsTimespecFromTimeout (
	uint_t Timeout,
	timespec* pTimespec
	)
{
	int Result = clock_gettime (CLOCK_REALTIME, pTimespec);
	ASSERT (Result == 0);

	pTimespec->tv_sec += Timeout / 1000;
	pTimespec->tv_nsec += (Timeout % 1000) * 1000000;
}

#endif

//.............................................................................

} // namespace g
} // namespace axl
