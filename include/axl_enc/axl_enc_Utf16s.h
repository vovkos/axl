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

#define _AXL_ENC_UTF16S_H

#include "axl_enc_Utf16.h"
#include "axl_enc_Utf16sDfa.h"

// UTF-16-S is a stream-based encoder/decoder for UTF-16

namespace axl {
namespace enc {

//..............................................................................

class Utf16sEncoderBase: public Utf16EncoderBase {
public:
	enum {
		MaxEncodeLength = 4 // no more than four 8-bit codeunits per codepoint
	};

	typedef char C;

public:
	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t unused = 0
	) {
		return Utf16EncoderBase::getEncodeLength(cp) * 2;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16sEncoder: public Utf16sEncoderBase {
public:
	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		return (C*)Utf16Encoder::encode((utf16_t*)p, cp);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16sEncoder_be: public Utf16sEncoderBase {
public:
	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		return (C*)Utf16Encoder_be::encode((utf16_t*)p, cp);
	}
};

//..............................................................................

template <typename Dfa0>
class Utf16sDecoderImpl {
public:
	typedef char C;
	typedef Dfa0 Dfa;

public:
	template <typename Emitter>
	static
	const C*
	decode(
		Dfa& dfa,
		Emitter& emitter,
		const C* p,
		const C* end
	) {
		if (Dfa::IsReverse)
			for (; p > end && emitter.canEmit(); p--) {
				uchar_t c = *p;
				Dfa next = dfa.decode(c);
				if (next.isError()) {
					if (dfa.getPendingLength() >= 2)
						emitter.emitCu(p + 1, dfa.getCodePoint() & 0xffff);
					if (next.getState() == Dfa::State_Error)
						emitter.emitCu(p - 1, next.getCodePoint() & 0xffff);
					else if (next.isReady())
						emitter.emitCpAfterCu(p - 1, next.getCodePoint());
				} else if (next.isReady())
					emitter.emitCp(p - 1, next.getCodePoint());

				dfa = next;
			}
		else
			for (; p < end && emitter.canEmit(); p++) {
				uchar_t c = *p;
				Dfa next = dfa.decode(c);
				if (next.isError()) {
					if (dfa.getPendingLength() >= 2)
						emitter.emitCu(p - 1, dfa.getCodePoint() & 0xffff);
					if (next.getState() == Dfa::State_Error)
						emitter.emitCu(p + 1, next.getCodePoint() & 0xffff);
					else if (next.isReady())
						emitter.emitCpAfterCu(p + 1, next.getCodePoint());
				} else if (next.isReady())
					emitter.emitCp(p + 1, next.getCodePoint());

				dfa = next;
			}

		return p;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf16DecoderBase<Utf16sDecoderImpl<Utf16sDfa> >           Utf16sDecoder;
typedef Utf16DecoderBase<Utf16sDecoderImpl<Utf16sReverseDfa> >    Utf16sReverseDecoder;
typedef Utf16DecoderBase<Utf16sDecoderImpl<Utf16sDfa_be> >        Utf16sDecoder_be;
typedef Utf16DecoderBase<Utf16sDecoderImpl<Utf16sReverseDfa_be> > Utf16sReverseDecoder_be;

//..............................................................................

class Utf16s {
public:
	typedef char C;
	typedef Utf16sEncoder Encoder;
	typedef Utf16sDecoder Decoder;
	typedef Utf16sReverseDecoder ReverseDecoder;

public:
	static
	const uint8_t*
	getBom() {
		return Utf16::getBom();
	}

	static
	size_t
	getBomLength() {
		return Utf16::getBomLength();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf16s_be {
public:
	typedef char C;
	typedef Utf16sEncoder_be Encoder;
	typedef Utf16sDecoder_be Decoder;
	typedef Utf16sReverseDecoder_be ReverseDecoder;

public:
	static
	const uint8_t*
	getBom() {
		return Utf16_be::getBom();
	}

	static
	size_t
	getBomLength() {
		return Utf16_be::getBomLength();
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
