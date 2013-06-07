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
	rtl::CArrayT <CVariable*> m_InitalizedGlobalVariableArray;
	rtl::CArrayT <CVariable*> m_StaticDestructArray; // variables allocated for static new instances of classes with destructors
	rtl::CArrayT <llvm::GlobalVariable*> m_LlvmGlobalVariableArray;

	CVariable* m_pScopeLevelVariable;

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

	rtl::CArrayT <CVariable*>
	GetStaticDestructArray ()
	{
		return m_StaticDestructArray;
	}

	CVariable*
	CreateVariable (
		EStorage StorageKind,
		const rtl::CString& Name,
		const rtl::CString& QualifiedName,
		CType* pType,
		int PtrTypeFlags = 0,
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
	AllocateVariable (CVariable* pVariable);

	bool
	InitializeVariable (CVariable* pVariable);

	bool
	AllocateGlobalVariables ();

	bool
	InitializeGlobalVariables ();

	CVariable*
	GetScopeLevelVariable ()
	{
		ASSERT (m_pScopeLevelVariable); // should be called after AllocateGlobalVariables ()
		return m_pScopeLevelVariable;
	}
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

