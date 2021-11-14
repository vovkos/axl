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
	m_unionData = NULL;
	m_nextState = NULL;
}

NfaState::~NfaState() {
	if (m_stateKind == NfaStateKind_MatchCharSet) {
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

//..............................................................................

} // namespace re
} // namespace axl
