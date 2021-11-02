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
#include "axl_re_RegexState.h"
#include "axl_re_Regex.h"
#include "axl_re_Nfa.h"

namespace axl {
namespace re {

//..............................................................................

RegexState::RegexState() {
	m_regex = NULL;
	m_codec = NULL;
	m_consumingStateSetIdx = 0;
	m_prevCharFlags = false;
	m_lastAcceptState = NULL;
	m_flags = 0;
	m_matchLengthLimit = 0;
	m_offset = 0;
	m_matchOffset = 0;
	m_lastAcceptMatchLength = 0;
	m_consumedLength = 0;
	m_match = NULL;
}

void
RegexState::reset() {
	m_regex = NULL;
	m_codec = NULL;
	m_consumingStateSetIdx = 0;
	m_prevCharFlags = 0;
	m_lastAcceptState = NULL;
	m_flags = 0;
	m_matchLengthLimit = 0;
	m_offset = 0;
	m_matchOffset = 0;
	m_lastAcceptMatchLength = 0;
	m_consumedLength = 0;
	m_match = NULL;
	m_matchList.clear();
	m_subMatchArray.clear();
}

void
RegexState::initialize(
	Regex* regex,
	enc::CharCodec* codec
) {
	reset();
	m_regex = regex;
	m_codec = codec;
	size_t stateCount = regex->getNfaStateCount();
	m_consumingStateSetTable[0].setBitCount(stateCount);
	m_consumingStateSetTable[0].clear();
	m_consumingStateSetTable[1].setBitCount(stateCount);
	m_consumingStateSetTable[1].clear();
	m_nonConsumingStateSet.setBitCount(stateCount);
	m_nonConsumingStateSet.clear();
	m_offset = 0;
	m_matchOffset = 0;
	addState(regex->getNfaState(0));
}

bool
RegexState::exec(
	const void* p0,
	size_t size
) {
	char const* p = (char*)p0;
	char const* end = p + size;

	bool result = false;

	while (p < end) {
		uchar_t cplBuffer[DecodeBufferSize];
		utf32_t textBuffer[DecodeBufferSize];
		size_t takenSize;
		size_t length = m_codec->decode_utf32(cplBuffer, textBuffer, DecodeBufferSize, p, end - p, &takenSize);
		p += takenSize;

		for (size_t i = 0; i < length; i++) {
			utf32_t c = textBuffer[i];
			uint_t anchors = 0;

			if (!m_offset || (m_prevCharFlags & PrevCharFlag_NewLine))
				anchors |= FsmAnchor_Begin;

			if (c == '\r') {
				anchors |= FsmAnchor_End;
			} else if (c == '\n') {
				anchors |= FsmAnchor_End;
				m_prevCharFlags |= PrevCharFlag_NewLine;
			}

			if (enc::utfIsLetterOrDigit(c)) {
				if (!(m_prevCharFlags & PrevCharFlag_AlphaNum)) {
					anchors |= FsmAnchor_Word;
					m_prevCharFlags |= PrevCharFlag_AlphaNum;
				}
			} else {
				if (m_prevCharFlags & PrevCharFlag_AlphaNum) {
					anchors |= FsmAnchor_Word;
					m_prevCharFlags &= ~PrevCharFlag_AlphaNum;
				}
			}

			advanceNonConsumingStates(anchors);
			m_nonConsumingStateSet.clear(); // nc-states that were not advanced, die out

			size_t srcSetIdx = m_consumingStateSetIdx;
			size_t dstSetIdx = !srcSetIdx;

			m_consumingStateSetTable[dstSetIdx].clear();
			m_consumingStateSetIdx = dstSetIdx;

			for (size_t j = 0;; j++) {
				j = m_consumingStateSetTable[srcSetIdx].findBit(j);
				if (j == -1)
					break;

				const NfaState* state = m_regex->getNfaState(j);
				switch (state->m_matchCondition.m_matchKind) {
				case FsmMatchKind_Char:
					if (state->m_matchCondition.m_char == c)
						addState(state->m_nextState);
					break;

				case FsmMatchKind_CharSet:
					if (state->m_matchCondition.m_charSet->isSet(c))
						addState(state->m_nextState);
					break;

				case FsmMatchKind_AnyChar:
					addState(state->m_nextState);
					break;

				default:
					ASSERT(false);
				}
			}

			m_offset += cplBuffer[i];
		}
	}

	return result;
}

bool
RegexState::eof() {
	uint_t anchors = FsmAnchor_End;
	if (m_prevCharFlags & PrevCharFlag_AlphaNum)
		anchors |= FsmAnchor_Word;

	advanceNonConsumingStates(anchors);
	return m_lastAcceptState != NULL;
}

void
RegexState::addState(const NfaState* state) {
	if (state->m_matchCondition.isConsuming())
		m_consumingStateSetTable[m_consumingStateSetIdx].setBit(state->m_id);
	else
		m_nonConsumingStateSet.setBit(state->m_id);
}

void
RegexState::advanceNonConsumingStates(uint32_t anchors) {
	for (;;) {
		size_t i = m_nonConsumingStateSet.findBit(0);
		if (i == -1)
			break;

		m_nonConsumingStateSet.setBit(i, false);
		const NfaState* state = m_regex->getNfaState(i);
		switch (state->m_stateKind) {
		case NfaStateKind_Accept:
			printf("%p: accept\n", m_offset);
			m_lastAcceptState = state;
			m_lastAcceptMatchLength = m_offset - m_matchOffset;
			break;

		case NfaStateKind_Split:
			addState(state->m_nextState);
			addState(state->m_splitState);
			break;

		case NfaStateKind_OpenCapture:
			printf("%p: open capture(%d)\n", m_offset, state->m_captureId);
			addState(state->m_nextState);
			break;

		case NfaStateKind_CloseCapture:
			printf("%p: close capture(%d)\n", m_offset, state->m_captureId);
			addState(state->m_nextState);
			break;

		case NfaStateKind_Match:
			ASSERT(state->m_matchCondition.m_matchKind == FsmMatchKind_Anchor);
			if (anchors & state->m_matchCondition.m_anchor)
				addState(state->m_nextState);
			break;

		default:
			ASSERT(false);
		}
	}
}

//..............................................................................

} // namespace re
} // namespace axl
