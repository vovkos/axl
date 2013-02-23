#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::GetPropertyThinPtr (
	CProperty* pProperty,
	CClosure* pClosure,
	CPropertyPtrType* pPtrType,
	CValue* pResultValue
	)
{
	ASSERT (pProperty->GetType ()->Cmp (pPtrType->GetTargetType ()) == 0);

	if (pProperty->GetType ()->GetFlags () & EPropertyTypeFlag_Augmented)
	{
		err::SetFormatStringError (_T("augmented properties are not yet supported"), pPtrType->GetTypeString ());
		return false;
	}

	bool Result = GetPropertyVTable (pProperty, pClosure, pResultValue);
	if (!Result)
		return false;

	pResultValue->OverrideType (pPtrType);
	return true;
}

bool
COperatorMgr::GetPropertyVTable (
	CProperty* pProperty,
	CClosure* pClosure,
	CValue* pResultValue
	)
{
	if (pProperty->IsVirtual ())
	{
		bool Result = GetVirtualPropertyMember (pProperty, pClosure, pResultValue);
		if (!Result)
			return false;
	}

	return pProperty->GetVTablePtrValue (pResultValue);
}

bool
COperatorMgr::GetPropertyVTable (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);
	
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	EPropertyPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	CType* pVTableType = pPtrType->GetTargetType ()->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe);

	switch (PtrTypeKind)
	{
	case EPropertyPtrType_Normal:
		break;

	case EPropertyPtrType_Weak:
		err::SetFormatStringError (_T("cannot invoke weak '%s'"), pPtrType->GetTypeString ());
		return false;
	
	case EPropertyPtrType_Thin:
		if (OpValue.GetValueKind () == EValue_Property)
			return GetPropertyVTable (OpValue.GetProperty (), OpValue.GetClosure (), pResultValue);

		// else fall through

	case EPropertyPtrType_Unsafe:
		if (pPtrType->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Augmented)
			m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, pVTableType, pResultValue);
		else
			*pResultValue = OpValue;

		return true;

	default:
		ASSERT (false);
	}

	CType* pClosureType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr);

	CValue ClosureValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 0, pVTableType, pResultValue);
	m_pModule->m_LlvmBuilder.CreateExtractValue (OpValue, 1, pClosureType, &ClosureValue);

	pResultValue->SetClosure (OpValue.GetClosure ());

	CClosure* pClosure = pResultValue->GetClosure ();
	if (!pClosure)
		pClosure = pResultValue->CreateClosure ();

	pClosure->GetArgList ()->InsertHead (ClosureValue);
	return true;
}

bool
COperatorMgr::GetPropertyGetter (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	bool Result;

	if (OpValue.GetValueKind () == EValue_Property)
	{
		*pResultValue = OpValue.GetProperty ()->GetGetter ();
		pResultValue->SetClosure (OpValue.GetClosure ());
		return true;
	}

	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);	
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->HasClosure () ? 
		pPtrType->GetTargetType ()->GetAbstractPropertyMemberType () :
		pPtrType->GetTargetType ();

	CValue VTableValue;
	Result = GetPropertyVTable (OpValue, &VTableValue);
	if (!Result)
		return false;

	EFunctionPtrType GetterPtrTypeKind = pPtrType->GetPtrTypeKind () == EPropertyPtrType_Unsafe ? 
		EFunctionPtrType_Unsafe : 
		EFunctionPtrType_Thin;

	CValue PfnValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (VTableValue, 0, NULL, &PfnValue);
	m_pModule->m_LlvmBuilder.CreateLoad (
		PfnValue, 
		pPropertyType->GetGetterType ()->GetFunctionPtrType (GetterPtrTypeKind), 
		pResultValue
		);

	pResultValue->SetClosure (VTableValue.GetClosure ());
	return true;
}

bool
COperatorMgr::GetPropertySetter (
	const CValue& OpValue,
	const CValue& ArgValue,
	CValue* pResultValue
	)
{
	bool Result;

	if (OpValue.GetValueKind () == EValue_Property)
	{
		*pResultValue = OpValue.GetProperty ()->GetSetter ();
		pResultValue->SetClosure (OpValue.GetClosure ());
		return true;
	}

	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);	
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->HasClosure () ? 
		pPtrType->GetTargetType ()->GetAbstractPropertyMemberType () :
		pPtrType->GetTargetType ();

	if (pPropertyType->IsReadOnly ())
	{
		err::SetFormatStringError (_T("read-only '%s' has no setter"), pPropertyType->GetTypeString ());
		return false;
	}

	size_t i;

	CFunctionTypeOverload* pSetterType = pPropertyType->GetSetterType (); 
	if (!pSetterType->IsOverloaded ())
	{
		i = 0;
	}
	else if (ArgValue.IsEmpty ())
	{
		err::SetFormatStringError (_T("cannot choose one of '%d' setter overloads"), pSetterType->GetOverloadCount ());
		return false;
	}
	else
	{
		rtl::CBoxListT <CValue> ArgList;
		ArgList.InsertTail (ArgValue);

		CClosure* pClosure = OpValue.GetClosure ();
		if (pClosure)
		{
			Result = pClosure->Apply (&ArgList);
			if (!Result)
				return false;
		}
		
		i = pSetterType->ChooseOverload (ArgList);
		if (i == -1)
			return false;
	}
	
	CValue VTableValue;
	Result = GetPropertyVTable (OpValue, &VTableValue);
	if (!Result)
		return false;

	EFunctionPtrType SetterPtrTypeKind = pPtrType->GetPtrTypeKind () == EPropertyPtrType_Unsafe ? 
		EFunctionPtrType_Unsafe : 
		EFunctionPtrType_Thin;

	CValue PfnValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (VTableValue, i + 1, NULL, &PfnValue);
	m_pModule->m_LlvmBuilder.CreateLoad (
		PfnValue, 
		pSetterType->GetOverload (i)->GetFunctionPtrType (SetterPtrTypeKind), 
		pResultValue
		);

	pResultValue->SetClosure (VTableValue.GetClosure ());
	return true;
}

bool
COperatorMgr::GetPropertyPropValue (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	if (OpValue.GetValueKind () != EValue_Property)
	{
		err::SetFormatStringError (_T("'propvalue' field for property pointers is not implemented yet"));
		return false;
	}

	CProperty* pProperty = OpValue.GetProperty ();
	CStructField* pField = pProperty->GetPropValue ();
	if (!pField)
	{
		err::SetFormatStringError (_T("'%s' has no 'propvalue' field"), pProperty->m_Tag);
		return false;
	}

	if (pField->GetStorageKind () != EStorage_Static)
	{
		err::SetFormatStringError (_T("non-static 'propvalue' is not implemented yet"));
		return false;
	}

	CVariable* pStaticVariable = pProperty->GetStaticDataVariable ();
	ASSERT (pStaticVariable);

	CBaseTypeCoord Coord;
	return GetStructFieldMember (pStaticVariable, pField, &Coord, pResultValue);
}

bool
COperatorMgr::GetPropertyOnChangeEvent (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("'onchange' is not implemented yet"));
	return false;

/*

	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);
	if (!Result)
		return false;

	CType* pOpType = OpValue.GetType ();
	if (!pOpType->IsBindablePropertyType ())
	{
		err::SetFormatStringError (_T("'onchange' can only be applied to bindable properties"));
		return false;
	}

	EType OpTypeKind = pOpType->GetTypeKind ();
	if (OpTypeKind != EType_Property)
	{
		err::SetFormatStringError (_T("'onchange' for property pointers is not implemented yet"));
		return false;
	}

	CPropertyType* pPropertyType = (CPropertyType*) pOpType;
	if (pPropertyType->GetParentClassType ())
	{
		err::SetFormatStringError (_T("'onchange' for member properties is not implemented yet"));
		return false;
	}

//	CVariable* pEventVariable = pPropertyType->GetEventVariable ();
//	pResultValue->SetVariable (pEventVariable);
	return true;
*/
}

bool
COperatorMgr::GetProperty (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	
	if (pPtrType->GetTargetType ()->GetFlags () & EPropertyTypeFlag_AutoGet)
		return GetPropertyPropValue (OpValue, pResultValue);

	CValue GetterValue;
	return 
		GetPropertyGetter (OpValue, &GetterValue) &&
		CallOperator (GetterValue, NULL, pResultValue);
}

bool
COperatorMgr::SetProperty (
	const CValue& OpValue,
	const CValue& SrcValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	
	rtl::CBoxListT <CValue> ArgList;
	ArgList.InsertTail (SrcValue);

	CValue SetterValue;
	CValue ReturnValue;
	return 
		GetPropertySetter (OpValue, SrcValue, &SetterValue) &&
		CallOperator (SetterValue, &ArgList, &ReturnValue);
}


/*

bool
COperatorMgr::GetProperty (
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	ASSERT (RawOpValue.GetType ()->GetTypeKind () == EType_PropertyRef);

	CPropertyType* pPropertyType;
	CValue OpValue;
	CValue InterfaceValue;

	if (RawOpValue.GetType ()->GetTypeKind () == EType_PropertyPtr)
	{
		m_pModule->m_LlvmBuilder.CheckNullPtr (RawOpValue);

		CPropertyPtrType* pPropertyPointerType = (CPropertyPtrType*) RawOpValue.GetType ();
		pPropertyType = pPropertyPointerType->GetPropertyType ();
		m_pModule->m_LlvmBuilder.CreateExtractValue (RawOpValue, 0, pPropertyType, &OpValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (RawOpValue, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_Object), &InterfaceValue);
	}
	else
	{
		ASSERT (RawOpValue.GetType ()->GetTypeKind () == EType_Property);
		pPropertyType = (CPropertyType*) RawOpValue.GetType ();
		OpValue = RawOpValue;
	}
	
	CFunction* pFunction = pPropertyType->GetGetter ();
	ASSERT (pFunction->GetPropertyType () == pPropertyType);

	CValue FunctionValue;

	if (OpValue.GetValueKind () == EValue_Property)
	{
		FunctionValue = pFunction;
	}
	else
	{
		// pfn*

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (OpValue, pFunction->GetPropertyVTableIndex (), NULL, &PtrValue);

		// pfn

		m_pModule->m_LlvmBuilder.CreateLoad (PtrValue, pFunction->GetType ()->GetDataPtrType (EType_DataPtr_u), &FunctionValue);
	}

	if (RawOpValue.GetType ()->GetTypeKind () == EType_PropertyPtr)
	{
		m_pModule->m_LlvmBuilder.CreateClosureFunctionPtr (
			FunctionValue, 
			InterfaceValue,
			pFunction->GetType ()->GetFunctionPtrType (),
			&FunctionValue
			);
	}

	FunctionValue.SetClosure (OpValue.GetClosure ());
	return CallOperator (FunctionValue, NULL, pResultValue);
}

bool
COperatorMgr::SetProperty (
	const CValue& SrcValue,
	const CValue& RawDstValue
	)
{
	ASSERT (RawDstValue.GetType ()->GetTypeKind () == EType_PropertyRef);

	bool Result;

	CPropertyType* pPropertyType;
	CValue DstValue;
	CValue InterfaceValue;
	CValue EventValue;

	if (RawDstValue.GetType ()->GetTypeKind () == EType_PropertyPtr)
	{
		CPropertyPtrType* pPropertyPointerType = (CPropertyPtrType*) RawDstValue.GetType ();
		pPropertyType = pPropertyPointerType->GetPropertyType ();
		m_pModule->m_LlvmBuilder.CreateExtractValue (RawDstValue, 0, pPropertyType, &DstValue);
		m_pModule->m_LlvmBuilder.CreateExtractValue (RawDstValue, 1, m_pModule->m_TypeMgr.GetStdType (EStdType_Object), &InterfaceValue);
	}
	else
	{
		ASSERT (RawDstValue.GetType ()->GetTypeKind () == EType_Property);
		pPropertyType = (CPropertyType*) RawDstValue.GetType ();
		DstValue = RawDstValue;
		
		if (pPropertyType->GetFlags () & EPropertyTypeFlag_Bindable)
		{
			CVariable* pEventVariable = pPropertyType->GetEventVariable ();
			EventValue.SetVariable (pEventVariable);
		}
	}

	if (pPropertyType->IsReadOnly ())
	{
		err::SetFormatStringError (_T("cannot assign to a read-only property"));
		return false;
	}

	CClosure* pClosure = DstValue.GetClosure ();

	rtl::CBoxListT <CValue> ArgList;
	ArgList.InsertTail (SrcValue);

	if (pClosure)
		pClosure->Apply (&ArgList);

	CFunction* pSetter = pPropertyType->GetSetter);
	CFunction* pFunction = pSetter->ChooseOverload (&ArgList);
	if (!pFunction)
		return false;

	ASSERT (pFunction->GetPropertyType () == pPropertyType);

	CValue FunctionValue;
	if (DstValue.GetValueKind () == EValue_Property)
	{
		FunctionValue = pFunction;
	}
	else
	{
		// pfn*

		CValue PtrValue;
		m_pModule->m_LlvmBuilder.CreateGep2 (DstValue, pFunction->GetPropertyVTableIndex (), NULL, &PtrValue);

		// pfn

		m_pModule->m_LlvmBuilder.CreateLoad (PtrValue, pFunction->GetType ()->GetDataPtrType (EType_DataPtr_u), &FunctionValue);
	}

	if (RawDstValue.GetType ()->GetTypeKind () == EType_PropertyPtr)
	{
		m_pModule->m_LlvmBuilder.CreateClosureFunctionPtr (
			FunctionValue, 
			InterfaceValue,
			pFunction->GetType ()->GetFunctionPtrType (),
			&FunctionValue
			);
	}

	CValue ReturnValue;
	Result = CallOperator (FunctionValue, &ArgList, &ReturnValue);
	if (!Result)
		return false;

	if (!EventValue.IsEmpty ())
	{
		CValue EventPtrValue;
		bool Result = PrepareDataPtr (EventValue, EPrepareDataPtrFlag_Store, &EventPtrValue);
		if (!Result)
			return false;

		CFunction* pFireSimpleEvent = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_FireSimpleEvent);
		m_pModule->m_LlvmBuilder.CreateCall (
			pFireSimpleEvent,
			pFireSimpleEvent->GetType (),
			EventPtrValue,
			&ReturnValue
			);
	}

	return true;
}

*/

//.............................................................................

} // namespace jnc {
} // namespace axl {
