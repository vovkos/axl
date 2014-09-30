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

class CWorkerThreadPool
{
protected:
	struct TThreadEntry: rtl::TListLink
	{
		CWorkerThread m_thread;
		size_t m_reserveEventCount;
		size_t m_pinCount;
	};

	class CThreadPin: public ref::IRefCount
	{
	public:
		CWorkerThreadPool* m_threadPool;
		TThreadEntry* m_threadEntry;
		size_t m_reserveEventCount;

	public:
		CThreadPin ();

		~CThreadPin ();
	};

protected:
	friend class CThreadPin;

	mt::CLock m_lock;
	rtl::CStdListT <TThreadEntry> m_threadList;

public:
	~CWorkerThreadPool ()
	{
		stop ();
	}

	bool 
	start (size_t threadCount = 0); // 0 -- auto (processor count)

	bool 
	grow (size_t threadCount = 0);

	void 
	stop (ulong_t timeout = -1);

	ref::CPtrT <CWorkerThread> 
	getThread (size_t reserveEventCount = 0);
};

//.............................................................................

} // namespace exe
} // namespace axl
