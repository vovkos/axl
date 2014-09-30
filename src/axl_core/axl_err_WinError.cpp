#include "pch.h"
#include "axl_err_WinError.h"

namespace axl {
namespace err {

//.............................................................................

rtl::CString 
CWinErrorProvider::getErrorDescription (dword_t code)
{
	wchar_t* message = NULL;
	
	::FormatMessageW ( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS |
		FORMAT_MESSAGE_MAX_WIDTH_MASK, // no line breaks please
		NULL,
		code,
		MAKELANGID (LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPWSTR) &message, 
		0, 
		NULL
		);

	if (!message)
		return rtl::CString::format_s ("winerror #%d", code);

	rtl::CString description = message;
	::LocalFree (message);
	
	return description; 
}

//.............................................................................

TError*
CWinError::create (dword_t code)
{
	TError* error = getBuffer (sizeof (TError));
	if (!error)
		return NULL;

	error->m_size = sizeof (TError);
	error->m_guid = GUID_WinError;
	error->m_code = code;
	return error;
}

//.............................................................................

} // namespace err
} // namespace axl
