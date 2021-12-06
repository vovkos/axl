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

#define _AXL_ENC_UTF_H

#include "axl_enc_Ascii.h"
#include "axl_enc_Utf8.h"
#include "axl_enc_Utf16.h"
#include "axl_enc_Utf32.h"

namespace axl {
namespace enc {

//..............................................................................

enum UtfKind {
	UtfKind_Utf8 = 0,
	UtfKind_Utf16,
	UtfKind_Utf16_be,
	UtfKind_Utf32,
	UtfKind_Utf32_be,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getUtfKindString(UtfKind utfKind);

//..............................................................................

class Ascii {
public:
	typedef char C;
	typedef AsciiEncoder Encoder;
	typedef AsciiDecoder Decoder;
	typedef AsciiReverseDecoder ReverseDecoder;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf8 {
public:
	typedef utf8_t C;
	typedef Utf8Encoder Encoder;
	typedef Utf8Decoder Decoder;
	typedef Utf8ReverseDecoder ReverseDecoder;

public:
	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf8;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xef, 0xbb, 0xbf };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 3;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16 {
public:
	typedef utf16_t C;
	typedef Utf16Encoder Encoder;
	typedef Utf16Decoder Decoder;
	typedef Utf16ReverseDecoder ReverseDecoder;

public:
	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf16;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xff, 0xfe };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 2;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16_be {
public:
	typedef utf16_t C;
	typedef Utf16Encoder_be Encoder;
	typedef Utf16Decoder_be Decoder;
	typedef Utf16ReverseDecoder_be ReverseDecoder;

public:
	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf16_be;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xfe, 0xff };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 2;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32 {
public:
	typedef utf32_t C;
	typedef Utf32Encoder Encoder;
	typedef Utf32Decoder Decoder;
	typedef Utf32ReverseDecoder ReverseDecoder;

public:
	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf32;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xff, 0xfe, 0x00, 0x00 };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 4;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32_be {
public:
	typedef utf32_t C;
	typedef Utf32Encoder Encoder;
	typedef Utf32Decoder Decoder;
	typedef Utf32ReverseDecoder ReverseDecoder;

public:
	static
	UtfKind
	getUtfKind() {
		return UtfKind_Utf32_be;
	}

	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0x00, 0x00, 0xfe, 0xff };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 4;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
