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
#include "axl_cf_String.h"
#include "axl_err_Error.h"

namespace axl {
namespace cf {

//..............................................................................

bool
String::create(CFStringRef src)
{
	CFStringRef newString = ::CFStringCreateCopy(kCFAllocatorDefault, src);
	if (!newString)
		return err::fail(err::SystemErrorCode_InsufficientResources);

	attach(newString);
	return true;
}

bool
String::create(
	const char* p,
	size_t length
	)
{
	if (length == -1)
		length = strlen_s(p);

	CFStringRef newString = ::CFStringCreateWithBytes(
		kCFAllocatorDefault,
		(const UInt8*)p,
		length,
		kCFStringEncodingUTF8,
		false
		);

	if (!newString)
		return err::fail(err::SystemErrorCode_InsufficientResources);

	attach(newString);
	return true;
}

//..............................................................................

} // namespace cf
} // namespace axl
