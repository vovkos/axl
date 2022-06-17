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

#pragma once

#define _AXL_RE_EXEC_H

#include "axl_re_StateInit.h"

namespace axl {
namespace re {

struct StateImpl;
struct NfaState;

//..............................................................................

enum ExecFlag {
	ExecFlag_Stream          = 0x01, // feed data chunk-by-chunk, then call re::Regex::eof()
	ExecFlag_Reverse         = 0x02, // run the reverse DFA to find the beginning of the match
	ExecFlag_DisableReverse  = 0x04, // don't run the reverse DFA to find beginnings of matches
	ExecFlag_DisableCapture  = 0x08, // don't capture sub-matches
	ExecFlag_AnchorDataBegin = 0x10, // match must start on the first byte of data
	ExecFlag_AnchorDataEnd   = 0x20, // match must end on the last byte of data
	ExecFlag_ExactMatch      = ExecFlag_AnchorDataBegin | ExecFlag_AnchorDataEnd,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ExecResult {
	ExecResult_Match              = 3,  // match with data
	ExecResult_MatchOffsetsOnly   = 2,  // match without data
	ExecResult_MatchEndOffsetOnly = 1,  // match without data and begin offset
	ExecResult_NoMatch            = 0,
	ExecResult_Continue           = -1, // continue feeding data
	ExecResult_ContinueBackward   = -2, // continue feeding backward data
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ExecEngineKind {
	ExecEngineKind_Undefined,
	ExecEngineKind_Dfa,
	ExecEngineKind_Nfa,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class ExecEngine {
protected:
	enum CharFlag {
		// can be combined with Anchor-s, so make sure these don't overlap

		CharFlag_Cr    = 0x100,
		CharFlag_Lf    = 0x200,
		CharFlag_Nl    = CharFlag_Lf,
		CharFlag_Word  = 0x400,
		CharFlag_Other = 0x800, // denotes pre-calculated flags
	};

protected:
	ExecEngineKind m_engineKind;
	StateImpl* m_parent;
	const void* m_lastExecData;
	size_t m_lastExecOffset;
	size_t m_lastExecEndOffset;
	size_t m_offset;
	uint_t m_execFlags; // cache to save one indirection
	ExecResult m_execResult;
	enc::DecoderState m_decoderState;
	utf32_t m_prevChar;
	uint_t m_prevCharFlags;

public:
	ExecEngine(
		ExecEngineKind engineKind,
		StateImpl* parent
	);

	virtual
	~ExecEngine() {}

	ExecEngineKind
	getEngineKind() const {
		return m_engineKind;
	}

	StateImpl*
	getParent() const {
		return m_parent;
	}

	ExecResult
	getExecResult() const {
		return m_execResult;
	}

	bool
	isFinalized() const {
		return m_execResult >= 0;
	}

	enc::DecoderState
	getDecoderState() {
		return m_decoderState;
	}

	uint_t
	getPrevCharFlags() const {
		return m_prevCharFlags ? m_prevCharFlags : calcCharFlags(m_prevChar);
	}

	virtual
	ExecEngine*
	clone(StateImpl* parent) = 0;

	virtual
	void
	exec(
		const void* p,
		size_t size
	) = 0;

	virtual
	void
	eof(bool isLastExec) = 0;

protected:
	void
	initialize(const StateInit& init);

	void
	copy(const ExecEngine* src);

	void
	resetLastExecData();

	static
	uint_t
	calcCharFlags(utf32_t c);

	static
	uint_t
	calcAnchors(
		uint_t anchors,
		uint_t prevCharFlags,
		uint_t charFlags
	);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
ExecEngine::resetLastExecData() {
	m_lastExecData = NULL;
	m_lastExecOffset = m_offset;
	m_lastExecEndOffset = m_offset;
}

inline
uint_t
ExecEngine::calcCharFlags(utf32_t c) {
	return
		c == '\r' ? CharFlag_Cr :
		c == '\n' ? CharFlag_Lf | Anchor_BeginLine :
		enc::isWord(c) ? CharFlag_Word :
		CharFlag_Other;
}

inline
uint_t
ExecEngine::calcAnchors(
	uint_t anchors,
	uint_t prevCharFlags,
	uint_t charFlags
) {
	if (charFlags & (CharFlag_Cr | CharFlag_Lf))
		anchors |= Anchor_EndLine;

	if ((prevCharFlags & CharFlag_Word) ^ (charFlags & CharFlag_Word))
		anchors |= Anchor_WordBoundary;
	else
		anchors |= Anchor_NotWordBoundary;

	return anchors;
}

//..............................................................................

class ExecEngineFactory {
public:
	virtual
	ExecEngine*
	createExecEngine(StateImpl* parent) = 0;
};

//..............................................................................

class ExecNfaEngine: public ExecEngine {
public:
	ExecNfaEngine(StateImpl* parent):
		ExecEngine(ExecEngineKind_Nfa, parent) {
	}

	virtual
	void
	initialize(
		const StateInit& init,
		const NfaState* state
	) = 0;

protected:
	uint_t
	calcAnchorsUpdateCharFlags(utf32_t c);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
uint_t
ExecNfaEngine::calcAnchorsUpdateCharFlags(utf32_t c) {
	ASSERT(m_prevCharFlags);
	uint_t charFlags = calcCharFlags(c);
	uint_t anchors = calcAnchors(m_prevCharFlags, m_prevCharFlags, charFlags);
	m_prevCharFlags = charFlags;
	return anchors;
}

//..............................................................................

template <
	typename T,
	typename Base,
	typename Decoder
>
class ExecImpl: public Base {
public:
	ExecImpl(StateImpl* parent):
		Base(parent) {}

	// ExecEngine

	virtual
	ExecEngine*
	clone(StateImpl* parent) {
		T* exec = AXL_MEM_NEW_ARGS(T, (parent));
		exec->copy(this);
		return exec;
	}

	virtual
	void
	exec(
		const void* p,
		size_t size
	) {
		this->m_lastExecData = p;
		this->m_lastExecOffset = this->m_offset;
		this->m_lastExecEndOffset = this->m_offset + size;
		Decoder::decode(&this->m_decoderState, *static_cast<T*>(this), (char*)p, (char*)p + size);
	}

	// DecodeEmitter

	bool
	canEmit() const {
		return this->m_execResult < 0;
	}

	void
	emitReplacement(
		const char* p,
		utf32_t c
	) {
		static_cast<T*>(this)->emitCodePoint(p, c);
	}
};

//..............................................................................

#define _AXL_RE_TRACE_CAPTURE 0

#if (_AXL_RE_TRACE_CAPTURE)
#	define AXL_RE_TRACE_CAPTURE AXL_TRACE
#else
#	define AXL_RE_TRACE_CAPTURE (void)
#endif

//..............................................................................

} // namespace re
} // namespace axl
