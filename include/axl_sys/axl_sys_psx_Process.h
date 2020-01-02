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

#define _AXL_SYS_PSX_PROCESS_H

#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace psx {

//..............................................................................

sl::String
getProcessImageName(pid_t pid);

//..............................................................................

} // namespace psx
} // namespace sys
} // namespace axl
