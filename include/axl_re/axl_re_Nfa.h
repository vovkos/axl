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

//..............................................................................

enum Anchor {
	Anchor_Begin  = 0x01,
	Anchor_End    = 0x02,
	Anchor_Word   = 0x04,

	Anchor_Last   = Anchor_Word,
	Anchor__Count = 3,
	Anchor__TransitionMapSize = 1 << Anchor__Count,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char* getAnchorString(uint_t anchors);

//..............................................................................

enum NfaStateKind {
	NfaStateKind_Undefined = 0,
	NfaStateKind_Epsilon,
	NfaStateKind_Split,
	NfaStateKind_Accept,
	NfaStateKind_OpenCapture,
	NfaStateKind_CloseCapture,
	NfaStateKind_MatchAnchor,
	NfaStateKind_MatchChar,
	NfaStateKind_MatchCharSet,
	NfaStateKind_MatchAnyChar,

	NfaStateKind_FirstConsuming = NfaStateKind_MatchChar,
	NfaStateKind_FirstStorable  = NfaStateKind_Split,
	NfaStateKind_LastStorable   = NfaStateKind_MatchAnyChar,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct NfaState: sl::ListLink {
	NfaStateKind m_stateKind;
	size_t m_id;

	union {
		intptr_t m_unionData;
		size_t m_captureId;
		size_t m_acceptId;
		utf32_t m_char;
		Anchor m_anchor;
		CharSet* m_charSet;
		NfaState* m_splitState;
	};

	NfaState* m_nextState;

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
	createAccept(size_t acceptId);

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
};

//..............................................................................

class NfaStateSet {
protected:
	sl::Array<const NfaState*> m_array;
	sl::BitMap m_map;

public:
	NfaStateSet() {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	NfaStateSet(NfaStateSet&& src) {
		move(std::move(src));
	}
#endif

	NfaStateSet(const NfaStateSet& src) {
		copy(src);
	}

	const NfaState*
	operator [] (size_t i) const {
		return m_array[i];
	}

	bool
	isEmpty() const {
		return m_array.isEmpty();
	}

	size_t
	getCount() const {
		return m_array.getCount();
	}

#if (_AXL_CPP_HAS_RVALUE_REF)
	void
	move(NfaStateSet&& src) {
		m_array.move(std::move(src.m_array));
		m_map.move(std::move(src.m_map));
	}
#endif

	void
	copy(const NfaStateSet& src) {
		m_array.copy(src.m_array);
		m_map.copy(src.m_map);
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

	int
	cmp(const NfaStateSet& set) const {
		return m_map.cmp(set.m_map);
	}

	bool
	isEqual(const NfaStateSet& set) const {
		return m_map.isEqual(set.m_map);
	}

	size_t
	hash() const {
		return m_map.hash();
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

template <typename T>
class NfaStateSetMap: public sl::HashTable<
	NfaStateSet*,
	T,
	sl::HashDuckType<NfaStateSet>,
	sl::EqDuckType<NfaStateSet>
> {
};

//..............................................................................

} // namespace re
} // namespace axl
