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
#include "axl_sec_StaticCode.h"
#include "axl_sec_Error.h"

namespace axl {
namespace sec {

//..............................................................................

bool
StaticCode::createWithPath(
	CFURLRef url,
	SecCSFlags flags
	)
{
	clear();
	OSStatus status = ::SecStaticCodeCreateWithPath(url, flags, &m_p);
	return completeWithOsStatus(status);
}

bool
StaticCode::createWithPath(
	const sl::StringRef& path,
	bool isDirectory,
	SecCSFlags flags
	)
{
	cf::Url url;
	url.create(path, kCFURLPOSIXPathStyle, isDirectory);
	return createWithPath(url, flags);
}

//..............................................................................

} // namespace sec
} // namespace axl
