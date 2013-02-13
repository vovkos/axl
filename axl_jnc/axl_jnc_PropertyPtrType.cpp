#include "stdafx.h"
#include "axl_jnc_PropertyPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetPropertyPtrTypeKindString (EPropertyPtrType PtrTypeKind)
{
	static const tchar_t* StringTable [EPropertyPtrType__Count] = 
	{
		_T("closure"),  // EPropertyPtrType_Normal = 0,
		_T("weak"),     // EPropertyPtrType_Weak,
		_T("thin"),     // EPropertyPtrType_Thin,
		_T("unsafe"),   // EPropertyPtrType_Unsafe,
	};

	return PtrTypeKind >= 0 && PtrTypeKind < EPropertyPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		_T("undefined-property-ptr-kind");
}

//.............................................................................

CPropertyPtrType::CPropertyPtrType ()
{
	m_TypeKind = EType_PropertyPtr;
	m_PtrTypeKind = EPropertyPtrType_Normal;
	m_Size = sizeof (TPropertyPtr);
	m_pTargetType = NULL;
}

CStructType* 
CPropertyPtrType::GetClosurePropertyPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetClosurePropertyPtrStructType (m_pTargetType);
}

CStructType* 
CPropertyPtrType::GetWeakClosurePropertyPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetWeakClosurePropertyPtrStructType (m_pTargetType);
}

CStructType* 
CPropertyPtrType::GetThinAuPropertyPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetThinAuPropertyPtrStructType (m_pTargetType);
}

CStructType* 
CPropertyPtrType::GetUnsafeAuPropertyPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetUnsafeAuPropertyPtrStructType (m_pTargetType);
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
	m_TypeString = m_pTargetType->GetReturnType ()->GetTypeString ();
	m_TypeString += _T(' ');

	if (m_Flags & EPtrTypeFlag_NoNull)
		m_TypeString += _T("nonull ");

	m_TypeString += m_pTargetType->GetTypeModifierString ();

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += GetPropertyPtrTypeKindString (m_PtrTypeKind);
		m_TypeString += _T(' ');
	}

	m_TypeString += m_TypeKind == EType_PropertyRef ? "property&" : "property*";

	if (m_pTargetType->IsIndexed ())
	{
		m_TypeString += _T(' ');
		m_TypeString += m_pTargetType->GetGetterType ()->GetArgTypeString ();
	}
}

void
CPropertyPtrType::PrepareLlvmType ()
{
	if (!(m_pTargetType->GetFlags () & EPropertyTypeFlag_Bindable))
	{
		switch (m_PtrTypeKind)
		{
		case EPropertyPtrType_Normal:
			m_pLlvmType = GetClosurePropertyPtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Thin:
		case EPropertyPtrType_Unsafe:
			m_pLlvmType = m_pTargetType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe)->GetLlvmType ();
			break;

		case EPropertyPtrType_Weak:
			m_pLlvmType = GetWeakClosurePropertyPtrStructType ()->GetLlvmType ();
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
			m_pLlvmType = GetClosurePropertyPtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Thin:
			m_pLlvmType = GetThinAuPropertyPtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Unsafe:
			m_pLlvmType = GetUnsafeAuPropertyPtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Weak:
			m_pLlvmType = GetWeakClosurePropertyPtrStructType ()->GetLlvmType ();
			break;

		default:
			ASSERT (false);
		}
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
