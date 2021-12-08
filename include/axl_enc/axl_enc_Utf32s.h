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

#include "axl_enc_Utf32.h"
#include "axl_enc_Utf32sDfa.h"

namespace axl {
namespace enc {

//..............................................................................

class Utf32sEncoderBase {
public:
	enum {
		MaxEncodeLength = 4 // always four 8-bit codeunits per codepoint
	};

	typedef char C;

public:
	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t unused = 0
	) {
		return 4;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32sEncoder: public Utf32sEncoderBase {
public:
	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		return (C*)Utf32Encoder::encode((utf32_t*)p, cp);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32sEncoder_be: public Utf32sEncoderBase {
public:
	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		return (C*)Utf32Encoder_be::encode((utf32_t*)p, cp);
	}
};

//..............................................................................

template <
	typename IsReverse,
	typename Dfa0
>
class Utf32sDecoderBase {
public:
	enum {
		MaxEmitLength = 1, // always one codepoint
	};

	typedef char C;
	typedef Dfa0 Dfa;

public:
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
		return decode(Dfa(), emitter, src, srcEnd);
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
		if (IsReverse()())
			while (src > srcEnd && emitter.canEmit()) {
				C c = *src--;
				uint_t state = dfa.decode(c);
				if (Dfa::isReady(state))
					emitter.emitCodePoint(src, dfa.getCodePoint());
			}
		else
			while (src < srcEnd && emitter.canEmit()) {
				C c = *src++;
				uint_t state = dfa.decode(c);
				if (Dfa::isReady(state))
					emitter.emitCodePoint(src, dfa.getCodePoint());
			}

		return src;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf32sDecoderBase<sl::False, Utf32sDfa>    Utf32sDecoder;
typedef Utf32sDecoderBase<sl::False, Utf32sDfa_be> Utf32sDecoder_be;
typedef Utf32sDecoderBase<sl::True,  Utf32sDfa_be> Utf32sReverseDecoder;
typedef Utf32sDecoderBase<sl::True,  Utf32sDfa>    Utf32sReverseDecoder_be;

//..............................................................................

class Utf32s {
public:
	typedef char C;
	typedef Utf32sEncoder Encoder;
	typedef Utf32sDecoder Decoder;
	typedef Utf32sReverseDecoder ReverseDecoder;

public:
	static
	const uint8_t*
	getBom() {
		return Utf32::getBom();
	}

	static
	size_t
	getBomLength() {
		return Utf32::getBomLength();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32s_be {
public:
	typedef char C;
	typedef Utf32sEncoder_be Encoder;
	typedef Utf32sDecoder_be Decoder;
	typedef Utf32sReverseDecoder_be ReverseDecoder;

public:
	static
	const uint8_t*
	getBom() {
		return Utf32_be::getBom();
	}

	static
	size_t
	getBomLength() {
		return Utf32_be::getBomLength();
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
