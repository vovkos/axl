// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_WIN_SEMAPHORE_H

#include "axl_sys_win_WaitableHandle.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

class Semaphore: public WaitableHandle
{
public:
	bool 
	create (
		SECURITY_ATTRIBUTES* secAttr,
		uint_t initialCount,
		uint_t maxCount,
		const sl::StringRef& name = NULL
		)
	{
		close ();

		m_h = ::CreateSemaphore (secAttr, initialCount, maxCount, name);
		return err::complete (m_h != NULL);
	}

	bool 
	open (
		uint_t access,
		bool doInheritHandle,
		const sl::StringRef& name
		)
	{
		close ();

		m_h = ::OpenSemaphore (access, doInheritHandle, name);
		return err::complete (m_h != NULL);
	}

	bool
	signal (uint_t count = 1)
	{
		bool_t result = ::ReleaseSemaphore (m_h, count, NULL);
		return err::complete (result);
	}
};

//.............................................................................

} // namespace axl
} // namespace sys
} // namespace win
