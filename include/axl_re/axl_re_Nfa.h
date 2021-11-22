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
	NfaStateKind_Start,
	NfaStateKind_Accept,
	NfaStateKind_Concat,
	NfaStateKind_Epsilon,
	NfaStateKind_Split,
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

struct NfaState: sl::ListLink {
	NfaStateKind m_stateKind;
	size_t m_id;
	NfaState* m_nextState;      // all except NfaStateKind_Accept

	union {
		intptr_t m_unionData;
		size_t m_acceptId;      // NfaStateKind_Accept
		size_t m_captureId;     // NfaStateKind_Open/ClostCapture
		Anchor m_anchor;        // NfaStateKind_MatchAnchor
		utf32_t m_char;         // NfaStateKind_MatchChar
		CharSet* m_charSet;     // NfaStateKind_MatchCharSet
		NfaState* m_prevState;  // NfaStateKind_Concat
		NfaState* m_splitState; // NfaStateKind_Split
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
	createEpsilon(NfaState* nextState);

	void
	createSplit(
		NfaState* nextState,
		NfaState* splitState
	);

	void
	createAccept(size_t id);

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

//..............................................................................

// NFA sets are ORDERED by STATE PRIORITY

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

	void
	buildEpsilonClosure() {
		buildAnchorClosure(0);
	}

	void
	buildAnchorClosure(uint_t anchors);
};

//..............................................................................

template <typename T>
class NfaStateSetMap: public sl::HashTable<
	const NfaStateSet*,
	T,
	sl::HashDuckType<NfaStateSet>,
	sl::EqDuckType<NfaStateSet>
> {
};

//..............................................................................

} // namespace re
} // namespace axl
