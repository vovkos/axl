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

#define _AXL_ENC_UNICODE_H

#include "axl_sl_Operator.h"

namespace axl {
namespace enc {

//..............................................................................

enum StdChar {
	StdChar_Undefined   = 0xffff,
	StdChar_Replacement = 0xfffd,
};

//..............................................................................

bool
isPrintable(utf32_t c);

bool
isPrintableNonMark(utf32_t c);

bool
isSpace(utf32_t c);

bool
isPunctuation(utf32_t c);

bool
isLetter(utf32_t c);

bool
isDigit(utf32_t c);

bool
isNumber(utf32_t c);

bool
isLetterOrDigit(utf32_t c);

bool
isLetterOrNumber(utf32_t c);

bool
isLowerCase(utf32_t c);

bool
isUpperCase(utf32_t c);

//..............................................................................

// case ops

utf32_t
toLowerCase(utf32_t c);

utf32_t
toUpperCase(utf32_t c);

utf32_t
toCaseFolded(utf32_t c);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ToLowerCase {
public:
	utf32_t
	operator () (utf32_t x) const {
		return toLowerCase(x);
	}
};

class ToUpperCase {
public:
	utf32_t
	operator () (utf32_t x) const {
		return toUpperCase(x);
	}
};

class ToCaseFolded {
public:
	utf32_t
	operator () (utf32_t x) const {
		return toCaseFolded(x);
	}
};

//..............................................................................

// common result structure for encoders/decoders/converters

template <
	typename DstUnit,
	typename SrcUnit
>
struct EncodeResult {
	DstUnit* m_dst;
	const SrcUnit* m_src;

	EncodeResult() {
		m_dst = NULL;
		m_src = NULL;
	}

	EncodeResult(
		DstUnit* dst,
		const SrcUnit* src
	) {
		m_dst = dst;
		m_src = src;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct EncodeLengthResult {
	size_t m_dstLength; // in code-units
	size_t m_srcLength; // in code-units

	EncodeLengthResult() {
		m_dstLength = 0;
		m_srcLength = 0;
	}

	EncodeLengthResult(
		size_t dstLength,
		size_t srcLength
	) {
		m_dstLength = dstLength;
		m_srcLength = srcLength;
	}
};

//..............................................................................

// apply a per-code-point operation before encoding

template <
	typename EncoderBase,
	typename Op // e.g. ToLowerCase, ToUpperCase, ToCaseFold
>
class OpEncoder: public EncoderBase {
public:
	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t replacement = StdChar_Replacement
	) {
		return EncoderBase::getEncodeLength(Op()(cp), replacement);
	}

	static
	typename EncoderBase::C*
	encode(
		typename EncoderBase::C* p,
		utf32_t cp,
		utf32_t replacement = StdChar_Replacement
	) {
		return EncoderBase::encode(p, Op()(cp), replacement);
	}
};

//..............................................................................
/*

// Unicode char encoder interface:

class Encoder {
public:
	enum {
		MaxEncodeLength, // maximum number of code units per code point
	};

	typedef int C;

public:
	static
	size_t
	getEncodeLength(
		utf32_t cp,
		utf32_t replacement = StdChar_Replacement
	);

	static
	C*
	encode(
		C* p,
		utf32_t cp,
		utf32_t replacement = StdChar_Replacement
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// Unicode char decoder interface:

class Decoder {
public:
	enum {
		MaxEmitLength,   // maximum number of code points emitted at once
	};

	typedef int C;     // code unit type
	typedef int State; // decoder state storage (if any)

public:
	template <typename Encoder>
	static
	size_t
	count(
		State* state,
		const C* p,
		const C* end,
		utf32_t replacement = StdChar_Replacement
	);

	template <typename Encoder>
	static
	size_t
	count(
		const C* p,
		const C* end,
		utf32_t replacement = StdChar_Replacement
	);

	template <typename Encoder>
	static
	EncodeResult<typename Encoder::C, C>
	decode(
		State* state,
		typename Encoder::C* dst,
		typename Encoder::C* dstEnd,
		const C* src,
		const C* srcEnd,
		utf32_t replacement = StdChar_Replacement
	);

	template <typename Encoder>
	static
	EncodeResult<typename Encoder::C, C>
	decode(
		typename Encoder::C* dst,
		typename Encoder::C* dstEnd,
		const C* src,
		const C* srcEnd,
		utf32_t replacement = StdChar_Replacement
	);
};

*/
//..............................................................................

template <
	typename DstEncoding0,
	typename SrcEncoding0,
	typename Op = sl::NoOp<utf32_t> // e.g. ToLowerCase, ToUpperCase, ToCaseFold
>
class Convert {
public:
	typedef DstEncoding0 DstEncoding;
	typedef SrcEncoding0 SrcEncoding;
	typedef typename DstEncoding::C DstUnit;
	typedef typename SrcEncoding::C SrcUnit;
	typedef OpEncoder<typename DstEncoding::Encoder, Op> Encoder;
	typedef typename SrcEncoding::Decoder Decoder;
	typedef typename SrcEncoding::Decoder::State DecoderState;
	typedef EncodeResult<DstUnit, SrcUnit> ConvertResult;

public:
	static
	size_t
	calcRequiredLength(
		DecoderState* state,
		const SrcUnit* p,
		const SrcUnit* end,
		utf32_t replacement = StdChar_Replacement
	) {
		return Decoder::count<Encoder>(state, p, end, replacement);
	}

	static
	size_t
	calcRequiredLength(
		DecoderState* state,
		const SrcUnit* p,
		size_t length,
		utf32_t replacement = StdChar_Replacement
	) {
		return Decoder::count<Encoder>(state, p, p + length, replacement);
	}

	static
	size_t
	calcRequiredLength(
		const SrcUnit* p,
		const SrcUnit* end,
		utf32_t replacement = StdChar_Replacement
	) {
		return Decoder::count<Encoder>(p, end, replacement);
	}

	static
	size_t
	calcRequiredLength(
		const SrcUnit* p,
		size_t length,
		utf32_t replacement = StdChar_Replacement
	) {
		return Decoder::count<Encoder>(p, p + length, replacement);
	}

	static
	ConvertResult
	convert(
		DecoderState* state,
		DstUnit* dst,
		DstUnit* dstEnd,
		const SrcUnit* src,
		const SrcUnit* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		return Decoder::decode<Encoder>(state, dst, dstEnd, src, srcEnd, replacement);
	}

	static
	EncodeLengthResult
	convert(
		DecoderState* state,
		DstUnit* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult result = Decoder::decode<Encoder>(state, dst, dst + dstLength, src, src + srcLength, replacement);
		return EncodeLengthResult(result.m_dst - dst, result.m_src - src);
	}

	static
	ConvertResult
	convert(
		DstUnit* dst,
		DstUnit* dstEnd,
		const SrcUnit* src,
		const SrcUnit* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		return Decoder::decode<Encoder>(dst, dstEnd, src, srcEnd, replacement);
	}

	static
	EncodeLengthResult
	convert(
		DstUnit* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult result = Decoder::decode<Encoder>(dst, dst + dstLength, src, src + srcLength, replacement);
		return EncodeLengthResult(result.m_dst - dst, result.m_src - src);
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
