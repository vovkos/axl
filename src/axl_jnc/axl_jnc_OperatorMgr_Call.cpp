#include "pch.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

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
COperatorMgr::GetVarArgType (
	CType* pType,
	bool IsUnsafeVarArg
	)
{
	EType TypeKind = pType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_PropertyRef:
		return GetVarArgType (((CPropertyPtrType*) pType)->GetTargetType ()->GetReturnType (), IsUnsafeVarArg);

	case EType_DataRef:
		return GetVarArgType (((CDataPtrType*) pType)->GetTargetType (), IsUnsafeVarArg);

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

CType*
COperatorMgr::GetCallOperatorResultType (
	const CValue& RawOpValue,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	CValue OpValue;

	EValue OpValueKind = RawOpValue.GetValueKind ();	
	if (OpValueKind == EValue_FunctionTypeOverload)
	{
		size_t i = RawOpValue.GetFunctionTypeOverload ()->ChooseOverload (*pArgList);
		if (i == -1)
			return NULL;

		CFunctionType* pFunctionType = RawOpValue.GetFunctionTypeOverload ()->GetOverload (i);
		return pFunctionType->GetReturnType ();
	}
	
	ASSERT (OpValueKind == EValue_Type);
	PrepareOperandType (RawOpValue, &OpValue);
		
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
	CValue ReturnValue;

	if (!pResultValue)
		pResultValue = &ReturnValue;

	EValue OpValueKind = RawOpValue.GetValueKind ();	
	if (OpValueKind == EValue_AutoEv)
	{
		CFunction* pFunction = RawOpValue.GetAutoEv ()->GetStarter ();
		OpValue.SetFunction (pFunction);
		OpValue.SetClosure (RawOpValue.GetClosure ());
	}
	else if (OpValueKind == EValue_Function)
	{
		CFunction* pFunction = RawOpValue.GetFunction ();
		if (pFunction->IsOverloaded ())
		{
			pFunction = RawOpValue.GetFunction ()->ChooseOverload (*pArgList);
			if (!pFunction)
				return false;
		}

		OpValue.SetFunction (pFunction);
		OpValue.SetClosure (RawOpValue.GetClosure ());
	}
	else
	{
		PrepareOperandType (RawOpValue, &OpValue);
		uint_t OpFlags = OpValue.GetType ()->GetTypeKind () == EType_Multicast ? EOpFlag_KeepDataRef : 0;

		Result = PrepareOperand (RawOpValue, &OpValue, OpFlags);
		if (!Result)
			return false;
	}

	if (OpValue.GetType ()->GetTypeKind () == EType_DataRef)
	{
		CDataPtrType* pPtrType = (CDataPtrType*) OpValue.GetType ();
		ASSERT (pPtrType->GetTargetType ()->GetTypeKind () == EType_Multicast);

		CMulticastType* pMulticastType = (CMulticastType*) pPtrType->GetTargetType ();
		CFunction* pCallMethod = pMulticastType->GetMethod (EMulticastMethod_Call, pPtrType->GetPtrTypeKind ());

		ref::CPtrT <CClosure> Closure = OpValue.GetClosure ();
		if (!Closure)
			Closure = OpValue.CreateClosure ();

		CValue PtrValue;
		UnaryOperator (EUnOp_Addr, OpValue, &PtrValue);
		Closure->GetArgList ()->InsertHead (PtrValue);

		OpValue.SetFunction (pCallMethod);
		OpValue.SetClosure (Closure);
	}

	rtl::CBoxListT <CValue> EmptyArgList;
	if (!pArgList)
		pArgList = &EmptyArgList;

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

		if (pFunction->IsVirtual ())
		{
			Result = GetVirtualMethod (pFunction, pClosure, &OpValue);
			if (!Result)
				return false;
		}

		return CallImpl (OpValue, pFunction->GetType (), pArgList, pResultValue);
	}

	CType* pOpType = OpValue.GetType ();
	EType OpTypeKind = pOpType->GetTypeKind ();

	if (OpTypeKind != EType_FunctionRef && OpTypeKind != EType_FunctionPtr)
	{
		err::SetFormatStringError ("cannot call '%s'", pOpType->GetTypeString ().cc ());
		return NULL;
	}

	CFunctionPtrType* pFunctionPtrType = (CFunctionPtrType*) pOpType;
	return pFunctionPtrType->HasClosure () ? 
		CallClosureFunctionPtr (OpValue, pArgList, pResultValue) : 
		CallImpl (OpValue, pFunctionPtrType->GetTargetType (), pArgList, pResultValue);
}

bool
COperatorMgr::CallBaseTypeConstructor (
	CType* pType,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	bool Result;

	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (
		pFunction->GetFunctionKind () == EFunction_Constructor && 
		pFunction->GetParentType ()->IsDerivableType ()
		);

	CDerivableType* pParentType = (CDerivableType* ) pFunction->GetParentType ();
	CBaseType* pBaseType = pParentType->FindBaseType (pType);
	if (!pBaseType)
	{
		err::SetFormatStringError (
			"'%s' is not a base type of '%s'", 
			pType->GetTypeString ().cc (), 
			pParentType->GetTypeString ().cc ()
			);
		return false;
	}

	if (pBaseType->GetFlags () & EBaseTypeFlag_Constructed)
	{
		err::SetFormatStringError ("'%s' is already constructed", pType->GetTypeString ().cc ());
		return false;
	}

	if (pType->GetTypeKind () != EType_Class || !((CClassType*) pType)->GetConstructor ())
	{
		err::SetFormatStringError ("'%s' has no constructor", pType->GetTypeString ().cc ());
		return false;
	}

	pArgList->InsertHead (m_pModule->m_FunctionMgr.GetThisValue ());

	CFunction* pConstructor = ((CClassType*) pType)->GetConstructor ();
	Result = CallOperator (pConstructor, pArgList);
	if (!Result)
		return false;

	pBaseType->MarkConstructed ();
	return true;
}

bool
COperatorMgr::PostBaseTypeConstructorList ()
{
	bool Result;

	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (
		pFunction->GetFunctionKind () == EFunction_Constructor && 
		pFunction->GetParentType ()->GetTypeKind () == EType_Class
		);

	CClassType* pParentType = (CClassType*) pFunction->GetParentType ();

	rtl::CBoxListT <CValue> ArgList;
	ArgList.InsertHead (m_pModule->m_FunctionMgr.GetThisValue ());

	rtl::CIteratorT <CBaseType> BaseType = pParentType->GetBaseTypeList ().GetHead ();
	for (; BaseType; BaseType++)
	{
		if ((BaseType->GetFlags () & EBaseTypeFlag_Constructed) ||
			BaseType->GetType ()->GetTypeKind () != EType_Class)
			continue;

		CClassType* pBaseClassType = (CClassType*) BaseType->GetType ();
		CFunction* pConstructor = pBaseClassType->GetConstructor ();
		if (!pConstructor)
			continue;

		pConstructor = pBaseClassType->GetDefaultConstructor ();
		if (!pConstructor)
		{
			err::SetFormatStringError ("'%s' has no default constructor", pBaseClassType->GetTypeString ().cc ());
			return false;
		}

		Result = CallOperator (pConstructor, &ArgList);
		if (!Result)
			return false;
	}

	pParentType->ClearAllBaseTypeConstructedFlags ();

	CFunction* pPreConstructor = pParentType->GetPreConstructor ();
	return pPreConstructor ? CallOperator (pPreConstructor, &ArgList) : true;
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
	 
	CValue ScopeBaseLevelValue = m_pModule->m_FunctionMgr.GetScopeLevelValue ();
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
COperatorMgr::CallClosureFunctionPtr (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	CFunctionPtrType* pFunctionPointerType = (CFunctionPtrType*) OpValue.GetType ();
	ASSERT (pFunctionPointerType->GetTypeKind () == EType_FunctionPtr);

	CFunctionType* pFunctionType = pFunctionPointerType->GetTargetType ();
	CFunctionType* pAbstractMethodType = pFunctionType->GetStdObjectMemberMethodType ();

	CheckFunctionPtrNull (OpValue);
	
	CValue PfnValue;
	CValue IfaceValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, pAbstractMethodType->GetFunctionPtrType (EFunctionPtrType_Thin), &PfnValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr), &IfaceValue);
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

		m_pModule->m_LlvmBuilder.CreateComment ("update scope level before call");
		m_pModule->m_LlvmBuilder.CreateStore (ScopeLevelValue, m_pModule->m_VariableMgr.GetScopeLevelVariable ());
	}

	m_pModule->m_LlvmBuilder.CreateCall (
		PfnValue,
		pFunctionType,
		ArgArray,
		ArgArray.GetCount (),
		pResultValue
		);

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {