// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Type.h"
#include "axl_jnc_Scope.h"

namespace axl {
namespace jnc {

class CScope;

//.............................................................................

class CFunctionArg: public CUserModuleItem
{
protected:
	friend class CTypeMgr;

	CType* m_pType;
	int m_PtrTypeFlags;
	rtl::CBoxListT <CToken> m_Initializer;
	rtl::CString m_InitializerString;

public:
	CFunctionArg ();

	CType* 
	GetType ()
	{
		return m_pType;
	}

	int 
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
};

//.............................................................................

class CFunctionArgTuple: public rtl::TListLink
{
protected:
	friend class CTypeMgr;

	CFunctionArg* m_ArgArray [2] [2]; // const x this

public:
	CFunctionArgTuple ()
	{
		memset (this, 0, sizeof (CFunctionArgTuple));
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
