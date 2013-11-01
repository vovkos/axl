// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_TLSMGR_H

#include "axl_rtl_Singleton.h"
#include "axl_rtl_BoxList.h"
#include "axl_rtl_Array.h"
#include "axl_ref_Ptr.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	define AXL_MT_TLS_CALLBACK_SECTION ".CRT$XLD"
#endif

namespace axl {
namespace mt {

//.............................................................................

typedef ref::CPtrT <void> CTlsValue;

class CTlsMgr
{
protected:
	struct TPage
	{
		rtl::CArrayT <rtl::CBoxListEntryT <CTlsValue>*> m_Array;
		rtl::CBoxListT <CTlsValue> m_ValueList;
	};

protected:
#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t m_TlsIdx;
	static bool m_IsDead;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	pthread_key_t m_TlsKey;
#endif

	int32_t m_SlotCount;

public:
	CTlsMgr ();

	~CTlsMgr ();

	size_t
	CreateSlot ()
	{
		return mt::AtomicInc (&m_SlotCount) - 1;
	}

	CTlsValue
	GetSlotValue (size_t Slot);

	CTlsValue
	SetSlotValue (
		size_t Slot,
		const CTlsValue& Value
		);

#if (_AXL_ENV == AXL_ENV_WIN)
	static
	void
	NTAPI
	TlsCallback (
		HANDLE hModule,
		dword_t Reason,
		void* pReserved
		);
#endif

protected:
	TPage*
	GetCurrentThreadPage ();

#if (_AXL_ENV == AXL_ENV_WIN)
	TPage*
	FindCurrentThreadPage ()
	{
		return (TPage*) TlsGetValue (m_TlsIdx);
	}

	void
	SetCurrentThreadPage (TPage* pPage)
	{
		TlsSetValue (m_TlsIdx, pPage);
	}
#elif (_AXL_ENV == AXL_ENV_POSIX)
	TPage*
	FindCurrentThreadPage ()
	{
		return (TPage*) pthread_getspecific (m_TlsKey);
	}

	void
	SetCurrentThreadPage (TPage* pPage)
	{
		pthread_setspecific (m_TlsKey, pPage);
	}

	static
	void
	TlsDestructor (void* p)
	{
		ASSERT (p);
		AXL_MEM_DELETE ((TPage*) p);
	}
#endif
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
CTlsMgr*
GetTlsMgr ()
{
	return rtl::GetSingleton <CTlsMgr> ();
}

//.............................................................................

} // namespace mt
} // namespace axl
