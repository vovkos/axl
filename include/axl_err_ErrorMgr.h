// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_ERRORMGR_H

#include "axl_err_Error.h"
#include "axl_rtl_HashTable.h"
#include "axl_rtl_Singleton.h"
#include "axl_rtl_String.h"
#include "axl_mt_TlsMgr.h"

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

	rtl::HashTableMap <
		rtl::Guid, 
		ErrorProvider*, 
		rtl::HashDjb2 <rtl::Guid>,
		rtl::CmpBin <rtl::Guid>
		> m_providerMap;

public:
	ErrorMgr ();

	void
	registerProvider (
		const rtl::Guid& guid,
		ErrorProvider* provider
		);

	ErrorProvider* 
	findProvider (const rtl::Guid& guid);

	ErrorMode
	getErrorMode ()
	{
		ThreadEntry* entry = findThreadEntry ();
		return entry ? entry->m_mode : ErrorMode_NoThrow;
	}

	ErrorMode
	setErrorMode (ErrorMode mode);

	Error
	getError ();

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
	return rtl::getSingleton <ErrorMgr> ();
}

//.............................................................................

} // namespace err
} // namespace axl

