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

#define _AXL_SYS_WIN_WINCRYPT_H

#include "axl_sl_String.h"
#include "axl_sl_Array.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
cryptQueryObjectFile(
	const sl::StringRef_w& fileName,
	dword_t expectedContentTypeFlags,
	dword_t expectedFormatTypeFlags,
	dword_t flags,
    dword_t* encodingType,
	dword_t* contentType,
	dword_t* formatType,
	HCERTSTORE* certStore,
	HCRYPTMSG* cryptMsg,
	const void** context
);

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

size_t
cryptDecodeObject(
	sl::Array<char>* buffer,
	dword_t encodingType,
	const char* oid,
	const void* p,
	size_t size,
	dword_t flags = 0
);

size_t
cryptDecodeObject(
	void* buffer,
	size_t bufferSize,
	dword_t encodingType,
	const char* oid,
	const void* p,
	size_t size,
	dword_t flags = 0
);

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
