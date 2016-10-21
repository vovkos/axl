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
#include "axl_io_UsbError.h"

namespace axl {
namespace io {

//..............................................................................

err::ErrorHdr*
UsbError::create (int code)
{
	err::ErrorHdr* error = createBuffer (sizeof (err::ErrorHdr));
	if (!error)
		return NULL;

	error->m_size = sizeof (err::ErrorHdr);
	error->m_guid = g_usbErrorGuid;
	error->m_code = code;
	return error;
}

//..............................................................................

} // namespace io
} // namespace axl
