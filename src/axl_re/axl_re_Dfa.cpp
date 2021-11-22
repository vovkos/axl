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

	#define INDENT "      "

	size_t count = m_nfaStateSet.getCount();
	for (size_t i = 0; i < count; i++)
		fprintf(file, "%02d ", m_nfaStateSet[i]->m_id);

	fprintf(file, "})\n");

	if (m_flags & DfaStateFlag_Accept)
		fprintf(file, INDENT "accept(%d)\n", m_acceptId);

	sl::ConstHashTableIterator<uint_t, const DfaState*> it = m_anchorTransitionMap.getHead();
	for (; it; it++)
		fprintf(file, INDENT "%s -> %02d\n", getAnchorsString(it->getKey()).sz(), it->m_value->m_id);

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

	buildCharTransitionMap(state);

	if ((state->m_flags & (DfaStateFlag_AnchorTransition | DfaStateFlag_HasMatchAnchor)) == DfaStateFlag_HasMatchAnchor)
		buildAnchorTransitionMap(state);

	if ((state->m_flags & DfaStateFlag_Accept) &&
		state->m_anchorTransitionMap.isEmpty() &&
		state->m_charTransitionMap.isEmpty())
		state->m_flags |= DfaStateFlag_InstaMatch;

	state->m_flags |= DfaStateFlag_TransitionMaps;
	state->m_id = m_regex->m_dfaStateList.getCount();

	m_regex->m_preDfaStateList.remove(state);
	m_regex->m_dfaStateList.insertTail(state);
}

struct DfaAnchorCalcEntry {
	const NfaState* m_nfaState;
	uint_t m_anchors;

	DfaAnchorCalcEntry() {}

	DfaAnchorCalcEntry(
		const NfaState* nfaState,
		uint_t anchors
	) {
		setup(nfaState, anchors);
	}

	void
	setup(
		const NfaState* nfaState,
		uint_t anchors
	) {
		m_nfaState = nfaState;
		m_anchors = anchors;
	}
};

void
DfaBuilder::buildAnchorTransitionMap(DfaState* dfaState) {
	ASSERT(dfaState->m_flags & DfaStateFlag_HasMatchAnchor);

	DfaAnchorTransitionPreMap::Iterator it;
	DfaAnchorTransitionPreMap anchorTransitionPreMap;

	char buffer[256];
	sl::Array<DfaAnchorCalcEntry> stack(rc::BufKind_Stack, buffer, sizeof(buffer));

	// stage 1 -- add anchor transition roots (order is not important)

	size_t count = dfaState->m_nfaStateSet.getCount();
	for (size_t i = 0; i < count; i++) {
		const NfaState* nfaState = dfaState->m_nfaStateSet[i];
		if (nfaState->m_stateKind == NfaStateKind_MatchAnchor) {
			it = anchorTransitionPreMap.visit(nfaState->m_anchor);
			if (it->m_value.isEmpty())
				it->m_value = dfaState->m_nfaStateSet; // build anchor closure later

			stack.append(DfaAnchorCalcEntry(nfaState->m_nextState, nfaState->m_anchor));
		}
	}

	// stage 2 -- calculate which anchor combinations are possible; add anchor transition map entries (but don't fill NFA state sets yet)

	sl::BitMap nfaStateSet = dfaState->m_nfaStateSet.getMap();
	while (!stack.isEmpty()) {
		DfaAnchorCalcEntry entry = stack.getBackAndPop();
		const NfaState* nfaState = entry.m_nfaState;

		while (!nfaStateSet.getBit(nfaState->m_id)) {
			nfaStateSet.setBit(nfaState->m_id);

			switch (nfaState->m_stateKind) {
				uint_t anchors;

			case NfaStateKind_Split:
				if (!nfaStateSet.getBit(nfaState->m_splitState->m_id))
					stack.append(DfaAnchorCalcEntry(nfaState->m_splitState, entry.m_anchors));

				// ... and fall-through

			case NfaStateKind_OpenCapture:
			case NfaStateKind_CloseCapture:
				nfaState = nfaState->m_nextState;
				break;

			case NfaStateKind_MatchAnchor:
				anchors = entry.m_anchors | nfaState->m_anchor;
				it = anchorTransitionPreMap.visit(anchors);
				if (it->m_value.isEmpty())
					it->m_value = dfaState->m_nfaStateSet; // build anchor closure later

				if (anchors == entry.m_anchors) { // no need to go deeper
					nfaState = nfaState->m_nextState;
					break;
				}

				stack.append(DfaAnchorCalcEntry(nfaState->m_nextState, anchors));
				// ... and fall-through

			default:
				goto Break2;
			}
		}
	Break2:;
	}

	ASSERT(!anchorTransitionPreMap.isEmpty());

	// stage 3 -- finalize anchor transition map (build epsilon+anchor closures and cache resulting DFA states)

	it = anchorTransitionPreMap.getHead();
	for (; it; it++) {
		uint_t anchors = it->getKey();
		it->m_value.buildAnchorClosure(anchors);
		DfaState* dfaAnchorTransition = m_regex->getDfaState(it->m_value);
		dfaAnchorTransition->m_flags |= DfaStateFlag_AnchorTransition; // prevent buildAnchorTransitionMap on this state
		dfaState->m_anchorTransitionMap[anchors] = dfaAnchorTransition;
		dfaState->m_anchorMask |= anchors;
	}
}

void
DfaBuilder::buildCharTransitionMap(DfaState* dfaState) {
	ASSERT(dfaState->m_id == -1);

	m_charRangeMap.clear();
	size_t nfaStateCount = dfaState->m_nfaStateSet.getCount();
	const DfaState* dfaSearchStartState = m_regex->getDfaSearchStartState();

	// stage 1 -- visit all breaking points in the char range map

	for (size_t i = 0; i < nfaStateCount; i++) {
		const NfaState* nfaState = dfaState->m_nfaStateSet[i];
		if (nfaState->m_stateKind < NfaStateKind_FirstMatch)
			continue;

		switch (nfaState->m_stateKind) {
		case NfaStateKind_MatchAnchor:
			dfaState->m_flags |= DfaStateFlag_HasMatchAnchor;
			break;

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

	// stage 2 -- "colorize" the char range map from top priority to bottom

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

	// stage 3 -- finalize transition map

	CharRangeMapIterator it = m_charRangeMap.getHead();
	while (it != m_charRangeMap.getTail()) {
		CharRangeMapIterator nextIt = it.getNext();

		if (it->m_value.isEmpty()) {
			it = nextIt;
			continue;
		}

		it->m_value.buildEpsilonClosure();

		dfaState->m_charTransitionMap.add(
			it->getKey(),
			nextIt->getKey() - 1,
			m_regex->getDfaState(it->m_value)
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
		it->m_value.add(state);
	} while ((++it)->getKey() <= to);
}

//..............................................................................

} // namespace re
} // namespace axl
