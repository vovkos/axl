#include "pch.h"
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
		return GetVirtualProperty (pProperty, pClosure, pResultValue);

	*pResultValue = pProperty->GetVTablePtrValue ();
	return true;
}

bool
COperatorMgr::GetPropertyVTable (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);
	
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	EPropertyPtrType PtrTypeKind = pPtrType->GetPtrTypeKind ();

	CType* pVTableType = pPtrType->GetTargetType ()->GetVTableStructType ()->GetDataPtrType_c ();

	switch (PtrTypeKind)
	{
	case EPropertyPtrType_Normal:
		break;

	case EPropertyPtrType_Weak:
		err::SetFormatStringError ("cannot invoke weak '%s'", pPtrType->GetTypeString ().cc ());
		return false;
	
	case EPropertyPtrType_Thin:
		if (OpValue.GetValueKind () == EValue_Property)
			return GetPropertyVTable (OpValue.GetProperty (), OpValue.GetClosure (), pResultValue);

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
	pResultValue->InsertToClosureHead (ClosureValue);
	return true;
}

CType*
COperatorMgr::GetPropertyGetterType (const CValue& RawOpValue)
{
	CPropertyType* pPropertyType;

	CValue OpValue;
	PrepareOperandType (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);

	if (OpValue.GetValueKind () == EValue_Property)
	{
		pPropertyType = OpValue.GetProperty ()->GetType ();
	}
	else
	{
		ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);

		CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
		pPropertyType = pPtrType->HasClosure () ? 
			pPtrType->GetTargetType ()->GetStdObjectMemberPropertyType () :
			pPtrType->GetTargetType ();
	}

	return GetFunctionType (OpValue, pPropertyType->GetGetterType ());
}

bool
COperatorMgr::GetPropertyGetterType (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pResultType = GetPropertyGetterType (OpValue);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
	return true;
}

bool
COperatorMgr::GetPropertyGetter (
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	bool Result;

	CValue OpValue;
	Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);
	if (!Result)
		return false;

	if (OpValue.GetValueKind () == EValue_Property)
	{
		*pResultValue = OpValue.GetProperty ()->GetGetter ();
		pResultValue->SetClosure (OpValue.GetClosure ());
		return true;
	}	

	ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);	

	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->HasClosure () ? 
		pPtrType->GetTargetType ()->GetStdObjectMemberPropertyType () :
		pPtrType->GetTargetType ();

	CValue VTableValue;
	Result = GetPropertyVTable (OpValue, &VTableValue);
	if (!Result)
		return false;

	size_t Index = (pPropertyType->GetFlags () & EPropertyTypeFlag_Bindable) ? 1 : 0;

	CValue PfnValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (VTableValue, Index, NULL, &PfnValue);
	m_pModule->m_LlvmBuilder.CreateLoad (
		PfnValue, 
		pPropertyType->GetGetterType ()->GetFunctionPtrType (EFunctionPtrType_Thin, pPtrType->GetFlags ()), 
		pResultValue
		);

	pResultValue->SetClosure (VTableValue.GetClosure ());
	return true;
}

CType*
COperatorMgr::GetPropertySetterType (
	const CValue& RawOpValue,
	const CValue& ArgValue
	)
{
	CValue OpValue;
	PrepareOperandType (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);

	CPropertyType* pPropertyType;

	if (OpValue.GetValueKind () == EValue_Property)
	{
		pPropertyType = OpValue.GetProperty ()->GetType ();
	}
	else
	{
		ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);

		CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
		pPropertyType = pPtrType->HasClosure () ? 
			pPtrType->GetTargetType ()->GetStdObjectMemberPropertyType () :
			pPtrType->GetTargetType ();
	}

	if (pPropertyType->IsReadOnly ())
	{
		err::SetFormatStringError ("read-only '%s' has no setter", pPropertyType->GetTypeString ().cc ());
		return NULL;
	}

	CFunctionTypeOverload* pSetterTypeOverload = pPropertyType->GetSetterType ();
	size_t i = pSetterTypeOverload->ChooseSetterOverload (ArgValue);
	if (i == -1)
	{
		err::SetFormatStringError ("cannot choose one of '%d' setter overloads", pSetterTypeOverload->GetOverloadCount ());
		return NULL;
	}

	CFunctionType* pSetterType = pSetterTypeOverload->GetOverload (i);
	return GetFunctionType (OpValue, pSetterType);
}

bool
COperatorMgr::GetPropertySetterType (
	const CValue& OpValue,
	const CValue& ArgValue,
	CValue* pResultValue
	)
{
	CType* pResultType = GetPropertySetterType (OpValue, ArgValue);
	if (!pResultType)
		return false;

	pResultValue->SetType (pResultType);
	return true;
}

bool
COperatorMgr::GetPropertySetter (
	const CValue& RawOpValue,
	const CValue& ArgValue,
	CValue* pResultValue
	)
{
	bool Result;

	CValue OpValue;
	Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);
	if (!Result)
		return false;

	ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);	

	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->HasClosure () ? 
		pPtrType->GetTargetType ()->GetStdObjectMemberPropertyType () :
		pPtrType->GetTargetType ();

	if (pPropertyType->IsReadOnly ())
	{
		err::SetFormatStringError ("read-only '%s' has no setter", pPropertyType->GetTypeString ().cc ());
		return false;
	}

	if (OpValue.GetValueKind () == EValue_Property)
	{
		*pResultValue = OpValue.GetProperty ()->GetSetter ();
		pResultValue->SetClosure (OpValue.GetClosure ());
		return true;
	}

	CFunctionTypeOverload* pSetterTypeOverload = pPropertyType->GetSetterType ();
	size_t i = 0;

	if (pSetterTypeOverload->IsOverloaded ())
	{
		if (!ArgValue)
		{
			err::SetFormatStringError ("no argument value to help choose one of '%d' setter overloads", pSetterTypeOverload->GetOverloadCount ());
			return false;
		}
		
		i = pSetterTypeOverload->ChooseSetterOverload (ArgValue);
		if (i == -1)
		{
			err::SetFormatStringError ("cannot choose one of '%d' setter overloads", pSetterTypeOverload->GetOverloadCount ());
			return false;
		}
	}

	CFunctionType* pSetterType = pSetterTypeOverload->GetOverload (i);

	CValue VTableValue;
	Result = GetPropertyVTable (OpValue, &VTableValue);
	if (!Result)
		return false;

	size_t Index = (pPropertyType->GetFlags () & EPropertyTypeFlag_Bindable) ? 2 : 1;
	Index += i;

	CValue PfnValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (VTableValue, Index, NULL, &PfnValue);
	m_pModule->m_LlvmBuilder.CreateLoad (
		PfnValue, 
		pSetterType->GetFunctionPtrType (EFunctionPtrType_Thin, pPtrType->GetFlags ()), 
		pResultValue
		);

	pResultValue->SetClosure (VTableValue.GetClosure ());
	return true;
}

CType*
COperatorMgr::GetPropertyBinderType (const CValue& RawOpValue)
{
	CPropertyType* pPropertyType;

	CValue OpValue;
	PrepareOperandType (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);

	if (OpValue.GetValueKind () == EValue_Property)
	{
		pPropertyType = OpValue.GetProperty ()->GetType ();
	}
	else
	{
		ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);	

		CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
		pPropertyType = pPtrType->HasClosure () ? 
			pPtrType->GetTargetType ()->GetStdObjectMemberPropertyType () :
			pPtrType->GetTargetType ();
	}

	if (!(pPropertyType->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		err::SetFormatStringError ("'%s' has no 'onchange' binder", pPropertyType->GetTypeString ().cc ());
		return NULL;
	}

	return GetFunctionType (OpValue, pPropertyType->GetBinderType ());
}

bool
COperatorMgr::GetPropertyBinderType (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pType = GetPropertyBinderType (OpValue);
	if (!pType)
		return false;

	pResultValue->SetType (pType);
	return true;		
}

bool
COperatorMgr::GetPropertyBinder (
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	bool Result;

	CValue OpValue;
	Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);
	if (!Result)
		return false;

	ASSERT (OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr);	

	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	CPropertyType* pPropertyType = pPtrType->GetTargetType ();

	if (!(pPropertyType->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		err::SetFormatStringError ("'%s' has no 'onchange' binder", pPropertyType->GetTypeString ().cc ());
		return false;
	}

	if (OpValue.GetValueKind () == EValue_Property)
	{
		*pResultValue = OpValue.GetProperty ()->GetBinder ();
		pResultValue->SetClosure (OpValue.GetClosure ());
		return true;
	}

	if (pPtrType->HasClosure ())
		pPropertyType = pPropertyType->GetStdObjectMemberPropertyType ();

	CValue VTableValue;
	Result = GetPropertyVTable (OpValue, &VTableValue);
	if (!Result)
		return false;

	CValue PfnValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (VTableValue, 0, NULL, &PfnValue);
	m_pModule->m_LlvmBuilder.CreateLoad (
		PfnValue, 
		pPropertyType->GetBinderType ()->GetFunctionPtrType (EFunctionPtrType_Thin, pPtrType->GetFlags ()), 
		pResultValue
		);

	pResultValue->SetClosure (VTableValue.GetClosure ());
	return true;
}

bool
COperatorMgr::GetProperty (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);

	if (OpValue.GetValueKind () == EValue_Property)
	{
		CProperty* pProperty = OpValue.GetProperty ();
		if (pProperty->GetFlags () & EPropertyFlag_AutoGet)
			return GetPropertyPropValue (OpValue, pResultValue);
	}

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

	if (OpValue.GetValueKind () == EValue_Property)
	{
		CProperty* pProperty = OpValue.GetProperty ();
		if (pProperty->GetFlags () & EPropertyFlag_AutoSet)
		{
			ASSERT (pProperty->GetFlags () & EPropertyTypeFlag_Bindable);

			CValue PropValue;
			CValue OnChangeValue;
		
			return
				GetPropertyPropValue (OpValue, &PropValue) &&
				GetPropertyOnChange (OpValue, &OnChangeValue) &&
				StoreDataRef (PropValue, SrcValue) &&
				MemberOperator (OnChangeValue, "Call", &OnChangeValue) &&
				CallOperator (OnChangeValue);
		}
	}

	CValue SetterValue;
	return 
		GetPropertySetter (OpValue, SrcValue, &SetterValue) &&
		CallOperator (SetterValue, SrcValue);
}

CType*
COperatorMgr::GetPropertyPropValueType (const CValue& OpValue)
{
	if (OpValue.GetValueKind () != EValue_Property || 
		!(OpValue.GetProperty ()->GetFlags () & EPropertyFlag_AutoGet))
	{
		err::SetFormatStringError ("'%s' has no 'propvalue' field", OpValue.GetType ()->GetTypeString ().cc ());
		return NULL;
	}

	CType* pType;

	CModuleItem* pPropValue = OpValue.GetProperty ()->GetPropValue ();
	pType = pPropValue->GetItemKind () == EModuleItem_StructField ? 
		((CStructField*) pPropValue)->GetType() :
		((CVariable*) pPropValue)->GetType();

	return pType->GetDataPtrType (EType_DataRef, EDataPtrType_Thin);
}

bool
COperatorMgr::GetPropertyPropValueType (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pType = GetPropertyPropValueType (OpValue);
	if (!pType)
		return false;

	pResultValue->SetType (pType);
	return true;
}

bool
COperatorMgr::GetPropertyPropValue (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	if (OpValue.GetValueKind () != EValue_Property || 
		!(OpValue.GetProperty ()->GetFlags () & EPropertyFlag_AutoGet))
	{
		err::SetFormatStringError ("'%s' has no 'propvalue' field", OpValue.GetType ()->GetTypeString ().cc ());
		return false;
	}

	return GetPropertyField (OpValue, OpValue.GetProperty ()->GetPropValue (), pResultValue);
}

CType*
COperatorMgr::GetPropertyOnChangeType (const CValue& RawOpValue)
{
	CValue OpValue;
	PrepareOperandType (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);

	if (!(OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr) || 
		!(((CPropertyPtrType*) OpValue.GetType ())->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		err::SetFormatStringError ("'%s' has no 'onchange' field", OpValue.GetType ()->GetTypeString ().cc ());
		return NULL;
	}

	return m_pModule->GetSimpleType (EStdType_SimpleEventPtr);
}

bool
COperatorMgr::GetPropertyOnChangeType (
	const CValue& OpValue,
	CValue* pResultValue
	)
{
	CType* pType = GetPropertyPropValueType (OpValue);
	if (!pType)
		return false;

	pResultValue->SetType (pType);
	return true;
}

bool
COperatorMgr::GetPropertyOnChange (
	const CValue& RawOpValue,
	CValue* pResultValue
	)
{
	CValue OpValue;
	bool Result = PrepareOperand (RawOpValue, &OpValue, EOpFlag_KeepPropertyRef);
	if (!Result)
		return false;

	if (!(OpValue.GetType ()->GetTypeKindFlags () & ETypeKindFlag_PropertyPtr) || 
		!(((CPropertyPtrType*) OpValue.GetType ())->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		err::SetFormatStringError ("'%s' has no 'onchange' field", OpValue.GetType ()->GetTypeString ().cc ());
		return NULL;
	}

	if (OpValue.GetValueKind () == EValue_Property)
		return GetPropertyField (OpValue, OpValue.GetProperty ()->GetOnChange (), pResultValue);

	CValue BinderValue;
	return 
		GetPropertyBinder (OpValue, &BinderValue) &&
		CallOperator (BinderValue, NULL, pResultValue);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
