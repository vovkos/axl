#include "stdafx.h"
#include "axl_err_NtError.h"
#include "axl_err_WinError.h"

namespace axl {
namespace err {

//.............................................................................

rtl::CString 
CNtErrorProvider::GetErrorDescription (long Status)
{
	typedef DWORD (WINAPI* FRtlNtStatusToDosError)(long);
	static FRtlNtStatusToDosError _pfnRtlNtStatusToDosError = NULL;

	if (!_pfnRtlNtStatusToDosError) // no need in interlockedcmpxcg -- the worst thing is pfn gets overwritten
	{
		HMODULE hNtDll = GetModuleHandle ("ntdll.dll");
		if (hNtDll)
			_pfnRtlNtStatusToDosError = (FRtlNtStatusToDosError) GetProcAddress (hNtDll, _T("RtlNtStatusToDosError"));
		
		if (!_pfnRtlNtStatusToDosError)
			return rtl::CString::Format_s (_T("ntstatus #%x"), Status);
	}

	DWORD WinError = _pfnRtlNtStatusToDosError (Status);
	if (WinError == ERROR_MR_MID_NOT_FOUND)
		return rtl::CString::Format_s (_T("ntstatus #%x"), Status);

	return CWinErrorProvider::GetErrorDescription (WinError);
}

//.............................................................................

TError*
CNtError::Create (long Status)
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
