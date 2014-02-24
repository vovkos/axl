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

class CSemaphore: public CWaitableHandle
{
public:
	bool 
	Create (
		SECURITY_ATTRIBUTES* pSecAttr,
		uint_t InitialCount,
		uint_t MaxCount,
		const char* pName
		)
	{
		Close ();

		m_h = ::CreateSemaphore (pSecAttr, InitialCount, MaxCount, pName);
		return err::Complete (m_h != NULL);
	}

	bool 
	Open (
		uint_t Access,
		bool DoInheritHandle,
		const char* pName
		)
	{
		Close ();

		m_h = ::OpenSemaphore (Access, DoInheritHandle, pName);
		return err::Complete (m_h != NULL);
	}

	bool
	Signal (uint_t Count = 1)
	{
		bool_t Result = ::ReleaseSemaphore (m_h, Count, NULL);
		return err::Complete (Result);
	}
};

//.............................................................................

} // namespace axl
} // namespace mt
} // namespace win
