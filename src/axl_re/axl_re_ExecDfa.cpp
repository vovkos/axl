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
	m_execResult = ExecResult_Undefined;
	m_decoderState = 0;
	m_dfaState = NULL;
	m_matchEndOffset = -1;
	m_matchAcceptId = -1;
}

void
ExecDfaBase::copy(const ExecDfaBase* src) {
	m_decoderState = src->m_decoderState;
	m_dfaState = src->m_dfaState;
	m_matchEndOffset = src->m_matchEndOffset;
	m_matchAcceptId = src->m_matchAcceptId;
}

void
ExecDfaBase::reset(size_t offset) {
	m_decoderState = 0;
	m_matchEndOffset = -1;
	m_matchAcceptId = -1;

	gotoDfaState(
		offset,
		(m_parent->m_execFlags & RegexExecFlag_AnchorDataBegin) ?
			m_parent->m_regex->getDfaMatchStartState() :
			m_parent->m_regex->getDfaSearchStartState()
	);
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
		m_matchEndOffset = m_parent->m_lastExecOffset + (char*)p - (char*)m_parent->m_lastExecBuffer;
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
	}

	gotoDfaStateImpl(state);
}

bool
ExecDfaBase::eof() {
	uint_t anchors = Anchor_EndLine | Anchor_EndText;
	if (m_parent->m_prevCharFlags & StateImpl::CharFlag_Word)
		anchors |= Anchor_WordBoundary;
	else
		anchors |= Anchor_NotWordBoundary;

	anchors &= m_dfaState->m_anchorMask;
	if (anchors) {
		const DfaState* anchorState = m_dfaState->m_anchorTransitionMap.findValue(anchors, NULL);
		ASSERT(anchorState);
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

	if (m_parent->m_execFlags & RegexExecFlag_AnchorDataEnd) {
		if (!isEof)
			return true; // can't verify until we see EOF

		if (m_matchEndOffset != m_parent->m_lastExecOffset + m_parent->m_lastExecSize)
			return false;
	}

	// TODO: roll back to the beginning of the match

	m_parent->createMatch(m_matchAcceptId, MatchPos(0, m_matchEndOffset));
	return true;
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
	bool
	exec(
		const void* p,
		size_t size
	) {
		m_execResult = ExecResult_Undefined;
		p = Encoding::Decoder::decode(&m_decoderState, *this, (char*)p, (char*)p + size);
		m_parent->m_offset = m_parent->m_lastExecOffset + (char*)p - (char*)m_parent->m_lastExecBuffer;
		return m_execResult != ExecResult_False; // undefined or true => true
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
			uint_t anchors = m_parent->calcAnchorsUpdateCharFlags(c) & m_dfaState->m_anchorMask;
			if (anchors) {
				const DfaState* anchorState = m_dfaState->m_anchorTransitionMap.findValue(anchors, NULL);
				ASSERT(anchorState);
				gotoDfaState(p, anchorState);
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
