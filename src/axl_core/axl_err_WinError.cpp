#include "pch.h"
#include "axl_err_WinError.h"

namespace axl {
namespace err {

//.............................................................................

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
		return sl::String::format_s ("winerror #%d", code);

	sl::String description = message;
	::LocalFree (message);
	
	return description; 
}

//.............................................................................

ErrorHdr*
WinError::create (dword_t code)
{
	ErrorHdr* error = getBuffer (sizeof (ErrorHdr));
	if (!error)
		return NULL;

	error->m_size = sizeof (ErrorHdr);
	error->m_guid = g_winErrorGuid;
	error->m_code = code;
	return error;
}

//.............................................................................

} // namespace err
} // namespace axl
