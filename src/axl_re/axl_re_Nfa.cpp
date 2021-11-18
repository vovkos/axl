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
#include "axl_re_Nfa.h"
#include "axl_re_Regex.h"

namespace axl {
namespace re {

//..............................................................................

const char* getAnchorString(uint_t anchors) {
	const char* stringTable[Anchor__TransitionMapSize] = {
		"",      // 0
		"^",     // Anchor_Begin
		"$",     // Anchor_End
		"^$",    // Anchor_Begin | Anchor_End
		"\\b",   // Anchor_Word
		"\\b^",  // Anchor_Word | Anchor_Begin
		"\\b$",  // Anchor_Word | Anchor_End
		"\\b^$", // Anchor_Word | Anchor_Begin | Anchor_End
	};

	return anchors < countof(stringTable) ? stringTable[anchors] : "invalid-anchor";
}

//..............................................................................

NfaState::NfaState() {
	m_stateKind = NfaStateKind_Undefined;
	m_id = -1;
	m_flags = 0;
	m_unionData = NULL;
	m_nextState = NULL;
	m_demuxState = NULL;
}

NfaState::~NfaState() {
	if (m_stateKind == NfaStateKind_MatchCharSet && !m_demuxState) {
		ASSERT(m_charSet);
		AXL_MEM_DELETE(m_charSet);
	}
}

bool
NfaState::isMatchChar(utf32_t c) const {
	switch (m_stateKind) {
	case NfaStateKind_MatchChar:
		return c == m_char;

	case NfaStateKind_MatchCharSet:
		ASSERT(m_charSet);
		return m_charSet->isSet(c);

	case NfaStateKind_MatchAnyChar:
		return true;

	default:
		ASSERT(false);
		return false;
	}
}

void
NfaState::createEpsilon(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Epsilon;
	m_nextState = nextState;
}

void
NfaState::createSplit(
	NfaState* nextState,
	NfaState* splitState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Split;
	m_nextState = nextState;
	m_splitState = splitState;
}

void
NfaState::createAccept(size_t acceptId) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Accept;
	m_acceptId = acceptId;
}

void
NfaState::createMatchAnchor(
	Anchor anchor,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_MatchAnchor;
	m_anchor = anchor;
	m_nextState = nextState;
}

void
NfaState::createMatchChar(
	utf32_t c,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_MatchChar;
	m_char = c;
	m_nextState = nextState;
}

void
NfaState::createMatchCharSet(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_MatchCharSet;
	m_charSet = AXL_MEM_NEW(CharSet);
	m_nextState = nextState;
}

void
NfaState::createMatchAnyChar(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_MatchAnyChar;
	m_nextState = nextState;
}

void
NfaState::createOpenCapture(
	size_t captureId,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_OpenCapture;
	m_captureId = captureId;
	m_nextState = nextState;
}

void
NfaState::createCloseCapture(
	size_t captureId,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_CloseCapture;
	m_captureId = captureId;
	m_nextState = nextState;
}

void
NfaState::addChar(utf32_t c) {
	switch (m_stateKind) {
	case NfaStateKind_MatchChar:
		if (m_char == c)
			break; // no change

		m_stateKind = NfaStateKind_MatchCharSet;
		ASSERT(!m_charSet);
		m_charSet = AXL_MEM_NEW(CharSet);
		m_charSet->add(m_char);

		// and fall through

	case NfaStateKind_MatchCharSet:
		m_charSet->add(c);
		break;

	case NfaStateKind_MatchAnyChar:
		break; // no change

	default:
		ASSERT(false);
	}
}

void
NfaState::addCharRange(
	utf32_t from,
	utf32_t to
) {
	switch (m_stateKind) {
	case NfaStateKind_MatchChar:
		if (from == to && from == m_char)
			break; // no change

		m_stateKind = NfaStateKind_MatchCharSet;
		ASSERT(!m_charSet);
		m_charSet = AXL_MEM_NEW(CharSet);
		m_charSet->add(m_char);

		// and fall through

	case NfaStateKind_MatchCharSet:
		ASSERT(m_charSet);
		m_charSet->add(from, to);
		break;

	case NfaStateKind_MatchAnyChar:
		break; // no change

	default:
		ASSERT(false);
	}
}

void
NfaState::resolveOutStates() {
	if (m_nextState && m_nextState->m_stateKind == NfaStateKind_Epsilon)
		m_nextState = m_nextState->resolveEpsilon();

	if (m_stateKind == NfaStateKind_Split && m_splitState->m_stateKind == NfaStateKind_Epsilon)
		m_splitState = m_splitState->resolveEpsilon();
}

NfaState*
NfaState::resolveEpsilon() {
	ASSERT(m_stateKind == NfaStateKind_Epsilon);

	NfaState* result = m_nextState;
	while (result->m_stateKind == NfaStateKind_Epsilon)
		result = result->m_nextState;

	return result;
}

void
NfaState::copy(NfaState& src) {
	if (m_stateKind == NfaStateKind_MatchCharSet) {
		ASSERT(m_charSet);
		AXL_MEM_DELETE(m_charSet);
	}

	m_stateKind = src.m_stateKind;
	m_id = src.m_id;
	m_acceptId = src.m_acceptId;
	m_captureId = src.m_captureId;
	m_nextState = src.m_nextState;
	m_splitState = src.m_splitState;

	if (m_stateKind == NfaStateKind_MatchCharSet) {
		ASSERT(!m_charSet && src.m_charSet);
		m_charSet = AXL_MEM_NEW(CharSet);
		m_charSet->copy(*src.m_charSet);
	}
}

//..............................................................................

bool
NfaStateSet::add(const NfaState* state) {
	if (m_map.getBit(state->m_id))
		return false;

	m_array.append(state);
	m_map.setBitResize(state->m_id);
	return true;
}

void
NfaStateSet::buildEpsilonClosure() {
	char buffer[256];
	sl::Array<const NfaState*> workingSet(rc::BufKind_Stack, buffer, sizeof(buffer));

	size_t count = m_array.getCount();
	for (size_t i = 0; i < count; i++) {
		const NfaState* state = m_array[i];
		if (state->m_stateKind <= NfaStateKind_LastEpsilon)
			workingSet.append(state);
	}

	while (!workingSet.isEmpty()) {
		const NfaState* state = workingSet.getBackAndPop();
		switch (state->m_stateKind) {
			bool isAdded;

		case NfaStateKind_Split:
			isAdded = add(state->m_nextState);
			if (isAdded && state->m_nextState->m_stateKind <= NfaStateKind_LastEpsilon)
				workingSet.append(state->m_nextState);

			isAdded = add(state->m_splitState);
			if (isAdded && state->m_splitState->m_stateKind <= NfaStateKind_LastEpsilon)
				workingSet.append(state->m_splitState);

			break;

		case NfaStateKind_OpenCapture:
		case NfaStateKind_CloseCapture:
			isAdded = add(state->m_nextState);
			if (isAdded && state->m_nextState->m_stateKind <= NfaStateKind_LastEpsilon)
				workingSet.append(state->m_nextState);

			break;
		}
	}
}

//..............................................................................

struct NfaDemuxWorkingSetEntry {
	NfaState* m_state;
	size_t m_alternativeId; // -1 -- not in the closure

	NfaDemuxWorkingSetEntry() {}

	NfaDemuxWorkingSetEntry(
		NfaState* state,
		size_t alternativeId = -1
	) {
		m_state = state;
		m_alternativeId = alternativeId;
	}
};

NfaState*
NfaDemuxer::demuxState(NfaState* state) {
	ASSERT(!state->m_demuxState && (state->m_flags & NfaStateFlag_StartEpsilonClosure));

	NfaState* demuxState = AXL_MEM_NEW_ARGS(NfaState, (*state));
	demuxState->m_demuxState = NULL;
	demuxState->m_flags &= ~NfaStateFlag_StartEpsilonClosure;
	demuxState->m_flags |= NfaStateFlag_Demux;
	demuxState->m_id = m_regex->m_nfaStateList.getCount();
	m_regex->m_nfaStateList.insertTail(demuxState);
	state->m_demuxState = demuxState;
	return demuxState;
}

void
NfaDemuxer::demux() {
	ASSERT(!m_regex->m_nfaStateList.isEmpty());

	sl::Iterator<NfaState> it = m_regex->m_nfaStateList.getHead();

	ASSERT(it->m_stateKind == NfaStateKind_Split && it.getNext()->m_stateKind == NfaStateKind_MatchAnyChar);
	it.inc(2); // skip the search prefix (.*)

	// demux states that are accessible from the outside of the start epsilon closure

	for (; it; it++) {
		NfaState* state = *it;

		if (state->m_stateKind == NfaStateKind_Split && !(state->m_flags & NfaStateFlag_StartEpsilonClosure)) { // split from outside of the closure into the closure
			if (state->m_nextState->m_flags & NfaStateFlag_StartEpsilonClosure)
				state->m_nextState = getDemuxState(state->m_nextState);

			if (state->m_splitState->m_flags & NfaStateFlag_StartEpsilonClosure)
				state->m_splitState = getDemuxState(state->m_splitState);
		} else if (state->m_stateKind >= NfaStateKind_MatchAnchor) { // match into the closure
			if (state->m_nextState->m_flags & NfaStateFlag_StartEpsilonClosure)
				state->m_nextState = getDemuxState(state->m_nextState);
		}
	}
}

//..............................................................................

} // namespace re
} // namespace axl
