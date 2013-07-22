#include "pch.h"
#include "axl_mt_psx_Mutex.h"
#include "axl_g_Time.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool 
CMutex::Lock (uint_t Timeout)
{	
	int Result;
	
	switch (Timeout)
	{
	case 0:
		Result = pthread_mutex_trylock (&m_Mutex);
		break;
		
	case -1:
		Result = pthread_mutex_lock (&m_Mutex);		
		break;
		
	default:		
		timespec Timespec = { 0 };
		g::GetAbsTimespecFromTimeout (Timeout, &Timespec);
		Result = pthread_mutex_timedlock (&m_Mutex, &Timespec);
	}
			
	return Result == 0 ? true : err::Fail (Result);
}	

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
