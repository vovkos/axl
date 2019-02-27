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
#include "axl_cf_Number.h"
#include "axl_err_Error.h"

namespace axl {
namespace cf {

//..............................................................................

bool
NumberRef::create(
	CFNumberType type,
	const void* p
	)
{
	CFNumberRef newNumber = ::CFNumberCreate(kCFAllocatorDefault, type, p);
	if (!newNumber)
		return err::fail(err::SystemErrorCode_InsufficientResources);

	attach(newNumber);
	return true;
}

//..............................................................................

} // namespace cf
} // namespace axl
