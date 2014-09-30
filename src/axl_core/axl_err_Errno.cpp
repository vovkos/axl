#include "pch.h"
#include "axl_err_Errno.h"

namespace axl {
namespace err {

//.............................................................................

ErrorData*
Errno::create (int code)
{
	ErrorData* error = getBuffer (sizeof (ErrorData));
	if (!error)
		return NULL;

	error->m_size = sizeof (ErrorData);
	error->m_guid = GUID_Errno;
	error->m_code = code;
	return error;
}

//.............................................................................

} // namespace err
} // namespace axl
