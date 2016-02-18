// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_SYS_WIN_EVENT_H

#include "axl_sys_win_WaitableHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

class Event: public WaitableHandle
{
public:
	bool 
	create (
		SECURITY_ATTRIBUTES* secAttr = NULL,
		bool isManualReset = false,
		bool isSignalled = false,
		const wchar_t* name = NULL
		)
	{
		close ();

		m_h = ::CreateEventW (secAttr, isManualReset, isSignalled, name);
		return err::complete (m_h != NULL);
	}

	bool 
	open (
		uint_t access = EVENT_ALL_ACCESS,
		bool doInheritHandle = false,
		const wchar_t* name = NULL
		)
	{
		close ();

		m_h = ::OpenEventW (access, doInheritHandle, name);
		return err::complete (m_h != NULL);
	}

	bool
	signal ()
	{ 
		bool_t result = ::SetEvent (m_h);
		return err::complete (result);
	}

	bool
	reset ()
	{ 
		bool_t result = ::ResetEvent (m_h); 
		return err::complete (result);
	}

	bool
	pulse ()
	{ 
		bool_t result = ::PulseEvent (m_h); 
		return err::complete (result);
	}
};

//.............................................................................

} // namespace win
} // namespace sys
} // namespace axl
