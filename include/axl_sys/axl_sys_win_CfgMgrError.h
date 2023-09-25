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

#define _AXL_SYS_WIN_CFGMGR_H

#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

// {8F392936-624E-4EFF-85CA-A49EC07AB041}
AXL_SL_DEFINE_GUID(
	g_cfgMgrErrorGuid,
	0x8f392936, 0x624e, 0x4eff, 0x85, 0xca, 0xa4, 0x9e, 0xc0, 0x7a, 0xb0, 0x41
);

//..............................................................................

class CfgMgrErrorProvider: public err::ErrorProvider {
public:
	static
	sl::StringRef
	getErrorDescription(dword_t code);

	virtual
	sl::StringRef
	getErrorDescription(const err::ErrorRef& error) {
		return getErrorDescription(error->m_code);
	}
};

//..............................................................................

class CfgMgrError: public err::Error {
public:
	CfgMgrError() {}

	CfgMgrError(dword_t code) {
		create(code);
	}

	size_t
	create(dword_t code) {
		return createSimpleError(g_cfgMgrErrorGuid, code);
	}
};

//..............................................................................

inline
size_t
setCfgMgrError(dword_t code) {
	return err::setError(CfgMgrError(code));
}

inline
bool
completeWithCfgMgrError(dword_t code) {
	return !code ? true : fail(CfgMgrError(code));
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
