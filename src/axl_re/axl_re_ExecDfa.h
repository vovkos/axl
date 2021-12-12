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

#include "axl_re_State.h"
#include "axl_re_Exec.h"

namespace axl {
namespace re {

struct DfaState;

//..............................................................................

class ExecDfaBase: public ExecEngine {
protected:
	enum Flag {
		// running the DFA backwards to find the beginning of the match
		Flag_Backward = 0x01,
	};

protected:
	uint_t m_flags;
	const DfaState* m_dfaState;
	const void* m_p;
	const void* m_matchEnd;
	size_t m_baseOffset;
	size_t m_matchAcceptId;
	size_t m_matchEndOffset;
	size_t m_savedMatchEndOffset;
#if (_AXL_DEBUG)
	size_t m_savedMatchAcceptId;
#endif

public:
	ExecDfaBase(StateImpl* parent);

	virtual
	void
	reset(size_t offset);

	virtual
	bool
	eof() {
		processBoundary(m_offset, Anchor_EndLine | Anchor_EndText | Anchor_WordBoundary);
		return finalize(true);
	}

protected:
	void
	copy(const ExecDfaBase* src);

	void
	gotoDfaStateImpl(const DfaState* state);

	void
	gotoDfaState(
		size_t offset,
		const DfaState* state
	);

	void
	gotoDfaState(
		const void* p,
		const DfaState* state
	);

	void
	processBoundary(
		size_t offset,
		uint_t anchors
	);

	bool
	finalize(bool isEof);

	bool
	createMatch(const MatchPos& pos);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecEngine*
createExecDfa(StateImpl* parent);

//..............................................................................

} // namespace re
} // namespace axl
