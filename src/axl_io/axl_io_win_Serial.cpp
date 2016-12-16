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

#include "pch.h"
#include "axl_io_win_Serial.h"
#include "axl_sl_String.h"

namespace axl {
namespace io {
namespace win {

//..............................................................................

bool
Serial::open (
	const sl::StringRef& name,
	uint_t flags
	)
{
	close ();

	char buffer [256];
	sl::String_w deviceName (ref::BufKind_Stack, buffer, sizeof (buffer));
	deviceName = L"\\\\.\\";

	if (name.isPrefix ("\\\\.\\"))
		deviceName += name.getSubString (4);
	else
		deviceName += name;

	m_h = ::CreateFileW (
		deviceName,
		GENERIC_READ | GENERIC_WRITE,
		0, NULL,
		OPEN_EXISTING,
		flags,
		NULL
		);

	return err::complete (m_h != INVALID_HANDLE_VALUE);
}

dword_t
Serial::getStatusLines ()
{
	ASSERT (isOpen ());

	dword_t lines;

	bool_t result = ::GetCommModemStatus (m_h, &lines);
	if (!result)
		return err::failWithLastSystemError (-1);

	return lines;
}

dword_t
Serial::getWaitMask ()
{
	dword_t mask;

	bool_t result = ::GetCommMask (m_h, &mask);
	if (!result)
		return err::failWithLastSystemError (-1);

	return mask;
}

bool
Serial::overlappedWait (dword_t* event)
{
	dword_t actualSize; // unused
	StdOverlapped overlapped;
	bool result = overlappedWait (event, &overlapped);	
	return result ? getOverlappedResult (&overlapped, &actualSize) : false;
}

//..............................................................................

} // namespace win
} // namespace io
} // namespace axl
