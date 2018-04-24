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

#pragma once

#define _AXL_SYS_READWRITELOCK_H

#include "axl_sys_Atomic.h"
#include "axl_sys_Event.h"
#include "axl_io_Mapping.h"

namespace axl {
namespace sys {

//..............................................................................

class ReadWriteLock
{
protected:
	// locking data may be shared between processes

	struct Data
	{
		volatile int32_t m_lock;
		volatile uint32_t m_activeReadCount;
		volatile uint32_t m_queuedReadCount;
		volatile uint32_t m_writeCount; // one or zero active, the rest is on queue
	};

#if (_AXL_OS_WIN)
	class WinEventWaitHelper: public sys::win::Event
	{
	public:
		bool
		wait (uint_t timeout)
		{
			return sys::win::Event::wait (timeout) == sys::win::WaitResult_Object0;
		}
	};

	class WinEvent: public WinEventWaitHelper
	{
	public:
		WinEventWaitHelper*
		operator -> ()
		{
			return this;
		}
	};
#elif (_AXL_OS_POSIX)
	enum EventKind
	{
		EventKind_UnnamedEvent,
		EventKind_NamedSem,
	};

	class EventBase // pseudo-virtual base
	{
	protected:
		EventKind m_eventKind;

	public:
		virtual ~EventBase ()
		{
		}

		// hot path functions are non-virtual for better performance

		bool
		signal ();

		void
		reset ();

		bool
		wait (uint_t timeout);
	};

	class UnnamedEvent: public EventBase
	{
		friend class EventBase;

	protected:
		sys::EventBase m_event;

	public:
		UnnamedEvent (bool isNotificationEvent):
			m_event (isNotificationEvent)
		{
			m_eventKind = EventKind_UnnamedEvent;
		}
	};

	class NamedSemEvent: public EventBase
	{
		friend class EventBase;

	protected:
		sys::psx::NamedSem m_sem;
		sl::String m_name;

	public:
		NamedSemEvent ()
		{
			m_eventKind = EventKind_NamedSem;
		}

		bool
		create (const sl::StringRef& name);

		bool
		open (const sl::StringRef& name);
	};
#endif

protected:
	Data* m_data;
	io::File m_file;
	io::Mapping m_mapping;

#if (_AXL_OS_WIN)
	WinEvent m_readEvent;
	WinEvent m_writeEvent;
#elif (_AXL_OS_POSIX)
	EventBase* m_readEvent;
	EventBase* m_writeEvent;
#endif

public:
	ReadWriteLock ();

	~ReadWriteLock ()
	{
		close ();
	}

	void
	close ();

	bool
	create ();

	bool
	create (
		const sl::StringRef& fileName,
		const sl::StringRef& readEventName,
		const sl::StringRef& writeEventName
		);

	bool
	open (
		const sl::StringRef& fileName,
		const sl::StringRef& readEventName,
		const sl::StringRef& writeEventName
		);

	bool
	readLock (uint_t timeout = -1);

	void
	readUnlock ();

	bool
	writeLock (uint_t timeout = -1);

	void
	writeUnlock ();
};

//..............................................................................

} // namespace sys
} // namespace axl
