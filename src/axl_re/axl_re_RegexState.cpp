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

#define _AXL_RE_TRACE_CAPTURE 0

#if (_AXL_RE_TRACE_CAPTURE)
#	define AXL_RE_TRACE_CAPTURE TRACE
#else
#	define AXL_RE_TRACE_CAPTURE (void)
#endif

//..............................................................................

RegexMatch::RegexMatch() {
	m_offset = 0;
	m_endOffset = 0;
	m_charCodec = NULL;
	m_p = NULL;
}

sl::StringRef
RegexMatch::cacheText() const {
	ASSERT(m_p && m_charCodec);

	m_text = m_charCodec == enc::getCharCodec(enc::CharCodecKind_Utf8) ?
		sl::StringRef(m_p, getSize()) :
		m_charCodec->decode_utf8(m_p, getSize());

	return m_text;
}

//..............................................................................

RegexStateImpl::Nfa::Nfa() {
	m_consumingStateSetIdx = 0;
	m_nonConsumingStateSetIdx = 0;
	m_lastAcceptStateId = -1;
	m_isEmpty = false;
}

void
RegexStateImpl::Nfa::reset(RegexStateImpl* parent) {
	m_consumingStateSetTable[0].clear();
	m_consumingStateSetTable[1].clear();
	m_nonConsumingStateSetTable[0].clear();
	m_nonConsumingStateSetTable[1].clear();
	m_consumingStateSetIdx = 0;
	addState(parent->m_regex->getNfaStartState());
}

bool
RegexStateImpl::Nfa::exec(
	RegexStateImpl* parent,
	const void* p0,
	size_t size
) {
	char const* p = (char*)p0;
	char const* end = p + size;

	while (p < end) {
		uchar_t cplBuffer[DecodeBufferSize];
		utf32_t textBuffer[DecodeBufferSize];
		size_t takenSize;
		size_t length = parent->m_decoder.decode(cplBuffer, textBuffer, DecodeBufferSize, p, end - p, &takenSize);
		p += takenSize;

		for (size_t i = 0; i < length; i++) {
			utf32_t c = textBuffer[i];
			uint_t anchors = parent->calcAnchors(c);

			advanceNonConsumingStates(parent, anchors);
			advanceConsumingStates(parent, c);

			if (m_isEmpty)
				return parent->finalize(false);

			parent->m_offset += cplBuffer[i];
		}
	}

	return true;
}

bool
RegexStateImpl::Nfa::eof(RegexStateImpl* parent) {
	uint_t anchors = Anchor_End;
	if (parent->m_prevCharFlags & CharFlag_AlphaNum)
		anchors |= Anchor_Word;

	advanceNonConsumingStates(parent, anchors);
	return parent->finalize(true);
}

void
RegexStateImpl::Nfa::addState(const NfaState* state) {
	if (state->isConsuming())
		m_consumingStateSetTable[m_consumingStateSetIdx].add(state);
	else
		m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx].add(state);

	m_isEmpty = false;
}

void
RegexStateImpl::Nfa::advanceNonConsumingStates(
	RegexStateImpl* parent,
	uint32_t anchors
) {
	while (!m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx].isEmpty()) {
		size_t srcSetIdx = m_nonConsumingStateSetIdx;
		size_t dstSetIdx = !srcSetIdx;

		m_nonConsumingStateSetTable[dstSetIdx].clear();
		m_nonConsumingStateSetIdx = dstSetIdx;

		size_t count = m_consumingStateSetTable[srcSetIdx].getCount();
		for (size_t i = 0; i < count; i++) {
			const NfaState* state = m_consumingStateSetTable[srcSetIdx][i];
			switch (state->m_stateKind) {
			case NfaStateKind_Accept:
				if (parent->m_offset > parent->m_match.m_endOffset || state->m_id < m_lastAcceptStateId) {
					m_lastAcceptStateId = state->m_id;
					parent->accept(state->m_acceptId);
				}

				break;

			case NfaStateKind_Split:
				addState(state->m_nextState);
				addState(state->m_splitState);
				break;

			case NfaStateKind_OpenCapture:
				if (!(parent->m_execFlags & RegexExecFlag_NoCapture))
					parent->openCapture(state->m_captureId);
				addState(state->m_nextState);
				break;

			case NfaStateKind_CloseCapture:
				if (!(parent->m_execFlags & RegexExecFlag_NoCapture))
					parent->closeCapture(state->m_captureId);
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
RegexStateImpl::Nfa::advanceConsumingStates(
	RegexStateImpl* parent,
	utf32_t c
) {
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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
RegexStateImpl::Dfa::reset(RegexStateImpl* parent) {
	gotoState(parent, parent->m_regex->getDfaStartState());
}

bool
RegexStateImpl::Dfa::exec(
	RegexStateImpl* parent,
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
		size_t length = parent->m_decoder.decode(cplBuffer, textBuffer, DecodeBufferSize, p, end - p, &takenSize);
		p += takenSize;

		for (size_t i = 0; i < length; i++) {
			utf32_t c = textBuffer[i];
			uint_t anchors = parent->calcAnchors(c) & m_state->m_anchorMask;
			if (anchors) {
				ASSERT(m_state->m_anchorTransitionMap[anchors]);
				gotoState(parent, m_state->m_anchorTransitionMap[anchors]);
			}

			const DfaState* state = m_state->m_charTransitionMap.find(c);
			if (!state)
				return parent->finalize(false);

			parent->m_offset += cplBuffer[i];
			gotoState(parent, state);

			if (state->m_flags & DfaStateFlag_Final)
				return parent->finalize(false);
		}
	}

	return true;
}

bool
RegexStateImpl::Dfa::eof(RegexStateImpl* parent) {
	uint_t anchors = Anchor_End;
	if (parent->m_prevCharFlags & CharFlag_AlphaNum)
		anchors |= Anchor_Word;

	anchors &= m_state->m_anchorMask;
	if (anchors) {
		ASSERT(m_state->m_anchorTransitionMap[anchors]);
		gotoState(parent, m_state->m_anchorTransitionMap[anchors]);
	}

	return parent->finalize(true);
}

void
RegexStateImpl::Dfa::gotoState(
	RegexStateImpl* parent,
	const DfaState* state
) {
	// build DFA on-the-fly

	if (!(state->m_flags & DfaStateFlag_CharTransitionMap)) {
		DfaBuilder builder(parent->m_regex);
		builder.buildCharTransitionMap((DfaState*)state);
	}

	// process capturing groups

	if (!(parent->m_execFlags & RegexExecFlag_NoCapture)) {
		size_t count = state->m_openCaptureIdSet.getCount();
		for (size_t i = 0; i < count; i++)
			parent->openCapture(state->m_openCaptureIdSet[i]);

		count = state->m_closeCaptureIdSet.getCount();
		for (size_t i = 0; i < count; i++)
			parent->closeCapture(state->m_closeCaptureIdSet[i]);
	}

	// process accept state

	if (state->m_flags & DfaStateFlag_Accept)
		parent->accept(state->m_acceptId);

	m_state = state;
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

void
RegexStateImpl::freeFsm() {
	if (!m_fsm)
		return;

	if (m_execFlags & RegexExecFlag_Nfa)
		AXL_MEM_DELETE(m_nfa);
	else
		AXL_MEM_DELETE(m_dfa);

	m_fsm = NULL;
}

void
RegexStateImpl::initialize() {
	m_regex = NULL;
	m_lastExecBuffer = NULL;
	m_lastExecOffset = 0;
	m_lastExecSize = 0;
	m_execFlags = 0;
	m_prevCharFlags = 0;
	m_offset = 0;
	m_matchAcceptId = -1;
	m_fsm = NULL;
}

void
RegexStateImpl::initialize(
	uint_t execFlags,
	enc::CharCodec* codec
) {
	freeFsm();

	m_regex = NULL;
	m_decoder.setup(codec);
	m_execFlags = execFlags;
	m_prevCharFlags = RegexStateImpl::CharFlag_NewLine;
}

void
RegexStateImpl::postInitialize(Regex* regex) {
	ASSERT(!m_regex && !m_fsm);

	m_regex = regex;

	if (!(m_execFlags & RegexExecFlag_Nfa)) {
		m_dfa = AXL_MEM_NEW(RegexStateImpl::Dfa);
	} else {
		size_t stateCount = regex->getNfaStateCount();
		m_nfa = AXL_MEM_NEW(RegexStateImpl::Nfa);
		m_nfa->m_consumingStateSetTable[0].reserve(stateCount);
		m_nfa->m_consumingStateSetTable[1].reserve(stateCount);
		m_nfa->m_nonConsumingStateSetTable[0].reserve(stateCount);
		m_nfa->m_nonConsumingStateSetTable[1].reserve(stateCount);
	}

	reset();
}

void
RegexStateImpl::reset(size_t offset) {
	m_offset = offset;
	m_matchAcceptId = -1;
	m_match.m_offset = offset;
	m_match.m_endOffset = -1;
	m_subMatchList.clear();
	m_subMatchArray.clear();
	m_capturePosArray.clear();

	if (m_execFlags & RegexExecFlag_Nfa)
		m_nfa->reset(this);
	else
		m_dfa->reset(this);
}

void
RegexStateImpl::copy(const RegexStateImpl& src) {
	freeFsm();

	m_regex = src.m_regex;
	m_decoder = src.m_decoder;
	m_execFlags = src.m_execFlags;
	m_prevCharFlags = src.m_prevCharFlags;
	m_offset = src.m_offset;
	m_matchAcceptId = src.m_matchAcceptId;

	if (src.m_fsm)
		if (m_execFlags & RegexExecFlag_Nfa)
			m_nfa = AXL_MEM_NEW_ARGS(Nfa, (*src.m_nfa));
		else
			m_dfa = AXL_MEM_NEW_ARGS(Dfa, (*src.m_dfa));

	m_match = src.m_match;
	m_subMatchList.clear();

	if (src.m_subMatchArray.isEmpty()) {
		m_subMatchArray.clear();
		return;
	}

	ASSERT(src.m_subMatchArray[0] == &src.m_match);
	m_subMatchArray.setCount(src.m_subMatchArray.getCount());
	m_subMatchArray[0] = &m_match;

	sl::ConstBoxIterator<RegexMatch> it = src.m_subMatchList.getHead();
	for (size_t i = 1; it; it++, i++) {
		RegexMatch* match = m_subMatchList.insertTail().p();
		*match = *it;
		m_subMatchArray[i] = match;
	}
}

uint32_t
RegexStateImpl::calcAnchors(utf32_t c) {
	uint_t anchors = 0;
	uint_t charFlags = 0;

	if (m_prevCharFlags & CharFlag_NewLine)
		anchors |= Anchor_Begin;

	if (c == '\r') {
		anchors |= Anchor_End;
	} else if (c == '\n') {
		anchors |= Anchor_End;
		charFlags |= CharFlag_NewLine;
	}

	if (enc::utfIsLetterOrDigit(c)) {
		if (!(m_prevCharFlags & CharFlag_AlphaNum))
			anchors |= Anchor_Word;
		charFlags |= CharFlag_AlphaNum;
	} else if (m_prevCharFlags & CharFlag_AlphaNum) {
		anchors |= Anchor_Word;
	}

	m_prevCharFlags = charFlags;
	return anchors;
}

void
RegexStateImpl::openCapture(size_t captureId) {
	ASSERT(!(m_execFlags & RegexExecFlag_NoCapture));
	AXL_RE_TRACE_CAPTURE("%p: open capture(%d)\n", m_offset, captureId);

	if (captureId >= m_capturePosArray.getCount())
		m_capturePosArray.setCount(captureId + 1);

	m_capturePosArray[captureId].m_offset = m_offset;
	m_capturePosArray[captureId].m_endOffset = m_offset;
}

void
RegexStateImpl::closeCapture(size_t captureId) {
	ASSERT(!(m_execFlags & RegexExecFlag_NoCapture));
	AXL_RE_TRACE_CAPTURE("%p: close capture(%d)\n", m_offset, captureId);

	ASSERT(captureId < m_capturePosArray.getCount());
	m_capturePosArray[captureId].m_endOffset = m_offset;
}

void
RegexStateImpl::accept(size_t acceptId) {
	AXL_RE_TRACE_CAPTURE("%p: accept(%d)\n", m_offset, acceptId);
	m_matchAcceptId = acceptId;
	m_match.m_endOffset = m_offset;
}

bool
RegexStateImpl::finalize(bool isEof) {
	if (m_matchAcceptId == -1)
		return false;

	if (!m_subMatchArray.isEmpty()) { // already finalized
		ASSERT(m_subMatchArray.getFront() == &m_match);
		return true;
	}

	if (m_execFlags & RegexExecFlag_AnchorDataEnd) {
		if (!isEof)
			return m_lastExecOffset + m_lastExecSize == m_match.m_endOffset + m_decoder.getAccumulatorCount(); // account for the incomplete codepoint

		if (m_lastExecOffset + m_lastExecSize != m_match.m_endOffset)
			return false;
	}

	if (!(m_execFlags & RegexExecFlag_Stream)) {
		m_match.m_charCodec = m_decoder.getCharCodec();
		m_match.m_p = (char*)m_lastExecBuffer + m_match.m_offset - m_lastExecOffset;
	}

	if (m_execFlags & RegexExecFlag_NoCapture)
		m_subMatchArray.copy(&m_match);
	else if (m_regex->getRegexKind() != RegexKind_Switch)
		createSubMatches(0, m_regex->getCaptureCount());
	else
		createSubMatches(
			m_regex->getSwitchCaseBaseCaptureId(m_matchAcceptId),
			m_regex->getSwitchCaseCaptureCount(m_matchAcceptId)
		);

	return true;
}

void
RegexStateImpl::createSubMatches(
	size_t baseCaptureId,
	size_t captureCount
) {
	m_subMatchArray.setCountZeroConstruct(captureCount + 1);
	m_subMatchArray[0] = &m_match;

	size_t count = m_capturePosArray.getCount();
	if (count > captureCount)
		count = captureCount;

	for (size_t i = baseCaptureId; i < count; i++) {
		const CapturePos& pos = m_capturePosArray[i];
		if (pos.m_endOffset == -1)
			continue;

		ASSERT(pos.m_offset != -1 && pos.m_offset <= pos.m_endOffset);

		RegexMatch* match = m_subMatchList.insertTail().p();
		match->m_offset = pos.m_offset;
		match->m_endOffset = pos.m_endOffset;

		if (!(m_execFlags & RegexExecFlag_Stream)) {
			match->m_charCodec = m_decoder.getCharCodec();
			match->m_p = (char*)m_lastExecBuffer + pos.m_offset - m_lastExecOffset;
		}

		m_subMatchArray[i - baseCaptureId + 1] = match;
	}
}

bool
RegexStateImpl::exec(
	const void* p0,
	size_t size
) {
	m_lastExecBuffer = p0;
	m_lastExecOffset = m_offset;
	m_lastExecSize = size;

	bool result;

	if (m_execFlags & RegexExecFlag_AnchorDataBegin)
		result = (m_execFlags & RegexExecFlag_Nfa) ?
			m_nfa->exec(this, p0, size) :
			m_dfa->exec(this, p0, size);
	else {
		size_t offset = m_offset;
		const char* p = (char*)p0;
		const char* end = p + size;

		if (m_execFlags & RegexExecFlag_Nfa)
			do {
				result = m_nfa->exec(this, p, end - p);
				if (result)
					break;

				m_offset = m_match.m_offset = ++offset;
				m_capturePosArray.clear();
				m_nfa->reset(this);
			} while (++p < end);
		else
			do {
				result = m_dfa->exec(this, p, end - p);
				if (result)
					break;

				m_offset = m_match.m_offset = ++offset;
				m_capturePosArray.clear();
				m_dfa->reset(this);
			} while (++p < end);
	}

	return result;
}

//..............................................................................

void
RegexState::initialize(
	uint_t execFlags,
	enc::CharCodec* codec
) {
	if (!m_p)
		m_p.createBuffer();
	else
		ASSERT(m_p.isExclusive());

	m_p->initialize(execFlags, codec);
}

void
RegexState::initialize(Regex* regex) {
	ASSERT(!m_p);
	m_p.createBuffer();
	m_p->initialize(0, enc::getCharCodec(enc::CharCodecKind_Utf8));
	m_p->postInitialize(regex);
}

//..............................................................................

} // namespace re
} // namespace axl
