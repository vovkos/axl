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
#include "axl_jnc_ThreadModule.h"

namespace axl {
namespace jnc {

//.............................................................................

class CPreModule
{
protected: 
	CModule* m_pPrevModule;

	CPreModule ()
	{
		m_pPrevModule = SetCurrentThreadModule ((CModule*) this);
	}

	void
	RestorePrevModule ()
	{
		SetCurrentThreadModule (m_pPrevModule);
	}
};

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class CModule: protected CPreModule
{
protected:
	rtl::CString m_FilePath;

	CFunction* m_pConstructor;
	CFunction* m_pDestructor;

	rtl::CArrayT <CModuleItem*> m_CalcLayoutArray;
	rtl::CArrayT <CModuleItem*> m_CompileArray;

public:
	llvm::Module* m_pLlvmModule;

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
	CModule ();

	~CModule ()
	{
		Clear ();
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

	void
	Clear ();

	bool
	Create (const rtl::CString& FilePath);

	bool
	Link (CModule* pModule);

	bool
	Compile ();

protected:
	bool
	CreateDefaultConstructor ();

	bool
	CreateDefaultDestructor ();
};

//.............................................................................

} // namespace jnc {
} // namespace axl {
