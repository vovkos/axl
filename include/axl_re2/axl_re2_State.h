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
class State;

//..............................................................................

enum ExecResult {
	ExecResult_ErrorInconsistent = -2, // RE2::SM::ErrorInconsistent - reverse scan couldn't find a match (inconsistent data)
	ExecResult_ErrorOutOfMemory  = -1, // RE2::SM::ErrorOutOfMemory  - DFA run out-of-memory
	ExecResult_Mismatch          = 0,  // RE2::SM::Mismatch          - match can't be found
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

class Match {
	friend class State;
	friend class Regex;

protected:
	uint64_t m_offset;
	uint64_t m_endOffset;
	sl::StringRef m_text;

public:
	Match() {
		m_offset = -1;
		m_endOffset = -1;
	}

	Match(
		uint64_t offset,
		const sl::StringRef& text
	);

	bool
	isValid() const {
		ASSERT((m_offset == -1) == (m_endOffset == -1)); // both -1 or both not -1
		return m_offset != -1;
	}

	bool
	hasText() const {
		return m_text.cp() != NULL;
	}

	uint64_t
	getOffset() const {
		ASSERT(isValid());
		return m_offset;
	}

	uint64_t
	getEndOffset() const {
		ASSERT(isValid());
		return m_endOffset;
	}

	uint64_t
	getSize() const {
		ASSERT(isValid());
		return m_endOffset - m_offset;
	}

	const sl::StringRef&
	getText() const {
		ASSERT(hasText());
		return m_text;
	}

	void
	reset();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
Match::Match(
	uint64_t offset,
	const sl::StringRef& text
) {
	m_offset = offset;
	m_endOffset = offset + text.getLength();
	m_text = text;
}

inline
void
Match::reset() {
	m_offset = -1;
	m_endOffset = -1;
	m_text.clear();
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class State {
	friend class Regex;

protected:
	class Impl; // RE2::SM::State

protected:
	Impl* m_impl;
	mutable Match m_match;

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

	uint_t
	getMatchId() const;

	const Match&
	getMatch() const;

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
		int eofChar
	);

	void
	prepareMatch() const;
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
const Match&
State::getMatch() const {
	ASSERT(isMatch());

	if (m_match.m_offset == -1)
		prepareMatch();

	return m_match;
}

//..............................................................................

} // namespace re2
} // namespace axl
