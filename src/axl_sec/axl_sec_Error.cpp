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
#include "axl_sec_Error.h"
#include "axl_cf_String.h"

namespace axl {
namespace sec {

//..............................................................................

sl::StringRef
ErrorProvider::getErrorDescription(OSStatus status)
{
	CFStringRef description = ::SecCopyErrorMessageString(status, NULL);
	sl::String string = cf::String(description, true).getString();
	return !string.isEmpty() ? string : sl::formatString("OSStatus(%d)", status);
}

//..............................................................................

} // namespace sec
} // namespace axl
