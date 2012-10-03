// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_IOCP_H

#include "axl_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................
	
class CIocp: public CHandle
{
public:
	bool 
	Create (ulong_t ThreadCount)
	{
		Close ();

		m_h = ::CreateIoCompletionPort (INVALID_HANDLE_VALUE, NULL, 0, ThreadCount);
		return err::Complete (m_h != NULL);
	}

	bool 
	Associate (
		HANDLE hIo,
		void* pContext
		)
	{
		ASSERT (IsOpen ());

		HANDLE h = ::CreateIoCompletionPort (hIo, m_h, (uintptr_t) pContext, 0);
		return err::Complete (h != NULL);
	}

	bool 
	PostCompletionStatus (
		ulong_t ActualSize,
		void* pContext,
		OVERLAPPED* pOverlapped
		)
    {
		ASSERT (IsOpen ());

		bool_t Result = ::PostQueuedCompletionStatus(m_h, ActualSize, (uintptr_t) pContext, pOverlapped);
		return err::Complete (Result);
    }

	bool 
	GetCompletionStatus (
		ulong_t* pActualSize,
		void** ppContext,
		OVERLAPPED** ppOverlapped,
		ulong_t Timeout
		)
    {
		ASSERT (IsOpen ());

		bool_t Result = ::GetQueuedCompletionStatus(m_h, pActualSize, (ULONG_PTR*) ppContext, ppOverlapped, Timeout);
		return err::Complete (Result);
    }
};

//.............................................................................

} // namespace win
} // namespace axl
