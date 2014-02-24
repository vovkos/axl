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
		EErrorMode m_Mode;
		CError m_Error;
	};

protected:
	mt::CLock m_Lock;

	size_t m_TlsSlot;

	rtl::CHashTableMapT <
		rtl::TGuid, 
		CErrorProvider*, 
		rtl::CHashDjb2T <rtl::TGuid>,
		rtl::CCmpBinT <rtl::TGuid>
		> m_ProviderMap;

public:
	CErrorMgr ();

	void
	RegisterProvider (
		const rtl::TGuid& Guid,
		CErrorProvider* pProvider
		);

	CErrorProvider* 
	FindProvider (const rtl::TGuid& Guid);

	EErrorMode
	GetErrorMode ()
	{
		TThreadEntry* pEntry = FindThreadEntry ();
		return pEntry ? pEntry->m_Mode : EErrorMode_NoThrow;
	}

	EErrorMode
	SetErrorMode (EErrorMode Mode);

	CError
	GetError ();

	void
	SetError (const CError& Error);

protected:
	TThreadEntry*
	FindThreadEntry ()
	{
		return (TThreadEntry*) (void*) mt::GetTlsMgr ()->GetSlotValue (m_TlsSlot);
	}

	TThreadEntry*
	GetThreadEntry ();
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

inline
CErrorMgr* 
GetErrorMgr ()
{
	return rtl::GetSingleton <CErrorMgr> ();
}

//.............................................................................

} // namespace err
} // namespace axl

