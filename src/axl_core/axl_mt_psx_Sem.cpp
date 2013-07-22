#include "pch.h"
#include "axl_mt_psx_Sem.h"
#include "axl_g_Time.h"

namespace axl {
namespace mt {
namespace psx {

//.............................................................................

bool 
CSem::Wait (uint_t Timeout)
{	
	int Result;
	
	switch (Timeout)
	{
	case 0:
		Result = sem_wait (&m_Sem);
		break;
		
	case -1:
		Result = sem_trywait (&m_Sem);		
		break;
		
	default:		
		timespec Timespec = { 0 };
		g::GetAbsTimespecFromTimeout (Timeout, &Timespec);
		Result = sem_timedwait (&m_Sem, &Timespec);
	}		

	return Result == 0 ? true : err::Fail (Result);
}	

//.............................................................................

} // namespace psx
} // namespace mt
} // namespace axl
