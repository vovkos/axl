#include "pch.h"
#include "axl_mt_psx_Cond.h"
#include "axl_g_Time.h"

#include "axl_mt_Event.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool 
CCond::Wait (
	pthread_mutex_t* pMutex,
	uint_t Timeout
	)
{	
	int Result;
	
	switch (Timeout)
	{
	case -1:
		Result = pthread_cond_wait (&m_Cond, pMutex);		
		break;
		
	default:		
		timespec Timespec = { 0 };
		g::GetAbsTimespecFromTimeout (Timeout, &Timespec);
		Result = pthread_cond_timedwait (&m_Cond, pMutex, &Timespec);
	}		

	return Result == 0 ? true : err::Fail (Result);
}	

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
