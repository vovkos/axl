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
	encode(
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

public:
	template <typename Emitter>
	static
	const C*
	decode(
		DecoderState* unused,
		Emitter& emitter,
		const C* src,
		const C* srcEnd
	) {
		return decode(emitter, src, srcEnd);
	}

	template <typename Emitter>
	static
	const C*
	decode(
		Emitter& emitter,
		const C* src,
		const C* srcEnd
	) {
		if (IsReverse()())
			while (src > srcEnd && emitter.canEmit()) {
				utf32_t c = *src--;
				emitter.emitCodePoint(src, IsBigEndian()() ? sl::swapByteOrder32(c) : c);
			}
		else
			while (src < srcEnd && emitter.canEmit()) {
				utf32_t c = *src++;
				emitter.emitCodePoint(src, IsBigEndian()() ? sl::swapByteOrder32(c) : c);
			}

		return src;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf32DecoderBase<sl::False, sl::False> Utf32Decoder;
typedef Utf32DecoderBase<sl::True, sl::False>  Utf32ReverseDecoder;
typedef Utf32DecoderBase<sl::False, sl::True>  Utf32Decoder_be;
typedef Utf32DecoderBase<sl::True, sl::True>   Utf32ReverseDecoder_be;

//..............................................................................

class Utf32 {
public:
	typedef utf32_t C;
	typedef Utf32Encoder Encoder;
	typedef Utf32Decoder Decoder;
	typedef Utf32ReverseDecoder ReverseDecoder;

public:
	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xff, 0xfe, 0x00, 0x00 };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 4;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class Utf32_be {
public:
	typedef utf32_t C;
	typedef Utf32Encoder Encoder;
	typedef Utf32Decoder Decoder;
	typedef Utf32ReverseDecoder ReverseDecoder;

public:
	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0x00, 0x00, 0xfe, 0xff };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 4;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
