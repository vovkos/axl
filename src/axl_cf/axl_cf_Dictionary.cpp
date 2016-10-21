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
#include "axl_cf_Dictionary.h"

namespace axl {
namespace cf {

//..............................................................................

bool
MutableDictionaryRef::create (CFDictionaryRef src)
{
	CFMutableDictionaryRef newDictionary = ::CFDictionaryCreateMutableCopy (kCFAllocatorDefault, 0, src);
	if (!newDictionary)
		return err::fail (err::SystemErrorCode_InsufficientResources);

	this->attach (newDictionary);
	return true;
}

//..............................................................................

} // namespace cf
} // namespace axl
