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

#define _AXL_ENC_UTF16DECODER_H

#include "axl_enc_Unicode.h"
#include "axl_enc_Utf16Dfa.h"
#include "axl_sl_ByteOrder.h"

namespace axl {
namespace enc {

// emits a replacement char per each unpaired surrogate

//..............................................................................

template <typename Dfa0>
class Utf16DecoderBase {
public:
	typedef Dfa0 Dfa;
	typedef UtfDecodeResult<utf16_t> DecodeResult;
	typedef uint32_t Storage;

public:
	size_t
	calcCodePointCount(
		Storage* storage,
		const utf16_t* p,
		const utf16_t* end
	);

	size_t
	calcCodePointCount(
		const utf16_t* p,
		const utf16_t* end
	) {
		return calcCodePointCount(Dfa(), p, end);
	}

	DecodeResult
	decode(
		Storage* storage,
		utf32_t* dst, // utf32_t[2 or more]
		utf32_t* dstEnd,
		const utf16_t* src,
		const utf16_t* srcEnd,
		utf32_t replacement = StdChar_Replacement
	);

	DecodeResult
	decode(
		utf32_t* dst, // utf32_t[2 or more]
		utf32_t* dstEnd,
		const utf16_t* src,
		const utf16_t* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		return decode(Dfa(), dst, dstEnd, src, srcEnd, replacement);
	}

protected:
	size_t
	calcCodePointCount(
		Dfa& dfa,
		const utf16_t* p,
		const utf16_t* end
	);

	DecodeResult
	decode(
		Dfa& dfa,
		utf32_t* dst,
		utf32_t* dstEnd,
		const utf16_t* src,
		const utf16_t* srcEnd,
		utf32_t replacement
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Dfa>
size_t
Utf16DecoderBase<Dfa>::calcCodePointCount(
	Storage* storage,
	const utf16_t* p,
	const utf16_t* end
) {
	Dfa dfa(*storage);
	size_t count = calcCodePointCount(dfa, p, end);
	*storage = dfa.save();
	return count;
}

template <typename Dfa>
size_t
Utf16DecoderBase<Dfa>::calcCodePointCount(
	Dfa& dfa,
	const utf16_t* src,
	const utf16_t* srcEnd
) {
	size_t count = 0;

	if (Dfa::IsReverse()())
		for (; src > srcEnd; src--) {
			uint_t state = dfa.count(*src);
			if (Dfa::isError(state))
				count++;

			if (Dfa::isReady(state))
				count++;
		}
	else
		for (; src < srcEnd; src++) {
			uint_t state = dfa.count(*src);
			if (Dfa::isError(state))
				count++;

			if (Dfa::isReady(state))
				count++;
		}

	return count;
}

template <typename Dfa>
UtfDecodeResult<utf16_t>
Utf16DecoderBase<Dfa>::decode(
	Storage* storage,
	utf32_t* dst,
	utf32_t* dstEnd,
	const utf16_t* src,
	const utf16_t* srcEnd,
	utf32_t replacement
) {
	Dfa dfa(*storage);
	DecodeResult result = decode(dfa, dst, dstEnd, src, srcEnd, replacement);
	*storage = dfa.save();
	return result;
}

template <typename Dfa>
UtfDecodeResult<utf16_t>
Utf16DecoderBase<Dfa>::decode(
	Dfa& dfa,
	utf32_t* dst,
	utf32_t* dstEnd,
	const utf16_t* src,
	const utf16_t* srcEnd,
	utf32_t replacement
) {
	ASSERT(dstEnd - dst >= 2);
	dstEnd--;

	if (Dfa::IsReverse()())
		for (; dst < dstEnd && src > srcEnd; src--) {
			uint_t state = dfa.count(*src);
			if (Dfa::isError(state))
				*dst++ = replacement;

			if (Dfa::isReady(state))
				*dst++ = dfa.getCp();
		}
	else
		for (; dst < dstEnd && src < srcEnd; src++) {
			uint_t state = dfa.count(*src);
			if (Dfa::isError(state))
				*dst++ = replacement;

			if (Dfa::isReady(state))
				*dst++ = dfa.getCp();
		}

	return DecodeResult(dst, src);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf16DecoderBase<Utf16Dfa>           Utf16Decoder;
typedef Utf16DecoderBase<Utf16ReverseDfa>    Utf16ReverseDecoder;
typedef Utf16DecoderBase<Utf16Dfa_be>        Utf16Decoder_be;
typedef Utf16DecoderBase<Utf16ReverseDfa_be> Utf16ReverseDecoder_be;

//..............................................................................

} // namespace enc
} // namespace axl
