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

#define _AXL_SYS_WIN_WINTRUST_H

#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
verifyTrustFile(const sl::StringRef_w& fileName);

bool
verifyTrustFile(const sl::StringRef& fileName);

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
