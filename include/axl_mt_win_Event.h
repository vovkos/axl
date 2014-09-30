// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_EVENT_H

#include "axl_mt_win_WaitableHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
namespace win {

//.............................................................................

class Event: public WaitableHandle
{
public:
	bool 
	create (
		SECURITY_ATTRIBUTES* secAttr,
		bool isManualReset,
		bool isSignalled,
		const wchar_t* name
		)
	{
		close ();

		m_h = ::CreateEventW (secAttr, isManualReset, isSignalled, name);
		return err::complete (m_h != NULL);
	}

	bool 
	open (
		uint_t access,
		bool doInheritHandle,
		const wchar_t* name
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
} // namespace mt 
} // namespace axl
