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

#define _AXL_SEC_STATICCODE_H

#include "axl_sec_Error.h"
#include "axl_cf_Url.h"

namespace axl {
namespace sec {

//..............................................................................

class StaticCode: public cf::TypeBase<SecStaticCodeRef> {
public:
	StaticCode() {}

	StaticCode(const StaticCode& src):
		cf::TypeBase<SecStaticCodeRef>(src) {}

#if (_AXL_CPP_HAS_RVALUE_REF)
	StaticCode(StaticCode&& src):
		cf::TypeBase<SecStaticCodeRef>(std::move(src)) {}
#endif

	StaticCode(
		SecStaticCodeRef p,
		bool isAttach = false
	):
		cf::TypeBase<SecStaticCodeRef>(p, isAttach) {}

	bool
	createWithPath(
		CFURLRef url,
		SecCSFlags flags = kSecCSDefaultFlags
	);

	bool
	createWithPath(
		const sl::StringRef& path,
		bool isDirectory,
		SecCSFlags flags = kSecCSDefaultFlags
	);

	bool
	copySigningInformation(
		SecCSFlags flags,
		CFDictionaryRef* dictionary
	) {
		OSStatus status = ::SecCodeCopySigningInformation(m_p, flags, dictionary);
		return completeWithOsStatus(status);
	}
};

//..............................................................................

} // namespace sec
} // namespace axl
