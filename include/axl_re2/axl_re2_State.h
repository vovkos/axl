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

#define _AXL_RE2_STATE_H

namespace axl {
namespace re2 {

class Regex;

//..............................................................................

enum ExecResult {
	ExecResult_ErrorInconsistent = -2, // RE2::SM::ErrorInconsistent - reverse scan couldn't find a match (inconsistent data)
	ExecResult_ErrorOutOfMemory  = -1, // RE2::SM::ErrorOutOfMemory  - DFA run out-of-memory
	ExecResult_Mismatch          = 0,  // RE2::SM::Mismatch          - match can't be found; next exec will reset & restart
	ExecResult_Continue,               // RE2::SM::Continue,         - match not found yet; continue feeding next chunks of data
	ExecResult_ContinueBackward,       // RE2::SM::ContinueBackward  - match end found; continue feeding previous chunks of data
	ExecResult_Match,                  // RE2::SM::Match,            - match end & start found
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum Anchor {
	Anchor_None      = 0, // RE2::UNANCHORED
	Anchor_Start     = 1, // RE2::ANCHOR_START
	Anchor_FullMatch = 2, // RE2::ANCHOR_BOTH
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum {
	EofChar = 256, // RE2::SM::kByteEndText
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class State {
	friend class Regex;

protected:
	class Impl; // RE2::SM::State

protected:
	Impl* m_impl;

public:
	State(Anchor anchor = Anchor_None) {
		init(anchor, 0, EofChar, -1, EofChar);
	}

	State(
		Anchor anchor,
		uint64_t baseOffset,
		int baseChar
	) {
		init(anchor, baseOffset, baseChar, -1, EofChar);
	}

	State(
		Anchor anchor,
		uint64_t baseOffset,
		int baseChar,
		uint64_t eofOffset,
		int eofChar = EofChar
	) {
		init(anchor, baseOffset, baseChar, eofOffset, eofChar);
	}

	State(const State& src);

#if (_AXL_CPP_HAS_RVALUE_REF)
	State(State&& src);
#endif

	~State();

#if (_AXL_CPP_HAS_RVALUE_REF)
	State&
	operator = (State&& src);
#endif

	State&
	operator = (const State& src);

	operator bool () const {
		return isMatch();
	}

	bool
	isMatch() const;

	Anchor
	getAnchor() const;

	uint64_t
	getBaseOffset() const;

	uint64_t
	getEofOffset() const;

	uint64_t
	getMatchOffset() const;

	uint64_t
	getMatchEndOffset() const;

	uint64_t
	getMatchSize() const;

	uint_t
	getMatchId() const;

	int
	getBaseChar() const;

	int
	getEofChar() const;

	int
	getMatchLastChar() const;

	int
	getMatchNextChar() const;

	void
	reset(Anchor anchor = Anchor_None) {
		reset(anchor, 0, EofChar, -1, EofChar);
	}

	void
	reset(
		Anchor anchor,
		uint64_t baseOffset,
		int baseChar
	) {
		reset(anchor, baseOffset, baseChar, -1, EofChar);
	}

	void
	reset(
		Anchor anchor,
		uint64_t baseOffset,
		int baseChar,
		uint64_t eofOffset,
		int eofChar = EofChar
	);

	void
	setEof(
		uint64_t offset,
		int eofChar = EofChar
	);

protected:
	void
	init(
		Anchor anchor,
		uint64_t baseOffset,
		int baseChar,
		uint64_t eofOffset,
		int eofChar = EofChar
	);
};

//..............................................................................

} // namespace re2
} // namespace axl
