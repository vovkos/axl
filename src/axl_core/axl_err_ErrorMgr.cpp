#include "pch.h"
#include "axl_err_ErrorMgr.h"
#include "axl_err_Errno.h"
#include "axl_dbg_Trace.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	include "axl_err_WinError.h"
#	include "axl_err_NtError.h"
#endif

namespace axl {
namespace err {

//.............................................................................

CErrorMgr::CErrorMgr ()
{
	m_TlsSlot = mt::GetTlsMgr ()->CreateSlot ();

	RegisterProvider (GUID_StdError, rtl::GetSimpleSingleton <CStdErrorProvider> ());
	RegisterProvider (GUID_Errno, rtl::GetSimpleSingleton <CErrnoProvider> ());

#if (_AXL_ENV == AXL_ENV_WIN)
	RegisterProvider (GUID_WinError, rtl::GetSimpleSingleton <CWinErrorProvider> ());
	RegisterProvider (GUID_NtError, rtl::GetSimpleSingleton <CNtErrorProvider> ());
#endif
}

void
CErrorMgr::RegisterProvider (
	const rtl::TGuid& Guid,
	CErrorProvider* pProvider
	)
{
	mt::CScopeLock ScopeLock (&m_Lock);
	m_ProviderMap.Goto (Guid)->m_Value = pProvider;
}

CErrorProvider* 
CErrorMgr::FindProvider (const rtl::TGuid& Guid)
{
	mt::CScopeLock ScopeLock (&m_Lock);
	rtl::CHashTableMapIteratorT <rtl::TGuid, CErrorProvider*> It = m_ProviderMap.Find (Guid);
	return It ? It->m_Value : NULL;
}

EErrorMode
CErrorMgr::SetErrorMode (EErrorMode Mode)
{
	TThreadEntry* pEntry = GetThreadEntry ();
	EErrorMode OldMode = pEntry->m_Mode;
	pEntry->m_Mode = Mode;
	return OldMode;
}

CError
CErrorMgr::GetError ()
{
	TThreadEntry* pEntry = FindThreadEntry ();
	if (pEntry && pEntry->m_Error)
		return pEntry->m_Error;

	return NoError;
}

void
CErrorMgr::SetError (const CError& Error)
{	
	TThreadEntry* pEntry = GetThreadEntry ();
	pEntry->m_Error = Error;
	TThreadEntry* pEntry2 = FindThreadEntry ();
	
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

CErrorMgr::TThreadEntry*
CErrorMgr::GetThreadEntry ()
{
	TThreadEntry* pEntry = FindThreadEntry ();
	if (pEntry)
		return pEntry;

	ref::CPtrT <TThreadEntry> Entry = AXL_REF_NEW (ref::CBoxT <TThreadEntry>);
	mt::GetTlsMgr ()->SetSlotValue (m_TlsSlot, Entry);
	return Entry;
}

//.............................................................................

} // namespace err
} // namespace axl
