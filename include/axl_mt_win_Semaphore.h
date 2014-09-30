// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_SEMAPHORE_H

#include "axl_mt_win_WaitableHandle.h"

namespace axl {
namespace mt {
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
		const char* name
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
		const char* name
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
} // namespace mt
} // namespace win
