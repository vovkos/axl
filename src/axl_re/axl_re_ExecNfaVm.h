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

//..............................................................................

class ExecNfaVm: public ExecEngine {
public:
	ExecNfaVm(StateImpl* parent):
		ExecEngine(parent) {
	}

	virtual
	ExecEngine*
	clone(StateImpl* parent);

	virtual
	void
	reset(size_t offset);

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

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

ExecEngine*
createExecNfaVm(
	StateImpl* parent,
	enc::CharCodecKind codecKind
);

//..............................................................................

} // namespace re
} // namespace axl
