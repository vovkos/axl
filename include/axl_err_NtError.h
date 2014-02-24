// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2014. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_NTERROR_H

#include "axl_err_ErrorMgr.h"

namespace axl {
namespace err {

//.............................................................................

#if (_AXL_ENV != AXL_ENV_NT)
typedef long NTSTATUS;
#endif

//.............................................................................
	
// {81443347-3BC9-4d5e-8B23-32D780EDB52B}
AXL_RTL_DEFINE_GUID (
	GUID_NtError,
	0x81443347, 0x3bc9, 0x4d5e, 0x8b, 0x23, 0x32, 0xd7, 0x80, 0xed, 0xb5, 0x2b
	);

//.............................................................................
	
class CNtErrorProvider: public CErrorProvider
{
public:
	static
	rtl::CString 
	GetErrorDescription (NTSTATUS Status);

	virtual 
	rtl::CString 
	GetErrorDescription (const TError* pError)
	{
		return GetErrorDescription (pError->m_Code);
	}
};

//.............................................................................

class CNtError: public CError
{
public:
	CNtError ()
	{
	}

	CNtError (NTSTATUS Status)
	{
		Create (Status);
	}

	TError* 
	Create (NTSTATUS Status);
};

//.............................................................................

inline
CError
SetNtError (NTSTATUS Status)
{
	SetError (CNtError (Status));
}

//.............................................................................


} // namespace err
} // namespace axl

