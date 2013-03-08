#include "stdafx.h"
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
		return false;

	EType TypeKind = OpValue.GetType ()->GetTypeKind ();
	if (TypeKind != EType_FunctionRef && TypeKind != EType_FunctionPtr)
	{
		err::SetFormatStringError (_T("closure operator cannot be applied to '%s'"), OpValue.GetType ()->GetTypeString ());
		return false;
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
		err::SetFormatStringError (_T("closure operator cannot be applied to '%s'"), OpValue.GetType ()->GetTypeString ());
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
			return false;

		CFunctionType* pFunctionType = RawOpValue.GetFunctionTypeOverload ()->GetOverload (i);
		return pFunctionType->GetReturnType ();
	}
	
	ASSERT (OpValueKind == EValue_Type);
	PrepareOperandType (RawOpValue, &OpValue);
		
	CFunctionType* pFunctionType;
		
	CType* pType = OpValue.GetType ();
	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Function:
		pFunctionType = (CFunctionType*) pType;
		break;

	case EType_FunctionRef:
	case EType_FunctionPtr:
		pFunctionType = ((CFunctionPtrType*) pType)->GetTargetType ();
		break;

	default:
		err::SetFormatStringError (_T("cannot call '%s'"), OpValue.GetType ()->GetTypeString ());
		return false;
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
		int OpFlags = OpValue.GetType ()->GetTypeKind () == EType_Multicast ? EOpFlag_KeepDataRef : 0;

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
			Result = GetVirtualMethodMember (pFunction, pClosure, &OpValue);
			if (!Result)
				return false;
		}

		return CallImpl (OpValue, pFunction->GetType (), pArgList, pResultValue);
	}

	CType* pOpType = OpValue.GetType ();
	EType OpTypeKind = pOpType->GetTypeKind ();

	if (OpTypeKind != EType_FunctionRef && OpTypeKind != EType_FunctionPtr)
	{
		err::SetFormatStringError (_T("cannot call '%s'"), pOpType->GetTypeString ());
		return false;
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
	ASSERT (pFunction->GetFunctionKind () == EFunction_Constructor);

	CClassType* pClassType = pFunction->GetClassType ();
	CBaseType* pBaseType = pClassType->FindBaseType (pType);

	if (!pBaseType)
	{
		err::SetFormatStringError (_T("'%s' is not a base type of '%s'"), pType->GetTypeString (), pClassType->GetTypeString ());
		return false;
	}
	
	if (pType->GetTypeKind () != EType_Class)
	{
		err::SetFormatStringError (_T("non-class inheritance is not implemented yet"));
		return false;
	}

	CClassType* pBaseClassType = (CClassType*) pType;
	CFunction* pConstructor = pBaseClassType->GetConstructor ();
	if (!pConstructor)
	{
		err::SetFormatStringError (_T("'%s' has no constructor"), pBaseClassType->GetTypeString ());
		return false;
	}

	pArgList->InsertHead (m_pModule->m_FunctionMgr.GetThisValue ());

	Result = CallOperator (pConstructor, pArgList);
	if (!Result)
		return false;

	pClassType->MarkConstructed (pBaseType);
	return true;
}

bool
COperatorMgr::PostBaseTypeConstructorList ()
{
	bool Result;

	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction->GetFunctionKind () == EFunction_Constructor);

	rtl::CBoxListT <CValue> ArgList;
	ArgList.InsertHead (m_pModule->m_FunctionMgr.GetThisValue ());

	CClassType* pClassType = pFunction->GetClassType ();	
	rtl::CIteratorT <CBaseType> BaseType = pClassType->GetBaseTypeList ().GetHead ();
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
			err::SetFormatStringError (_T("'%s' has no default constructor"), pBaseClassType->GetTypeString ());
			return false;
		}

		Result = CallOperator (pConstructor, &ArgList);
		if (!Result)
			return false;
	}

	CFunction* pPreConstructor = pFunction->GetClassType ()->GetPreConstructor ();
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
COperatorMgr::CallClosureFunctionPtr (
	const CValue& OpValue,
	rtl::CBoxListT <CValue>* pArgList,
	CValue* pResultValue
	)
{
	CFunctionPtrType* pFunctionPointerType = (CFunctionPtrType*) OpValue.GetType ();
	ASSERT (pFunctionPointerType->GetTypeKind () == EType_FunctionPtr);

	CFunctionType* pFunctionType = pFunctionPointerType->GetTargetType ();
	CFunctionType* pAbstractMethodType = pFunctionType->GetStdObjectMethodMemberType ();

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
