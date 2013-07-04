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
	m_pShadowStackTopVariable = NULL;
}

void
CVariableMgr::Clear ()
{
	m_VariableList.Clear ();
	m_AliasList.Clear ();
	m_GlobalVariableArray.Clear ();
	m_StaticDestructList.Clear ();

	m_pScopeLevelVariable = NULL;
	m_pShadowStackTopVariable = NULL;
}

CVariable*
CVariableMgr::CreateVariable (
	EStorage StorageKind,
	const rtl::CString& Name,
	const rtl::CString& QualifiedName,
	CType* pType,
	uint_t PtrTypeFlags,
	rtl::CBoxListT <CToken>* pConstructor,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	CVariable* pVariable = AXL_MEM_NEW (CVariable);
	pVariable->m_pModule = m_pModule;
	pVariable->m_Name = Name;
	pVariable->m_QualifiedName = QualifiedName;
	pVariable->m_Tag = QualifiedName;
	pVariable->m_pType = pType;
	pVariable->m_StorageKind = StorageKind;
	pVariable->m_PtrTypeFlags = PtrTypeFlags;

	if (pConstructor)
		pVariable->m_Constructor.TakeOver (pConstructor);

	if (pInitializer)
		pVariable->m_Initializer.TakeOver (pInitializer);

	m_VariableList.InsertTail (pVariable);

	if (StorageKind == EStorage_Static)	
		m_GlobalVariableArray.Append (pVariable);

	if (pType->GetTypeKindFlags () & ETypeKindFlag_Import)
	{
		pVariable->m_pType_i = (CImportType*) pType;
		m_pModule->MarkForLayout (pVariable);
	}

	return pVariable;
}

llvm::GlobalVariable*
CVariableMgr::CreateLlvmGlobalVariable (
	CType* pType,
	const char* pTag
	)
{
	llvm::GlobalVariable* pLlvmValue = new llvm::GlobalVariable (
		*m_pModule->m_pLlvmModule,
		pType->GetLlvmType (),
		false,
		llvm::GlobalVariable::ExternalLinkage,
		(llvm::Constant*) pType->GetZeroValue ().GetLlvmValue (),
		pTag
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
CVariableMgr::AllocateInitializeGlobalVariables ()
{
	bool Result;

	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);
	m_pScopeLevelVariable = CreateVariable (EStorage_Static, "ScopeLevel", "jnc.ScopeLevel", pType);	
	m_pScopeLevelVariable->m_pLlvmAllocValue = CreateLlvmGlobalVariable (pType, m_pScopeLevelVariable->m_Tag);
	m_pScopeLevelVariable->m_pLlvmValue = m_pScopeLevelVariable->m_pLlvmAllocValue;
	m_pModule->m_LlvmBuilder.CreateStore (pType->GetZeroValue (), m_pScopeLevelVariable);

	size_t Count = m_GlobalVariableArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = m_GlobalVariableArray [i];
		if (pVariable == m_pScopeLevelVariable)
			continue;

		CType* pType = pVariable->GetType ();

		pVariable->m_pLlvmAllocValue = CreateLlvmGlobalVariable (pType, pVariable->GetQualifiedName ());

		CValue PtrValue (pVariable->m_pLlvmAllocValue, pType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe));
		Result = m_pModule->m_OperatorMgr.Prime (EStorage_Static, PtrValue, pType, &PtrValue);
		if (!Result)
			return false;

		pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();
	}

	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = m_GlobalVariableArray [i];

		Result = m_pModule->m_OperatorMgr.ParseInitializer (
			pVariable, 
			pVariable->m_Constructor,
			pVariable->m_Initializer
			);

		if (!Result)
			return false;
	}

	return true;
}

bool
CVariableMgr::AllocateInitializeVariable (
	CVariable* pVariable,
	bool AllocateOnly
	)
{
	ASSERT (!pVariable->m_pLlvmValue);

	bool Result;

	EStorage StorageKind = pVariable->m_StorageKind;
	CType* pType = pVariable->m_pType;

	CValue PtrValue;
	Result = m_pModule->m_OperatorMgr.Allocate (StorageKind, pType, pVariable->m_Tag, &PtrValue);
	if (!Result)
		return false;

	pVariable->m_pLlvmAllocValue = PtrValue.GetLlvmValue ();
	if (AllocateOnly)
	{
		ASSERT (pVariable->m_pType->GetTypeKind () != EType_Class);
		pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();
		return true;
	}
	
	if (StorageKind != EStorage_Static)
		return InitializeVariable (pVariable, PtrValue);

	TOnceStmt Stmt;
	m_pModule->m_ControlFlowMgr.OnceStmt_Create (&Stmt);

	CToken::CPos Pos;
	Result = 
		m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&Stmt, Pos) &&
		InitializeVariable (pVariable, PtrValue);

	if (!Result)
		return false;

	m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&Stmt, Pos);

	return true;
}

bool
CVariableMgr::InitializeVariable (
	CVariable* pVariable,
	const CValue& RawPtrValue
	)
{
	bool Result;

	if (pVariable->m_pType->GetTypeKind () == EType_Class)
	{
		CValue PtrValue;
		Result = m_pModule->m_OperatorMgr.Prime (pVariable->m_StorageKind, RawPtrValue, pVariable->m_pType, &PtrValue);
		if (!Result)
			return false;

		pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();
	}
	else
	{
		pVariable->m_pLlvmValue = pVariable->m_pLlvmAllocValue;

		if (pVariable->m_Initializer.IsEmpty () || 
			pVariable->m_Initializer.GetHead ()->m_Token == '{') 
		{
			m_pModule->m_LlvmBuilder.CreateStore (pVariable->m_pType->GetZeroValue (), RawPtrValue);
		}
	}

	Result = m_pModule->m_OperatorMgr.ParseInitializer (pVariable, pVariable->m_Constructor, pVariable->m_Initializer);
	if (!Result)
		return false;

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {