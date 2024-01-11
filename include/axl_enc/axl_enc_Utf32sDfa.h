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

#define _AXL_ENC_UTF32SDFA_H

#include "axl_enc_UtfDfa.h"

// UTF-32-S DFA is for stream-based decoding of UTF-32

namespace axl {
namespace enc {

//..............................................................................

struct Utf32sDfaRoot {};

template <bool IsBigEndian>
class Utf32sDfaBase: public UtfDfa<Utf32sDfaBase<IsBigEndian>, Utf32sDfaRoot> {
public:
	typedef UtfDfa<Utf32sDfaBase<IsBigEndian>, Utf32sDfaRoot> BaseType;

	enum State {
		State_Start     = 0,
		State_1         = 1,
		State_2         = 2,
		State_3         = 3,
		State_Ready     = 4,

		State_ErrorBit  = 0,  // never yield errors
		State_IndexMask = -1,
	};

public:
	Utf32sDfaBase() {}

	Utf32sDfaBase(DecoderState storage) {
		this->load(storage);
	}

	static
	size_t
	getPendingLength(DecoderState storage) {
		return BaseType::extractState(storage) & 3;
	}

	uint_t
	decode(uchar_t c);

	// skip codepoint calculations when simply counting codepoints

	uint_t
	count(uchar_t c) {
		return this->m_state = (this->m_state & 3) + 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <bool IsBigEndian>
uint_t
Utf32sDfaBase<IsBigEndian>::decode(uchar_t c) {
	uint_t prevState = this->m_state & 3;
	uint_t nextState = prevState + 1;

	if (IsBigEndian)
		this->m_cp = prevState ? this->m_cp | (c << ((3 - prevState) << 3)) : c << 24;
	else
		this->m_cp = prevState ? this->m_cp | (c << (prevState << 3)) : c;

	return this->m_state = nextState;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf32sDfaBase<false> Utf32sDfa;
typedef Utf32sDfaBase<true>  Utf32sDfa_be;

//..............................................................................

} // namespace enc
} // namespace axl
