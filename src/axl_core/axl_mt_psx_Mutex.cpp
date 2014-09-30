#include "pch.h"
#include "axl_mt_psx_Mutex.h"
#include "axl_g_Time.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool 
CMutex::lock (uint_t timeout)
{	
	int result;
	
	switch (timeout)
	{
	case 0:
		result = pthread_mutex_trylock (&m_mutex);
		break;
		
	case -1:
		result = pthread_mutex_lock (&m_mutex);		
		break;
		
	default:		
		timespec timespec = { 0 };
		g::getAbsTimespecFromTimeout (timeout, &timespec);
		result = pthread_mutex_timedlock (&m_mutex, &timespec);
	}
			
	return result == 0 ? true : err::fail (result);
}	

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
