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
isWord(utf32_t c);

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

// the decoder state (if any) must fit in 32 bits

typedef uint32_t DecoderState;

//..............................................................................

/*

// Unicode char encoder interface:

class Encoder {
public:
	enum {
		MaxEncodeLength, // maximum number of code units per code point
	};

	typedef <code-unit-type> C;

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

// Unicode char decode emitter interface:

class Emitter {
public:
	bool
	canEmit() const;

	// invoked for each codepoint (return true to continue, false to halt)

	void
	emitCodePoint(
		const C* src,
		utf32_t cp
	);

	void
	emitReplacement(const C* src);
};

// Unicode char decoder interface:

class Decoder {
public:
	enum {
		MaxEmitLength, // maximum number of code points emitted at once
	};

	typedef <code-unit-type> C;

public:
	template <typename Emitter>
	static
	const C*
	decode(
		DecoderState* state,
		Emitter& emitter,
		const C* src,
		const C* srcEnd
	);

	template <typename Emitter>
	static
	const C*
	decode(
		Emitter& emitter,
		const C* src,
		const C* srcEnd
	);
};

*/

//..............................................................................

// common result structures for encoding converstions

template <
	typename DstUnit,
	typename SrcUnit
>
struct ConvertResult {
	DstUnit* m_dst;
	const SrcUnit* m_src;

	ConvertResult() {
		m_dst = NULL;
		m_src = NULL;
	}

	ConvertResult(
		DstUnit* dst,
		const SrcUnit* src
	) {
		m_dst = dst;
		m_src = src;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct ConvertLengthResult {
	size_t m_dstLength; // in code-units
	size_t m_srcLength; // in code-units

	ConvertLengthResult() {
		m_dstLength = 0;
		m_srcLength = 0;
	}

	ConvertLengthResult(
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
	typedef typename DstEncoding::Encoder Encoder;
	typedef typename SrcEncoding::Decoder Decoder;
	typedef ConvertResult<DstUnit, SrcUnit> ConvertResult;

	class CountingEmitter {
	protected:
		size_t m_length;
		utf32_t m_replacement;

	public:
		CountingEmitter(utf32_t replacement) {
			m_length = 0;
			m_replacement = replacement;
		}

		size_t
		getLength() const {
			return m_length;
		}

		bool
		canEmit() const {
			return true;
		}

		void
		emitCodePoint(
			const SrcUnit* p,
			utf32_t cp
		) {
			m_length += Encoder::getEncodeLength(Op()(cp), m_replacement);
		}

		void
		emitReplacement(const SrcUnit* p) {
			m_length += Encoder::getEncodeLength(m_replacement);
		}
	};

	class EncodingEmitter_u {
	protected:
		DstUnit* m_p;
		utf32_t m_replacement;

	public:
		EncodingEmitter_u(
			DstUnit* p,
			utf32_t replacement
		) {
			m_p = p;
			m_replacement = replacement;
		}

		DstUnit*
		p() const {
			return m_p;
		}

		bool
		canEmit() const {
			return true;
		}

		void
		emitCodePoint(
			const SrcUnit* p,
			utf32_t cp
		) {
			m_p = Encoder::encode(m_p, Op()(cp), m_replacement);
		}

		void
		emitReplacement(const SrcUnit* p) {
			m_p = Encoder::encode(m_p, m_replacement);
		}
	};

	class EncodingEmitter_s: public EncodingEmitter_u {
	protected:
		DstUnit* m_end;

	public:
		EncodingEmitter_s(
			DstUnit* p,
			DstUnit* end,
			utf32_t replacement
		): EncodingEmitter_u(p, replacement) {
			ASSERT(end - p >= Decoder::MaxEmitLength * Encoder::MaxEncodeLength);
			m_end = end - Decoder::MaxEmitLength * Encoder::MaxEncodeLength - 1;
		}

		bool
		canEmit() const {
			return m_p < m_end;
		}
	};

public:
	static
	size_t
	calcRequiredLength(
		DecoderState* state,
		const SrcUnit* p,
		const SrcUnit* end,
		utf32_t replacement = StdChar_Replacement
	) {
		CountingEmitter emitter(replacement);
		Decoder::decode(state, emitter, p, end);
		return emitter.getLength();
	}

	static
	size_t
	calcRequiredLength(
		DecoderState* state,
		const SrcUnit* p,
		size_t length,
		utf32_t replacement = StdChar_Replacement
	) {
		return calcRequiredLength(state, p, p + length, replacement);
	}

	static
	size_t
	calcRequiredLength(
		const SrcUnit* p,
		const SrcUnit* end,
		utf32_t replacement = StdChar_Replacement
	) {
		CountingEmitter emitter(replacement);
		Decoder::decode(emitter, p, end);
		return emitter.getLength();
	}

	static
	size_t
	calcRequiredLength(
		const SrcUnit* p,
		size_t length,
		utf32_t replacement = StdChar_Replacement
	) {
		return calcRequiredLength(p, p + length, replacement);
	}

	static
	ConvertResult
	convert_u(
		DecoderState* state,
		DstUnit* dst,
		const SrcUnit* src,
		const SrcUnit* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		EncodingEmitter_u emitter(dst, replacement);
		src = Decoder::decode(state, emitter, src, srcEnd);
		return ConvertResult(emitter.p(), src);
	}

	static
	ConvertLengthResult
	convert_u(
		DecoderState* state,
		DstUnit* dst,
		const SrcUnit* src,
		size_t srcLength,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult result = convert_u(state, dst, src, src + srcLength, replacement);
		return ConvertLengthResult(result.m_dst - dst, result.m_src - src);
	}

	static
	ConvertResult
	convert_u(
		DstUnit* dst,
		const SrcUnit* src,
		const SrcUnit* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		EncodingEmitter_u emitter(dst, replacement);
		src = Decoder::decode(emitter, src, srcEnd);
		return ConvertResult(emitter.p(), src);
	}

	static
	ConvertLengthResult
	convert_u(
		DstUnit* dst,
		const SrcUnit* src,
		size_t srcLength,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult result = convert_u(dst, src, src + srcLength, replacement);
		return ConvertLengthResult(result.m_dst - dst, result.m_src - src);
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
		EncodingEmitter_s emitter(dst, dstEnd, replacement);
		src = Decoder::decode(state, emitter, src, srcEnd);
		return ConvertResult(emitter.p(), src);
	}

	static
	ConvertLengthResult
	convert(
		DecoderState* state,
		DstUnit* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult result = convert(state, dst, dst + dstLength, src, src + srcLength, replacement);
		return ConvertLengthResult(result.m_dst - dst, result.m_src - src);
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
		EncodingEmitter_s emitter(dst, dstEnd, replacement);
		src = Decoder::decode(emitter, src, srcEnd);
		return ConvertResult(emitter.p(), src);
	}

	static
	ConvertLengthResult
	convert(
		DstUnit* dst,
		size_t dstLength,
		const SrcUnit* src,
		size_t srcLength,
		utf32_t replacement = StdChar_Replacement
	) {
		ConvertResult result = convert(dst, dst + dstLength, src, src + srcLength, replacement);
		return ConvertLengthResult(result.m_dst - dst, result.m_src - src);
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
