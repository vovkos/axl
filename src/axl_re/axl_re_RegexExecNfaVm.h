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

class RegexExecNfaVm: public RegexExecEngine {
public:
	RegexExecNfaVm(RegexStateImpl* parent):
		RegexExecEngine(parent) {
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
};

//..............................................................................

} // namespace re
} // namespace axl
