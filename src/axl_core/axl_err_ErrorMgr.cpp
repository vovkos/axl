#include "pch.h"
#include "axl_err_ErrorMgr.h"
#include "axl_err_Errno.h"
#include "axl_dbg_Trace.h"
#include "axl_sys_SjljTry.h"

#if (_AXL_OS_WIN)
#	include "axl_err_WinError.h"
#	include "axl_err_NtError.h"
#elif (_AXL_OS_DARWIN)
#	include "axl_sys_drw_MachError.h"
#endif

namespace axl {
namespace err {

//.............................................................................

ErrorMgr::ErrorMgr ()
{
	m_tlsSlot = sys::getTlsMgr ()->createSlot ();

	registerProvider (g_stdErrorGuid, sl::getSimpleSingleton <StdErrorProvider> ());
	registerProvider (g_ErrnoGuid, sl::getSimpleSingleton <ErrnoProvider> ());

#if (_AXL_OS_WIN)
	registerProvider (g_winErrorGuid, sl::getSimpleSingleton <WinErrorProvider> ());
	registerProvider (g_ntErrorGuid, sl::getSimpleSingleton <NtErrorProvider> ());
#elif (_AXL_OS_DARWIN)
	registerProvider (sys::drw::g_MachErrorGuid, sl::getSimpleSingleton <sys::drw::MachErrorProvider> ());
#endif
}

void
ErrorMgr::registerProvider (
	const sl::Guid& guid,
	ErrorProvider* provider
	)
{
	sys::ScopeLock scopeLock (&m_lock);
	m_providerMap.visit (guid)->m_value = provider;
}

ErrorProvider* 
ErrorMgr::findProvider (const sl::Guid& guid)
{
	sys::ScopeLock scopeLock (&m_lock);
	sl::HashTableMapIterator <sl::Guid, ErrorProvider*> it = m_providerMap.find (guid);
	return it ? it->m_value : NULL;
}

Error
ErrorMgr::getLastError ()
{
	ThreadEntry* entry = findThreadEntry ();
	if (entry && !entry->m_error.isEmpty ())
		return entry->m_error;  
	
	return &g_noError;
}

void
ErrorMgr::setError (const Error& error)
{	
	ThreadEntry* entry = getThreadEntry ();
	entry->m_error = error;
}

ErrorMgr::ThreadEntry*
ErrorMgr::getThreadEntry ()
{
	ThreadEntry* entry = findThreadEntry ();
	if (entry)
		return entry;

	ref::Ptr <ThreadEntry> newEntry = AXL_REF_NEW (ref::Box <ThreadEntry>);
	sys::getTlsMgr ()->setSlotValue (m_tlsSlot, newEntry);
	return newEntry;
}

//.............................................................................

} // namespace err
} // namespace axl
