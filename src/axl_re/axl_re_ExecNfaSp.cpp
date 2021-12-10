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
	ExecEngine(parent) {
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
	m_capturePosArray = src->m_capturePosArray;
	m_consumingStateSetIdx = src->m_consumingStateSetIdx;
	m_nonConsumingStateSetIdx = src->m_nonConsumingStateSetIdx;
	m_consumingStateSetTable[m_consumingStateSetIdx] = src->m_consumingStateSetTable[m_consumingStateSetIdx];
	m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx] = src->m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx];
}

void
ExecNfaSpBase::reset(size_t offset) {
	ExecEngine::reset(offset);
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

	advanceNonConsumingStates(Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary);
}

void
ExecNfaSpBase::addState(const NfaState* state) {
	if (state->isConsuming())
		m_consumingStateSetTable[m_consumingStateSetIdx].add(state);
	else
		m_nonConsumingStateSetTable[m_nonConsumingStateSetIdx].add(state);

	m_isEmpty = false;
}

void
ExecNfaSpBase::openCapture(size_t captureId) {
	ASSERT(!(m_parent->m_execFlags & RegexExecFlag_DisableCapture));

	size_t offset = m_parent->m_offset;
	AXL_RE_TRACE_CAPTURE("%p: open capture(%d)\n", offset, captureId);

	if (captureId >= m_capturePosArray.getCount())
		m_capturePosArray.setCount(captureId + 1);

	m_capturePosArray[captureId].m_offset = offset;
	m_capturePosArray[captureId].m_endOffset = offset;
}

void
ExecNfaSpBase::closeCapture(size_t captureId) {
	ASSERT(!(m_parent->m_execFlags & RegexExecFlag_DisableCapture));

	size_t offset = m_parent->m_offset;
	AXL_RE_TRACE_CAPTURE("%p: close capture(%d)\n", offset, captureId);

	ASSERT(captureId < m_capturePosArray.getCount());
	m_capturePosArray[captureId].m_endOffset = offset;
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

bool
ExecNfaSpBase::finalize(bool isEof) {
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

	m_parent->createMatch(m_matchAcceptId, MatchPos(0, m_matchEndOffset));
	return true;
}

//..............................................................................

template <typename Encoding>
class ExecNfaSp: public ExecNfaSpBase {
public:
	ExecNfaSp(StateImpl* parent):
		ExecNfaSpBase(parent) {}

	// ExecEngine

	virtual
	ExecEngine*
	clone(StateImpl* parent) {
		ExecNfaSp* exec = AXL_MEM_NEW_ARGS(ExecNfaSp, (parent));
		exec->copy(this);
		return exec;
	}

	virtual
	void
	exec(
		const void* p,
		size_t size
	) {
		Encoding::Decoder::decode(&m_decoderState, *this, (char*)p, (char*)p + size);
	}

	// DecodeEmitter

	bool
	canEmit() const {
		return m_execResult == ExecResult_Undefined;
	}

	void
	emitReplacement(const char* p) {
		emitCodePoint(p, enc::StdChar_Replacement);
	}

	void
	emitCodePoint(
		const char* p,
		utf32_t c
	) {
		uint_t anchors = m_parent->calcAnchorsUpdateCharFlags(c);
		advanceNonConsumingStates(anchors);
		advanceConsumingStates(c);

		if (m_isEmpty) {
			m_execResult = (ExecResult)finalize(false);
			return;
		}

		m_parent->m_offset = m_parent->m_lastExecOffset + p - (char*)m_parent->m_lastExecBuffer;
	}
};

//..............................................................................

ExecEngine*
createExecNfaSp(
	StateImpl* parent,
	enc::CharCodecKind codecKind
) {
	switch (codecKind) {
	case enc::CharCodecKind_Ascii:
		return AXL_MEM_NEW_ARGS(ExecNfaSp<enc::Ascii>, (parent));
	case enc::CharCodecKind_Utf8:
		return AXL_MEM_NEW_ARGS(ExecNfaSp<enc::Utf8>, (parent));
	case enc::CharCodecKind_Utf16:
		return AXL_MEM_NEW_ARGS(ExecNfaSp<enc::Utf16s>, (parent));
	case enc::CharCodecKind_Utf16_be:
		return AXL_MEM_NEW_ARGS(ExecNfaSp<enc::Utf16s_be>, (parent));
	case enc::CharCodecKind_Utf32:
		return AXL_MEM_NEW_ARGS(ExecNfaSp<enc::Utf32s>, (parent));
	case enc::CharCodecKind_Utf32_be:
		return AXL_MEM_NEW_ARGS(ExecNfaSp<enc::Utf32s_be>, (parent));
	default:
		ASSERT(false);
		return AXL_MEM_NEW_ARGS(ExecNfaSp<enc::Ascii>, (parent));
	}

	return NULL;
}

//..............................................................................

} // namespace re
} // namespace axl
