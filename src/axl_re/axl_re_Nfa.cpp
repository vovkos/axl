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

const char*
getAnchorString(Anchor anchor) {
	const char* stringTable[] = {
		"^",     // Anchor_BeginLine
		"$",     // Anchor_EndLine
		"\\A",   // Anchor_BeginText
		"\\z",   // Anchor_EndText
		"\\b",   // Anchor_WordBoundary
		"\\B",   // Anchor_NotWordBoundary
	};

	size_t i = sl::getLoBitIdx(anchor);
	return i < countof(stringTable) ? stringTable[i] : "invalid-anchor";
}

sl::String
getAnchorsString(uint_t anchors) {
	sl::String string;

	if (anchors & Anchor_BeginLine)
		string = "^";

	if (anchors & Anchor_EndLine)
		string += "$";

	if (anchors & Anchor_BeginText)
		string += "\\A";

	if (anchors & Anchor_EndText)
		string += "\\z";

	if (anchors & Anchor_WordBoundary)
		string += "\\b";

	if (anchors & Anchor_NotWordBoundary)
		string += "\\B";

	return string;
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
NfaState::createAccept(size_t id) {
	ASSERT(!m_stateKind && !m_nextState);

	m_stateKind = NfaStateKind_Accept;
	m_acceptId = id;
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
	m_captureId = src.m_captureId;
	m_nextState = src.m_nextState;
	m_splitState = src.m_splitState;

	if (m_stateKind == NfaStateKind_MatchCharSet) {
		ASSERT(!m_charSet && src.m_charSet);
		m_charSet = AXL_MEM_NEW(CharSet);
		m_charSet->copy(*src.m_charSet);
	}
}

#if (_AXL_DEBUG)

void
NfaState::print(FILE* file) const {
	fprintf(file, "%02d: ", m_id);

	char buffer[256];
	sl::String string(rc::BufKind_Stack, buffer, sizeof(buffer));

	switch (m_stateKind) {
	case NfaStateKind_Accept:
		fprintf(file, "accept");
		break;

	case NfaStateKind_Epsilon:
		ASSERT(m_nextState);
		fprintf(file, "eps -> %02d", m_nextState->m_id);
		break;

	case NfaStateKind_Split:
		ASSERT(m_nextState && m_splitState);
		fprintf(file, "split -> %02d : %02d", m_nextState->m_id, m_splitState->m_id);
		break;

	case NfaStateKind_MatchChar:
		ASSERT(m_nextState);
		getCharString(&string, m_char);
		fprintf(file, "'%s' -> %02d", string.sz(), m_nextState->m_id);
		break;

	case NfaStateKind_MatchCharSet:
		ASSERT(m_nextState);
		m_charSet->getString(&string);
		fprintf(file, "[%s] -> %02d", string.sz(), m_nextState->m_id);
		break;

	case NfaStateKind_MatchAnyChar:
		ASSERT(m_nextState);
		fprintf(file, ". -> %02d", m_nextState->m_id);
		break;

	case NfaStateKind_MatchAnchor:
		ASSERT(m_nextState);
		fprintf(file, "%s -> %02d", getAnchorString(m_anchor), m_nextState->m_id);
		break;

	case NfaStateKind_OpenCapture:
		ASSERT(m_nextState);
		fprintf(file, "open(%d) -> %02d", m_captureId, m_nextState->m_id);
		break;

	case NfaStateKind_CloseCapture:
		ASSERT(m_nextState);
		fprintf(file, "close(%d) -> %02d", m_captureId, m_nextState->m_id);
		break;

	default:
		ASSERT(false);
	}

	fprintf(file, "\n");
}

#endif

//..............................................................................

bool
NfaStateSet::add(const NfaState* state) {
	if (m_map.getBit(state->m_id) || isAccept())
		return false;

	m_array.append(state);
	m_map.setBitResize(state->m_id);
	return true;
}

void
NfaStateSet::buildAnchorClosure(uint_t anchors) {
	char buffer[256];
	sl::Array<const NfaState*> stack(rc::BufKind_Stack, buffer, sizeof(buffer));

	// push to stack in the inversed order

	size_t count = m_array.getCount();
	stack.setCount(count);
	for (size_t i = 0, j = count - 1; i < count; i++, j--)
		stack[i] = m_array[j];

	m_array.clear();
	m_map.clear();

	while (!stack.isEmpty()) {
		const NfaState* state = stack.getBackAndPop();
		while (!m_map.getBit(state->m_id)) {
			m_array.append(state);
			m_map.setBit(state->m_id);

			switch (state->m_stateKind) {
			case NfaStateKind_Accept:
				return; // we are done

			case NfaStateKind_Split:
				if (!m_map.getBit(state->m_splitState->m_id))
					stack.append(state->m_splitState);

				// ... and fall-through

			case NfaStateKind_OpenCapture:
			case NfaStateKind_CloseCapture:
				state = state->m_nextState;
				break;

			case NfaStateKind_MatchAnchor:
				if (anchors & state->m_anchor) {
					state = state->m_nextState;
					break;
				}

				// ... and fall-through

			default:
				goto Break2;
			}
		}
	Break2:;
	}
}

//..............................................................................

} // namespace re
} // namespace axl
