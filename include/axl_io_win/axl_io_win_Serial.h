//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#pragma once

#define _AXL_WIN_SERIAL_H

#include "axl_io_win_FileHandle.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

class Serial: public FileHandle
{
public:
	bool
	open (
		const sl::StringRef& name,
		uint_t flags = 0
		);

	bool 
	clearError (
		dword_t* errors,
		COMSTAT* stat
		)
	{
		bool_t result = ::ClearCommError (m_h, errors, stat);
		return err::complete (result);
	}

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
	wait (dword_t* event)
	{
		bool_t result = ::WaitCommEvent (m_h, event, NULL);
		return err::complete (result);
	}

	bool
	overlappedWait (
		dword_t* event,
		OVERLAPPED* overlapped
		)
	{
		bool_t result = ::WaitCommEvent (m_h, event, overlapped);
		return completeOverlappedRequest (result);
	}

	bool
	overlappedWait (dword_t* event);

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
};

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
