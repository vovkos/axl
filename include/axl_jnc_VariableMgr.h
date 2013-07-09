// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_Variable.h"
#include "axl_jnc_Alias.h"

namespace axl {
namespace jnc {

class CClassType;
class CFunction;

//.............................................................................

// for static variables and static destructors we need to check 
// whether the item was actually created or not

struct TStaticDestruct: rtl::TListLink 
{
	CVariable* m_pFlagVariable;
	CFunction* m_pDestructor;
	CVariable* m_pVariable; // could be null for static destructors
};

//.............................................................................

enum EStdVariable
{
	EStdVariable_ScopeLevel = 0,
	EStdVariable_GcShadowStackTop,

	EStdVariable__Count,
};

//.............................................................................

class CVariableMgr
{
	friend class CModule;

protected:
	CModule* m_pModule;

	rtl::CStdListT <CVariable> m_VariableList;
	rtl::CStdListT <CAlias> m_AliasList;

	rtl::CArrayT <CVariable*> m_GlobalVariableArray;
	rtl::CArrayT <CVariable*> m_GlobalGcRootArray;
	rtl::CArrayT <llvm::GlobalVariable*> m_LlvmGlobalVariableArray;

	rtl::CArrayT <CVariable*> m_GlobalDestructArray;
	rtl::CStdListT <TStaticDestruct> m_StaticDestructList;

	CVariable* m_StdVariableArray [EStdVariable__Count];

public:
	CVariableMgr ();

	CModule* 
	GetModule ()
	{
		return m_pModule;
	}

	void
	Clear ();

	CVariable*
	GetStdVariable (EStdVariable Variable);
	
	rtl::CArrayT <CVariable*> 
	GetGlobalVariableArray ()
	{
		return m_GlobalVariableArray;
	}

	rtl::CArrayT <CVariable*> 
	GetGlobalGcRootArray ()
	{
		return m_GlobalGcRootArray;
	}

	rtl::CArrayT <CVariable*> 
	GetGlobalDestructArray ()
	{
		return m_GlobalDestructArray;
	}

	rtl::CConstListT <TStaticDestruct>
	GetStaticDestructList ()
	{
		return m_StaticDestructList;
	}

	void
	AddToStaticDestructList (
		CVariable* pFlagVariable,
		CFunction* pDestructor,
		CVariable* pVariable = NULL
		);

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

	CVariable*
	CreateOnceFlagVariable ();		

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
	AllocatePrimeGlobalVariable (CVariable* pVariable);

	bool
	AllocatePrimeGlobalVariables ();

	bool
	InitializeGlobalVariables ();

	bool
	AllocatePrimeInitializeVariable (CVariable* pVariable)
	{
		return pVariable->m_StorageKind == EStorage_Static ? 
			AllocatePrimeInitializeStaticVariable (pVariable) :
			AllocatePrimeInitializeNonStaticVariable (pVariable);
	}

protected:	
	CVariable*
	CreateScopeLevelVariable ();

	bool
	AllocatePrimeInitializeStaticVariable (CVariable* pVariable);

	bool
	AllocatePrimeInitializeNonStaticVariable (CVariable* pVariable);
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

