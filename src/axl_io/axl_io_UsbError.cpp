#include "pch.h"
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//.............................................................................

err::ErrorHdr*
UsbError::create (int code)
{
	err::ErrorHdr* error = getBuffer (sizeof (err::ErrorHdr));
	if (!error)
		return NULL;

	error->m_size = sizeof (err::ErrorHdr);
	error->m_guid = g_usbErrorGuid;
	error->m_code = code;
	return error;
}

//.............................................................................

} // namespace io
} // namespace axl
