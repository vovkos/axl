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

		State_ErrorBit    = 1 * CcCount,   // 8   - invalid sequence bit
	};

	enum {
		StateCount = 16,
	};

protected:
	static const uchar_t m_dfa[StateCount * CcCount];
	static const uchar_t m_pendingLengthTable[StateCount];

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
		typedef void EmitFn(
			Emitter& emitter,
			const utf8_t* p,
			uint32_t cp
		);

		static EmitFn* emitTable[StateCount] = {
			&emitNothing,               // 0   - State_Start
			&emitNothing,               // 8   - unused
			&emitPendingCus_State_1_2,  // 16  - State_1_2
			&emitPendingCus_State_1_2,  // 24  - State_1_2_Error
			&emitPendingCus_State_1_3,  // 32  - State_1_3
			&emitPendingCus_State_1_3,  // 40  - State_1_3_Error
			&emitPendingCus_State_2_3,  // 48  - State_2_3
			&emitNothing,               // 56  - unused
			&emitPendingCus_State_1_4,  // 64  - State_1_4
			&emitPendingCus_State_1_4,  // 72  - State_1_4_Error
			&emitPendingCus_State_2_4,  // 80  - State_2_4
			&emitNothing,               // 88  - unused
			&emitPendingCus_State_3_4,  // 96  - State_3_4
			&emitNothing,               // 104 - State_Error
			&emitNothing,               // 112 - State_Ready
			&emitNothing,               // 120 - State_Ready_Error
		};

		emitTable[m_state >> 3](emitter, p, m_cp);
	}

protected:
	Utf8Dfa(
		uint_t state,
		utf32_t cp
	) {
		init(state, cp);
	}

	template <typename Emitter>
	static
	void
	emitNothing(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_1_2(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p, 0xc0 | (acc & 0x3f));
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_1_3(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p, 0xe0 | (acc & 0x3f));
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_2_3(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p - 1, 0xe0 | ((acc >> 6) & 0x3f));
		emitter.emitCu(p, 0x80 | (acc & 0x3f));
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_1_4(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p, 0xf0 | (acc & 0x3f));
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_2_4(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p - 1, 0xf0 | ((acc >> 6) & 0x3f));
		emitter.emitCu(p, 0x80 | (acc & 0x3f));
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_3_4(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p - 2, 0xf0 | ((acc >> 12) & 0x3f));
		emitter.emitCu(p - 1, 0x80 | ((acc >> 6) & 0x3f));
		emitter.emitCu(p, 0x80 | (acc & 0x3f));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf8Dfa::m_dfa[] = {
//  00..0f             80..bf       c0..df            e0..ef           f0..f7           f8..ff
	State_Ready,       State_Error, State_1_2,        State_1_3,       State_1_4,       State_Error,  0, 0,  // 0   - State_Start
	0,                 0,           0,                0,               0,               0,            0, 0,  // 8   - unused
	State_Ready_Error, State_Ready, State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 16  - State_1_2
	State_Ready_Error, State_Ready, State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 24  - State_1_2_Error
	State_Ready_Error, State_2_3,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 32  - State_1_3
	State_Ready_Error, State_2_3,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 40  - State_1_3_Error
	State_Ready_Error, State_Ready, State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 48  - State_2_3
	0,                 0,           0,                0,               0,               0,            0, 0,  // 56  - unused
	State_Ready_Error, State_2_4,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 64  - State_1_4
	State_Ready_Error, State_2_4,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 72  - State_1_4_Error
	State_Ready_Error, State_3_4,   State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 80  - State_2_4
	0,                 0,           0,                0,               0,               0,            0, 0,  // 88  - unused
	State_Ready_Error, State_Ready, State_1_2_Error,  State_1_3_Error, State_1_4_Error, State_Error,  0, 0,  // 96  - State_3_4
	State_Ready,       State_Error, State_1_2,        State_1_3,       State_1_4,       State_Error,  0, 0,  // 104 - State_Error
	State_Ready,       State_Error, State_1_2,        State_1_3,       State_1_4,       State_Error,  0, 0,  // 112 - State_Ready
	State_Ready,       State_Error, State_1_2,        State_1_3,       State_1_4,       State_Error,  0, 0,  // 120 - State_Ready_Error
};

AXL_SELECT_ANY const uchar_t Utf8Dfa::m_pendingLengthTable[] = {
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

class Utf8ReverseDfa: public UtfDfa<Utf8ReverseDfa, Utf8CcMap> {
public:
	enum {
		IsReverse = true,
	};

	// don't pre-multiply state values because we need state bits
	// for branchless codepoint calculations; right-shifting would
	// kill all the benefits of pre-multiplication

	enum State {
		State_Start         = 0,   // start state
		State_Cb_1          = 2,   // 1st continuation byte
		State_Cb_2          = 4,   // 2nd continuation byte
		State_Cb_3          = 6,   // 3rd continuation byte
		State_Cb_3_Error    = 7,   // 3rd continuation byte (with error)
		State_Error         = 9,   // invalid sequence
		State_Ready         = 10,  // codepoint is ready
		State_Ready_Error   = 11,  // codepoint is ready (with error)
		State_Ready_Error_2 = 13,  // codepoint is ready (with double error)
		State_Ready_Error_3 = 15,  // codepoint is ready (with triple error)

		State_ErrorBit      = 1,   // invalid sequence bit
	};

	enum {
		StateCount = 16,
	};

protected:
	static const uchar_t m_dfa[StateCount * CcCount];
	static const uchar_t m_pendingLengthTable[StateCount];
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
		return m_pendingLengthTable[extractState(storage)];
	}

	size_t
	getPendingLength() const {
		return m_pendingLengthTable[m_state];
	}

	void
	reset() {
		m_state = State_Start;
		m_acc = 0;
	}

	void
	load(uint32_t storage) {
		m_state = storage >> 24;
		m_acc = storage & 0x00ffffff;
	}

	uint32_t
	save() const {
		return (m_state << 24) | (m_acc & 0x00ffffff);
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
		typedef void EmitFn(
			Emitter& emitter,
			const utf8_t* p,
			uint32_t cp
		);

		static EmitFn* emitTable[StateCount] = {
			&emitNothing,                // 0  - State_Start
			&emitNothing,                // 1  - unused
			&emitPendingCus_State_Cb_1,  // 2  - State_Cb_1
			&emitNothing,                // 3  - unused
			&emitPendingCus_State_Cb_2,  // 4  - State_Cb_2
			&emitNothing,                // 5  - unused
			&emitPendingCus_State_Cb_3,  // 6  - State_Cb_3
			&emitPendingCus_State_Cb_3,  // 7  - State_Cb_3_Error
			&emitNothing,                // 8  - unused
			&emitNothing,                // 9  - State_Error
			&emitNothing,                // 10 - State_Ready
			&emitNothing,                // 11 - State_Ready_Error
			&emitNothing,                // 12 - unused
			&emitNothing,                // 13 - State_Ready_Error_2
			&emitNothing,                // 14 - unused
			&emitNothing,                // 15 - State_Ready_Error_3
		};

		emitTable[m_state](emitter, p, m_acc);
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

	template <typename Emitter>
	static
	void
	emitNothing(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_Cb_1(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p, 0x80 | (acc & 0x3f));
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_Cb_2(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p, 0x80 | (acc & 0x3f));
		emitter.emitCu(p, 0x80 | ((acc >> 6) & 0x3f));
	}

	template <typename Emitter>
	static
	void
	emitPendingCus_State_Cb_3(
		Emitter& emitter,
		const utf8_t* p,
		uint32_t acc
	) {
		emitter.emitCu(p, 0x80 | (acc & 0x3f));
		emitter.emitCu(p, 0x80 | ((acc >> 6) & 0x3f));
		emitter.emitCu(p, 0x80 | ((acc >> 12) & 0x3f));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

AXL_SELECT_ANY const uchar_t Utf8ReverseDfa::m_dfa[] = {
//  00..0f               80..bf            c0..df               e0..ef             f0..f7       f8..ff
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error, State_Error,  0, 0,  // 0  - State_Start
	0,                   0,                0,                   0,                 0,           0,            0, 0,  // 1  - unused
	State_Ready_Error,   State_Cb_2,       State_Ready,         State_Error,       State_Error, State_Error,  0, 0,  // 2  - State_Cb_1
	0,                   0,                0,                   0,                 0,           0,            0, 0,  // 3  - unused
	State_Ready_Error_2, State_Cb_3,       State_Ready_Error,   State_Ready,       State_Error, State_Error,  0, 0,  // 4  - State_Cb_2
	0,                   0,                0,                   0,                 0,           0,            0, 0,  // 5  - unused
	State_Ready_Error_3, State_Cb_3_Error, State_Ready_Error_2, State_Ready_Error, State_Ready, State_Error,  0, 0,  // 6  - State_Cb_3
	State_Ready_Error_3, State_Cb_3_Error, State_Ready_Error_2, State_Ready_Error, State_Ready, State_Error,  0, 0,  // 7  - State_Cb_3_Error
	0,                   0,                0,                   0,                 0,           0,            0, 0,  // 8  - unused
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error, State_Error,  0, 0,  // 9  - State_Error
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error, State_Error,  0, 0,  // 10 - State_Ready
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error, State_Error,  0, 0,  // 11 - State_Ready_Error
	0,                   0,                0,                   0,                 0,           0,            0, 0,  // 12 - unused
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error, State_Error,  0, 0,  // 13 - State_Ready_Error_2
	0,                   0,                0,                   0,                 0,           0,            0, 0,  // 14 - unused
	State_Ready,         State_Cb_1,       State_Error,         State_Error,       State_Error, State_Error,  0, 0,  // 15 - State_Ready_Error_3
};

AXL_SELECT_ANY const uchar_t Utf8ReverseDfa::m_pendingLengthTable[] = {
	0,  // 0  - State_Start
	0,  // 1  - unused
	1,  // 2  - State_Cb_1
	0,  // 3  - unused
	2,  // 4  - State_Cb_2
	0,  // 5  - unused
	3,  // 6  - State_Cb_3
	3,  // 7  - State_Cb_3_Error
	0,  // 8  - unused
	0,  // 9  - State_Error
	0,  // 10 - State_Ready
	0,  // 11 - State_Ready_Error
	0,  // 12 - unused
	0,  // 13 - State_Ready_Error_2
	0,  // 14 - unused
	0,  // 15 - State_Ready_Error_3
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// use branchless arithmetics as it's (most likely) faster on modern CPUs
// TODO: still need some benchmarking against simpler and shorter branched impls

inline
Utf8ReverseDfa
Utf8ReverseDfa::decode(uchar_t c) {
	uint_t cc = m_map[c];
	uint_t prevState = m_state;
	uint_t nextState = m_dfa[(prevState << 3) + cc];
	uint_t acc = m_acc;
	uint_t cp = m_cp;

	if (cc == Cc_Cb) {
		ASSERT(nextState <= State_Cb_3_Error); // continuation bytes always get us here
		uint_t shift = 6 * ((nextState >> 1) - 1);
		acc >>= 6 & -(nextState & 1); // drop last continuation byte on error
		acc |= (c & 0x3f) << shift;   // shift according to the new position
	} else {
		ASSERT(nextState >= State_Error); // error or ready
		uint_t shift = 6 * ((prevState >> 1) & (((prevState & 8) >> 3) - 1)); // zero if state >= 8
		acc |= ((0xff >> cc) & c) << shift; // shift according to the previous position
		acc >>= 6 * ((nextState - 9) >> 1); // drop continuation bytes on errors
		cp = acc;
		acc = 0;
	}

	return Utf8ReverseDfa(nextState, cp, acc);
}

//..............................................................................

} // namespace enc
} // namespace axl
