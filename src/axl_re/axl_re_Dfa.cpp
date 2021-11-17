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

#include "pch.h"
#include "axl_re_Dfa.h"
#include "axl_re_Regex.h"

#define _AXL_RE_DFA_MERGE_CHAR_RANGES 1

namespace axl {
namespace re {

//..............................................................................

void
DfaCharTransitionMap::add(
	utf32_t from,
	utf32_t to,
	const DfaState* state,
	uint_t flags
) {
	ASSERT(from >= 0 && to <= 0x7fffffff && from <= to);

	Iterator it = m_map.visit(from);
	Iterator prevIt = it.getPrev();
	ASSERT(!prevIt || prevIt->m_value.m_last < from);

#if (_AXL_RE_DFA_MERGE_CHAR_RANGES)
	if (prevIt &&
		prevIt->m_value.m_last + 1 == from &&
		prevIt->m_value.m_state == state &&
		prevIt->m_value.m_flags == flags
	) {
		prevIt->m_value.m_last = to;
		m_map.erase(it);
		it = prevIt;
	} else
#endif
	{
		it->m_value.m_last = to;
		it->m_value.m_state = state;
		it->m_value.m_flags = flags;
	}
}

//..............................................................................

DfaState::DfaState() {
	m_id = -1;
	m_flags = 0;
	m_anchorMask = 0;
	m_acceptId = -1;
}

void
DfaState::finalize() {
	ASSERT(!(m_flags & DfaStateFlag_Accept));

	size_t count = m_nfaStateSet.getCount();
	for (size_t i = 0; i < count; i++) {
		const NfaState* nfaState = m_nfaStateSet[i];
		ASSERT(nfaState->m_stateKind != NfaStateKind_Epsilon);

		if (nfaState->m_stateKind == NfaStateKind_Accept &&
			(!(m_flags & DfaStateFlag_Accept) || nfaState->m_acceptId < m_acceptId)
		) {
			m_flags |= DfaStateFlag_Accept;
			m_acceptId = nfaState->m_acceptId;
		}
	}
}

//..............................................................................

void
DfaBuilder::buildTransitionMaps(DfaState* state) {
	ASSERT(!(state->m_flags & DfaStateFlag_TransitionMaps) && state->m_id == -1);

	buildAnchorTransitionMap(state);
	buildCharTransitionMap(state);

	if ((state->m_flags & DfaStateFlag_Accept) && !state->m_anchorMask && state->m_charTransitionMap.isEmpty())
		state->m_flags |= DfaStateFlag_Final; // final states insta-match

	state->m_flags |= DfaStateFlag_TransitionMaps;
	state->m_id = m_regex->m_dfaStateList.getCount();

	m_regex->m_preDfaStateList.remove(state);
	m_regex->m_dfaStateList.insertTail(state);
}

struct DfaAtmWorkingSetEntry {
	DfaState* m_dfaState;
	const NfaState* m_nfaState;
	uint_t m_anchors;

	DfaAtmWorkingSetEntry() {}

	DfaAtmWorkingSetEntry(
		DfaState* dfaState,
		const NfaState* nfaState,
		uint_t anchors = 0
	) {
		setup(dfaState, nfaState, anchors);
	}

	void
	setup(
		DfaState* dfaState,
		const NfaState* nfaState,
		uint_t anchors = 0
	) {
		m_dfaState = dfaState;
		m_nfaState = nfaState;
		m_anchors = anchors;
	}
};

void
DfaBuilder::buildAnchorTransitionMap(DfaState* dfaState) {
	DfaState* anchorTransitionMap[Anchor__TransitionMapSize] = { 0 };
	uint_t anchorMask = 0;

	char buffer[256];
	sl::Array<DfaAtmWorkingSetEntry> workingSet(rc::BufKind_Stack, buffer, sizeof(buffer));

	size_t count = dfaState->m_nfaStateSet.getCount();
	for (size_t i = 0; i < count; i++) {
		const NfaState* nfaState = dfaState->m_nfaStateSet[i];
		if (nfaState->m_stateKind != NfaStateKind_MatchAnchor)
			continue;

		ASSERT(nfaState->m_anchor <= Anchor_Last);
		DfaState* anchorState = anchorTransitionMap[nfaState->m_anchor];
		if (!anchorState) {
			anchorState = AXL_MEM_NEW(DfaState);
			anchorState->m_nfaStateSet.copy(dfaState->m_nfaStateSet);
			anchorTransitionMap[nfaState->m_anchor] = anchorState;
		}

		workingSet.append(DfaAtmWorkingSetEntry(anchorState, nfaState, nfaState->m_anchor));
	}

	while (!workingSet.isEmpty()) {
		DfaAtmWorkingSetEntry entry = workingSet.getBackAndPop();

		bool isAdded;
		uint_t anchors;
		DfaState* anchorState;

		switch (entry.m_nfaState->m_stateKind) {
		case NfaStateKind_Split:
			isAdded = entry.m_dfaState->m_nfaStateSet.add(entry.m_nfaState->m_nextState);
			if (isAdded && entry.m_nfaState->m_nextState->m_stateKind <= NfaStateKind_MatchAnchor)
				workingSet.append(DfaAtmWorkingSetEntry(entry.m_dfaState, entry.m_nfaState->m_nextState, entry.m_anchors));

			isAdded = entry.m_dfaState->m_nfaStateSet.add(entry.m_nfaState->m_splitState);
			if (isAdded && entry.m_nfaState->m_splitState->m_stateKind <= NfaStateKind_MatchAnchor)
				workingSet.append(DfaAtmWorkingSetEntry(entry.m_dfaState, entry.m_nfaState->m_splitState, entry.m_anchors));

			break;

		case NfaStateKind_OpenCapture:
		case NfaStateKind_CloseCapture:
			isAdded = entry.m_dfaState->m_nfaStateSet.add(entry.m_nfaState->m_nextState);
			if (isAdded && entry.m_nfaState->m_nextState->m_stateKind <= NfaStateKind_MatchAnchor)
				workingSet.append(DfaAtmWorkingSetEntry(entry.m_dfaState, entry.m_nfaState->m_nextState, entry.m_anchors));

			break;

		case NfaStateKind_MatchAnchor:
			anchorMask |= entry.m_nfaState->m_anchor;
			anchors = entry.m_anchors | entry.m_nfaState->m_anchor;
			anchorState = anchorTransitionMap[anchors];
			if (!anchorState) {
				anchorState = AXL_MEM_NEW(DfaState);
				anchorState->m_nfaStateSet.copy(entry.m_dfaState->m_nfaStateSet);
				anchorTransitionMap[anchors] = anchorState;
			}

			isAdded = anchorState->m_nfaStateSet.add(entry.m_nfaState->m_nextState);
			if (isAdded && entry.m_nfaState->m_nextState->m_stateKind <= NfaStateKind_MatchAnchor)
				workingSet.append(DfaAtmWorkingSetEntry(anchorState, entry.m_nfaState->m_nextState, anchors));

			break;
		}
	}

	if (anchorMask) {
		dfaState->m_anchorTransitionMap.setCount(anchorMask + 1);
		for (size_t i = 0; i <= anchorMask; i++) {
			dfaState->m_anchorTransitionMap[i] = anchorTransitionMap[i] ?
				m_regex->addDfaState(anchorTransitionMap[i]) :
				NULL;
		}

		dfaState->m_anchorMask = anchorMask;
	}
}

void
DfaBuilder::buildCharTransitionMap(DfaState* dfaState) {
	ASSERT(dfaState->m_id == -1);

	m_charRangeMap.clear();

	size_t nfaStateCount = dfaState->m_nfaStateSet.getCount();

	sl::BitMap epsilonClosureMinusSearchStart; // exluding the DFA search start state
	const DfaState* dfaSearchStartState = m_regex->getDfaSearchStartState();
	if (dfaSearchStartState) {
		// build epsilon closure minus DFA search start;
		// can only check SPLITs because we've already did buildEpsilonClosure() for dfaState

		for (size_t i = 0; i < nfaStateCount; i++) {
			const NfaState* nfaState = dfaState->m_nfaStateSet[i];
			if (nfaState->m_stateKind == NfaStateKind_Split && !dfaSearchStartState->m_nfaStateSet.find(nfaState)) {
				epsilonClosureMinusSearchStart.setBitResize(nfaState->m_nextState->m_id);
				epsilonClosureMinusSearchStart.setBitResize(nfaState->m_splitState->m_id);
			}
		}
	}

	// add all transitions to the char range map

	for (size_t i = 0; i < nfaStateCount; i++) {
		const NfaState* nfaState = dfaState->m_nfaStateSet[i];
		if (nfaState->m_stateKind < NfaStateKind_FirstConsuming)
			continue;

		uint_t transitionFlags =
			dfaSearchStartState ?
			!dfaSearchStartState->m_nfaStateSet.find(nfaState->m_id) || // not in the DFA search start epsilon-closure OR...
			epsilonClosureMinusSearchStart.getBit(nfaState->m_id) ?     // ... can epsilon-jump here from some other state
			DfaTransitionFlag_Alive :
			0 :
			DfaTransitionFlag_Alive; // !dfaSearchStartState

		switch (nfaState->m_stateKind) {
		case NfaStateKind_MatchChar:
			addMatchChar(nfaState->m_char, nfaState->m_nextState, transitionFlags);
			break;

		case NfaStateKind_MatchCharSet:
			addMatchCharSet(*nfaState->m_charSet, nfaState->m_nextState, transitionFlags);
			break;

		case NfaStateKind_MatchAnyChar:
			addMatchAnyChar(nfaState->m_nextState, transitionFlags);
			break;
		}
	}

	sl::List<CharRangeListEntry> charRangeList;

	// split transitions into non-overlapping state sets

	CharRangeMapIterator it = m_charRangeMap.getHead();
	while (it != m_charRangeMap.getTail()) {
		CharRangeMapIterator nextIt = it.getNext();
		uint32_t c = it->getKey();

		charRangeList.insertListTail(&it->m_value.m_rangeList);
		sl::Iterator<CharRangeListEntry> it2 = charRangeList.getHead();
		while (it2) {
			sl::Iterator<CharRangeListEntry> nextIt = it2.getNext();

			if (it2->m_last < c)
				charRangeList.erase(it2);
			else {
				it->m_value.m_nfaStateSet.add(it2->m_nfaState);
				it->m_value.m_transitionFlags |= it2->m_transitionFlags;
			}

			it2 = nextIt;
		}

		it = nextIt;
	}

	// finalize transition map

	it = m_charRangeMap.getHead();
	while (it != m_charRangeMap.getTail()) {
		CharRangeMapIterator nextIt = it.getNext();

		if (it->m_value.m_nfaStateSet.isEmpty()) {
			it = nextIt;
			continue;
		}

		DfaState* targetState = AXL_MEM_NEW(DfaState);
		sl::takeOver(&targetState->m_nfaStateSet, &it->m_value.m_nfaStateSet);
		targetState->m_nfaStateSet.buildEpsilonClosure();
		targetState = m_regex->addDfaState(targetState);

		dfaState->m_charTransitionMap.add(it->getKey(), nextIt->getKey() - 1, targetState, it->m_value.m_transitionFlags);
		it = nextIt;
	}
}

void
DfaBuilder::buildFullDfa() {
	if (m_regex->m_dfaStateList.isEmpty() &&
		m_regex->m_preDfaStateList.isEmpty() &&
		m_regex->m_nfaSearchStartState
	) {
		// from the clean slate it's safe to first build the match DFA without checking for the search DFA

		NfaState* prevNfaSearchStartState = m_regex->m_nfaSearchStartState;
		m_regex->m_nfaSearchStartState = NULL;
		m_regex->getDfaMatchStartState();

		while (!m_regex->m_preDfaStateList.isEmpty())
			buildTransitionMaps(*m_regex->m_preDfaStateList.getHead());

		m_regex->m_nfaSearchStartState = prevNfaSearchStartState;
		m_regex->getDfaSearchStartState();

		while (!m_regex->m_preDfaStateList.isEmpty())
			buildTransitionMaps(*m_regex->m_preDfaStateList.getHead());
	} else {
		m_regex->getDfaMatchStartState();
		m_regex->getDfaSearchStartState();

		while (!m_regex->m_preDfaStateList.isEmpty())
			buildTransitionMaps(*m_regex->m_preDfaStateList.getHead());
	}
}

void
DfaBuilder::addMatchCharSet(
	const CharSet& charSet,
	const NfaState* state,
	uint_t transitionFlags
) {
	CharSet::ConstIterator it = charSet.getHead();
	for (; it; it++)
		addMatchCharRange(it->getKey(), it->m_value, state, transitionFlags);
}

void
DfaBuilder::addMatchCharRange(
	uint32_t from,
	uint32_t to,
	const NfaState* state,
	uint_t transitionFlags
) {
	ASSERT(from >= 0 && from <= to);

	CharRangeMapIterator it = m_charRangeMap.visit(from);
	CharRangeMapIterator endIt = m_charRangeMap.visit(to + 1);

	CharRangeListEntry* entry = AXL_MEM_NEW(CharRangeListEntry);
	entry->m_last = to;
	entry->m_nfaState = state;
	entry->m_transitionFlags = transitionFlags;
	it->m_value.m_rangeList.insertTail(entry);
}

//..............................................................................

} // namespace re
} // namespace axl
