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
#include "axl_re_ExecNfaVm.h"
#include "axl_re_Regex.h"
#include "axl_enc_Utf16s.h"
#include "axl_enc_Utf32s.h"

namespace axl {
namespace re {

//..............................................................................

ExecDfaBase::ExecDfaBase(StateImpl* parent):
	ExecEngine(parent) {
	m_flags = 0;
	m_dfaState = NULL;
	m_p = NULL;
	m_matchEnd = NULL;
	m_baseOffset = 0;
	m_matchAcceptId = -1;
	m_matchEndOffset = -1;
	m_savedMatchEndOffset = -1;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = -1;
#endif
}

void
ExecDfaBase::copy(const ExecDfaBase* src) {
	ExecEngine::copy(src);
	m_flags = src->m_flags;
	m_dfaState = src->m_dfaState;
	m_matchEnd = src->m_matchEnd;
	m_matchAcceptId = src->m_matchAcceptId;
	m_matchEndOffset = src->m_matchEndOffset;
	m_savedMatchEndOffset = src->m_savedMatchEndOffset;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = src->m_savedMatchAcceptId;
#endif
}

void
ExecDfaBase::reset(size_t offset) {
	ExecEngine::reset(offset);
	m_flags = 0;
	m_matchEnd = NULL;
	m_baseOffset = offset;
	m_matchAcceptId = -1;
	m_matchEndOffset = -1;
	m_savedMatchEndOffset = -1;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = -1;
#endif

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataBegin) {
		gotoDfaState(offset, m_parent->m_regex->getDfaMatchStartState());
	} else {
		gotoDfaState(offset, m_parent->m_regex->getDfaSearchStartState());
	}

	processBoundary(offset, Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary);
}

inline
void
ExecDfaBase::gotoDfaStateImpl(const DfaState* state) {
	if (!(state->m_flags & DfaStateFlag_Ready)) {
		DfaBuilder builder(
			(state->m_flags & DfaStateFlag_Reverse) ?
				&m_parent->m_regex->m_dfaReverseProgram :
				&m_parent->m_regex->m_dfaProgram
		);
		builder.buildTransitionMaps((DfaState*)state);
	}

	m_dfaState = state;
}

inline
void
ExecDfaBase::gotoDfaState(
	const void* p,
	const DfaState* state
) {
	if (state->m_flags & DfaStateFlag_Accept) {
		m_matchAcceptId = state->m_acceptId;
		m_matchEnd = p;
	}

	gotoDfaStateImpl(state);
}

inline
void
ExecDfaBase::gotoDfaState(
	size_t offset,
	const DfaState* state
) {
	if (state->m_flags & DfaStateFlag_Accept) {
		m_matchAcceptId = state->m_acceptId;
		m_matchEndOffset = offset;
		m_matchEnd = NULL;
	}

	gotoDfaStateImpl(state);
}

inline
void
ExecDfaBase::processBoundary(
	size_t offset,
	uint_t anchors
) {
	anchors &= m_dfaState->m_anchorMask;
	if (anchors) {
		const DfaState* anchorState = m_dfaState->m_anchorTransitionMap.findValue(anchors, NULL);
		if (anchorState)
			gotoDfaState(offset, anchorState);
	}
}

bool
ExecDfaBase::finalize(bool isEof) {
	if (m_parent->m_matchAcceptId != -1) // already finalized
		return true;

	if (m_matchAcceptId == -1)
		return false;

	size_t matchEndOffset = m_matchEnd ?
		m_lastExecOffset + (char*)m_matchEnd - (char*)m_lastExecData :
		m_matchEndOffset;

	if (m_flags & Flag_Backward) {
		ASSERT(m_matchAcceptId == m_savedMatchAcceptId);
		createMatch(MatchPos(matchEndOffset + 1, m_savedMatchEndOffset));
		return true;
	}

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataEnd) {
		if (!isEof)
			return true; // can't verify until we see EOF

		if (matchEndOffset != m_lastExecEndOffset)
			return false;
	}

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataBegin) {
		createMatch(MatchPos(m_baseOffset, matchEndOffset));
		return true;
	}

	// reset the engine before going backwards (but keep the pre-calculated m_prevCharFlags)

#if (_AXL_DEBUG)
	m_savedMatchAcceptId = m_matchAcceptId;
#endif
	m_savedMatchEndOffset = matchEndOffset;
	m_flags = Flag_Backward;
	m_decoderState = 0;
	m_matchAcceptId = -1;
	m_matchEndOffset = -1;
	m_matchEnd = NULL;

	gotoDfaState(matchEndOffset, m_parent->m_regex->getDfaReverseMatchStartState());
	if (isEof && matchEndOffset == m_lastExecEndOffset)
		processBoundary(matchEndOffset, Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary);

	exec(m_lastExecData, matchEndOffset);
	size_t offset = (char*)m_p - (char*)m_lastExecData;
	ASSERT(m_execResult != ExecResult_False);

	if (offset == m_baseOffset - 1)
		processBoundary(offset, Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary);

	return finalize(true);
}

bool
ExecDfaBase::createMatch(const MatchPos& pos) {
	if (!pos.isInside(m_lastExecOffset, m_lastExecEndOffset)) {
		// match is scattered across multiple blocks; don't run the NFA
		m_parent->createMatch(m_matchAcceptId, m_lastExecOffset, m_lastExecData, pos);
		return true;
	}

	// TODO: use NFA-SP if possible

	const NfaState* nfaState = m_parent->m_regex->getRegexKind() == RegexKind_Switch ?
		m_parent->m_regex->getSwitchCaseNfaMatchStartState(m_matchAcceptId) :
		m_parent->m_regex->getNfaMatchStartState();

	const void* p = (char*)m_lastExecData - m_lastExecOffset + pos.m_offset;
	size_t length = pos.m_endOffset - pos.m_offset;

	ExecNfaEngine* engine = createExecNfaVm(m_parent);
	engine->reset(pos.m_offset, nfaState);
	engine->preExec(p, length);
	engine->exec(p, length);
	bool result = engine->eof();
	AXL_MEM_DELETE(engine);

	return result;
}

//..............................................................................

template <typename Encoding>
class ExecDfa: public ExecDfaBase {
public:
	ExecDfa(StateImpl* parent):
		ExecDfaBase(parent) {}

	// ExecEngine

	virtual
	ExecEngine*
	clone(StateImpl* parent) {
		ExecDfa* exec = AXL_MEM_NEW_ARGS(ExecDfa, (parent));
		exec->copy(this);
		return exec;
	}

	virtual
	void
	exec(
		const void* p,
		size_t size
	) {
		if (m_flags & Flag_Backward) {
			const char* end = (char*)p - 1;
			m_p = end + size;
			p = Encoding::ReverseDecoder::decode(&m_decoderState, *this, (char*)m_p, end);
		} else {
			const char* end = (char*)p + size;
			m_p = p;
			p = Encoding::Decoder::decode(&m_decoderState, *this, (char*)p, end);
		}

		m_offset += (char*)m_p - (char*)p;
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
		if (m_dfaState->m_anchorMask) {
			uint_t anchors = (m_flags & Flag_Backward) ?
				calcReverseAnchorsUpdateCharFlags(c) :
				calcAnchorsUpdateCharFlags(c);

			anchors &= m_dfaState->m_anchorMask;
			if (anchors) {
				const DfaState* anchorState = m_dfaState->m_anchorTransitionMap.findValue(anchors, NULL);
				if (anchorState)
					gotoDfaState(m_p, anchorState);
			}
		} else {
			m_prevChar = c;
			m_prevCharFlags = 0;
		}

		const DfaState* nextState = m_dfaState->m_charTransitionMap.find(c);
		if (!nextState) {
			m_execResult = (ExecResult)finalize(false);
			return;
		}

		gotoDfaState(p, nextState);

		if (nextState->m_flags & DfaStateFlag_Dead)
			m_execResult = (ExecResult)finalize(false);

		m_p = p;
	}
};

//..............................................................................

ExecEngine*
createExecDfa(StateImpl* parent) {
	switch (parent->m_codecKind) {
	case enc::CharCodecKind_Ascii:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Ascii>, (parent));
	case enc::CharCodecKind_Utf8:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf8>, (parent));
	case enc::CharCodecKind_Utf16:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf16s>, (parent));
	case enc::CharCodecKind_Utf16_be:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf16s_be>, (parent));
	case enc::CharCodecKind_Utf32:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf32s>, (parent));
	case enc::CharCodecKind_Utf32_be:
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Utf32s_be>, (parent));
	default:
		ASSERT(false);
		return AXL_MEM_NEW_ARGS(ExecDfa<enc::Ascii>, (parent));
	}
}

//..............................................................................

} // namespace re
} // namespace axl
