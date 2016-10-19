#include "pch.h"
#include "axl_sys_drw_MachError.h"

namespace axl {
namespace sys {
namespace drw {

//..............................................................................

err::ErrorHdr*
MachError::create (mach_error_t code)
{
	err::ErrorHdr* error = createBuffer (sizeof (err::ErrorHdr));
	if (!error)
		return NULL;

	error->m_size = sizeof (err::ErrorHdr);
	error->m_guid = g_MachErrorGuid;
	error->m_code = code;
	return error;
}

//..............................................................................

} // namespace drw
} // namespace err
} // namespace axl
