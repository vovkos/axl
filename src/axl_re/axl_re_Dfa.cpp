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

void
DfaCharTransitionMap::add(
	utf32_t from,
	utf32_t to,
	const DfaState* state
) {
	ASSERT(from >= 0 && to <= 0x7fffffff && from <= to);

	Iterator it = m_map.visit(from);
	it->m_value.m_last = to;
	it->m_value.m_state = state;

#if (_AXL_DEBUG)
	ConstIterator prevIt = it.getPrev();
	ConstIterator nextIt = it.getNext();
	ASSERT(!prevIt || prevIt->m_value.m_last < from);
	ASSERT(!nextIt || nextIt->getKey() > to);
#endif
}

//..............................................................................

bool
DfaCaptureIdSet::add(size_t captureId) {
	if (m_map.getBit(captureId))
		return false;

	m_array.append(captureId);
	m_map.setBitResize(captureId);
	return true;
}

//..............................................................................

DfaState::DfaState() {
	m_id = -1;
	m_flags = 0;
	m_anchorMask = 0;
	m_acceptId = -1;
	m_acceptNfaStateId = -1;
}

void
DfaState::copy(const DfaState& src) {
	// don't update id

	m_flags = src.m_flags & ~DfaStateFlag_CharTransitionMap;
	m_acceptId = src.m_acceptId;
	m_acceptNfaStateId = src.m_acceptNfaStateId;
	m_openCaptureIdSet = src.m_openCaptureIdSet;
	m_closeCaptureIdSet = src.m_closeCaptureIdSet;
	m_nfaStateSet.copy(src.m_nfaStateSet);
	m_anchorTransitionMap.clear();
	m_charTransitionMap.clear();
}

void
DfaState::finalize() {
	ASSERT(!(m_flags & DfaStateFlag_Accept));

	size_t count = m_nfaStateSet.getCount();
	for (size_t i = 0; i < count; i++) {
		const NfaState* nfaState = m_nfaStateSet[i];
		ASSERT(nfaState->m_stateKind != NfaStateKind_Epsilon);

		switch (nfaState->m_stateKind) {
		case NfaStateKind_Accept:
			if (!(m_flags & DfaStateFlag_Accept) || nfaState->m_id < m_acceptNfaStateId) {
				m_acceptNfaStateId = nfaState->m_id;
				m_acceptId = nfaState->m_acceptId;
			}

			m_flags |= DfaStateFlag_Accept;
			break;

		case NfaStateKind_OpenCapture:
			ASSERT(nfaState->m_captureId != -1);
			m_openCaptureIdSet.add(nfaState->m_captureId);
			break;

		case NfaStateKind_CloseCapture:
			ASSERT(nfaState->m_captureId != -1);
			m_closeCaptureIdSet.add(nfaState->m_captureId);
			break;
		}
	}
}

//..............................................................................

struct DfaWorkingSetEntry {
	DfaState* m_dfaState;
	const NfaState* m_nfaState;
	uint_t m_anchors;

	DfaWorkingSetEntry() {}

	DfaWorkingSetEntry(
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

//..............................................................................

void
DfaBuilder::makeEpsilonClosure(DfaState* dfaState) {
	ASSERT(!(dfaState->m_flags & DfaStateFlag_EpsilonClosure));

	DfaState* anchorTransitionMap[Anchor__TransitionMapSize] = { 0 };
	uint_t anchorMask = 0;

	sl::Array<DfaWorkingSetEntry> workingSet;
	size_t count = dfaState->m_nfaStateSet.getCount();
	workingSet.setCount(count);

	for (size_t i = 0; i < count; i++)
		workingSet[i].setup(dfaState, dfaState->m_nfaStateSet[i]);

	while (!workingSet.isEmpty()) {
		DfaWorkingSetEntry entry = workingSet.getBackAndPop();

		bool isAdded;
		uint_t anchors;
		DfaState* anchorState;

		switch (entry.m_nfaState->m_stateKind) {
		case NfaStateKind_Accept:
			if (entry.m_nfaState->m_nextState)
				entry.m_dfaState->m_nfaStateSet.add(entry.m_nfaState->m_nextState);
			break;

		case NfaStateKind_Split:
			isAdded = entry.m_dfaState->m_nfaStateSet.add(entry.m_nfaState->m_nextState);
			if (isAdded)
				workingSet.append(DfaWorkingSetEntry(entry.m_dfaState, entry.m_nfaState->m_nextState, entry.m_anchors));

			isAdded = entry.m_dfaState->m_nfaStateSet.add(entry.m_nfaState->m_splitState);
			if (isAdded)
				workingSet.append(DfaWorkingSetEntry(entry.m_dfaState, entry.m_nfaState->m_splitState, entry.m_anchors));

			break;

		case NfaStateKind_OpenCapture:
		case NfaStateKind_CloseCapture:
			isAdded = entry.m_dfaState->m_nfaStateSet.add(entry.m_nfaState->m_nextState);
			if (isAdded)
				workingSet.append(DfaWorkingSetEntry(entry.m_dfaState, entry.m_nfaState->m_nextState, entry.m_anchors));

			break;

		case NfaStateKind_MatchAnchor:
			anchorMask |= entry.m_nfaState->m_anchor;
			anchors = entry.m_anchors | entry.m_nfaState->m_anchor;
			anchorState = anchorTransitionMap[anchors];
			if (!anchorState) {
				anchorState = AXL_MEM_NEW(DfaState);
				anchorState->m_nfaStateSet.copy(dfaState->m_nfaStateSet);
				anchorTransitionMap[anchors] = anchorState;
			}

			isAdded = anchorState->m_nfaStateSet.add(entry.m_nfaState->m_nextState);
			if (isAdded)
				workingSet.append(DfaWorkingSetEntry(entry.m_dfaState, entry.m_nfaState->m_nextState, entry.m_anchors));

			break;
		}
	}

	if (anchorMask) {
		dfaState->m_anchorTransitionMap.setCount(anchorMask + 1);
		for (size_t i = 0; i <= anchorMask; i++)
			dfaState->m_anchorTransitionMap[i] = anchorTransitionMap[i] ?
				m_regex->addDfaState(anchorTransitionMap[i]) :
				NULL;

		dfaState->m_anchorMask = anchorMask;
	}
}

void
DfaBuilder::buildCharTransitionMap(DfaState* dfaState) {
	ASSERT(!(dfaState->m_flags & DfaStateFlag_CharTransitionMap) && dfaState->m_id == -1);

	m_charRangeMap.clear();

	size_t nfaStateCount = dfaState->m_nfaStateSet.getCount();
	for (size_t i = 0; i < nfaStateCount; i++) {
		const NfaState* nfaState = dfaState->m_nfaStateSet[i];

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

	sl::List<CharRangeListEntry> charRangeList;

	// pass 1 -- build state sets

	CharRangeMapIterator prevIt;
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
			else
				it->m_value.m_nfaStateSet.add(it2->m_nfaState);

			it2 = nextIt;
		}

		if (prevIt && it->m_value.m_nfaStateSet.cmp(prevIt->m_value.m_nfaStateSet)) // merge identical state sets
			m_charRangeMap.erase(it);

		it = nextIt;
	}

	// pass 2 -- finalize transition map

	it = m_charRangeMap.getHead();
	while (it != m_charRangeMap.getTail()) {
		CharRangeMapIterator nextIt = it.getNext();

		if (it->m_value.m_nfaStateSet.isEmpty()) {
			it = nextIt;
			continue;
		}

		DfaState* targetState = AXL_MEM_NEW(DfaState);
		sl::takeOver(&targetState->m_nfaStateSet, &it->m_value.m_nfaStateSet);
		makeEpsilonClosure(targetState);
		targetState = m_regex->addDfaState(targetState);

		dfaState->m_charTransitionMap.add(it->getKey(), nextIt->getKey() - 1, targetState);
		it = nextIt;
	}

	dfaState->m_flags |= DfaStateFlag_CharTransitionMap;

	if ((dfaState->m_flags & DfaStateFlag_Accept) && !dfaState->m_anchorMask && dfaState->m_charTransitionMap.isEmpty())
		dfaState->m_flags |= DfaStateFlag_Final; // final states insta-match

	dfaState->m_id = m_regex->m_dfaStateList.getCount();

	m_regex->m_preDfaStateList.remove(dfaState);
	m_regex->m_dfaStateList.insertTail(dfaState);
}

void
DfaBuilder::buildFullDfa() {
	if (m_regex->isEmpty())
		return;

	m_regex->getDfaStartState(); // bootstrap

	while (!m_regex->m_preDfaStateList.isEmpty())
		buildCharTransitionMap(*m_regex->m_preDfaStateList.getHead());
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
	CharRangeMapIterator endIt = m_charRangeMap.visit(to + 1);

	CharRangeListEntry* entry = AXL_MEM_NEW(CharRangeListEntry);
	entry->m_last = to;
	entry->m_nfaState = state;
	it->m_value.m_rangeList.insertTail(entry);
}

//..............................................................................

} // namespace re
} // namespace axl
