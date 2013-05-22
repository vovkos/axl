#include "pch.h"
#include "axl_jnc_AutoEvPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const char*
GetAutoEvPtrTypeKindString (EAutoEvPtrType PtrTypeKind)
{
	static const char* StringTable [EAutoEvPtrType__Count] = 
	{
		"closure",  // EAutoEvPtrType_Normal = 0,
		"weak",     // EAutoEvPtrType_Weak,
		"thin",     // EAutoEvPtrType_Thin,
		"unsafe",   // EAutoEvPtrType_Unsafe,
	};

	return (size_t) PtrTypeKind < EAutoEvPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		"undefined-autoev-ptr-kind";
}

//.............................................................................

CAutoEvPtrType::CAutoEvPtrType ()
{
	m_TypeKind = EType_AutoEvPtr;
	m_PtrTypeKind = EAutoEvPtrType_Normal;
	m_Size = sizeof (TAutoEvPtr);
	m_pTargetType = NULL;
}

CStructType* 
CAutoEvPtrType::GetAutoEvPtrStructType ()
{
	return m_pModule->m_TypeMgr.GetAutoEvPtrStructType (m_pTargetType);
}

CStructType* 
CAutoEvPtrType::GetAutoEvPtrStructType_w ()
{
	return m_pModule->m_TypeMgr.GetAutoEvPtrStructType_w (m_pTargetType);
}

rtl::CString
CAutoEvPtrType::CreateSignature (
	CAutoEvType* pAutoEvType,
	EType TypeKind,
	EAutoEvPtrType PtrTypeKind,
	uint_t Flags
	)
{
	rtl::CString Signature = TypeKind == EType_AutoEvRef ? 'R' : 'P';

	switch (PtrTypeKind)
	{
	case EAutoEvPtrType_Thin:
		Signature += 't';
		break;

	case EAutoEvPtrType_Weak:
		Signature += 'w';
		break;

	case EAutoEvPtrType_Unsafe:
		Signature += 'u';
		break;
	}

	if (Flags & EPtrTypeFlag_Nullable)
		Signature += 'n';

	Signature += pAutoEvType->GetSignature ();
	return Signature;
}

void
CAutoEvPtrType::PrepareTypeString ()
{
	if (m_Flags & EPtrTypeFlag_Nullable)
		m_TypeString += "nullable ";

	if (m_PtrTypeKind != EAutoEvPtrType_Normal)
	{
		m_TypeString += GetAutoEvPtrTypeKindString (m_PtrTypeKind);
		m_TypeString += ' ';
	}

	m_TypeString += m_TypeKind == EType_AutoEvRef ? "autoev& " : "autoev* ";
	m_TypeString += m_pTargetType->GetStarterType ()->GetArgString ();
}

void
CAutoEvPtrType::PrepareLlvmType ()
{
	switch (m_PtrTypeKind)
	{
	case EAutoEvPtrType_Normal:
		m_pLlvmType = GetAutoEvPtrStructType ()->GetLlvmType ();
		break;

	case EAutoEvPtrType_Weak:
		m_pLlvmType = GetAutoEvPtrStructType_w ()->GetLlvmType ();
		break;

	case EAutoEvPtrType_Thin:
	case EAutoEvPtrType_Unsafe:
		m_pLlvmType = m_pTargetType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe)->GetLlvmType ();
		break;

	default:
		ASSERT (false);
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
