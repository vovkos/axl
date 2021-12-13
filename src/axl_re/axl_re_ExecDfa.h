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
	const DfaState* m_state;
	const void* m_p;
	const void* m_matchEnd;
	size_t m_baseOffset;
	size_t m_matchAcceptId;
	size_t m_matchEndOffset;

	union {
		uint_t m_matchCharFlags;      // forward DFA only
		size_t m_savedMatchEndOffset; // reverse DFA only
	};

#if (_AXL_DEBUG)
	size_t m_savedMatchAcceptId;      // reverse DFA only
#endif

public:
	ExecDfaBase(StateImpl* parent);

protected:
	void
	copy(const ExecDfaBase* src);

	void
	copyForReverse(const ExecDfaBase* src);

	void
	gotoStateImpl(const DfaState* state);
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecEngine*
createExecDfa(StateImpl* parent);

//..............................................................................

} // namespace re
} // namespace axl
