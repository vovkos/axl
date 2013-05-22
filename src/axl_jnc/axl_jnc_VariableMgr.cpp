#include "pch.h"
#include "axl_jnc_VariableMgr.h"
#include "axl_jnc_Parser.llk.h"
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
	m_InitalizedGlobalVariableArray.Clear ();
	m_StaticDestructArray.Clear ();
	m_pScopeLevelVariable = NULL;
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
	if (pType->GetTypeKind () == EType_Class)
		pType = ((CClassType*) pType)->GetClassPtrType ();

	CVariable* pVariable = AXL_MEM_NEW (CVariable);
	pVariable->m_pModule = m_pModule;
	pVariable->m_Name = Name;
	pVariable->m_QualifiedName = QualifiedName;
	pVariable->m_Tag = QualifiedName;
	pVariable->m_pType = pType;
	pVariable->m_StorageKind = StorageKind;
	pVariable->m_PtrTypeFlags = PtrTypeFlags;

	if (pInitializer)
		pVariable->m_Initializer.TakeOver (pInitializer);

	m_VariableList.InsertTail (pVariable);

	if (StorageKind == EStorage_Static)	
	{
		pVariable->m_pLlvmValue = CreateLlvmGlobalVariable (pType, QualifiedName);
		m_GlobalVariableArray.Append (pVariable);

		if (pInitializer)
			m_InitalizedGlobalVariableArray.Append (pVariable);
	}

	return pVariable;
}

llvm::GlobalVariable*
CVariableMgr::CreateLlvmGlobalVariable (
	CType* pType,
	const char* pTag,
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
CVariableMgr::InitializeGlobalVariables ()
{
	bool Result;

	size_t Count = m_GlobalVariableArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = m_GlobalVariableArray [i];
		m_pModule->m_LlvmBuilder.CreateStore (pVariable->GetType ()->GetZeroValue (), pVariable);
	}

	Count = m_InitalizedGlobalVariableArray.GetCount ();	
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = m_InitalizedGlobalVariableArray [i];

		Result = m_pModule->m_OperatorMgr.ParseInitializer (pVariable, pVariable->GetInitializer ());
		if (!Result)
			return false;
	}

	return true;
}

bool
CVariableMgr::AllocateVariable (CVariable* pVariable)
{
	if (pVariable->m_pLlvmValue)
		return true;

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
	m_pScopeLevelVariable = CreateVariable (EStorage_Static, "ScopeLevel", "jnc.ScopeLevel", pType);

	return m_pScopeLevelVariable;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

