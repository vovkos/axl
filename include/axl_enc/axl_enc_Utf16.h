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

#define _AXL_ENC_UTF16_H

#include "axl_enc_Unicode.h"
#include "axl_enc_Utf16Dfa.h"
#include "axl_sl_ByteOrder.h"

namespace axl {
namespace enc {

//..............................................................................

class Utf16EncoderBase {
public:
	enum {
		MaxEncodeLength = 2 // no more than two 16-bit codeunits per codepoint
	};

	typedef utf16_t C;

public:
	static
	bool
	isSurrogateNeeded(uint32_t cp) {
		return cp >= 0x10000;
	}

	static
	utf16_t
	getHiSurrogate(uint32_t cp) {
		return 0xd800 + (((cp - 0x10000) >> 10) & 0x3ff);
	}

	static
	utf16_t
	getLoSurrogate(uint32_t cp) {
		return 0xdc00 + (cp & 0x3ff);
	}

	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t unused = 0
	) {
		return isSurrogateNeeded(cp) ? 2 : 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16Encoder: public Utf16EncoderBase {
public:
	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		if (!isSurrogateNeeded(cp)) {
			*p = (utf16_t)cp;
			return p + 1;
		} else {
			p[0] = getHiSurrogate(cp);
			p[1] = getLoSurrogate(cp);
			return p + 2;
		}
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16Encoder_be: public Utf16EncoderBase {
public:
	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		if (!isSurrogateNeeded(cp)) {
			*p = sl::swapByteOrder16((utf16_t)cp);
			return p + 1;
		} else {
			p[0] = sl::swapByteOrder16(getHiSurrogate(cp));
			p[1] = sl::swapByteOrder16(getLoSurrogate(cp));
			return p + 2;
		}
	}
};

//..............................................................................

// emits a replacement char per each unpaired surrogate

template <typename Dfa0>
class Utf16DecoderBase {
public:
	enum {
		MaxEmitLength = 2, // replacement + cp
	};

	typedef utf16_t C;
	typedef Dfa0 Dfa;
	typedef uint32_t State;

public:
	template <typename Encoder>
	static
	size_t
	count(
		State* state,
		const C* p,
		const C* end,
		utf32_t replacement = StdChar_Replacement
	) {
		Dfa dfa(*state);
		size_t result = count<Encoder>(dfa, p, end, replacement);
		*state = dfa.save();
		return result;
	}

	template <typename Encoder>
	static
	size_t
	count(
		const C* p,
		const C* end,
		utf32_t replacement = StdChar_Replacement
	) {
		return count<Encoder>(Dfa(), p, end, replacement);
	}

	template <typename Encoder>
	static
	EncodeResult<typename Encoder::C, C>
	decode(
		State* state,
		typename Encoder::C* dst, // provide room to encode at least 2 codepoints
		typename Encoder::C* dstEnd,
		const C* src,
		const C* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		Dfa dfa(*state);
		EncodeResult<typename Encoder::C, C> result = decode<Encoder>(dfa, dst, dstEnd, src, srcEnd, replacement);
		*state = dfa.save();
		return result;
	}

	template <typename Encoder>
	static
	EncodeResult<typename Encoder::C, C>
	decode(
		typename Encoder::C* dst, // provide room to encode at least 2 codepoints
		typename Encoder::C* dstEnd,
		const C* src,
		const C* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		return decode<Encoder>(Dfa(), dst, dstEnd, src, srcEnd, replacement);
	}

protected:
	template <typename Encoder>
	static
	size_t
	count(
		Dfa& dfa,
		const C* src,
		const C* srcEnd,
		utf32_t replacement
	) {
		size_t length = 0;

		if (Dfa::IsReverse)
			for (; src > srcEnd; src--) {
				uint_t state = dfa.count(*src);
				if (Dfa::isError(state))
					length += Encoder::getEncodeLength(replacement);

				if (Dfa::isReady(state))
					length += Encoder::getEncodeLength(dfa.getCodePoint(), replacement);
			}
		else
			for (; src < srcEnd; src++) {
				uint_t state = dfa.count(*src);
				if (Dfa::isError(state))
					length += Encoder::getEncodeLength(replacement);

				if (Dfa::isReady(state))
					length += Encoder::getEncodeLength(dfa.getCodePoint(), replacement);
			}

		return length;
	}

	template <typename Encoder>
	static
	EncodeResult<typename Encoder::C, C>
	decode(
		Dfa& dfa,
		typename Encoder::C* dst,
		typename Encoder::C* dstEnd,
		const C* src,
		const C* srcEnd,
		utf32_t replacement
	) {
		ASSERT(dstEnd - dst >= MaxEmitLength * Encoder::MaxEncodeLength);
		dstEnd -= MaxEmitLength * Encoder::MaxEncodeLength - 1;

		if (Dfa::IsReverse)
			for (; dst < dstEnd && src > srcEnd; src--) {
				uint_t state = dfa.count(*src);
				if (Dfa::isError(state))
					dst = Encoder::encode(dst, replacement);

				if (Dfa::isReady(state))
					dst = Encoder::encode(dst, dfa.getCodePoint(), replacement);
			}
		else
			for (; dst < dstEnd && src < srcEnd; src++) {
				uint_t state = dfa.count(*src);
				if (Dfa::isError(state))
					dst = Encoder::encode(dst, replacement);

				if (Dfa::isReady(state))
					dst = Encoder::encode(dst, dfa.getCodePoint(), replacement);
			}

		return EncodeResult<typename Encoder::C, C>(dst, src);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf16DecoderBase<Utf16Dfa>           Utf16Decoder;
typedef Utf16DecoderBase<Utf16ReverseDfa>    Utf16ReverseDecoder;
typedef Utf16DecoderBase<Utf16Dfa_be>        Utf16Decoder_be;
typedef Utf16DecoderBase<Utf16ReverseDfa_be> Utf16ReverseDecoder_be;

//..............................................................................

} // namespace enc
} // namespace axl
