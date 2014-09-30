// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_EXE_WORKERTHREADPOOL_H

#include "axl_ref_RefCount.h"
#include "axl_exe_WorkerThread.h"

namespace axl {
namespace exe {

//.............................................................................

class WorkerhreadPool
{
protected:
	struct ThreadEntry: rtl::ListLink
	{
		Workerhread m_thread;
		size_t m_reserveEventCount;
		size_t m_pinCount;
	};

	class ThreadPin: public ref::IRefCount
	{
	public:
		WorkerhreadPool* m_threadPool;
		ThreadEntry* m_threadEntry;
		size_t m_reserveEventCount;

	public:
		ThreadPin ();

		~ThreadPin ();
	};

protected:
	friend class ThreadPin;

	mt::Lock m_lock;
	rtl::StdList <ThreadEntry> m_threadList;

public:
	~WorkerhreadPool ()
	{
		stop ();
	}

	bool 
	start (size_t threadCount = 0); // 0 -- auto (processor count)

	bool 
	grow (size_t threadCount = 0);

	void 
	stop (ulong_t timeout = -1);

	ref::Ptr <Workerhread> 
	getThread (size_t reserveEventCount = 0);
};

//.............................................................................

} // namespace exe
} // namespace axl
