#include "pch.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//.............................................................................

err::TError*
CUsbError::create (int code)
{
	err::TError* error = getBuffer (sizeof (err::TError));
	if (!error)
		return NULL;

	error->m_size = sizeof (err::TError);
	error->m_guid = GUID_UsbError;
	error->m_code = code;
	return error;
}

//.............................................................................

} // namespace io
} // namespace axl
