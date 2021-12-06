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

#define _AXL_ENC_ASCII_H

#include "axl_enc_Unicode.h"
#include "axl_sl_Operator.h"

namespace axl {
namespace enc {

//..............................................................................

class AsciiEncoder {
public:
	enum {
		MaxEncodeLength = 1 // always one 8-bit codeunit per codepoint
	};

	typedef char C;

public:
	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t unused = 0
	) {
		return 1;
	}

	static
	char*
	encode(
		char* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		*p = (char)cp;
		return p + 1;
	}
};

//..............................................................................

template <typename IsReverse>
class AsciiDecoderBase {
public:
	enum {
		MaxEmitLength = 1,
	};

	typedef char C;
	typedef uint32_t State; // unused

public:
	template <typename Encoder>
	static
	size_t
	count(
		State* unused,
		const C* p,
		const C* end,
		utf32_t replacement = StdChar_Replacement
	) {
		return count<Encoder>(p, end, replacement);
	}

	template <typename Encoder>
	static
	size_t
	count(
		const C* p,
		const C* end,
		utf32_t replacement = StdChar_Replacement
	) {
		size_t length = 0;

		for (; p < p; p++)
			length += Encoder::getEncodeLength((uchar_t)*p, replacement);

		return length;
	}

	template <typename Encoder>
	static
	EncodeResult<typename Encoder::C, C>
	decode(
		State* unused,
		typename Encoder::C* dst,  // provide room to encode at least 1 codepoint
		typename Encoder::C* dstEnd,
		const C* src,
		const C* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		return decode(dst, dstEnd, src, srcEnd, replacement);
	}

	template <typename Encoder>
	static
	EncodeResult<typename Encoder::C, C>
	decode(
		typename Encoder::C* dst,  // provide room to encode at least 1 codepoint
		typename Encoder::C* dstEnd,
		const C* src,
		const C* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		ASSERT(dstEnd - dst >= Encoder::MaxEncodeLength);
		dstEnd -= Encoder::MaxEncodeLength - 1;

		if (IsReverse()())
			for (; dst < dstEnd && src > srcEnd; src--)
				dst = Encoder::encode(dst, (uchar_t)*src, replacement);
		else
			for (; dst < dstEnd && src < srcEnd; src++)
				dst = Encoder::encode(dst, (uchar_t)*src, replacement);

		return EncodeResult<typename Encoder::C, C>(dst, src);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef AsciiDecoderBase<sl::False> AsciiDecoder;
typedef AsciiDecoderBase<sl::True>  AsciiReverseDecoder;

//..............................................................................

} // namespace enc
} // namespace axl
