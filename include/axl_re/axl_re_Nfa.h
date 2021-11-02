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

#include "axl_re_FsmMatchCondition.h"

namespace axl {
namespace re {

//..............................................................................

enum NfaStateKind {
	NfaStateKind_Undefined = 0,
	NfaStateKind_Accept,
	NfaStateKind_Epsilon,
	NfaStateKind_Split,
	NfaStateKind_OpenCapture,
	NfaStateKind_CloseCapture,
	NfaStateKind_Match,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

struct NfaState: sl::ListLink {
	NfaStateKind m_stateKind;
	uint_t m_id;

	FsmMatchCondition m_matchCondition;

	union {
		size_t m_captureId;
		void* m_acceptContext;
	};

	NfaState* m_nextState;
	NfaState* m_splitState;

	NfaState();

	void
	createAccept(void* context);

	void
	createEpsilon(NfaState* nextState);

	void
	createSplit(
		NfaState* nextState,
		NfaState* splitState
	);

	void
	createMatchAnchor(
		FsmAnchor anchor,
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
	resolveOutStates();

	NfaState*
	resolveEpsilon();

	void
	copy(NfaState& src);
};

//..............................................................................

struct NfaStateSet {
	sl::Array<NfaState*> m_stateArray;
	sl::BitMap m_stateSet;

	bool
	addState(NfaState* state);

	int
	cmp(const NfaStateSet& set) const {
		return m_stateSet.cmp(set.m_stateSet);
	}

	bool
	isEqual(const NfaStateSet& set) const {
		return m_stateSet.isEqual(set.m_stateSet);
	}

	size_t
	hash() const {
		return m_stateSet.hash();
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
