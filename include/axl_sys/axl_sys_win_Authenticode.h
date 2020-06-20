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

#define _AXL_SYS_WIN_AUTHENTICODE_H

#include "axl_sl_Array.h"
#include "axl_sl_String.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
verifyAuthenticodeSignature(
	const sl::StringRef_w& fileName,
	sl::Array<char>* serialNumber,
	sl::String_w* programName,
	sl::String_w* subjectName,
	sl::String_w* issuerName,
	uint64_t* timestamp
	);

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
