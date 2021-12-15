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

#include "axl_re_Nfa.h"

namespace axl {
namespace re {

struct StateImpl;
struct NfaState;

//..............................................................................

enum ExecFlag {
	ExecFlag_Stream          = 0x01, // feed data chunk-by-chunk, then call re::Regex::eof()
	ExecFlag_DisableCapture  = 0x02, // don't capture sub-matches
	ExecFlag_AnchorDataBegin = 0x04, // match must start on the first byte of data
	ExecFlag_AnchorDataEnd   = 0x08, // match must end on the last byte of data
	ExecFlag_ExactMatch      = ExecFlag_AnchorDataBegin | ExecFlag_AnchorDataEnd,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

enum ExecResult {
	ExecResult_Match            = 2,
	ExecResult_MatchOffsetsOnly = 1,
	ExecResult_NoMatch          = 0,
	ExecResult_Continue         = -1, // continue feeding data
	ExecResult_ContinueBackward = -2, // continue feeding backward data
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
		CharFlag_Other = 0x800, // the token of pre-calculated flags
	};

protected:
	StateImpl* m_parent;
	const void* m_lastExecData;
	size_t m_lastExecOffset;
	size_t m_lastExecEndOffset;
	size_t m_offset;
	enc::DecoderState m_decoderState;
	utf32_t m_prevChar;
	uint_t m_prevCharFlags;
	ExecResult m_execResult;

public:
	ExecEngine(StateImpl* parent);

	virtual
	~ExecEngine() {}

	StateImpl*
	getParent() const {
		return m_parent;
	}

	size_t
	getOffset() const {
		return m_offset;
	}

	ExecResult
	getExecResult() const {
		return m_execResult;
	}

	bool
	isFinalized() const {
		return m_execResult >= 0;
	}

	virtual
	ExecEngine*
	clone(StateImpl* parent) = 0;

	virtual
	void
	reset(size_t offset);

	virtual
	void
	exec(
		const void* p,
		size_t size
	) = 0;

	virtual
	void
	eof() = 0;

protected:
	void
	copy(const ExecEngine* src);

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

class ExecNfaEngine: public ExecEngine {
protected:
	uint_t m_execFlags; // cache to save one indirection

public:
	ExecNfaEngine(StateImpl* parent);

	virtual
	void
	reset(
		size_t offset,
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
	typename Encoding
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
		m_lastExecData = p;
		m_lastExecOffset = m_offset;
		m_lastExecEndOffset = m_offset + size;
		Encoding::Decoder::decode(&m_decoderState, *static_cast<T*>(this), (char*)p, (char*)p + size);
	}

	// DecodeEmitter

	bool
	canEmit() const {
		return m_execResult < 0;
	}

	void
	emitReplacement(const char* p) {
		static_cast<T*>(this)->emitCodePoint(p, enc::StdChar_Replacement);
	}
};

//..............................................................................

#define _AXL_RE_TRACE_CAPTURE 0

#if (_AXL_RE_TRACE_CAPTURE)
#	define AXL_RE_TRACE_CAPTURE AXL_TRACE
#else
#	define AXL_RE_TRACE_CAPTURE (void)
#endif

} // namespace re
} // namespace axl
