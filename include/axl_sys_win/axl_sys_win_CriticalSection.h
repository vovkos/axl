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

#define _AXL_SYS_WIN_CRITICALSECTION_H

#include "axl_g_Pch.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

class CriticalSection: public CRITICAL_SECTION
{
public:
	CriticalSection()
	{
		::InitializeCriticalSection(this);
	}

	~CriticalSection()
	{
		::DeleteCriticalSection(this);
	}

	void
	enter()
	{
		::EnterCriticalSection(this);
	}

	void
	leave()
	{
		::LeaveCriticalSection(this);
	}
};

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
