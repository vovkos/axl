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

#define _AXL_ENC_UTF8DECODER_H

#include "axl_enc_Unicode.h"
#include "axl_enc_Utf8Dfa.h"

namespace axl {
namespace enc {

// emits a replacement char per each byte of an invalid UTF-8 sequence

//..............................................................................

template <typename Dfa0>
class Utf8DecoderBase {
public:
	typedef Dfa0 Dfa;
	typedef UtfDecodeResult<utf8_t> DecodeResult;
	typedef uint32_t Storage;

public:
	size_t
	calcCodePointCount(
		Storage* storage,
		const utf8_t* p,
		const utf8_t* end
	);

	size_t
	calcCodePointCount(
		const utf8_t* p,
		const utf8_t* end
	) {
		return calcCodePointCount(Dfa(), p, end);
	}

	DecodeResult
	decode(
		Storage* storage,
		utf32_t* dst, // utf32_t[4 or more]
		utf32_t* dstEnd,
		const utf8_t* src,
		const utf8_t* srcEnd,
		utf32_t replacement = StdChar_Replacement
	);

	DecodeResult
	decode(
		utf32_t* dst, // utf32_t[4 or more]
		utf32_t* dstEnd,
		const utf8_t* src,
		const utf8_t* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		return decode(Dfa(), dst, dstEnd, src, srcEnd, replacement);
	}

protected:
	size_t
	calcCodePointCount(
		Dfa& dfa,
		const utf8_t* p,
		const utf8_t* end
	);

	DecodeResult
	decode(
		Dfa& dfa,
		utf32_t* dst,
		utf32_t* dstEnd,
		const utf8_t* src,
		const utf8_t* srcEnd,
		utf32_t replacement
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename Dfa>
size_t
Utf8DecoderBase<Dfa>::calcCodePointCount(
	Storage* storage,
	const utf8_t* p,
	const utf8_t* end
) {
	Dfa dfa(*storage);
	size_t count = calcCodePointCount(dfa, p, end);
	*storage = dfa.save();
	return count;
}

template <typename Dfa>
size_t
Utf8DecoderBase<Dfa>::calcCodePointCount(
	Dfa& dfa,
	const utf8_t* src,
	const utf8_t* srcEnd
) {
	size_t count = 0;

	if (Dfa::IsReverse) {
		for (const utf8_t* p = src; src > srcEnd; src--) {
			uint_t state = dfa.count(*src);
			if (Dfa::isError(state))
				if (state == Dfa::State_Error)
					do {
						count++;
					} while (--p >= src);
				else {
					const char* p0 = p - Dfa::getCombinedErrorCount(state);
					do {
						count++;
					} while (--p > p0);
				}

			if (Dfa::isReady(state)) {
				count++;
				p = src - 1;
			}
		}
	} else {
		for (const utf8_t* p = src; src < srcEnd; src++) {
			uint_t state = dfa.count(*src);
			if (Dfa::isError(state))
				if (state == Dfa::State_Error)
					do {
						count++;
					} while (++p <= src);
				else
					do {
						count++;
					} while (++p < src);

			if (Dfa::isReady(state)) {
				count++;
				p = src + 1;
			}
		}
	}

	return count;
}

template <typename Dfa>
UtfDecodeResult<utf8_t>
Utf8DecoderBase<Dfa>::decode(
	Storage* storage,
	utf32_t* dst,
	utf32_t* dstEnd,
	const utf8_t* src,
	const utf8_t* srcEnd,
	utf32_t replacement
) {
	Dfa dfa(*storage);
	DecodeResult result = decode(dfa, dst, dstEnd, src, srcEnd, replacement);
	*storage = dfa.save();
	return result;
}

template <typename Dfa>
UtfDecodeResult<utf8_t>
Utf8DecoderBase<Dfa>::decode(
	Dfa& dfa,
	utf32_t* dst,
	utf32_t* dstEnd,
	const utf8_t* src,
	const utf8_t* srcEnd,
	utf32_t replacement
) {
	ASSERT(dstEnd - dst >= 4);
	dstEnd -= 3;

	if (Dfa::IsReverse) {
		for (const utf8_t* p = src; dst < dstEnd && src > srcEnd; src--) {
			uint_t state = dfa.count(*src);
			if (Dfa::isError(state))
				if (state == Dfa::State_Error)
					do {
						*dst++ = replacement;
					} while (--p >= src);
				else {
					const char* p0 = p - Dfa::getCombinedErrorCount(state);
					do {
						*dst++ = replacement;
					} while (--p > src);
				}

			if (Dfa::isReady(state)) {
				*dst++ = dfa.getCp();
				p = src - 1;
			}
		}
	} else {
		for (const utf8_t* p = src; dst < dstEnd && src < srcEnd; src++) {
			uint_t state = dfa.count(*src);
			if (Dfa::isError(state))
				if (state == Dfa::State_Error)
					do {
						*dst++ = replacement;
					} while (++p <= src);
				else
					do {
						*dst++ = replacement;
					} while (++p < src);

			if (Dfa::isReady(state)) {
				*dst++ = dfa.getCp();
				p = src + 1;
			}
		}
	}

	return DecodeResult(dst, src);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf8DecoderBase<Utf8Dfa>        Utf8Decoder;
typedef Utf8DecoderBase<Utf8ReverseDfa> Utf8ReverseDecoder;

//..............................................................................

} // namespace enc
} // namespace axl
