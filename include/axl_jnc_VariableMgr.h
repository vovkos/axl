// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Variable.h"
#include "axl_jnc_Alias.h"

namespace axl {
namespace jnc {

class CClassType;

//.............................................................................

class CVariableMgr
{
	friend class CModule;

protected:
	CModule* m_pModule;

	rtl::CStdListT <CVariable> m_VariableList;
	rtl::CStdListT <CAlias> m_AliasList;

	rtl::CArrayT <CVariable*> m_GlobalVariableArray;
	rtl::CBoxListT <CValue> m_StaticDestructList;
	rtl::CArrayT <llvm::GlobalVariable*> m_LlvmGlobalVariableArray;
	
	CVariable* m_pScopeLevelVariable;
	CVariable* m_pShadowStackTopVariable;

public:
	CVariableMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();
	
	rtl::CArrayT <CVariable*> 
	GetGlobalVariableArray ()
	{
		return m_GlobalVariableArray;
	}

	rtl::CConstBoxListT <CValue>
	GetStaticDestructList ()
	{
		return m_StaticDestructList;
	}

	void
	AddToStaticDestructList (const CValue& Value)
	{
		m_StaticDestructList.InsertTail (Value);
	}

	CVariable*
	CreateVariable (
		EStorage StorageKind,
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		CType* pType,
		uint_t PtrTypeFlags = 0,
		rtl::CBoxListT <CToken>* pConstructor = NULL,
		rtl::CBoxListT <CToken>* pInitializer = NULL
		);
		
	llvm::GlobalVariable*
	CreateLlvmGlobalVariable (
		CType* pType,
		const char* pTag
		);

	CAlias*
	CreateAlias (
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		CType* pType,
		rtl::CBoxListT <CToken>* pInitializer
		);

	bool
	AllocateInitializeVariable (
		CVariable* pVariable,
		bool AllocateOnly = false
		);

	bool
	AllocateInitializeGlobalVariables ();

	CVariable*
	GetScopeLevelVariable ()
	{
		ASSERT (m_pScopeLevelVariable); // should be called after AllocateInitializeGlobalVariables ()
		return m_pScopeLevelVariable;
	}

	CVariable*
	GetShadowStackTopVariable ()
	{
		ASSERT (m_pShadowStackTopVariable); // should be called after AllocateInitializeGlobalVariables ()
		return m_pShadowStackTopVariable;
	}

protected:	
	bool
	InitializeVariable (
		CVariable* pVariable,
		const CValue& PtrValue
		);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

