#include "stdafx.h"
#include "axl_jnc_AutoEvPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetAutoEvPtrTypeKindString (EAutoEvPtrType PtrTypeKind)
{
	static const tchar_t* StringTable [EAutoEvPtrType__Count] = 
	{
		_T("closure"),  // EAutoEvPtrType_Normal = 0,
		_T("weak"),     // EAutoEvPtrType_Weak,
		_T("thin"),     // EAutoEvPtrType_Thin,
		_T("unsafe"),   // EAutoEvPtrType_Unsafe,
	};

	return (size_t) PtrTypeKind < EAutoEvPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		_T("undefined-autoev-ptr-kind");
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

rtl::CStringA
CAutoEvPtrType::CreateSignature (
	CAutoEvType* pAutoEvType,
	EType TypeKind,
	EAutoEvPtrType PtrTypeKind,
	int Flags
	)
{
	rtl::CStringA Signature = TypeKind == EType_AutoEvRef ? 'R' : 'P';

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
		m_TypeString += _T("nullable ");

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += GetAutoEvPtrTypeKindString (m_PtrTypeKind);
		m_TypeString += _T(' ');
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
