// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_ImportType.h"
#include "axl_jnc_Scope.h"

namespace axl {
namespace jnc {

class CScope;

//.............................................................................

class CFunctionArg: public CUserModuleItem
{
	friend class CTypeMgr;
	friend class CFunction;
	friend class CClassType;

protected:
	CType* m_pType;
	CImportType* m_pType_i;
	uint_t m_PtrTypeFlags;
	rtl::CBoxListT <CToken> m_Initializer;
	rtl::CString m_InitializerString;

public:
	CFunctionArg ();

	CType* 
	GetType ()
	{
		return m_pType;
	}

	CImportType* 
	GetType_i ()
	{
		return m_pType_i;
	}

	uint_t 
	GetPtrTypeFlags ()
	{
		return m_PtrTypeFlags;
	}

	rtl::CConstBoxListT <CToken> 
	GetInitializer ()
	{
		return m_Initializer;
	}

	rtl::CString 
	GetInitializerString ()
	{
		if (m_InitializerString.IsEmpty ())
			m_InitializerString = CToken::GetTokenListString (m_Initializer);

		return m_InitializerString;
	}

protected:
	virtual 
	bool
	CalcLayout ();
};

//.............................................................................

struct TFunctionArgTuple: rtl::TListLink
{
	CFunctionArg* m_ArgArray [2] [2] [2]; // this x const x volatile
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
