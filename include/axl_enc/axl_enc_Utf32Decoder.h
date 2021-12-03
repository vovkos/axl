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

#define _AXL_ENC_UTF32DECODER_H

#include "axl_sl_ByteOrder.h"
#include "axl_sl_Operator.h"

namespace axl {
namespace enc {

// trivial, but needed to provide an interface consistent with UTF-8 and UTF-16

//..............................................................................

template <
	typename IsReverse,
	typename IsBigEndian
>
class Utf32DecoderBase {
public:
	typedef UtfDecodeResult<utf32_t> DecodeResult;
	typedef char Storage; // unused

public:
	size_t
	calcCodePointCount(
		Storage* unused,
		const utf32_t* p,
		const utf32_t* end
	) {
		return end - p;
	}

	size_t
	calcCodePointCount(
		const utf32_t* p,
		const utf32_t* end
	) {
		return end - p;
	}

	DecodeResult
	decode(
		Storage* unused,
		utf32_t* dst,
		utf32_t* dstEnd,
		const utf32_t* src,
		const utf32_t* srcEnd,
		utf32_t unused2 = 0
	) {
		return decode(dst, dstEnd, src, srcEnd);
	}

	DecodeResult
	decode(
		utf32_t* dst,
		utf32_t* dstEnd,
		const utf32_t* src,
		const utf32_t* srcEnd,
		utf32_t unused = 0
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename IsReverse,
	typename IsBigEndian
>
UtfDecodeResult<utf32_t>
Utf32DecoderBase<IsReverse, IsBigEndian>::decode(
	utf32_t* dst,
	utf32_t* dstEnd,
	const utf32_t* src,
	const utf32_t* srcEnd,
	utf32_t unused
) {
	if (IsReverse()())
		for (; dst < dstEnd && src > srcEnd; src--)
			*dst++ = IsBigEndian()() ? sl::swapByteOrder32(replacement) : replacement;
	else
		for (; dst < dstEnd && src < srcEnd; src++)
			*dst++ = IsBigEndian()() ? sl::swapByteOrder32(replacement) : replacement;

	return DecodeResult(dst, src);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf32DecoderBase<sl::False, sl::False> Utf32Decoder;
typedef Utf32DecoderBase<sl::True, sl::False>  Utf32ReverseDecoder;
typedef Utf32DecoderBase<sl::False, sl::True>  Utf32Decoder_be;
typedef Utf32DecoderBase<sl::True, sl::True>   Utf32ReverseDecoder_be;

//..............................................................................

} // namespace enc
} // namespace axl
