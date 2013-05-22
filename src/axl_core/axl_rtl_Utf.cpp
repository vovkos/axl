#include "pch.h"
#include "axl_rtl_Utf.h"

namespace axl {
namespace rtl {

//.............................................................................

const char*
GetUtfKindString (EUtf UtfKind)
{
	static const char* StringTable [] = 
	{
		"UTF-8",        // EUtf_Utf8 = 0,
		"UTF-16 (LE)",  // EUtf_Utf16,
		"UTF-16 (BE)",  // EUtf_Utf16_be,
		"UTF-32 (LE)",  // EUtf_Utf32,
		"UTF-32 (BE)",  // EUtf_Utf32_be,
	};

	return (size_t) UtfKind < countof (StringTable) ? 
		StringTable [UtfKind] : 
		"undefined-utf-kind";
}

//.............................................................................

} // namespace rtl
} // namespace axl
