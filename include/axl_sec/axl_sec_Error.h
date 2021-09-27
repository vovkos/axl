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

#define _AXL_SEC_ERROR_H

#include "axl_sec_Pch.h"
#include "axl_err_ErrorMgr.h"

namespace axl {
namespace sec {

//..............................................................................

// {9a5e82f3-da70-4177-a3b9-0030a23b90dc}
AXL_SL_DEFINE_GUID(
	g_errorGuid,
	0x9a5e82f3, 0xda70, 0x4177, 0xa3, 0xb9, 0x00, 0x30, 0xa2, 0x3b, 0x90, 0xdc
);

//..............................................................................

class ErrorProvider: public err::ErrorProvider {
public:
	static
	sl::StringRef
	getErrorDescription(OSStatus code);

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error) {
		return getErrorDescription(error->m_code);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
void
registerErrorProvider() {
	err::getErrorMgr()->registerProvider(
		g_errorGuid,
		sl::getSimpleSingleton<ErrorProvider> ()
	);
}

//..............................................................................

class Error: public err::Error {
public:
	Error() {}

	Error(OSStatus status) {
		create(status);
	}

	size_t
	create(OSStatus status) {
		return createSimpleError(g_errorGuid, status);
	}
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
bool
completeWithOsStatus(OSStatus status) {
	return status == errSecSuccess ? true : err::fail(Error(status));
}

//..............................................................................

} // namespace sec
} // namespace axl
