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

	memset (m_StdVariableArray, 0, sizeof (m_StdVariableArray));
}

void
CVariableMgr::Clear ()
{
	m_VariableList.Clear ();
	m_AliasList.Clear ();

	m_StaticVariableArray.Clear ();
	m_StaticGcRootArray.Clear ();
	m_GlobalStaticVariableArray.Clear ();
	m_GlobalStaticDestructArray.Clear ();
	m_LazyStaticDestructList.Clear ();

	m_TlsVariableArray.Clear ();
	m_TlsGcRootArray.Clear ();

	memset (m_StdVariableArray, 0, sizeof (m_StdVariableArray));
}

void
CVariableMgr::AddToLazyStaticDestructList (
	CVariable* pFlagVariable,
	CFunction* pDestructor,
	CVariable* pVariable
	)
{
	TLazyStaticDestruct* pDestruct = AXL_MEM_NEW (TLazyStaticDestruct);
	pDestruct->m_pFlagVariable = pFlagVariable;
	pDestruct->m_pDestructor = pDestructor;
	pDestruct->m_pVariable = pVariable;

	m_LazyStaticDestructList.InsertTail (pDestruct);
}

CVariable*
CVariableMgr::GetStdVariable (EStdVariable Variable)
{
	ASSERT ((size_t) Variable < EStdFunc__Count);

	if (m_StdVariableArray [Variable])
		return m_StdVariableArray [Variable];

	CVariable* pVariable;

	switch (Variable)
	{
	case EStdVariable_ScopeLevel:
		pVariable = CreateScopeLevelVariable ();
		break;

	default:
		ASSERT (false);
		pVariable = NULL;
	}

	m_StdVariableArray [Variable] = pVariable;
	return pVariable;
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

	switch (StorageKind)
	{
	case EStorage_Static:
		m_StaticVariableArray.Append (pVariable);

		if (m_pModule->m_NamespaceMgr.GetCurrentNamespace ()->GetNamespaceKind () == ENamespace_Global)
			m_GlobalStaticVariableArray.Append (pVariable);

		break;

	case EStorage_Thread:
		m_TlsVariableArray.Append (pVariable);
		break;

	case EStorage_Stack:
		break;

	default:
		ASSERT (false);
	}
	
	if (pType->GetTypeKindFlags () & ETypeKindFlag_Import)
	{
		pVariable->m_pType_i = (CImportType*) pType;
		m_pModule->MarkForLayout (pVariable);
	}

	return pVariable;
}

CVariable*
CVariableMgr::CreateOnceFlagVariable ()
{
	return CreateVariable (
		EStorage_Static, 
		"once_flag", 
		"once_flag", 
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32),
		EPtrTypeFlag_Volatile
		);
}

llvm::GlobalVariable*
CVariableMgr::CreateLlvmGlobalVariable (
	CType* pType,
	const char* pTag
	)
{
	llvm::GlobalVariable* pLlvmValue = new llvm::GlobalVariable (
		*m_pModule->GetLlvmModule (),
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
CVariableMgr::AllocatePrimeStaticVariables ()
{
	bool Result;

	size_t Count = m_StaticVariableArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = m_StaticVariableArray [i];

		Result = AllocatePrimeStaticVariable (pVariable);
		if (!Result)
			return false;
	}

	return true;
}

bool
CVariableMgr::AllocatePrimeStaticVariable (CVariable* pVariable)
{
	ASSERT (pVariable->m_StorageKind == EStorage_Static);
	ASSERT (m_pModule->m_ControlFlowMgr.GetCurrentBlock () == m_pModule->GetConstructor ()->GetEntryBlock ());

	CType* pType = pVariable->GetType ();

	pVariable->m_pLlvmAllocValue = CreateLlvmGlobalVariable (pType, pVariable->GetQualifiedName ());

	CValue PtrValue (pVariable->m_pLlvmAllocValue, pType->GetDataPtrType (EDataPtrType_Thin, EPtrTypeFlag_Unsafe));
	bool Result = m_pModule->m_OperatorMgr.Prime (EStorage_Static, PtrValue, pType, &PtrValue);
	if (!Result)
		return false;

	pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();

	if (pVariable->m_pType->GetFlags () & ETypeFlag_GcRoot)
		m_StaticGcRootArray.Append (pVariable);

	return true;
}

bool
CVariableMgr::InitializeGlobalStaticVariables ()
{
	bool Result;

	size_t Count = m_GlobalStaticVariableArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CVariable* pVariable = m_GlobalStaticVariableArray [i];

		Result = m_pModule->m_OperatorMgr.ParseInitializer (
			pVariable, 
			pVariable->m_Constructor,
			pVariable->m_Initializer
			);

		if (pVariable->m_pType->GetTypeKind () == EType_Class &&
			((CClassType*) pVariable->m_pType)->GetDestructor ())
			m_GlobalStaticDestructArray.Append (pVariable);

		if (!Result)
			return false;
	}

	return true;
}

bool
CVariableMgr::AllocatePrimeInitializeStaticVariable (CVariable* pVariable)
{
	bool Result;

	// allocate and prime in module constructor

	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (m_pModule->GetConstructor ()->GetEntryBlock ());

	AllocatePrimeStaticVariable (pVariable);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pBlock);

	// initialize within 'once' block

	TOnceStmt Stmt;
	m_pModule->m_ControlFlowMgr.OnceStmt_Create (&Stmt);

	CToken::CPos Pos;
	
	Result = 
		m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&Stmt, Pos) &&
		m_pModule->m_OperatorMgr.ParseInitializer (pVariable, pVariable->m_Constructor, pVariable->m_Initializer);

	if (!Result)
		return false;

	if (pVariable->m_pType->GetTypeKind () == EType_Class &&
		((CClassType*) pVariable->m_pType)->GetDestructor ())
	{
		AddToLazyStaticDestructList (
			Stmt.m_pFlagVariable, 
			((CClassType*) pVariable->m_pType)->GetDestructor (), 
			pVariable
			);
	}

	m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&Stmt, Pos);

	return true;
}

bool
CVariableMgr::AllocatePrimeInitializeNonStaticVariable (CVariable* pVariable)
{
	bool Result;

	CValue PtrValue;
	Result = m_pModule->m_OperatorMgr.Allocate (
		pVariable->m_StorageKind, 
		pVariable->m_pType, 
		pVariable->m_Tag, 
		&PtrValue
		);

	if (!Result)
		return false;

	pVariable->m_pLlvmAllocValue = PtrValue.GetLlvmValue ();

	if (pVariable->m_pType->GetTypeKind () == EType_Class)
	{
		Result = m_pModule->m_OperatorMgr.Prime (pVariable->m_StorageKind, PtrValue, pVariable->m_pType, &PtrValue);
		if (!Result)
			return false;

		pVariable->m_pLlvmValue = PtrValue.GetLlvmValue ();

		if (pVariable->m_StorageKind == EStorage_Stack &&
			pVariable->m_pType->GetTypeKind () == EType_Class &&
			((CClassType*) pVariable->m_pType)->GetDestructor ())
		{
			m_pModule->m_NamespaceMgr.GetCurrentScope ()->AddToDestructArray (pVariable);
		}
	}
	else
	{
		pVariable->m_pLlvmValue = pVariable->m_pLlvmAllocValue;

		if (pVariable->m_Initializer.IsEmpty () || 
			pVariable->m_Initializer.GetHead ()->m_Token == '{') 
		{
			m_pModule->m_LlvmBuilder.CreateStore (pVariable->m_pType->GetZeroValue (), PtrValue);
		}
	}

	Result = m_pModule->m_OperatorMgr.ParseInitializer (pVariable, pVariable->m_Constructor, pVariable->m_Initializer);
	if (!Result)
		return false;

	return true;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CVariable*
CVariableMgr::CreateScopeLevelVariable ()
{
	CType* pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT);
	
	CVariable* pVariable = CreateVariable (
		EStorage_Thread, 
		"t_ScopeLevel", 
		"jnc.t_ScopeLevel", 
		pType
		);	

	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (m_pModule->GetConstructor ()->GetEntryBlock ());

	llvm::GlobalVariable* pLlvmValue = new llvm::GlobalVariable (
		*m_pModule->GetLlvmModule (),
		pType->GetLlvmType (),
		false,
		llvm::GlobalVariable::ExternalLinkage,
		(llvm::Constant*) pType->GetZeroValue ().GetLlvmValue (),
		pVariable->m_Tag.cc ()
		);

	pVariable->m_pLlvmAllocValue = pLlvmValue;
	pVariable->m_pLlvmValue = pLlvmValue;

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pBlock);
	return pVariable;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {