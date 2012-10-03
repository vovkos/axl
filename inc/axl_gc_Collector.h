// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2011. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_gc_Mutator.h"
#include "axl_rtl_BuddyAllocMap.h"
#include "axl_rtl_BitMap.h"
#include "axl_mt_Lock.h"
#include "axl_mt_Event.h"
#include "axl_mt_Thread.h"

// non-moving incremental concurrent mark-sweep collector with buddy allocator
// variation of good old Dijkstra "on-the-fly gc" algortithm

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// if allocation is done during <idle> state, block is put into current thread green slot.
// that is done to prevent highly unlikely yet possible situation that <mark> state is entered
// BETWEEN allocation was done and allocated block was made reachable from the root set by mutator.
// when collector enters <mark> state it first marks all threads green slots (no need to traverse)
// note that green sets are only used during allocation in <idle> state
// if allocation is done during <mark> state, new block is marked grey (no need to traverse)
// green slots are empty most of the time, they're only alive between Allocate() and OnPointerAssign()

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// buddy allocator requires no sweeping per se, so <sweep> state is actually when destructors are called
// but only (mark / not-mark) is important for mutator <-> collectory sync
	
//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

// whenever mutator changes reachability graph by assigning a non-null pointer, it must notify GC about it
// first assign, then call OnPointerAssign() 
// no need to lock these operations in any way, and pointer assign doesnt even need to be atomic
// however, two rules must be followed:
//
// (1) after each successfull Allocate() new block should be either 
//     (1.1) added to reachability graph, in which case OnPointerAssign() should be called, or
//     (1.2) discarded, in which case DiscardLastAllocate() should be called
//     failing to do so will result in ASSERT() on the next alloc in the same thread, or, if thread dies immediatly, 
//     the allocated memory would never be reclaimed
//
// (2) OnPointerAssign() should be called after EACH and EVERY assign of a non-null pointer that changes reachability graph
//     it's not ok to do a couple of assigns and then call OnPointerAssign()
//     failing to do so might result in incorrectly freeing of a live object
//

namespace axl {
namespace gc {

//.............................................................................

class CCollector: public mt::CThreadImplT <CCollector>
{
protected:
	enum EState
	{
		EState_Idle = 0,
		EState_Mark,  
	};

	enum EDef
	{
		EDef_ClusterSize = sizeof(void*)
	};

protected:
	IMutator* m_pMutator;
	
	mt::CLock m_Lock;
	mt::CEvent m_IdleEvent;
	mt::CEvent m_WakeUpEvent;

	volatile EState m_State;
	volatile bool m_TerminateThread;

	rtl::CStdListT <axl_gc_TValueEntry> m_ValueFreeList;
	rtl::CStdListT <axl_gc_TValueEntry> m_DestructList;
	rtl::CStdListT <axl_gc_TValueEntry> m_GreenList; 
	rtl::CStdListT <axl_gc_TValueEntry> m_MutatorGrayList; 

	rtl::CArrayT <axl_gc_TValue> m_TraceSetArray [2]; // array of axl_gc_TValue
	size_t m_CurrentTraceSet;

	rtl::CBuddyAllocMap m_MapArray [2];
	size_t m_AllocateMap; 

	rtl::CBitMap m_MutatorGrayMap;
	rtl::CBitMap m_CollectorGrayMap;
		 
	ulong_t m_GreenSlotTls;

	// black map is m_MapArray [!m_AllocateMap] and is only touched within collector thread

	void* m_pPool;
	size_t m_PoolSize; // == ClusterMap::m_TotalSize * AXL_GC_CLUSTER_SIZE, but it's nice to have it plain text
	size_t m_Trigger;  // if allocation map's FreeSizeTop drops below trigger, collection cycle is started 

#ifdef _DEBUG
	size_t m_CycleCount;
	size_t m_GreenHitCount;
	size_t m_SecondChanceCount;
#endif

public:
	CCollector ();

	~CCollector ()
	{
		Close ();
	}

	bool 
	Create (
		IMutator* pMutator, 
		size_t Width, 
		size_t Height, 
		uint_t TriggerPercent = 25
		// when free size drops below the trigger, collection cycle is started 
		// zero enforces 'stop-the-world' semantics -- collection is initated on failed allocation and is waited upon
		);

	void 
	Close();

	void 
	Collect ();

	void* 
	Allocate (const axl_gc_TType* pType);

	void 
	OnPointerAssign (axl_gc_TValue* pTarget);

	void 
	DiscardLastAllocate (axl_gc_TValue* pValue); // pValue is just for debug assert

	// it is recommended to use PushBulk() during IMutator::GetRootSet() and Push() during IMutator::Trace()
	// however they could be used interchangebly

	bool 
	Push (const axl_gc_TValue* pValue);

	bool 
	PushBulk (
		const axl_gc_TValue* pValue, 
		size_t Count
		);

protected:
	bool
	IsInPool (const axl_gc_TValue* pValue);

	size_t
	GetClusterAddress (const axl_gc_TValue* pValue);

	static
	size_t
	GetClusterCount (const axl_gc_TValue* pValue);

	void
	CCollector::StartCycle_l ();

	void*
	TryAllocate_l (const axl_gc_TType* pType);

	rtl::CArrayT <axl_gc_TValue>*
	GetCurrentTraceSet ()
	{
		return &m_TraceSetArray [m_CurrentTraceSet];
	}

	rtl::CArrayT <axl_gc_TValue>*
	FlipTraceSet ();

	rtl::CArrayT <axl_gc_TValue>*
	Mark (rtl::CBuddyAllocMap* pBlackMap);

	rtl::CArrayT <axl_gc_TValue>*
	Trace ();

	void
	Cycle ();

	void
	GetUnmarkedDestructors_l (
		rtl::CBuddyAllocMap* pBlackMap,
		rtl::CStdListT <axl_gc_TValueEntry>* pList
		);

	axl_gc_TValueEntry* 
	AllocateValueEntry ();

	friend class mt::CThreadImplT <CCollector>;

	ulong_t
	ThreadProc ();
};

//.............................................................................

} // namespace gc {
} // namespace axl {
