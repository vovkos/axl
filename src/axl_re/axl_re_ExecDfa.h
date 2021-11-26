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

class ExecDfa: public ExecEngine {
protected:
	const DfaState* m_state;
	size_t m_matchEndOffset;
	size_t m_matchAcceptId;

public:
	ExecDfa(StateImpl* parent);

	virtual
	void
	reset();

	virtual
	bool
	exec(
		const void* p,
		size_t size
	);

	virtual
	bool
	eof();

protected:
	void
	gotoState(const DfaState* state);

	bool
	finalize(bool isEof);
};

//..............................................................................

} // namespace re
} // namespace axl
