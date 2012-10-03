#include "stdafx.h"
#include "axl_err_ErrorMgr.h"
#include "axl_err_ErrorMgr.h"
#include "axl_mt_Thread.h"
#include "axl_mt_Timer.h"
#include "axl_dbg_Trace.h"

namespace axl {
namespace err {

//.............................................................................

void
CErrorMgr::RegisterProvider (
	const GUID& Guid,
	IErrorProvider* pProvider
	)
{
	m_Lock.Lock ();
	m_ProviderMap.Goto (Guid)->m_Value = pProvider;
	m_Lock.Unlock ();
}

IErrorProvider* 
CErrorMgr::FindProvider (const GUID& Guid)
{
	m_Lock.Lock ();
	rtl::CHashTableMapIteratorT <GUID, IErrorProvider*> It = m_ProviderMap.Find (Guid);
	m_Lock.Unlock ();
	return It ? It->m_Value : NULL;
}

EErrorMode
CErrorMgr::SetErrorMode (EErrorMode Mode)
{
	CThreadEntry* pEntry = GetThreadEntry ();
	EErrorMode OldMode = pEntry->m_Mode;
	pEntry->m_Mode = Mode;
	return OldMode;
}

CError
CErrorMgr::GetError ()
{
	CThreadEntry* pEntry = FindThreadEntry ();
	if (pEntry && pEntry->m_Error)
		return pEntry->m_Error;

	return NoError;
}

void
CErrorMgr::SetError (const CError& Error)
{
	CThreadEntry* pEntry = GetThreadEntry ();
	pEntry->m_Error = Error;

	switch (pEntry->m_Mode)
	{
	case EErrorMode_NoThrow:
		break;

	case EErrorMode_CppException:
		throw Error;

	case EErrorMode_SehException:
	case EErrorMode_SetJmpLongJmp:
		TRACE ("*** unsupported error mode\n");

	default:
		ASSERT (false);
	}
}

CErrorMgr::CThreadEntry*
CErrorMgr::FindThreadEntry ()
{
	ulong_t ThreadId = mt::GetCurrentThreadId ();

	m_Lock.Lock ();
	rtl::CHashTableMapIteratorT <ulong_t, CThreadEntry*> It = m_ThreadMap.Find (ThreadId);
	m_Lock.Unlock ();

	return It ? It->m_Value : NULL;
}

CErrorMgr::CThreadEntry*
CErrorMgr::GetThreadEntry ()
{
	ulong_t ThreadId = mt::GetCurrentThreadId ();

	m_Lock.Lock ();

	CThreadEntry* pThread;

	rtl::CHashTableMapIteratorT <ulong_t, CThreadEntry*> It = m_ThreadMap.Goto (ThreadId);
	if (It->m_Value)
	{
		pThread = It->m_Value;
		m_ThreadList.Remove (pThread);
	}
	else
	{
		pThread = AXL_MEM_NEW (CThreadEntry);
		pThread->m_ThreadId = ThreadId;
		It->m_Value = pThread;
	}

	pThread->m_AccessTime = mt::GetCurrentTime ();
	m_ThreadList.InsertHead (pThread);

	m_Lock.Unlock ();

	return pThread;
}

//.............................................................................

} // namespace err
} // namespace axl
