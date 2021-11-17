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

#define _AXL_RE_DFA_H

#include "axl_re_Nfa.h"
#include "axl_re_CharRangeMap.h"

namespace axl {
namespace re {

class Regex;
struct DfaState;

//..............................................................................

// when searching, DFA never completely dies out due to the (.*) prefix.
// hence, we explicitly mark those transitions that keep the "matching" portion
// of the DFA alive

enum DfaTransitionFlag {
	DfaTransitionFlag_Alive = 0x01,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct DfaTransition {
	const DfaState* m_state;
	uint_t m_flags;

	DfaTransition() {
		m_state = NULL;
		m_flags = 0;
	}

	operator bool () const {
		return m_state != NULL;
	}
};

//..............................................................................

class DfaCharTransitionMap {
public:
	struct Value: DfaTransition {
		utf32_t m_last;
	};

	typedef sl::RbTreeIterator<utf32_t, Value> Iterator;
	typedef sl::ConstRbTreeIterator<utf32_t, Value> ConstIterator;

protected:
	sl::RbTree<utf32_t, Value> m_map;

public:
	bool
	isEmpty() const {
		return m_map.isEmpty();
	}

	void
	clear() {
		m_map.clear();
	}

	size_t
	getCount() const {
		return m_map.getCount();
	}

	ConstIterator
	getHead() const {
		return m_map.getHead();
	}

	ConstIterator
	getTail() const {
		return m_map.getTail();
	}

	DfaTransition
	find(utf32_t c) const {
		ConstIterator it = m_map.find(c, sl::BinTreeFindRelOp_Le);
		return it && c <= it->m_value.m_last ? it->m_value : DfaTransition();
	}

	void
	add(
		utf32_t from,
		utf32_t to,
		const DfaState* state,
		uint_t flags
	);
};

//..............................................................................

enum DfaStateFlag {
	DfaStateFlag_Accept         = 0x01,
	DfaStateFlag_Final          = 0x02,
	DfaStateFlag_TransitionMaps = 0x10,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct DfaState: sl::ListLink {
	size_t m_id;
	uint_t m_flags;
	uint_t m_anchorMask;
	size_t m_acceptId;
	NfaStateSet m_nfaStateSet;
	sl::Array<const DfaState*> m_anchorTransitionMap;
	DfaCharTransitionMap m_charTransitionMap;

	DfaState();

	void
	finalize();
};

//..............................................................................

class DfaBuilder {
protected:
	// ensure unsigned comparisons in CharRangeMap (we need 0x80000000)

	struct CharRangeListEntry: sl::ListLink {
		uint32_t m_last;
		uint_t m_transitionFlags;
		const NfaState* m_nfaState;
	};

	struct CharRangeMapEntry {
		sl::List<CharRangeListEntry> m_rangeList;
		NfaStateSet m_nfaStateSet;
		uint_t m_transitionFlags;
	};

	typedef sl::RbTree<uint32_t, CharRangeMapEntry> CharRangeMap;
	typedef CharRangeMap::Iterator CharRangeMapIterator;

protected:
	Regex* m_regex;
	sl::RbTree<uint32_t, CharRangeMapEntry> m_charRangeMap;

public:
	DfaBuilder(Regex* regex) {
		m_regex = regex;
	}

	void
	buildTransitionMaps(DfaState* state);

	void
	buildFullDfa();

protected:
	void
	buildAnchorTransitionMap(DfaState* state);

	void
	buildCharTransitionMap(DfaState* state);

	void
	addMatchChar(
		uint32_t c,
		const NfaState* state,
		uint_t transitionFlags
	) {
		addMatchCharRange(c, c, state, transitionFlags);
	}

	void
	addMatchCharSet(
		const CharSet& charSet,
		const NfaState* state,
		uint_t transitionFlags
	);

	void
	addMatchAnyChar(
		const NfaState* state,
		uint_t transitionFlags
	) {
		addMatchCharRange(0, 0x7fffffff, state, transitionFlags);
	}

	void
	addMatchCharRange(
		uint32_t from,
		uint32_t to,
		const NfaState* state,
		uint_t transitionFlags
	);
};

//..............................................................................


} // namespace re
} // namespace axl
