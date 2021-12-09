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
#include "axl_enc_Utf16s.h"
#include "axl_enc_Utf32s.h"

namespace axl {
namespace re {

//..............................................................................

ExecDfaBase::ExecDfaBase(StateImpl* parent):
	ExecEngine(parent) {
	m_direction = Direction_Forward;
	m_dfaState = NULL;
	m_savedMatchEndOffset = -1;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = -1;
#endif
}

void
ExecDfaBase::copy(const ExecDfaBase* src) {
	ExecEngine::copy(src);
	m_direction = src->m_direction;
	m_dfaState = src->m_dfaState;
	m_savedMatchEndOffset = src->m_savedMatchEndOffset;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = src->m_savedMatchAcceptId;
#endif
}

void
ExecDfaBase::reset(size_t offset) {
	ExecEngine::reset(offset);
	m_savedMatchEndOffset = -1;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = -1;
#endif

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataBegin) {
		m_matchBeginOffset = offset;
		gotoDfaState(offset, m_parent->m_regex->getDfaMatchStartState());
	} else {
		gotoDfaState(offset, m_parent->m_regex->getDfaSearchStartState());
	}
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

bool
ExecDfaBase::eof() {
	uint_t anchors = (Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary) & m_dfaState->m_anchorMask;
	if (anchors) {
		const DfaState* anchorState = m_dfaState->m_anchorTransitionMap.findValue(anchors, NULL);
		if (anchorState)
			gotoDfaState(m_parent->m_offset, anchorState);
	}

	return finalize(true);
}

bool
ExecDfaBase::finalize(bool isEof) {
	if (m_parent->m_matchAcceptId != -1) // already finalized
		return true;

	if (m_matchAcceptId == -1)
		return false;

	size_t matchEndOffset = m_matchEnd ?
		m_parent->m_lastExecOffset + (char*)m_matchEnd - (char*)m_parent->m_lastExecBuffer :
		m_matchEndOffset;

	if (m_direction == Direction_Backward) {
		ASSERT(m_matchAcceptId == m_savedMatchAcceptId);
		m_parent->createMatch(m_matchAcceptId, MatchPos(matchEndOffset + 1, m_savedMatchEndOffset));
		return true;
	}

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataEnd) {
		if (!isEof)
			return true; // can't verify until we see EOF

		if (matchEndOffset != m_parent->m_lastExecOffset + m_parent->m_lastExecSize)
			return false;
	}

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataBegin) {
		m_parent->createMatch(m_matchAcceptId, MatchPos(m_matchBeginOffset, matchEndOffset));
		return true;
	}

	// reset the engine before going backwards

#if (_AXL_DEBUG)
	m_savedMatchAcceptId = m_matchAcceptId;
#endif
	m_savedMatchEndOffset = matchEndOffset;
	m_direction = Direction_Backward;
	m_decoderState = 0;
	m_matchAcceptId = -1;
	m_matchEndOffset = -1;
	m_matchEnd = NULL;

	gotoDfaState(matchEndOffset, m_parent->m_regex->getDfaReverseMatchStartState());
	exec(m_parent->m_lastExecBuffer, matchEndOffset);
	ASSERT(m_execResult != ExecResult_False);

	if (m_parent->m_offset == m_parent->m_baseOffset && m_dfaState->m_anchorMask) {
		uint_t anchors = (Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary) & m_dfaState->m_anchorMask;
		if (anchors) {
			const DfaState* anchorState = m_dfaState->m_anchorTransitionMap.findValue(anchors, NULL);
			if (anchorState)
				gotoDfaState(m_parent->m_offset - 1, anchorState); // backward offsets are one-off
		}
	}

	return finalize(true);
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
		if (m_direction == Direction_Forward) {
			const char* end = (char*)p + size;
			m_p = p;
			p = Encoding::Decoder::decode(&m_decoderState, *this, (char*)p, end);
		} else {
			const char* end = (char*)p - 1;
			m_p = end + size;
			p = Encoding::ReverseDecoder::decode(&m_decoderState, *this, (char*)m_p, end);
		}
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
			uint_t anchors = m_direction == Direction_Backward ?
				m_parent->calcReverseAnchorsUpdateCharFlags(c) :
				m_parent->calcAnchorsUpdateCharFlags(c);

			anchors &= m_dfaState->m_anchorMask;
			if (anchors) {
				const DfaState* anchorState = m_dfaState->m_anchorTransitionMap.findValue(anchors, NULL);
				if (anchorState)
					gotoDfaState(m_p, anchorState);
			}
		} else {
			m_parent->m_prevChar = c;
			m_parent->m_prevCharFlags = 0;
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
createExecDfa(
	StateImpl* parent,
	enc::CharCodecKind codecKind
) {
	switch (codecKind) {
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
