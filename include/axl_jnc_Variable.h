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

class CVariable: public CUserModuleItem
{
	friend class CVariableMgr;
	friend class CParser;

protected:
	CType* m_pType;
	int m_PtrTypeFlags;
	rtl::CBoxListT <CToken> m_Initializer;
	rtl::CString m_InitializerString;

	CScope* m_pScope;
	llvm::Value* m_pLlvmValue;

public:
	CVariable ();

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

	CScope*
	GetScope ()
	{
		return m_pScope;
	}

	size_t
	GetScopeLevel ()
	{
		return m_pScope ? m_pScope->GetLevel () : 0;
	}

	llvm::Value* 
	GetLlvmValue ()
	{
		return m_pLlvmValue;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
