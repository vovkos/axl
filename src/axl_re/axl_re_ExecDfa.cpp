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
	m_forwardEngine = NULL;
	m_state = NULL;
	m_p = NULL;
	m_matchEnd = NULL;
	m_baseOffset = 0;
	m_matchAcceptId = -1;
	m_matchCharFlags = 0;
	m_matchEndOffset = -1;
	m_savedMatchEndOffset = -1;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = -1;
#endif
}

void
ExecDfaBase::copy(const ExecDfaBase* src) {
	ExecEngine::copy(src);
	m_forwardEngine = src->m_forwardEngine;
	m_state = src->m_state;
	m_matchEnd = src->m_matchEnd;
	m_baseOffset = src->m_baseOffset;
	m_matchAcceptId = src->m_matchAcceptId;
	m_matchCharFlags = src->m_matchCharFlags;
	m_matchEndOffset = src->m_matchEndOffset;
	m_savedMatchEndOffset = src->m_savedMatchEndOffset;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = src->m_savedMatchAcceptId;
#endif
}

void
ExecDfaBase::setForwardEngine(ExecDfaBase* forwardEngine) {
	m_forwardEngine = forwardEngine;
	m_lastExecData = forwardEngine->m_lastExecData;
	m_lastExecOffset = forwardEngine->m_lastExecOffset;
	m_lastExecEndOffset = forwardEngine->m_lastExecEndOffset;
	m_baseOffset = forwardEngine->m_baseOffset;
#if (_AXL_DEBUG)
	m_savedMatchAcceptId = forwardEngine->m_matchAcceptId;
#endif
}

inline
void
ExecDfaBase::gotoStateImpl(const DfaState* state) {
	if (!(state->m_flags & DfaStateFlag_Ready))
		m_parent->m_regex->prepareDfaState(state);

	m_state = state;
}

//..............................................................................

template <
	typename IsReverse,
	typename Encoding
>
class ExecDfa: public ExecImpl<
	ExecDfa<IsReverse, Encoding>,
	ExecDfaBase,
	Encoding
> {
public:
	ExecDfa(StateImpl* parent):
		ExecImpl<ExecDfa, ExecDfaBase, Encoding>(parent) {
		ASSERT(!IsReverse()());
	}

	ExecDfa(ExecDfaBase* forwardEngine):
		ExecImpl<ExecDfa, ExecDfaBase, Encoding>(forwardEngine->getParent()) {
		ASSERT(IsReverse()());
		setForwardEngine(forwardEngine);
	}

	virtual
	~ExecDfa() {
		if (IsReverse()() && m_forwardEngine)
			AXL_MEM_DELETE(m_forwardEngine);
	}

	void
	reverse(
		bool isEof,
		size_t matchEndOffset,
		uint_t matchCharFlags
	) {
		ASSERT(IsReverse()() && m_forwardEngine && matchEndOffset >= m_lastExecOffset);

		if (isEof && matchEndOffset == m_lastExecEndOffset)
			m_prevCharFlags = Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary;
		else {
			// flip BeginLine/EndLine
			m_prevCharFlags = matchCharFlags & ~Anchor_BeginLine;
			if (matchCharFlags & (CharFlag_Cr | CharFlag_Lf))
				m_prevCharFlags |= Anchor_EndLine;
		}

		m_savedMatchEndOffset = m_offset = matchEndOffset;
		m_execResult = ExecResult_ContinueBackward;
		gotoState(matchEndOffset, m_parent->m_regex->getDfaReverseStartState());

		size_t size = matchEndOffset - m_lastExecOffset;
		if (size) {
			exec(m_lastExecData, size);
			ASSERT(m_execResult);
		}
	}

	virtual
	void
	reset(size_t offset) {
		if (IsReverse()()) {
			ASSERT(m_forwardEngine);
			m_forwardEngine->reset(offset);
			m_parent->m_engine = m_forwardEngine;
			AXL_MEM_DELETE(this);
			return;
		}

		ExecEngine::reset(offset);
		m_matchEnd = NULL;
		m_baseOffset = offset;
		m_matchCharFlags = 0;
		m_matchAcceptId = -1;
		m_matchEndOffset = -1;
		m_savedMatchEndOffset = -1;
	#if (_AXL_DEBUG)
		m_savedMatchAcceptId = -1;
	#endif

		gotoState(
			offset,
			m_execFlags & ExecFlag_AnchorDataBegin ?
				m_parent->m_regex->getDfaMatchStartState() :
				m_parent->m_regex->getDfaSearchStartState()
		);
	}

	virtual
	void
	exec(
		const void* p,
		size_t size
	) {
		const void* p0;

		m_lastExecData = p;

		if (IsReverse()()) {
			const char* end = (char*)p - 1;
			m_p = p0 = end + size;
			m_lastExecOffset = m_offset - size;
			m_lastExecEndOffset = m_offset;
			p = Encoding::ReverseDecoder::decode(&m_decoderState, *this, (char*)m_p, end);
		} else {
			const char* end = (char*)p + size;
			m_p = p0 = p;
			m_lastExecOffset = m_offset;
			m_lastExecEndOffset = m_offset + size;
			p = Encoding::Decoder::decode(&m_decoderState, *this, (char*)p, end);
		}

		m_offset += (char*)p - (char*)p0;
		if (IsReverse()() && !isFinalized() && m_offset == m_baseOffset) {
			processBoundary(m_offset - 1, m_prevCharFlags | Anchor_BeginLine | Anchor_BeginText | Anchor_WordBoundary);
			finalize(false);
		}

		if (m_matchEnd) {
			m_matchEndOffset = m_lastExecOffset + (char*)m_matchEnd - (char*)p0;
			m_matchEnd = NULL;
		}
	}

	virtual
	void
	eof() {
		ASSERT(!IsReverse()() && !isFinalized());
		processBoundary(m_offset, m_prevCharFlags | Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary);
		finalize(true);
	}

	inline
	void
	emitCodePoint(
		const char* p,
		utf32_t c
	) {
		if (m_state->m_anchorMask) {
			uint_t anchors = IsReverse()() ?
				calcReverseAnchorsUpdateCharFlags(c) :
				calcAnchorsUpdateCharFlags(c);

			anchors &= m_state->m_anchorMask;
			if (anchors) {
				const DfaState* anchorState = m_state->m_anchorTransitionMap.findValue(anchors, NULL);
				if (anchorState)
					gotoState(m_p, anchorState, m_prevCharFlags);
			}
		} else {
			m_prevChar = c;
			m_prevCharFlags = 0;
		}

		const DfaState* nextState = m_state->m_charTransitionMap.find(c);
		if (!nextState) {
			finalize(false);
			return;
		}

		gotoState(p, nextState);

		if (nextState->m_flags & DfaStateFlag_Dead)
			finalize(false);

		m_p = p;
	}

protected:
	inline
	void
	gotoState(
		size_t offset,
		const DfaState* state
	) {
		gotoStateImpl(state);

		if (state->m_flags & DfaStateFlag_Accept) {
			m_matchAcceptId = state->m_acceptId;
			m_matchEndOffset = offset;
			m_matchEnd = NULL;

			if (!IsReverse()())
				m_matchCharFlags = CharFlag_Other;
		}
	}

	inline
	void
	gotoState(
		const void* p,
		const DfaState* state,
		uint_t matchCharFlags = CharFlag_Other
	) {
		gotoStateImpl(state);

		if (state->m_flags & DfaStateFlag_Accept) {
			m_matchAcceptId = state->m_acceptId;
			m_matchEnd = p;

			if (!IsReverse()())
				m_matchCharFlags = matchCharFlags;
		}
	}

	void
	processBoundary(
		size_t offset,
		uint_t anchors
	) {
		anchors &= m_state->m_anchorMask;
		if (anchors) {
			const DfaState* anchorState = m_state->m_anchorTransitionMap.findValue(anchors, NULL);
			if (anchorState)
				gotoState(offset, anchorState);
		}
	}

	void
	finalize(bool isEof)  {
		ASSERT(m_parent->m_match.getOffset() == -1);

		if (m_matchAcceptId == -1) {
			m_execResult = ExecResult_NoMatch;
			return;
		}

		size_t matchEndOffset = m_matchEnd ?
			m_lastExecOffset + (char*)m_matchEnd - (char*)m_lastExecData :
			m_matchEndOffset;

		if (IsReverse()()) {
			ASSERT(m_matchAcceptId == m_savedMatchAcceptId);
			createMatch(MatchPos(matchEndOffset + 1, m_savedMatchEndOffset));
			return;
		}

		if (m_execFlags & ExecFlag_AnchorDataEnd) {
			if (!isEof)
				return; // can't verify until we see EOF

			if (matchEndOffset != m_lastExecEndOffset) {
				m_execResult = ExecResult_NoMatch;
				return;
			}
		}

		if (m_execFlags & ExecFlag_AnchorDataBegin) {
			createMatch(MatchPos(m_baseOffset, matchEndOffset));
			return;
		}

		ExecDfa<sl::True, Encoding>* reverseDfa = AXL_MEM_NEW_ARGS((ExecDfa<sl::True, Encoding>), (this));
		m_parent->m_engine = reverseDfa; // replace engine
		reverseDfa->reverse(isEof, matchEndOffset, m_matchCharFlags);
		if (!reverseDfa->isFinalized())
			m_parent->preCreateMatch(m_matchAcceptId, matchEndOffset);

		m_execResult = ExecResult_Match; // stop decoder from feeding us more chars
	}

	void
	createMatch(const MatchPos& pos)  {
		if (!pos.isInside(m_lastExecOffset, m_lastExecEndOffset)) {
			// match is scattered across stream; return offsets only
			m_parent->createMatch(m_matchAcceptId, m_lastExecOffset, NULL, pos);
			m_execResult = ExecResult_MatchOffsetsOnly;
		} else if (
			(m_execFlags & ExecFlag_DisableCapture) ||
			!m_parent->m_regex->getMatchCaptureCount(m_matchAcceptId)
		) {
			// no captures so no need to run the NFA
			m_parent->createMatch(m_matchAcceptId, m_lastExecOffset, m_lastExecData, pos);
			m_execResult = ExecResult_Match;
		} else {
			// TODO: use NFA-SP when possible
			const NfaState* nfaState = m_parent->m_regex->getMatchNfaStartState(m_matchAcceptId);
			const void* p = (char*)m_lastExecData - m_lastExecOffset + pos.m_offset;
			size_t length = pos.m_endOffset - pos.m_offset;

			ExecNfaEngine* engine = createExecNfaVm(m_parent);
			engine->reset(pos.m_offset, nfaState);
			engine->exec(p, length);
			engine->eof();
			m_execResult = engine->getExecResult();
			AXL_MEM_DELETE(engine);
		}
	}

	static
	inline
	uint_t
	calcReverseCharFlags(utf32_t c)  {
		ASSERT(IsReverse()());

		return
			c == '\r' ? CharFlag_Cr | Anchor_EndLine :
			c == '\n' ? CharFlag_Lf | Anchor_EndLine :
			enc::isWord(c) ? CharFlag_Word :
			CharFlag_Other;
	}

	static
	inline
	uint_t
	calcReverseAnchors(
		uint_t anchors,
		uint_t prevCharFlags,
		uint_t charFlags
	) {
		ASSERT(IsReverse()());

		if (charFlags & CharFlag_Lf)
			anchors |= Anchor_BeginLine;

		if ((prevCharFlags & CharFlag_Word) ^ (charFlags & CharFlag_Word))
			anchors |= Anchor_WordBoundary;
		else
			anchors |= Anchor_NotWordBoundary;

		return anchors;
	}

	inline
	uint_t
	calcReverseAnchorsUpdateCharFlags(utf32_t c)  {
		ASSERT(IsReverse()());

		uint_t charFlags = calcReverseCharFlags(c);
		uint_t anchors = m_prevCharFlags ?
			calcReverseAnchors(m_prevCharFlags, m_prevCharFlags, charFlags) :
			calcReverseAnchors(0, calcReverseCharFlags(m_prevChar), charFlags);

		m_prevCharFlags = charFlags;
		return anchors;
	}

	inline
	uint_t
	calcAnchorsUpdateCharFlags(utf32_t c) {
		ASSERT(!IsReverse()());

		uint_t charFlags = calcCharFlags(c);
		uint_t anchors = m_prevCharFlags ?
			calcAnchors(m_prevCharFlags, m_prevCharFlags, charFlags) :
			calcAnchors(0, calcCharFlags(m_prevChar), charFlags);

		m_prevCharFlags = charFlags;
		return anchors;
	}
};

//..............................................................................

ExecEngine*
createExecDfa(StateImpl* parent) {
	switch (parent->m_init.m_codecKind) {
	case enc::CharCodecKind_Ascii:
		return AXL_MEM_NEW_ARGS((ExecDfa<sl::False, enc::Ascii>), (parent));
	case enc::CharCodecKind_Utf8:
		return AXL_MEM_NEW_ARGS((ExecDfa<sl::False, enc::Utf8>), (parent));
	case enc::CharCodecKind_Utf16:
		return AXL_MEM_NEW_ARGS((ExecDfa<sl::False, enc::Utf16s>), (parent));
	case enc::CharCodecKind_Utf16_be:
		return AXL_MEM_NEW_ARGS((ExecDfa<sl::False, enc::Utf16s_be>), (parent));
	case enc::CharCodecKind_Utf32:
		return AXL_MEM_NEW_ARGS((ExecDfa<sl::False, enc::Utf32s>), (parent));
	case enc::CharCodecKind_Utf32_be:
		return AXL_MEM_NEW_ARGS((ExecDfa<sl::False, enc::Utf32s_be>), (parent));
	default:
		ASSERT(false);
		return AXL_MEM_NEW_ARGS((ExecDfa<sl::False, enc::Ascii>), (parent));
	}
}

//..............................................................................

} // namespace re
} // namespace axl
