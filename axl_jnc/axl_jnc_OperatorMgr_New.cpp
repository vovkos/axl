#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::InitializeObject (
	EStorage StorageKind,
	const CValue& ObjPtrValue,
	CClassType* pClassType,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	int Flags = EObjectFlag_Alive;
	switch (StorageKind)
	{
	case EStorage_Static:
		Flags |= EObjectFlag_Static;
		break;

	case EStorage_Stack:
		Flags |= EObjectFlag_Stack;
		break;

	case EStorage_UHeap:
		Flags |= EObjectFlag_UHeap;
		break;
	}

	if (pClassType->GetFlags () & EClassTypeFlag_Abstract)
	{
		err::SetFormatStringError (_T("cannot instantiate abstract '%s'"), pClassType->GetTypeString ());
		return false;
	}

	CValue ScopeLevelValue;

	if (StorageKind == EStorage_Stack)
		ScopeLevelValue = CalcScopeLevelValue (m_pModule->m_NamespaceMgr.GetCurrentScope ());
	else
		ScopeLevelValue.SetConstSizeT (0);

	m_pModule->m_LlvmBuilder.CreateComment ("initialize object");
		
	CFunction* pInializer = pClassType->GetInitializer ();
	m_pModule->m_LlvmBuilder.CreateCall3 (
		pInializer,
		pInializer->GetType (),
		ObjPtrValue,
		ScopeLevelValue,
		CValue (Flags, EType_Int_p),
		NULL
		);

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, pClassType->GetClassPtrType (), pResultValue);

	CFunction* pConstructor = pClassType->GetConstructor ();
	if (!pConstructor)
	{
		if (pArgList && !pArgList->IsEmpty ())
		{
			err::SetFormatStringError (_T("'%s' has no constructor"), pClassType->GetTypeString ());
			return false;
		}

		return true;
	}

	rtl::CBoxListT <CValue> ArgList;
	if (!pArgList)
		pArgList = &ArgList;

	pArgList->InsertHead (*pResultValue);

	return CallOperator (pConstructor, pArgList);
}

bool
COperatorMgr::Allocate (
	EStorage StorageKind,
	CType* pType,
	const tchar_t* pTag,
	CValue* pResultValue
	)
{
	bool Result;

	llvm::GlobalVariable* pLlvmGlobalVariable;
	CFunction* pAlloc;

	CType* pPtrType = pType->GetDataPtrType (EDataPtrType_Unsafe);
	CValue PtrValue;

	switch (StorageKind)
	{
	case EStorage_Static:
		pLlvmGlobalVariable = m_pModule->m_VariableMgr.CreateLlvmGlobalVariable (pType, pTag);
		PtrValue.SetLlvmValue (pLlvmGlobalVariable, pPtrType);
		break;

	case EStorage_Stack:
		if (!m_pModule->m_NamespaceMgr.GetCurrentScope ())
		{
			err::SetFormatStringError (_T("'stack new' operator can only be called at local scope"));
			return false;
		}

		if (m_pModule->m_ControlFlowMgr.GetFlags () & EControlFlowFlag_HasJump)
		{
			err::SetFormatStringError (_T("'stack new' cannot be used in branched expression"));
			return false;
		}

		m_pModule->m_LlvmBuilder.CreateAlloca (pType, pTag, pPtrType, &PtrValue);
		break;

	case EStorage_Heap:
	case EStorage_UHeap:
		pAlloc = m_pModule->m_FunctionMgr.GetStdFunction (StorageKind == EStorage_UHeap ? EStdFunc_UHeapAlloc : EStdFunc_HeapAlloc);

		m_pModule->m_LlvmBuilder.CreateCall (
			pAlloc,
			pAlloc->GetType (),
			CValue (&pType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)),
			&PtrValue
			);

		m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pPtrType, &PtrValue);
		break;

	case EStorage_Member:
		if (!m_pModule->m_FunctionMgr.GetMemberNewField ())
		{
			err::SetFormatStringError (_T("'member new' operator can only be called from field initalizer"));
			return false;
		}

		if (m_pModule->m_ControlFlowMgr.GetFlags () & EControlFlowFlag_HasJump)
		{
			err::SetFormatStringError (_T("'member new' cannot be used in branched expression"));
			return false;
		}

		Result = 
			GetField (m_pModule->m_FunctionMgr.GetMemberNewField (), NULL, &PtrValue) &&
			UnaryOperator (EUnOp_Addr, &PtrValue);

		if (!Result)
			return false;
		
		m_pModule->m_FunctionMgr.NextMemberNewField ();
		break;

	default:
		err::SetFormatStringError (_T("invalid storage specifier '%s' in 'new' operator"), GetStorageKindString (StorageKind));
		return false;
	}

	*pResultValue = PtrValue;
	return true;
}

bool
COperatorMgr::NewOperator (
	EStorage StorageKind,
	CType* pType,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	bool Result;

	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();

	CType* pAllocType = pType;
	if (pType->GetTypeKind () == EType_Class)
		pAllocType = ((CClassType*) pType)->GetClassStructType ();

	CValue PtrValue;
	Result = Allocate (StorageKind, pAllocType, _T("new"), &PtrValue);
	if (!Result)
		return false;
	
	if (pType->GetTypeKind () == EType_Class)
	{
		CClassType* pClassType = (CClassType*) pType;
	
		if (StorageKind == EStorage_Static)
		{
			CToken::CPos Pos;

			TOnceStmt Stmt;
			m_pModule->m_ControlFlowMgr.OnceStmt_Create (&Stmt);

			Result = 
				m_pModule->m_ControlFlowMgr.OnceStmt_PreBody (&Stmt, Pos) &&
				InitializeObject (StorageKind, PtrValue, pClassType, pArgList, pResultValue);

			if (!Result)
				return false;
				
			m_pModule->m_ControlFlowMgr.OnceStmt_PostBody (&Stmt, Pos);
		}
		else
		{
			Result = InitializeObject (StorageKind, PtrValue, pClassType, pArgList, pResultValue);
			if (!Result)
				return false;
		}
		
		if (StorageKind == EStorage_Stack && pClassType->GetDestructor ())
			pScope->AddToDestructList (*pResultValue);
	}
	else
	{
		CValue ScopeLevelValue;

		if (StorageKind == EStorage_Stack)
			CalcScopeLevelValue (pScope, &ScopeLevelValue);
		else
			ScopeLevelValue.SetConstSizeT (0);
		
		pResultValue->SetThinDataPtr (
			PtrValue.GetLlvmValue (),
			pType->GetDataPtrType (EDataPtrType_Thin),
			PtrValue,
			pType->GetSize (),
			ScopeLevelValue
			);
	}

	return true;
}

bool
COperatorMgr::DeleteOperator (const CValue& RawOpValue)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	CFunction* pFree;

	CValue PtrValue;
	EType TypeKind = OpValue.GetType ()->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_DataPtr:
		{
		EDataPtrType PtrTypeKind = ((CDataPtrType*) OpValue.GetType ())->GetPtrTypeKind ();		
		if (PtrTypeKind == EDataPtrType_Thin)
		{
			err::SetFormatStringError (_T("invalid 'delete' on variable- or field-derived pointer"));
			return false;
		}

		Result = PrepareDataPtr (OpValue, ERuntimeError_LoadOutOfRange, &PtrValue);
		if (!Result)
			return false;

		pFree = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_UHeapFree);	
		}
		break;

	case EType_ClassPtr:
		{
		CClassType* pClassType = ((CClassPtrType*) OpValue.GetType ())->GetTargetType ();
		CFunction* pDestructor = pClassType->GetDestructor ();
		if (pDestructor)
		{
			Result = CallOperator (pDestructor, OpValue);
			if (!Result)
				return false;			
		}
		else
		{
			CheckClassPtrNull (OpValue);
		}

		pFree = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_UHeapFreeClassPtr);	
		PtrValue = OpValue;
		}
		break;

	default:
		err::SetFormatStringError (_T("cannot delete '%s'"), OpValue.GetType ()->GetTypeString ());
		return false;
	}

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);
	m_pModule->m_LlvmBuilder.CreateCall (pFree, pFree->GetType (), PtrValue, &ReturnValue);
	return true;
}

bool
COperatorMgr::ProcessDestructList (const rtl::CConstBoxListT <CValue>& List)
{
	rtl::CBoxIteratorT <CValue> It = List.GetHead ();
	for (; It; It++)
	{
		CValue Value = *It;
		ASSERT (Value.GetType ()->GetTypeKind () == EType_ClassPtr);
		
		CClassType* pType = ((CClassPtrType*) Value.GetType ())->GetTargetType ();

		CFunction* pDestructor = pType->GetDestructor ();
		ASSERT (pDestructor);

		m_pModule->m_LlvmBuilder.CreateCall (pDestructor, pDestructor->GetType (), Value, NULL);		
	}

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {