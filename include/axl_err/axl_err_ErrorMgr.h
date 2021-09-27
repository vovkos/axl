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

#define _AXL_ERR_ERRORMGR_H

#include "axl_err_Error.h"
#include "axl_sl_HashTable.h"
#include "axl_sl_String.h"
#include "axl_sl_Singleton.h"
#include "axl_sys_TlsMgr.h"

namespace axl {
namespace err {

//..............................................................................

class ErrorRouter {
public:
	virtual
	void
	routeError(const ErrorHdr* error) = 0;

	virtual
	const char*
	routeErrorDescription(const ErrorHdr* error) = 0;
};

//..............................................................................

class ErrorMgr: public ErrorRouter {
protected:
	struct ThreadEntry {
		Error m_error;
	};

protected:
	sys::Lock m_lock;
	size_t m_tlsSlot;
	sl::DuckTypeHashTable<sl::Guid, ErrorProvider*> m_providerMap;
	ErrorRouter* m_router;

public:
	ErrorMgr();

	void
	registerProvider(
		const sl::Guid& guid,
		ErrorProvider* provider
	);

	ErrorProvider*
	findProvider(const sl::Guid& guid);

	ErrorRouter*
	getRouter() {
		return m_router;
	}

	void
	setRouter(ErrorRouter* router) {
		ASSERT(router != this);
		m_router = router;
	}

	ErrorRef
	getLastError();

	void
	setError(const ErrorRef& error);

	sl::StringRef
	getErrorDescription(const ErrorRef& error);

protected:
	virtual
	void
	routeError(const ErrorHdr* error) {
		setError(error);
	}

	virtual
	const char*
	routeErrorDescription(const ErrorHdr* error);

	ThreadEntry*
	findThreadEntry() {
		return (ThreadEntry*)(void*)sys::getTlsMgr()->getSlotValue(m_tlsSlot);
	}

	ThreadEntry*
	getThreadEntry();
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ErrorMgr*
getErrorMgr() {
	return sl::getSingleton<ErrorMgr> ();
}

//..............................................................................

} // namespace err
} // namespace axl
