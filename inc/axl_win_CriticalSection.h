// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_CRITICALSECTION_H

#include "axl_g_Def.h"

namespace axl {
namespace win {

//.............................................................................

class CCriticalSection
{
protected:
	CRITICAL_SECTION m_CriticalSection;

public:
	CCriticalSection ()
	{
		::InitializeCriticalSection (&m_CriticalSection);
	}

	~CCriticalSection ()
	{
		::DeleteCriticalSection (&m_CriticalSection);
	}

	operator CRITICAL_SECTION* ()
	{ 
		return &m_CriticalSection; 
	}

	void 
	Enter ()
	{
		::EnterCriticalSection (&m_CriticalSection);
	}

	void 
	Leave ()
	{
		::LeaveCriticalSection (&m_CriticalSection);
	}
};

//.............................................................................

} // namespace win {
} // namespace axl {
