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

#include "axl_re_RegexState.h"

namespace axl {
namespace re {

//..............................................................................

class RegexExecDfa: public RegexExecEngine {
protected:
	const DfaState* m_state;

public:
	RegexExecDfa(RegexStateImpl* parent):
		RegexExecEngine(parent) {
		m_state = NULL;
	}

	virtual
	sl::ArrayRef<RegexMatchPos>
	getCapturePosArray() {
		return sl::ArrayRef<RegexMatchPos>(); // DFA doesn't capture
	}

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
};

//..............................................................................

} // namespace re
} // namespace axl
