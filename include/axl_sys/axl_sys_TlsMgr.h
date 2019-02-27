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

#pragma once

#define _AXL_SYS_TLSMGR_H

#include "axl_sl_BoxList.h"
#include "axl_sl_Array.h"
#include "axl_ref_Ptr.h"
#include "axl_sl_Singleton.h"

#if (_AXL_OS_WIN)
#	define AXL_SYS_TLS_CALLBACK_SECTION ".CRT$XLD"
#endif

namespace axl {
namespace sys {

//..............................................................................

typedef ref::Ptr<void> TlsValue;

class TlsMgr
{
protected:
	struct Page
	{
		sl::Array<sl::BoxListEntry<TlsValue>*> m_array;
		sl::BoxList<TlsValue> m_valueList;
	};

protected:
#if (_AXL_OS_WIN)
	uint_t m_tlsIdx;
	static bool m_isDead;
#elif (_AXL_OS_POSIX)
	pthread_key_t m_tlsKey;
#endif

	int32_t m_slotCount;

public:
	TlsMgr();

	~TlsMgr();

	size_t
	createSlot()
	{
		return sys::atomicInc(&m_slotCount) - 1;
	}

	TlsValue
	getSlotValue(size_t slot);

	TlsValue
	setSlotValue(
		size_t slot,
		const TlsValue& value
		);

#if (_AXL_OS_WIN)
	static
	void
	NTAPI
	tlsCallback(
		HANDLE hModule,
		dword_t reason,
		void* reserved
		);
#endif

protected:
	Page*
	getCurrentThreadPage();

#if (_AXL_OS_WIN)
	Page*
	findCurrentThreadPage()
	{
		return (Page*) ::TlsGetValue(m_tlsIdx);
	}

	void
	setCurrentThreadPage(Page* page)
	{
		::TlsSetValue(m_tlsIdx, page);
	}
#elif (_AXL_OS_POSIX)
	Page*
	findCurrentThreadPage()
	{
		return (Page*) ::pthread_getspecific(m_tlsKey);
	}

	void
	setCurrentThreadPage(Page* page)
	{
		::pthread_setspecific(m_tlsKey, page);
	}

	static
	void
	tlsDestructor(void* p)
	{
		ASSERT(p);
		AXL_MEM_DELETE((Page*)p);
	}
#endif
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
TlsMgr*
getTlsMgr()
{
	return sl::getSingleton<TlsMgr> ();
}

//..............................................................................

} // namespace sys
} // namespace axl
