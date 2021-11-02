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

NfaState::NfaState() {
	m_stateKind = NfaStateKind_Undefined;
	m_id = -1;
	m_acceptContext = NULL;
	m_nextState = NULL;
	m_splitState = NULL;
}

void
NfaState::createAccept(void* context) {
	ASSERT(!m_stateKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_Accept;
	m_acceptContext = context;
}

void
NfaState::createEpsilon(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_Epsilon;
	m_nextState = nextState;
}

void
NfaState::createSplit(
	NfaState* nextState,
	NfaState* splitState
) {
	ASSERT(!m_stateKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_Split;
	m_nextState = nextState;
	m_splitState = splitState;
}

void
NfaState::createMatchAnchor(
	FsmAnchor anchor,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_matchCondition.m_matchKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_Match;
	m_matchCondition.m_matchKind = FsmMatchKind_Anchor;
	m_matchCondition.m_anchor = anchor;
	m_nextState = nextState;
}

void
NfaState::createMatchChar(
	utf32_t c,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_matchCondition.m_matchKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_Match;
	m_matchCondition.m_matchKind = FsmMatchKind_Char;
	m_matchCondition.m_char = c;
	m_nextState = nextState;
}

void
NfaState::createMatchCharSet(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_matchCondition.m_matchKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_Match;
	m_matchCondition.m_matchKind = FsmMatchKind_CharSet;
	m_matchCondition.m_charSet = AXL_MEM_NEW(CharSet);
	m_nextState = nextState;
}

void
NfaState::createMatchAnyChar(NfaState* nextState) {
	ASSERT(!m_stateKind && !m_matchCondition.m_matchKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_Match;
	m_matchCondition.m_matchKind = FsmMatchKind_AnyChar;
	m_nextState = nextState;
}

void
NfaState::createOpenCapture(
	size_t captureId,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_OpenCapture;
	m_captureId = captureId;
	m_nextState = nextState;
}

void
NfaState::createCloseCapture(
	size_t captureId,
	NfaState* nextState
) {
	ASSERT(!m_stateKind && !m_nextState && !m_splitState);

	m_stateKind = NfaStateKind_CloseCapture;
	m_captureId = captureId;
	m_nextState = nextState;
}

void
NfaState::resolveOutStates() {
	if (m_nextState && m_nextState->m_stateKind == NfaStateKind_Epsilon)
		m_nextState = m_nextState->resolveEpsilon();

	if (m_splitState && m_splitState->m_stateKind == NfaStateKind_Epsilon)
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
	m_stateKind = src.m_stateKind;
	m_id = src.m_id;
	m_acceptContext = src.m_acceptContext;
	m_captureId = src.m_captureId;
	m_nextState = src.m_nextState;
	m_splitState = src.m_splitState;
	m_matchCondition.copy(src.m_matchCondition);
}

//..............................................................................

bool
NfaStateSet::addState(NfaState* state) {
	if (m_stateSet.getBit(state->m_id))
		return false;

	m_stateArray.append(state);

	if (state->m_id >= m_stateSet.getBitCount())
		m_stateSet.setBitCount(state->m_id + 1);

	m_stateSet.setBit(state->m_id);
	return true;
}

//..............................................................................

} // namespace re
} // namespace axl
