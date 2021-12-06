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

#define _AXL_ENC_UTF32_H

#include "axl_enc_Unicode.h"
#include "axl_sl_ByteOrder.h"

namespace axl {
namespace enc {

//..............................................................................

class Utf32EncoderBase {
public:
	enum {
		MaxEncodeLength = 1 // always one 32-bit codeunit per codepoint
	};

	typedef utf32_t C;

public:
	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t unused = 0
	) {
		return 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32Encoder: public Utf32EncoderBase {
public:
	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		*p = cp;
		return p + 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32Encoder_be: public Utf32EncoderBase {
public:
	static
	C*
	encodeCodePoint(
		C* p,
		utf32_t x,
		utf32_t unused = 0
	) {
		*p = sl::swapByteOrder32(x);
		return p + 1;
	}
};

//..............................................................................

template <
	typename IsReverse,
	typename IsBigEndian
>
class Utf32DecoderBase {
public:
	enum {
		MaxEmitLength = 1,
	};

	typedef utf32_t C;
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

		for (; p < end; p++) {
			utf32_t cp = IsBigEndian()() ? sl::swapByteOrder32(*p) : *p;
			length += Encoder::getEncodeLength(cp, replacement);
		}

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
		return decode<Encoder>(dst, dstEnd, src, srcEnd, replacement);
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
			for (; dst < dstEnd && src > srcEnd; src--) {
				utf32_t cp = IsBigEndian()() ? sl::swapByteOrder32(*src) : *src;
				dst = Encoder::encode(dst, cp, replacement);
			}
		else
			for (; dst < dstEnd && src < srcEnd; src++) {
				utf32_t cp = IsBigEndian()() ? sl::swapByteOrder32(*src) : *src;
				dst = Encoder::encode(dst, cp, replacement);
			}

		return EncodeResult<typename Encoder::C, C>(dst, src);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf32DecoderBase<sl::False, sl::False> Utf32Decoder;
typedef Utf32DecoderBase<sl::True, sl::False>  Utf32ReverseDecoder;
typedef Utf32DecoderBase<sl::False, sl::True>  Utf32Decoder_be;
typedef Utf32DecoderBase<sl::True, sl::True>   Utf32ReverseDecoder_be;

//..............................................................................

} // namespace enc
} // namespace axl
