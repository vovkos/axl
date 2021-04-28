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
#include "axl_err_ErrorMgr.h"
#include "axl_err_Errno.h"
#include "axl_sys_SjljTry.h"

#if (_AXL_OS_WIN)
#	include "axl_sys_win_WinError.h"
#	include "axl_sys_win_NtStatus.h"
#elif (_AXL_OS_DARWIN)
#	include "axl_sys_drw_MachError.h"
#endif

namespace axl {
namespace err {

//..............................................................................

ErrorMgr::ErrorMgr()
{
	m_tlsSlot = sys::getTlsMgr()->createSlot();
	m_router = NULL;

	registerProvider(g_stdErrorGuid, sl::getSimpleSingleton<StdErrorProvider> ());
	registerProvider(g_errnoGuid, sl::getSimpleSingleton<ErrnoProvider> ());

#if (_AXL_OS_WIN)
	registerProvider(sys::win::g_winErrorGuid, sl::getSimpleSingleton<sys::win::WinErrorProvider> ());
	registerProvider(sys::win::g_ntStatusGuid, sl::getSimpleSingleton<sys::win::NtStatusProvider> ());
#elif (_AXL_OS_DARWIN)
	registerProvider(sys::drw::g_MachErrorGuid, sl::getSimpleSingleton<sys::drw::MachErrorProvider> ());
#endif
}

void
ErrorMgr::registerProvider(
	const sl::Guid& guid,
	ErrorProvider* provider
	)
{
	sys::ScopeLock scopeLock(&m_lock);
	m_providerMap.visit(guid)->m_value = provider;
}

ErrorProvider*
ErrorMgr::findProvider(const sl::Guid& guid)
{
	sys::ScopeLock scopeLock(&m_lock);
	sl::HashTableIterator<sl::Guid, ErrorProvider*> it = m_providerMap.find(guid);
	return it ? it->m_value : NULL;
}

ErrorRef
ErrorMgr::getLastError()
{
	ThreadEntry* entry = findThreadEntry();
	if (entry && !entry->m_error.isEmpty())
		return entry->m_error;

	return &g_noError;
}

void
ErrorMgr::setError(const ErrorRef& error)
{
	ThreadEntry* entry = getThreadEntry();
	entry->m_error = error;

	if (m_router)
		m_router->routeError(error);
}

sl::StringRef
ErrorMgr::getErrorDescription(const ErrorRef& error)
{
	if (m_router)
		return m_router->routeErrorDescription(error);

	ErrorProvider* provider = findProvider(error->m_guid);

	return provider ?
		provider->getErrorDescription(error) :
		sl::formatString("%s::%d", error->m_guid.getString().sz(), error->m_code);
}

const char*
ErrorMgr::routeErrorDescription(const ErrorHdr* error)
{
	if (m_router)
		return m_router->routeErrorDescription(error);

	static volatile int32_t flag = 0;
	sl::String* buffer = sl::getSingleton<sl::String>(&flag);
	*buffer = getErrorDescription(error);
	return *buffer;
}

ErrorMgr::ThreadEntry*
ErrorMgr::getThreadEntry()
{
	ThreadEntry* entry = findThreadEntry();
	if (entry)
		return entry;

	rc::Ptr<ThreadEntry> newEntry = AXL_RC_NEW(rc::Box<ThreadEntry>);
	sys::getTlsMgr()->setSlotValue(m_tlsSlot, newEntry);
	return newEntry;
}

//..............................................................................

} // namespace err
} // namespace axl
