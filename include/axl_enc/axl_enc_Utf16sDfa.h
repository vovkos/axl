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

#define _AXL_ENC_UTF16SDFA_H

#include "axl_enc_Utf16Dfa.h"

// UTF-16-S DFA is for stream-based decoding of UTF-16

namespace axl {
namespace enc {

//..............................................................................

class Utf16sDfaTable: public UtfDfa<Utf16sDfaTable, Utf16CcMap> {
public:
	// pre-multiply state values for a tiny bit faster table lookups

	enum State {
		State_Start             = 0 * CcCount,  // 0  - start state
		State_1_2               = 2 * CcCount,  // 8  - half of the 1st code-unit
		State_3_4               = 4 * CcCount,  // 16 - half of the 2nd code-unit
		State_Error             = 5 * CcCount,  // 20 - unpaired trail surrogate
		State_HiSurrogate       = 6 * CcCount,  // 24 - lead surrogate
		State_HiSurrogate_Error = 7 * CcCount,  // 28 - lead surrogate (with error)
		State_Ready             = 8 * CcCount,  // 32 - codepoint is ready
		State_Ready_Error       = 9 * CcCount,  // 36 - codepoint is ready (with error)
		State_ReadyPair         = 10 * CcCount, // 40 - codepoint is ready (surrogate pair)

		State_ErrorBit          = 1 * CcCount,  // 4  - invalid sequence bit
		State_LastHalf          = State_3_4,
	};

	enum {
		StateCount  = 11,
		IsReverse   = false,
		IsBigEndian = false,
	};

protected:
	static const uchar_t m_dfa[StateCount * CcCount];
	static const uchar_t m_pendingLengthTable[StateCount];
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf16sDfaTable::m_dfa[] = {
//  Cc_Single          Cc_HiSurrogate           Cc_LoSurrogate
	State_1_2,         State_1_2,               State_1_2,        0,  // 0  - State_Start
	0,                 0,                       0,                0,  // 4  - unused
	State_Ready,       State_HiSurrogate,       State_Error,      0,  // 8  - State_1_2
	0,                 0,                       0,                0,  // 12 - unused
	State_Ready_Error, State_HiSurrogate_Error, State_ReadyPair,  0,  // 16 - State_3_4
	State_1_2,         State_1_2,               State_1_2,        0,  // 20 - State_Error
	State_3_4,         State_3_4,               State_3_4,        0,  // 24 - State_HiSurrogate
	State_3_4,         State_3_4,               State_3_4,        0,  // 28 - State_HiSurrogate_Error
	State_1_2,         State_1_2,               State_1_2,        0,  // 32 - State_Ready
	State_1_2,         State_1_2,               State_1_2,        0,  // 36 - State_Ready_Error
	State_1_2,         State_1_2,               State_1_2,        0,  // 40 - State_ReadyPair
};

AXL_SELECT_ANY const uchar_t Utf16sDfaTable::m_pendingLengthTable[] = {
	0,  // 0  - State_Start
	0,  // 4  - unused
	1,  // 8  - State_1_2
	0,  // 12 - unused
	3,  // 16 - State_3_4
	0,  // 20 - State_Error
	2,  // 24 - State_HiSurrogate
	2,  // 28 - State_HiSurrogate_Error
	0,  // 32 - State_Ready
	0,  // 36 - State_Ready_Error
	0,  // 40 - State_ReadyPair
};

//..............................................................................

class Utf16sDfaTable_be: public UtfDfa<Utf16sDfaTable_be, Utf16CcMap> {
public:
	// pre-multiply state values for a tiny bit faster table lookups

	enum State {
		State_Start                    = 0 * CcCount,  // 0  - start state
		State_PendingError             = 2 * CcCount,  // 8  - half of an unpaired trail surrogate
		State_PendingHiSurrogate       = 4 * CcCount,  // 16 - half of a lead surrogate
		State_PendingHiSurrogate_Error = 6 * CcCount,  // 24 - half of a lead surrogate (with error)
		State_PendingReady             = 8 * CcCount,  // 32 - half of a single codepoint
		State_PendingReady_Error       = 10 * CcCount, // 40 - half of a single codepoint (with error)
		State_PendingReadyPair         = 12 * CcCount, // 48 - half of a lead surrogate
		State_Error                    = 13 * CcCount, // 52 - unpaired trail surrogate
		State_HiSurrogate              = 14 * CcCount, // 56 - trail surrogate
		State_HiSurrogate_Error        = 15 * CcCount, // 60 - trail surrogate (with error)
		State_Ready                    = 16 * CcCount, // 64 - codepoint is ready
		State_Ready_Error              = 17 * CcCount, // 68 - codepoint is ready (with error)
		State_ReadyPair                = 18 * CcCount, // 72 - codepoint is ready (surrogate pair)

		State_ErrorBit                 = 1 * CcCount,  // 4  - invalid sequence bit
		State_LastHalf                 = State_PendingReadyPair,
	};

	enum {
		StateCount  = 19,
		IsReverse   = false,
		IsBigEndian = true,
	};

protected:
	static const uchar_t m_dfa[StateCount * CcCount];
	static const uchar_t m_pendingLengthTable[StateCount];
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf16sDfaTable_be::m_dfa[] = {
//  Cc_Single                 Cc_HiSurrogate                  Cc_LoSurrogate
	State_PendingReady,       State_PendingHiSurrogate,       State_PendingError,      0,  // 0  - State_Start
	0,                        0,                              0,                       0,  // 4  - unused
	State_Error,              State_Error,                    State_Error,             0,  // 8  - State_PendingError
	0,                        0,                              0,                       0,  // 12 - unused
	State_HiSurrogate,        State_HiSurrogate,              State_HiSurrogate,       0,  // 16 - State_PendingHiSurrogate
	0,                        0,                              0,                       0,  // 20 - unused
	State_HiSurrogate_Error,  State_HiSurrogate_Error,        State_HiSurrogate_Error, 0,  // 24 - State_PendingHiSurrogate_Error
	0,                        0,                              0,                       0,  // 28 - unused
	State_Ready,              State_Ready,                    State_Ready,             0,  // 32 - State_PendingReady
	0,                        0,                              0,                       0,  // 36 - unused
	State_Ready_Error,        State_Ready_Error,              State_Ready_Error,       0,  // 40 - State_PendingReady_Error
	0,                        0,                              0,                       0,  // 44 - unused
	State_ReadyPair,          State_ReadyPair,                State_ReadyPair,         0,  // 48 - State_PendingReadyPair
	State_PendingReady,       State_PendingHiSurrogate,       State_PendingError,      0,  // 52 - State_Error
	State_PendingReady_Error, State_PendingHiSurrogate_Error, State_PendingReadyPair,  0,  // 56 - State_HiSurrogate
	State_PendingReady_Error, State_PendingHiSurrogate_Error, State_PendingReadyPair,  0,  // 60 - State_HiSurrogate_Error
	State_PendingReady,       State_PendingHiSurrogate,       State_PendingError,      0,  // 64 - State_Ready
	State_PendingReady,       State_PendingHiSurrogate,       State_PendingError,      0,  // 68 - State_Ready_Error
	State_PendingReady,       State_PendingHiSurrogate,       State_PendingError,      0,  // 72 - State_ReadyPair
};

AXL_SELECT_ANY const uchar_t Utf16sDfaTable_be::m_pendingLengthTable[] = {
	0,  // 0  - State_Start
	0,  // 4  - unused
	1,  // 8  - State_PendingError
	0,  // 12 - unused
	1,  // 16 - State_PendingHiSurrogate
	0,  // 20 - unused
	3,  // 24 - State_PendingHiSurrogate_Error
	0,  // 28 - unused
	1,  // 32 - State_PendingReady
	0,  // 36 - unused
	3,  // 40 - State_PendingReady_Error
	0,  // 44 - unused
	3,  // 48 - State_PendingReadyPair
	0,  // 52 - State_Error
	2,  // 56 - State_HiSurrogate
	2,  // 60 - State_HiSurrogate_Error
	0,  // 64 - State_Ready
	0,  // 68 - State_Ready_Error
	0,  // 72 - State_ReadyPair
};

//..............................................................................

class Utf16sReverseDfaTable: public UtfDfa<Utf16sReverseDfaTable, Utf16CcMap> {
public:
	// pre-multiply state values for a tiny bit faster table lookups

	enum State {
		State_Start                    = 0 * CcCount,  // 0  - start state
		State_PendingError             = 2 * CcCount,  // 8  - half of an unpaired lead surrogate
		State_PendingLoSurrogate       = 4 * CcCount,  // 16 - half of a trail surrogate
		State_PendingLoSurrogate_Error = 6 * CcCount,  // 24 - half of a trail surrogate (with error)
		State_PendingReady             = 8 * CcCount,  // 32 - half of a single codepoint
		State_PendingReady_Error       = 10 * CcCount, // 40 - half of a single codepoint (with error)
		State_PendingReadyPair         = 12 * CcCount, // 48 - half of a lead surrogate
		State_Error                    = 13 * CcCount, // 52 - unpaired lead surrogate
		State_LoSurrogate              = 14 * CcCount, // 56 - trail surrogate
		State_LoSurrogate_Error        = 15 * CcCount, // 60 - trail surrogate (with error)
		State_Ready                    = 16 * CcCount, // 64 - codepoint is ready
		State_Ready_Error              = 17 * CcCount, // 68 - codepoint is ready (with error)
		State_ReadyPair                = 18 * CcCount, // 72 - codepoint is ready (surrogate pair)

		State_ErrorBit                 = 1 * CcCount,  // 4  - invalid sequence bit
		State_LastHalf                 = State_PendingReadyPair,
	};

	enum {
		StateCount  = 19,
		IsReverse   = true,
		IsBigEndian = true,
	};

protected:
	static const uchar_t m_dfa[StateCount * CcCount];
	static const uchar_t m_pendingLengthTable[StateCount];
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf16sReverseDfaTable::m_dfa[] = {
//  Cc_Single                 Cc_HiSurrogate                Cc_LoSurrogate
	State_PendingReady,       State_PendingError,           State_PendingLoSurrogate,        0,  // 0  - State_Start
	0,                        0,                            0,                               0,  // 4  - unused
	State_Error,              State_Error,                  State_Error,                     0,  // 8  - State_PendingError
	0,                        0,                            0,                               0,  // 12 - unused
	State_LoSurrogate,        State_LoSurrogate,            State_LoSurrogate,               0,  // 16 - State_PendingLoSurrogate
	0,                        0,                            0,                               0,  // 20 - unused
	State_LoSurrogate_Error,  State_LoSurrogate_Error,      State_LoSurrogate_Error,         0,  // 24 - State_PendingLoSurrogate_Error
	0,                        0,                            0,                               0,  // 28 - unused
	State_Ready,              State_Ready,                  State_Ready,                     0,  // 32 - State_PendingReady
	0,                        0,                            0,                               0,  // 36 - unused
	State_Ready_Error,        State_Ready_Error,            State_Ready_Error,               0,  // 40 - State_PendingReady_Error
	0,                        0,                            0,                               0,  // 44 - unused
	State_ReadyPair,          State_ReadyPair,              State_ReadyPair,                 0,  // 48 - State_PendingReadyPair
	State_PendingReady,       State_PendingError,           State_PendingLoSurrogate,        0,  // 52 - State_Error
	State_PendingReady_Error, State_PendingReadyPair,       State_PendingLoSurrogate_Error,  0,  // 56 - State_LoSurrogate
	State_PendingReady_Error, State_PendingReadyPair,       State_PendingLoSurrogate_Error,  0,  // 60 - State_LoSurrogate_Error
	State_PendingReady,       State_PendingError,           State_PendingLoSurrogate,        0,  // 64 - State_Ready
	State_PendingReady,       State_PendingError,           State_PendingLoSurrogate,        0,  // 68 - State_Ready_Error
	State_PendingReady,       State_PendingError,           State_PendingLoSurrogate,        0,  // 72 - State_ReadyPair
};

AXL_SELECT_ANY const uchar_t Utf16sReverseDfaTable::m_pendingLengthTable[] = {
	0,  // 0  - State_Start
	0,  // 4  - unused
	1,  // 8  - State_PendingError
	0,  // 12 - unused
	1,  // 16 - State_PendingLoSurrogate
	0,  // 20 - unused
	3,  // 24 - State_PendingLoSurrogate_Error
	0,  // 28 - unused
	1,  // 32 - State_PendingReady
	0,  // 36 - unused
	3,  // 40 - State_PendingReady_Error
	0,  // 44 - unused
	3,  // 48 - State_PendingReadyPair
	0,  // 52 - State_Error
	2,  // 56 - State_LoSurrogate
	2,  // 60 - State_LoSurrogate_Error
	0,  // 64 - State_Ready
	0,  // 68 - State_Ready_Error
	0,  // 72 - State_ReadyPair
};

//..............................................................................

class Utf16sReverseDfaTable_be: public UtfDfa<Utf16sReverseDfaTable_be, Utf16CcMap> {
public:
	// pre-multiply state values for a tiny bit faster table lookups

	enum State {
		State_Start             = 0 * CcCount,  // 0  - start state
		State_1_2               = 2 * CcCount,  // 8  - half of the 1st code-unit
		State_3_4               = 4 * CcCount,  // 16 - half of the 2nd code-unit
		State_Error             = 5 * CcCount,  // 20 - unpaired lead surrogate
		State_LoSurrogate       = 6 * CcCount,  // 24 - trail surrogate
		State_LoSurrogate_Error = 7 * CcCount,  // 28 - trail surrogate (with error)
		State_Ready             = 8 * CcCount,  // 32 - codepoint is ready
		State_Ready_Error       = 9 * CcCount,  // 36 - codepoint is ready (with error)
		State_ReadyPair         = 10 * CcCount, // 40 - codepoint is ready (surrogate pair)

		State_ErrorBit          = 1 * CcCount,  // 4  - invalid sequence bit
		State_LastHalf          = State_3_4,
	};

	enum {
		StateCount  = 11,
		IsReverse   = true,
		IsBigEndian = false,
	};

protected:
	static const uchar_t m_dfa[StateCount * CcCount];
	static const uchar_t m_pendingLengthTable[StateCount];
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf16sReverseDfaTable_be::m_dfa[] = {
//  Cc_Single          Cc_HiSurrogate   Cc_LoSurrogate
	State_1_2,         State_1_2,       State_1_2,                0,  // 0  - State_Start
	0,                 0,               0,                        0,  // 4  - unused
	State_Ready,       State_Error,     State_LoSurrogate,        0,  // 8  - State_1_2
	0,                 0,               0,                        0,  // 12 - unused
	State_Ready_Error, State_ReadyPair, State_LoSurrogate_Error,  0,  // 16 - State_3_4
	State_1_2,         State_1_2,       State_1_2,                0,  // 20 - State_Error
	State_3_4,         State_3_4,       State_3_4,                0,  // 24 - State_LoSurrogate
	State_3_4,         State_3_4,       State_3_4,                0,  // 28 - State_LoSurrogate_Error
	State_1_2,         State_1_2,       State_1_2,                0,  // 32 - State_Ready
	State_1_2,         State_1_2,       State_1_2,                0,  // 36 - State_Ready_Error
	State_1_2,         State_1_2,       State_1_2,                0,  // 40 - State_ReadyPair
};

AXL_SELECT_ANY const uchar_t Utf16sReverseDfaTable_be::m_pendingLengthTable[] = {
	0,  // 0  - State_Start
	0,  // 4  - unused
	1,  // 8  - State_1_2
	0,  // 12 - unused
	3,  // 16 - State_3_4
	0,  // 20 - State_Error
	2,  // 24 - State_LoSurrogate
	2,  // 28 - State_LoSurrogate_Error
	0,  // 32 - State_Ready
	0,  // 36 - State_Ready_Error
	0,  // 40 - State_ReadyPair
};

//..............................................................................

// common algorithm for UTF-16-S and reverse UTF-16-S;
// differs in DFA state transition tables only

/*
	Utf16sDfa dfa;

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

template <typename DfaTable>
class Utf16sDfaBase: public DfaTable
{
public:
	Utf16sDfaBase() {}

	Utf16sDfaBase(uint32_t storage) {
		load(storage);
	}

	static
	uint_t
	getPendingLength(uint_t storage) {
		return m_pendingLengthTable[extractState(storage) >> 2];
	}

	uint_t
	decode(uchar_t c);

	// skip codepoint calculations when simply counting codepoints

	uint_t
	count(uchar_t c) {
		uint_t cc = m_map[c];
		return m_state = T::m_dfa[m_state + cc];
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename DfaTable>
uint_t
Utf16sDfaBase<DfaTable>::decode(uchar_t c) {
	uint_t cc = m_map[c];
	uint_t nextState = m_dfa[m_state + cc];

	if (nextState <= State_LastHalf) { // half of the next code-unit
		m_cp &= 0x0000ffff; // use this byte for storage: 0x00ff0000
		m_cp |= c << 16;
	} else {
		utf16_t cu = IsBigEndian ?
			((m_cp >> 8) & 0xff00) | c :
			(m_cp >> 16) | (c << 8);

		m_cp = nextState == State_ReadyPair ? IsReverse ?
			0x10000 - (0xd800 << 10) - 0xdc00 + (cu << 10) + (m_cp & 0xffff) :
			0x10000 - (0xd800 << 10) - 0xdc00 + ((m_cp & 0xffff) << 10) + cu :
			cu;
	}

	return m_state = nextState;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

typedef Utf16sDfaBase<Utf16sDfaTable>            Utf16sDfa;
typedef Utf16sDfaBase<Utf16sDfaTable_be>         Utf16sDfa_be;
typedef Utf16sDfaBase<Utf16sReverseDfaTable>     Utf16sReverseDfa;
typedef Utf16sDfaBase<Utf16sReverseDfaTable_be>  Utf16sReverseDfa_be;

//..............................................................................

} // namespace enc
} // namespace axl
