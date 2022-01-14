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
	ExecEngine(ExecEngineKind_Dfa, parent) {
	m_forwardEngine = NULL;
	m_state = NULL;
	m_p = NULL;
	m_matchEnd = NULL;
	m_baseCharFlags = 0;
	m_baseOffset = 0;
	m_matchAcceptId = -1;
	m_matchEndOffset = -1;
	m_savedMatchAcceptId = -1;
	m_savedMatchEndOffset = -1;
}

void
ExecDfaBase::copy(const ExecDfaBase* src) {
	ExecEngine::copy(src);
	m_forwardEngine = src->m_forwardEngine;
	m_state = src->m_state;
	m_matchEnd = src->m_matchEnd;
	m_baseCharFlags = src->m_baseCharFlags;
	m_baseOffset = src->m_baseOffset;
	m_matchAcceptId = src->m_matchAcceptId;
	m_matchEndOffset = src->m_matchEndOffset;
	m_savedMatchAcceptId = src->m_savedMatchAcceptId;
	m_savedMatchEndOffset = src->m_savedMatchEndOffset;
}

inline
void
ExecDfaBase::setForwardEngine(ExecDfaBase* forwardEngine) {
	m_forwardEngine = forwardEngine;
	m_matchAcceptId = forwardEngine->m_matchAcceptId;
	m_savedMatchAcceptId = forwardEngine->m_matchAcceptId;
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
	typename Encoding::Decoder
> {
public:
	ExecDfa(StateImpl* parent):
		ExecImpl<ExecDfa, ExecDfaBase, Encoding::Decoder>(parent) {
	}

	ExecDfa(ExecDfaBase* forwardEngine):
		ExecImpl<ExecDfa, ExecDfaBase, Encoding::Decoder>(forwardEngine->getParent()) {
		ASSERT(IsReverse()());
		setForwardEngine(forwardEngine);
	}

	virtual
	~ExecDfa() {
		if (IsReverse()() && m_forwardEngine)
			AXL_MEM_DELETE(m_forwardEngine);
	}

	virtual
	void
	initialize(
		const StateInit& init,
		const DfaState* state
	) {
		ExecEngine::initialize(init);
		m_baseCharFlags = init.m_baseCharFlags;
		m_baseOffset = init.m_baseOffset;
		m_matchEnd = NULL;
		m_matchAcceptId = -1;
		m_matchEndOffset = -1;

		if (IsReverse()()) {
			m_savedMatchAcceptId = init.m_matchAcceptId;
			m_savedMatchEndOffset = init.m_offset;
			m_execResult = ExecResult_ContinueBackward;
		}

		gotoState(init.m_offset, state);
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

			// be careful not to overshoot base offset

			if (m_baseOffset > m_lastExecOffset) {
				size_t delta = m_baseOffset - m_lastExecOffset;
				end += delta;
				m_lastExecData = (char*)p + delta;
				m_lastExecOffset = m_baseOffset;
			}

			p = Encoding::ReverseDecoder::decode(&m_decoderState, *this, (char*)m_p, end);
		} else {
			const char* end = (char*)p + size;
			m_p = p0 = p;
			m_lastExecOffset = m_offset;
			m_lastExecEndOffset = m_offset + size;
			p = Encoding::Decoder::decode(&m_decoderState, *this, (char*)p, end);
		}

		m_offset += (char*)p - (char*)p0;
		if (IsReverse()() && !isFinalized() && m_offset <= m_baseOffset) {
			ASSERT(m_offset == m_baseOffset);

			if (m_matchAcceptId == -1) // didn't reach the beginning of the match (due to the base offset limit)
				m_matchAcceptId = m_savedMatchAcceptId;
			else
				ASSERT(m_matchAcceptId == m_savedMatchAcceptId);

			createMatch(m_baseCharFlags, MatchPos(m_baseOffset, m_savedMatchEndOffset));
			return;
		}

		if (m_matchEnd) {
			m_matchEndOffset = IsReverse()() ?
				m_lastExecEndOffset + (char*)m_matchEnd - (char*)p0 - 1 :
				m_lastExecOffset + (char*)m_matchEnd - (char*)p0;

			m_matchEnd = NULL;
		}
	}

	virtual
	void
	eof() {
		ASSERT(!IsReverse()() && !isFinalized());

		m_lastExecData = NULL;
		m_lastExecOffset = m_offset;
		m_lastExecEndOffset = m_offset;

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
					gotoState(m_p, anchorState);
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
		}
	}

	inline
	void
	gotoState(
		const void* p,
		const DfaState* state
	) {
		gotoStateImpl(state);

		if (state->m_flags & DfaStateFlag_Accept) {
			m_matchAcceptId = state->m_acceptId;
			m_matchEnd = p;
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
			createMatch(m_prevCharFlags, MatchPos(matchEndOffset + 1, m_savedMatchEndOffset));
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
			createMatch(m_baseCharFlags, MatchPos(m_baseOffset, matchEndOffset));
			return;
		}

		if (m_execFlags & ExecFlag_DisableReverse)
			m_parent->preCreateMatch(m_matchAcceptId, matchEndOffset);
		else
			reverse(isEof, matchEndOffset);

		m_execResult = ExecResult_MatchEndOffsetOnly; // reverse will report the full match
	}

	void
	reverse(
		bool isEof,
		size_t matchEndOffset
	) {
		ASSERT(!IsReverse()() && matchEndOffset >= m_lastExecOffset);

		ExecDfa<sl::True, Encoding>* reverseDfa = AXL_MEM_NEW_ARGS((ExecDfa<sl::True, Encoding>), (this));
		m_parent->m_engine = reverseDfa; // replace engine

		uint_t prevCharFlags;

		if (isEof && matchEndOffset == m_lastExecEndOffset)
			prevCharFlags = Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary;
		else {
			// flip BeginLine/EndLine
			prevCharFlags = m_prevCharFlags & ~Anchor_BeginLine;
			if (prevCharFlags & (CharFlag_Cr | CharFlag_Lf))
				prevCharFlags |= Anchor_EndLine;
		}

		StateInit init;
		init.m_prevCharFlags = prevCharFlags;
		init.m_offset = matchEndOffset;
		init.m_baseCharFlags = m_baseCharFlags;
		init.m_baseOffset = m_baseOffset;
		init.m_matchAcceptId = m_matchAcceptId;
		reverseDfa->initialize(init, m_parent->m_regex->getDfaReverseStartState());

		size_t size = matchEndOffset - m_lastExecOffset;
		if (size) {
			reverseDfa->exec(m_lastExecData, size);
			ASSERT(m_execResult);
		}

		if (!reverseDfa->isFinalized())
			m_parent->preCreateMatch(m_matchAcceptId, matchEndOffset);
	}

	void
	createMatch(
		uint_t prevCharFlags,
		const MatchPos& pos
	)  {
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

			StateInit init;
			init.m_prevCharFlags = prevCharFlags;
			init.m_offset = pos.m_offset;

			ExecNfaEngine* engine = createExecNfaVm(m_parent);
			engine->initialize(init, nfaState);
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

template <
	typename IsReverse,
	typename Encoding
>
class ExecDfaFactory: public ExecEngineFactory {
public:
	virtual
	ExecEngine*
	createExecEngine(StateImpl* parent) {
		return AXL_MEM_NEW_ARGS((ExecDfa<IsReverse, Encoding>), (parent));
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecDfaBase*
createExecDfa(StateImpl* parent) {
	static ExecDfaFactory<sl::False, enc::Ascii>     asciiFactory;
	static ExecDfaFactory<sl::False, enc::Utf8>      utf8Factory;
	static ExecDfaFactory<sl::False, enc::Utf16s>    utf16Factory;
	static ExecDfaFactory<sl::False, enc::Utf16s_be> utf16Factory_be;
	static ExecDfaFactory<sl::False, enc::Utf32s>    utf32Factory;
	static ExecDfaFactory<sl::False, enc::Utf32s_be> utf32Factory_be;

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

	return (ExecDfaBase*)factoryTable[i]->createExecEngine(parent);
}

ExecDfaBase*
createExecDfaReverse(StateImpl* parent) {
	static ExecDfaFactory<sl::True, enc::Ascii>     asciiFactory;
	static ExecDfaFactory<sl::True, enc::Utf8>      utf8Factory;
	static ExecDfaFactory<sl::True, enc::Utf16s>    utf16Factory;
	static ExecDfaFactory<sl::True, enc::Utf16s_be> utf16Factory_be;
	static ExecDfaFactory<sl::True, enc::Utf32s>    utf32Factory;
	static ExecDfaFactory<sl::True, enc::Utf32s_be> utf32Factory_be;

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

	return (ExecDfaBase*)factoryTable[i]->createExecEngine(parent);
}

//..............................................................................

} // namespace re
} // namespace axl
