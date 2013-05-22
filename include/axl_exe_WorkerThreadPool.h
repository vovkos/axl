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
		CWorkerThread m_Thread;
		size_t m_ReserveEventCount;
		size_t m_PinCount;
	};

	class CThreadPin: public ref::IRefCount
	{
	public:
		CWorkerThreadPool* m_pThreadPool;
		TThreadEntry* m_pThreadEntry;
		size_t m_ReserveEventCount;

	public:
		CThreadPin ();

		~CThreadPin ();
	};

protected:
	friend class CThreadPin;

	mt::CLock m_Lock;
	rtl::CStdListT <TThreadEntry> m_ThreadList;

public:
	~CWorkerThreadPool ()
	{
		Stop ();
	}

	bool 
	Start (size_t ThreadCount = 0); // 0 -- auto (processor count)

	bool 
	Grow (size_t ThreadCount = 0);

	void 
	Stop (ulong_t Timeout = -1);

	ref::CPtrT <CWorkerThread> 
	GetThread (size_t ReserveEventCount = 0);
};

//.............................................................................

} // namespace exe
} // namespace axl
