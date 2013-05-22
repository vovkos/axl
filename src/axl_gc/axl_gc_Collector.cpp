#include "pch.h"
#include "axl_gc_Collector.h"
#include "axl_err_Error.h"

namespace axl {
namespace gc {

//.............................................................................

// helper

bool
AppendValueListToArray (
	rtl::CStdListT <axl_gc_TValueEntry>* pList,
	rtl::CArrayT <axl_gc_TValue>* pArray
	)
{
	if (pList->IsEmpty ())
		return true;

	size_t Count = pArray->GetCount ();
	if (!pArray->SetCount (Count + pList->GetCount ()))
		return false;

	rtl::CIteratorT <axl_gc_TValueEntry> It = pList->GetHead ();
	axl_gc_TValue* pValue = *pArray + Count;

	for (; It; It++, pValue++)
		*pValue = It->m_Value;

	return true;
}

//.............................................................................

CCollector::CCollector ():
	m_IdleEvent (mt::CEvent::EKind_Notification)
{
	m_pMutator = NULL;

	m_State = EState_Idle;
	m_TerminateThread = false;

	m_IdleEvent.Signal ();

	m_AllocateMap = 0;
	m_GreenSlotTls = -1;
	m_CurrentTraceSet = 0;

	m_pPool = NULL;
	m_PoolSize = 0;
	m_Trigger = 0;

#ifdef _DEBUG
	m_CycleCount = 0;
	m_GreenHitCount = 0;
	m_SecondChanceCount = 0;
#endif
}

bool
CCollector::IsInPool (const axl_gc_TValue* pValue)
{
	void* pPoolEnd = (uchar_t*) m_pPool + m_PoolSize;
	if (pValue->m_p < m_pPool || pValue->m_p >= pPoolEnd)
		return false;

	ASSERT ((uchar_t*) pValue->m_p + pValue->m_Type.m_Size <= (uchar_t*) pPoolEnd);
	return true;
}

size_t
CCollector::GetClusterAddress (const axl_gc_TValue* pValue)
{
	ASSERT (IsInPool (pValue));
	ASSERT (!(((uchar_t*) pValue->m_p - (uchar_t*) m_pPool) & (EDef_ClusterSize - 1)));
	return ((uchar_t*) pValue->m_p - (uchar_t*) m_pPool) / EDef_ClusterSize;
}

size_t
CCollector::GetClusterCount (const axl_gc_TValue* pValue)
{
	return (pValue->m_Type.m_Size + EDef_ClusterSize - 1) / EDef_ClusterSize;
}

bool
CCollector::Create (
	IMutator* pMutator,
	size_t Width,
	size_t Height,
	uint_t TriggerPercent
	)
{
	bool_t Result;
	size_t ClusterCount;

	Close ();

	m_GreenSlotTls = ::TlsAlloc ();
	if (m_GreenSlotTls == -1)
		return err::FailWithLastSystemError ();

	Result = 
		m_MapArray [0].Create (Width, Height) &&
		m_MapArray [1].Create (Width, Height);
	
	if (!Result)
		return false;

	ClusterCount = m_MapArray [0].GetTotalSize ();

	Result = m_MutatorGrayMap.Create (ClusterCount);
	if (!Result)
		return false;

	m_PoolSize = ClusterCount * EDef_ClusterSize;
	m_pPool = AXL_MEM_ALLOC (m_PoolSize);
	if (!m_pPool)
		return false;

	m_pMutator = pMutator;
	m_AllocateMap = 0;
	m_Trigger = MulDiv ((int) ClusterCount, TriggerPercent, 100);
	m_State = EState_Idle;
	m_TerminateThread = false;

#ifdef _DEBUG
	m_CycleCount = 0;
	m_GreenHitCount = 0;
	m_SecondChanceCount = 0;
#endif

	m_IdleEvent.Signal ();
	m_WakeUpEvent.Reset ();

	return Start ();
}

void
CCollector::Close ()
{
	m_TerminateThread = true;
	m_WakeUpEvent.Signal ();
	WaitAndClose ();

	// should be empty...

	ASSERT (m_DestructList.IsEmpty ()); 
	ASSERT (m_GreenList.IsEmpty ());
	ASSERT (m_MutatorGrayList.IsEmpty ());

	// ...but clean up anyway

	m_ValueFreeList.InsertListTail (&m_GreenList);
	m_ValueFreeList.InsertListTail (&m_MutatorGrayList);
	m_ValueFreeList.InsertListTail (&m_DestructList);

	ASSERT (m_MapArray [m_AllocateMap].IsEmpty ()); // should be...
	
	if (m_pPool && m_MapArray [m_AllocateMap].IsEmpty ()) // ...but better to leak than to crash
		AXL_MEM_FREE (m_pPool);

	m_pPool = NULL;
	m_PoolSize = 0;

	m_MapArray [0].Close ();
	m_MapArray [1].Close ();

	if (m_GreenSlotTls != -1)
		::TlsFree (m_GreenSlotTls);
}

axl_gc_TValueEntry* 
CCollector::AllocateValueEntry ()
{
	return m_ValueFreeList.IsEmpty () ? 
		m_ValueFreeList.RemoveHead () :
		AXL_MEM_NEW (axl_gc_TValueEntry);
}

void
CCollector::StartCycle_l () 
{
	ASSERT (m_State != EState_Mark);
	m_State = EState_Mark;
	m_WakeUpEvent.Signal ();
	m_IdleEvent.Reset ();
}

void*
CCollector::TryAllocate_l (const axl_gc_TType* pType)
{
	axl_gc_TValue Value;
	
	size_t Address;
	size_t ClusterCount = (pType->m_Size + EDef_ClusterSize - 1) / EDef_ClusterSize;

	Address = m_MapArray [m_AllocateMap].Allocate (ClusterCount);
	if (Address == -1)
	{
		// failed allocation triggers a new collection cycle (if not yet)

		if (m_State != EState_Mark)
			StartCycle_l ();

		return NULL;
	}

	Value.m_p = (uchar_t*) m_pPool + Address * EDef_ClusterSize;
	Value.m_Type = *pType;

	memset (Value.m_p, 0, pType->m_Size);

	if (pType->m_Type & axl_gc_ETypeFlag_Destruct)
	{
		axl_gc_TValueEntry* pValueEntry = AllocateValueEntry ();
		pValueEntry->m_Value = Value;

		// add to destruct list

		m_DestructList.InsertTail (pValueEntry);
	}

	if (m_State == EState_Mark) 
	{
		axl_gc_TValueEntry* pValueEntry = AllocateValueEntry ();
		pValueEntry->m_Value = Value;

		// mark gray

		ASSERT (!m_MutatorGrayMap.GetBit (Address));

		m_MutatorGrayMap.SetBitRange (Address, Address + ClusterCount, true);
		m_MutatorGrayList.InsertTail (pValueEntry);
	}
	else 
	{
		axl_gc_TValueEntry* pValueEntry = AllocateValueEntry ();
		pValueEntry->m_Value = Value;

		// mark green

		ASSERT (!::TlsGetValue (m_GreenSlotTls));

		::TlsSetValue (m_GreenSlotTls, pValueEntry);
		m_GreenList.InsertTail (pValueEntry);

		// ... and check if we hit the trigger for a new collection cycle

		if (m_MapArray [m_AllocateMap].GetFreeSizeTop () < m_Trigger)
			StartCycle_l ();
	}

	return Value.m_p;
}

void
CCollector::Collect ()
{
	m_Lock.Lock ();
	if (m_State != EState_Mark)
		StartCycle_l ();
	m_Lock.Unlock ();
	m_IdleEvent.Wait ();
}

void*
CCollector::Allocate (const axl_gc_TType* pType)
{
	void* p;

	m_Lock.Lock ();

	p = TryAllocate_l (pType);
	if (p)
	{
		m_Lock.Unlock ();
		return p;
	}

	// wait for the second chance

#ifdef _DEBUG
	m_SecondChanceCount++;
#endif

	m_Lock.Unlock ();

	m_IdleEvent.Wait ();

	m_Lock.Lock ();
	p = TryAllocate_l (pType);
	m_Lock.Unlock ();

	if (!p)
	{
		err::SetError (err::EStatus_InsufficientResources);
		return NULL;
	}

	return p;
}
 
void
CCollector::OnPointerAssign (axl_gc_TValue* pTarget)
{
	axl_gc_TValueEntry* pValueEntry;
	bool_t IsGreen;

	if (!IsInPool (pTarget))
		return; 

	pValueEntry = (axl_gc_TValueEntry*) ::TlsGetValue (m_GreenSlotTls);
	IsGreen = pValueEntry && pValueEntry->m_Value.m_p == pTarget->m_p;

	m_Lock.Lock ();
	
	if (IsGreen)
	{
		// clear green

		m_GreenList.Remove (pValueEntry);
		m_ValueFreeList.InsertTail (pValueEntry);
		::TlsSetValue (m_GreenSlotTls, NULL);
	}

	if (m_State == EState_Mark)
	{
		size_t Address = GetClusterAddress (pTarget); 

		if (!m_MutatorGrayMap.GetBit (Address))
		{
			// mark gray

			size_t ClusterCount = GetClusterCount (pTarget);
			m_MutatorGrayMap.SetBitRange (Address, Address + ClusterCount, true);

			pValueEntry = AllocateValueEntry ();
			pValueEntry->m_Value = *pTarget;
			m_MutatorGrayList.InsertTail (pValueEntry);
		}
	}

	m_Lock.Unlock ();
}

void
CCollector::DiscardLastAllocate (axl_gc_TValue* pValue) // this is just for debug assert
{
	axl_gc_TValueEntry* pValueEntry = (axl_gc_TValueEntry*) ::TlsGetValue (m_GreenSlotTls);

	ASSERT (
		pValueEntry && 
		pValueEntry->m_Value.m_p == pValue->m_p &&
		pValueEntry->m_Value.m_Type.m_Size != pValue->m_Type.m_Size
		);

	if (!pValueEntry || pValueEntry->m_Value.m_p != pValue->m_p) // better to leak than to crash
		return;

	// clear green

	m_Lock.Lock ();
	m_GreenList.Remove (pValueEntry);
	m_ValueFreeList.InsertTail (pValueEntry);
	m_Lock.Unlock ();

	::TlsSetValue (m_GreenSlotTls, NULL);
}

bool
CCollector::Push (const axl_gc_TValue* pValue)
{
	size_t Address;
	size_t ClusterCount;

	rtl::CArrayT <axl_gc_TValue>* pCurrentTraceSet = GetCurrentTraceSet ();

	if (!IsInPool (pValue))
		return false;

	Address = GetClusterAddress (pValue);
	if (m_CollectorGrayMap.GetBit (Address)) // already marked
		return false;

	ClusterCount = GetClusterCount (pValue);
	m_CollectorGrayMap.SetBitRange (Address, Address + ClusterCount, true);
	pCurrentTraceSet->Append (pValue, 1);
	return true;
}

bool
CCollector::PushBulk (
	const axl_gc_TValue* pValue,
	size_t Count
	)
{
	rtl::CArrayT <axl_gc_TValue>* pCurrentTraceSet = GetCurrentTraceSet ();
	pCurrentTraceSet->Append (pValue, Count);
	return true;
}

void
CCollector::GetUnmarkedDestructors_l (
	rtl::CBuddyAllocMap* pBlackMap,
	rtl::CStdListT <axl_gc_TValueEntry>* pList
	)
{
	rtl::CIteratorT <axl_gc_TValueEntry> It = m_DestructList.GetHead ();
	while (It)
	{
		rtl::CIteratorT <axl_gc_TValueEntry> Next = It.GetInc (1);
		
		axl_gc_TValueEntry* pValueEntry = *It;

		size_t Address = CCollector::GetClusterAddress (&pValueEntry->m_Value);
		if (!pBlackMap->GetBit (Address))
		{
			m_DestructList.Remove (It);
			pList->InsertTail (pValueEntry);
		}

		It = Next;
	}
}

rtl::CArrayT <axl_gc_TValue>*
CCollector::FlipTraceSet ()
{
	rtl::CArrayT <axl_gc_TValue>* pCurrentTraceSet;

	m_CurrentTraceSet = !m_CurrentTraceSet;
	pCurrentTraceSet = CCollector::GetCurrentTraceSet ();
	pCurrentTraceSet->Clear ();
	return pCurrentTraceSet;
}

rtl::CArrayT <axl_gc_TValue>*
CCollector::Mark (rtl::CBuddyAllocMap* pBlackMap)
{
	rtl::CArrayT <axl_gc_TValue>* pCurrentTraceSet = CCollector::GetCurrentTraceSet ();

	size_t Count = pCurrentTraceSet->GetCount ();
	axl_gc_TValue* pValue = *pCurrentTraceSet;
	axl_gc_TValue* pEnd = *pCurrentTraceSet + Count;

	pCurrentTraceSet = CCollector::FlipTraceSet ();
	pCurrentTraceSet->Reserve (Count);

	for (; pValue < pEnd; pValue++)
	{
		size_t Address;
		size_t ClusterCount;

		if (!CCollector::IsInPool (pValue))
			continue;

		Address = CCollector::GetClusterAddress (pValue);
		if (pBlackMap->GetBit (Address)) // already marked
			continue;

		ClusterCount = GetClusterCount (pValue);
		m_CollectorGrayMap.SetBitRange (Address, Address + ClusterCount, true);
		pBlackMap->Mark (Address, ClusterCount);

		pCurrentTraceSet->Append (pValue, 1);
	}

	return pCurrentTraceSet;
}

rtl::CArrayT <axl_gc_TValue>*
CCollector::Trace ()
{
	rtl::CArrayT <axl_gc_TValue>* pCurrentTraceSet = CCollector::GetCurrentTraceSet ();

	size_t Count = pCurrentTraceSet->GetCount ();
	axl_gc_TValue* pValue = *pCurrentTraceSet;
	axl_gc_TValue* pEnd = *pCurrentTraceSet + Count;

	pCurrentTraceSet = CCollector::FlipTraceSet ();

	for (; pValue < pEnd; pValue++)
	{
		if (pValue->m_Type.m_Type & axl_gc_ETypeFlag_Trace)
			m_pMutator->TraceValue (this, pValue);
	}

	return pCurrentTraceSet;
}

void
CCollector::Cycle ()
{
	rtl::CBuddyAllocMap* pBlackMap;
	rtl::CArrayT <axl_gc_TValue>* pCurrentTraceSet;

	ASSERT (m_State == EState_Mark); // otherwise why are we here?

#ifdef _DEBUG
	m_CycleCount++;
#endif

	// clear mark maps

	pBlackMap = &m_MapArray [!m_AllocateMap];  // is only touched withing worker thread
	pBlackMap->Clear (); 
	m_CollectorGrayMap.Clear ();

	// get root set 

	pCurrentTraceSet = CCollector::FlipTraceSet ();
	m_pMutator->GetRootSet (this);

	// add green set (don't clear the list though: it should be cleared in OnPointerAssign ()

	m_Lock.Lock (); 

	if (!m_GreenList.IsEmpty ())
	{
#ifdef _DEBUG
		m_GreenHitCount++;
#endif
		AppendValueListToArray (&m_GreenList, pCurrentTraceSet);
	}

	m_Lock.Unlock ();

	// mark breadth first (exit the loop with lock held)

	for (;;)
	{
		CCollector::Mark (pBlackMap);
		CCollector::Trace ();
		
		// add values marked gray by mutator and sync gray maps

		pCurrentTraceSet = CCollector::GetCurrentTraceSet ();

		m_Lock.Lock ();
		AppendValueListToArray (&m_MutatorGrayList, pCurrentTraceSet);
		m_ValueFreeList.InsertListTail (&m_MutatorGrayList);
		m_MutatorGrayMap.Merge (m_CollectorGrayMap, rtl::EBitOp_Or);

		if (pCurrentTraceSet->IsEmpty ())
			break; // exit with lock held

		m_Lock.Unlock ();
	} 

	// we marked everything, finish the cycle

	rtl::CStdListT <axl_gc_TValueEntry> DestructList;
	CCollector::GetUnmarkedDestructors_l (pBlackMap, &DestructList);
	m_MutatorGrayMap.Clear ();
	m_AllocateMap = !m_AllocateMap;
	m_State = EState_Idle;
	m_IdleEvent.Signal ();
	m_Lock.Unlock ();

	// destruct...

	rtl::CIteratorT <axl_gc_TValueEntry> It = DestructList.GetHead ();
	for (; It; It++)
		m_pMutator->DestructValue (&It->m_Value);

	// ...and return it to the free list

	m_Lock.Lock ();
	m_ValueFreeList.InsertListTail (&DestructList);
	m_Lock.Unlock ();
}

ulong_t
CCollector::ThreadProc ()
{
	m_Thread.SetPriority (THREAD_PRIORITY_LOWEST);

	for (;;)
	{
		m_WakeUpEvent.Wait ();

		Cycle (); // do a final cycle even if terminating

		if (m_TerminateThread)
			break;
	}

	return 0;
}

//.............................................................................

} // namespace gc {
} // namespace axl {
