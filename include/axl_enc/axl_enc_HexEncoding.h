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

#define _AXL_ENC_HEXENCODING_H

#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace enc {

//..............................................................................

class GetHexChar_l {
public:
	char
	operator () (uchar_t x) {
		static char charTable[] = "0123456789abcdef";
		return charTable[x & 0xf];
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class GetHexChar_u {
public:
	char
	operator () (uchar_t x) {
		static char charTable[] = "0123456789ABCDEF";
		return charTable[x & 0xf];
	}
};

//..............................................................................

enum HexEncodingFlag {
	HexEncodingFlag_UpperCase = 0x01,
	HexEncodingFlag_NoSpace   = 0x02,
	HexEncodingFlag_Multiline = 0x04,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class HexEncoding {
public:
	static
	uchar_t
	getHexCharValue(char x) {
		return
			x >= '0' && x <= '9' ? x - '0' :
			x >= 'a' && x <= 'f' ? 10 + x - 'a' :
			x >= 'A' && x <= 'F' ? 10 + x - 'A' : 0;
	}

	static
	char
	getHexChar_l(uchar_t x) {
		return GetHexChar_l()(x);
	}

	static
	char
	getHexChar_u(uchar_t x) {
		return GetHexChar_u()(x);
	}

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
		const sl::StringRef& source
	);

	static
	sl::Array<char>
	decode(const sl::StringRef& source) {
		sl::Array<char> buffer;
		decode(&buffer, source);
		return buffer;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
