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
#include "axl_sl_Operator.h"

// UTF-32-S DFA is for stream-based decoding of UTF-32

namespace axl {
namespace enc {

//..............................................................................

struct Utf32sDfaRoot {};

template <typename IsBigEndian>
class Utf32sDfaBase: public UtfDfa<Utf32sDfaBase<IsBigEndian>, Utf32sDfaRoot> {
public:
	enum State {
		State_Start    = 0,
		State_1        = 1,
		State_2        = 2,
		State_3        = 3,
		State_Ready    = 4,

		State_ErrorBit = 0,  // never yield errors
	};

public:
	Utf32sDfaBase() {}

	Utf32sDfaBase(uint32_t storage) {
		load(storage);
	}

	static
	uint_t
	getPendingLength(uint_t storage) {
		return extractState(storage) & 3;
	}

	uint_t
	decode(uchar_t c);

	// skip codepoint calculations when simply counting codepoints

	uint_t
	count(uchar_t c) {
		return m_state = (m_state & 3) + 1;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename IsBigEndian>
uint_t
Utf32sDfaBase<IsBigEndian>::decode(uchar_t c) {
	uint_t prevState = m_state & 3;
	uint_t nextState = prevState + 1;

	if (IsBigEndian()())
		m_cp = prevState ? c << (prevState << 3) : c;
	else
		m_cp = prevState ? c << ((3 - prevState) << 3) : c << 24;

	return m_state = nextState;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf32sDfaBase<sl::False> Utf32sDfa;
typedef Utf32sDfaBase<sl::True>  Utf32sDfa_be;

//..............................................................................

} // namespace enc
} // namespace axl
