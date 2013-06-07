#include "pch.h"
#include "axl_jnc_ClassPtrType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

const char*
GetClassPtrTypeKindString (EClassPtrType PtrTypeKind)
{
	static const char* StringTable [EClassPtrType__Count] = 
	{
		"strong", // EClassPtrType_Normal = 0,
		"weak",   // EClassPtrType_Weak,
		"unsafe", // EClassPtrType_Unsafe,
	};

	return (size_t) PtrTypeKind < EClassPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		"undefined-class-ptr-kind";
}

uint_t
PromoteClassPtrTypeModifiers (uint_t Modifiers)
{
	uint_t PromotedModifiers = 0;

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

bool
PromoteClassPtrTypeModifiers (uint_t* pModifiers)
{
	uint_t Modifiers = *pModifiers;
	if (!(Modifiers & ETypeModifierMask_ClassPtr))
		return true;

	uint_t PromotedModifiers = PromoteClassPtrTypeModifiers (Modifiers);
	if (Modifiers & PromotedModifiers)
	{
		err::SetFormatStringError ("type modifier '%s' used more than once", GetTypeModifierString (Modifiers & PromotedModifiers));
		return false;
	}

	*pModifiers |= PromotedModifiers;
	*pModifiers &= ~ETypeModifierMask_ClassPtr;
	return true;
}

uint_t
DemoteClassPtrTypeModifiers (uint_t Modifiers)
{
	uint_t DemotedModifiers = 0;

	if (Modifiers & ETypeModifier_Nullable_p)
		DemotedModifiers |= ETypeModifier_Nullable;

	if (Modifiers & ETypeModifier_Const_p)
		DemotedModifiers |= ETypeModifier_Const;

	if (Modifiers & ETypeModifier_Weak_p)
		DemotedModifiers |= ETypeModifier_Weak;

	if (Modifiers & ETypeModifier_Unsafe_p)
		DemotedModifiers |= ETypeModifier_Unsafe;

	return DemotedModifiers;
}

bool
DemoteClassPtrTypeModifiers (uint_t* pModifiers)
{
	uint_t Modifiers = *pModifiers;
	if (!(Modifiers & ETypeModifierMask_ClassPtr_p))
		return true;

	uint_t DemotedModifiers = DemoteClassPtrTypeModifiers (Modifiers);
	if (Modifiers & DemotedModifiers)
	{
		err::SetFormatStringError ("type modifier '%s' used more than once", GetTypeModifierString (Modifiers & DemotedModifiers));
		return false;
	}

	*pModifiers |= DemotedModifiers;
	*pModifiers &= ~ETypeModifierMask_ClassPtr_p;
	return true;
}

//.............................................................................

CClassPtrType::CClassPtrType ()
{
	m_TypeKind = EType_ClassPtr;
	m_PtrTypeKind = EClassPtrType_Normal;
	m_pTargetType = NULL;
	m_Size = sizeof (TInterface*);
}

rtl::CString
CClassPtrType::CreateSignature (
	CClassType* pClassType,
	EClassPtrType PtrTypeKind,
	uint_t Flags
	)
{
	rtl::CString Signature = 'P';

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
		m_TypeString += " const";

	if (m_Flags & EPtrTypeFlag_Nullable)
		m_TypeString += " nullable";

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += ' ';
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
