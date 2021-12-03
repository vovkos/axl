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

#define _AXL_ENC_UNICODE_H

#include "axl_g_Pch.h"

namespace axl {
namespace enc {

//..............................................................................

enum StdChar {
	StdChar_Undefined   = 0xffff,
	StdChar_Replacement = 0xfffd,
};

//..............................................................................

bool
isPrintable(utf32_t c);

bool
isPrintableNonMark(utf32_t c);

bool
isSpace(utf32_t c);

bool
isPunctuation(utf32_t c);

bool
isLetter(utf32_t c);

bool
isDigit(utf32_t c);

bool
isNumber(utf32_t c);

bool
isLetterOrDigit(utf32_t c);

bool
isLetterOrNumber(utf32_t c);

bool
isLowerCase(utf32_t c);

bool
isUpperCase(utf32_t c);

//..............................................................................

// case ops

utf32_t
toLowerCase(utf32_t c);

utf32_t
toUpperCase(utf32_t c);

utf32_t
toCaseFolded(utf32_t c);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ToLowerCase {
public:
	utf32_t
	operator () (utf32_t x) const {
		return toLowerCase(x);
	}
};

class ToUpperCase {
public:
	utf32_t
	operator () (utf32_t x) const {
		return toUpperCase(x);
	}
};

class ToCaseFolded {
public:
	utf32_t
	operator () (utf32_t x) const {
		return toCaseFolded(x);
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
