// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_ERRORMGR_H

#include "axl_err_Error.h"
#include "axl_rtl_HashTable.h"
#include "axl_rtl_Singleton.h"

namespace axl {
namespace err {

//.............................................................................

class CErrorMgr
{
protected:
	class CThreadEntry: public rtl::TListLink
	{
	public:
		ulong_t m_ThreadId;
		ulong_t m_AccessTime;
		EErrorMode m_Mode;
		CError m_Error;
	};

protected:
	mt::CLock m_Lock;

	rtl::CHashTableMapT <
		GUID, 
		IErrorProvider*, 
		rtl::CHashDjb2T <GUID>,
		rtl::CCmpBinT <GUID>
		> m_ProviderMap;

	rtl::CHashTableMapT <
		ulong_t, 
		CThreadEntry*, 
		rtl::CHashIdT <ulong_t>
		> m_ThreadMap;

	rtl::CStdListT <CThreadEntry> m_ThreadList;
	
public:
	void
	RegisterProvider (
		const GUID& Guid,
		IErrorProvider* pProvider
		);

	IErrorProvider* 
	FindProvider (const GUID& Guid);

	EErrorMode
	GetErrorMode ()
	{
		CThreadEntry* pEntry = FindThreadEntry ();
		return pEntry ? pEntry->m_Mode : EErrorMode_NoThrow;
	}

	EErrorMode
	SetErrorMode (EErrorMode Mode);

	CError
	GetError ();

	void
	SetError (const CError& Error);

protected:
	CThreadEntry*
	FindThreadEntry ();

	CThreadEntry*
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

