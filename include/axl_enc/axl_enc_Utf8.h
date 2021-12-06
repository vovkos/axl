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

#define _AXL_ENC_UTF8_H

#include "axl_enc_Unicode.h"
#include "axl_enc_Utf8Dfa.h"

namespace axl {
namespace enc {

//..............................................................................

class Utf8Encoder {
public:
	enum {
		MaxEncodeLength = 4 // no more than 4 bytes per codepoint
	};

	typedef utf8_t C;

public:
	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t replacement = StdChar_Replacement
	) {
		return
			cp < 0x80 ? 1 :      // 0xxxxxxx
			cp < 0x10000 ?
				cp < 0x800 ? 2 : // 110xxxxx 10xxxxxx
				3 :              // 1110xxxx 10xxxxxx 10xxxxxx
			cp < 0x200000 ? 4 :  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			getEncodeLength(replacement);
	}

	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t replacement = StdChar_Replacement
	) {
		if (cp < 0x80) {
			p[0] = (uint8_t)cp;                             // 0xxxxxxx
			return p + 1;
		} else if (cp < 0x10000) {
			if (cp < 0x800) {
				p[0] = (uint8_t)(cp >> 6)          | 0xc0;  // 110xxxxx 10xxxxxx
				p[1] = (uint8_t)(cp & 0x3f)        | 0x80;
				return p + 2;
			} else {
				p[0] = (uint8_t)(cp >> 12)         | 0xe0;  // 1110xxxx 10xxxxxx 10xxxxxx
				p[1] = (uint8_t)((cp >> 6) & 0x3f) | 0x80;
				p[2] = (uint8_t)(cp & 0x3f)        | 0x80;
				return p + 3;
			}
		} else if (cp < 0x200000) {
			p[0] = (uint8_t)(cp >> 18)             | 0xf0; // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
			p[1] = (uint8_t)((cp >> 12) & 0x3f)    | 0x80;
			p[2] = (uint8_t)((cp >> 6) & 0x3f)     | 0x80;
			p[3] = (uint8_t)(cp & 0x3f)            | 0x80;
			return p + 4;
		} else {
			return encode(p, replacement);
		}
	}
};

//..............................................................................

// emits a replacement char per each byte of an invalid UTF-8 sequence

template <typename Dfa0>
class Utf8DecoderBase {
public:
	enum {
		MaxEmitLength = 4, // 4 replacements (or 3 replacement + codepoint)
	};

	typedef utf8_t C;
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
		typename Encoder::C* dst, // provide room to encode at least 4 codepoints
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
		typename Encoder::C* dst, // provide room to encode at least 4 codepoints
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
			for (const C* p = src; src > srcEnd; src--) {
				uint_t state = dfa.count(*src);
				if (Dfa::isError(state))
					if (state == Dfa::State_Error)
						do {
							length += Encoder::getEncodeLength(replacement);
						} while (--p >= src);
					else {
						const C* p0 = p - Dfa::getCombinedErrorCount(state);
						do {
							length += Encoder::getEncodeLength(replacement);
						} while (--p > p0);
					}

				if (Dfa::isReady(state)) {
					length += Encoder::getEncodeLength(dfa.getCodePoint(), replacement);
					p = src - 1;
				}
			}
		else
			for (const C* p = src; src < srcEnd; src++) {
				uint_t state = dfa.count(*src);
				if (Dfa::isError(state))
					if (state == Dfa::State_Error)
						do {
							length += Encoder::getEncodeLength(replacement);
						} while (++p <= src);
					else
						do {
							length += Encoder::getEncodeLength(replacement);
						} while (++p < src);

				if (Dfa::isReady(state)) {
					length += Encoder::getEncodeLength(dfa.getCodePoint(), replacement);
					p = src + 1;
				}
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
			for (const C* p = src; dst < dstEnd && src > srcEnd; src--) {
				uint_t state = dfa.count(*src);
				if (Dfa::isError(state))
					if (state == Dfa::State_Error)
						do {
							dst = Encoder::encode(dst, replacement);
						} while (--p >= src);
					else {
						const C* p0 = p - Dfa::getCombinedErrorCount(state);
						do {
							dst = Encoder::encode(dst, replacement);
						} while (--p > src);
					}

				if (Dfa::isReady(state)) {
					dst = Encoder::encode(dst, dfa.getCodePoint(), replacement);
					p = src - 1;
				}
			}
		else
			for (const C* p = src; dst < dstEnd && src < srcEnd; src++) {
				uint_t state = dfa.count(*src);
				if (Dfa::isError(state))
					if (state == Dfa::State_Error)
						do {
							dst = Encoder::encode(dst, replacement);
						} while (++p <= src);
					else
						do {
							dst = Encoder::encode(dst, replacement);
						} while (++p < src);

				if (Dfa::isReady(state)) {
					dst = Encoder::encode(dst, dfa.getCodePoint(), replacement);
					p = src + 1;
				}
			}

		return EncodeResult<typename Encoder::C, C>(dst, src);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf8DecoderBase<Utf8Dfa>        Utf8Decoder;
typedef Utf8DecoderBase<Utf8ReverseDfa> Utf8ReverseDecoder;

//..............................................................................

} // namespace enc
} // namespace axl
