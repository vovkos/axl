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
#include "axl_sys_win_Bstr.h"
#include "axl_err_Error.h"

namespace axl {
namespace sys {
namespace win {

//..............................................................................

bool
Bstr::copy(
	BSTR p,
	size_t length
) {
	close();

	m_h = ::SysAllocStringLen(p, length);
	return m_h ? true : err::fail(err::SystemErrorCode_InsufficientResources);
}

//..............................................................................

} // namespace win
} // namespace sys
} // namespace axl
