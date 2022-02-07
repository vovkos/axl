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
	m_prevEngine = NULL;
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
	m_prevEngine = NULL; // don't copy src engine (we keep it for timely deletion only)
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
uint_t
ExecDfaBase::calcReverseCharFlags(utf32_t c)  {
	return
		c == '\r' ? CharFlag_Cr | Anchor_EndLine :
		c == '\n' ? CharFlag_Lf | Anchor_EndLine :
		enc::isWord(c) ? CharFlag_Word :
		CharFlag_Other;
}

inline
uint_t
ExecDfaBase::calcReverseAnchors(
	uint_t anchors,
	uint_t prevCharFlags,
	uint_t charFlags
) {
	if (charFlags & CharFlag_Lf)
		anchors |= Anchor_BeginLine;

	if ((prevCharFlags & CharFlag_Word) ^ (charFlags & CharFlag_Word))
		anchors |= Anchor_WordBoundary;
	else
		anchors |= Anchor_NotWordBoundary;

	return anchors;
}

inline
void
ExecDfaBase::gotoStateImpl(const DfaState* state) {
	if (!(state->m_flags & DfaStateFlag_Ready))
		m_parent->m_regex->prepareDfaState(state);

	m_state = state;
}
//..............................................................................

// after we run out of NFA states on the forward run, we have to rollback to
// the last match end offset -- this scanner does just that; then it creates
// and switches to the reverse DFA

template <typename Encoding>
class ExecReverseOffsetScanner: public ExecImpl<
	ExecReverseOffsetScanner<Encoding>,
	ExecDfaBase,
	typename Encoding::Decoder
> {
public:
	ExecReverseOffsetScanner(StateImpl* parent):
		ExecImpl<ExecReverseOffsetScanner, ExecDfaBase, Encoding::Decoder>(parent) {
	}

	ExecReverseOffsetScanner(
		ExecDfaBase* prevEngine,
		size_t matchEndOffset
	):
		ExecImpl<ExecReverseOffsetScanner, ExecDfaBase, Encoding::Decoder>(prevEngine->getParent()) {
		m_prevEngine = prevEngine;
		m_savedMatchEndOffset = matchEndOffset;
	}

	virtual
	void
	initialize(
		const StateInit& init,
		const DfaState* unused
	) {
		ExecEngine::initialize(init);
		m_baseCharFlags = init.m_baseCharFlags;
		m_baseOffset = init.m_baseOffset;
		m_matchEnd = NULL;
		m_matchAcceptId = -1;
		m_matchEndOffset = -1;
		m_savedMatchAcceptId = init.m_matchAcceptId;
		m_execResult = ExecResult_ContinueBackward;
	}

	virtual
	void
	exec(
		const void* p,
		size_t size
	) {
		ASSERT(m_offset > m_savedMatchEndOffset);

		// be careful not to overshoot base offset

		size_t maxSize = m_offset - m_baseOffset;
		if (size > maxSize) {
			size_t delta = size - maxSize;
			p = (char*)p + delta;
			size -= delta;
		}

		const void* p0;
		const char* end = (char*)p - 1;
		m_p = p0 = end + size;
		m_lastExecData = p;
		m_lastExecOffset = m_offset - size;
		m_lastExecEndOffset = m_offset;

		maxSize = m_offset - m_savedMatchEndOffset;
		if (size > maxSize)
			end += size - maxSize;

		p = Encoding::ReverseDecoder::decode(&m_decoderState, *this, (char*)p0, end);
		m_offset += (char*)p - (char*)p0;

		if (m_offset <= m_savedMatchEndOffset) { // reached the end of match; exec the reverse DFA now
			ASSERT(m_offset == m_savedMatchEndOffset);
			execReverseDfa();
		}
	}

	virtual
	void
	eof() {
		ASSERT(false);
	}

	inline
	void
	emitCodePoint(
		const char* p,
		utf32_t c
	) {
		m_prevChar = c;
		m_p = p;
	}

protected:
	void
	execReverseDfa() {
		ExecDfa<sl::True, Encoding>* reverseDfa = AXL_MEM_NEW_ARGS((ExecDfa<sl::True, Encoding>), (this));
		m_parent->m_engine = reverseDfa; // replace engine

		StateInit init;
		init.m_prevCharFlags = calcReverseCharFlags(m_prevChar);
		init.m_offset = m_savedMatchEndOffset;
		init.m_baseCharFlags = m_baseCharFlags;
		init.m_baseOffset = m_baseOffset;
		init.m_matchAcceptId = m_savedMatchAcceptId;
		reverseDfa->initialize(init, m_parent->m_regex->getDfaReverseStartState());
		reverseDfa->exec(m_lastExecData, m_savedMatchEndOffset - m_lastExecOffset);
		m_execResult = reverseDfa->getExecResult();
	}
};

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

	ExecDfa(ExecDfaBase* prevEngine):
		ExecImpl<ExecDfa, ExecDfaBase, Encoding::Decoder>(prevEngine->getParent()) {
		ASSERT(IsReverse()());
		m_prevEngine = prevEngine;
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

		if (IsReverse()()) {
			// be careful not to overshoot base offset

			size_t maxSize = m_offset - m_baseOffset;
			if (size > maxSize) {
				size_t delta = size - maxSize;
				p = (char*)p + delta;
				size -= delta;
			}

			const char* end = (char*)p - 1;
			m_p = p0 = end + size;
			m_lastExecData = p;
			m_lastExecOffset = m_offset - size;
			m_lastExecEndOffset = m_offset;
			p = Encoding::ReverseDecoder::decode(&m_decoderState, *this, (char*)p0, end);
		} else {
			const char* end = (char*)p + size;
			m_p = p0 = p;
			m_lastExecData = p;
			m_lastExecOffset = m_offset;
			m_lastExecEndOffset = m_offset + size;
			p = Encoding::Decoder::decode(&m_decoderState, *this, (char*)p0, end);
		}

		m_offset += (char*)p - (char*)p0;

		if (IsReverse()() && !isFinalized() && m_offset <= m_baseOffset) {
			ASSERT(m_offset == m_baseOffset);

			size_t matchOffset;

			if (m_matchAcceptId == -1) { // didn't reach the beginning of the match (due to the base offset limit)
				m_matchAcceptId = m_savedMatchAcceptId;
				matchOffset = m_baseOffset;
			} else {
				ASSERT(m_matchAcceptId == m_savedMatchAcceptId);
				if (m_matchEnd) {
					m_matchEndOffset = m_lastExecEndOffset + (char*)m_matchEnd - (char*)p0 - 1;
					m_matchEnd = NULL;
				}

				matchOffset = m_matchEndOffset + 1;
			}

			createMatch(m_baseCharFlags, MatchPos(matchOffset, m_savedMatchEndOffset));
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

		m_p = NULL;
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
		m_p = p;

		if (nextState->m_flags & DfaStateFlag_Dead)
			finalize(false);
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
		ASSERT(!IsReverse()());

		size_t offset = m_lastExecOffset + (char*)m_p - (char*)m_lastExecData;
		ASSERT(matchEndOffset <= offset);

		StateInit init;
		init.m_offset = offset;
		init.m_baseCharFlags = m_baseCharFlags;
		init.m_baseOffset = m_baseOffset;
		init.m_matchAcceptId = m_matchAcceptId;

		ExecResult execResult;

		if (matchEndOffset < offset) {
			ExecReverseOffsetScanner<Encoding>* scanner = AXL_MEM_NEW_ARGS(
				(ExecReverseOffsetScanner<Encoding>),
				(this, matchEndOffset)
			);

			m_parent->m_engine = scanner; // replace engine

			scanner->initialize(init, NULL);
			scanner->exec(m_lastExecData, offset - m_lastExecOffset);
			execResult = scanner->getExecResult();
		} else {
			ExecDfa<sl::True, Encoding>* reverseDfa = AXL_MEM_NEW_ARGS((ExecDfa<sl::True, Encoding>), (this));
			m_parent->m_engine = reverseDfa; // replace engine

			uint_t prevCharFlags;

			if (isEof)
				prevCharFlags = Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary;
			else {
				// flip BeginLine/EndLine
				prevCharFlags = m_prevCharFlags & ~Anchor_BeginLine;
				if (prevCharFlags & (CharFlag_Cr | CharFlag_Lf))
					prevCharFlags |= Anchor_EndLine;
			}

			init.m_prevCharFlags = prevCharFlags;

			reverseDfa->initialize(init, m_parent->m_regex->getDfaReverseStartState());
			reverseDfa->exec(m_lastExecData, matchEndOffset - m_lastExecOffset);
			execResult = reverseDfa->getExecResult();
		}

		ASSERT(execResult);
		if (execResult < 0) // not finalized yet
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
