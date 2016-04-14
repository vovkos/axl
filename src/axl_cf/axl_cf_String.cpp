#include "pch.h"
#include "axl_cf_String.h"
#include "axl_err_Error.h"

namespace axl {
namespace cf {

//.............................................................................

bool
StringRef::create (CFStringRef src)
{
	CFStringRef newString = ::CFStringCreateCopy (kCFAllocatorDefault, src);
	if (!newString)
		return err::fail (err::SystemErrorCode_InsufficientResources);

	attach (newString);
	return true;
}

bool
StringRef::create (
	const char* p,
	size_t length
	)
{
	if (length == -1)
		length = axl_strlen (p);

	CFStringRef newString = ::CFStringCreateWithBytes (
		kCFAllocatorDefault,
	   (const UInt8*) p,
	   length,
	   kCFStringEncodingUTF8,
	   false
	   );

	if (!newString)
		return err::fail (err::SystemErrorCode_InsufficientResources);

	attach (newString);
	return true;
}

//.............................................................................

} // namespace cf
} // namespace axl
