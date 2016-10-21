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
#include "axl_sys_win_WinError.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

sl::String
WinErrorProvider::getErrorDescription (dword_t code)
{
	wchar_t* message = NULL;

	::FormatMessageW (
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK, // no line breaks please
		NULL,
		code,
		MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPWSTR) &message,
		0,
		NULL
		);

	if (!message)
		return sl::formatString ("winerror #%d", code);

	sl::String description = message;
	::LocalFree (message);

	return description;
}

//..............................................................................

size_t
WinError::create (dword_t code)
{
	err::ErrorHdr* error = createBuffer (sizeof (err::ErrorHdr));
	if (!error)
		return NULL;

	error->m_size = sizeof (err::ErrorHdr);
	error->m_guid = g_winErrorGuid;
	error->m_code = code;
	return sizeof (err::ErrorHdr);
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
