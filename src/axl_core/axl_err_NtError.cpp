#include "pch.h"
#include "axl_err_NtError.h"
#include "axl_err_WinError.h"

namespace axl {
namespace err {

//.............................................................................

rtl::CString 
CNtErrorProvider::getErrorDescription (NTSTATUS status)
{
	typedef dword_t (WINAPI* FRtlNtStatusToDosError) (NTSTATUS);
	static FRtlNtStatusToDosError _pfRtlNtStatusToDosError = NULL;

	if (!_pfRtlNtStatusToDosError) // no need in interlockedcmpxcg -- the worst thing is pf gets overwritten
	{
		HMODULE hNtDll = ::GetModuleHandleW (L"ntdll.dll");
		if (hNtDll)
			_pfRtlNtStatusToDosError = (FRtlNtStatusToDosError) ::GetProcAddress (hNtDll, "RtlNtStatusToDosError");
		
		if (!_pfRtlNtStatusToDosError)
			return rtl::CString::format_s ("ntstatus #%x", status);
	}

	dword_t winError = _pfRtlNtStatusToDosError (status);
	if (winError == ERROR_MR_MID_NOT_FOUND)
		return rtl::CString::format_s ("ntstatus #%x", status);

	return CWinErrorProvider::getErrorDescription (winError);
}

//.............................................................................

TError*
CNtError::create (NTSTATUS status)
{
	TError* error = getBuffer (sizeof (TError));
	if (!error)
		return NULL;

	error->m_size = sizeof (TError);
	error->m_guid = GUID_NtError;
	error->m_code = status;
	return error;
}

//.............................................................................

} // namespace err
} // namespace axl
