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

class State {
	friend class Regex;

public:
	enum {
		EofChar = 256,
	};

protected:
	class Impl; // RE2::SM::State

protected:
	Impl* m_impl;

public:
	State() {
		init(0, EofChar, -1, EofChar);
	}

	State(
		uint64_t baseOffset,
		int baseChar
	) {
		init(baseOffset, baseChar, -1, EofChar);
	}

	State(
		uint64_t baseOffset,
		int baseChar,
		uint64_t eofOffset,
		int eofChar = EofChar
	) {
		init(baseOffset, baseChar, eofOffset, eofChar);
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
	reset();

	void
	reset(
		uint64_t baseOffset,
		int baseChar
	) {
		reset(baseOffset, baseChar, -1, EofChar);
	}

	void
	reset(
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
		uint64_t baseOffset,
		int baseChar,
		uint64_t eofOffset,
		int eofChar = EofChar
	);
};

//..............................................................................

} // namespace re2
} // namespace axl
