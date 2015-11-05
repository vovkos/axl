// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_CRITICALSECTION_H

#include "axl_g_Def.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

class CriticalSection: public CRITICAL_SECTION
{
public:
	CriticalSection ()
	{
		::InitializeCriticalSection (this);
	}

	~CriticalSection ()
	{
		::DeleteCriticalSection (this);
	}

	void 
	enter ()
	{
		::EnterCriticalSection (this);
	}

	void 
	leave ()
	{
		::LeaveCriticalSection (this);
	}
};

//.............................................................................

} // namespace win
} // namespace mt 
} // namespace axl
