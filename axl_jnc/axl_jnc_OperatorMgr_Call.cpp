#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::NormalizeFunctionPointer (
	const CValue& Value,
	CValue* pResultValue
	)
{
	CFunctionPtrType* pFunctionPtrType = (CFunctionPtrType*) Value.GetType ();
	ASSERT (pFunctionPtrType->GetTypeKind () == EType_FunctionPtr);

	CValue PfnValue;
	CValue IfaceValue;

	m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 0, pFunctionPtrType->GetFunctionType ()->GetFunctionPtrType (EFunctionPtrType_Unsafe), &PfnValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (Value, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &IfaceValue);

	ref::CPtrT <CClosure> OldClosure = Value.GetClosure ();

	*pResultValue = PfnValue;

	CClosure* pClosure;

	if (OldClosure)
	{
		pResultValue->SetClosure (OldClosure);
		pClosure = OldClosure;
	}
	else
	{
		pClosure = pResultValue->CreateClosure (EClosure_Function);
	}

	pClosure->GetArgList ()->InsertHead (IfaceValue);
	return true;
}

CType*
COperatorMgr::GetVarArgType (
	CType* pType,
	bool IsUnsafeVarArg
	)
{
	EType TypeKind = pType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_DataRef:
		return GetVarArgType (((CDataPtrType*) pType)->GetDataType (), IsUnsafeVarArg);

	case EType_BitField:
		return GetVarArgType (((CBitFieldType*) pType)->GetBaseType (), IsUnsafeVarArg);

	case EType_Enum:
		return m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32);

	case EType_Float:
		return m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double);

	default:
		if (pType->IsIntegerType ())
		{
			if (pType->GetSize () <= 4)
				return m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32);
			else
				return m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64);
		}

		return pType;
	}
}

bool
COperatorMgr::CallOperator (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	bool Result;

	rtl::CBoxListT <CValue> EmptyArgList;
	if (!pArgList)
		pArgList = &EmptyArgList;

	CValue OpValue;
	if (RawOpValue.GetValueKind () == EValue_Function && RawOpValue.GetFunction ()->IsOverloaded ())
	{
		CFunction* pFunction = RawOpValue.GetFunction ()->ChooseOverload (pArgList);
		if (!pFunction)
			return false;

		OpValue.SetFunction (pFunction);
	}
	else
	{
		Result = PrepareOperand (RawOpValue, &OpValue);
		if (!Result)
			return false;
	}

	CClosure* pClosure = OpValue.GetClosure ();
	if (pClosure)
	{
		Result = pClosure->Apply (pArgList);
		if (!Result)
			return false;
	}

	if (OpValue.GetValueKind () == EValue_Function)
	{
		CFunction* pFunction = OpValue.GetFunction ();

		if (pFunction->GetStorageKind () == EStorage_Virtual)
		{
			if (!pClosure)
			{
				err::SetFormatStringError (_T("virtual method call requires an object pointer"));
				return false;
			}

			Result = GetVirtualMethodMember (pFunction, pClosure, &OpValue);
			if (!Result)
				return false;
		}

		return CallImpl (OpValue, pFunction->GetType (), pArgList, pResultValue);
	}
	
	CType* pOpType = OpValue.GetType ();
	EType OpTypeKind = pOpType->GetTypeKind ();

	if (OpTypeKind == EType_Event)
	{
		return CallEvent (OpValue, pArgList);
	}
	else if (OpTypeKind == EType_FunctionRef || OpTypeKind == EType_FunctionPtr)
	{
		EFunctionPtrType PtrTypeKind = ((CFunctionPtrType*) pOpType)->GetPtrTypeKind ();

		if (PtrTypeKind == EFunctionPtrType_Thin || PtrTypeKind == EFunctionPtrType_Unsafe)
			return CallImpl (OpValue, (CFunctionType*) ((CFunctionPtrType*) pOpType)->GetFunctionType (), pArgList, pResultValue);

		return CallFunctionPtr (OpValue, pArgList, pResultValue);
	}
	else 
	{
		err::SetFormatStringError (_T("cannot call '%s'"), pOpType->GetTypeString ());
		return false;
	}
}

bool
COperatorMgr::CalcScopeLevelValue (
	CScope* pScope,
	CValue* pScopeLevelValue
	)
{
	if (!pScope)
	{
		pScopeLevelValue->SetConstSizeT (0);
		return true;
	}

	m_pModule->m_LlvmBuilder.CreateComment ("calc scope level value");

	CFunction* pCurrentFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pCurrentFunction && pCurrentFunction->GetScopeLevelVariable ());

	CValue ScopeBaseLevelValue;
	m_pModule->m_LlvmBuilder.CreateLoad (pCurrentFunction->GetScopeLevelVariable (), NULL, &ScopeBaseLevelValue);

	CValue ScopeIncValue (pScope->GetLevel (), EType_SizeT);

	m_pModule->m_LlvmBuilder.CreateAdd_i (
		ScopeBaseLevelValue, 
		ScopeIncValue, 
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), 
		pScopeLevelValue
		);

	return true;
}

bool
COperatorMgr::CastArgList (
	CFunctionType* pFunctionType,
	rtl::CBoxListT <CValue>* pArgList,
	rtl::CArrayT <CValue>* pArgArray
	)
{
	bool Result;

	rtl::CArrayT <CType*> ArgTypeArray = pFunctionType->GetArgTypeArray ();

	size_t FormalArgCount = ArgTypeArray.GetCount ();
	size_t ActualArgCount = pArgList->GetCount ();

	bool IsVarArg = (pFunctionType->GetFlags () & EFunctionTypeFlag_VarArg) != 0;
	bool IsUnsafeVarArg = (pFunctionType->GetFlags () & EFunctionTypeFlag_UnsafeVarArg) != 0;

	if (IsVarArg && ActualArgCount < FormalArgCount ||
		!IsVarArg && ActualArgCount != FormalArgCount)
	{
		err::SetFormatStringError (
			_T("'%s' takes %d arguments; %d passed"), 
			pFunctionType->GetTypeString (), 
			FormalArgCount, 
			ActualArgCount
			);
		return false;
	}

	pArgArray->Clear ();
	pArgArray->Reserve (ActualArgCount);
	
	rtl::CBoxIteratorT <CValue> Arg = pArgList->GetHead ();
	for (size_t i = 0; i < FormalArgCount; Arg++, i++)
	{
		CType* pFormalArgType = ArgTypeArray [i];
		
		CValue ArgCast;
		Result = 
			CheckCastKind (*Arg, pFormalArgType) &&
			CastOperator (*Arg, pFormalArgType, &ArgCast);

		if (!Result)
			return false;

		pArgArray->Append (ArgCast);
	}

	// vararg

	for (; Arg; Arg++)
	{
		CType* pFormalArgType = GetVarArgType (Arg->GetType (), IsUnsafeVarArg);

		CValue ArgCast;
		Result = CastOperator (*Arg, pFormalArgType, &ArgCast);
		if (!Result)
			return false;

		pArgArray->Append (ArgCast);
	}

	return true;
}

bool
COperatorMgr::CallEvent (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	CEventType* pEventType = (CEventType*) OpValue.GetType ();
	ASSERT (pEventType->GetTypeKind () == EType_Event);
	
	CFunctionPtrType* pFunctionPointerType = pEventType->GetFunctionPtrType ();
	CFunctionType* pFunctionType = pFunctionPointerType->GetFunctionType ();

	CValue HandlerValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, NULL, &HandlerValue);

	CType* pHandlerPtrType = pEventType->GetHandlerStructType ()->GetDataPtrType (EDataPtrType_Unsafe);

	CValue HandlerVariable;
	m_pModule->m_LlvmBuilder.CreateAlloca (pHandlerPtrType, "event_handler", NULL, &HandlerVariable);
	m_pModule->m_LlvmBuilder.CreateStore (HandlerValue, HandlerVariable);

	CBasicBlock* pConditionBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("event_cond"));
	CBasicBlock* pBodyBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("event_loop"));
	CBasicBlock* pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("event_follow"));

	m_pModule->m_ControlFlowMgr.Follow (pConditionBlock);

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateLoad (HandlerVariable, NULL, &HandlerValue);
	m_pModule->m_LlvmBuilder.CreateEq_i (HandlerValue, pHandlerPtrType->GetZeroValue (), &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFollowBlock, pBodyBlock, pBodyBlock);

	CValue FunctionPtrValue;
	m_pModule->m_LlvmBuilder.CreateLoad (HandlerValue, NULL, &HandlerValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (HandlerValue, 0, pEventType->GetFunctionPtrType (), &FunctionPtrValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (HandlerValue, 1, NULL, &HandlerValue);
	m_pModule->m_LlvmBuilder.CreateStore (HandlerValue, HandlerVariable);

	CValue ResultValue;
	CallFunctionPtr (FunctionPtrValue, pArgList, &ResultValue);

	m_pModule->m_ControlFlowMgr.Jump (pConditionBlock, pFollowBlock);

	return true;
}

bool
COperatorMgr::CallFunctionPtr (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	CFunctionPtrType* pFunctionPointerType = (CFunctionPtrType*) OpValue.GetType ();
	ASSERT (pFunctionPointerType->GetTypeKind () == EType_FunctionPtr);

	CFunctionType* pFunctionType = pFunctionPointerType->GetFunctionType ();

	bool Result = m_pModule->m_LlvmBuilder.CheckNullPtr (OpValue);
	if (!Result)
		return false;
	
	CValue PfnValue;
	CValue IfaceValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, pFunctionPointerType->GetFunctionType ()->GetFunctionPtrType (EFunctionPtrType_Thin), &PfnValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &IfaceValue);
	pArgList->InsertHead (IfaceValue);

	return CallImpl (PfnValue, pFunctionPointerType->GetFunctionType (), pArgList, pResultValue);
}

bool
COperatorMgr::CallImpl (
	const CValue& PfnValue,
	CFunctionType* pFunctionType,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	char Buffer [256];
	rtl::CArrayT <CValue> ArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));

	bool Result = CastArgList (pFunctionType, pArgList, &ArgArray);
	if (!Result)
		return false;

	CScope* pCurrentScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	CValue ScopeLevelValue = CalcScopeLevelValue (pCurrentScope);

	m_pModule->m_LlvmBuilder.CreateComment ("update scope level before call");
	m_pModule->m_LlvmBuilder.CreateStore (ScopeLevelValue, m_pModule->m_VariableMgr.GetScopeLevelVariable ());

	m_pModule->m_LlvmBuilder.CreateCall (
		PfnValue,
		pFunctionType,
		ArgArray,
		ArgArray.GetCount (),
		pResultValue
		);

	return true;
}

bool
COperatorMgr::ClosureOperator (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return false;

	EType TypeKind = OpValue.GetType ()->GetTypeKind ();
	if (TypeKind != EType_FunctionRef && TypeKind != EType_FunctionPtr)
	{
		err::SetFormatStringError (_T("closure operator cannot be applied to '%s'"), OpValue.GetType ()->GetTypeString ());
		return false;
	}

	*pResultValue = OpValue;
	
	CClosure* pClosure = pResultValue->GetClosure ();
	if (!pClosure)
		pClosure = pResultValue->CreateClosure (EClosure_Function);

	ASSERT (pClosure->GetClosureKind () == EClosure_Function); 
	// cause property closure should have been eliminated in PrepareOperand ()

	pClosure->CombineClosure (pArgList);
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
