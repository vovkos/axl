#include "pch.h"
#include "axl_mt_win_JobObject.h"

namespace axl {
namespace mt {
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
} // namespace mt
} // namespace axl
