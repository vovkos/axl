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
#include "axl_re_ExecDfa.h"
#include "axl_re_Dfa.h"
#include "axl_re_Regex.h"

namespace axl {
namespace re {

//..............................................................................

ExecDfa::ExecDfa(StateImpl* parent):
	ExecEngine(parent) {
	m_state = NULL;
	m_matchEndOffset = -1;
	m_matchAcceptId = -1;
}

void
ExecDfa::reset() {
	m_matchEndOffset = -1;
	m_matchAcceptId = -1;

	gotoState(
		(m_parent->m_execFlags & RegexExecFlag_AnchorDataBegin) ?
			m_parent->m_regex->getDfaMatchStartState() :
			m_parent->m_regex->getDfaSearchStartState()
	);
}

inline
void
ExecDfa::gotoState(const DfaState* state) {
	// build DFA on-the-fly

	if (!(state->m_flags & DfaStateFlag_Ready)) {
		DfaBuilder builder(
			(state->m_flags & DfaStateFlag_Reverse) ?
				&m_parent->m_regex->m_dfaReverseProgram :
				&m_parent->m_regex->m_dfaProgram
		);
		builder.buildTransitionMaps((DfaState*)state);
	}

	// process accept state

	if (state->m_flags & DfaStateFlag_Accept) {
		m_matchAcceptId = state->m_acceptId;
		m_matchEndOffset = m_parent->m_offset;
	}

	m_state = state;
}

bool
ExecDfa::exec(
	const void* p0,
	size_t size
) {
	char const* p = (char*)p0;
	char const* end = p + size;

	while (p < end) {
		uchar_t cplBuffer[DecodeBufferSize];
		utf32_t textBuffer[DecodeBufferSize];
		size_t takenSize;
		size_t length = m_parent->m_decoder.decode(cplBuffer, textBuffer, DecodeBufferSize, p, end - p, &takenSize);
		p += takenSize;

		for (size_t i = 0; i < length; i++) {
			utf32_t c = textBuffer[i];

			if (m_state->m_anchorMask) {
				uint_t anchors = m_parent->calcAnchorsUpdateCharFlags(c) & m_state->m_anchorMask;
				if (anchors) {
					const DfaState* anchorState = m_state->m_anchorTransitionMap.findValue(anchors, NULL);
					if (anchorState)
						gotoState(anchorState);
				}
			} else {
				m_parent->m_prevChar = c;
				m_parent->m_prevCharFlags = 0;
			}

			const DfaState* nextState = m_state->m_charTransitionMap.find(c);
			if (!nextState)
				return finalize(false);

			m_parent->m_offset += cplBuffer[i];
			gotoState(nextState);

			if (nextState->m_flags & DfaStateFlag_Dead)
				return finalize(false);
		}
	}

	return true;
}

bool
ExecDfa::eof() {
	uint_t anchors = Anchor_EndLine | Anchor_EndText;
	if (m_parent->m_prevCharFlags & StateImpl::CharFlag_Word)
		anchors |= Anchor_WordBoundary;
	else
		anchors |= Anchor_NotWordBoundary;

	anchors &= m_state->m_anchorMask;
	if (anchors) {
		ASSERT(m_state->m_anchorTransitionMap[anchors]);
		gotoState(m_state->m_anchorTransitionMap[anchors]);
	}

	return finalize(true);
}

bool
ExecDfa::finalize(bool isEof) {
	if (m_parent->m_matchAcceptId != -1) // already finalized
		return true;

	if (m_matchAcceptId == -1)
		return false;

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataEnd) {
		if (!isEof)
			return true; // can't verify until we see EOF

		if (m_matchEndOffset != m_parent->m_lastExecOffset + m_parent->m_lastExecSize)
			return false;
	}

	// roll back to the beginning



	m_parent->createMatch(m_matchAcceptId, MatchPos(0, m_matchEndOffset));
	return true;
}

//..............................................................................

} // namespace re
} // namespace axl
