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
#include "axl_enc_Utf.h"
#include "axl_enc_Utf8Decoder.h"
#include "axl_enc_Utf16Decoder.h"
#include "axl_enc_Utf32Decoder.h"

#define INCLUDED_FROM_UCHAR_C
#define INCLUDED_FROM_UCASE_CPP

#include "icu_def.h"
#include "icu_uchar_props_data.h"
#include "icu_ucase_props_data.h"

namespace axl {
namespace enc {

//..............................................................................

const char*
getUtfKindString(UtfKind utfKind) {
	static const char* stringTable[] = {
		"UTF-8",                // UtfKind_Utf8 = 0,
		"UTF-16",               // UtfKind_Utf16,
		"UTF-16 (big-endian)",  // UtfKind_Utf16_be,
		"UTF-32",               // UtfKind_Utf32,
		"UTF-32 (big-endian)",  // UtfKind_Utf32_be,
	};

	return (size_t)utfKind < countof(stringTable) ?
		stringTable[utfKind] :
		"undefined-utf-kind";
}

//..............................................................................

} // namespace enc
} // namespace axl
