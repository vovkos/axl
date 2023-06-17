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
#include "axl_sys_TlsMgr.h"

namespace axl {
namespace sys {

//..............................................................................

#if (_AXL_OS_WIN)

bool TlsMgr::m_isDestructed = false;

TlsMgr::TlsMgr() {
	m_tlsIdx = ::TlsAlloc();
	m_slotCount = 0;
}

TlsMgr::~TlsMgr() {
	Page* page = findCurrentThreadPage();
	if (page) {
		AXL_MEM_DELETE(page);
		setCurrentThreadPage(NULL);
	}

	::TlsFree(m_tlsIdx);
	m_isDestructed = true;
}

void
NTAPI
TlsMgr::tlsCallback(
	HANDLE hModule,
	dword_t reason,
	void* reserved
) {
	if (reason != DLL_THREAD_DETACH || m_isDestructed)
		return;

	TlsMgr* self = getTlsMgr();

	Page* page = self->findCurrentThreadPage();
	if (!page)
		return;

	AXL_MEM_DELETE(page);

	::TlsSetValue(self->m_tlsIdx, NULL);
}

#elif (_AXL_OS_POSIX)

TlsMgr::TlsMgr() {
	::pthread_key_create(&m_tlsKey, tlsDestructor);
	m_slotCount = 0;
}

TlsMgr::~TlsMgr() {
	Page* page = findCurrentThreadPage();
	if (page) {
		AXL_MEM_DELETE(page);
		setCurrentThreadPage(NULL);
	}

	::pthread_key_delete(m_tlsKey);
}

#endif

void*
TlsMgr::setSlotValue(
	size_t slot,
	const TlsValue& value
) {
	Page* page = getCurrentThreadPage();

	size_t count = page->m_array.getCount();
	if (slot >= count) {
		if (!value)
			return NULL;

		page->m_array.setCountZeroConstruct(slot + 1);
	}

	sl::BoxListEntry<TlsValue>* entry = page->m_array[slot];

	void* prev = NULL;

	if (entry) {
		prev = entry->m_value;

		if (value) {
			entry->m_value = value;
		} else {
			page->m_valueList.remove(entry);
			page->m_array[slot] = NULL;
		}
	} else if (value) {
		entry = page->m_valueList.insertTail(value).getEntry();
		page->m_array[slot] = entry;
	}

	return prev;
}

TlsMgr::Page*
TlsMgr::getCurrentThreadPage() {
	Page* page = findCurrentThreadPage();
	if (page)
		return page;

	page = AXL_MEM_NEW(Page);
	setCurrentThreadPage(page);
	return page;
}

//..............................................................................

#if (_AXL_OS_WIN)

size_t
createSimpleTlsSlot() {
	dword_t slot = ::TlsAlloc();
	if (slot == TLS_OUT_OF_INDEXES) {
		err::setLastSystemError();
		return -1;
	}

	return slot;
}

bool
deleteSimpleTlsSlot(size_t slot) {
	bool_t result = ::TlsFree(slot);
	return err::complete(result);
}

#else

size_t
createSimpleTlsSlot() {
	pthread_key_t key;
	int result = ::pthread_key_create(&key, NULL);
	if (result != 0) {
		err::setError(result);
		return -1;
	}

	ASSERT(sizeof(key) <= sizeof(size_t));
	return (size_t)key;
}

bool
deleteSimpleTlsSlot(size_t key) {
	int result = ::pthread_key_delete((pthread_key_t)key);
	return result != 0 ? err::fail(result) : true;
}

#endif

//..............................................................................


} // namespace sys
} // namespace axl

//..............................................................................

#if (_AXL_OS_WIN)

#pragma section(AXL_SYS_TLS_CALLBACK_SECTION, long, read)

extern "C"
__declspec(allocate(AXL_SYS_TLS_CALLBACK_SECTION))
PIMAGE_TLS_CALLBACK axl_g_sys_pfTlsCallback = axl::sys::TlsMgr::tlsCallback;

#ifdef _WIN64
#	pragma comment(linker, "/INCLUDE:_tls_used")
#	pragma comment(linker, "/INCLUDE:axl_g_sys_pfTlsCallback")
#else
#	pragma comment(linker, "/INCLUDE:__tls_used")
#	pragma comment(linker, "/INCLUDE:_axl_g_sys_pfTlsCallback")
#endif

#endif

//..............................................................................
