// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#define _AXL_MT_TLSMGR_H

#include "axl_sl_BoxList.h"
#include "axl_sl_Array.h"
#include "axl_ref_Ptr.h"
#include "axl_mt_Singleton.h"

#if (_AXL_ENV == AXL_ENV_WIN)
#	define AXL_MT_TLS_CALLBACK_SECTION ".CRT$XLD"
#endif

namespace axl {
namespace mt {

//.............................................................................

typedef ref::Ptr <void> TlsValue;

class TlsMgr
{
protected:
	struct Page
	{
		sl::Array <sl::BoxListEntry <TlsValue>*> m_array;
		sl::BoxList <TlsValue> m_valueList;
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
	TlsMgr ();

	~TlsMgr ();

	size_t
	createSlot ()
	{
		return mt::atomicInc (&m_slotCount) - 1;
	}

	TlsValue
	getSlotValue (size_t slot);

	TlsValue
	setSlotValue (
		size_t slot,
		const TlsValue& value
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
	Page*
	getCurrentThreadPage ();

#if (_AXL_ENV == AXL_ENV_WIN)
	Page*
	findCurrentThreadPage ()
	{
		return (Page*) ::TlsGetValue (m_tlsIdx);
	}

	void
	setCurrentThreadPage (Page* page)
	{
		::TlsSetValue (m_tlsIdx, page);
	}
#elif (_AXL_ENV == AXL_ENV_POSIX)
	Page*
	findCurrentThreadPage ()
	{
		return (Page*) ::pthread_getspecific (m_tlsKey);
	}

	void
	setCurrentThreadPage (Page* page)
	{
		::pthread_setspecific (m_tlsKey, page);
	}

	static
	void
	tlsDestructor (void* p)
	{
		ASSERT (p);
		AXL_MEM_DELETE ((Page*) p);
	}
#endif
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
TlsMgr*
getTlsMgr ()
{
	return mt::getSingleton <TlsMgr> ();
}

//.............................................................................

} // namespace mt
} // namespace axl
