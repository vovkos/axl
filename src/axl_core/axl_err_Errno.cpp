#include "pch.h"
#include "axl_err_Errno.h"

namespace axl {
namespace err {

//.............................................................................

TError*
CErrno::create (int code)
{
	TError* error = getBuffer (sizeof (TError));
	if (!error)
		return NULL;

	error->m_size = sizeof (TError);
	error->m_guid = GUID_Errno;
	error->m_code = code;
	return error;
}

//.............................................................................

} // namespace err
} // namespace axl
