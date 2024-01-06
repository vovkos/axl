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
		uint32_t cp,
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
		uint32_t cp,
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
			p[0] = (uint8_t)(cp >> 18)             | 0xf0;  // 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
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

// emits a replacement per each byte of an invalid UTF-8 sequence

template <typename Dfa0>
class Utf8DecoderBase {
public:
	typedef utf8_t C;
	typedef Dfa0 Dfa;

	enum {
		MaxEmitLength = 4, // 4 replacements (or 3 replacement + codepoint)
		IsReverse     = Dfa::IsReverse,
	};

public:
	static
	size_t
	getPendingLength(DecoderState state) {
		return Dfa::getPendingLength(state);
	}

	template <typename Emitter>
	static
	void
	emitPendingCus(
		DecoderState state,
		Emitter& emitter,
		const C* p
	) {
		Dfa dfa(state);
		return dfa.emitPendingCus(emitter, p);
	}

	template <typename Emitter>
	static
	const C*
	decode(
		DecoderState* state,
		Emitter& emitter,
		const C* p,
		const C* end
	) {
		Dfa dfa(*state);
		p = decode(dfa, emitter, p, end);
		*state = dfa.save();
		return p;
	}

	template <typename Emitter>
	static
	const C*
	decode(
		Emitter& emitter,
		const C* p,
		const C* end
	) {
		Dfa dfa;
		return decode(dfa, emitter, p, end);
	}

protected:
	template <typename Emitter>
	static
	const C*
	decode(
		Dfa& dfa,
		Emitter& emitter,
		const C* p0,
		const C* end
	) {
		sl::PtrIterator<const C, Dfa::IsReverse> p = p0;
		for (; p < end && emitter.canEmit(); p++) {
			uchar_t c = *p;
			Dfa next = dfa.decode(c);
			if (next.getState() == Dfa::State_Ready)
				emitter.emitCp(p + 1, next.getCodePoint());
			else if (next.isError()) {
				dfa.emitPendingCus(emitter, p);
				if (next.isReady())
					emitter.emitCpAfterCu(p + 1, next.getCodePoint());
				else if (next.getState() == Dfa::State_Error)
					emitter.emitCu(p + 1, c);
			}

			dfa = next;
		}

		return p;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf8DecoderBase<Utf8Dfa>        Utf8Decoder;
typedef Utf8DecoderBase<Utf8ReverseDfa> Utf8ReverseDecoder;

//..............................................................................

class Utf8 {
public:
	typedef utf8_t C;
	typedef Utf8Encoder Encoder;
	typedef Utf8Decoder Decoder;
	typedef Utf8ReverseDecoder ReverseDecoder;

public:
	static
	const uint8_t*
	getBom() {
		static uint8_t bom[] = { 0xef, 0xbb, 0xbf };
		return bom;
	}

	static
	size_t
	getBomLength() {
		return 3;
	}
};

//..............................................................................

} // namespace enc
} // namespace axl
