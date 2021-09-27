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

#pragma once

#define _AXL_ENC_BASE32ENCODING_H

#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace enc {

//..............................................................................

class GetBase32Char {
public:
	char
	operator () (uchar_t x) {
		static const char charTable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
		return charTable[x & 0x1f];
	}
};

//..............................................................................

enum Base32EncodingFlag {
	Base32EncodingFlag_NoPadding  = 0x01,
	Base32EncodingFlag_Multiline  = 0x02,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Base32Encoding {
public:
	static
	size_t
	encode(
		sl::String* string,
		const void* p,
		size_t size,
		uint_t flags = 0
	);

	static
	sl::String
	encode(
		const void* p,
		size_t size,
		uint_t flags = 0
	) {
		sl::String string;
		encode(&string, p, size, flags);
		return string;
	}

	static
	size_t
	decode(
		sl::Array<char>* buffer,
		const sl::StringRef& string
	);

	static
	sl::Array<char>
	decode(const sl::StringRef& string) {
		sl::Array<char> buffer;
		decode(&buffer, string);
		return buffer;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
