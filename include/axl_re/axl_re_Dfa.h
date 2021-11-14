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

	void
	add(
		utf32_t from,
		utf32_t to,
		const DfaState* state
	);
};

//..............................................................................

class DfaCaptureIdSet {
protected:
	sl::BitMap m_map;
	sl::Array<size_t> m_array;

public:
	size_t
	operator [] (size_t i) const  {
		return m_array[i];
	}

	bool
	isEmpty() const {
		return m_array.isEmpty();
	}

	size_t
	getCount() const  {
		return m_array.getCount();
	}

	bool
	add(size_t captureId);
};

//..............................................................................

enum DfaStateFlag {
	DfaStateFlag_Accept            = 0x01,
	DfaStateFlag_Final             = 0x02,
	DfaStateFlag_EpsilonClosure    = 0x10,
	DfaStateFlag_CharTransitionMap = 0x20,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct DfaState: sl::ListLink {
	size_t m_id;
	uint_t m_flags;
	uint_t m_anchorMask;
	size_t m_acceptId;
	size_t m_acceptNfaStateId;
	DfaCaptureIdSet m_openCaptureIdSet;
	DfaCaptureIdSet m_closeCaptureIdSet;
	NfaStateSet m_nfaStateSet;
	sl::Array<const DfaState*> m_anchorTransitionMap;
	DfaCharTransitionMap m_charTransitionMap;

	DfaState();

	void
	copy(const DfaState& src);

	void
	finalize();
};

//..............................................................................

class DfaBuilder {
protected:
	// ensure unsigned comparisons in CharRangeMap (we need 0x80000000)

	struct CharRangeListEntry: sl::ListLink {
		uint32_t m_last;
		const NfaState* m_nfaState;
	};

	struct CharRangeMapEntry {
		sl::List<CharRangeListEntry> m_rangeList;
		NfaStateSet m_nfaStateSet;
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
	makeEpsilonClosure(DfaState* state);

	void
	buildCharTransitionMap(DfaState* state);

	void
	buildFullDfa();

protected:
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
