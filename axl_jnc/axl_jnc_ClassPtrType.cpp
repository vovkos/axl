#include "stdafx.h"
#include "axl_jnc_ClassPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const tchar_t*
GetClassPtrTypeKindString (EClassPtrType PtrTypeKind)
{
	static const tchar_t* StringTable [EClassPtrType__Count] = 
	{
		_T("strong"), // EClassPtrType_Normal = 0,
		_T("weak"),   // EClassPtrType_Weak,
		_T("unsafe"), // EClassPtrType_Unsafe,
	};

	return PtrTypeKind >= 0 && PtrTypeKind < EClassPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		_T("undefined-class-ptr-kind");
}

//.............................................................................

CClassPtrType::CClassPtrType ()
{
	m_TypeKind = EType_ClassPtr;
	m_PtrTypeKind = EClassPtrType_Normal;
	m_pTargetType = NULL;
	m_Size = sizeof (TInterface*);
}

rtl::CStringA
CClassPtrType::CreateSignature (
	CClassType* pClassType,
	EClassPtrType PtrTypeKind,
	int Flags
	)
{
	rtl::CStringA Signature = 'P';

	switch (PtrTypeKind)
	{
	case EClassPtrType_Weak:
		Signature += 'w';
		break;

	case EClassPtrType_Unsafe:
		Signature += 'u';
		break;
	}

	if (Flags & EPtrTypeFlag_Const)
		Signature += 'c';

	if (Flags & EPtrTypeFlag_ReadOnly)
		Signature += 'r';

	if (Flags & EPtrTypeFlag_NoNull)
		Signature += 'n';

	Signature += pClassType->GetSignature ();
	return Signature;
}

void
CClassPtrType::PrepareTypeString ()
{
	if (m_Flags & EPtrTypeFlag_Const)
		m_TypeString += _T("const ");

	if (m_Flags & EPtrTypeFlag_ReadOnly)
		m_TypeString += _T("readonly ");

	if (m_Flags & EPtrTypeFlag_NoNull)
		m_TypeString += _T("nonull ");

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += GetClassPtrTypeKindString (m_PtrTypeKind);
		m_TypeString += _T(' ');
	}

	m_TypeString += m_pTargetType->GetTypeString ();
	m_TypeString += "&";
}

void
CClassPtrType::PrepareLlvmType ()
{
	m_pLlvmType = llvm::PointerType::get (m_pTargetType->GetIfaceStructType ()->GetLlvmType (), 0);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
