// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_IOCP_H

#include "axl_g_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................
	
class CIocp: public g::win::CHandle
{
public:
	bool 
	Create (dword_t ThreadCount)
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
		HANDLE h = ::CreateIoCompletionPort (hIo, m_h, (uintptr_t) pContext, 0);
		return err::Complete (h != NULL);
	}

	bool 
	PostCompletionStatus (
		dword_t ActualSize,
		void* pContext,
		OVERLAPPED* pOverlapped
		)
	{
		bool_t Result = ::PostQueuedCompletionStatus (m_h, ActualSize, (uintptr_t) pContext, pOverlapped);
		return err::Complete (Result);
	}

	bool 
	GetCompletionStatus (
		dword_t* pActualSize,
		void** ppContext,
		OVERLAPPED** ppOverlapped,
		uint_t Timeout
		)
	{
		bool_t Result = ::GetQueuedCompletionStatus (m_h, pActualSize, (ULONG_PTR*) ppContext, ppOverlapped, Timeout);
		return err::Complete (Result);
	}
};

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
