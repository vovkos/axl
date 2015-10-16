#include "pch.h"
#include "axl_sys_win_JobObject.h"

namespace axl {
namespace sys {
namespace win {

//.............................................................................

size_t 
JobObject::queryInformation (
	JOBOBJECTINFOCLASS infoClass,
	void* p,
	size_t size
	)
{
	dword_t resultSize;
	bool_t result = ::QueryInformationJobObject (m_h, infoClass, p,  (dword_t) size, &resultSize);
	if (!result)
	{
		err::setLastSystemError ();
		return -1;
	}

	return resultSize;
}

//.............................................................................

} // namespace win
} // namespace sys
} // namespace axl
