#include "stdafx.h"
#include "axl_jnc_OperatorMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
COperatorMgr::GetPropertyGetter (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("get property 'get' member is not implemented yet"));
	return false;
}

bool
COperatorMgr::GetPropertySetter (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("get property 'set' member is not implemented yet"));
	return false;
}

bool
COperatorMgr::GetPropertyPropValue (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("get property 'propvalue' member is not implemented yet"));
	return false;
}

bool
COperatorMgr::GetPropertyOnChangeEvent (
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	err::SetFormatStringError (_T("get property 'onchange' member is not implemented yet"));
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
COperatorMgr::GetPropertyOperator (
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	ASSERT (RawOpValue.GetType ()->GetTypeKind () == EType_PropertyRef);

	err::SetFormatStringError (_T("get property operator is not implemented yet"));
	return false;

/*
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

*/
}

bool
COperatorMgr::SetPropertyOperator (
	const CValue& SrcValue,
	const CValue& RawDstValue
	)
{
	ASSERT (RawDstValue.GetType ()->GetTypeKind () == EType_PropertyRef);

	err::SetFormatStringError (_T("get property operator is not implemented yet"));
	return false;

/*
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
		bool Result = PrepareDataRef (EventValue, EPrepareDataRefFlag_Store, &EventPtrValue);
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

	*/
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
