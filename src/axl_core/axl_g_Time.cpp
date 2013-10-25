#include "pch.h"
#include "axl_g_Time.h"

namespace axl {
namespace g {

//.............................................................................

uint64_t
GetTimestamp ()
{
	uint64_t Timestamp;

#if (_AXL_ENV == AXL_ENV_WIN)
	GetSystemTimeAsFileTime ((FILETIME*) &Timestamp);
#else
	timespec Time;
	clock_gettime (CLOCK_REALTIME, &Time);
	Timestamp = (uint64_t) Time.tv_sec * 10000000 + Time.tv_nsec / 100;
#endif

	return Timestamp;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
Sleep (uint32_t MsCount)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	::Sleep (MsCount);
#else
	timespec Timespec;
	g::GetTimespecFromTimeout (MsCount, &Timespec);
	nanosleep (&Timespec, NULL);
#endif
}

//.............................................................................

uint64_t
TTime::GetTimestampImpl (
	bool IsLocal,
	int TimeZone
	) const
{
	uint64_t Timestamp = 0;

#if (_AXL_ENV == AXL_ENV_WIN)
	SYSTEMTIME SysTime = { 0 };
	SysTime.wYear   = m_Year;
	SysTime.wMonth  = m_Month;
	SysTime.wDay    = m_MonthDay;
	SysTime.wHour   = m_Hour;
	SysTime.wMinute = m_Minute;
	SysTime.wSecond = m_Second;

	SystemTimeToFileTime (&SysTime, (FILETIME*) &Timestamp);

	if (IsLocal)
		FileTimeToLocalFileTime ((const FILETIME*) &Timestamp, (FILETIME*) &Timestamp);
	else
		Timestamp += (int64_t) GetTimeZoneOffsetInMinutes (TimeZone) * 60 * 10000000;

#else
	tm TmStruct = { 0 };
	TmStruct.tm_year = m_Year;
	TmStruct.tm_mon  = m_Month;
	TmStruct.tm_mday = m_MonthDay;
	TmStruct.tm_hour = m_Hour;
	TmStruct.tm_min  = m_Minute;
	TmStruct.tm_sec  = m_Second;

	time_t PosixTime = IsLocal ?
		mktime (&TmStruct) :
		timegm (&TmStruct) + GetTimeZoneOffsetInMinutes (TimeZone) * 60;

	Timestamp = (uint64_t) PosixTime * 10000000;
#endif

	return
		Timestamp +
		m_MilliSecond * 10000 +
		m_MicroSecond * 10 +
		m_NanoSecond / 100;
}

void
TTime::SetTimestampImpl (
	uint64_t Timestamp,
	bool IsLocal,
	int TimeZone
	)
{
#if (_AXL_ENV == AXL_ENV_WIN)
	SYSTEMTIME SysTime = { 0 };

	if (IsLocal)
		FileTimeToLocalFileTime ((const FILETIME*) &Timestamp, (FILETIME*) &Timestamp);
	else
		Timestamp += (int64_t) GetTimeZoneOffsetInMinutes (TimeZone) * 60 * 10000000;

	FileTimeToSystemTime ((const FILETIME*) &Timestamp, &SysTime);

	m_Year        = SysTime.wYear;
	m_Month       = SysTime.wMonth;
	m_MonthDay    = SysTime.wDay;
	m_DayOfWeek   = SysTime.wDayOfWeek;
	m_Hour        = SysTime.wHour;
	m_Minute      = SysTime.wMinute;
	m_Second      = SysTime.wSecond;

#else
	tm* pTmStruct;

	time_t PosixTime = Timestamp / 10000000;

	if (IsLocal)
	{
		pTmStruct = localtime (&PosixTime);
	}
	else
	{
		PosixTime += GetTimeZoneOffsetInMinutes (TimeZone) * 60;
		pTmStruct = gmtime (&PosixTime);
	}

	m_Year        = pTmStruct->tm_year;
	m_Month       = pTmStruct->tm_mon;
	m_MonthDay    = pTmStruct->tm_mday;
	m_DayOfWeek   = pTmStruct->tm_wday;
	m_Hour        = pTmStruct->tm_hour;
	m_Minute      = pTmStruct->tm_min;
	m_Second      = pTmStruct->tm_sec;
#endif

	m_MilliSecond = (Timestamp / 10000) % 1000;
	m_MicroSecond = (Timestamp / 10) % 1000;
	m_NanoSecond  = (Timestamp % 10) * 100;

}

size_t
TTime::Format (
	rtl::CString* pString,
	const char* pFormat
	) const
{
	static const char* WeekDayShortNameTable [7] =
	{
		"Sun",
		"Mon",
		"Tue",
		"Wed",
		"Thu",
		"Fri",
		"Sat",
	};

	static const char* WeekDayFullNameTable [7] =
	{
		"Sunday",
		"Monday",
		"Tuesday",
		"Wednsday",
		"Thursday",
		"Friday",
		"Saturday",
	};

	static const char* MonthShortNameTable [12] =
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

	static const char* MonthFullNameTable [12] =
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

	pString->Clear ();

	const char* p = pFormat;
	const char* p0 = p;

	for (; *p; p++)
	{
		if (*p != '%')
			continue;

		pString->Append (p0, p - p0);

		p++;
		if (!*p)
			return pString->GetLength ();

		int h12;

		switch (*p)
		{
		case 'h':
			pString->AppendFormat ("%02d", m_Hour);
			break;

		case 'H':
			h12 = m_Hour % 12;
			pString->AppendFormat ("%d", h12 ? h12 : 12);
			break;

		case 'm':
			pString->AppendFormat ("%02d", m_Minute);
			break;

		case 's':
			pString->AppendFormat ("%02d", m_Second);
			break;

		case 'l':
			pString->AppendFormat ("%03d", m_MilliSecond);
			break;

		case 'c':
			pString->AppendFormat ("%03d", m_MicroSecond);
			break;

		case 'p':
			pString->Append (m_Hour >= 12 ? 'p' : 'a');
			break;

		case 'P':
			pString->Append (m_Hour >= 12 ? 'P' : 'A');
			break;

		case 'y':
			pString->AppendFormat ("%02d", (m_Year + 1900) % 100);
			break;

		case 'Y':
			pString->AppendFormat ("%04d", m_Year + 1900);
			break;

		case 'D':
			pString->AppendFormat ("%02d", m_MonthDay);
			break;

		case 'd':
			pString->AppendFormat ("%d", m_MonthDay);
			break;

		case 'M':
			pString->AppendFormat ("%02d", m_Month);
			break;

		case 'o':
			pString->AppendFormat ("%d", m_Month);
			break;

		case 'n':
			pString->Append (MonthShortNameTable [m_Month % 12]);
			break;

		case 'N':
			pString->Append (MonthFullNameTable [m_Month % 12]);
			break;

		case 'w':
			pString->Append (WeekDayShortNameTable [m_DayOfWeek % 7]);
			break;

		case 'W':
			pString->Append (WeekDayFullNameTable [m_DayOfWeek % 7]);
			break;

		default:
			pString->Append (*p);
		}

		p0 = p + 1;
	}

	pString->Append (p0, p - p0);
	return pString->GetLength ();
}

//.............................................................................

} // namespace g
} // namespace axl
