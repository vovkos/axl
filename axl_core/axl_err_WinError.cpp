#include "stdafx.h"
#include "axl_err_WinError.h"

namespace axl {
namespace err {

//.............................................................................

rtl::CString 
CWinErrorProvider::GetErrorDescription (DWORD Error)
{
	tchar_t* pMessage = NULL;
	
	FormatMessage ( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK, // no line breaks please
		NULL,
		Error,
		MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &pMessage, 0, NULL
		);

	if (!pMessage)
		return rtl::CString::Format_s (_T("winerror #%d"), Error);

	rtl::CString Description = pMessage;
	LocalFree (pMessage);
	
	return Description; 
}

//.............................................................................

TError*
CWinError::Create (DWORD Error)
{
	TError* pError = GetBuffer (sizeof (TError));
	if (!pError)
		return NULL;

	pError->m_Size = sizeof (TError);
	pError->m_Guid = GUID_WinError;
	pError->m_Code = Error;
	return pError;
}

//.............................................................................

} // namespace err
} // namespace axl
