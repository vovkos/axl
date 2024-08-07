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

#define _AXL_ENC_UTF16DFA_H

#include "axl_enc_UtfDfa.h"
#include "axl_sl_ByteOrder.h"

//
// AXL UTF-16 DFA specifics:
//
// - Never backtrack on errors;
// - Provide both forward and backward DFAs;
// - Yield isomorphic codepoint sequences on both forward and backward DFAs;
// - Serialize the state of DFAs to/from a single uint32_t;
// - Omit codepoint calculations when only need to count codepoints;
// - Suitable for both:
//   (a) Emitting valid codepoints only
//   (c) Emitting one replacement codepoint per unpaired surrogate
//

namespace axl {
namespace enc {

//..............................................................................

// char class map

class Utf16CcMap {
public:
	enum Cc {
		Cc_Single,      // single codepoint
		Cc_HiSurrogate, // d800..dbff - high surrogate
		Cc_LoSurrogate, // dc00..dfff - low surrogate
	};

public:
	// there are 3 really; round up to 4 so that multiplication by CcCount
	// does not break state bit properties

	enum {
		CcCount = 4,
	};

protected:
	static const uchar_t m_map[];
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf16CcMap::m_map[] = {
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 00..0f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 10..1f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 20..2f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 30..3f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 40..4f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 50..5f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 60..6f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 70..7f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 80..8f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 90..9f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // a0..af
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // b0..bf
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // c0..cf
	0, 0, 0, 0, 0, 0, 0, 0,  1, 1, 1, 1, 2, 2, 2, 2,  // d0..df
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // e0..ef
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // f0..ff
};

//..............................................................................

class Utf16DfaTable: public UtfDfa<Utf16DfaTable, Utf16CcMap> {
public:
	// pre-multiply state values for a tiny bit faster table lookups

	enum State {
		State_Start             = 0 * CcCount,  // 0  - start state
		State_Error             = 1 * CcCount,  // 4  - unpaired trail surrogate
		State_HiSurrogate       = 2 * CcCount,  // 8  - lead surrogate
		State_HiSurrogate_Error = 3 * CcCount,  // 12 - lead surrogate (with error)
		State_Ready             = 4 * CcCount,  // 16 - codepoint is ready
		State_Ready_Error       = 5 * CcCount,  // 20 - codepoint is ready (with error)
		State_ReadyPair         = 6 * CcCount,  // 24 - codepoint is ready (surrogate pair)

		State_ErrorBit          = 1 * CcCount,  // 4  - invalid sequence bit
		State_IndexMask         = -1,
	};

	enum {
		StateCount = 7,
		IsReverse  = false,
	};

protected:
	static const uchar_t m_dfa[StateCount * CcCount];
	static const uchar_t m_pendingLengthTable[StateCount];
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf16DfaTable::m_dfa[] = {
//  Cc_Single          Cc_HiSurrogate           Cc_LoSurrogate
	State_Ready,       State_HiSurrogate,       State_Error,      0,  // 0  - State_Start
	State_Ready,       State_HiSurrogate,       State_Error,      0,  // 4  - State_Error
	State_Ready_Error, State_HiSurrogate_Error, State_ReadyPair,  0,  // 8  - State_HiSurrogate
	State_Ready_Error, State_HiSurrogate_Error, State_ReadyPair,  0,  // 12 - State_HiSurrogate_Error
	State_Ready,       State_HiSurrogate,       State_Error,      0,  // 16 - State_Ready
	State_Ready,       State_HiSurrogate,       State_Error,      0,  // 20 - State_Ready_Error
	State_Ready,       State_HiSurrogate,       State_Error,      0,  // 24 - State_ReadyPair
};

AXL_SELECT_ANY const uchar_t Utf16DfaTable::m_pendingLengthTable[] = {
	0,  // 0  - State_Start
	0,  // 4  - State_Error
	1,  // 8  - State_HiSurrogate
	1,  // 12 - State_HiSurrogate_Error
	0,  // 16 - State_Ready
	0,  // 20 - State_Ready_Error
	0,  // 24 - State_ReadyPair
};

//..............................................................................

class Utf16ReverseDfaTable: public UtfDfa<Utf16ReverseDfaTable, Utf16CcMap> {
public:
	// pre-multiply state values for a tiny bit faster table lookups

	enum State {
		State_Start             = 0 * CcCount,  // 0  - start state
		State_Error             = 1 * CcCount,  // 4  - unpaired trail surrogate
		State_LoSurrogate       = 2 * CcCount,  // 8  - trail surrogate
		State_LoSurrogate_Error = 3 * CcCount,  // 12 - trail surrogate (with error)
		State_Ready             = 4 * CcCount,  // 16 - codepoint is ready
		State_Ready_Error       = 5 * CcCount,  // 20 - codepoint is ready (with error)
		State_ReadyPair         = 6 * CcCount,  // 24 - codepoint is ready (surrogate pair)

		State_ErrorBit          = 1 * CcCount,  // 4  - invalid sequence bit
		State_IndexMask         = -1,
	};

	enum {
		StateCount = 7,
		IsReverse = true,
	};

protected:
	static const uchar_t m_dfa[StateCount * CcCount];
	static const uchar_t m_pendingLengthTable[StateCount];
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf16ReverseDfaTable::m_dfa[] = {
//  Cc_Single          Cc_HiSurrogate   Cc_LoSurrogate
	State_Ready,       State_Error,     State_LoSurrogate,       0,  // 0  - State_Start
	State_Ready,       State_Error,     State_LoSurrogate,       0,  // 4  - State_Error
	State_Ready_Error, State_ReadyPair, State_LoSurrogate_Error, 0,  // 8  - State_LoSurrogate
	State_Ready_Error, State_ReadyPair, State_LoSurrogate_Error, 0,  // 12 - State_LoSurrogate_Error
	State_Ready,       State_Error,     State_LoSurrogate,       0,  // 16 - State_Ready
	State_Ready,       State_Error,     State_LoSurrogate,       0,  // 20 - State_Ready_Error
	State_Ready,       State_Error,     State_LoSurrogate,       0,  // 24 - State_ReadyPair
};

AXL_SELECT_ANY const uchar_t Utf16ReverseDfaTable::m_pendingLengthTable[] = {
	0,  // 0  - State_Start
	0,  // 4  - State_Error
	1,  // 8  - State_LoSurrogate
	1,  // 12 - State_LoSurrogate_Error
	0,  // 16 - State_Ready
	0,  // 20 - State_Ready_Error
	0,  // 24 - State_ReadyPair
};

//..............................................................................

// common algorithm for UTF-16 and reverse UTF-16;
// differs in DFA state transition tables only

/*
	Utf16Dfa dfa;

	// (a) only emit valid codepoints

	for (size_t i = 0; i < length; i++) {
		uint_t state = dfa.decode(data[i]);
		if (Utf16Dfa::isAccept(state))
			; // emit codepoint
	}

	// (b) emit one replacement codepoint per unpaired surrogate

	for (size_t i = 0; i < length; i++) {
		uint_t state = dfa.decode(data[i]);

		if (Utf16Dfa::isError(state))
			; // emit replacement codepoint

		if (Utf16Dfa::isAccept(state))
			; // emit codepoint
	}
*/

template <
	typename DfaTable,
	typename IsBigEndian
>
class Utf16DfaBase: public DfaTable
{
public:
	Utf16DfaBase() {}

	Utf16DfaBase(DecoderState storage) {
		this->load(storage);
	}

	static
	size_t
	getPendingLength(DecoderState storage) {
		return DfaTable::m_pendingLengthTable[DfaTable::extractState(storage) >> 2];
	}

	size_t
	getPendingLength() const {
		return DfaTable::m_pendingLengthTable[this->m_state >> 2];
	}

	Utf16DfaBase
	decode(uint16_t c);

	// skip codepoint calculations when simply counting codepoints

	uint_t
	count(uint16_t c) {
		uint_t cc = DfaTable::m_map[IsBigEndian()() ? c & 0xff : c >> 8];
		return this->m_state = DfaTable::m_dfa[this->m_state + cc];
	}

protected:
	Utf16DfaBase(
		uint_t state,
		utf32_t cp
	) {
		this->init(state, cp);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <
	typename DfaTable,
	typename IsBigEndian
>
Utf16DfaBase<DfaTable, IsBigEndian>
Utf16DfaBase<DfaTable, IsBigEndian>::decode(uint16_t c) {
	uint_t cc = DfaTable::m_map[IsBigEndian()() ? c & 0xff : c >> 8];
	uint_t nextState = DfaTable::m_dfa[this->m_state + cc];

	if (IsBigEndian()())
		c = sl::swapByteOrder16(c);

	utf32_t cp = nextState == DfaTable::State_ReadyPair ? DfaTable::IsReverse ?
		0x10000 - (0xd800 << 10) - 0xdc00 + (c << 10) + this->m_cp :
		0x10000 - (0xd800 << 10) - 0xdc00 + (this->m_cp << 10) + c :
		c;

	return Utf16DfaBase(nextState, cp);
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf16DfaBase<Utf16DfaTable, sl::False>        Utf16Dfa;
typedef Utf16DfaBase<Utf16DfaTable, sl::True>         Utf16Dfa_be;
typedef Utf16DfaBase<Utf16ReverseDfaTable, sl::False> Utf16ReverseDfa;
typedef Utf16DfaBase<Utf16ReverseDfaTable, sl::True>  Utf16ReverseDfa_be;

//..............................................................................

} // namespace enc
} // namespace axl
