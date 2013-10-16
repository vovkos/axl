#include "pch.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
COperatorMgr::CallTraceFunction (
	const char* pFunctionName,
	const char* pString
	)
{
	CModuleItem* pItem = m_pModule->m_NamespaceMgr.GetGlobalNamespace ()->FindItem (pFunctionName);
	if (pItem && pItem->GetItemKind () == EModuleItem_Function)
	{
		CValue LiteralValue;
		LiteralValue.SetCharArray (pString);
		m_pModule->m_OperatorMgr.CallOperator ((CFunction*) pItem, LiteralValue);
	}
}

CType*
COperatorMgr::GetFunctionType (
	const CValue& OpValue,
	CFunctionType* pFunctionType
	)	
{
	CFunctionPtrType* pFunctionPtrType = pFunctionType->GetFunctionPtrType (
		EType_FunctionRef, 
		EFunctionPtrType_Thin
		);

	CClosure* pClosure = OpValue.GetClosure ();
	if (!pClosure)
		return pFunctionPtrType;

	return GetClosureOperatorResultType (pFunctionPtrType, pClosure->GetArgList ());
}

CType*
COperatorMgr::GetClosureOperatorResultType (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue);
	if (!Result)
		return NULL;

	EType TypeKind = OpValue.GetType ()->GetTypeKind ();
	if (TypeKind != EType_FunctionRef && TypeKind != EType_FunctionPtr)
	{
		err::SetFormatStringError (
			"closure operator cannot be applied to '%s'", 
			OpValue.GetType ()->GetTypeString ().cc () // thanks a lot gcc
			);
		return NULL;
	}

	ref::CPtrT <CClosure> Closure = AXL_REF_NEW (CClosure);
	Closure->Append (*pArgList);
	return Closure->GetClosureType (OpValue.GetType ());
}

bool
COperatorMgr::GetClosureOperatorResultType (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	CType* pResultType = GetClosureOperatorResultType (RawOpValue, pArgList);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
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
		err::SetFormatStringError (
			"closure operator cannot be applied to '%s'", 
			OpValue.GetType ()->GetTypeString ().cc ()
			);
		return false;
	}

	*pResultValue = OpValue;
	
	CClosure* pClosure = pResultValue->GetClosure ();
	if (!pClosure)
		pClosure = pResultValue->CreateClosure ();

	pClosure->Append (*pArgList);
	return true;
}

CType*
COperatorMgr::GetUnsafeVarArgType (CType* pType)
{
	for (;;)
	{
		CType* pPrevType = pType;
		
		EType TypeKind = pType->GetTypeKind ();
		switch (TypeKind)
		{
		case EType_PropertyRef:
			pType = ((CPropertyPtrType*) pType)->GetTargetType ()->GetReturnType ();
			break;

		case EType_DataRef:
			pType = ((CDataPtrType*) pType)->GetTargetType ();
			break;

		case EType_BitField:
			pType = ((CBitFieldType*) pType)->GetBaseType ();
			break;

		case EType_Enum:
			pType = ((CEnumType*) pType)->GetBaseType ();
			break;

		case EType_Float:
			pType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Double);
			break;

		case EType_Array:
			pType = ((CArrayType*) pType)->GetElementType ()->GetDataPtrType_c ();
			break;

		case EType_DataPtr:
			pType = ((CDataPtrType*) pType)->GetTargetType ()->GetDataPtrType_c ();
			break;

		default:
			if (pType->GetTypeKindFlags () & ETypeKindFlag_Integer)
			{
				pType = pType->GetSize () > 4 ?
					m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int64) :
					m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int32);
			}
		}

		if (pType == pPrevType)
			return pType;
	}
}

CType*
COperatorMgr::GetCallOperatorResultType (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	bool Result;

	CValue OpValue;
	PrepareOperandType (RawOpValue, &OpValue);

	if (OpValue.GetType ()->GetTypeKind () == EType_ClassPtr)
	{
		CFunction* pCallOperator = ((CClassPtrType*) OpValue.GetType ())->GetTargetType ()->GetCallOperator ();
		if (!pCallOperator)
		{
			err::SetFormatStringError ("cannot call '%s'", OpValue.GetType ()->GetTypeString ().cc ());
			return NULL;
		}

		CValue ObjValue = OpValue;
		
		OpValue.SetFunctionTypeOverload (pCallOperator->GetTypeOverload ());
		OpValue.InsertToClosureTail (ObjValue);
	}

	ref::CPtrT <CClosure> Closure = OpValue.GetClosure ();
	if (Closure)
	{
		Result = Closure->Apply (pArgList);
		if (!Result)
			return NULL;
	}

	if (RawOpValue.GetValueKind () == EValue_FunctionTypeOverload)
	{
		size_t i = RawOpValue.GetFunctionTypeOverload ()->ChooseOverload (*pArgList);
		if (i == -1)
			return NULL;

		CFunctionType* pFunctionType = RawOpValue.GetFunctionTypeOverload ()->GetOverload (i);
		return pFunctionType->GetReturnType ();
	}
		
	CFunctionType* pFunctionType;
		
	CType* pOpType = OpValue.GetType ();
	EType TypeKind = pOpType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_Function:
		pFunctionType = (CFunctionType*) pOpType;
		break;

	case EType_FunctionRef:
	case EType_FunctionPtr:
		pFunctionType = ((CFunctionPtrType*) pOpType)->GetTargetType ();
		break;

	default:
		err::SetFormatStringError ("cannot call '%s'", pOpType->GetTypeString ().cc ());
		return NULL;
	}

	return pFunctionType->GetReturnType ();
}

bool
COperatorMgr::GetCallOperatorResultType (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	CType* pResultType = GetCallOperatorResultType (RawOpValue, pArgList);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
	return true;
}

bool
COperatorMgr::CallOperator (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	bool Result;

	CValue OpValue;
	CValue UnusedReturnValue;
	rtl::CBoxListT <CValue> EmptyArgList;

	if (!pResultValue)
		pResultValue = &UnusedReturnValue;

	if (!pArgList)
		pArgList = &EmptyArgList;

	Result = PrepareOperand (RawOpValue, &OpValue, 0);
	if (!Result)
		return false;

	if (OpValue.GetType ()->GetTypeKind () == EType_ClassPtr)
	{
		CClassPtrType* pPtrType = (CClassPtrType*) OpValue.GetType ();

		CFunction* pCallOperator = pPtrType->GetTargetType ()->GetCallOperator ();
		if (!pCallOperator)
		{
			err::SetFormatStringError ("cannot call '%s'", pPtrType->GetTypeString ().cc ());
			return false;
		}

		if ((pCallOperator->GetFlags () & EMulticastMethodFlag_InaccessibleViaEventPtr) && pPtrType->IsEventPtrType ())
		{
			err::SetFormatStringError ("'Call' is inaccessible via 'event' pointer");
			return false;
		}

		CValue ObjValue = OpValue;
		
		OpValue.SetFunction (pCallOperator);
		OpValue.InsertToClosureTail (ObjValue);
	}

	ref::CPtrT <CClosure> Closure = OpValue.GetClosure ();
	if (Closure)
	{
		Result = Closure->Apply (pArgList);
		if (!Result)
			return false;
	}

	if (OpValue.GetValueKind () == EValue_Function && OpValue.GetFunction ()->IsOverloaded ())
	{
		CFunction* pFunction = OpValue.GetFunction ()->ChooseOverload (*pArgList);
		if (!pFunction)
			return false;

		OpValue.SetFunction (pFunction);
		OpValue.SetClosure (Closure);
	}

	if (OpValue.GetValueKind () == EValue_Function)
	{
		CFunction* pFunction = OpValue.GetFunction ();

		if (pFunction->IsVirtual ())
		{
			Result = GetVirtualMethod (pFunction, Closure, &OpValue);
			if (!Result)
				return false;
		}

		return CallImpl (OpValue, pFunction->GetType (), pArgList, pResultValue);
	}

	CType* pOpType = OpValue.GetType ();
	if (!(pOpType->GetTypeKindFlags () & ETypeKindFlag_FunctionPtr) || 
		((CFunctionPtrType*) pOpType)->GetPtrTypeKind () == EFunctionPtrType_Weak)
	{
		err::SetFormatStringError ("cannot call '%s'", pOpType->GetTypeString ().cc ());
		return false;
	}

	CFunctionPtrType* pFunctionPtrType = ((CFunctionPtrType*) pOpType);
	return pFunctionPtrType->HasClosure () ? 
		CallClosureFunctionPtr (OpValue, pArgList, pResultValue) : 
		CallImpl (OpValue, pFunctionPtrType->GetTargetType (), pArgList, pResultValue);
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

	CLlvmScopeComment Comment (&m_pModule->m_LlvmIrBuilder, "calc scope level value");
	 
	CValue ScopeBaseLevelValue = m_pModule->m_FunctionMgr.GetScopeLevelValue ();
	CValue ScopeIncValue (pScope->GetLevel (), EType_SizeT);

	m_pModule->m_LlvmIrBuilder.CreateAdd_i (
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

	rtl::CArrayT <CFunctionArg*> ArgArray = pFunctionType->GetArgArray ();

	size_t FormalArgCount = ArgArray.GetCount ();
	size_t ActualArgCount = pArgList->GetCount ();

	bool IsVarArg = (pFunctionType->GetFlags () & EFunctionTypeFlag_VarArg) != 0;
	bool IsUnsafeVarArg = (pFunctionType->GetFlags () & EFunctionTypeFlag_UnsafeVarArg) != 0;

	size_t CommonArgCount;

	if (ActualArgCount <= FormalArgCount)
	{
		CommonArgCount = ActualArgCount;
	}
	else if (IsVarArg)
	{
		CommonArgCount = FormalArgCount;
	}
	else
	{
		err::SetFormatStringError ("too many arguments in a call to '%s'", pFunctionType->GetTypeString ().cc ());
		return false;
	}

	pArgArray->Clear ();
	pArgArray->Reserve (ActualArgCount);
	
	size_t i = 0;
	rtl::CBoxIteratorT <CValue> Arg = pArgList->GetHead ();

	// common for both formal and actual

	for (; i < CommonArgCount; Arg++, i++)
	{
		CValue ArgValue = *Arg;

		CFunctionArg* pArg = ArgArray [i];
		if (ArgValue.IsEmpty ())
		{
			rtl::CConstBoxListT <CToken> Initializer = pArg->GetInitializer ();
			if (Initializer.IsEmpty ())
			{
				err::SetFormatStringError (
					"argument (%d) of '%s' has no default value", 
					i + 1, 
					pFunctionType->GetTypeString ().cc ()
					);
				return false;
			}

			Result = EvaluateAlias (Initializer, &ArgValue);
			if (!Result)
				return false;
		}

		CType* pFormalArgType = pArg->GetType ();
	
		CValue ArgCast;
		Result = 
			CheckCastKind (ArgValue, pFormalArgType) &&
			CastOperator (ArgValue, pFormalArgType, &ArgCast);

		if (!Result)
			return false;

		pArgArray->Append (ArgCast);
	}

	// default formal arguments

	for (; i < FormalArgCount; i++)
	{
		CValue ArgValue;

		CFunctionArg* pArg = ArgArray [i];
		rtl::CConstBoxListT <CToken> Initializer = pArg->GetInitializer ();
		if (Initializer.IsEmpty ())
		{
			err::SetFormatStringError (
				"argument (%d) of '%s' has no default value", 
				i + 1, 
				pFunctionType->GetTypeString ().cc ()
				);
			return false;
		}

		Result = EvaluateAlias (Initializer, &ArgValue);
		if (!Result)
			return false;

		CType* pFormalArgType = pArg->GetType ();
	
		CValue ArgCast;
		Result = 
			CheckCastKind (ArgValue, pFormalArgType) &&
			CastOperator (ArgValue, pFormalArgType, &ArgCast);

		if (!Result)
			return false;

		pArgArray->Append (ArgCast);
	}

	// vararg arguments

	for (; Arg; Arg++)
	{
		if (Arg->IsEmpty ())
		{
			err::SetFormatStringError ("vararg arguments cannot be skipped");
			return false;
		}
		
		ASSERT (IsUnsafeVarArg);
		CType* pFormalArgType = GetUnsafeVarArgType (Arg->GetType ());

		CValue ArgCast;
		Result = CastOperator (*Arg, pFormalArgType, &ArgCast);
		if (!Result)
			return false;

		pArgArray->Append (ArgCast);
	}

	return true;
}

bool
COperatorMgr::CallClosureFunctionPtr (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_FunctionPtr);

	CFunctionPtrType* pFunctionPointerType = (CFunctionPtrType*) OpValue.GetType ();
	CFunctionType* pFunctionType = pFunctionPointerType->GetTargetType ();
	CFunctionType* pAbstractMethodType = pFunctionType->GetStdObjectMemberMethodType ();

	CheckFunctionPtrNull (OpValue);
	
	CValue PfnValue;
	CValue IfaceValue;
	m_pModule->m_LlvmIrBuilder.CreateExtractValue (OpValue, 0, pAbstractMethodType->GetFunctionPtrType (EFunctionPtrType_Thin), &PfnValue);
	m_pModule->m_LlvmIrBuilder.CreateExtractValue (OpValue, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &IfaceValue);
	pArgList->InsertHead (IfaceValue);

	return CallImpl (PfnValue, pAbstractMethodType, pArgList, pResultValue);
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

	CValue ScopeLevelValue = m_pModule->m_FunctionMgr.GetScopeLevelValue ();
	if (ScopeLevelValue)
	{
		CScope* pCurrentScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
		CValue ScopeLevelValue = CalcScopeLevelValue (pCurrentScope);

		CLlvmScopeComment Comment (&m_pModule->m_LlvmIrBuilder, "update scope level before call");
		CVariable* pVariable = m_pModule->m_VariableMgr.GetStdVariable (EStdVariable_ScopeLevel);
		m_pModule->m_LlvmIrBuilder.CreateStore (ScopeLevelValue, pVariable);
	}

	GcSafePoint ();

	m_pModule->m_LlvmIrBuilder.CreateCall (
		PfnValue,
		pFunctionType,
		ArgArray,
		ArgArray.GetCount (),
		pResultValue
		);

	if ((pFunctionType->GetFlags () & EFunctionTypeFlag_Unwinder) && 
		m_pModule->m_ControlFlowMgr.IsUnwindingEnabled ())
	{
		CValue IndicatorValue;

		Result = 
			GetUnwindIndicator (*pResultValue, &IndicatorValue) &&
			m_pModule->m_ControlFlowMgr.Unwind (IndicatorValue);

		if (!Result)
			return false;
	}

	return true;
}

bool
COperatorMgr::GetUnwindIndicator (
	const CValue& ReturnValue,
	CValue* pResultValue
	)
{
	// todo: more logic here
	// if function has unwinder specifier, check retval against unwinder expression
	// else, for integers, compare to -1
	// else, apply logical-not

	return UnaryOperator (EUnOp_LogNot, ReturnValue, pResultValue);
}

void
COperatorMgr::GcSafePoint ()
{
	CFunction* pGcSafePoint = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_GcSafePoint);

	CValue ResultValue;
	m_pModule->m_LlvmIrBuilder.CreateCall (
		pGcSafePoint,
		pGcSafePoint->GetType (),
		&ResultValue
		);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {