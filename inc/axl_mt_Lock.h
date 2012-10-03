// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_LOCK_H

#include "axl_win_CriticalSection.h"

namespace axl {
namespace mt {

//.............................................................................

class CLock
{
public:
	win::CCriticalSection m_CriticalSection;

public:
	operator CRITICAL_SECTION* ()
	{ 
		return m_CriticalSection;
	}

	void 
	Lock ()
	{
		m_CriticalSection.Enter ();
	}

	void 
	Unlock ()
	{
		m_CriticalSection.Leave ();
	}
};

//.............................................................................

} // namespace mt
} // namespace axl
