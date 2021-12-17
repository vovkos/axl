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
getNfaStateKindCodeString(NfaStateKind stateKind) {
	const char* stringTable[] = {
		"?", // NfaStateKind_Undefined = 0,
		"!", // NfaStateKind_Accept,
		"-", // NfaStateKind_Epsilon,
		"|", // NfaStateKind_Split,
		"+", // NfaStateKind_Link,
		">", // NfaStateKind_Sequence,
 		"(", // NfaStateKind_OpenCapture,
		")", // NfaStateKind_CloseCapture,
		"m", // NfaStateKind_MatchAnchor,
		"m", // NfaStateKind_MatchChar,
		"m", // NfaStateKind_MatchCharSet,
		"m", // NfaStateKind_MatchAnyChar,
	};

	return (size_t)stateKind < countof(stringTable) ?
		stringTable[stateKind] :
		stringTable[0];
}

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
	return i < countof(stringTable) ?
		stringTable[i] :
		"invalid-anchor";
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
	m_flags = 0;
	m_id = -1;
	m_nextState = NULL;
	m_unionData[0] = 0;
	m_unionData[1] = 0;
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

	switch (m_stateKind) {
	case NfaStateKind_Split:
		if (m_splitState->m_stateKind == NfaStateKind_Epsilon)
			m_splitState = m_splitState->resolveEpsilon();
		break;

	case NfaStateKind_Link:
		if (m_opState->m_stateKind == NfaStateKind_Epsilon)
			m_opState = m_opState->resolveEpsilon();

		if (m_reverseState->m_stateKind == NfaStateKind_Epsilon)
			m_reverseState = m_reverseState->resolveEpsilon();
		break;

	case NfaStateKind_Sequence:
		if (m_tailState->m_stateKind == NfaStateKind_Epsilon)
			m_tailState = m_tailState->resolveEpsilon();
		break;
	}
}

NfaState*
NfaState::resolveEpsilon() {
	ASSERT(m_stateKind == NfaStateKind_Epsilon);
	if (m_nextState->m_stateKind == NfaStateKind_Epsilon)
		m_nextState = m_nextState->resolveEpsilon(); // avoid going through the same epsilon chains
	return m_nextState;
}

void
NfaState::copy(NfaState& src) {
	if (m_stateKind == NfaStateKind_MatchCharSet) {
		ASSERT(m_charSet);
		AXL_MEM_DELETE(m_charSet);
	}

	m_stateKind = src.m_stateKind;
	m_id = src.m_id;
	m_flags = src.m_flags;
	m_nextState = src.m_nextState;
	m_unionData[0] = src.m_unionData[0];
	m_unionData[1] = src.m_unionData[1];

	if (m_stateKind == NfaStateKind_MatchCharSet) {
		ASSERT(src.m_charSet);
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
		fprintf(file, "accept(%d)", m_acceptId);
		break;

	case NfaStateKind_Epsilon:
		ASSERT(m_nextState);
		fprintf(file, "eps -> %02d", m_nextState->m_id);
		break;

	case NfaStateKind_Split:
		ASSERT(m_nextState && m_splitState);
		fprintf(file, "split -> %02d : %02d", m_nextState->m_id, m_splitState->m_id);
		break;

	case NfaStateKind_Link:
		ASSERT(m_nextState && m_reverseState && m_opState);
		fprintf(
			file,
			"link %02d -> %02d : %02d",
			m_opState->m_id,
			m_nextState->m_id,
			m_reverseState->m_id
		);
		break;

	case NfaStateKind_Sequence:
		ASSERT(m_nextState && m_tailState);
		fprintf(file, "sequence -> %02d : %02d", m_nextState->m_id, m_tailState->m_id);
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

// instantiate templated closure builders

bool (NfaStateSet::*NfaStateSet_buildClosure0)() = &NfaStateSet::buildEpsilonClosure<sl::False>;
bool (NfaStateSet::*NfaStateSet_buildClosure1)() = &NfaStateSet::buildEpsilonClosure<sl::True>;
bool (NfaStateSet::*NfaStateSet_buildClosure2)(uint_t) = &NfaStateSet::buildAnchorClosure<sl::False>;
bool (NfaStateSet::*NfaStateSet_buildClosure3)(uint_t) = &NfaStateSet::buildAnchorClosure<sl::True>;
bool (NfaStateSet::*NfaStateSet_buildClosure4)() = &NfaStateSet::buildRollbackClosure;

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

bool
NfaStateSet::add(const NfaState* state) {
	if (m_map.getBit(state->m_id) || isAccept())
		return false;

	m_array.append(state);
	m_map.setBitResize(state->m_id);
	return true;
}

// push to stack in inversed order so that we pop in correct order

inline
void
initializeClosureStack(
	sl::Array<const NfaState*>* stack,
	const sl::ArrayRef<const NfaState*>& array
) {
	size_t count = array.getCount();
	stack->setCount(count);
	for (size_t i = 0, j = count - 1; i < count; i++, j--)
		(*stack)[i] = array[j];
}

// this one takes special care of sequences -- it inverses the order of adjacent nullable sequence entries

inline
void
initializeRollbackClosureStack(
	sl::Array<const NfaState*>* stack,
	const sl::ArrayRef<const NfaState*>& array
) {
	// push to stack in inversed order so that we pop in correct order

	sl::BitMap map;

	size_t count = array.getCount();
	stack->reserve(count);
	for (intptr_t i = count - 1; i >= 0; i--) {
		const NfaState* state = array[i];
		if (state->m_stateKind != NfaStateKind_Link || map.getBit(state->m_id))
			continue;

		map.setBitResize(state->m_id);

		if (!(state->m_opState->m_flags & NfaStateFlag_Nullable)) {
			stack->append(state->m_opState);
			continue;
		}

		// special treatment required for nullable sequence entries -- must inverse order within this sequence;
		// first, scan to the very first non-nullable entry (or all the way to the sequence head)

		const NfaState* p = state;
		while (p->m_reverseState->m_stateKind == NfaStateKind_Link) {
			p = p->m_reverseState;
			if (!(p->m_opState->m_flags & NfaStateFlag_Nullable))
				break;
		}

		// now, push all predcessors to gain the reversed order

		for (; p != state; p = p->m_nextState)
			if (!map.getBit(p->m_id)) {
				map.setBitResize(p->m_id);
				stack->append(p->m_opState);
			}

		// finally, push this nullable entry

		stack->append(state->m_opState);
	}
}

template <
	typename IsRollback,
	typename IsReverse,
	typename UseAnchors
>
bool
NfaStateSet::buildClosureImpl(uint_t anchors) {
	char buffer[256];
	sl::Array<const NfaState*> stack(rc::BufKind_Stack, buffer, sizeof(buffer));

	if (IsRollback()())
		initializeRollbackClosureStack(&stack, m_array);
	else
		initializeClosureStack(&stack, m_array);

	m_array.clear();
	m_map.clear();

	bool result = false;

	while (!stack.isEmpty()) {
		const NfaState* state = stack.getBackAndPop();
		while (!m_map.getBit(state->m_id)) {
			m_map.setBit(state->m_id);

			switch (state->m_stateKind) {
			case NfaStateKind_Accept:
				m_array.append(state);
 				return true; // done!

			case NfaStateKind_Link:
				if (!IsRollback()() && !IsReverse()()) // links are only needed for rollbacks
					m_array.append(state);

				state = IsReverse()() ? state->m_reverseState : state->m_nextState;
				if (state->m_stateKind == NfaStateKind_Link)
					state = state->m_opState;
				break;

			case NfaStateKind_Sequence:
				state = IsReverse()() ? state->m_tailState->m_opState : state->m_nextState->m_opState;
				break;

			case NfaStateKind_Split:
				if (!m_map.getBit(state->m_splitState->m_id))
					stack.append(state->m_splitState);

				// ... and fall-through

			case NfaStateKind_OpenCapture:
			case NfaStateKind_CloseCapture:
				state = state->m_nextState;
				break;

			case NfaStateKind_MatchAnchor:
				if (UseAnchors()()) {
					if (anchors & state->m_anchor) {
						state = state->m_nextState;
						break;
					}

					goto Break2; // don't add match anchor states to anchor closures
				}

				// ... and fall-through

			case NfaStateKind_MatchChar:
			case NfaStateKind_MatchCharSet:
			case NfaStateKind_MatchAnyChar:
				m_array.append(state);
				result = true;

				// ... and fall-through

			default:
				goto Break2;
			}
		}
	Break2:;
	}

	return result;
}

//..............................................................................

NfaProgram::NfaProgram() {
	m_matchStartState = NULL;
	m_searchStartState = NULL;
	m_captureCount = 0;
}

void
NfaProgram::clear() {
	m_stateList.clear();
	m_matchStartState = NULL;
	m_searchStartState = NULL;
	m_captureCount = 0;
}

void
NfaProgram::finalize(bool isMatchOnly) {
	if (m_stateList.isEmpty())
		return;

	// assign start states

	m_matchStartState = *m_stateList.getHead();

	if (!isMatchOnly) {
		NfaState* matchAny = AXL_MEM_NEW(NfaState);
		NfaState* split = AXL_MEM_NEW(NfaState);
		split->createSplit(m_matchStartState, matchAny);
		matchAny->createMatchAnyChar(split);
		m_stateList.insertHead(matchAny);
		m_stateList.insertHead(split);
		m_searchStartState = split;
	}

	// get rid of epsilons and assign state ids

	sl::List<NfaState> epsilonList; // must keep epsilon states alive while finalizing

	sl::Iterator<NfaState> it = m_stateList.getHead();
	for (uint_t i = 0; it; ) {
		NfaState* state = *it++;
		if (state->m_stateKind == NfaStateKind_Epsilon) {
			m_stateList.remove(state);
			epsilonList.insertTail(state);
		} else {
			state->m_id = i++;
			state->resolveOutStates();
		}
	}
}

#if (_AXL_DEBUG)

void
NfaProgram::print(FILE* file) const {
	sl::String string;

	sl::ConstIterator<NfaState> it = m_stateList.getHead();
	for (; it; it++) {
		const NfaState* state = *it;
		fprintf(
			file,
			"%c%c",
			state == m_matchStartState ? 'M' :
			state == m_searchStartState ? 'S' : ' ',
			(state->m_flags & NfaStateFlag_Nullable) ? '?' : ' '
		);

		state->print(file);
	}
}

#endif

//..............................................................................

} // namespace re
} // namespace axl
