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

#define _AXL_ENC_ASCII_H

#include "axl_enc_Unicode.h"
#include "axl_sl_Operator.h"

namespace axl {
namespace enc {

//..............................................................................

class AsciiEncoder {
public:
	enum {
		MaxEncodeLength = 1 // always one 8-bit codeunit per codepoint
	};

	typedef char C;

public:
	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t unused = 0
	) {
		return 1;
	}

	static
	char*
	encode(
		char* p,
		utf32_t cp,
		utf32_t unused = 0
	) {
		*p = (char)cp;
		return p + 1;
	}
};

//..............................................................................

template <typename IsReverse>
class AsciiDecoderBase {
public:
	enum {
		MaxEmitLength = 1,
	};

	typedef char C;

public:
	static
	size_t
	getPendingLength(DecoderState unused) {
		return 0;
	}

	template <typename Emitter>
	static
	const C*
	decode(
		DecoderState* unused,
		Emitter& action,
		const C* src,
		const C* srcEnd
	) {
		return decode(action, src, srcEnd);
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
				utf32_t cp = (uchar_t)*src--;
				emitter.emitCp(src, cp);
			}
		else
			while (src < srcEnd && emitter.canEmit()) {
				utf32_t cp = (uchar_t)*src++;
				emitter.emitCp(src, cp);
			}

		return src;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef AsciiDecoderBase<sl::False> AsciiDecoder;
typedef AsciiDecoderBase<sl::True>  AsciiReverseDecoder;

//..............................................................................

class Ascii {
public:
	typedef char C;
	typedef AsciiEncoder Encoder;
	typedef AsciiDecoder Decoder;
	typedef AsciiReverseDecoder ReverseDecoder;
};

//..............................................................................

} // namespace enc
} // namespace axl
