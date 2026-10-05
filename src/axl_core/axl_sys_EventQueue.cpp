//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_sys_EventQueue.h"
#include "axl_err_Error.h"
#include "axl_sl_Array.h"

namespace axl {
namespace sys {

//..............................................................................

handle_t
EventQueue::asyncWait(
	uint_t eventMask,
	EventQueueAsyncWaitFunc* func,
	void* context
) {
	eventMask |= EventQueueEvent_Closed; // always respect close

	m_lock.lock();
	if (uint_t triggeredEvents = m_activeEvents & eventMask) {
		m_lock.unlock();
		func(triggeredEvents, context);
		return 0; // not added
	}

	AsyncWait* wait = new AsyncWait;
	wait->m_mask = eventMask;
	wait->m_func = func;
	wait->m_context = context;
	m_asyncWaitList.insertTail(wait);
	handle_t handle = (handle_t)m_asyncWaitMap.add(wait);
	wait->m_handle = handle;
	m_lock.unlock();
	return handle;
}

uint_t
EventQueue::blockingWait(
	uint_t eventMask,
	uint_t timeout
) {
	eventMask |= EventQueueEvent_Closed; // always respect close

	m_lock.lock();
	if (uint_t triggeredEvents = m_activeEvents & eventMask) {
		m_lock.unlock();
		return triggeredEvents;
	}

	Event event;
	SyncWait wait;
	wait.m_mask = eventMask;
	wait.m_event = &event;
	m_syncWaitList.insertTail(&wait);
	m_lock.unlock();

	event.wait(timeout);

	m_lock.lock();
	uint_t triggeredEvents = m_activeEvents & eventMask;
	m_syncWaitList.remove(&wait);
	m_lock.unlock();
	return triggeredEvents;
}

EventQueueMultiWait*
EventQueue::multiWait(
	uint_t eventMask,
	EventQueueMultiWait* waitArray,
	size_t count,
	uint_t timeout
) {
	eventMask |= EventQueueEvent_Closed; // always respect close

	Event event;
	char buffer[256];
	sl::Array<SyncWait> syncWaitArray(rc::BufKind_Stack, buffer, sizeof(buffer));
	syncWaitArray.setCount(count); // never reallocated after this -- the queues link the entries
	SyncWait* syncWaitTable = syncWaitArray.p();

	bool isTriggered = false;
	for (size_t i = 0; i < count; i++) {
		EventQueueMultiWait* wait = &waitArray[i];
		SyncWait* syncWait = &syncWaitTable[i];
		syncWait->m_mask = eventMask;
		syncWait->m_event = NULL; // not linked

		EventQueue* queue = wait->m_queue;
		queue->m_lock.lock();
		wait->m_triggeredEvents = queue->m_activeEvents & eventMask;
		if (wait->m_triggeredEvents)
			isTriggered = true;
		else {
			syncWait->m_event = &event;
			queue->m_syncWaitList.insertTail(syncWait);
		}

		queue->m_lock.unlock();
	}

	if (!isTriggered)
		event.wait(timeout);

	EventQueueMultiWait* resultWait = NULL;
	for (size_t i = 0; i < count; i++) {
		EventQueueMultiWait* wait = &waitArray[i];
		SyncWait* syncWait = &syncWaitTable[i];

		EventQueue* queue = wait->m_queue;
		queue->m_lock.lock();
		wait->m_triggeredEvents = queue->m_activeEvents & eventMask;
		if (syncWait->m_event)
			queue->m_syncWaitList.remove(syncWait);

		queue->m_lock.unlock();

		if (wait->m_triggeredEvents && !resultWait)
			resultWait = wait;
	}

	return resultWait ? resultWait : err::fail<EventQueueMultiWait*>(NULL, err::Error(err::SystemErrorCode_IoTimeout));
}

bool
EventQueue::cancelWait(handle_t handle) {
	m_lock.lock();

	sl::HandleTableIterator<AsyncWait*> it = m_asyncWaitMap.find((uintptr_t)handle);
	if (!it) {
		m_lock.unlock();
		return err::fail(err::Error(err::SystemErrorCode_InvalidParameter));
	}

	m_asyncWaitList.erase(it->m_value);
	m_asyncWaitMap.erase(it);
	m_lock.unlock();
	return true;
}

void
EventQueue::setEvents_l(uint_t events) {
	if ((m_activeEvents & events) == events) {
		m_lock.unlock();
		return;
	}

	m_activeEvents |= events;

	sl::Iterator<SyncWait> syncIt = m_syncWaitList.getHead();
	for (; syncIt; syncIt++)
		if (syncIt->m_mask & m_activeEvents)
			syncIt->m_event->signal();

	sl::List<AsyncWait> triggeredList;
	sl::Iterator<AsyncWait> asyncIt = m_asyncWaitList.getHead();
	while (asyncIt) {
		AsyncWait* wait = *asyncIt;
		asyncIt++;

		uint_t triggeredEvents = wait->m_mask & m_activeEvents;
		if (!triggeredEvents)
			continue;

		wait->m_mask = triggeredEvents;
		m_asyncWaitList.remove(wait);
		m_asyncWaitMap.eraseKey((uintptr_t)wait->m_handle);
		triggeredList.insertTail(wait);
	}

	m_lock.unlock();

	while (!triggeredList.isEmpty()) {
		AsyncWait* wait = triggeredList.removeHead();
		wait->m_func(wait->m_mask, wait->m_context);
		delete wait;
	}
}

//..............................................................................

} // namespace sys
} // namespace axl
