#include "pch.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//.............................................................................

err::ErrorData*
UsbError::create (int code)
{
	err::ErrorData* error = getBuffer (sizeof (err::ErrorData));
	if (!error)
		return NULL;

	error->m_size = sizeof (err::ErrorData);
	error->m_guid = GUID_UsbError;
	error->m_code = code;
	return error;
}

//.............................................................................

} // namespace io
} // namespace axl
