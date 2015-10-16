// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_WIN_SERIAL_H

#include "axl_io_win_FileHandle.h"
#include "axl_err_Error.h"

namespace axl {
namespace io {
namespace win {

//.............................................................................

class Serial: public FileHandle
{
public:
	bool
	open (
		const char* name,
		uint_t flags = 0
		);

	bool
	getSettings (DCB* dcb)
	{
		bool_t result = ::GetCommState (m_h, dcb);
		return err::complete (result);
	}

	bool
	setSettings (const DCB* dcb)
	{
		bool_t result = ::SetCommState (m_h, (DCB*) dcb);
		return err::complete (result);
	}

	bool
	getTimeouts (COMMTIMEOUTS* timeouts)
	{
		bool_t result = ::GetCommTimeouts (m_h, timeouts);
		return err::complete (result);
	}

	bool
	setTimeouts (const COMMTIMEOUTS* timeouts)
	{
		bool_t result = ::SetCommTimeouts (m_h, (COMMTIMEOUTS*) timeouts);
		return err::complete (result);
	}

	bool
	escape (uint_t function)
	{
		bool_t result = ::EscapeCommFunction (m_h, function);
		return err::complete (result);
	}

	dword_t
	getStatusLines ();

	bool
	setDtr (bool isSet)
	{
		return escape (isSet ? SETDTR : CLRDTR);
	}

	bool
	setRts (bool isSet)
	{
		return escape (isSet ? SETRTS : CLRRTS);
	}

	dword_t
	getWaitMask ();

	bool
	setWaitMask (dword_t mask)
	{
		bool_t result = ::SetCommMask (m_h, mask);
		return err::complete (result);
	}

	bool
	wait (
		dword_t* event,
		OVERLAPPED* overlapped
		)
	{
		bool_t result = ::WaitCommEvent (m_h, event, overlapped);
		return completeAsyncRequest (result, overlapped);
	}

	size_t
	read (
		void* p,
		size_t size
		);

	size_t
	write (
		const void* p,
		size_t size
		);

	bool
	read (
		void* p,
		dword_t size,
		dword_t* actualSize,
		OVERLAPPED* overlapped
		)
	{
		bool_t result = ::ReadFile (m_h, p, size, actualSize, overlapped);
		return completeAsyncRequest (result, overlapped);
	}

	bool
	write (
		const void* p,
		dword_t size,
		dword_t* actualSize,
		OVERLAPPED* overlapped
		)
	{
		bool_t result = ::WriteFile (m_h, p, size, actualSize, overlapped);
		return completeAsyncRequest (result, overlapped);
	}
};

//.............................................................................

} // namespace win
} // namespace io
} // namespace axl
