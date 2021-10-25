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
	m_isPrevCharAlphanumeric = false;
	m_lastAcceptState = NULL;
	m_flags = 0;
	m_offset = 0;
	m_matchLengthLimit = 0;
	m_matchOffset = 0;
	m_replayBufferOffset = 0;
	m_replayLength = 0;
	m_lastAcceptMatchLength = 0;
	m_consumedLength = 0;
	m_match = NULL;
}

void
RegexState::reset() {
	m_regex = NULL;
	m_codec = NULL;
	m_consumingStateSetIdx = 0;
	m_lastAcceptState = NULL;
	m_isPrevCharAlphanumeric = false;
	m_offset = 0;
}

void
RegexState::initialize(
	Regex* regex,
	enc::CharCodec* codec
) {
	reset();
	m_regex = regex;
	m_codec = codec;
	size_t stateCount = regex->getStateCount();
	m_consumingStateSetTable[0].setBitCount(stateCount);
	m_consumingStateSetTable[0].clear();
	m_consumingStateSetTable[1].setBitCount(stateCount);
	m_consumingStateSetTable[1].clear();
	m_nonConsumingStateSet.setBitCount(stateCount);
	m_nonConsumingStateSet.clear();
	m_offset = 0;
	m_matchOffset = 0;
	addState(regex->getState(0));
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
			bool isAlphanumeric = enc::utfIsLetterOrDigit(c);

			uint_t anchors = !m_offset ? Anchor_Begin : 0;
			if (isAlphanumeric != m_isPrevCharAlphanumeric)
				anchors |= Anchor_Word;

			m_isPrevCharAlphanumeric = isAlphanumeric;

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

				NfaState* state = m_regex->getState(j);
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
	uint_t anchors = Anchor_End;
	if (m_isPrevCharAlphanumeric)
		anchors |= Anchor_Word;

	advanceNonConsumingStates(anchors);
	return m_lastAcceptState != NULL;
}

void
RegexState::addState(NfaState* state) {
	if (state->m_stateKind >= NfaStateKind_FirstConsuming)
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
		NfaState* state = m_regex->getState(i);
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

		case NfaStateKind_MatchAnchor:
			if (anchors & state->m_anchor)
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
