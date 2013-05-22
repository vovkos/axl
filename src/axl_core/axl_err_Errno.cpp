#include "pch.h"
#include "axl_err_Errno.h"

namespace axl {
namespace err {

//.............................................................................

TError*
CErrno::Create (int Error)
{
	TError* pError = GetBuffer (sizeof (TError));
	if (!pError)
		return NULL;

	pError->m_Size = sizeof (TError);
	pError->m_Guid = GUID_Errno;
	pError->m_Code = Error;
	return pError;
}

//.............................................................................

} // namespace err
} // namespace axl
