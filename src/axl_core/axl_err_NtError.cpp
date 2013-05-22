#include "pch.h"
#include "axl_err_NtError.h"
#include "axl_err_WinError.h"

namespace axl {
namespace err {

//.............................................................................

rtl::CString 
CNtErrorProvider::GetErrorDescription (NTSTATUS Status)
{
	typedef dword_t (WINAPI* FRtlNtStatusToDosError) (NTSTATUS);
	static FRtlNtStatusToDosError _pfRtlNtStatusToDosError = NULL;

	if (!_pfRtlNtStatusToDosError) // no need in interlockedcmpxcg -- the worst thing is pf gets overwritten
	{
		HMODULE hNtDll = GetModuleHandleW (L"ntdll.dll");
		if (hNtDll)
			_pfRtlNtStatusToDosError = (FRtlNtStatusToDosError) GetProcAddress (hNtDll, "RtlNtStatusToDosError");
		
		if (!_pfRtlNtStatusToDosError)
			return rtl::CString::Format_s ("ntstatus #%x", Status);
	}

	dword_t WinError = _pfRtlNtStatusToDosError (Status);
	if (WinError == ERROR_MR_MID_NOT_FOUND)
		return rtl::CString::Format_s ("ntstatus #%x", Status);

	return CWinErrorProvider::GetErrorDescription (WinError);
}

//.............................................................................

TError*
CNtError::Create (NTSTATUS Status)
{
	TError* pError = GetBuffer (sizeof (TError));
	if (!pError)
		return NULL;

	pError->m_Size = sizeof (TError);
	pError->m_Guid = GUID_NtError;
	pError->m_Code = Status;
	return pError;
}

//.............................................................................

} // namespace err
} // namespace axl
