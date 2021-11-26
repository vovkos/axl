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

#include "pch.h"
#include "axl_re_ExecNfaVm.h"

namespace axl {
namespace re {

//..............................................................................

void
ExecNfaVm::reset() {
}

bool
ExecNfaVm::exec(
	const void* p,
	size_t size
) {
	return true;
}

bool
ExecNfaVm::eof() {
	return true;
}


//..............................................................................

} // namespace re
} // namespace axl
