// This file is part of AXL (R) Library
// Tibbo Technology Inc (C) 2004-2013. All rights reserved
// Author: Vladimir Gladkov

#pragma once

#include "axl_jnc_AttributeMgr.h"
#include "axl_jnc_TypeMgr.h"
#include "axl_jnc_NamespaceMgr.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_VariableMgr.h"
#include "axl_jnc_ConstMgr.h"
#include "axl_jnc_ControlFlowMgr.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_LlvmBuilder.h"
#include "axl_mt_TlsSlot.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

// makes it convenient to initialize childs (especially operators)

class CPreModule
{
protected: 
	CModule* m_pPrevModule;

	CPreModule ()
	{
		m_pPrevModule = mt::SetTlsSlotValue <CModule> ((CModule*) this);
	}

	void
	RestorePrevModule ()
	{
		mt::SetTlsSlotValue <CModule> (m_pPrevModule);
	}
};

//.............................................................................

class CModule: CPreModule
{
protected:
	rtl::CString m_FilePath;

	CFunction* m_pConstructor;
	CFunction* m_pDestructor;

	rtl::CArrayT <CModuleItem*> m_CalcLayoutArray;
	rtl::CArrayT <CModuleItem*> m_CompileArray;

	llvm::Module* m_pLlvmModule;

public:
	CTypeMgr m_TypeMgr;
	CAttributeMgr m_AttributeMgr;
	CNamespaceMgr m_NamespaceMgr;
	CFunctionMgr m_FunctionMgr;
	CVariableMgr m_VariableMgr;
	CConstMgr m_ConstMgr;
	CControlFlowMgr m_ControlFlowMgr;
	COperatorMgr m_OperatorMgr;	
	CLlvmBuilder m_LlvmBuilder;

public:
	CModule ()
	{
		m_pLlvmModule = NULL;
	}

	~CModule ()
	{
		Clear ();
	}

	llvm::Module* 
	GetLlvmModule ()
	{
		return m_pLlvmModule;
	}

	CType*
	GetSimpleType (EType TypeKind)
	{
		return m_TypeMgr.GetPrimitiveType (TypeKind);
	}

	CType*
	GetSimpleType (EStdType StdType)
	{
		return m_TypeMgr.GetStdType (StdType);
	}

	rtl::CString
	GetFilePath ()
	{
		return m_FilePath;
	}

	CFunction* 
	GetConstructor ()
	{
		return m_pConstructor;
	}

	bool
	SetConstructor (CFunction* pFunction);

	CFunction* 
	GetDestructor ()
	{
		return m_pDestructor;
	}

	bool
	SetDestructor (CFunction* pFunction);

	void
	SetFunctionPointer (
		llvm::ExecutionEngine* pLlvmExecutionEngine,
		CFunction* pFunction,
		void* pf
		)
	{
		pLlvmExecutionEngine->addGlobalMapping (pFunction->GetLlvmFunction (), pf);
	}

	void
	SetFunctionPointer (
		llvm::ExecutionEngine* pLlvmExecutionEngine,
		EStdFunc FuncKind,
		void* pf
		)
	{
		SetFunctionPointer (pLlvmExecutionEngine, m_FunctionMgr.GetStdFunction (FuncKind), pf);			
	}

	bool
	SetFunctionPointer (
		llvm::ExecutionEngine* pLlvmExecutionEngine,
		const char* pName,
		void* pf
		);

	void 
	MarkForLayout (CModuleItem* pItem);

	void 
	MarkForCompile (CModuleItem* pItem);

	bool
	Create (
		const rtl::CString& FilePath,
		llvm::Module* pLlvmModule
		);

	void
	Clear ();

	bool
	Link (CModule* pModule);

	bool
	Compile ();

protected:
	bool
	CreateDefaultConstructor ();

	void
	CreateDefaultDestructor ();
};

//.............................................................................

typedef mt::CScopeTlsSlotT <CModule> CScopeThreadModule;

inline
CModule*
GetCurrentThreadModule ()
{
	return mt::GetTlsSlotValue <CModule> ();
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
