// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
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
		rtl::CArrayT <rtl::CBoxListEntryT <CTlsValue>*> m_array;
		rtl::CBoxListT <CTlsValue> m_valueList;
	};

protected:
#if (_AXL_ENV == AXL_ENV_WIN)
	uint_t m_tlsIdx;
	static bool m_isDead;
#elif (_AXL_ENV == AXL_ENV_POSIX)
	pthread_key_t m_tlsKey;
#endif

	int32_t m_slotCount;

public:
	CTlsMgr ();

	~CTlsMgr ();

	size_t
	createSlot ()
	{
		return mt::atomicInc (&m_slotCount) - 1;
	}

	CTlsValue
	getSlotValue (size_t slot);

	CTlsValue
	setSlotValue (
		size_t slot,
		const CTlsValue& value
		);

#if (_AXL_ENV == AXL_ENV_WIN)
	static
	void
	NTAPI
	tlsCallback (
		HANDLE hModule,
		dword_t reason,
		void* reserved
		);
#endif

protected:
	TPage*
	getCurrentThreadPage ();

#if (_AXL_ENV == AXL_ENV_WIN)
	TPage*
	findCurrentThreadPage ()
	{
		return (TPage*) ::TlsGetValue (m_tlsIdx);
	}

	void
	setCurrentThreadPage (TPage* page)
	{
		::TlsSetValue (m_tlsIdx, page);
	}
#elif (_AXL_ENV == AXL_ENV_POSIX)
	TPage*
	findCurrentThreadPage ()
	{
		return (TPage*) pthread_getspecific (m_tlsKey);
	}

	void
	setCurrentThreadPage (TPage* page)
	{
		pthread_setspecific (m_tlsKey, page);
	}

	static
	void
	tlsDestructor (void* p)
	{
		ASSERT (p);
		AXL_MEM_DELETE ((TPage*) p);
	}
#endif
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
CTlsMgr*
getTlsMgr ()
{
	return rtl::getSingleton <CTlsMgr> ();
}

//.............................................................................

} // namespace mt
} // namespace axl
