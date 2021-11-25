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

#define _AXL_RE_NFA_H

#include "axl_re_CharSet.h"

namespace axl {
namespace re {

class Regex;

//..............................................................................

enum Anchor {
	Anchor_BeginLine       = 0x01,  // ^
	Anchor_EndLine         = 0x02,  // $
	Anchor_BeginText       = 0x04,  // \A
	Anchor_EndText         = 0x08,  // \z
	Anchor_WordBoundary    = 0x10,  // \b
	Anchor_NotWordBoundary = 0x20,  // \B
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getAnchorString(Anchor anchor);

sl::String
getAnchorsString(uint_t anchors);

//..............................................................................

enum NfaStateKind {
	NfaStateKind_Undefined = 0,
	NfaStateKind_Accept,
	NfaStateKind_Epsilon,
	NfaStateKind_Split,
	NfaStateKind_Link,
	NfaStateKind_Sequence,
 	NfaStateKind_OpenCapture,
	NfaStateKind_CloseCapture,
	NfaStateKind_MatchAnchor,
	NfaStateKind_MatchChar,
	NfaStateKind_MatchCharSet,
	NfaStateKind_MatchAnyChar,

	NfaStateKind_Last           = NfaStateKind_MatchAnyChar,
	NfaStateKind_LastEpsilon    = NfaStateKind_CloseCapture,
	NfaStateKind_FirstMatch     = NfaStateKind_MatchAnchor,
	NfaStateKind_FirstConsuming = NfaStateKind_MatchChar,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getNfaStateKindCodeString(NfaStateKind stateKind);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum NfaStateFlag {
	NfaStateFlag_Nullable = 0x01,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct NfaState: sl::ListLink {
	NfaStateKind m_stateKind;
	uint_t m_flags;
	size_t m_id;
	NfaState* m_nextState;      // all except NfaStateKind_Accept

	union {
		uintptr_t m_unionData[2];
		size_t m_acceptId;      // NfaStateKind_Accept
		size_t m_captureId;     // NfaStateKind_Open/CloseCapture
		Anchor m_anchor;        // NfaStateKind_MatchAnchor
		utf32_t m_char;         // NfaStateKind_MatchChar
		CharSet* m_charSet;     // NfaStateKind_MatchCharSet
		NfaState* m_splitState; // NfaStateKind_Split
		NfaState* m_tailState;  // NfaStateKind_Sequence
		struct {                // NfaStateKind_Link
			NfaState* m_opState;
			NfaState* m_reverseState;
		};
	};

	NfaState();
	~NfaState();

	bool
	isConsuming() const {
		return m_stateKind >= NfaStateKind_FirstConsuming;
	}

	bool
	isMatchChar(utf32_t c) const;

	bool
	isMatchAnchor(uint_t anchors) const {
		ASSERT(m_stateKind == NfaStateKind_MatchAnchor);
		return (anchors & m_anchor) != 0;
	}

	void
	createAccept(size_t id);

	void
	createEpsilon(NfaState* nextState);

	void
	createSplit(
		NfaState* nextState,
		NfaState* splitState
	);

	void
	createLink(
		NfaState* opState,
		NfaState* reverseState
	);

	void
	createSequence(
		NfaState* headState,
		NfaState* tailState
	);

	void
	createMatchAnchor(
		Anchor anchor,
		NfaState* nextState
	);

	void
	createMatchChar(
		utf32_t c,
		NfaState* nextState
	);

	void
	createMatchCharSet(NfaState* nextState);

	void
	createMatchAnyChar(NfaState* nextState);

	void
	createOpenCapture(
		size_t captureId,
		NfaState* nextState
	);

	void
	createCloseCapture(
		size_t captureId,
		NfaState* nextState
	);

	void
	finalizeLink(NfaState* nextState)  {
		ASSERT(m_stateKind == NfaStateKind_Link && !m_nextState);
		m_nextState = nextState;
	}

	void
	addChar(utf32_t c);

	void
	addCharRange(
		utf32_t from,
		utf32_t to
	);

	void
	resolveOutStates();

	NfaState*
	resolveEpsilon();

	void
	copy(NfaState& src);

#if (_AXL_DEBUG)
	void
	print(FILE* file = stdout) const;
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
NfaState::createAccept(size_t id) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Accept;
	m_acceptId = id;
}

inline
void
NfaState::createEpsilon(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Epsilon;
	m_nextState = nextState;
}

inline
void
NfaState::createSplit(
	NfaState* nextState,
	NfaState* splitState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Split;
	m_nextState = nextState;
	m_splitState = splitState;
}

inline
void
NfaState::createLink(
	NfaState* opState,
	NfaState* reverseState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Link;
	m_opState = opState;
	m_reverseState = reverseState;
}

inline
void
NfaState::createSequence(
	NfaState* headState,
	NfaState* tailState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Sequence;
	m_nextState = headState;
	m_tailState = tailState;
}

inline
void
NfaState::createMatchAnchor(
	Anchor anchor,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_MatchAnchor;
	m_anchor = anchor;
	m_nextState = nextState;
}

inline
void
NfaState::createMatchChar(
	utf32_t c,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_MatchChar;
	m_char = c;
	m_nextState = nextState;
}

inline
void
NfaState::createMatchCharSet(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_MatchCharSet;
	m_charSet = AXL_MEM_NEW(CharSet);
	m_nextState = nextState;
}

inline
void
NfaState::createMatchAnyChar(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_MatchAnyChar;
	m_nextState = nextState;
}

inline
void
NfaState::createOpenCapture(
	size_t captureId,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_OpenCapture;
	m_flags = nextState->m_flags;
	m_captureId = captureId;
	m_nextState = nextState;
}

inline
void
NfaState::createCloseCapture(
	size_t captureId,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_CloseCapture;
	m_captureId = captureId;
	m_nextState = nextState;
}

//..............................................................................

// NFA sets are strictly ORDERED by STATE PRIORITY

class NfaStateSet {
protected:
	sl::Array<const NfaState*> m_array;
	sl::BitMap m_map;

public:
	const NfaState*
	operator [] (size_t i) const {
		return m_array[i];
	}

	bool
	isEmpty() const {
		return m_array.isEmpty();
	}

	bool
	isAccept() const {
		return !m_array.isEmpty() && m_array.getBack()->m_stateKind == NfaStateKind_Accept;
	}

	size_t
	getCount() const {
		return m_array.getCount();
	}

	const NfaState*
	getLastState() const {
		return m_array.getBack();
	}

	const sl::BitMap&
	getMap() const {
		return m_map;
	}

	bool
	find(size_t id) const {
		return m_map.getBit(id);
	}

	bool
	find(const NfaState* state) const {
		return m_map.getBit(state->m_id);
	}

	bool
	isEqual(const NfaStateSet& set) const {
		size_t count = m_array.getCount();
		return count == set.m_array.getCount() && memcmp(m_array, set.m_array, count * sizeof(NfaState*)) == 0;
	}

	size_t
	hash() const {
		return sl::djb2(m_array, m_array.getCount() * sizeof(NfaState*));
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	void
	move(NfaStateSet&& src) {
		sl::takeOver(this, &src);
	}
#endif

	void
	copy(const NfaStateSet& src) {
		*this = src;
	}

	void
	clear() {
		m_array.clear();
		m_map.clear();
	}

	void
	reserve(size_t count) {
		m_array.reserve(count);
		m_map.setBitCount(count);
	}

	bool
	add(const NfaState* state);

	template <typename IsReverse>
	void
	buildEpsilonClosure() {
		buildClosureImpl<sl::False, IsReverse, sl::False>(0);
	}

	template <typename IsReverse>
	void
	buildAnchorClosure(uint_t anchors) {
		buildClosureImpl<sl::False, IsReverse, sl::True>(anchors);
	}

	void
	buildRollbackClosure() {
		buildClosureImpl<sl::True, sl::False, sl::False>(0);
	}

protected:
	template <
		typename IsRollback,
		typename IsReverse,
		typename UseAnchors
	>
	void
	buildClosureImpl(uint_t anchors);
};

//..............................................................................

template <typename T>
class NfaStateSetMap: public sl::HashTable<
	NfaStateSet,
	T,
	sl::HashDuckType<NfaStateSet>,
	sl::EqDuckType<NfaStateSet>,
	const NfaStateSet&
> {
};

//..............................................................................

struct NfaProgram {
	sl::List<NfaState> m_stateList;
	NfaState* m_matchStartState;
	NfaState* m_searchStartState;
	size_t m_captureCount;

	NfaProgram();

	void
	clear();

	void
	finalize(bool isMatchOnly = false);

	NfaState*
	getLastState() {
		return *m_stateList.getTail();
	}

	NfaState*
	addState();

	NfaState*
	insertState(NfaState* beforeState);

	NfaState*
	insertSplitState(
		NfaState* beforeState,
		NfaState* splitState
	);

#if (_AXL_DEBUG)
	void
	print(FILE* file) const;
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
NfaState*
NfaProgram::addState() {
	NfaState* state = AXL_MEM_NEW(NfaState);
	m_stateList.insertTail(state);
	return state;
}

inline
NfaState*
NfaProgram::insertState(NfaState* beforeState) {
	NfaState* state = AXL_MEM_NEW(NfaState);
	m_stateList.insertBefore(state, beforeState);
	return state;
}

inline
NfaState*
NfaProgram::insertSplitState(
	NfaState* beforeState,
	NfaState* splitState
) {
	NfaState* state = insertState(beforeState);
	state->createSplit(beforeState, splitState);
	return state;
}

//..............................................................................

} // namespace re
} // namespace axl
