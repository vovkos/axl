#include "pch.h"
#include "axl_cf_Number.h"
#include "axl_err_Error.h"

namespace axl {
namespace cf {

//.............................................................................

bool
NumberRef::create (
	CFNumberType type,
	const void* p
	)
{
	CFNumberRef newNumber = ::CFNumberCreate (kCFAllocatorDefault, type, p);
	if (!newNumber)
		return err::fail (err::SystemErrorCode_InsufficientResources);

	attach (newNumber);
	return true;
}

//.............................................................................

} // namespace cf
} // namespace axl
