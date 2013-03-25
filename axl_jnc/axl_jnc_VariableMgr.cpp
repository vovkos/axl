#include "stdafx.h"
#include "axl_jnc_VariableMgr.h"
#include "axl_jnc_Parser.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CVariableMgr::CVariableMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);
	m_pScopeLevelVariable = NULL;
}

void
CVariableMgr::Clear ()
{
	m_VariableList.Clear ();
	m_AliasList.Clear ();
	m_GlobalVariableArray.Clear ();
	m_pScopeLevelVariable = NULL;
}

CVariable*
CVariableMgr::CreateVariable (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CType* pType,
	int PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	if (pType->GetTypeKind () == EType_Class)
		pType = ((CClassType*) pType)->GetClassPtrType ();

	CVariable* pVariable = AXL_MEM_NEW (CVariable);
	pVariable->m_pModule = m_pModule;
	pVariable->m_Name = Name;
	pVariable->m_QualifiedName = QualifiedName;
	pVariable->m_Tag = QualifiedName;
	pVariable->m_pType = pType;
	pVariable->m_PtrTypeFlags = PtrTypeFlags;

	if (pInitializer)
		pVariable->m_Initializer.TakeOver (pInitializer);

	m_VariableList.InsertTail (pVariable);
	return pVariable;
}

CVariable*
CVariableMgr::CreateVariable (
	EStorage StorageKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CType* pType,
	int PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	CVariable* pVariable = CreateVariable (Name, QualifiedName, pType, PtrTypeFlags, pInitializer);
	pVariable->m_StorageKind = StorageKind;

	if (StorageKind == EStorage_Static)
		m_GlobalVariableArray.Append (pVariable);

	return pVariable;
}

llvm::GlobalVariable*
CVariableMgr::CreateLlvmGlobalVariable (
	CType* pType,
	const tchar_t* pTag,
	bool IsThreadLocal
	)
{
	llvm::GlobalVariable* pLlvmValue = new llvm::GlobalVariable (
		*m_pModule->m_pLlvmModule,
		pType->GetLlvmType (),
		false,
		llvm::GlobalVariable::ExternalLinkage,
		(llvm::Constant*) pType->GetZeroValue ().GetLlvmValue (),
		pTag,
		0,
		IsThreadLocal
		);

	m_LlvmGlobalVariableArray.Append (pLlvmValue);
	return pLlvmValue;
}

CAlias*
CVariableMgr::CreateAlias (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CType* pType,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	ASSERT (pInitializer);

	CAlias* pAlias = AXL_MEM_NEW (CAlias);
	pAlias->m_Name = Name;
	pAlias->m_QualifiedName = QualifiedName;
	pAlias->m_Tag = QualifiedName;
	pAlias->m_Initializer.TakeOver (pInitializer);

	m_AliasList.InsertTail (pAlias);

	return pAlias;
}

bool
CVariableMgr::AllocateGlobalVariables ()
{
	bool Result;

	size_t Count = m_GlobalVariableArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		Result = AllocateVariable (m_GlobalVariableArray [i]);
		if (!Result)
			return false;
	}

	return true;
}

bool
CVariableMgr::AllocateVariable (CVariable* pVariable)
{
	ASSERT (!pVariable->m_pLlvmValue);

	CValue PtrValue;
	bool Result = m_pModule->m_OperatorMgr.Allocate (
		pVariable->m_StorageKind, 
		pVariable->m_pType,
		pVariable->m_Tag,
		&PtrValue
		);

	if (!Result)
		return false;
		
	pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();
	return true;
}

bool
CVariableMgr::InitializeVariable (CVariable* pVariable)
{
	bool Result;

	CValue PtrValue;
	Result = m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, pVariable, &PtrValue);
	ASSERT (Result);

	if (pVariable->m_Initializer.IsEmpty ()) // no initializer
		return m_pModule->m_OperatorMgr.InitializeData (pVariable->GetStorageKind (), PtrValue, pVariable->GetType ());

	if (pVariable->m_Initializer.GetHead ()->m_Token == '{') // curly intializer
	{
		Result = m_pModule->m_OperatorMgr.InitializeData (pVariable->GetStorageKind (), PtrValue, pVariable->GetType ());
		if (!Result)
			return false;
	}

	return m_pModule->m_OperatorMgr.ParseInitializer (pVariable, pVariable->GetInitializer ());

	#pragma AXL_TODO ("refine static variable initialization")

/*
	TOnceStmt Stmt;
	m_pModule->m_ControlFlowMgr.OnceStmt_Create (&Stmt);

	Result = 
		m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&Stmt, pVariable->m_Initializer.GetHead ()->m_Pos) &&
		m_pModule->m_OperatorMgr.ParseInitializer (pVariable, pVariable->GetInitializer ());

	m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&Stmt, pVariable->m_Initializer.GetTail ()->m_Pos);
*/

	return true;
}

CVariable*
CVariableMgr::GetScopeLevelVariable ()
{
	if (m_pScopeLevelVariable)
		return m_pScopeLevelVariable;

	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);

	m_pScopeLevelVariable = CreateVariable (EStorage_Static, _T("ScopeLevel"), _T("jnc.ScopeLevel"), pType);

	AllocateVariable (m_pScopeLevelVariable);

	return m_pScopeLevelVariable;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

