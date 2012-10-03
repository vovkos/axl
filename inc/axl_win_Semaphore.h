// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SEMAPHORE_H

#include "axl_win_WaitableHandle.h"

namespace axl {
namespace win {

//.............................................................................

class CSemaphore: public CWaitableHandle
{
public:
	bool 
	Create (
		SECURITY_ATTRIBUTES* pSecAttr,
		ulong_t InitialCount,
		ulong_t MaxCount,
		const tchar_t* pName
		)
	{
		Close ();

		m_h = ::CreateSemaphore (pSecAttr, InitialCount, MaxCount, pName);
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

		m_h = ::OpenSemaphore (Access, DoInheritHandle, pName);
		return err::Complete (m_h != NULL);
	}

	bool
	Signal (ulong_t Count = 1)
	{
		bool_t Result = ::ReleaseSemaphore (m_h, Count, NULL);
		return err::Complete (Result);
	}
};

//.............................................................................

} // namespace axl
} // namespace win
