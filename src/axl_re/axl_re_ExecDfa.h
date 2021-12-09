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

namespace axl {
namespace re {

struct DfaState;

//..............................................................................

class ExecDfaBase: public ExecEngine {
protected:
	enum Direction {
		Direction_Forward,
		Direction_Backward,
	};

protected:
	Direction m_direction;
	const DfaState* m_dfaState;
	const void* m_matchEnd;
	size_t m_matchAcceptId;
	size_t m_matchEndOffset;
	union {
		size_t m_matchBeginOffset;
		size_t m_savedMatchEndOffset;
	};

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
	eof();

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

	bool
	finalize(bool isEof);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecEngine*
createExecDfa(
	StateImpl* parent,
	enc::CharCodecKind codecKind
);

//..............................................................................

} // namespace re
} // namespace axl
