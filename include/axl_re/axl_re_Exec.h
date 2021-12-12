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
		CharFlag_Cr    = 0x01,
		CharFlag_Lf    = 0x02,
		CharFlag_Nl    = CharFlag_Lf,
		CharFlag_Word  = 0x04,
		CharFlag_Other = 0x08, // non-zero
		CharFlag_ForcedWordBoundary = 0x10, // enforce word boundary at the 1st char
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

	bool
	getExecResult() const {
		return m_execResult != ExecResult_False; // true or undefined
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
		uint_t prevCharFlags,
		uint_t charFlags
	);

	uint_t
	calcAnchorsUpdateCharFlags(utf32_t c);

	uint_t
	calcReverseAnchorsUpdateCharFlags(utf32_t c);
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
		c == '\n' ? CharFlag_Lf :
		c == '_' || enc::isLetterOrDigit(c) ? CharFlag_Word :
		CharFlag_Other;
}

inline
uint_t
ExecEngine::calcAnchors(
	uint_t prevCharFlags,
	uint_t charFlags
) {
	uint_t anchors = 0;

	if (prevCharFlags & CharFlag_Lf)
		anchors |= Anchor_BeginLine;

	if (charFlags & (CharFlag_Cr | CharFlag_Lf))
		anchors |= Anchor_EndLine;

	if ((prevCharFlags & CharFlag_Word) ^ (charFlags & CharFlag_Word))
		anchors |= Anchor_WordBoundary;
	else
		anchors |= Anchor_NotWordBoundary;

	return anchors;
}

inline
uint_t
ExecEngine::calcAnchorsUpdateCharFlags(utf32_t c) {
	uint_t charFlags = calcCharFlags(c);
	uint_t anchors = m_prevCharFlags ?
		calcAnchors(m_prevCharFlags, charFlags) :
		calcAnchors(calcCharFlags(m_prevChar), calcCharFlags(c));

	m_prevCharFlags = charFlags;
	return anchors;
}

inline
uint_t
ExecEngine::calcReverseAnchorsUpdateCharFlags(utf32_t c) {
	uint_t charFlags = calcCharFlags(c);
	uint_t anchors = m_prevCharFlags ?
		calcAnchors(charFlags, m_prevCharFlags) :
		calcAnchors(calcCharFlags(c), calcCharFlags(m_prevChar));

	m_prevCharFlags = charFlags;
	return anchors;
}

//..............................................................................

class ExecNfaEngine: public ExecEngine {
public:
	ExecNfaEngine(StateImpl* parent):
		ExecEngine(parent) {}

	virtual
	void
	reset(
		size_t offset,
		const NfaState* state
	) = 0;
};

//..............................................................................

} // namespace re
} // namespace axl
