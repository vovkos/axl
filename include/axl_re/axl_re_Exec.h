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

class ExecEngine {
protected:
	enum ExecResult {
		ExecResult_Undefined = -1,
		ExecResult_False     = false,
		ExecResult_True      = true,
	};

	enum CharFlag {
		// can be combined with Anchor-s, so make sure these don't overlap

		CharFlag_Cr    = 0x100,
		CharFlag_Lf    = 0x200,
		CharFlag_Nl    = CharFlag_Lf,
		CharFlag_Word  = 0x400,
		CharFlag_Other = 0x800, // token of pre-calculated flags
	};

protected:
	StateImpl* m_parent;
	const void* m_lastExecData;
	size_t m_lastExecOffset;
	size_t m_lastExecEndOffset;
	ExecResult m_execResult;
	enc::DecoderState m_decoderState;

	utf32_t m_prevChar;
	uint_t m_prevCharFlags;
	size_t m_offset;

public:
	ExecEngine(StateImpl* parent);

	virtual
	~ExecEngine() {}

	StateImpl*
	getParent() const {
		return m_parent;
	}

	bool
	getExecResult() const {
		return m_execResult != ExecResult_False; // true or undefined
	}

	bool
	isFinalized() const {
		return m_execResult != ExecResult_Undefined;
	}

	void
	preExec(
		const void* p,
		size_t size
	);

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
	bool
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
void
ExecEngine::preExec(
	const void* p,
	size_t size
) {
	m_execResult = ExecResult_Undefined;
	m_lastExecData = p;
	m_lastExecOffset = m_offset;
	m_lastExecEndOffset = m_offset + size;
}

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
		Encoding::Decoder::decode(&m_decoderState, *static_cast<T*>(this), (char*)p, (char*)p + size);
	}

	// DecodeEmitter

	bool
	canEmit() const {
		return m_execResult == ExecResult_Undefined;
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
