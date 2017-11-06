//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_sys_Time.h"
#include "axl_sl_CallOnce.h"

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

typedef 
void
WINAPI 
GetSystemTimePreciseAsFileTimeFunc (FILETIME* time);

GetSystemTimePreciseAsFileTimeFunc* systemTimePreciseAsFileTime = NULL;

void
initPreciseTimestamps ()
{
	HMODULE kernel32 = ::GetModuleHandleW (L"kernel32.dll");
	ASSERT (kernel32);

	systemTimePreciseAsFileTime = (GetSystemTimePreciseAsFileTimeFunc*) ::GetProcAddress (kernel32, "GetSystemTimePreciseAsFileTime");
	if (!systemTimePreciseAsFileTime)
		systemTimePreciseAsFileTime = ::GetSystemTimeAsFileTime;
}

#elif (_AXL_OS_DARWIN)

void
initPreciseTimestamps ()
{
	// need to experiment with mach_absolute_time/mach_timebase_info and clock_get_time/host_get_clock_service
}

#else

void
initPreciseTimestamps ()
{
	// nothing to do here
}

#endif

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

uint64_t
getTimestamp ()
{
	uint64_t timestamp;

#if (_AXL_OS_WIN)
	::GetSystemTimeAsFileTime ((FILETIME*) &timestamp);
#elif (_AXL_OS_POSIX)
	timespec tspec;
#	if (_AXL_OS_DARWIN)
	timeval tval;
	gettimeofday (&tval, NULL);
	tspec.tv_sec = tval.tv_sec;
	tspec.tv_nsec = tval.tv_usec * 1000;
#	elif (defined CLOCK_REALTIME_COARSE)
	clock_gettime (CLOCK_REALTIME_COARSE, &tspec);
#	else
	clock_gettime (CLOCK_REALTIME, &tspec);
#	endif
	timestamp = (uint64_t) (tspec.tv_sec + AXL_SYS_EPOCH_DIFF) * 10000000 + tspec.tv_nsec / 100;
#endif

	return timestamp;
}

uint64_t
getPreciseTimestamp ()
{
	uint64_t timestamp;

#if (_AXL_OS_WIN)
	systemTimePreciseAsFileTime ((FILETIME*) &timestamp);
#elif (_AXL_OS_POSIX)
	timespec tspec;
#	if (_AXL_OS_DARWIN)
	timeval tval;
	gettimeofday (&tval, NULL);
	tspec.tv_sec = tval.tv_sec;
	tspec.tv_nsec = tval.tv_usec * 1000;
#	else
	clock_gettime (CLOCK_REALTIME, &tspec);
#	endif
	timestamp = (uint64_t) (tspec.tv_sec + AXL_SYS_EPOCH_DIFF) * 10000000 + tspec.tv_nsec / 100;
#endif

	return timestamp;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
sleep (uint32_t msCount)
{
#if (_AXL_OS_WIN)
	::Sleep (msCount);
#else
	timespec timespec;
	getTimespecFromTimeout (msCount, &timespec);
	nanosleep (&timespec, NULL);
#endif
}

//..............................................................................

uint64_t
Time::getTimestampImpl (
	bool isLocal,
	int timeZone
	) const
{
	uint64_t timestamp = 0;

#if (_AXL_OS_WIN)
	SYSTEMTIME sysTime = { 0 };
	sysTime.wYear   = m_year;
	sysTime.wMonth  = m_month + 1;
	sysTime.wDay    = m_monthDay;
	sysTime.wHour   = m_hour;
	sysTime.wMinute = m_minute;
	sysTime.wSecond = m_second;

	::SystemTimeToFileTime (&sysTime, (FILETIME*) &timestamp);

	if (isLocal)
		::FileTimeToLocalFileTime ((const FILETIME*) &timestamp, (FILETIME*) &timestamp);
	else
		timestamp += (int64_t) getTimeZoneOffsetInMinutes (timeZone) * 60 * 10000000;

#else
	tm tmStruct = { 0 };
	tmStruct.tm_year = m_year - 1900;
	tmStruct.tm_mon  = m_month;
	tmStruct.tm_mday = m_monthDay;
	tmStruct.tm_hour = m_hour;
	tmStruct.tm_min  = m_minute;
	tmStruct.tm_sec  = m_second;

	time_t posixTime = isLocal ?
		mktime (&tmStruct) :
		timegm (&tmStruct) + getTimeZoneOffsetInMinutes (timeZone) * 60;

	timestamp = (uint64_t) (posixTime + AXL_SYS_EPOCH_DIFF) * 10000000;
#endif

	return
		timestamp +
		m_milliSecond * 10000 +
		m_microSecond * 10 +
		m_nanoSecond / 100;
}

void
Time::setTimestampImpl (
	uint64_t timestamp,
	bool isLocal,
	int timeZone
	)
{
#if (_AXL_OS_WIN)
	SYSTEMTIME sysTime = { 0 };

	if (isLocal)
		::FileTimeToLocalFileTime ((const FILETIME*) &timestamp, (FILETIME*) &timestamp);
	else
		timestamp += (int64_t) getTimeZoneOffsetInMinutes (timeZone) * 60 * 10000000;

	::FileTimeToSystemTime ((const FILETIME*) &timestamp, &sysTime);

	m_year        = sysTime.wYear;
	m_month       = sysTime.wMonth - 1;
	m_monthDay    = sysTime.wDay;
	m_dayOfWeek   = sysTime.wDayOfWeek;
	m_hour        = sysTime.wHour;
	m_minute      = sysTime.wMinute;
	m_second      = sysTime.wSecond;

#else
	tm* tmStruct;

	time_t posixTime = timestamp / 10000000 - AXL_SYS_EPOCH_DIFF;

	if (isLocal)
	{
		tmStruct = localtime (&posixTime);
	}
	else
	{
		posixTime += getTimeZoneOffsetInMinutes (timeZone) * 60;
		tmStruct = gmtime (&posixTime);
	}

	m_year        = tmStruct->tm_year + 1900;
	m_month       = tmStruct->tm_mon;
	m_monthDay    = tmStruct->tm_mday;
	m_dayOfWeek   = tmStruct->tm_wday;
	m_hour        = tmStruct->tm_hour;
	m_minute      = tmStruct->tm_min;
	m_second      = tmStruct->tm_sec;
#endif

	m_milliSecond = (timestamp / 10000) % 1000;
	m_microSecond = (timestamp / 10) % 1000;
	m_nanoSecond  = (timestamp % 10) * 100;
}

/*
	%h	24-hour
	%H	12-hour
	%m	minute
	%s	second
	%l	millisecond
	%c	microsecond
	%p	p/a (lower-case pm/am)
	%P	P/A (upper-case PM/AM)
	%y	2-digit year
	%Y	4-digit year
	%D	day of month (leading zero if one-digit)
	%d	day of month (no leading-zero)
	%M	month number (leading zero if one-digit)
	%o	month number (no leading-zero)
	%n	month name (short)
	%N	month name (full)
	%w	day of week (short)
	%W	day of week (full)
*/

size_t
Time::format (
	sl::String* string,
	const sl::StringRef& formatString
	) const
{
	static const char* weekDayShortNameTable [7] =
	{
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
	};

	static const char* weekDayFullNameTable [7] =
	{
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednsday",
		"Thursday",
		"Friday",
		"Saturday",
	};

	static const char* monthShortNameTable [12] =
	{
		"Jan",
		"Feb",
		"Mar",
		"Apr",
		"May",
		"Jun",
		"Jul",
		"Aug",
		"Sep",
		"Oct",
		"Nov",
		"Dec",
	};

	static const char* monthFullNameTable [12] =
	{
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December",
	};

	string->clear ();

	const char* p = formatString.cp ();
	const char* end = formatString.getEnd ();
	const char* p0 = p;

	for (; p < end; p++)
	{
		if (*p != '%')
			continue;

		string->append (p0, p - p0);

		p++;
		if (p >= end)
			return string->getLength ();

		int h12;

		switch (*p)
		{
		case 'h':
			string->appendFormat ("%02d", m_hour);
			break;

		case 'H':
			h12 = m_hour % 12;
			string->appendFormat ("%d", h12 ? h12 : 12);
			break;

		case 'm':
			string->appendFormat ("%02d", m_minute);
			break;

		case 's':
			string->appendFormat ("%02d", m_second);
			break;

		case 'l':
			string->appendFormat ("%03d", m_milliSecond);
			break;

		case 'c':
			string->appendFormat ("%03d", m_microSecond);
			break;

		case 'p':
			string->append (m_hour >= 12 ? 'p' : 'a');
			break;

		case 'P':
			string->append (m_hour >= 12 ? 'P' : 'A');
			break;

		case 'y':
			string->appendFormat ("%02d", m_year % 100);
			break;

		case 'Y':
			string->appendFormat ("%04d", m_year);
			break;

		case 'D':
			string->appendFormat ("%02d", m_monthDay);
			break;

		case 'd':
			string->appendFormat ("%d", m_monthDay);
			break;

		case 'M':
			string->appendFormat ("%02d", m_month + 1);
			break;

		case 'o':
			string->appendFormat ("%d", m_month + 1);
			break;

		case 'n':
			string->append (monthShortNameTable [m_month % 12]);
			break;

		case 'N':
			string->append (monthFullNameTable [m_month % 12]);
			break;

		case 'w':
			string->append (weekDayShortNameTable [m_dayOfWeek % 7]);
			break;

		case 'W':
			string->append (weekDayFullNameTable [m_dayOfWeek % 7]);
			break;

		default:
			string->append (*p);
		}

		p0 = p + 1;
	}

	string->append (p0, p - p0);
	return string->getLength ();
}

//..............................................................................

#if (_AXL_OS_POSIX)

void
getAbsTimespecFromTimeout (
	uint_t timeout,
	timespec* tspec
	)
{
#if (_AXL_OS_DARWIN)
	timeval tval;
	gettimeofday (&tval, NULL);
	tspec->tv_sec = tval.tv_sec;
	tspec->tv_nsec = tval.tv_usec * 1000;
#else
	int result = clock_gettime (CLOCK_REALTIME, tspec);
	ASSERT (result == 0);
#endif

	uint64_t nsec = tspec->tv_nsec + (uint64_t) (timeout % 1000) * 1000000;
	tspec->tv_sec += timeout / 1000 + nsec / 1000000000ULL;
	tspec->tv_nsec = nsec % 1000000000ULL;
}

#endif

//..............................................................................

} // namespace sys
} // namespace axl
