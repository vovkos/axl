#include "pch.h"
#include "axl_xml_ExpatError.h"

namespace axl {
namespace xml {

//..............................................................................

err::ErrorHdr*
ExpatError::create (XML_Error code)
{
	err::ErrorHdr* error = createBuffer (sizeof (err::ErrorHdr));
	if (!error)
		return NULL;

	error->m_size = sizeof (err::ErrorHdr);
	error->m_guid = g_expatErrorGuid;
	error->m_code = code;
	return error;
}

//..............................................................................

} // namespace xml
} // namespace axl
