#include "pch.h"
#include "axl_err_NtError.h"
#include "axl_err_WinError.h"

namespace axl {
namespace err {

//.............................................................................

typedef 
dword_t 
WINAPI
RtlNtStatusToDosErrorFunc (NTSTATUS);

rtl::String 
NtErrorProvider::getErrorDescription (NTSTATUS status)
{
	static RtlNtStatusToDosErrorFunc* rtlNtStatusToDosErrorFunc = NULL;

	if (!rtlNtStatusToDosErrorFunc) // no need in interlockedcmpxcg -- the worst thing might happen is ptr gets overwritten
	{
		HMODULE ntDll = ::GetModuleHandleW (L"ntdll.dll");
		if (ntDll)
			rtlNtStatusToDosErrorFunc = (RtlNtStatusToDosErrorFunc*) ::GetProcAddress (ntDll, "RtlNtStatusToDosError");
		
		if (!rtlNtStatusToDosErrorFunc)
			return rtl::String::format_s ("ntstatus #%x", status);
	}

	dword_t winError = rtlNtStatusToDosErrorFunc (status);
	if (winError == ERROR_MR_MID_NOT_FOUND)
		return rtl::String::format_s ("ntstatus #%x", status);

	return WinErrorProvider::getErrorDescription (winError);
}

//.............................................................................

ErrorData*
NtError::create (NTSTATUS status)
{
	ErrorData* error = getBuffer (sizeof (ErrorData));
	if (!error)
		return NULL;

	error->m_size = sizeof (ErrorData);
	error->m_guid = g_ntErrorGuid;
	error->m_code = status;
	return error;
}

//.............................................................................

} // namespace err
} // namespace axl
