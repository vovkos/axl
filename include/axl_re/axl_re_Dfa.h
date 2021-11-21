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

#define _AXL_RE_DFA_MERGE_CHAR_RANGES 1

namespace axl {
namespace re {

class Regex;
struct DfaState;

//..............................................................................

class DfaCharTransitionMap {
public:
	struct Value {
		utf32_t m_last;
		const DfaState* m_state;
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

	const DfaState*
	find(utf32_t c) const {
		ConstIterator it = m_map.find(c, sl::BinTreeFindRelOp_Le);
		return it && c <= it->m_value.m_last ? it->m_value.m_state : NULL;
	}

	bool
	add(
		utf32_t from,
		utf32_t to,
		const DfaState* state
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
	size_t m_acceptId;
	uint_t m_flags;
	uint_t m_anchorMask;
	NfaStateSet m_nfaStateSet;
	sl::Array<const DfaState*> m_anchorTransitionMap;
	DfaCharTransitionMap m_charTransitionMap;

	DfaState();

	bool
	isFinal() const {
		return m_anchorTransitionMap.isEmpty() && m_charTransitionMap.isEmpty();
	}

#if (_AXL_DEBUG)
	void
	print(FILE* file = stdout) const;
#endif
};

//..............................................................................

class DfaBuilder {
protected:
	// ensure unsigned comparisons in CharRangeMap (we need 0x80000000)

	typedef sl::RbTree<uint32_t, NfaStateSet> CharRangeMap;
	typedef CharRangeMap::Iterator CharRangeMapIterator;

protected:
	Regex* m_regex;
	CharRangeMap m_charRangeMap;

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
	visitMatchChar(uint32_t c) {
		visitMatchCharRange(c, c);
	}

	void
	visitMatchCharSet(const CharSet& charSet);

	void
	visitMatchAnyChar() {
		visitMatchCharRange(0, 0x7fffffff);
	}

	void
	visitMatchCharRange(
		uint32_t from,
		uint32_t to
	);

	void
	addMatchChar(
		uint32_t c,
		const NfaState* state
	) {
		addMatchCharRange(c, c, state);
	}

	void
	addMatchCharSet(
		const CharSet& charSet,
		const NfaState* state
	);

	void
	addMatchAnyChar(const NfaState* state) {
		addMatchCharRange(0, 0x7fffffff, state);
	}

	void
	addMatchCharRange(
		uint32_t from,
		uint32_t to,
		const NfaState* state
	);
};

//..............................................................................


} // namespace re
} // namespace axl
