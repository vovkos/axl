// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2012. All rights reserved
// Author: Vladimir Gladkov

#pragma once 

#define _AXL_ERR_WINERRORPROVIDER_H

#include "axl_err_ErrorMgr.h"

namespace axl {
namespace err {

//.............................................................................

// {54E100E8-2137-40b8-BCD8-00AC1D0BAA16}
AXL_DEFINE_GUID (
	GUID_WinError,
	0x54e100e8, 0x2137, 0x40b8, 0xbc, 0xd8, 0x0, 0xac, 0x1d, 0xb, 0xaa, 0x16
	);

//.............................................................................

class CWinErrorProvider: public IErrorProvider
{
public:
	AXL_OBJ_SIMPLE_CLASS (CWinErrorProvider, IErrorProvider)

public:
	static 
	void
	Register ()
	{
		err::GetErrorMgr ()->RegisterProvider (GUID_WinError, rtl::GetSimpleSingleton <CWinErrorProvider> ());
	}

	static
	rtl::CString 
	GetErrorDescription (DWORD Error);

	virtual 
	rtl::CString 
	GetErrorDescription (const TError* pError)
	{
		return GetErrorDescription (pError->m_Code);
	}
};

//.............................................................................

class CWinError: public CError
{
public:
	CWinError ()
	{
	}

	CWinError (DWORD Error)
	{
		Create (Error);
	}

	TError* 
	Create (DWORD Error);
};

//.............................................................................

inline
CError
SetWinError (DWORD Error)
{
	SetError (CWinError (Error));
}

//.............................................................................

} // namespace err
} // namespace axl

