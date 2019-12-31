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

#pragma once

#define _AXL_SYS_WIN_PERFCOUNTER_H

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class PerfCounter
{
protected:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_startTime;

public:
	PerfCounter()
	{
		start();
	}

	bool
	start()
	{
		bool_t result =
			::QueryPerformanceFrequency(&m_frequency) &&
			::QueryPerformanceCounter(&m_startTime);

		return err::complete(result);
	}

	uint64_t
	getElapsedTime()
	{
		LARGE_INTEGER time;
		::QueryPerformanceCounter(&time);
		return ((time.QuadPart - m_startTime.QuadPart) * 1000000 / m_frequency.QuadPart);
	}
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
