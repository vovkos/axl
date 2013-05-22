#include "pch.h"
#include "axl_jnc_PropertyPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const char*
GetPropertyPtrTypeKindString (EPropertyPtrType PtrTypeKind)
{
	static const char* StringTable [EPropertyPtrType__Count] = 
	{
		"closure",  // EPropertyPtrType_Normal = 0,
		"weak",     // EPropertyPtrType_Weak,
		"thin",     // EPropertyPtrType_Thin,
		"unsafe",   // EPropertyPtrType_Unsafe,
	};

	return (size_t) PtrTypeKind < EPropertyPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		"undefined-property-ptr-kind";
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
CPropertyPtrType::GetPropertyPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetPropertyPtrStructType (m_pTargetType);
}

CStructType* 
CPropertyPtrType::GetPropertyPtrStructType_w ()
{
	return m_pModule->m_TypeMgr.GetPropertyPtrStructType_w (m_pTargetType);
}

CStructType* 
CPropertyPtrType::GetAuPropertyPtrStructType_t ()
{
	return m_pModule->m_TypeMgr.GetAuPropertyPtrStructType_t (m_pTargetType);
}

CStructType* 
CPropertyPtrType::GetAuPropertyPtrStructType_u ()
{
	return m_pModule->m_TypeMgr.GetAuPropertyPtrStructType_u (m_pTargetType);
}

rtl::CString
CPropertyPtrType::CreateSignature (
	CPropertyType* pPropertyType,
	EType TypeKind,
	EPropertyPtrType PtrTypeKind,
	uint_t Flags
	)
{
	rtl::CString Signature = TypeKind == EType_PropertyRef ? 'R' : 'P';

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

	if (Flags & EPtrTypeFlag_Nullable)
		Signature += 'n';

	Signature += pPropertyType->GetSignature ();
	return Signature;
}

void
CPropertyPtrType::PrepareTypeString ()
{
	m_TypeString = m_pTargetType->GetReturnType ()->GetTypeString ();
	m_TypeString += ' ';

	if (m_Flags & EPtrTypeFlag_Nullable)
		m_TypeString += "nullable ";

	m_TypeString += m_pTargetType->GetTypeModifierString ();

	if (m_PtrTypeKind != EPropertyPtrType_Normal)
	{
		m_TypeString += GetPropertyPtrTypeKindString (m_PtrTypeKind);
		m_TypeString += ' ';
	}

	m_TypeString += m_TypeKind == EType_PropertyRef ? "property&" : "property*";

	if (m_pTargetType->IsIndexed ())
	{
		m_TypeString += ' ';
		m_TypeString += m_pTargetType->GetGetterType ()->GetArgString ();
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
			m_pLlvmType = GetPropertyPtrStructType ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Weak:
			m_pLlvmType = GetPropertyPtrStructType_w ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Thin:
		case EPropertyPtrType_Unsafe:
			m_pLlvmType = m_pTargetType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe)->GetLlvmType ();
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

		case EPropertyPtrType_Weak:
			m_pLlvmType = GetPropertyPtrStructType_w ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Thin:
			m_pLlvmType = GetAuPropertyPtrStructType_t ()->GetLlvmType ();
			break;

		case EPropertyPtrType_Unsafe:
			m_pLlvmType = GetAuPropertyPtrStructType_u ()->GetLlvmType ();
			break;

		default:
			ASSERT (false);
		}
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
