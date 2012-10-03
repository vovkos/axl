// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SERIAL_H

#include "axl_win_Handle.h"
#include "axl_err_Error.h"

namespace axl {
namespace win {

//.............................................................................
	
class CSerial: public CFileHandle
{
public:
	bool 
	Open (const tchar_t* pName);

	bool 
	SetSettings (const DCB* pDcb)
	{
		ASSERT (IsOpen ());

		bool_t Result = SetCommState (m_h, (DCB*) pDcb);
		return err::Complete (Result);
	}

	bool 
	GetSettings (DCB* pDcb)
	{
		ASSERT (IsOpen ());

		bool_t Result = GetCommState (m_h, pDcb);
		return err::Complete (Result);
	}

	bool 
	Escape (ulong_t Function)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::EscapeCommFunction (m_h, Function);
		return err::Complete (Result);
	}

	ulong_t
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

	bool
	SetWaitMask (ulong_t Mask)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::SetCommMask (m_h, Mask);
		return err::Complete (Result);
	}

	bool
	Wait (
		ulong_t* pEvent,
		OVERLAPPED* pOverlapped
		)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::WaitCommEvent (m_h, pEvent, pOverlapped);
		return err::Complete (Result);
	}

	bool 
	Read (
		void* p, 
		ulong_t Size, 
		ulong_t* pActualSize,
		OVERLAPPED* pOverlapped
		)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::ReadFile (m_h, p, Size, pActualSize, pOverlapped);
		return err::Complete (Result);
	}

	bool 
	Write (
		const void* p, 
		ulong_t Size,
		ulong_t* pActualSize,
		OVERLAPPED* pOverlapped
		)
	{
		ASSERT (IsOpen ());

		bool_t Result = ::WriteFile (m_h, p, (ulong_t) Size, pActualSize, pOverlapped);
		return err::Complete (Result);
	}
};

//.............................................................................

} // namespace win
} // namespace axl
