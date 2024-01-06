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

#define _AXL_ENC_LATIN1_H

#include "axl_enc_Unicode.h"
#include "axl_sl_PtrIterator.h"

namespace axl {
namespace enc {

//..............................................................................

class Latin1Encoder {
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

template <bool IsReverse0>
class Latin1DecoderBase {
public:
	enum {
		MaxEmitLength = 1,
		IsReverse     = IsReverse0
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
	void
	emitPendingCus(
		DecoderState state,
		Emitter& emitter,
		const C* p
	) {
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
		const C* src0,
		const C* srcEnd
	) {
		sl::PtrIterator<const C, IsReverse> src = src0;
		while (src < srcEnd && emitter.canEmit()) {
			utf32_t cp = (uchar_t)*src++;
			emitter.emitCp(src, cp);
		}

		return src;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Latin1DecoderBase<false> Latin1Decoder;
typedef Latin1DecoderBase<true>  Latin1ReverseDecoder;

//..............................................................................

class Latin1 {
public:
	typedef char C;
	typedef Latin1Encoder Encoder;
	typedef Latin1Decoder Decoder;
	typedef Latin1ReverseDecoder ReverseDecoder;
};

//..............................................................................

} // namespace enc
} // namespace axl
