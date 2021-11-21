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
#include "axl_re_RegexExecDfa.h"
#include "axl_re_Regex.h"

namespace axl {
namespace re {

//..............................................................................

RegexExecDfa::RegexExecDfa(RegexStateImpl* parent):
	RegexExecEngine(parent) {
	m_state = NULL;
	m_matchEndOffset = -1;
	m_matchAcceptId = -1;

}

void
RegexExecDfa::reset() {
	m_matchEndOffset = -1;
	m_matchAcceptId = -1;

	gotoState(
		(m_parent->m_execFlags & RegexExecFlag_AnchorDataBegin) ?
			m_parent->m_regex->getDfaMatchStartState() :
			m_parent->m_regex->getDfaSearchStartState()
	);
}

bool
RegexExecDfa::exec(
	const void* p0,
	size_t size
) {
	char const* p = (char*)p0;
	char const* end = p + size;

	bool isFirstCpl = true;

	while (p < end) {
		uchar_t cplBuffer[DecodeBufferSize];
		utf32_t textBuffer[DecodeBufferSize];
		size_t takenSize;
		size_t length = m_parent->m_decoder.decode(cplBuffer, textBuffer, DecodeBufferSize, p, end - p, &takenSize);
		p += takenSize;

		for (size_t i = 0; i < length; i++) {
			utf32_t c = textBuffer[i];
			uint_t anchors = m_parent->calcAnchors(c) & m_state->m_anchorMask;
			if (anchors) {
				ASSERT(m_state->m_anchorTransitionMap[anchors]);
				gotoState(m_state->m_anchorTransitionMap[anchors]);
			}

			const DfaState* nextState = m_state->m_charTransitionMap.find(c);
			if (!nextState)
				return m_parent->finalize(false);

			m_parent->m_offset += cplBuffer[i];
			gotoState(nextState);

			if (nextState->m_flags & DfaStateFlag_Final)
				return m_parent->finalize(false);
		}
	}

	return true;
}

bool
RegexExecDfa::eof() {
	uint_t anchors = Anchor_End;
	if (m_parent->m_prevCharFlags & RegexStateImpl::CharFlag_AlphaNum)
		anchors |= Anchor_Word;

	anchors &= m_state->m_anchorMask;
	if (anchors) {
		ASSERT(m_state->m_anchorTransitionMap[anchors]);
		gotoState(m_state->m_anchorTransitionMap[anchors]);
	}

	return m_parent->finalize(true);
}

void
RegexExecDfa::gotoState(const DfaState* state) {
	// build DFA on-the-fly

	if (!(state->m_flags & DfaStateFlag_TransitionMaps)) {
		DfaBuilder builder(m_parent->m_regex);
		builder.buildTransitionMaps((DfaState*)state);
	}

	// process accept state

	if (state->m_flags & DfaStateFlag_Accept) {
		m_matchAcceptId = state->m_acceptId;
		m_matchEndOffset = m_parent->m_offset;
	}

	m_state = state;
}

//..............................................................................

} // namespace re
} // namespace axl
