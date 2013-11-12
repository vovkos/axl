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
#include "axl_jnc_LlvmIrBuilder.h"
#include "axl_jnc_LlvmDiBuilder.h"
#include "axl_mt_TlsSlot.h"

namespace axl {
namespace jnc {

class CModule;

//.............................................................................

enum EModuleFlag
{
	EModuleFlag_DebugInfo  = 0x0001,
	EModuleFlag_IrComments = 0x0002,
};

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
	rtl::CString m_FileName;
	rtl::CString m_DirName;

	uint_t m_Flags;

	CFunction* m_pConstructor;
	CFunction* m_pDestructor;

	rtl::CArrayT <CModuleItem*> m_CalcLayoutArray;
	rtl::CArrayT <CModuleItem*> m_CompileArray;

	llvm::Module* m_pLlvmModule;
	llvm::DIFile m_LlvmDiFile;

public:
	CTypeMgr m_TypeMgr;
	CAttributeMgr m_AttributeMgr;
	CNamespaceMgr m_NamespaceMgr;
	CFunctionMgr m_FunctionMgr;
	CVariableMgr m_VariableMgr;
	CConstMgr m_ConstMgr;
	CControlFlowMgr m_ControlFlowMgr;
	COperatorMgr m_OperatorMgr;
	CLlvmIrBuilder m_LlvmIrBuilder;
	CLlvmDiBuilder m_LlvmDiBuilder;

public:
	CModule ()
	{
		m_pLlvmModule = NULL;
	}

	~CModule ()
	{
		Clear ();
	}

	llvm::LLVMContext*
	GetLlvmContext ()
	{
		return &m_pLlvmModule->getContext ();
	}

	llvm::Module*
	GetLlvmModule ()
	{
		return m_pLlvmModule;
	}

	llvm::DIFile
	GetLlvmDiFile ()
	{
		return m_LlvmDiFile;
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

	rtl::CString
	GetFileName ()
	{
		return m_FileName;
	}

	rtl::CString
	GetDirName ()
	{
		return m_DirName;
	}

	uint_t
	GetFlags ()
	{
		return m_Flags;
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

	bool
	SetFunctionPointer (
		llvm::ExecutionEngine* pLlvmExecutionEngine,
		const CQualifiedName& Name,
		void* pf
		);

	void
	MarkForLayout (
		CModuleItem* pItem,
		bool IsForced = false
		);

	void
	MarkForCompile (CModuleItem* pItem);

	CModuleItem*
	GetItemByName (const char* pName);

	CClassType*
	GetClassTypeByName (const char* pName);

	CFunction*
	GetFunctionByName (const char* pName);

	CProperty*
	GetPropertyByName (const char* pName);

	bool
	Create (
		const rtl::CString& FilePath,
		llvm::Module* pLlvmModule,
		uint_t Flags = 0
		);

	void
	Clear ();

	bool
	Parse (
		const char* pFilePath,
		const char* pSource,
		size_t Length = -1
		);

	bool
	ParseFile (const char* pFilePath);

	bool
	Link (CModule* pModule);

	bool
	Compile ();

	rtl::CString
	GetLlvmIrString ();

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
