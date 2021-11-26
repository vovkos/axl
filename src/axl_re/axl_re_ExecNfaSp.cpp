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
#include "axl_re_ExecNfaSp.h"
#include "axl_re_Regex.h"

namespace axl {
namespace re {

//..............................................................................

ExecNfaSp::ExecNfaSp(StateImpl* parent):
	ExecEngine(parent) {
	m_matchAcceptId = -1;
	m_matchEndOffset = -1;
	m_isEmpty = true;

	size_t stateCount = m_parent->m_regex->getNfaStateCount();
	m_consumingStateSetTable[0].reserve(stateCount);
	m_consumingStateSetTable[1].reserve(stateCount);
	m_consumingStateSetIdx = 0;
	m_nonConsumingStateSetTable[0].reserve(stateCount);
	m_nonConsumingStateSetTable[1].reserve(stateCount);
	m_nonConsumingStateSetIdx = 0;
}

void
ExecNfaSp::reset() {
	m_matchAcceptId = -1;
	m_matchEndOffset = -1;
	m_isEmpty = true;

	m_consumingStateSetTable[0].clear();
	m_consumingStateSetTable[1].clear();
	m_consumingStateSetIdx = 0;
	m_nonConsumingStateSetTable[0].clear();
	m_nonConsumingStateSetTable[1].clear();
	m_nonConsumingStateSetIdx = 0;
	m_capturePosArray.clear();

	addState(
		(m_parent->m_execFlags & RegexExecFlag_AnchorDataBegin) ?
			m_parent->m_regex->getNfaMatchStartState() :
			m_parent->m_regex->getNfaSearchStartState()
	);
}

bool
ExecNfaSp::exec(
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
			uint_t anchors = m_parent->calcAnchorsUpdateCharFlags(c);

			advanceNonConsumingStates(anchors);
			advanceConsumingStates(c);

			if (m_isEmpty)
				return finalize(false);

			m_parent->m_offset += cplBuffer[i];
		}
	}

	return true;
}

bool
ExecNfaSp::eof() {
	uint_t anchors = Anchor_EndLine | Anchor_EndText;
	if (m_parent->m_prevCharFlags & StateImpl::CharFlag_Word)
		anchors |= Anchor_WordBoundary;
	else
		anchors |= Anchor_NotWordBoundary;

	advanceNonConsumingStates(anchors);
	return finalize(true);
}

void
ExecNfaSp::addState(const NfaState* state) {
	if (state->isConsuming())
		m_consumingStateSetTable[m_consumingStateSetIdx].add(state);
	else
		m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx].add(state);

	m_isEmpty = false;
}

void
ExecNfaSp::openCapture(size_t captureId) {
	ASSERT(!(m_parent->m_execFlags & RegexExecFlag_DisableCapture));

	size_t offset = m_parent->m_offset;
	AXL_RE_TRACE_CAPTURE("%p: open capture(%d)\n", offset, captureId);

	if (captureId >= m_capturePosArray.getCount())
		m_capturePosArray.setCount(captureId + 1);

	m_capturePosArray[captureId].m_offset = offset;
	m_capturePosArray[captureId].m_endOffset = offset;
}

void
ExecNfaSp::closeCapture(size_t captureId) {
	ASSERT(!(m_parent->m_execFlags & RegexExecFlag_DisableCapture));

	size_t offset = m_parent->m_offset;
	AXL_RE_TRACE_CAPTURE("%p: close capture(%d)\n", offset, captureId);

	ASSERT(captureId < m_capturePosArray.getCount());
	m_capturePosArray[captureId].m_endOffset = offset;
}

void
ExecNfaSp::advanceNonConsumingStates(uint32_t anchors) {
	while (!m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx].isEmpty()) {
		size_t srcSetIdx = m_nonConsumingStateSetIdx;
		size_t dstSetIdx = !srcSetIdx;

		m_nonConsumingStateSetTable[dstSetIdx].clear();
		m_nonConsumingStateSetIdx = dstSetIdx;

		size_t count = m_nonConsumingStateSetTable[srcSetIdx].getCount();
		for (size_t i = 0; i < count; i++) {
			const NfaState* state = m_nonConsumingStateSetTable[srcSetIdx][i];
			switch (state->m_stateKind) {
			case NfaStateKind_Accept:
				if (state->m_acceptId < m_matchAcceptId)
					m_matchAcceptId = state->m_acceptId;

				m_matchEndOffset = m_parent->m_offset;
				break;

			case NfaStateKind_Split:
				addState(state->m_nextState);
				addState(state->m_splitState);
				break;

			case NfaStateKind_OpenCapture:
				if (!(m_parent->m_execFlags & RegexExecFlag_DisableCapture))
					openCapture(state->m_captureId);

				addState(state->m_nextState);
				break;

			case NfaStateKind_CloseCapture:
				if (!(m_parent->m_execFlags & RegexExecFlag_DisableCapture))
					closeCapture(state->m_captureId);

				addState(state->m_nextState);
				break;

			case NfaStateKind_MatchAnchor:
				if (anchors & state->m_anchor)
					addState(state->m_nextState);

				break;

			default:
				ASSERT(false); // the rest of states are consuming
			}
		}
	}
}

void
ExecNfaSp::advanceConsumingStates(utf32_t c) {
	size_t srcSetIdx = m_consumingStateSetIdx;
	size_t dstSetIdx = !srcSetIdx;

	m_consumingStateSetTable[dstSetIdx].clear();
	m_consumingStateSetIdx = dstSetIdx;
	m_isEmpty = true;

	size_t count = m_consumingStateSetTable[srcSetIdx].getCount();
	for (size_t i = 0; i < count; i++) {
		const NfaState* state = m_consumingStateSetTable[srcSetIdx][i];
		switch (state->m_stateKind) {
		case NfaStateKind_MatchChar:
			if (state->m_char == c)
				addState(state->m_nextState);

			break;

		case NfaStateKind_MatchCharSet:
			if (state->m_charSet->isSet(c))
				addState(state->m_nextState);

			break;

		case NfaStateKind_MatchAnyChar:
			addState(state->m_nextState);
			break;

		default:
			ASSERT(false); // the rest of states are non-consuming
		}
	}
}

bool
ExecNfaSp::finalize(bool isEof) {
	if (m_matchAcceptId == -1)
		return false;

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataEnd) {
		if (!isEof)
			return true; // can't verify until we see EOF

		if (m_matchEndOffset != m_parent->m_lastExecOffset + m_parent->m_lastExecSize)
			return false;
	}

	m_parent->createMatch(m_matchAcceptId, MatchPos(0, m_matchEndOffset));
	return true;
}

//..............................................................................

} // namespace re
} // namespace axl
