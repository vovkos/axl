// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_ERRORMGR_H

#include "axl_err_Error.h"
#include "axl_sl_HashTable.h"
#include "axl_sl_String.h"
#include "axl_mt_TlsMgr.h"
#include "axl_mt_Singleton.h"

namespace axl {
namespace err {

//.............................................................................

class ErrorMgr
{
protected:
	struct ThreadEntry
	{
		ErrorMode m_mode;
		Error m_error;
	};

protected:
	mt::Lock m_lock;

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

	ErrorMode
	getErrorMode ()
	{
		ThreadEntry* entry = findThreadEntry ();
		return entry ? entry->m_mode : ErrorMode_NoThrow;
	}

	ErrorMode
	setErrorMode (ErrorMode mode);

	Error
	getLastError ();

	void
	setError (const Error& error);

protected:
	ThreadEntry*
	findThreadEntry ()
	{
		return (ThreadEntry*) (void*) mt::getTlsMgr ()->getSlotValue (m_tlsSlot);
	}

	ThreadEntry*
	getThreadEntry ();
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
ErrorMgr* 
getErrorMgr ()
{
	return mt::getSingleton <ErrorMgr> ();
}

//.............................................................................

} // namespace err
} // namespace axl

