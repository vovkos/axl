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
#include "axl_sys_win_NtStatus.h"
#include "axl_sys_win_WinError.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

typedef
dword_t
WINAPI
RtlNtStatusToDosErrorFunc (long);

sl::String
NtStatusProvider::getErrorDescription (long status)
{
	static HMODULE ntDll = ::GetModuleHandleW (L"ntdll.dll");
	static RtlNtStatusToDosErrorFunc* rtlNtStatusToDosErrorFunc = (RtlNtStatusToDosErrorFunc*) ::GetProcAddress (ntDll, "RtlNtStatusToDosError");

	dword_t winError = status;

	if (rtlNtStatusToDosErrorFunc)
		winError = rtlNtStatusToDosErrorFunc (status);

	if (winError != ERROR_MR_MID_NOT_FOUND && winError != status)
		return WinErrorProvider::getErrorDescription (winError);

	wchar_t* message = NULL;

	::FormatMessageW (
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_FROM_HMODULE |
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK, // no line breaks please
		ntDll,
		status,
		MAKELANGID (LANG_ENGLISH, SUBLANG_DEFAULT),
		(LPWSTR) &message,
		0,
		NULL
		);

	if (!message)
		return sl::formatString ("ntstatus 0x%08x", status);

	sl::String description = message;
	::LocalFree (message);
	return description;
}

//..............................................................................

size_t
NtStatus::create (long status)
{
	err::ErrorHdr* error = createBuffer (sizeof (err::ErrorHdr));
	if (!error)
		return -1;

	error->m_size = sizeof (err::ErrorHdr);
	error->m_guid = g_ntStatusGuid;
	error->m_code = status;
	return sizeof (err::ErrorHdr);
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
