// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SERIAL_H

#include "axl_g_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class CSerial: public g::win::CFileHandle
{
public:
	bool
	Open (
		const char* pName,
		uint_t Flags = 0
		);

	bool
	GetSettings (DCB* pDcb)
	{
		bool_t Result = ::GetCommState (m_h, pDcb);
		return err::Complete (Result);
	}

	bool
	SetSettings (const DCB* pDcb)
	{
		bool_t Result = ::SetCommState (m_h, (DCB*) pDcb);
		return err::Complete (Result);
	}

	bool
	Escape (uint_t Function)
	{
		bool_t Result = ::EscapeCommFunction (m_h, Function);
		return err::Complete (Result);
	}

	dword_t
	GetStatusLines ();

	bool
	SetDtr (bool IsSet)
	{
		return Escape (IsSet ? SETDTR : CLRDTR);
	}

	bool
	SetRts (bool IsSet)
	{
		return Escape (IsSet ? SETRTS : CLRRTS);
	}

	dword_t
	GetWaitMask ();

	bool
	SetWaitMask (dword_t Mask)
	{
		bool_t Result = ::SetCommMask (m_h, Mask);
		return err::Complete (Result);
	}

	bool
	Wait (
		dword_t* pEvent,
		OVERLAPPED* pOverlapped
		)
	{
		bool_t Result = ::WaitCommEvent (m_h, pEvent, pOverlapped);
		return CompleteAsyncRequest (Result, pOverlapped);
	}

	dword_t
	Read (
		void* p,
		dword_t Size
		);

	dword_t
	Write (
		const void* p,
		dword_t Size
		);

	bool
	Read (
		void* p,
		dword_t Size,
		dword_t* pActualSize,
		OVERLAPPED* pOverlapped
		)
	{
		bool_t Result = ::ReadFile (m_h, p, Size, pActualSize, pOverlapped);
		return CompleteAsyncRequest (Result, pOverlapped);
	}

	bool
	Write (
		const void* p,
		dword_t Size,
		dword_t* pActualSize,
		OVERLAPPED* pOverlapped
		)
	{
		bool_t Result = ::WriteFile (m_h, p, Size, pActualSize, pOverlapped);
		return CompleteAsyncRequest (Result, pOverlapped);
	}
};

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
