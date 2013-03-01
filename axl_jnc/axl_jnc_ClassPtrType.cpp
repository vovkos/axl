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

	return (size_t) PtrTypeKind < EClassPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		_T("undefined-class-ptr-kind");
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

int 
PromoteClassPtrTypeModifiers (int Modifiers)
{
	int PromotedModifiers = 0;

	if (Modifiers & ETypeModifier_Nullable)
		PromotedModifiers |= ETypeModifier_Nullable_p;

	if (Modifiers & ETypeModifier_Const)
		PromotedModifiers |= ETypeModifier_Const_p;

	if (Modifiers & ETypeModifier_Weak)
		PromotedModifiers |= ETypeModifier_Weak_p;

	if (Modifiers & ETypeModifier_Unsafe)
		PromotedModifiers |= ETypeModifier_Unsafe_p;

	return PromotedModifiers;
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

	if (Flags & EPtrTypeFlag_Nullable)
		Signature += 'n';

	Signature += pClassType->GetSignature ();
	return Signature;
}

void
CClassPtrType::PrepareTypeString ()
{
	m_TypeString += m_pTargetType->GetTypeString ();

	if (m_Flags & EPtrTypeFlag_Const)
		m_TypeString += _T(" const");

	if (m_Flags & EPtrTypeFlag_Nullable)
		m_TypeString += _T(" nullable");

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += _T(' ');
		m_TypeString += GetClassPtrTypeKindString (m_PtrTypeKind);
	}

	m_TypeString += " class*";
}

void
CClassPtrType::PrepareLlvmType ()
{
	m_pLlvmType = llvm::PointerType::get (m_pTargetType->GetIfaceStructType ()->GetLlvmType (), 0);
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
