// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_WIN_EVENT_H

#include "axl_mt_win_WaitableHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace mt {
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
		const wchar_t* pName
		)
	{
		Close ();

		m_h = ::CreateEventW (pSecAttr, IsManualReset, IsSignalled, pName);
		return err::Complete (m_h != NULL);
	}

	bool 
	Open (
		uint_t Access,
		bool DoInheritHandle,
		const wchar_t* pName
		)
	{
		Close ();

		m_h = ::OpenEventW (Access, DoInheritHandle, pName);
		return err::Complete (m_h != NULL);
	}

	bool
	Signal ()
	{ 
		bool_t Result = ::SetEvent (m_h);
		return err::Complete (Result);
	}

	bool
	Reset ()
	{ 
		bool_t Result = ::ResetEvent (m_h); 
		return err::Complete (Result);
	}

	bool
	Pulse ()
	{ 
		bool_t Result = ::PulseEvent (m_h); 
		return err::Complete (Result);
	}
};

//.............................................................................

} // namespace win
} // namespace mt 
} // namespace axl
