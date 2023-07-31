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
	ExecDfaBase* m_prevEngine;    // reverse DFA only
	const DfaState* m_state;
	const void* m_p;
	const void* m_matchEnd;

	uint64_t m_baseOffset;
	uint64_t m_matchEndOffset;
	uint64_t m_savedMatchEndOffset; // reverse DFA only

	uint_t m_baseCharFlags;
	size_t m_matchAcceptId;
	size_t m_savedMatchAcceptId;  // reverse DFA only

public:
	ExecDfaBase(StateImpl* parent);

	virtual ~ExecDfaBase() {
		if (m_prevEngine)
			delete m_prevEngine;
	}

	size_t
	getDfaStateId() const {
		return m_state->m_id;
	}

	virtual
	void
	initialize(
		const StateInit& init,
		const DfaState* state
	) = 0;

protected:
	void
	copy(const ExecDfaBase* src);

	static
	uint_t
	calcReverseCharFlags(utf32_t c);

	static
	uint_t
	calcReverseAnchors(
		uint_t anchors,
		uint_t prevCharFlags,
		uint_t charFlags
	);

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
