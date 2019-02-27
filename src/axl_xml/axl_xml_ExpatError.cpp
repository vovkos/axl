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
#include "axl_xml_ExpatError.h"

namespace axl {
namespace xml {

//..............................................................................

err::ErrorHdr*
ExpatError::create(XML_Error code)
{
	err::ErrorHdr* error = createBuffer(sizeof(err::ErrorHdr));
	if (!error)
		return NULL;

	error->m_size = sizeof(err::ErrorHdr);
	error->m_guid = g_expatErrorGuid;
	error->m_code = code;
	return error;
}

//..............................................................................

} // namespace xml
} // namespace axl
