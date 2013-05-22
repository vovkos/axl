#include "pch.h"
#include "axl_log_VolatilePacketFilter.h"

namespace axl {
namespace log {

//.............................................................................

bool 
CVolatilePacketFilterStack::Filter (
	uint32_t PacketCode, 
	const void* p, 
	size_t Size, 
	uint_t* pVolatileFlags
	)
{
	size_t Count = GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		IVolatilePacketFilter* pFilter = m_p [i];
		bool Result = pFilter->Filter (PacketCode, p, Size, pVolatileFlags);
		if (Result)
			return true;
	}

	return false;
}

//.............................................................................

} // namespace log 
} // namespace axl
