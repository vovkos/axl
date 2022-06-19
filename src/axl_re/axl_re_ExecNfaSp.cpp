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
#include "axl_enc_Utf16s.h"
#include "axl_enc_Utf32s.h"

namespace axl {
namespace re {

//..............................................................................

ExecNfaSpBase::ExecNfaSpBase(StateImpl* parent):
	ExecNfaEngine(parent) {
	m_matchAcceptId = -1;
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
ExecNfaSpBase::copy(const ExecNfaSpBase* src) {
	ExecEngine::copy(src);
	m_matchAcceptId = src->m_matchAcceptId;
	m_matchPos = src->m_matchPos;
	m_capturePosArray = src->m_capturePosArray;
	m_consumingStateSetIdx = src->m_consumingStateSetIdx;
	m_nonConsumingStateSetIdx = src->m_nonConsumingStateSetIdx;
	m_consumingStateSetTable[m_consumingStateSetIdx] = src->m_consumingStateSetTable[m_consumingStateSetIdx];
	m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx] = src->m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx];
}

void
ExecNfaSpBase::initialize(
	const StateInit& init,
	const NfaState* state
) {
	ExecEngine::initialize(init);
	m_matchPos.m_offset = init.m_offset;
	m_matchAcceptId = -1;
	m_isEmpty = true;

	m_consumingStateSetTable[0].clear();
	m_consumingStateSetTable[1].clear();
	m_consumingStateSetIdx = 0;
	m_nonConsumingStateSetTable[0].clear();
	m_nonConsumingStateSetTable[1].clear();
	m_nonConsumingStateSetIdx = 0;
	m_capturePosArray.clear();

	addState(state);
}

void
ExecNfaSpBase::eof(bool isLastExecDataAvailable) {
	if (!isLastExecDataAvailable)
		resetLastExecData();

	advanceNonConsumingStates(m_prevCharFlags | Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary);
	finalize(true);
}

void
ExecNfaSpBase::addState(const NfaState* state) {
	if (!state->isConsuming())
		m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx].add(state);
	else {
		m_consumingStateSetTable[m_consumingStateSetIdx].add(state);
		m_isEmpty = false;
	}
}

void
ExecNfaSpBase::openCapture(size_t captureId) {
	AXL_RE_TRACE_CAPTURE("%p: open capture(%d)\n", m_offset, captureId);

	if (captureId >= m_capturePosArray.getCount())
		m_capturePosArray.setCount(captureId + 1);

	m_capturePosArray[captureId].m_offset = m_offset;
	m_capturePosArray[captureId].m_endOffset = m_offset;
}

void
ExecNfaSpBase::closeCapture(size_t captureId) {
	AXL_RE_TRACE_CAPTURE("%p: close capture(%d)\n", m_offset, captureId);

	ASSERT(captureId < m_capturePosArray.getCount());
	m_capturePosArray[captureId].m_endOffset = m_offset;
}

void
ExecNfaSpBase::advanceNonConsumingStates(uint32_t anchors) {
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

				m_matchPos.m_endOffset = m_offset;
				break;

			case NfaStateKind_Split:
				addState(state->m_nextState);
				addState(state->m_splitState);
				break;

			case NfaStateKind_OpenCapture:
				if (!(m_execFlags & ExecFlag_DisableCapture))
					openCapture(state->m_captureId);

				addState(state->m_nextState);
				break;

			case NfaStateKind_CloseCapture:
				if (!(m_execFlags & ExecFlag_DisableCapture))
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
ExecNfaSpBase::advanceConsumingStates(utf32_t c) {
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

void
ExecNfaSpBase::finalize(bool isEof) {
	ASSERT(m_parent->m_match.getOffset() == -1);

	if (m_matchAcceptId == -1) {
		m_execResult = ExecResult_NoMatch;
		return;
	}

	if (m_execFlags & ExecFlag_AnchorDataEnd) {
		if (!isEof)
			return; // can't verify until we see EOF

		if (m_matchPos.m_endOffset != m_lastExecEndOffset) {
			m_execResult = ExecResult_NoMatch;
			return;
		}
	}

	m_parent->createMatch(m_matchAcceptId, m_lastExecOffset, m_lastExecData, m_matchPos);
	m_execResult = ExecResult_Match;
}

//..............................................................................

template <typename Decoder>
class ExecNfaSp: public ExecImpl<
	ExecNfaSp<Decoder>,
	ExecNfaSpBase,
	Decoder
> {
public:
	ExecNfaSp(StateImpl* parent):
		ExecImpl<ExecNfaSp, ExecNfaSpBase, Decoder>(parent) {}

	void
	emitCp(
		const char* p,
		utf32_t c
	) {
		uint_t anchors = this->calcAnchorsUpdateCharFlags(c);
		this->advanceNonConsumingStates(anchors);

		if (this->m_isEmpty) {
			this->finalize(false);
			return;
		}

		this->m_offset = this->m_lastExecOffset + p - (char*)this->m_lastExecData;
		this->advanceConsumingStates(c);
	}
};

//..............................................................................

template <typename Decoder>
class ExecNfaSpFactory: public ExecEngineFactory {
public:
	virtual
	ExecEngine*
	createExecEngine(StateImpl* parent) {
		return AXL_MEM_NEW_ARGS(ExecNfaSp<Decoder>, (parent));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecNfaEngine*
createExecNfaSp(StateImpl* parent) {
	static ExecNfaSpFactory<enc::AsciiDecoder>     asciiFactory;
	static ExecNfaSpFactory<enc::Utf8Decoder>      utf8Factory;
	static ExecNfaSpFactory<enc::Utf16sDecoder>    utf16Factory;
	static ExecNfaSpFactory<enc::Utf16sDecoder_be> utf16Factory_be;
	static ExecNfaSpFactory<enc::Utf32sDecoder>    utf32Factory;
	static ExecNfaSpFactory<enc::Utf32sDecoder_be> utf32Factory_be;

	static ExecEngineFactory* factoryTable[enc::CharCodecKind__Count] = {
		&asciiFactory,
		&utf8Factory,
		&utf16Factory,
		&utf16Factory_be,
		&utf32Factory,
		&utf32Factory_be,
	};

	size_t i = parent->m_init.m_codecKind;
	if (i >= countof(factoryTable)) {
		ASSERT(false);
		i = enc::CharCodecKind_Ascii;
	}

	return (ExecNfaEngine*)factoryTable[i]->createExecEngine(parent);
}

//..............................................................................

} // namespace re
} // namespace axl
