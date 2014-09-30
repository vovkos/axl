#include "pch.h"
#include "axl_mt_TlsMgr.h"

namespace axl {
namespace mt {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

bool CTlsMgr::m_isDead = false;

CTlsMgr::CTlsMgr ()
{
	m_tlsIdx = ::TlsAlloc ();
	m_slotCount = 0;
}

CTlsMgr::~CTlsMgr ()
{
	TPage* page = findCurrentThreadPage ();
	if (page)
	{
		AXL_MEM_DELETE (page);
		setCurrentThreadPage (NULL);
	}

	::TlsFree (m_tlsIdx);
	m_isDead = true;
}

void
NTAPI
CTlsMgr::tlsCallback (
	HANDLE hModule,
	dword_t reason,
	void* reserved
	)
{
	if (reason != DLL_THREAD_DETACH || m_isDead)
		return;

	CTlsMgr* self = getTlsMgr ();

	TPage* page = self->findCurrentThreadPage ();
	if (!page)
		return;

	AXL_MEM_DELETE (page);

	::TlsSetValue (self->m_tlsIdx, NULL);
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

CTlsMgr::CTlsMgr ()
{
	pthread_key_create (&m_tlsKey, tlsDestructor);
	m_slotCount = 0;
}

CTlsMgr::~CTlsMgr ()
{
	TPage* page = findCurrentThreadPage ();
	if (page)
	{
		AXL_MEM_DELETE (page);
		setCurrentThreadPage (NULL);
	}

	pthread_key_delete (m_tlsKey);
}

#endif

CTlsValue
CTlsMgr::getSlotValue (size_t slot)
{
	TPage* page = findCurrentThreadPage ();
	if (!page)
		return ref::EPtr_Null;

	size_t count = page->m_array.getCount ();
	if (slot >= count)
		return ref::EPtr_Null;

	rtl::CBoxListEntryT <CTlsValue>* entry = page->m_array [slot];
	if (!entry)
		return ref::EPtr_Null;

	return entry->m_value;
}

CTlsValue
CTlsMgr::setSlotValue (
	size_t slot,
	const CTlsValue& value
	)
{
	TPage* page = getCurrentThreadPage ();

	size_t count = page->m_array.getCount ();
	if (slot >= count)
	{
		if (!value)
			return CTlsValue ();

		page->m_array.setCount (slot + 1);
	}

	rtl::CBoxListEntryT <CTlsValue>* entry = page->m_array [slot];

	CTlsValue oldValue;

	if (entry)
	{
		oldValue = entry->m_value;

		if (value)
		{
			entry->m_value = value;
		}
		else
		{
			page->m_valueList.remove (entry);
			page->m_array [slot] = NULL;
		}
	}
	else if (value)
	{
		entry = page->m_valueList.insertTail (value).getEntry ();
		page->m_array [slot] = entry;
	}

	return oldValue;
}

CTlsMgr::TPage*
CTlsMgr::getCurrentThreadPage ()
{
	TPage* page = findCurrentThreadPage ();
	if (page)
		return page;

	page = AXL_MEM_NEW (TPage);
	setCurrentThreadPage (page);
	return page;
}

//.............................................................................

} // namespace mt
} // namespace axl

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

#pragma section (AXL_MT_TLS_CALLBACK_SECTION, long, read)

extern "C"
__declspec(allocate (AXL_MT_TLS_CALLBACK_SECTION))
PIMAGE_TLS_CALLBACK g_axl_mt_pfTlsCallback = axl::mt::CTlsMgr::tlsCallback;

#ifdef _WIN64
#	pragma comment (linker, "/INCLUDE:_tls_used")
#	pragma comment (linker, "/INCLUDE:g_axl_mt_pfTlsCallback")
#else
#	pragma comment (linker, "/INCLUDE:__tls_used")
#	pragma comment (linker, "/INCLUDE:_g_axl_mt_pfTlsCallback")
#endif

#endif

//.............................................................................

