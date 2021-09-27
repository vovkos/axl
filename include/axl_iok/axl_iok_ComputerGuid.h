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

#define _AXL_IOK_COMPUTERGUID_H

#include "axl_sl_Array.h"

namespace axl {
namespace iok {

//..............................................................................

// Mac uses MAC-address of en0 as GUID for app receipt validation. While it's
// natural to assume MAC-address is always 6 bytes, we are using precisely the
// same algorithm as shown in the "Validating Receipts Locally" chapter of the
// Apple Developer Guide. It handles any length for MAC, so should we.

size_t
getComputerGuid(sl::Array<char>* guid);

inline
sl::Array<char>
getComputerGuid() {
	sl::Array<char> guid;
	getComputerGuid(&guid);
	return guid;
}

//..............................................................................

} // namespace iok
} // namespace axl
