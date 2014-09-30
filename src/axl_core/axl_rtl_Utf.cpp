#include "pch.h"
#include "axl_rtl_Utf.h"

namespace axl {
namespace rtl {

//.............................................................................

const char*
getUtfKindString (EUtf utfKind)
{
	static const char* stringTable [] = 
	{
		"UTF-8",                // EUtf_Utf8 = 0,
		"UTF-16",               // EUtf_Utf16,
		"UTF-16 (big-endian)",  // EUtf_Utf16_be,
		"UTF-32",               // EUtf_Utf32,
		"UTF-32 (big-endian)",  // EUtf_Utf32_be,
	};

	return (size_t) utfKind < countof (stringTable) ? 
		stringTable [utfKind] : 
		"undefined-utf-kind";
}

//.............................................................................

} // namespace rtl
} // namespace axl
