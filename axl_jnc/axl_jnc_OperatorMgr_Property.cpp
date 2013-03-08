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

	*pResultValue = pProperty->GetVTablePtrValue ();
	return true;
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

CType*
COperatorMgr::GetPropertyGetterType (const CValue& OpValue)
{
	CPropertyType* pPropertyType;

	if (OpValue.GetValueKind () == EValue_Property)
	{
		pPropertyType = OpValue.GetProperty ()->GetType ();
	}
	else
	{
		ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);	
		CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
		pPropertyType = pPtrType->HasClosure () ? 
			pPtrType->GetTargetType ()->GetStdObjectPropertyMemberType () :
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
		pPtrType->GetTargetType ()->GetStdObjectPropertyMemberType () :
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

CType*
COperatorMgr::GetPropertySetterType (
	const CValue& OpValue,
	const CValue& ArgValue
	)
{
	CPropertyType* pPropertyType;

	if (OpValue.GetValueKind () == EValue_Property)
	{
		pPropertyType = OpValue.GetProperty ()->GetType ();
	}
	else
	{
		ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);	
		CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
		pPropertyType = pPtrType->HasClosure () ? 
			pPtrType->GetTargetType ()->GetStdObjectPropertyMemberType () :
			pPtrType->GetTargetType ();
	}

	if (pPropertyType->IsReadOnly ())
	{
		err::SetFormatStringError (_T("read-only '%s' has no setter"), pPropertyType->GetTypeString ());
		return NULL;
	}

	CFunctionTypeOverload* pSetterTypeOverload = pPropertyType->GetSetterType ();
	size_t i = pSetterTypeOverload->ChooseSetterOverload (ArgValue);
	if (i == -1)
	{
		err::SetFormatStringError (_T("cannot choose one of '%d' setter overloads"), pSetterTypeOverload->GetOverloadCount ());
		return false;
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
		pPtrType->GetTargetType ()->GetStdObjectPropertyMemberType () :
		pPtrType->GetTargetType ();

	if (pPropertyType->IsReadOnly ())
	{
		err::SetFormatStringError (_T("read-only '%s' has no setter"), pPropertyType->GetTypeString ());
		return false;
	}

	CFunctionTypeOverload* pSetterTypeOverload = pPropertyType->GetSetterType ();
	size_t i = pSetterTypeOverload->ChooseSetterOverload (ArgValue);
	if (i == -1)
	{
		err::SetFormatStringError (_T("cannot choose one of '%d' setter overloads"), pSetterTypeOverload->GetOverloadCount ());
		return false;
	}

	CFunctionType* pSetterType = pSetterTypeOverload->GetOverload (i);

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
		pSetterType->GetFunctionPtrType (SetterPtrTypeKind), 
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
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();
	
	if (pPtrType->GetTargetType ()->GetFlags () & EPropertyTypeFlag_AutoGet)
	{
		return GetAuPropertyFieldMember (OpValue, EAuPropertyField_PropValue, pResultValue);
	}
	else
	{
		CValue GetterValue;
		return 
			GetPropertyGetter (OpValue, &GetterValue) &&
			CallOperator (GetterValue, NULL, pResultValue);
	}
}

bool
COperatorMgr::SetProperty (
	const CValue& OpValue,
	const CValue& SrcValue
	)
{
	ASSERT (OpValue.GetType ()->GetTypeKind () == EType_PropertyRef);
	CPropertyPtrType* pPtrType = (CPropertyPtrType*) OpValue.GetType ();

	if (pPtrType->GetTargetType ()->GetFlags () & EPropertyTypeFlag_AutoSet)
	{	
		ASSERT (pPtrType->GetTargetType ()->GetFlags () & EPropertyTypeFlag_Bindable);

		CValue PropValue;
		CValue OnChangeValue;
		
		return
			GetAuPropertyFieldMember (OpValue, EAuPropertyField_PropValue, &PropValue) &&
			GetAuPropertyFieldMember (OpValue, EAuPropertyField_OnChange, &OnChangeValue) &&
			StoreDataRef (PropValue, SrcValue) &&
			CallOperator (OnChangeValue);
	}
	else
	{
		CValue SetterValue;

		return 
			GetPropertySetter (OpValue, SrcValue, &SetterValue) &&
			CallOperator (SetterValue, SrcValue);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
