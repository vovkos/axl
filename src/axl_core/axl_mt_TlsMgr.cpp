#include "pch.h"
#include "axl_mt_TlsMgr.h"

namespace axl {
namespace mt {

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

bool CTlsMgr::m_IsDead = false;

CTlsMgr::CTlsMgr ()
{
	m_TlsIdx = TlsAlloc ();
	m_SlotCount = 0;
}

CTlsMgr::~CTlsMgr ()
{
	TPage* pPage = FindCurrentThreadPage ();
	if (pPage)
	{
		AXL_MEM_DELETE (pPage);
		SetCurrentThreadPage (NULL);
	}

	TlsFree (m_TlsIdx);
	m_IsDead = true;
}

void
NTAPI
CTlsMgr::TlsCallback (
	HANDLE hModule,
	dword_t Reason,
	void* pReserved
	)
{
	if (Reason != DLL_THREAD_DETACH || m_IsDead)
		return;

	CTlsMgr* pThis = GetTlsMgr ();

	TPage* pPage = pThis->FindCurrentThreadPage ();
	if (!pPage)
		return;

	AXL_MEM_DELETE (pPage);

	TlsSetValue (pThis->m_TlsIdx, NULL);
}

#elif (_AXL_ENV == AXL_ENV_POSIX)

CTlsMgr::CTlsMgr ()
{
	pthread_key_create (&m_TlsKey, TlsDestructor);
	m_SlotCount = 0;
}

CTlsMgr::~CTlsMgr ()
{
	TPage* pPage = FindCurrentThreadPage ();
	if (pPage)
	{
		AXL_MEM_DELETE (pPage);
		SetCurrentThreadPage (NULL);
	}

	pthread_key_delete (m_TlsKey);
}

#endif

CTlsValue
CTlsMgr::GetSlotValue (size_t Slot)
{
	TPage* pPage = FindCurrentThreadPage ();
	if (!pPage)
		return ref::EPtr_Null;

	size_t Count = pPage->m_Array.GetCount ();
	if (Slot >= Count)
		return ref::EPtr_Null;

	rtl::CBoxListEntryT <CTlsValue>* pEntry = pPage->m_Array [Slot];
	if (!pEntry)
		return ref::EPtr_Null;

	return pEntry->m_Value;
}

CTlsValue
CTlsMgr::SetSlotValue (
	size_t Slot,
	const CTlsValue& Value
	)
{
	TPage* pPage = GetCurrentThreadPage ();

	size_t Count = pPage->m_Array.GetCount ();
	if (Slot >= Count)
	{
		if (!Value)
			return CTlsValue ();

		pPage->m_Array.SetCount (Slot + 1);
	}

	rtl::CBoxListEntryT <CTlsValue>* pEntry = pPage->m_Array [Slot];

	CTlsValue OldValue;

	if (pEntry)
	{
		OldValue = pEntry->m_Value;

		if (Value)
		{
			pEntry->m_Value = Value;
		}
		else
		{
			pPage->m_ValueList.Remove (pEntry);
			pPage->m_Array [Slot] = NULL;
		}
	}
	else if (Value)
	{
		pEntry = pPage->m_ValueList.InsertTail (Value).GetEntry ();
		pPage->m_Array [Slot] = pEntry;
	}

	return OldValue;
}

CTlsMgr::TPage*
CTlsMgr::GetCurrentThreadPage ()
{
	TPage* pPage = FindCurrentThreadPage ();
	if (pPage)
		return pPage;

	pPage = AXL_MEM_NEW (TPage);
	SetCurrentThreadPage (pPage);
	return pPage;
}

//.............................................................................

} // namespace mt
} // namespace axl

//.............................................................................

#if (_AXL_ENV == AXL_ENV_WIN)

#pragma section (AXL_MT_TLS_CALLBACK_SECTION, long, read)

extern "C"
__declspec(allocate (AXL_MT_TLS_CALLBACK_SECTION))
PIMAGE_TLS_CALLBACK g_axl_mt_pfTlsCallback = axl::mt::CTlsMgr::TlsCallback;

#ifdef _WIN64
#	pragma comment (linker, "/INCLUDE:_tls_used")
#	pragma comment (linker, "/INCLUDE:g_axl_mt_pfTlsCallback")
#else
#	pragma comment (linker, "/INCLUDE:__tls_used")
#	pragma comment (linker, "/INCLUDE:_g_axl_mt_pfTlsCallback")
#endif

#endif

//.............................................................................

