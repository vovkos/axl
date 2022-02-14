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
	bool
	isSurrogate(uint32_t cp) {
		return cp - 0xd800 < 0x800;
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
		utf32_t replacement = StdChar_Replacement
	) {
		ASSERT(!isSurrogate(replacement));

		return
			isSurrogateNeeded(cp) ? 2 :
			isSurrogate(cp) ? getEncodeLength(replacement) :
			1;
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
		utf32_t replacement = StdChar_Replacement
	) {
		ASSERT(!isSurrogate(replacement));

		if (isSurrogateNeeded(cp)) {
			p[0] = getHiSurrogate(cp);
			p[1] = getLoSurrogate(cp);
			return p + 2;
		} else if (isSurrogate(cp)) {
			return encode(p, replacement);
		} else {
			*p = (utf16_t)cp;
			return p + 1;
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
		utf32_t replacement = StdChar_Replacement
	) {
		ASSERT(!isSurrogate(replacement));

		if (isSurrogateNeeded(cp)) {
			p[0] = sl::swapByteOrder16(getHiSurrogate(cp));
			p[1] = sl::swapByteOrder16(getLoSurrogate(cp));
			return p + 2;
		} else if (isSurrogate(cp)) {
			return encode(p, replacement);
		} else {
			*p = sl::swapByteOrder16((utf16_t)cp);
			return p + 1;
		}
	}
};

//..............................................................................

// emits a replacement per each unpaired surrogate

template <
	typename C0,
	typename Dfa0
>
class Utf16DecoderBase {
public:
	enum {
		MaxEmitLength = 2, // replacement + cp
	};

	typedef C0 C;
	typedef Dfa0 Dfa;

public:
	static
	size_t
	getPendingLength(DecoderState state) {
		return Dfa::getPendingLength(state);
	}

	template <typename Emitter>
	static
	const C*
	decode(
		DecoderState* state,
		Emitter& emitter,
		const C* src,
		const C* srcEnd
	) {
		Dfa dfa(*state);
		src = decode(dfa, emitter, src, srcEnd);
		*state = dfa.save();
		return src;
	}

	template <typename Emitter>
	static
	const C*
	decode(
		Emitter& emitter,
		const C* src,
		const C* srcEnd
	) {
		Dfa dfa;
		return decode(dfa, emitter, src, srcEnd);
	}

protected:
	template <typename Emitter>
	static
	const C*
	decode(
		Dfa& dfa,
		Emitter& emitter,
		const C* src,
		const C* srcEnd
	) {
		if (Dfa::IsReverse)
			while (src > srcEnd && emitter.canEmit()) {
				C c = *src--;
				uint_t state = dfa.decode(c);
				if (Dfa::isError(state))
					emitter.emitReplacement(src);

				if (Dfa::isReady(state))
					emitter.emitCodePoint(src, dfa.getCodePoint());
			}
		else
			while (src < srcEnd && emitter.canEmit()) {
				C c = *src++;
				uint_t state = dfa.decode(c);
				if (Dfa::isError(state))
					emitter.emitReplacement(src);

				if (Dfa::isReady(state))
					emitter.emitCodePoint(src, dfa.getCodePoint());
			}

		return src;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf16DecoderBase<utf16_t, Utf16Dfa>           Utf16Decoder;
typedef Utf16DecoderBase<utf16_t, Utf16ReverseDfa>    Utf16ReverseDecoder;
typedef Utf16DecoderBase<utf16_t, Utf16Dfa_be>        Utf16Decoder_be;
typedef Utf16DecoderBase<utf16_t, Utf16ReverseDfa_be> Utf16ReverseDecoder_be;

//..............................................................................

class Utf16 {
public:
	typedef utf16_t C;
	typedef Utf16Encoder Encoder;
	typedef Utf16Decoder Decoder;
	typedef Utf16ReverseDecoder ReverseDecoder;

public:
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

//..............................................................................

} // namespace enc
} // namespace axl
