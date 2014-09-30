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

class CErrorMgr
{
protected:
	struct TThreadEntry
	{
		EErrorMode m_mode;
		CError m_error;
	};

protected:
	mt::CLock m_lock;

	size_t m_tlsSlot;

	rtl::CHashTableMapT <
		rtl::TGuid, 
		CErrorProvider*, 
		rtl::CHashDjb2T <rtl::TGuid>,
		rtl::CCmpBinT <rtl::TGuid>
		> m_providerMap;

public:
	CErrorMgr ();

	void
	registerProvider (
		const rtl::TGuid& guid,
		CErrorProvider* provider
		);

	CErrorProvider* 
	findProvider (const rtl::TGuid& guid);

	EErrorMode
	getErrorMode ()
	{
		TThreadEntry* entry = findThreadEntry ();
		return entry ? entry->m_mode : EErrorMode_NoThrow;
	}

	EErrorMode
	setErrorMode (EErrorMode mode);

	CError
	getError ();

	void
	setError (const CError& error);

protected:
	TThreadEntry*
	findThreadEntry ()
	{
		return (TThreadEntry*) (void*) mt::getTlsMgr ()->getSlotValue (m_tlsSlot);
	}

	TThreadEntry*
	getThreadEntry ();
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
CErrorMgr* 
getErrorMgr ()
{
	return rtl::getSingleton <CErrorMgr> ();
}

//.............................................................................

} // namespace err
} // namespace axl

