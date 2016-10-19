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
	static RtlNtStatusToDosErrorFunc* rtlNtStatusToDosErrorFunc = NULL;

	if (!rtlNtStatusToDosErrorFunc) // no need in interlockedcmpxcg -- the worst thing might happen is ptr gets overwritten
	{
		HMODULE ntDll = ::GetModuleHandleW (L"ntdll.dll");
		if (ntDll)
			rtlNtStatusToDosErrorFunc = (RtlNtStatusToDosErrorFunc*) ::GetProcAddress (ntDll, "RtlNtStatusToDosError");

		if (!rtlNtStatusToDosErrorFunc)
			return sl::formatString ("ntstatus #%x", status);
	}

	dword_t winError = rtlNtStatusToDosErrorFunc (status);
	if (winError == ERROR_MR_MID_NOT_FOUND)
		return sl::formatString ("ntstatus #%x", status);

	return WinErrorProvider::getErrorDescription (winError);
}

//..............................................................................

size_t
NtStatus::create (long status)
{
	err::ErrorHdr* error = createBuffer (sizeof (err::ErrorHdr));
	if (!error)
		return NULL;

	error->m_size = sizeof (err::ErrorHdr);
	error->m_guid = g_ntStatusGuid;
	error->m_code = status;
	return sizeof (err::ErrorHdr);
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
