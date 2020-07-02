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
#include "axl_cf_Data.h"
#include "axl_err_Error.h"

namespace axl {
namespace cf {

//..............................................................................

bool
Data::create(CFDataRef src)
{
	CFDataRef data = ::CFDataCreateCopy(kCFAllocatorDefault, src);
	if (!data)
		return err::fail(err::SystemErrorCode_InsufficientResources);

	attach(data);
	return true;
}

bool
Data::create(
	const void* p,
	size_t size
	)
{
	CFDataRef data = ::CFDataCreate(
		kCFAllocatorDefault,
		(const UInt8*)p,
		size
		);

	if (!data)
		return err::fail(err::SystemErrorCode_InsufficientResources);

	attach(data);
	return true;
}

//..............................................................................

} // namespace cf
} // namespace axl
