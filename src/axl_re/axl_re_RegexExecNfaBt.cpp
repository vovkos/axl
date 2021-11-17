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
#include "axl_re_RegexExecNfaBt.h"

namespace axl {
namespace re {

//..............................................................................

void
RegexExecNfaBt::reset() {
}

bool
RegexExecNfaBt::exec(
	const void* p,
	size_t size
) {
	return err::fail("RegexExecNfaBt is not implemented yet");
}

bool
RegexExecNfaBt::eof() {
	return err::fail("RegexExecNfaBt is not implemented yet");
}

//..............................................................................

} // namespace re
} // namespace axl
