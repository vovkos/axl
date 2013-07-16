#include "pch.h"
#include "axl_jnc_TlsMgr.h"
#include "axl_jnc_Runtime.h"
#include "axl_mt_Thread.h"
#include "axl_mt_TlsMgr.h"

namespace axl {
namespace jnc {

//.............................................................................

CTlsDirectory::~CTlsDirectory ()
{
	size_t Count = m_Table.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		TTlsData* pTlsData = m_Table [i];
		if (pTlsData)
			pTlsData->m_pRuntime->DestroyTlsData (pTlsData);
	}
}

TTlsData* 
CTlsDirectory::FindTlsData (CRuntime* pRuntime)
{
	size_t Slot = pRuntime->GetTlsSlot ();
	return Slot < m_Table.GetCount () ? m_Table [Slot] : NULL;
}

TTlsData* 
CTlsDirectory::GetTlsData (CRuntime* pRuntime)
{
	size_t Slot = pRuntime->GetTlsSlot ();

	if (Slot >= m_Table.GetCount ())
		m_Table.SetCount (Slot + 1);

	TTlsData* pTlsData = m_Table [Slot];
	if (pTlsData)
	{
		ASSERT (pTlsData->m_pRuntime == pRuntime);
		return pTlsData;
	}

	pTlsData = pRuntime->CreateTlsData ();
	m_Table [Slot] = pTlsData;
	return pTlsData;
}

TTlsData*
CTlsDirectory::NullifyTlsData (CRuntime* pRuntime)
{
	size_t Slot = pRuntime->GetTlsSlot ();
	if (Slot >= m_Table.GetCount ()) 
		return NULL;
	
	TTlsData* pTlsData = m_Table [Slot];
	m_Table [Slot] = NULL;
	return pTlsData;
}

//.............................................................................

CTlsMgr::CTlsMgr ()
{
	m_MainThreadId = mt::GetCurrentThreadId ();
	m_TlsSlot = mt::GetTlsMgr ()->CreateSlot ();
	m_SlotCount = 0;
}

size_t 
CTlsMgr::CreateSlot ()
{
	ASSERT (mt::GetCurrentThreadId () == m_MainThreadId);

	size_t Slot;

	if (m_SlotCount < m_SlotMap.GetPageCount () * _AXL_PTR_BITNESS)
	{
		Slot = m_SlotMap.FindBit (0, false);
		ASSERT (Slot != -1);
	}
	else
	{
		Slot = m_SlotCount;
	}

	m_SlotMap.SetBitResize (Slot, true);
	return Slot;
}

void
CTlsMgr::DestroySlot (size_t Slot)
{
	ASSERT (mt::GetCurrentThreadId () == m_MainThreadId);
	ASSERT (m_SlotMap.GetBit (Slot));

	m_SlotMap.SetBit (Slot, false);
}

TTlsData*
CTlsMgr::FindTlsData (CRuntime* pRuntime)
{
	CTlsDirectory* pDirectory = (CTlsDirectory*) mt::GetTlsMgr ()->GetSlotValue (m_TlsSlot).p ();
	return pDirectory ? pDirectory->FindTlsData (pRuntime) : NULL;
}

TTlsData*
CTlsMgr::GetTlsData (CRuntime* pRuntime)
{
	CTlsDirectory* pDirectory = (CTlsDirectory*) mt::GetTlsMgr ()->GetSlotValue (m_TlsSlot).p ();
	if (pDirectory)
		return pDirectory->GetTlsData (pRuntime);

	ref::CPtrT <CTlsDirectory> Directory = AXL_REF_NEW (CTlsDirectory);
	mt::GetTlsMgr ()->SetSlotValue (m_TlsSlot, Directory);		
	pDirectory = Directory;

	return pDirectory->GetTlsData (pRuntime);
}

TTlsData*
CTlsMgr::NullifyTlsData (CRuntime* pRuntime)
{
	CTlsDirectory* pDirectory = (CTlsDirectory*) mt::GetTlsMgr ()->GetSlotValue (m_TlsSlot).p ();
	return pDirectory ? pDirectory->NullifyTlsData (pRuntime) : NULL;
}

//.............................................................................

} // namespace axl 
} // namespace jnc 
