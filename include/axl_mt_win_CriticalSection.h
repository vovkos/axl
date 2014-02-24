// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_CRITICALSECTION_H

#include "axl_g_Def.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

class CCriticalSection: public CRITICAL_SECTION
{
public:
	CCriticalSection ()
	{
		::InitializeCriticalSection (this);
	}

	~CCriticalSection ()
	{
		::DeleteCriticalSection (this);
	}

	void 
	Enter ()
	{
		::EnterCriticalSection (this);
	}

	void 
	Leave ()
	{
		::LeaveCriticalSection (this);
	}
};

//.............................................................................

} // namespace win
} // namespace mt 
} // namespace axl
