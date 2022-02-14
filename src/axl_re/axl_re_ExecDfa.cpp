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
		ExecImpl<ExecReverseOffsetScanner, ExecDfaBase, typename Encoding::Decoder>(parent) {
	}

	ExecReverseOffsetScanner(
		ExecDfaBase* prevEngine,
		size_t matchEndOffset
	):
		ExecImpl<ExecReverseOffsetScanner, ExecDfaBase, typename Encoding::Decoder>(prevEngine->getParent()) {
		this->m_prevEngine = prevEngine;
		this->m_savedMatchEndOffset = matchEndOffset;
	}

	virtual
	void
	initialize(
		const StateInit& init,
		const DfaState* unused
	) {
		ExecEngine::initialize(init);
		this->m_baseCharFlags = init.m_baseCharFlags;
		this->m_baseOffset = init.m_baseOffset;
		this->m_matchEnd = NULL;
		this->m_matchAcceptId = -1;
		this->m_matchEndOffset = -1;
		this->m_savedMatchAcceptId = init.m_matchAcceptId;
		this->m_execResult = ExecResult_ContinueBackward;
	}

	virtual
	void
	exec(
		const void* p,
		size_t size
	) {
		ASSERT(this->m_offset > this->m_savedMatchEndOffset);

		// be careful not to overshoot base offset

		size_t maxSize = this->m_offset - this->m_baseOffset;
		if (size > maxSize) {
			size_t delta = size - maxSize;
			p = (char*)p + delta;
			size -= delta;
		}

		const void* p0;
		const char* end = (char*)p - 1;
		this->m_p = p0 = end + size;
		this->m_lastExecData = p;
		this->m_lastExecOffset = this->m_offset - size;
		this->m_lastExecEndOffset = this->m_offset;

		maxSize = this->m_offset - this->m_savedMatchEndOffset;
		if (size > maxSize)
			end += size - maxSize;

		p = Encoding::ReverseDecoder::decode(&this->m_decoderState, *this, (char*)p0, end);
		this->m_offset += (char*)p - (char*)p0;

		if (this->m_offset <= this->m_savedMatchEndOffset) { // reached the end of match; exec the reverse DFA now
			ASSERT(this->m_offset == this->m_savedMatchEndOffset);
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
		this->m_prevChar = c;
		this->m_p = p;
	}

protected:
	void
	execReverseDfa();
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
		ExecImpl<ExecDfa, ExecDfaBase, typename Encoding::Decoder>(parent) {
	}

	ExecDfa(ExecDfaBase* prevEngine):
		ExecImpl<ExecDfa, ExecDfaBase, typename Encoding::Decoder>(prevEngine->getParent()) {
		ASSERT(IsReverse()());
		this->m_prevEngine = prevEngine;
	}

	virtual
	void
	initialize(
		const StateInit& init,
		const DfaState* state
	) {
		ExecEngine::initialize(init);
		this->m_baseCharFlags = init.m_baseCharFlags;
		this->m_baseOffset = init.m_baseOffset;
		this->m_matchEnd = NULL;
		this->m_matchAcceptId = -1;
		this->m_matchEndOffset = -1;

		if (IsReverse()()) {
			this->m_savedMatchAcceptId = init.m_matchAcceptId;
			this->m_savedMatchEndOffset = init.m_offset;
			this->m_execResult = ExecResult_ContinueBackward;
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

			size_t maxSize = this->m_offset - this->m_baseOffset;
			if (size > maxSize) {
				size_t delta = size - maxSize;
				p = (char*)p + delta;
				size -= delta;
			}

			const char* end = (char*)p - 1;
			this->m_p = p0 = end + size;
			this->m_lastExecData = p;
			this->m_lastExecOffset = this->m_offset - size;
			this->m_lastExecEndOffset = this->m_offset;
			p = Encoding::ReverseDecoder::decode(&this->m_decoderState, *this, (char*)p0, end);
		} else {
			const char* end = (char*)p + size;
			this->m_p = p0 = p;
			this->m_lastExecData = p;
			this->m_lastExecOffset = this->m_offset;
			this->m_lastExecEndOffset = this->m_offset + size;
			p = Encoding::Decoder::decode(&this->m_decoderState, *this, (char*)p0, end);
		}

		this->m_offset += (char*)p - (char*)p0;

		if (IsReverse()() && !this->isFinalized() && this->m_offset <= this->m_baseOffset) {
			ASSERT(this->m_offset == this->m_baseOffset);

			size_t matchOffset;

			if (this->m_matchAcceptId == -1) { // didn't reach the beginning of the match (due to the base offset limit)
				this->m_matchAcceptId = this->m_savedMatchAcceptId;
				matchOffset = this->m_baseOffset;
			} else {
				ASSERT(this->m_matchAcceptId == this->m_savedMatchAcceptId);
				if (this->m_matchEnd) {
					this->m_matchEndOffset = this->m_lastExecEndOffset + (char*)this->m_matchEnd - (char*)p0 - 1;
					this->m_matchEnd = NULL;
				}

				matchOffset = this->m_matchEndOffset + 1;
			}

			createMatch(this->m_baseCharFlags, MatchPos(matchOffset, this->m_savedMatchEndOffset));
			return;
		}

		if (this->m_matchEnd) {
			this->m_matchEndOffset = IsReverse()() ?
				this->m_lastExecEndOffset + (char*)this->m_matchEnd - (char*)p0 - 1 :
				this->m_lastExecOffset + (char*)this->m_matchEnd - (char*)p0;

			this->m_matchEnd = NULL;
		}
	}

	virtual
	void
	eof() {
		ASSERT(!IsReverse()() && !this->isFinalized());

		this->m_p = NULL;
		this->m_lastExecData = NULL;
		this->m_lastExecOffset = this->m_offset;
		this->m_lastExecEndOffset = this->m_offset;

		processBoundary(this->m_offset, this->m_prevCharFlags | Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary);
		finalize(true);
	}

	inline
	void
	emitCodePoint(
		const char* p,
		utf32_t c
	) {
		if (this->m_state->m_anchorMask) {
			uint_t anchors = IsReverse()() ?
				calcReverseAnchorsUpdateCharFlags(c) :
				calcAnchorsUpdateCharFlags(c);

			anchors &= this->m_state->m_anchorMask;
			if (anchors) {
				const DfaState* anchorState = this->m_state->m_anchorTransitionMap.findValue(anchors, NULL);
				if (anchorState)
					gotoState(this->m_p, anchorState);
			}
		} else {
			this->m_prevChar = c;
			this->m_prevCharFlags = 0;
		}

		const DfaState* nextState = this->m_state->m_charTransitionMap.find(c);
		if (!nextState) {
			finalize(false);
			return;
		}

		gotoState(p, nextState);
		this->m_p = p;

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
		this->gotoStateImpl(state);

		if (state->m_flags & DfaStateFlag_Accept) {
			this->m_matchAcceptId = state->m_acceptId;
			this->m_matchEndOffset = offset;
			this->m_matchEnd = NULL;
		}
	}

	inline
	void
	gotoState(
		const void* p,
		const DfaState* state
	) {
		this->gotoStateImpl(state);

		if (state->m_flags & DfaStateFlag_Accept) {
			this->m_matchAcceptId = state->m_acceptId;
			this->m_matchEnd = p;
		}
	}

	void
	processBoundary(
		size_t offset,
		uint_t anchors
	) {
		anchors &= this->m_state->m_anchorMask;
		if (anchors) {
			const DfaState* anchorState = this->m_state->m_anchorTransitionMap.findValue(anchors, NULL);
			if (anchorState)
				gotoState(offset, anchorState);
		}
	}

	void
	finalize(bool isEof)  {
		ASSERT(this->m_parent->m_match.getOffset() == -1);

		if (this->m_matchAcceptId == -1) {
			this->m_execResult = ExecResult_NoMatch;
			return;
		}

		size_t matchEndOffset = this->m_matchEnd ?
			this->m_lastExecOffset + (char*)this->m_matchEnd - (char*)this->m_lastExecData :
			this->m_matchEndOffset;

		if (IsReverse()()) {
			ASSERT(this->m_matchAcceptId == this->m_savedMatchAcceptId);
			createMatch(this->m_prevCharFlags, MatchPos(matchEndOffset + 1, this->m_savedMatchEndOffset));
			return;
		}

		if (this->m_execFlags & ExecFlag_AnchorDataEnd) {
			if (!isEof)
				return; // can't verify until we see EOF

			if (matchEndOffset != this->m_lastExecEndOffset) {
				this->m_execResult = ExecResult_NoMatch;
				return;
			}
		}

		if (this->m_execFlags & ExecFlag_AnchorDataBegin) {
			createMatch(this->m_baseCharFlags, MatchPos(this->m_baseOffset, matchEndOffset));
			return;
		}

		if (this->m_execFlags & ExecFlag_DisableReverse)
			this->m_parent->preCreateMatch(this->m_matchAcceptId, matchEndOffset);
		else
			reverse(isEof, matchEndOffset);

		this->m_execResult = ExecResult_MatchEndOffsetOnly; // reverse will report the full match
	}

	void
	reverse(
		bool isEof,
		size_t matchEndOffset
	) {
		ASSERT(!IsReverse()());

		size_t offset = this->m_lastExecOffset + (char*)this->m_p - (char*)this->m_lastExecData;
		ASSERT(matchEndOffset <= offset);

		StateInit init;
		init.m_offset = offset;
		init.m_baseCharFlags = this->m_baseCharFlags;
		init.m_baseOffset = this->m_baseOffset;
		init.m_matchAcceptId = this->m_matchAcceptId;

		ExecResult execResult;

		if (matchEndOffset < offset) {
			ExecReverseOffsetScanner<Encoding>* scanner = AXL_MEM_NEW_ARGS(
				ExecReverseOffsetScanner<Encoding>,
				(this, matchEndOffset)
			);

			this->m_parent->m_engine = scanner; // replace engine

			scanner->initialize(init, NULL);
			scanner->exec(this->m_lastExecData, offset - this->m_lastExecOffset);
			execResult = scanner->getExecResult();
		} else {
			typedef ExecDfa<sl::True, Encoding> ReverseDfa;
			ReverseDfa* reverseDfa = AXL_MEM_NEW_ARGS(ReverseDfa, (this));
			this->m_parent->m_engine = reverseDfa; // replace engine

			uint_t prevCharFlags;

			if (isEof)
				prevCharFlags = Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary;
			else {
				// flip BeginLine/EndLine
				prevCharFlags = this->m_prevCharFlags & ~Anchor_BeginLine;
				if (prevCharFlags & (ExecEngine::CharFlag_Cr | ExecEngine::CharFlag_Lf))
					prevCharFlags |= Anchor_EndLine;
			}

			init.m_prevCharFlags = prevCharFlags;

			reverseDfa->initialize(init, this->m_parent->m_regex->getDfaReverseStartState());
			reverseDfa->exec(this->m_lastExecData, matchEndOffset - this->m_lastExecOffset);
			execResult = reverseDfa->getExecResult();
		}

		ASSERT(execResult);
		if (execResult < 0) // not finalized yet
			this->m_parent->preCreateMatch(this->m_matchAcceptId, matchEndOffset);
	}

	void
	createMatch(
		uint_t prevCharFlags,
		const MatchPos& pos
	)  {
		if (!pos.isInside(this->m_lastExecOffset, this->m_lastExecEndOffset)) {
			// match is scattered across stream; return offsets only
			this->m_parent->createMatch(this->m_matchAcceptId, this->m_lastExecOffset, NULL, pos);
			this->m_execResult = ExecResult_MatchOffsetsOnly;
		} else if (
			(this->m_execFlags & ExecFlag_DisableCapture) ||
			!this->m_parent->m_regex->getMatchCaptureCount(this->m_matchAcceptId)
		) {
			// no captures so no need to run the NFA
			this->m_parent->createMatch(this->m_matchAcceptId, this->m_lastExecOffset, this->m_lastExecData, pos);
			this->m_execResult = ExecResult_Match;
		} else {
			// TODO: use NFA-SP when possible
			const NfaState* nfaState = this->m_parent->m_regex->getMatchNfaStartState(this->m_matchAcceptId);
			const void* p = (char*)this->m_lastExecData - this->m_lastExecOffset + pos.m_offset;
			size_t length = pos.m_endOffset - pos.m_offset;

			StateInit init;
			init.m_prevCharFlags = prevCharFlags;
			init.m_offset = pos.m_offset;

			ExecNfaEngine* engine = createExecNfaVm(this->m_parent);
			engine->initialize(init, nfaState);
			engine->exec(p, length);
			engine->eof();
			this->m_execResult = engine->getExecResult();
			AXL_MEM_DELETE(engine);
		}
	}

	static
	inline
	uint_t
	calcReverseCharFlags(utf32_t c)  {
		ASSERT(IsReverse()());

		return
			c == '\r' ? ExecEngine::CharFlag_Cr | Anchor_EndLine :
			c == '\n' ? ExecEngine::CharFlag_Lf | Anchor_EndLine :
			enc::isWord(c) ? ExecEngine::CharFlag_Word :
			ExecEngine::CharFlag_Other;
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

		if (charFlags & ExecEngine::CharFlag_Lf)
			anchors |= Anchor_BeginLine;

		if ((prevCharFlags & ExecEngine::CharFlag_Word) ^ (charFlags & ExecEngine::CharFlag_Word))
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
		uint_t anchors = this->m_prevCharFlags ?
			calcReverseAnchors(this->m_prevCharFlags, this->m_prevCharFlags, charFlags) :
			calcReverseAnchors(0, calcReverseCharFlags(this->m_prevChar), charFlags);

		this->m_prevCharFlags = charFlags;
		return anchors;
	}

	inline
	uint_t
	calcAnchorsUpdateCharFlags(utf32_t c) {
		ASSERT(!IsReverse()());

		uint_t charFlags = this->calcCharFlags(c);
		uint_t anchors = this->m_prevCharFlags ?
			ExecEngine::calcAnchors(this->m_prevCharFlags, this->m_prevCharFlags, charFlags) :
			ExecEngine::calcAnchors(0, ExecEngine::calcCharFlags(this->m_prevChar), charFlags);

		this->m_prevCharFlags = charFlags;
		return anchors;
	}
};


template <typename Encoding>
void
ExecReverseOffsetScanner<Encoding>::execReverseDfa() {
	typedef ExecDfa<sl::True, Encoding> ReverseDfa;
	ReverseDfa* reverseDfa = AXL_MEM_NEW_ARGS(ReverseDfa, (this));
	this->m_parent->m_engine = reverseDfa; // replace engine

	StateInit init;
	init.m_prevCharFlags = ExecDfaBase::calcReverseCharFlags(this->m_prevChar);
	init.m_offset = this->m_savedMatchEndOffset;
	init.m_baseCharFlags = this->m_baseCharFlags;
	init.m_baseOffset = this->m_baseOffset;
	init.m_matchAcceptId = this->m_savedMatchAcceptId;
	reverseDfa->initialize(init, this->m_parent->m_regex->getDfaReverseStartState());
	reverseDfa->exec(this->m_lastExecData, this->m_savedMatchEndOffset - this->m_lastExecOffset);
	this->m_execResult = reverseDfa->getExecResult();
}

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
		typedef ExecDfa<IsReverse, Encoding> Dfa;
		return AXL_MEM_NEW_ARGS(Dfa, (parent));
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
