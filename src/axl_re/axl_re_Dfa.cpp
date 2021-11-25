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
	m_rollbackState = NULL;
}

void
DfaState::print(FILE* file) const {
	fprintf(file, "%02d: nfa({ ", m_id);

	#define INDENT "      "

	size_t count = m_nfaStateSet.getCount();
	for (size_t i = 0; i < count; i++) {
		const NfaState* nfaState = m_nfaStateSet[i];
		fprintf(file, "%02d%s ", nfaState->m_id, getNfaStateKindCodeString(nfaState->m_stateKind));
	}

	fprintf(file, "})\n");

	if (m_rollbackState)
		fprintf(file, INDENT "rollback(%02d)\n", m_rollbackState->m_id);

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

DfaProgram::DfaProgram(uint_t stateFlags) {
	m_stateFlags = stateFlags;
	m_matchStartState = NULL;
	m_searchStartState = NULL;
}

void
DfaProgram::clear() {
	m_stateList.clear();
	m_preStateList.clear();
	m_stateMap.clear();
	m_matchStartState = NULL;
	m_searchStartState = NULL;
}

DfaState*
DfaProgram::getState(const NfaStateSet& nfaStateSet) {
	NfaStateSetMap<DfaState*>::Iterator it = m_stateMap.visit(nfaStateSet);
	if (it->m_value)
		return it->m_value;

	DfaState* state = AXL_MEM_NEW(DfaState);
	state->m_flags = m_stateFlags;
	state->m_nfaStateSet = nfaStateSet;

	if (nfaStateSet.isAccept()) {
		state->m_acceptId = state->m_nfaStateSet.getLastState()->m_acceptId;
		state->m_flags |= DfaStateFlag_Accept;
	}

	m_preStateList.insertTail(state);
	it->m_value = state;
	return state;
}

DfaState*
DfaProgram::createStartState(const NfaState* nfaState) {
	NfaStateSet nfaStateSet;
	nfaStateSet.add(nfaState);

	if (m_stateFlags & DfaStateFlag_Reverse)
		nfaStateSet.buildEpsilonClosure<sl::True>();
	else
		nfaStateSet.buildEpsilonClosure<sl::False>();

	return getState(nfaStateSet);
}

DfaState*
DfaProgram::createRollbackState(const DfaState* dfaState) {
	NfaStateSet nfaStateSet = dfaState->m_nfaStateSet;
	nfaStateSet.buildRollbackClosure();
	return getState(nfaStateSet);
}

#if (_AXL_DEBUG)

void
DfaProgram::print(FILE* file) const {
	sl::String string;

	sl::ConstIterator<DfaState> it = m_stateList.getHead();
	for (; it; it++) {
		const DfaState* state = *it;
		fprintf(
			file,
			"%c ",
			state == m_matchStartState ? 'M' :
			state == m_searchStartState ? 'S' : ' '
		);

		state->print(file);
	}
}

#endif

//..............................................................................

void
DfaBuilder::buildTransitionMaps(DfaState* state) {
	ASSERT(!(state->m_flags & DfaStateFlag_Ready) && state->m_id == -1);

	bool hasMatchAnchors = buildCharTransitionMap(state);
	if (hasMatchAnchors)
		buildAnchorTransitionMap(state);

	if (state->m_anchorTransitionMap.isEmpty() &&
		state->m_charTransitionMap.isEmpty())
		state->m_flags |= DfaStateFlag_Dead;

	state->m_flags |= DfaStateFlag_Ready;
	state->m_id = m_program->m_stateList.getCount();

	m_program->m_preStateList.remove(state);
	m_program->m_stateList.insertTail(state);
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

	// stage 3 -- finalize anchor transition map (build epsilon+anchor closures and cache resulting DFA states)

	ASSERT(!anchorTransitionPreMap.isEmpty());

	if (m_program->m_stateFlags & DfaStateFlag_Reverse)
		finalzeAnchorTransitionMap<sl::True>(dfaState, anchorTransitionPreMap);
	else
		finalzeAnchorTransitionMap<sl::False>(dfaState, anchorTransitionPreMap);
}

template <typename IsReverse>
void
DfaBuilder::finalzeAnchorTransitionMap(
	DfaState* state,
	DfaAnchorTransitionPreMap& preMap
) {
	DfaAnchorTransitionPreMap::Iterator it = preMap.getHead();

	for (; it; it++) {
		uint_t anchors = it->getKey();
		it->m_value.buildAnchorClosure<IsReverse>(anchors);
		DfaState* targetState = m_program->getState(it->m_value);
		state->m_anchorTransitionMap[anchors] = targetState;
		state->m_anchorMask |= anchors;
	}
}

bool
DfaBuilder::buildCharTransitionMap(DfaState* dfaState) {
	ASSERT(dfaState->m_id == -1);

	m_charRangeMap.clear();
	size_t nfaStateCount = dfaState->m_nfaStateSet.getCount();

	// stage 1 -- visit all breaking points in the char range map

	bool hasMatchAnchors = false;

	for (size_t i = 0; i < nfaStateCount; i++) {
		const NfaState* nfaState = dfaState->m_nfaStateSet[i];
		if (nfaState->m_stateKind < NfaStateKind_FirstMatch)
			continue;

		switch (nfaState->m_stateKind) {
		case NfaStateKind_MatchAnchor:
			hasMatchAnchors = true;
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

	if (m_program->m_stateFlags & DfaStateFlag_Reverse)
		finalzeCharTransitionMap<sl::True>(dfaState);
	else
		finalzeCharTransitionMap<sl::False>(dfaState);

	return hasMatchAnchors;
}

template <typename IsReverse>
void
DfaBuilder::finalzeCharTransitionMap(DfaState* state) {
	CharRangeMapIterator it = m_charRangeMap.getHead();
	while (it != m_charRangeMap.getTail()) {
		CharRangeMapIterator nextIt = it.getNext();
		if (it->m_value.isEmpty()) {
			it = nextIt;
			continue;
		}

		it->m_value.buildEpsilonClosure<IsReverse>();
		DfaState* targetState = m_program->getState(it->m_value);
		state->m_charTransitionMap.add(it->getKey(), nextIt->getKey() - 1, targetState);
		it = nextIt;
	}
}

//..............................................................................

} // namespace re
} // namespace axl
