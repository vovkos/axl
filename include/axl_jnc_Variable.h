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

class CVariable: public CUserModuleItem
{
	friend class CVariableMgr;
	friend class CFunctionMgr;
	friend class CParser;

protected:
	CType* m_pType;
	CImportType* m_pType_i;
	uint_t m_PtrTypeFlags;
	rtl::CBoxListT <CToken> m_Constructor;
	rtl::CBoxListT <CToken> m_Initializer;
	rtl::CString m_InitializerString;

	CScope* m_pScope;
	llvm::Value* m_pLlvmValue;
	llvm::Value* m_pLlvmAllocValue;

public:
	CVariable ();

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
	GetConstructor ()
	{
		return m_Constructor;
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

	llvm::Value* 
	GetLlvmAllocValue ()
	{
		return m_pLlvmAllocValue;
	}

protected:
	virtual 
	bool
	CalcLayout ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {


