#include "pch.h"
#include "axl_jnc_Multicast.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CMulticast::Clear ()
{
	m_Count = 0;
	if (m_pHandleTable)
		((rtl::CHandleTableT <size_t>*) m_pHandleTable)->Clear ();
}

handle_t
CMulticast::SetHandler (jnc::TFunctionPtr Ptr)
{
	if (Ptr.m_pf)
		return SetHandlerImpl (Ptr);

	Clear ();
	return NULL;
}

handle_t
CMulticast::SetHandler_w (jnc::TFunctionPtr_w Ptr)
{
	if (Ptr.m_pf)
		return SetHandlerImpl (Ptr);

	Clear ();
	return NULL;
}

handle_t
CMulticast::SetHandler_u (void* pf)
{
	if (pf)
		return SetHandlerImpl (pf);

	Clear ();
	return NULL;
}

rtl::CHandleTableT <size_t>*
CMulticast::GetHandleTable ()
{
	if (m_pHandleTable)
		return (rtl::CHandleTableT <size_t>*) m_pHandleTable;

	rtl::CHandleTableT <size_t>* pHandleTable = AXL_MEM_NEW (rtl::CHandleTableT <size_t>);
	m_pHandleTable = pHandleTable;
	return pHandleTable;
}

bool
CMulticast::SetCount (
	size_t Count,
	size_t PtrSize
	)
{
	if (m_MaxCount >= Count)
	{
		m_Count = Count;
		return true;		
	}

	size_t MaxCount = rtl::GetMinPower2Ge (Count);
	
	void* p = AXL_MEM_ALLOC (MaxCount * PtrSize);
	if (!p)
		return false;

	if (m_pPtrArray)
		memcpy (p, m_pPtrArray, m_Count * PtrSize);

	m_pPtrArray = p;
	m_MaxCount = MaxCount;
	m_Count = Count;
	return true;
}

//.............................................................................

} // namespace axl
} // namespace jnc
