#include "pch.h"
#include "axl_jnc_TlsMgr.h"
#include "axl_mt_Thread.h"
#include "axl_mt_TlsMgr.h"

namespace axl {
namespace jnc {

//.............................................................................

CTlsMgr::CDirectory::~CDirectory ()
{
	size_t Count = m_Table.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		mem::TBlock* pBlock = &m_Table [i];
		if (pBlock->m_p)
			AXL_MEM_FREE (pBlock->m_p);
	}
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

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

	m_SlotMap.SetBit (Slot, true);
	return Slot;
}

void
CTlsMgr::DestroySlot (size_t Slot)
{
	ASSERT (mt::GetCurrentThreadId () == m_MainThreadId);
	ASSERT (m_SlotMap.GetBit (Slot));

	m_SlotMap.SetBit (Slot, false);
}

void*
CTlsMgr::GetThreadVariableData (
	size_t Slot,
	size_t Size
	)
{
	CDirectory* pDirectory = (CDirectory*) mt::GetTlsMgr ()->GetSlotValue (m_TlsSlot).p ();
	if (!pDirectory)
	{
		pDirectory = AXL_REF_NEW (CDirectory);
		mt::GetTlsMgr ()->SetSlotValue (m_TlsSlot, pDirectory);
	}	

	if (Slot >= pDirectory->m_Table.GetCount ())
		pDirectory->m_Table.SetCount (Slot + 1);

	mem::TBlock* pBlock = &pDirectory->m_Table [Slot];
	ASSERT (!pBlock->m_p || pBlock->m_Size == Size);

	if (!pBlock->m_p)
	{
		pBlock->m_p = AXL_MEM_ALLOC (Size);
		pBlock->m_Size = Size;
	}

	return pBlock->m_p;
}

//.............................................................................

} // namespace axl 
} // namespace jnc 
