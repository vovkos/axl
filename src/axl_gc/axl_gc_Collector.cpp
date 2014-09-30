#include "pch.h"
#include "axl_gc_Collector.h"
#include "axl_err_Error.h"

namespace axl {
namespace gc {

//.............................................................................

// helper

bool
appendValueListToArray (
	rtl::StdList <axl_gc_TValueEntry>* list,
	rtl::Array <axl_gc_TValue>* array
	)
{
	if (list->isEmpty ())
		return true;

	size_t count = array->getCount ();
	if (!array->setCount (count + list->getCount ()))
		return false;

	rtl::Iterator <axl_gc_TValueEntry> it = list->getHead ();
	axl_gc_TValue* value = *array + count;

	for (; it; it++, value++)
		*value = it->m_value;

	return true;
}

//.............................................................................

Collector::Collector ():
	m_idleEvent (mt::Event::KindKind_Notification)
{
	m_mutator = NULL;

	m_state = StateKind_Idle;
	m_terminateThread = false;

	m_idleEvent.signal ();

	m_allocateMap = 0;
	m_greenSlotTls = -1;
	m_currentTraceSet = 0;

	m_pool = NULL;
	m_poolSize = 0;
	m_trigger = 0;

#ifdef _DEBUG
	m_cycleCount = 0;
	m_greenHitCount = 0;
	m_secondChanceCount = 0;
#endif
}

bool
Collector::isInPool (const axl_gc_TValue* value)
{
	void* poolEnd = (uchar_t*) m_pool + m_poolSize;
	if (value->m_p < m_pool || value->m_p >= poolEnd)
		return false;

	ASSERT ((uchar_t*) value->m_p + value->m_type.m_size <= (uchar_t*) poolEnd);
	return true;
}

size_t
Collector::getClusterAddress (const axl_gc_TValue* value)
{
	ASSERT (isInPool (value));
	ASSERT (!(((uchar_t*) value->m_p - (uchar_t*) m_pool) & (DefKind_ClusterSize - 1)));
	return ((uchar_t*) value->m_p - (uchar_t*) m_pool) / DefKind_ClusterSize;
}

size_t
Collector::getClusterCount (const axl_gc_TValue* value)
{
	return (value->m_type.m_size + DefKind_ClusterSize - 1) / DefKind_ClusterSize;
}

bool
Collector::create (
	IMutator* mutator,
	size_t width,
	size_t height,
	uint_t triggerPercent
	)
{
	bool_t result;
	size_t clusterCount;

	close ();

	m_greenSlotTls = ::TlsAlloc ();
	if (m_greenSlotTls == -1)
		return err::failWithLastSystemError ();

	result = 
		m_mapArray [0].create (width, height) &&
		m_mapArray [1].create (width, height);
	
	if (!result)
		return false;

	clusterCount = m_mapArray [0].getTotalSize ();

	result = m_mutatorGrayMap.create (clusterCount);
	if (!result)
		return false;

	m_poolSize = clusterCount * DefKind_ClusterSize;
	m_pool = AXL_MEM_ALLOC (m_poolSize);
	if (!m_pool)
		return false;

	m_mutator = mutator;
	m_allocateMap = 0;
	m_trigger = mulDiv ((int) clusterCount, triggerPercent, 100);
	m_state = StateKind_Idle;
	m_terminateThread = false;

#ifdef _DEBUG
	m_cycleCount = 0;
	m_greenHitCount = 0;
	m_secondChanceCount = 0;
#endif

	m_idleEvent.signal ();
	m_wakeUpEvent.reset ();

	return start ();
}

void
Collector::close ()
{
	m_terminateThread = true;
	m_wakeUpEvent.signal ();
	waitAndClose ();

	// should be empty...

	ASSERT (m_destructList.isEmpty ()); 
	ASSERT (m_greenList.isEmpty ());
	ASSERT (m_mutatorGrayList.isEmpty ());

	// ...but clean up anyway

	m_valueFreeList.insertListTail (&m_greenList);
	m_valueFreeList.insertListTail (&m_mutatorGrayList);
	m_valueFreeList.insertListTail (&m_destructList);

	ASSERT (m_mapArray [m_allocateMap].isEmpty ()); // should be...
	
	if (m_pool && m_mapArray [m_allocateMap].isEmpty ()) // ...but better to leak than to crash
		AXL_MEM_FREE (m_pool);

	m_pool = NULL;
	m_poolSize = 0;

	m_mapArray [0].close ();
	m_mapArray [1].close ();

	if (m_greenSlotTls != -1)
		::TlsFree (m_greenSlotTls);
}

axl_gc_TValueEntry* 
Collector::allocateValueEntry ()
{
	return m_valueFreeList.isEmpty () ? 
		m_valueFreeList.removeHead () :
		AXL_MEM_NEW (axl_gc_TValueEntry);
}

void
Collector::startCycle_l () 
{
	ASSERT (m_state != StateKind_Mark);
	m_state = StateKind_Mark;
	m_wakeUpEvent.signal ();
	m_idleEvent.reset ();
}

void*
Collector::tryAllocate_l (const axl_gc_TType* type)
{
	axl_gc_TValue value;
	
	size_t address;
	size_t clusterCount = (type->m_size + DefKind_ClusterSize - 1) / DefKind_ClusterSize;

	address = m_mapArray [m_allocateMap].allocate (clusterCount);
	if (address == -1)
	{
		// failed allocation triggers a new collection cycle (if not yet)

		if (m_state != StateKind_Mark)
			startCycle_l ();

		return NULL;
	}

	value.m_p = (uchar_t*) m_pool + address * DefKind_ClusterSize;
	value.m_type = *type;

	memset (value.m_p, 0, type->m_size);

	if (type->m_type & axl_gc_ETypeFlag_Destruct)
	{
		axl_gc_TValueEntry* valueEntry = allocateValueEntry ();
		valueEntry->m_value = value;

		// add to destruct list

		m_destructList.insertTail (valueEntry);
	}

	if (m_state == StateKind_Mark) 
	{
		axl_gc_TValueEntry* valueEntry = allocateValueEntry ();
		valueEntry->m_value = value;

		// mark gray

		ASSERT (!m_mutatorGrayMap.getBit (address));

		m_mutatorGrayMap.setBitRange (address, address + clusterCount, true);
		m_mutatorGrayList.insertTail (valueEntry);
	}
	else 
	{
		axl_gc_TValueEntry* valueEntry = allocateValueEntry ();
		valueEntry->m_value = value;

		// mark green

		ASSERT (!::tlsGetValue (m_greenSlotTls));

		::TlsSetValue (m_greenSlotTls, valueEntry);
		m_greenList.insertTail (valueEntry);

		// ... and check if we hit the trigger for a new collection cycle

		if (m_mapArray [m_allocateMap].getFreeSizeTop () < m_trigger)
			startCycle_l ();
	}

	return value.m_p;
}

void
Collector::collect ()
{
	m_lock.lock ();
	if (m_state != StateKind_Mark)
		startCycle_l ();
	m_lock.unlock ();
	m_idleEvent.wait ();
}

void*
Collector::allocate (const axl_gc_TType* type)
{
	void* p;

	m_lock.lock ();

	p = tryAllocate_l (type);
	if (p)
	{
		m_lock.unlock ();
		return p;
	}

	// wait for the second chance

#ifdef _DEBUG
	m_secondChanceCount++;
#endif

	m_lock.unlock ();

	m_idleEvent.wait ();

	m_lock.lock ();
	p = tryAllocate_l (type);
	m_lock.unlock ();

	if (!p)
	{
		err::setError (err::StatusKind_InsufficientResources);
		return NULL;
	}

	return p;
}
 
void
Collector::onPointerAssign (axl_gc_TValue* target)
{
	axl_gc_TValueEntry* valueEntry;
	bool_t isGreen;

	if (!isInPool (target))
		return; 

	valueEntry = (axl_gc_TValueEntry*) ::TlsGetValue (m_greenSlotTls);
	isGreen = valueEntry && valueEntry->m_value.m_p == target->m_p;

	m_lock.lock ();
	
	if (isGreen)
	{
		// clear green

		m_greenList.remove (valueEntry);
		m_valueFreeList.insertTail (valueEntry);
		::TlsSetValue (m_greenSlotTls, NULL);
	}

	if (m_state == StateKind_Mark)
	{
		size_t address = getClusterAddress (target); 

		if (!m_mutatorGrayMap.getBit (address))
		{
			// mark gray

			size_t clusterCount = getClusterCount (target);
			m_mutatorGrayMap.setBitRange (address, address + clusterCount, true);

			valueEntry = allocateValueEntry ();
			valueEntry->m_value = *target;
			m_mutatorGrayList.insertTail (valueEntry);
		}
	}

	m_lock.unlock ();
}

void
Collector::discardLastAllocate (axl_gc_TValue* value) // this is just for debug assert
{
	axl_gc_TValueEntry* valueEntry = (axl_gc_TValueEntry*) ::TlsGetValue (m_greenSlotTls);

	ASSERT (
		valueEntry && 
		valueEntry->m_value.m_p == value->m_p &&
		valueEntry->m_value.m_type.m_size != value->m_type.m_size
		);

	if (!valueEntry || valueEntry->m_value.m_p != value->m_p) // better to leak than to crash
		return;

	// clear green

	m_lock.lock ();
	m_greenList.remove (valueEntry);
	m_valueFreeList.insertTail (valueEntry);
	m_lock.unlock ();

	::TlsSetValue (m_greenSlotTls, NULL);
}

bool
Collector::push (const axl_gc_TValue* value)
{
	size_t address;
	size_t clusterCount;

	rtl::Array <axl_gc_TValue>* currentTraceSet = getCurrentTraceSet ();

	if (!isInPool (value))
		return false;

	address = getClusterAddress (value);
	if (m_collectorGrayMap.getBit (address)) // already marked
		return false;

	clusterCount = getClusterCount (value);
	m_collectorGrayMap.setBitRange (address, address + clusterCount, true);
	currentTraceSet->append (value, 1);
	return true;
}

bool
Collector::pushBulk (
	const axl_gc_TValue* value,
	size_t count
	)
{
	rtl::Array <axl_gc_TValue>* currentTraceSet = getCurrentTraceSet ();
	currentTraceSet->append (value, count);
	return true;
}

void
Collector::getUnmarkedDestructors_l (
	rtl::BuddyAllocMap* blackMap,
	rtl::StdList <axl_gc_TValueEntry>* list
	)
{
	rtl::Iterator <axl_gc_TValueEntry> it = m_destructList.getHead ();
	while (it)
	{
		rtl::Iterator <axl_gc_TValueEntry> next = it.getInc (1);
		
		axl_gc_TValueEntry* valueEntry = *it;

		size_t address = Collector::getClusterAddress (&valueEntry->m_value);
		if (!blackMap->getBit (address))
		{
			m_destructList.remove (it);
			list->insertTail (valueEntry);
		}

		it = next;
	}
}

rtl::Array <axl_gc_TValue>*
Collector::flipTraceSet ()
{
	rtl::Array <axl_gc_TValue>* currentTraceSet;

	m_currentTraceSet = !m_currentTraceSet;
	currentTraceSet = Collector::getCurrentTraceSet ();
	currentTraceSet->clear ();
	return currentTraceSet;
}

rtl::Array <axl_gc_TValue>*
Collector::mark (rtl::BuddyAllocMap* blackMap)
{
	rtl::Array <axl_gc_TValue>* currentTraceSet = Collector::getCurrentTraceSet ();

	size_t count = currentTraceSet->getCount ();
	axl_gc_TValue* value = *currentTraceSet;
	axl_gc_TValue* end = *currentTraceSet + count;

	currentTraceSet = Collector::flipTraceSet ();
	currentTraceSet->reserve (count);

	for (; value < end; value++)
	{
		size_t address;
		size_t clusterCount;

		if (!Collector::isInPool (value))
			continue;

		address = Collector::getClusterAddress (value);
		if (blackMap->getBit (address)) // already marked
			continue;

		clusterCount = getClusterCount (value);
		m_collectorGrayMap.setBitRange (address, address + clusterCount, true);
		blackMap->mark (address, clusterCount);

		currentTraceSet->append (value, 1);
	}

	return currentTraceSet;
}

rtl::Array <axl_gc_TValue>*
Collector::trace ()
{
	rtl::Array <axl_gc_TValue>* currentTraceSet = Collector::getCurrentTraceSet ();

	size_t count = currentTraceSet->getCount ();
	axl_gc_TValue* value = *currentTraceSet;
	axl_gc_TValue* end = *currentTraceSet + count;

	currentTraceSet = Collector::flipTraceSet ();

	for (; value < end; value++)
	{
		if (value->m_type.m_type & axl_gc_ETypeFlag_Trace)
			m_mutator->traceValue (this, value);
	}

	return currentTraceSet;
}

void
Collector::cycle ()
{
	rtl::BuddyAllocMap* blackMap;
	rtl::Array <axl_gc_TValue>* currentTraceSet;

	ASSERT (m_state == StateKind_Mark); // otherwise why are we here?

#ifdef _DEBUG
	m_cycleCount++;
#endif

	// clear mark maps

	blackMap = &m_mapArray [!m_allocateMap];  // is only touched withing worker thread
	blackMap->clear (); 
	m_collectorGrayMap.clear ();

	// get root set 

	currentTraceSet = Collector::flipTraceSet ();
	m_mutator->getRootSet (this);

	// add green set (don't clear the list though: it should be cleared in OnPointerAssign ()

	m_lock.lock (); 

	if (!m_greenList.isEmpty ())
	{
#ifdef _DEBUG
		m_greenHitCount++;
#endif
		appendValueListToArray (&m_greenList, currentTraceSet);
	}

	m_lock.unlock ();

	// mark breadth first (exit the loop with lock held)

	for (;;)
	{
		Collector::mark (blackMap);
		Collector::trace ();
		
		// add values marked gray by mutator and sync gray maps

		currentTraceSet = Collector::getCurrentTraceSet ();

		m_lock.lock ();
		appendValueListToArray (&m_mutatorGrayList, currentTraceSet);
		m_valueFreeList.insertListTail (&m_mutatorGrayList);
		m_mutatorGrayMap.merge (m_collectorGrayMap, rtl::BitOpKind_Or);

		if (currentTraceSet->isEmpty ())
			break; // exit with lock held

		m_lock.unlock ();
	} 

	// we marked everything, finish the cycle

	rtl::StdList <axl_gc_TValueEntry> destructList;
	Collector::getUnmarkedDestructors_l (blackMap, &destructList);
	m_mutatorGrayMap.clear ();
	m_allocateMap = !m_allocateMap;
	m_state = StateKind_Idle;
	m_idleEvent.signal ();
	m_lock.unlock ();

	// destruct...

	rtl::Iterator <axl_gc_TValueEntry> it = destructList.getHead ();
	for (; it; it++)
		m_mutator->destructValue (&it->m_value);

	// ...and return it to the free list

	m_lock.lock ();
	m_valueFreeList.insertListTail (&destructList);
	m_lock.unlock ();
}

ulong_t
Collector::threadProc ()
{
	m_thread.setPriority (THREAD_PRIORITY_LOWEST);

	for (;;)
	{
		m_wakeUpEvent.wait ();

		cycle (); // do a final cycle even if terminating

		if (m_terminateThread)
			break;
	}

	return 0;
}

//.............................................................................

} // namespace gc {
} // namespace axl {
