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
#include "axl_re_Dfa.h"

namespace axl {
namespace re {

struct DfaState;

//..............................................................................

class ExecDfaBase: public ExecEngine {
protected:
	ExecDfaBase* m_forwardEngine;     // reverse DFA only
	const DfaState* m_state;
	const void* m_p;
	const void* m_matchEnd;
	uint_t m_baseCharFlags;
	size_t m_baseOffset;
	size_t m_matchAcceptId;
	size_t m_matchEndOffset;
	size_t m_savedMatchEndOffset; // reverse DFA only

#if (_AXL_DEBUG)
	size_t m_savedMatchAcceptId;  // reverse DFA only
#endif

public:
	ExecDfaBase(StateImpl* parent);

	size_t
	getDfaStateId() const {
		return m_state->m_id;
	}

	virtual
	void
	reset(
		uint_t prevCharFlags,
		size_t offset,
		uint_t baseCharFlags,
		size_t baseOffset,
		const DfaState* state
	) = 0;

protected:
	void
	copy(const ExecDfaBase* src);

	void
	setForwardEngine(ExecDfaBase* forwardEngine);

	void
	gotoStateImpl(const DfaState* state);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecDfaBase*
createExecDfa(StateImpl* parent);

ExecDfaBase*
createExecDfaReverse(StateImpl* parent);

//..............................................................................

} // namespace re
} // namespace axl
