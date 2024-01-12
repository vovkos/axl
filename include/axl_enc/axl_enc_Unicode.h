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
#include "axl_sl_PtrIterator.h"

namespace axl {
namespace enc {

//..............................................................................

enum StdChar {
	StdChar_Undefined   = 0xffff, //   (Undefined Character)
	StdChar_Replacement = 0xfffd, // � (Replacement Character)
};

//..............................................................................

bool
isPrintable(utf32_t c);

bool
isPrintableNonMark(utf32_t c);

bool
isMark(utf32_t c);

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
	// called before processing next code unit

	bool
	canEmit() const;

	// properly decoded code point

	void
	emitCp(
		const C* src,
		utf32_t cp
	);

	// invalid code unit sequence (emitting each code unit in this sequence)

	void
	emitCu(
		const C* src,
		utf32_t cu
	);

	// a valid code point after an invalid code unit sequence (emitted together with invalid cu-s)

	void
	emitCpAfterCu(
		const C* src,
		utf32_t cp
	);
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
	typename Op = sl::Nop<utf32_t>, // e.g. ToLowerCase, ToUpperCase, ToCaseFold
	typename Decoder0 = typename SrcEncoding0::Decoder // allows for reverse decoding
>
class Convert {
public:
	typedef DstEncoding0 DstEncoding;
	typedef SrcEncoding0 SrcEncoding;
	typedef Decoder0 Decoder;
	typedef typename DstEncoding::C DstUnit;
	typedef typename SrcEncoding::C SrcUnit;
	typedef typename DstEncoding::Encoder Encoder;
	typedef ConvertResult<DstUnit, SrcUnit> Result;

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
		emitCp(
			const SrcUnit* p,
			utf32_t cp
		) {
			m_length += Encoder::getEncodeLength(Op()(cp), m_replacement);
		}

		void
		emitCu(
			const SrcUnit* p,
			utf32_t cu
		) {
			emitCp(p, cu);
		}

		void
		emitCpAfterCu(
			const SrcUnit* p,
			utf32_t cp
		) {
			emitCp(p, cp);
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
		emitCp(
			const SrcUnit* p,
			utf32_t cp
		) {
			m_p = Encoder::encode(m_p, Op()(cp), m_replacement);
		}

		void
		emitCu(
			const SrcUnit* p,
			utf32_t cu
		) {
			emitCp(p, cu);
		}

		void
		emitCpAfterCu(
			const SrcUnit* p,
			utf32_t cp
		) {
			emitCp(p, cp);
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
			m_end = end - (Decoder::MaxEmitLength * Encoder::MaxEncodeLength - 1);
		}

		bool
		canEmit() const {
			return this->m_p < m_end;
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
		const SrcUnit* p,
		const SrcUnit* end,
		utf32_t replacement = StdChar_Replacement
	) {
		CountingEmitter emitter(replacement);
		Decoder::decode(emitter, p, end);
		return emitter.getLength();
	}

	static
	Result
	convert_u(
		DecoderState* state,
		DstUnit* dst,
		const SrcUnit* src,
		const SrcUnit* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		EncodingEmitter_u emitter(dst, replacement);
		src = Decoder::decode(state, emitter, src, srcEnd);
		return Result(emitter.p(), src);
	}

	static
	Result
	convert_u(
		DstUnit* dst,
		const SrcUnit* src,
		const SrcUnit* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		EncodingEmitter_u emitter(dst, replacement);
		src = Decoder::decode(emitter, src, srcEnd);
		return Result(emitter.p(), src);
	}

	static
	Result
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
		return Result(emitter.p(), src);
	}

	static
	Result
	convert(
		DstUnit* dst,
		DstUnit* dstEnd,
		const SrcUnit* src,
		const SrcUnit* srcEnd,
		utf32_t replacement = StdChar_Replacement
	) {
		EncodingEmitter_s emitter(dst, dstEnd, replacement);
		src = Decoder::decode(emitter, src, srcEnd);
		return Result(emitter.p(), src);
	}
};

//..............................................................................

// locates offsets of code-units that decode into a given character position

template <
	typename Encoding0,
	typename Decoder0 = typename Encoding0::Decoder // allows for reverse decoding
>
class Locate {
public:
	typedef Encoding0 Encoding;
	typedef Decoder0 Decoder;
	typedef typename Encoding::C C;
	typedef sl::PtrIterator<const C, Decoder::IsReverse> PtrIterator;

	// alas, we can't use simple compensation for overshoot of pending CUs post-decode;
	// counter-example: the reverse UTF-8 decoder might emit two CBs, then a two-byte CP.
	// therefore, we have to range-check before emission of CUs or post-CU CPs.

protected:
	class Emitter {
	protected:
		size_t m_pos;
		size_t m_targetPos;
		const C* m_p;

	public:
		Emitter(
			size_t pos,
			const C* src
		) {
			m_pos = 0;
			m_targetPos = pos;
			m_p = src;
		}

		size_t
		getPos() const {
			return m_pos;
		}

		const C*
		getSrc() const {
			return m_p;
		}

		bool
		canEmit() const {
			return m_pos < m_targetPos;
		}

		void
		emitCp(
			const C* p,
			utf32_t cp
		) {
			m_p = p;
			m_pos++;
		}

		void
		emitCu(
			const C* p,
			utf32_t cu
		) {
			emitChecked(p);
		}

		void
		emitCpAfterCu(
			const C* p,
			utf32_t cp
		) {
			emitChecked(p);
		}

	protected:
		void
		emitChecked(const C* p) {
			if (canEmit()) {
				m_p = p;
				m_pos++;
			}
		}
	};

public:
	static
	ConvertLengthResult // ConvertLengthResult.m_srcLength could be negative!
	locate(
		DecoderState* state,
		size_t pos,
		const C* src,
		const C* srcEnd
	) {
		if (!pos) // shortcut
			return ConvertLengthResult(0, -Decoder::getPendingLength(*state));

		// if pos is non-zero, no need to adjust the initial src ptr (emitCp/emitCu will do that)

		Emitter emitter(pos, src);
		const C* p = Decoder::decode(state, emitter, src, srcEnd);
		size_t actualPos = emitter.getPos();
		intptr_t srcLength = PtrIterator::sub(actualPos < pos ? p : emitter.getSrc(), src);
		return ConvertLengthResult(actualPos, srcLength);
	}

	static
	ConvertLengthResult
	locate(
		size_t pos,
		const C* src,
		const C* srcEnd
	) {
		DecoderState state = 0;
		return locate(&state, pos, src, srcEnd);
	}
};

//..............................................................................

// simply advances decoder state over input

template <typename Decoder>
class Advance {
public:
	typedef typename Decoder::C C;

protected:
	struct Emitter {
		bool
		canEmit() const {
			return true;
		}

		void
		emitCp(
			const C* p,
			utf32_t cp
		) {
		}

		void
		emitCu(
			const C* p,
			utf32_t cu
		) {
		}

		void
		emitCpAfterCu(
			const C* p,
			utf32_t cp
		) {
		}
	};

public:
	static
	void
	advance(
		DecoderState* state,
		const C* src,
		const C* srcEnd
	) {
		Emitter emitter;
		Decoder::decode(state, emitter, src, srcEnd);
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
