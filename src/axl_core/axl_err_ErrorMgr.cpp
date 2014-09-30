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
	m_tlsSlot = mt::getTlsMgr ()->createSlot ();

	registerProvider (GUID_StdError, rtl::getSimpleSingleton <CStdErrorProvider> ());
	registerProvider (GUID_Errno, rtl::getSimpleSingleton <CErrnoProvider> ());

#if (_AXL_ENV == AXL_ENV_WIN)
	registerProvider (GUID_WinError, rtl::getSimpleSingleton <CWinErrorProvider> ());
	registerProvider (GUID_NtError, rtl::getSimpleSingleton <CNtErrorProvider> ());
#endif
}

void
CErrorMgr::registerProvider (
	const rtl::TGuid& guid,
	CErrorProvider* provider
	)
{
	mt::CScopeLock scopeLock (&m_lock);
	m_providerMap.visit (guid)->m_value = provider;
}

CErrorProvider* 
CErrorMgr::findProvider (const rtl::TGuid& guid)
{
	mt::CScopeLock scopeLock (&m_lock);
	rtl::CHashTableMapIteratorT <rtl::TGuid, CErrorProvider*> it = m_providerMap.find (guid);
	return it ? it->m_value : NULL;
}

EErrorMode
CErrorMgr::setErrorMode (EErrorMode mode)
{
	TThreadEntry* entry = getThreadEntry ();
	EErrorMode oldMode = entry->m_mode;
	entry->m_mode = mode;
	return oldMode;
}

CError
CErrorMgr::getError ()
{
	TThreadEntry* entry = findThreadEntry ();
	if (entry && entry->m_error)
		return entry->m_error;

	return noError;
}

void
CErrorMgr::setError (const CError& error)
{	
	TThreadEntry* entry = getThreadEntry ();
	entry->m_error = error;
	TThreadEntry* entry2 = findThreadEntry ();
	
	switch (entry->m_mode)
	{
	case EErrorMode_NoThrow:
		break;

	case EErrorMode_CppException:
		throw error;

	case EErrorMode_SehException:
	case EErrorMode_SetJmpLongJmp:
		TRACE ("*** unsupported error mode\n");

	default:
		ASSERT (false);
	}
}

CErrorMgr::TThreadEntry*
CErrorMgr::getThreadEntry ()
{
	TThreadEntry* entry = findThreadEntry ();
	if (entry)
		return entry;

	ref::CPtrT <TThreadEntry> newEntry = AXL_REF_NEW (ref::CBoxT <TThreadEntry>);
	mt::getTlsMgr ()->setSlotValue (m_tlsSlot, newEntry);
	return newEntry;
}

//.............................................................................

} // namespace err
} // namespace axl
