#include "pch.h"
#include "axl_mt_psx_Cond.h"
#include "axl_g_Time.h"

#include "axl_mt_Event.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool 
CCond::wait (
	pthread_mutex_t* mutex,
	uint_t timeout
	)
{	
	int result;
	
	switch (timeout)
	{
	case -1:
		result = pthread_cond_wait (&m_cond, mutex);		
		break;
		
	default:		
		timespec timespec = { 0 };
		g::getAbsTimespecFromTimeout (timeout, &timespec);
		result = pthread_cond_timedwait (&m_cond, mutex, &timespec);
	}		

	return result == 0 ? true : err::fail (result);
}	

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
