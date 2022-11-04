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

#include "axl_sl_String.h"

namespace axl {
namespace io {

//..............................................................................

ushort_t
chooseUsbStringDescriptorLanguage(
	const sl::String_utf16& string,
	uint_t preferredLangId = 0x09 // English (pass 0 to pick the very first)
);

//..............................................................................

} // namespace io
} // namespace axl
