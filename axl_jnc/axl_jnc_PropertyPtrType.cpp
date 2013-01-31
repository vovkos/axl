#include "stdafx.h"
#include "axl_jnc_PropertyPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetPropertyPtrTypeKindString (EPropertyPtrType PtrTypeKind)
{
	switch (PtrTypeKind)
	{
	case EPropertyPtrType_Normal:
		return _T("closure");

	case EPropertyPtrType_Thin:
		return _T("thin");

	case EPropertyPtrType_Weak:
		return _T("weak");

	case EPropertyPtrType_Unsafe:
		return _T("unsafe");

	default:
		return _T("undefined-property-ptr-kind");
	}
}

//.............................................................................

CPropertyPtrType::CPropertyPtrType ()
{
	m_TypeKind = EType_PropertyPtr;
	m_PtrTypeKind = EPropertyPtrType_Normal;
	m_Size = sizeof (TPropertyPtr);
	m_pPropertyType = NULL;
}

CStructType* 
CPropertyPtrType::GetPropertyPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetPropertyPtrStructType (m_pPropertyType);
}

CStructType* 
CPropertyPtrType::GetPropertyWeakPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetPropertyWeakPtrStructType (m_pPropertyType);
}

CStructType* 
CPropertyPtrType::GetBindablePropertyThinPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetBindablePropertyThinPtrStructType (m_pPropertyType);
}

CStructType* 
CPropertyPtrType::GetBindablePropertyUnsafePtrStructType ()
{
	return m_pModule->m_TypeMgr.GetBindablePropertyUnsafePtrStructType (m_pPropertyType);
}

rtl::CStringA
CPropertyPtrType::CreateSignature (
	CPropertyType* pPropertyType,
	EType TypeKind,
	EPropertyPtrType PtrTypeKind,
	int Flags
	)
{
	rtl::CStringA Signature = TypeKind == EType_PropertyRef ? 'R' : 'P';

	switch (PtrTypeKind)
	{
	case EPropertyPtrType_Thin:
		Signature += 't';
		break;

	case EPropertyPtrType_Weak:
		Signature += 'w';
		break;

	case EPropertyPtrType_Unsafe:
		Signature += 'u';
		break;
	}

	if (Flags & EPtrTypeFlag_NoNull)
		Signature += 'n';

	Signature += pPropertyType->GetSignature ();
	return Signature;
}

void
CPropertyPtrType::PrepareTypeString ()
{
	m_TypeString = m_pPropertyType->GetReturnType ()->GetTypeString ();
	m_TypeString += m_pPropertyType->GetTypeModifierString ();

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += _T(' ');
		m_TypeString += GetPropertyPtrTypeKindString (m_PtrTypeKind);
	}

	if (m_Flags & EPtrTypeFlag_NoNull)
		m_TypeString += _T(" nonull");

	m_TypeString += m_TypeKind == EType_PropertyRef ? " property&" : " property*";

	if (m_pPropertyType->IsIndexed ())
	{
		m_TypeString += _T(' ');
		m_TypeString += m_pPropertyType->GetGetterType ()->GetArgTypeString ();
	}
}

void
CPropertyPtrType::PrepareLlvmType ()
{
	if (!(m_pPropertyType->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		switch (m_PtrTypeKind)
		{
		case EPropertyPtrType_Normal:
			m_pLlvmType = GetPropertyPtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Thin:
		case EPropertyPtrType_Unsafe:
			m_pLlvmType = m_pPropertyType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe)->GetLlvmType ();
			break;

		case EPropertyPtrType_Weak:
			m_pLlvmType = GetPropertyWeakPtrStructType ()->GetLlvmType ();
			break;

		default:
			ASSERT (false);
		}
	}
	else
	{
		switch (m_PtrTypeKind)
		{
		case EPropertyPtrType_Normal:
			m_pLlvmType = GetPropertyPtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Thin:
			m_pLlvmType = GetBindablePropertyThinPtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Unsafe:
			m_pLlvmType = GetBindablePropertyUnsafePtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Weak:
			m_pLlvmType = GetPropertyWeakPtrStructType ()->GetLlvmType ();
			break;

		default:
			ASSERT (false);
		}
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
