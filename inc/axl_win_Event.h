// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_EVENT_H

#include "axl_win_WaitableHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................

class CEvent: public CWaitableHandle
{
public:
	bool 
	Create (
		SECURITY_ATTRIBUTES* pSecAttr,
		bool IsManualReset,
		bool IsSignalled,
		const tchar_t* pName
		)
	{
		Close ();

		m_h = ::CreateEvent (pSecAttr, IsManualReset, IsSignalled, pName);
		return err::Complete (m_h != NULL);
	}

	bool 
	Open (
		ulong_t Access,
		bool DoInheritHandle,
		const tchar_t* pName
		)
	{
		Close ();

		m_h = ::OpenEvent (Access, DoInheritHandle, pName);
		return err::Complete (m_h != NULL);
	}

	bool
	Signal ()
	{ 
		ASSERT (IsOpen ());

		bool_t Result = ::SetEvent (m_h);
		return err::Complete (Result);
	}

	bool
	Reset ()
	{ 
		ASSERT (IsOpen ());

		bool_t Result = ::ResetEvent (m_h); 
		return err::Complete (Result);
	}

	bool
	Pulse ()
	{ 
		ASSERT (IsOpen ());

		bool_t Result = ::PulseEvent (m_h); 
		return err::Complete (Result);
	}
};

//.............................................................................

} // namespace win
} // namespace axl
