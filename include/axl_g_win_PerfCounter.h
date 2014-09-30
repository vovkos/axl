// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_G_WIN_PERFCOUNTER_H

namespace axl {
namespace g {
namespace win {

//.............................................................................

class CPerfCounter
{
protected:
	LARGE_INTEGER m_frequency;
	LARGE_INTEGER m_startTime;

public:
	CPerfCounter ()
	{
		start ();
	}

	bool
	start ()
	{ 
		bool_t result = 
			::QueryPerformanceFrequency (&m_frequency) &&
			::QueryPerformanceCounter (&m_startTime);

		return err::complete (result);
	}

	uint64_t 
	getElapsedTime ()
	{ 
		LARGE_INTEGER time;
		::QueryPerformanceCounter (&time);
		return ((time.QuadPart - m_startTime.QuadPart) * 1000000 / m_frequency.QuadPart);
	}
};

//.............................................................................

} // namespace win
} // namespace g
} // namespace axl
