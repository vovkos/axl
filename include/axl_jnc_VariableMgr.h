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

// for non-global static variables and static destructors we need to check 
// whether the item was actually created or not (cause of lazy initialization)

struct TLazyStaticDestruct: rtl::TListLink 
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
	friend class CVariable;

protected:
	CModule* m_pModule;

	rtl::CStdListT <CVariable> m_VariableList;
	rtl::CStdListT <CAlias> m_AliasList;

	// static variables

	rtl::CArrayT <CVariable*> m_StaticVariableArray;
	rtl::CArrayT <CVariable*> m_StaticGcRootArray;
	rtl::CArrayT <CVariable*> m_GlobalStaticVariableArray;
	rtl::CArrayT <CVariable*> m_GlobalStaticDestructArray;
	rtl::CStdListT <TLazyStaticDestruct> m_LazyStaticDestructList;

	rtl::CArrayT <llvm::GlobalVariable*> m_LlvmGlobalVariableArray;

	// tls variables

	rtl::CArrayT <CVariable*> m_TlsVariableArray;
	rtl::CArrayT <CVariable*> m_TlsGcRootArray;
	CStructType* m_pTlsStructType;

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
	GetStaticVariableArray ()
	{
		return m_StaticVariableArray;
	}

	rtl::CArrayT <CVariable*> 
	GetStaticGcRootArray ()
	{
		return m_StaticGcRootArray;
	}

	rtl::CArrayT <CVariable*> 
	GetGlobalStaticVariableArray ()
	{
		return m_GlobalStaticVariableArray;
	}

	rtl::CArrayT <CVariable*> 
	GetGlobalStaticDestructArray ()
	{
		return m_GlobalStaticDestructArray;
	}

	rtl::CConstListT <TLazyStaticDestruct>
	GetLazyStaticDestructList ()
	{
		return m_LazyStaticDestructList;
	}

	rtl::CArrayT <CVariable*> 
	GetTlsVariableArray ()
	{
		return m_TlsVariableArray;
	}

	rtl::CArrayT <CVariable*> 
	GetTlsGcRootArray ()
	{
		return m_TlsGcRootArray;
	}

	CStructType* 
	GetTlsStructType ()
	{
		ASSERT (m_pTlsStructType);
		return m_pTlsStructType;
	}

	void
	AddToLazyStaticDestructList (
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
	CreateTlsStructType ();

	bool
	AllocatePrimeStaticVariable (CVariable* pVariable);

	bool
	AllocatePrimeStaticVariables ();

	bool
	InitializeGlobalStaticVariables ();

	bool
	AllocatePrimeInitializeVariable (CVariable* pVariable)
	{
		return pVariable->m_StorageKind == EStorage_Static ? 
			AllocatePrimeInitializeStaticVariable (pVariable) :
			AllocatePrimeInitializeNonStaticVariable (pVariable);
	}

	void
	AllocateTlsVariable (CVariable* pVariable);

	void
	DeallocateTlsVariableArray (
		const TTlsVariable* ppArray,
		size_t Count
		);

	void 
	DeallocateTlsVariableArray (const rtl::CArrayT <TTlsVariable>& Array)
	{
		DeallocateTlsVariableArray (Array, Array.GetCount ());
	}

protected:	
	bool
	AllocatePrimeInitializeStaticVariable (CVariable* pVariable);

	bool
	AllocatePrimeInitializeNonStaticVariable (CVariable* pVariable);

	void
	CreateStdVariables ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {

