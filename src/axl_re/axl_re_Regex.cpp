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
#include "axl_re_Regex.h"
#include "axl_re_RegexCompiler.h"

namespace axl {
namespace re {

//..............................................................................

static
inline
void
appendChar(
	sl::String* string,
	utf32_t c
) {
	if (enc::utfIsPrintable(c) && c < 0x80)
		string->append(c);
	else if (c <= 0xffff)
		string->appendFormat("\\u%04x", c);
	else
		string->appendFormat("\\U%08x", c);
}

static
sl::String
getCharSetString(const CharSet* charSet) {
	sl::String string;

	uint_t c1 = -1;

	CharSet::ConstIterator it = charSet->getHead();
	for (; it != 0; it++) {
		utf32_t from = it->getKey();
		utf32_t to = it->m_value;

		if (from == to)
			appendChar(&string, from);
 		else {
			appendChar(&string, from);
			string += '-';
			appendChar(&string, to);
		}
	}

	return string;
}

//..............................................................................

Regex::Regex() {
	m_regexKind = RegexKind_Undefined;
	m_groupCount = 0;
	m_maxSubMatchCount = 0;
}

void
Regex::clear() {
	m_regexKind = RegexKind_Undefined;
	m_groupCount = 0;
	m_maxSubMatchCount = 0;
	m_nfaStateList.clear();
	m_nfaStateArray.clear();
	m_dfaStateList.clear();
	m_dfaStateMap.clear();
	m_caseContextList.clear();
}

bool
Regex::compile(const sl::StringRef& source) {
	clear();
	m_regexKind = RegexKind_Normal;

	RegexCompiler compiler(this);
	bool result = compiler.compile(source);
	if (!result)
		return false;

	finalize();
	return true;
}

void
Regex::createSwitch(RegexKind switchKind) {
	clear();
	m_regexKind = switchKind;
}

bool
Regex::compileSwitchCase(
	const sl::StringRef& source,
	const rc::Ptr<void>& caseContext
) {
	ASSERT(m_regexKind = RegexKind_Switch);

	SwitchCaseContext* context = AXL_MEM_NEW(SwitchCaseContext);
	context->m_index = m_caseContextList.getCount();
	context->m_context = caseContext;
	context->m_firstGroupId = m_groupCount;
	m_caseContextList.insertTail(context);

	RegexCompiler compiler(this);
	bool result = compiler.compile(source, context);
	if (!result)
		return false;

	context->m_groupCount = m_groupCount - context->m_firstGroupId;
	return true;
}

void
Regex::finalize() {
	ASSERT(m_dfaStateList.isEmpty()); // only once

	// firstly, get rid of non-split epsilon transition

	sl::List<NfaState> epsilonList; // must keep epsilon states alive while finalizing
	m_nfaStateArray.reserve(m_nfaStateList.getCount() / 2); // estimate
	m_nfaStateArray.clear();

	sl::Iterator<NfaState> it = m_nfaStateList.getHead();
	for (uint_t i = 0; it; ) {
		NfaState* state = *it++;
		if (state->m_stateKind == NfaStateKind_Epsilon) {
			m_nfaStateList.remove(state);
			epsilonList.insertTail(state);
		} else {
			state->m_id = i++;
			m_nfaStateArray.append(state);
			state->resolveOutStates();
		}
	}

	ASSERT(m_nfaStateList.getCount() == m_nfaStateArray.getCount());
	if (m_nfaStateList.isEmpty())
		return;

	// now, create the initial DFA state

	DfaState* start = AXL_MEM_NEW(DfaState);
	start->addNfaState(*m_nfaStateList.getHead());
	start->makeEpsilonClosure();
	addDfaState(start);
}

DfaState*
Regex::addDfaState(DfaState* state) {
	NfaStateSetMap<DfaState*>::Iterator it = m_dfaStateMap.visit(&state->m_nfaStateSet);
	if (it->m_value) {
		AXL_MEM_DELETE(state);
		return it->m_value;
	}

	m_dfaStateList.insertTail(state);
	it->m_value = state;
	return state;
}

void
Regex::prepareDfaState(DfaState* state) {
#if (0)
	nfaTransitionMgr.clear();

	size_t nfaStateCount = dfaState->m_nfaStateSet.m_stateArray.getCount();
	for (size_t i = 0; i < nfaStateCount; i++) {
		NfaState* nfaState = dfaState->m_nfaStateSet.m_stateArray[i];
		if (nfaState->m_flags & NfaStateFlag_Match)
			nfaTransitionMgr.addMatchState(nfaState);
	}

	nfaTransitionMgr.finalize();

	sl::ConstList<NfaTransition> nfaTransitionList = nfaTransitionMgr.getTransitionList();
	sl::ConstIterator<NfaTransition> nfaTransitionIt = nfaTransitionList.getHead();
	for (; nfaTransitionIt; nfaTransitionIt++) {
		const NfaTransition* nfaTransition = *nfaTransitionIt;

		DfaState* dfaState2 = AXL_MEM_NEW(DfaState);

		size_t outCount = nfaTransition->m_outStateSet.m_stateArray.getCount();
		for (size_t i = 0; i < outCount; i++) {
			NfaState* nfaState = nfaTransition->m_outStateSet.m_stateArray[i];
			dfaState2->addNfaState(nfaState);
		}

		dfaState2->makeEpsilonClosure();

		NfaStateSetMap<DfaState*>::Iterator mapIt = dfaStateMap.visit(&dfaState2->m_nfaStateSet);
		if (mapIt->m_value) {
			AXL_MEM_DELETE(dfaState2);
			dfaState2 = mapIt->m_value;
		} else {
			m_regex->m_dfaStateList.insertTail(dfaState2);
			workingSet.append(dfaState2);
			mapIt->m_value = dfaState2;
		}

		DfaTransition* dfaTransition = AXL_MEM_NEW(DfaTransition);
		dfaTransition->m_matchCondition = nfaTransition->m_matchCondition;
		dfaTransition->m_outState = dfaState2;
		dfaState->m_transitionList.insertTail(dfaTransition);
	}
#endif
}

bool
Regex::match(
	enc::CharCodec* codec,
	const void* p,
	size_t size
) {
	if (isEmpty())
		return true;

	RegexState state(this, codec);
	state.exec(p, size);
	state.eof();
	return true;
}

#if (_AXL_DEBUG)
void
Regex::print() const {
	printf("NFA\n");
	printf("==================\n");

	sl::String string;

	sl::ConstIterator<NfaState> it = m_nfaStateList.getHead();
	for (; it; it++) {
		const NfaState* state = *it;
		printf("%02d: ", state->m_id);

		switch (state->m_stateKind) {
		case NfaStateKind_Accept:
			if (state->m_nextState)
				printf("accept -> %02d", state->m_nextState->m_id);
			else
				printf("accept (final)");
			break;

		case NfaStateKind_Epsilon:
			ASSERT(state->m_nextState && !state->m_splitState);
			printf("eps -> %02d", state->m_nextState->m_id);
			break;

		case NfaStateKind_Split:
			ASSERT(state->m_nextState && state->m_splitState);
			printf("split -> %02d : %02d", state->m_nextState->m_id, state->m_splitState->m_id);
			break;

		case NfaStateKind_Match:
			switch (state->m_matchCondition.m_matchKind) {
			case FsmMatchKind_Char:
				ASSERT(state->m_nextState && !state->m_splitState);

				string.clear();
				appendChar(&string, state->m_matchCondition.m_char);
				printf("'%s' -> %02d", string.sz(), state->m_nextState->m_id);
				break;

			case FsmMatchKind_CharSet:
				ASSERT(state->m_nextState && !state->m_splitState);
				printf("[%s] -> %02d", getCharSetString(state->m_matchCondition.m_charSet).sz(), state->m_nextState->m_id);
				break;

			case FsmMatchKind_AnyChar:
				ASSERT(state->m_nextState && !state->m_splitState);
				printf(". -> %02d", state->m_nextState->m_id);
				break;

			case FsmMatchKind_Anchor:
					ASSERT(state->m_nextState && !state->m_splitState);
					switch (state->m_matchCondition.m_anchor) {
					case FsmAnchor_Begin:
						printf("^ -> %02d", state->m_nextState->m_id);
						break;

					case FsmAnchor_End:
						printf("$ -> %02d", state->m_nextState->m_id);
						break;

					case FsmAnchor_Word:
						printf("\\b -> %02d", state->m_nextState->m_id);
						break;

					default:
							ASSERT(false);
					}

				break;
			}

		case NfaStateKind_OpenCapture:
			ASSERT(state->m_nextState && !state->m_splitState);
			printf("open(%d) -> %02d", state->m_captureId, state->m_nextState->m_id);
			break;

		case NfaStateKind_CloseCapture:
			ASSERT(state->m_nextState && !state->m_splitState);
			printf("close(%d) -> %02d", state->m_captureId, state->m_nextState->m_id);
			break;

		default:
			ASSERT(false);
		}

		printf("\n");
	}
}
#endif

//..............................................................................

} // namespace re
} // namespace axl
