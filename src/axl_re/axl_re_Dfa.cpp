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

namespace axl {
namespace re {

//..............................................................................

bool
DfaCharTransitionMap::add(
	utf32_t from,
	utf32_t to,
	const DfaState* state
) {
	ASSERT(from >= 0 && to <= 0x7fffffff && from <= to);

#if (_AXL_RE_DFA_MERGE_CHAR_RANGES)
	Iterator prevIt = m_map.find(from, sl::BinTreeFindRelOp_Lt);
	ASSERT(!prevIt || prevIt->m_value.m_last < from);

	if (prevIt &&
		prevIt->m_value.m_last + 1 == from &&
		prevIt->m_value.m_state == state
	) {
		prevIt->m_value.m_last = to;
		return false;
	}

	Iterator it = m_map.visit(from);
#else
#	if (_DEBUG)
	Iterator prevIt = m_map.find(from, sl::BinTreeFindRelOp_Lt);
	ASSERT(!prevIt || prevIt->m_value.m_last < from);
#	endif

	Iterator it = m_map.visit(from);
#endif

	it->m_value.m_last = to;
	it->m_value.m_state = state;
	return true;
}

//..............................................................................

DfaState::DfaState() {
	m_id = -1;
	m_acceptId = -1;
	m_flags = 0;
	m_anchorMask = 0;
}

void
DfaState::print(FILE* file) const {
	fprintf(file, "%02d: nfa({ ", m_id);

#undef INDENT
#define INDENT "      "

	size_t count = m_nfaStateSet.getCount();
	for (size_t i = 0; i < count; i++)
		fprintf(file, "%02d ", m_nfaStateSet[i]->m_id);

	fprintf(file, "})\n");

	if (m_flags & DfaStateFlag_Accept)
		fprintf(file, INDENT "accept %d\n", m_acceptId);

	count = m_anchorTransitionMap.getCount();
	for (size_t i = 0; i < count; i++) {
		const DfaState* state2 = m_anchorTransitionMap[i];
		if (state2)
			fprintf(file, INDENT "%s -> %02d\n", getAnchorString(i), state2->m_id);
	}

	char buffer[256];
	sl::String string(rc::BufKind_Stack, buffer, sizeof(buffer));

	DfaCharTransitionMap::ConstIterator it2 = m_charTransitionMap.getHead();
	for (; it2; it2++) {
		getCharRangeString(&string, it2->getKey(), it2->m_value.m_last);
		fprintf(file,
			INDENT "%s -> %02d",
			string.sz(),
			it2->m_value.m_state->m_id
		);

		fprintf(file, "\n");
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
		m_dfaState = dfaState;
		m_nfaState = nfaState;
		m_anchors = anchors;
	}
};

// TODO: REWORK! Now, NfaStateSet is ordered! Take special care of state priorities!

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
	const DfaState* dfaSearchStartState = m_regex->getDfaSearchStartState();

	// pass 1 -- visit all breaking points in the char range map

	for (size_t i = 0; i < nfaStateCount; i++) {
		const NfaState* nfaState = dfaState->m_nfaStateSet[i];
		if (nfaState->m_stateKind < NfaStateKind_FirstConsuming)
			continue;

		switch (nfaState->m_stateKind) {
		case NfaStateKind_MatchChar:
			visitMatchChar(nfaState->m_char);
			break;

		case NfaStateKind_MatchCharSet:
			visitMatchCharSet(*nfaState->m_charSet);
			break;

		case NfaStateKind_MatchAnyChar:
			visitMatchAnyChar();
			break;
		}
	}

	// pass 2 -- "colorize" the char range map from top priority to bottom

	for (size_t i = 0; i < nfaStateCount; i++) {
		const NfaState* nfaState = dfaState->m_nfaStateSet[i];
		if (nfaState->m_stateKind < NfaStateKind_FirstConsuming)
			continue;

		switch (nfaState->m_stateKind) {
		case NfaStateKind_MatchChar:
			addMatchChar(nfaState->m_char, nfaState->m_nextState);
			break;

		case NfaStateKind_MatchCharSet:
			addMatchCharSet(*nfaState->m_charSet, nfaState->m_nextState);
			break;

		case NfaStateKind_MatchAnyChar:
			addMatchAnyChar(nfaState->m_nextState);
			break;
		}
	}

	// finalize transition map

	CharRangeMapIterator it = m_charRangeMap.getHead();
	while (it != m_charRangeMap.getTail()) {
		CharRangeMapIterator nextIt = it.getNext();

		if (it->m_value.isEmpty()) {
			it = nextIt;
			continue;
		}

		DfaState* targetState = AXL_MEM_NEW(DfaState);
		sl::takeOver(&targetState->m_nfaStateSet, &it->m_value);
		targetState->m_nfaStateSet.buildEpsilonClosure();
		targetState = m_regex->addDfaState(targetState);

		dfaState->m_charTransitionMap.add(
			it->getKey(),
			nextIt->getKey() - 1,
			targetState
		);

		it = nextIt;
	}
}

void
DfaBuilder::buildFullDfa() {
	if (m_regex->m_dfaStateList.isEmpty() &&
		m_regex->m_preDfaStateList.isEmpty() &&
		m_regex->m_nfaSearchStartState
	) {
		// from the clean slate it's safe to first build the match DFA

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
		// otherwise, bootstrap (ensure start states are created) and then build the rest

		m_regex->getDfaMatchStartState();
		m_regex->getDfaSearchStartState();

		while (!m_regex->m_preDfaStateList.isEmpty())
			buildTransitionMaps(*m_regex->m_preDfaStateList.getHead());
	}
}

void
DfaBuilder::visitMatchCharSet(const CharSet& charSet) {
	CharSet::ConstIterator it = charSet.getHead();
	for (; it; it++)
		visitMatchCharRange(it->getKey(), it->m_value);
}

void
DfaBuilder::visitMatchCharRange(
	uint32_t from,
	uint32_t to
) {
	ASSERT(from >= 0 && from <= to);

	m_charRangeMap.visit(from);
	m_charRangeMap.visit(to + 1);
}

void
DfaBuilder::addMatchCharSet(
	const CharSet& charSet,
	const NfaState* state
) {
	CharSet::ConstIterator it = charSet.getHead();
	for (; it; it++)
		addMatchCharRange(it->getKey(), it->m_value, state);
}

void
DfaBuilder::addMatchCharRange(
	uint32_t from,
	uint32_t to,
	const NfaState* state
) {
	ASSERT(from >= 0 && from <= to);

	CharRangeMapIterator it = m_charRangeMap.visit(from);
	do {
		if (!it->m_value.isAccept())
			it->m_value.add(state);
	} while ((++it)->getKey() <= to);
}

//..............................................................................

} // namespace re
} // namespace axl
