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

class Collector: public mt::ThreadImpl <Collector>
{
protected:
	enum StateKind
	{
		StateKind_Idle = 0,
		StateKind_Mark,  
	};

	enum DefKind
	{
		DefKind_ClusterSize = sizeof(void*)
	};

protected:
	IMutator* m_mutator;
	
	mt::Lock m_lock;
	mt::Event m_idleEvent;
	mt::Event m_wakeUpEvent;

	volatile StateKind m_state;
	volatile bool m_terminateThread;

	rtl::StdList <axl_gc_TValueEntry> m_valueFreeList;
	rtl::StdList <axl_gc_TValueEntry> m_destructList;
	rtl::StdList <axl_gc_TValueEntry> m_greenList; 
	rtl::StdList <axl_gc_TValueEntry> m_mutatorGrayList; 

	rtl::Array <axl_gc_TValue> m_traceSetArray [2]; // array of axl_gc_TValue
	size_t m_currentTraceSet;

	rtl::BuddyAllocMap m_mapArray [2];
	size_t m_allocateMap; 

	rtl::BitMap m_mutatorGrayMap;
	rtl::BitMap m_collectorGrayMap;
		 
	ulong_t m_greenSlotTls;

	// black map is m_MapArray [!m_AllocateMap] and is only touched within collector thread

	void* m_pool;
	size_t m_poolSize; // == ClusterMap::m_TotalSize * AXL_GC_CLUSTER_SIZE, but it's nice to have it plain text
	size_t m_trigger;  // if allocation map's FreeSizeTop drops below trigger, collection cycle is started 

#ifdef _DEBUG
	size_t m_cycleCount;
	size_t m_greenHitCount;
	size_t m_secondChanceCount;
#endif

public:
	Collector ();

	~Collector ()
	{
		close ();
	}

	bool 
	create (
		IMutator* mutator, 
		size_t width, 
		size_t height, 
		uint_t triggerPercent = 25
		// when free size drops below the trigger, collection cycle is started 
		// zero enforces 'stop-the-world' semantics -- collection is initated on failed allocation and is waited upon
		);

	void 
	close();

	void 
	collect ();

	void* 
	allocate (const axl_gc_TType* type);

	void 
	onPointerAssign (axl_gc_TValue* target);

	void 
	discardLastAllocate (axl_gc_TValue* value); // pValue is just for debug assert

	// it is recommended to use PushBulk() during IMutator::GetRootSet() and Push() during IMutator::Trace()
	// however they could be used interchangebly

	bool 
	push (const axl_gc_TValue* value);

	bool 
	pushBulk (
		const axl_gc_TValue* value, 
		size_t count
		);

protected:
	bool
	isInPool (const axl_gc_TValue* value);

	size_t
	getClusterAddress (const axl_gc_TValue* value);

	static
	size_t
	getClusterCount (const axl_gc_TValue* value);

	void
	Collector::startCycle_l ();

	void*
	tryAllocate_l (const axl_gc_TType* type);

	rtl::Array <axl_gc_TValue>*
	getCurrentTraceSet ()
	{
		return &m_traceSetArray [m_currentTraceSet];
	}

	rtl::Array <axl_gc_TValue>*
	flipTraceSet ();

	rtl::Array <axl_gc_TValue>*
	mark (rtl::BuddyAllocMap* blackMap);

	rtl::Array <axl_gc_TValue>*
	trace ();

	void
	cycle ();

	void
	getUnmarkedDestructors_l (
		rtl::BuddyAllocMap* blackMap,
		rtl::StdList <axl_gc_TValueEntry>* list
		);

	axl_gc_TValueEntry* 
	allocateValueEntry ();

	friend class mt::ThreadImpl <Collector>;

	ulong_t
	threadProc ();
};

//.............................................................................

} // namespace gc {
} // namespace axl {
