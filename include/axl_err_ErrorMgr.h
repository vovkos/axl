// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2015. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_ERRORMGR_H

#include "axl_err_Error.h"
#include "axl_sl_HashTable.h"
#include "axl_sl_String.h"
#include "axl_sl_Singleton.h"
#include "axl_sys_TlsMgr.h"

namespace axl {
namespace err {

//.............................................................................

class ErrorMgr
{
protected:
	struct ThreadEntry
	{
		Error m_error;
	};

protected:
	sys::Lock m_lock;

	size_t m_tlsSlot;

	sl::HashTableMap <
		sl::Guid, 
		ErrorProvider*, 
		sl::HashDjb2 <sl::Guid>,
		sl::CmpBin <sl::Guid>
		> m_providerMap;

public:
	ErrorMgr ();

	void
	registerProvider (
		const sl::Guid& guid,
		ErrorProvider* provider
		);

	ErrorProvider* 
	findProvider (const sl::Guid& guid);

	ErrorRef
	getLastError ();

	void
	setError (const ErrorRef& error);

protected:
	ThreadEntry*
	findThreadEntry ()
	{
		return (ThreadEntry*) (void*) sys::getTlsMgr ()->getSlotValue (m_tlsSlot);
	}

	ThreadEntry*
	getThreadEntry ();
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ErrorMgr* 
getErrorMgr ()
{
	return sl::getSingleton <ErrorMgr> ();
}

//.............................................................................

} // namespace err
} // namespace axl

