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

inline
wchar_t*
getSystemMessage(dword_t code) {
	enum {
		Flags =
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS |
			FORMAT_MESSAGE_MAX_WIDTH_MASK, // no line breaks please

		EnglishLangId = MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
		NeutralLangId = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL),
	};

	// prefer english message, fall back localized defaults if failed

	wchar_t* message = NULL;
	::FormatMessageW(Flags, NULL, code, EnglishLangId, (LPWSTR) &message, 0, NULL);

	if (!message)
		::FormatMessageW(Flags, NULL, code, NeutralLangId, (LPWSTR) &message, 0, NULL);

	return message;
}

sl::String
WinErrorProvider::getErrorDescription(dword_t code) {
	wchar_t* message = getSystemMessage(code);
	if (!message) { // try again with HRESULT_FROM_SETUPAPI
		HRESULT hresult = HRESULT_FROM_SETUPAPI(code);
		message = getSystemMessage(hresult);
		if (!message)
			return sl::formatString("winerror #%d", code);
	}

	sl::String description = message;
	::LocalFree(message);

	return description;
}

//..............................................................................

} // namespace win
} // namespace err
} // namespace axl
