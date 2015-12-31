#include "pch.h"
#include "axl_cf_Dictionary.h"

namespace axl {
namespace cf {

//.............................................................................

bool
MutableDictionaryRef::create (CFDictionaryRef src)
{
	CFMutableDictionaryRef newDictionary = ::CFDictionaryCreateMutableCopy (kCFAllocatorDefault, 0, src);
	if (!newDictionary)
		return err::fail (err::SystemErrorCode_InsufficientResources);

	this->attach (newDictionary);
	return true;
}

//.............................................................................

} // namespace cf
} // namespace axl
