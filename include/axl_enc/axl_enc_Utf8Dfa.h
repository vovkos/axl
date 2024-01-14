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

#define _AXL_ENC_UTF8DFA_H

#include "axl_enc_UtfDfa.h"

//
// AXL UTF-8 DFA features:
//
// - Never backtrack on errors;
// - Provide both forward and backward DFAs;
// - Accept all codepoints (surrogates, overlong sequences, invalid codepoints);
// - Serialize the state of DFAs to/from a single uint32_t;
// - Omit codepoint calculations when only need to count codepoints;
// - Suitable for all of the following use-cases:
//   (a) Emitting valid codepoints only
//   (b) Emitting at least one replacement codepoint per invalid sequence
//   (c) Emitting one replacement codepoint per each byte of an invalid sequence
//
// To yield isomorphic codepoint sequences on both forward and backward DFAs,
// use (a) or (c) schemas
//

namespace axl {
namespace enc {

//..............................................................................

// UTF-8 char class map

class Utf8CcMap {
public:
	enum Cc {
		Cc_Ascii,   // 00..7f - ASCII range
		Cc_Cb,      // 80..bf - continuation byte
		Cc_Hdr_2,   // c0..df - header of a 2-byte sequence
		Cc_Hdr_3,   // e0..ef - header of a 3-byte sequence
		Cc_Hdr_4,   // f0..f7 - header of a 4-byte sequence
		Cc_Invalid, // f8..ff - not a part of any UTF-8 sequence
	};

public:
	// there are 6 really; round up to 8 so that multiplication by CcCount:
	// (1) does not break bit properties in forward DFA;
	// (2) can be optimized to << 3 in backward DFA.

	enum {
		CcCount = 8,
	};

protected:
	static const uchar_t m_map[];
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf8CcMap::m_map[] = {
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 00..0f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 10..1f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 20..2f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 30..3f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 40..4f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 50..5f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 60..6f
	0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  // 70..7f
	1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,  // 80..8f
	1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,  // 90..9f
	1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,  // a0..af
	1, 1, 1, 1, 1, 1, 1, 1,  1, 1, 1, 1, 1, 1, 1, 1,  // b0..bf
	2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2,  // c0..cf
	2, 2, 2, 2, 2, 2, 2, 2,  2, 2, 2, 2, 2, 2, 2, 2,  // d0..df
	3, 3, 3, 3, 3, 3, 3, 3,  3, 3, 3, 3, 3, 3, 3, 3,  // e0..ef
	4, 4, 4, 4, 4, 4, 4, 4,  5, 5, 5, 5, 5, 5, 5, 5,  // f0..ff
};

//..............................................................................

/*
	Utf8Dfa dfa;

	// (a) only emit valid codepoints

	for (size_t i = 0; i < length; i++) {
		dfa = dfa.decode(data[i]);
		if (dfa.isReady())
			; // emit codepoint
	}

	// (b) emit at least one replacement codepoint per invalid sequence

	for (size_t i = 0; i < length; i++) {
		dfa = dfa.decode(data[i]);

		if (dfa.isError())
			; // emit replacement codepoint

		if (dfa.isReady())
			; // emit codepoint
	}

	// (c) emit one replacement codepoint per byte of an invalid sequence

	for (size_t i = 0, j = 0; i < length; i++) {
		Utf8Dfa next = dfa.decode(data[i]);
		if (next.isError())
			if (next.getState() == Utf8Dfa::State_Error)
				do {
					; // emit replacement codepoint
				} while (++j <= i);
			else
				do {
					; // emit replacement codepoint
				} while (++j < i);

		if (next.isReady())
			; // emit codepoint

		dfa = next;
	}
*/

class Utf8Dfa: public UtfDfa<Utf8Dfa, Utf8CcMap> {
public:
	enum {
		IsReverse = false,
	};

	// pre-multiply state values for a tiny bit faster table lookups

	enum State {
		State_Start       = 0 * CcCount,   // 0   - start state
		State_1_2         = 2 * CcCount,   // 16  - 1st byte in a 2-byte sequence
		State_1_2_Error   = 3 * CcCount,   // 24  - 1st byte in a 2-byte sequence (with error)
		State_1_3         = 4 * CcCount,   // 32  - 1st byte in a 3-byte sequence
		State_1_3_Error   = 5 * CcCount,   // 40  - 1st byte in a 3-byte sequence (with error)
		State_2_3         = 6 * CcCount,   // 48  - 2nd byte in a 3-byte sequence
		State_1_4         = 8 * CcCount,   // 64  - 1st byte in a 4-byte sequence
		State_1_4_Error   = 9 * CcCount,   // 72  - 1st byte in a 4-byte sequence (with error)
		State_2_4         = 10 * CcCount,  // 80  - 2nd byte in a 4-byte sequence
		State_3_4         = 12 * CcCount,  // 96  - 3rd byte in a 4-byte sequence
		State_Error       = 13 * CcCount,  // 104 - invalid sequence
		State_Ready       = 14 * CcCount,  // 112 - codepoint is ready
		State_Ready_Error = 15 * CcCount,  // 120 - codepoint is ready (with error)
		State__Count      = 16,

		State_ErrorBit    = 1 * CcCount,   // 8   - invalid sequence bit
		State_IndexMask   = -1,
	};

protected:
	static const uint8_t m_dfa[State__Count * CcCount];
	static const uint_t m_pendingLengthTable[State__Count];

public:
	Utf8Dfa() {}

	Utf8Dfa(DecoderState storage) {
		load(storage);
	}

	static
	size_t
	getPendingLength(DecoderState storage) {
		return m_pendingLengthTable[extractState(storage) >> 3];
	}

	size_t
	getPendingLength() const {
		return m_pendingLengthTable[m_state >> 3];
	}

	Utf8Dfa
	decode(uchar_t c);

	// skip codepoint calculations when simply counting codepoints (return state)

	uint_t
	count(uchar_t c) {
		return m_state = m_dfa[m_state + m_map[c]];
	}

	template <typename Emitter>
	void
	emitPendingCus(
		Emitter& emitter,
		const utf8_t* p
	) {
		switch (m_state) {
		case State_1_2:
		case State_1_2_Error:
			emitter.emitCu(p, 0xc0 | m_cp);
			break;

		case State_1_3:
		case State_1_3_Error:
			emitter.emitCu(p, 0xe0 | m_cp);
			break;

		case State_2_3:
			emitter.emitCu(p - 1, 0xe0 | (m_cp >> 6));
			emitter.emitCu(p, 0x80 | (m_cp & 0x3f));
			break;

		case State_1_4:
		case State_1_4_Error:
			emitter.emitCu(p, 0xf0 | m_cp);
			break;

		case State_2_4:
			emitter.emitCu(p - 1, 0xf0 | (m_cp >> 6));
			emitter.emitCu(p, 0x80 | (m_cp & 0x3f));
			break;

		case State_3_4:
			emitter.emitCu(p - 2, 0xf0 | (m_cp >> 12));
			emitter.emitCu(p - 1, 0x80 | ((m_cp >> 6) & 0x3f));
			emitter.emitCu(p, 0x80 | (m_cp & 0x3f));
			break;
		}
	}

protected:
	Utf8Dfa(
		uint_t state,
		utf32_t cp
	) {
		init(state, cp);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uint8_t Utf8Dfa::m_dfa[] = {
//  00..0f             80..bf       c0..df            e0..ef           f0..f7           f8..ff
	State_Ready,       State_Error, State_1_2,        State_1_3,       State_1_4,       State_Error,  0, 0,  // 0  - State_Start
	0,                 0,           0,                0,               0,               0,            0, 0,  // 1  - unused
	State_Ready_Error, State_Ready, State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 2  - State_1_2
	State_Ready_Error, State_Ready, State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 3  - State_1_2_Error
	State_Ready_Error, State_2_3,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 4  - State_1_3
	State_Ready_Error, State_2_3,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 5  - State_1_3_Error
	State_Ready_Error, State_Ready, State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 6  - State_2_3
	0,                 0,           0,                0,               0,               0,            0, 0,  // 7  - unused
	State_Ready_Error, State_2_4,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 8  - State_1_4
	State_Ready_Error, State_2_4,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 9  - State_1_4_Error
	State_Ready_Error, State_3_4,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 10 - State_2_4
	0,                 0,           0,                0,               0,               0,            0, 0,  // 11 - unused
	State_Ready_Error, State_Ready, State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 12 - State_3_4
	State_Ready,       State_Error, State_1_2,        State_1_3,       State_1_4,       State_Error,  0, 0,  // 12 - State_Error
	State_Ready,       State_Error, State_1_2,        State_1_3,       State_1_4,       State_Error,  0, 0,  // 14 - State_Ready
	State_Ready,       State_Error, State_1_2,        State_1_3,       State_1_4,       State_Error,  0, 0,  // 15 - State_Ready_Error
};

AXL_SELECT_ANY const uint_t Utf8Dfa::m_pendingLengthTable[] = {
	0,  // 0   - State_Start
	0,  // 8   - unused
	1,  // 16  - State_1_2
	1,  // 24  - State_1_2_Error
	1,  // 32  - State_1_3
	1,  // 40  - State_1_3_Error
	2,  // 48  - State_2_3
	0,  // 56  - unused
	1,  // 64  - State_1_4
	1,  // 72  - State_1_4_Error
	2,  // 80  - State_2_4
	0,  // 88  - unused
	3,  // 96  - State_3_4
	0,  // 104 - State_Error
	0,  // 112 - State_Ready
	0,  // 120 - State_Ready_Error
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Utf8Dfa
Utf8Dfa::decode(uchar_t c) {
	uint_t cc = m_map[c];
	uint_t state = m_dfa[m_state + cc];
	utf32_t cp = cc == Cc_Cb ?
		(m_cp << 6) | (c & 0x3f) : // continuation byte
		(0xff >> cc) & c;          // starter byte (ASCII or header)

	return Utf8Dfa(state, cp);
}

//..............................................................................

/*
	Utf8ReverseDfa rdfa;

	// (a) only emit valid codepoints

	for (intptr_t i = length - 1; i >= 0; i--) {
		uint_t state = rdfa.decode(data[i]);

		if (Utf8ReverseDfa::isReady(state))
			; // emit cp
	}

	// (b) emit at least one replacement codepoint per invalid sequence

	for (intptr_t i = length - 1; i >= 0; i--) {
		uint_t state = rdfa.decode(data[i]);

		if (Utf8ReverseDfa::isError(state))
			; // emit replacement codepoint

		if (Utf8ReverseDfa::isReady(state))
			; // emit cp
	}

	// (c) emit one replacement codepoint per byte of an invalid sequence

	for (intptr_t i = length - 1, j = i; i >= 0; i--) {
		uint_t state = rdfa.decode(data[i]);
		if (Utf8ReverseDfa::isError(state))
			if (state == Utf8ReverseDfa::State_Error) // standalone error
				do {
					; // emit replacement codepoint
				} while (--j >= i);
			else { // combined error
				size_t errorCount = Utf8ReverseDfa::getCombinedErrorCount(state);
				intptr_t k = j - errorCount;
				do {
					; // emit replacement codepoint
				} while (--j > k);
			}

		if (Utf8ReverseDfa::isReady(state))
			; // emit cp
	}

*/

// states stores pre-calculated values for all necessary shifts

#define AXL_ENC_MAKE_UTF8R_STATE(row, pending, errors) ( \
	(pending) | \
	((row) * CcCount) | \
	(((pending) * 6) << 8) | \
	(((pending) ? ((pending) - 1) * 6 : 0) << 16) | \
	(((errors) * 6) << 24) \
)

class Utf8ReverseDfa: public UtfDfa<Utf8ReverseDfa, Utf8CcMap> {
public:
	enum {
		IsReverse = true,
	};

	enum State {
		State_Start         = AXL_ENC_MAKE_UTF8R_STATE(0,  0,  0),  // 0x00000000 - start state
		State_Error         = AXL_ENC_MAKE_UTF8R_STATE(1,  0,  0),  // 0x00000008 - invalid sequence
		State_Cb_1          = AXL_ENC_MAKE_UTF8R_STATE(2,  1,  0),  // 0x00000611 - 1st continuation byte
		State_Error_1       = AXL_ENC_MAKE_UTF8R_STATE(3,  0,  1),  // 0x06000018 - invalid sequence (with error CU)
		State_Cb_2          = AXL_ENC_MAKE_UTF8R_STATE(4,  2,  0),  // 0x00060c22 - 2nd continuation byte
		State_Error_2       = AXL_ENC_MAKE_UTF8R_STATE(5,  0,  2),  // 0x0c000028 - invalid sequence (with double error CU)
		State_Cb_3          = AXL_ENC_MAKE_UTF8R_STATE(6,  3,  0),  // 0x000c1233 - 3rd continuation byte
		State_Cb_3_Error    = AXL_ENC_MAKE_UTF8R_STATE(7,  3,  1),  // 0x060c123b - 3rd continuation byte (with error CU)
		State_Error_3       = AXL_ENC_MAKE_UTF8R_STATE(9,  0,  3),  // 0x12000048 - invalid sequence (with triple error CU)
		State_Ready         = AXL_ENC_MAKE_UTF8R_STATE(10, 0,  0),  // 0x00000050 - codepoint is ready
		State_Ready_Error   = AXL_ENC_MAKE_UTF8R_STATE(11, 0,  1),  // 0x06000058 - codepoint is ready (with error CU)
		State_Ready_Error_2 = AXL_ENC_MAKE_UTF8R_STATE(13, 0,  2),  // 0x0c000068 - codepoint is ready (with double error CU)
		State_Ready_Error_3 = AXL_ENC_MAKE_UTF8R_STATE(15, 0,  3),  // 0x12000078 - codepoint is ready (with triple error CU)
		State__Count        = 16,

		State_ErrorBit         = 1 * CcCount,   // 8   - invalid sequence bit
		State_IndexMask        = 0x0f * CcCount,
		State_CbCountMask      = 0x03,
		State_CbLoadMask       = 0x0000ff00,
		State_CbStoreMask      = 0x00ff0000,
		State_ErrorCbCountMask = 0xff000000,
	};

	enum {
		StateCount = 16,
	};

protected:
	static const uint32_t m_dfa[StateCount * CcCount];
	static const uint32_t m_stateDb[StateCount];
	uint_t m_acc;

public:
	Utf8ReverseDfa() {
		m_acc = 0;
	}

	Utf8ReverseDfa(DecoderState storage) {
		load(storage);
	}

	static
	size_t
	getPendingLength(DecoderState storage) {
		return extractState(storage) & State_CbCountMask;
	}

	size_t
	getPendingLength() const {
		return m_state & State_CbCountMask;
	}

	void
	reset() {
		m_state = State_Start;
		m_acc = 0;
	}

	void
	load(uint32_t storage) {
		m_state = m_stateDb[((storage >> 24) & State_IndexMask) / CcCount];
		m_acc = storage & 0x00ffffff;
	}

	uint32_t
	save() const {
		return (m_state << 24) | m_acc; // the hi-byte of acc is always zero
	}

	Utf8ReverseDfa
	decode(uchar_t c);

	// skip codepoint calculations when simply counting codepoints (return state)

	uint_t
	count(uchar_t c) {
		return m_state = m_dfa[(m_state << 3) + m_map[c]];
	}

	template <typename Emitter>
	void
	emitPendingCus(
		Emitter& emitter,
		const utf8_t* p
	) {
		switch (m_state) {
		case State_Cb_1:
			emitter.emitCu(p, 0x80 | (m_acc & 0x3f));
			break;

		case State_Cb_2:
			emitter.emitCu(p + 1, 0x80 | (m_acc & 0x3f));
			emitter.emitCu(p, 0x80 | ((m_acc >> 6) & 0x3f));
			break;

		case State_Cb_3:
		case State_Cb_3_Error:
			emitter.emitCu(p + 2, 0x80 | (m_acc & 0x3f));
			emitter.emitCu(p + 1, 0x80 | ((m_acc >> 6) & 0x3f));
			emitter.emitCu(p, 0x80 | ((m_acc >> 12) & 0x3f));
			break;
		}
	}

	template <typename Emitter>
	void
	emitErrorPendingCus(
		uchar_t c,
		uint_t nextState,
		Emitter& emitter,
		const utf8_t* p
	) {
		if (isReady(nextState)) {
			// State_Ready_Error_x is only possible for Cc_Ascii (0), Cc_Hdr_2 (2), Cc_Hdr_3 (3)
			// cc >> 1 yields exactly what we need: 0 -> 0, 2 -> 1, 3 -> 2
			uint_t cc = m_map[c];
			p += cc >> 1;
		}

		switch (nextState) {
		case State_Error_1:
		case State_Ready_Error:
			emitter.emitCu(p, 0x80 | (m_acc & 0x3f));
			break;

		case State_Error_2:
		case State_Ready_Error_2:
			emitter.emitCu(p + 1, 0x80 | (m_acc & 0x3f));
			emitter.emitCu(p, 0x80 | ((m_acc >> 6) & 0x3f));
			break;

		case State_Error_3:
		case State_Ready_Error_3:
			emitter.emitCu(p + 2, 0x80 | (m_acc & 0x3f));
			emitter.emitCu(p + 1, 0x80 | ((m_acc >> 6) & 0x3f));
			emitter.emitCu(p, 0x80 | ((m_acc >> 12) & 0x3f));
			break;

		case State_Cb_3_Error:
			emitter.emitCu(p + 2, 0x80 | (m_acc & 0x3f));
			break;
		}
	}

protected:
	Utf8ReverseDfa(
		uint_t state,
		utf32_t cp,
		uint_t acc
	) {
		init(state, cp);
		m_acc = acc;
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uint32_t Utf8ReverseDfa::m_stateDb[StateCount] = {
	State_Start,         // 0
	State_Error,         // 1
	State_Cb_1,          // 2
	State_Error_1,       // 3
	State_Cb_2,          // 4
	State_Error_2,       // 5
	State_Cb_3,          // 6
	State_Cb_3_Error,    // 7
	0,                   // 8
	State_Error_3,       // 9
	State_Ready,         // 10
	State_Ready_Error,   // 11
	0,                   // 12
	State_Ready_Error_2, // 13
	0,                   // 14
	State_Ready_Error_3, // 15
};

AXL_SELECT_ANY const uint32_t Utf8ReverseDfa::m_dfa[] = {
//  00..0f               80..bf            c0..df               e0..ef             f0..f7         f8..ff
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 0  - State_Start
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 1  - State_Error
	State_Ready_Error,   State_Cb_2,       State_Ready,         State_Error_1,     State_Error_1, State_Error_1, 0, 0,  // 2  - State_Cb_1
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 3  - State_Error_1
	State_Ready_Error_2, State_Cb_3,       State_Ready_Error,   State_Ready,       State_Error_2, State_Error_2, 0, 0,  // 4  - State_Cb_2
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 5  - State_Error_2
	State_Ready_Error_3, State_Cb_3_Error, State_Ready_Error_2, State_Ready_Error, State_Ready,   State_Error_3, 0, 0,  // 6  - State_Cb_3
	State_Ready_Error_3, State_Cb_3_Error, State_Ready_Error_2, State_Ready_Error, State_Ready,   State_Error_3, 0, 0,  // 7  - State_Cb_3_Error
	0,                   0,                0,                   0,                 0,             0,             0, 0,  // 8  - unused
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 9  - State_Error_3
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 10 - State_Ready
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 11 - State_Ready_Error
	0,                   0,                0,                   0,                 0,             0,             0, 0,  // 12 - unused
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 13 - State_Ready_Error_2
	0,                   0,                0,                   0,                 0,             0,             0, 0,  // 14 - unused
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error,   State_Error,   0, 0,  // 15 - State_Ready_Error_3
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Utf8ReverseDfa
Utf8ReverseDfa::decode(uchar_t c) {
	uint_t cc = m_map[c];
	uint_t prevState = m_state;
	uint_t nextState = m_dfa[(prevState & State_IndexMask) + cc];
	uint_t acc = m_acc;
	uint_t cp = m_cp;

	if (cc == Cc_Cb) {
		acc >>= ((nextState & State_ErrorCbCountMask) >> 24); // drop the last continuation byte on Cb3_Error
		acc |= (c & 0x3f) << ((nextState & State_CbStoreMask) >> 16);
	} else {
		acc |= ((0xff >> cc) & c) << ((prevState & State_CbLoadMask) >> 8); // shift according to the previous position
		acc >>= ((nextState & State_ErrorCbCountMask) >> 24); // drop continuation bytes on errors
		cp = acc;
		acc = 0;
	}

	return Utf8ReverseDfa(nextState, cp, acc);
}

//..............................................................................

} // namespace enc
} // namespace axl
