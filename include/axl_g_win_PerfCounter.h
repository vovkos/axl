// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
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
	LARGE_INTEGER m_Frequency;
	LARGE_INTEGER m_StartTime;

public:
	CPerfCounter ()
	{
		Start ();
	}

	bool
	Start ()
	{ 
		bool_t Result = 
			::QueryPerformanceFrequency (&m_Frequency) &&
			::QueryPerformanceCounter (&m_StartTime);

		return err::Complete (Result);
	}

	uint64_t 
	GetElapsedTime ()
	{ 
		LARGE_INTEGER Time;
		::QueryPerformanceCounter (&Time);
		return ((Time.QuadPart - m_StartTime.QuadPart) * 1000000 / m_Frequency.QuadPart);
	}
};

//.............................................................................

} // namespace win
} // namespace g
} // namespace axl
