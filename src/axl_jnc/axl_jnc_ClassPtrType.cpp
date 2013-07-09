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
	};

	return (size_t) PtrTypeKind < EClassPtrType__Count ? 
		StringTable [PtrTypeKind] : 
		"undefined-class-ptr-kind";
}

//.............................................................................

CClassPtrType::CClassPtrType ()
{
	m_TypeKind = EType_ClassPtr;
	m_PtrTypeKind = EClassPtrType_Normal;
	m_pTargetType = NULL;
	m_Size = sizeof (void*);
	m_AlignFactor = sizeof (void*);
}

rtl::CString
CClassPtrType::CreateSignature (
	CClassType* pClassType,
	EType TypeKind,
	EClassPtrType PtrTypeKind,
	uint_t Flags
	)
{
	rtl::CString Signature = TypeKind == EType_ClassRef ? "RC" : "PC";

	if (PtrTypeKind == EClassPtrType_Weak)
		Signature += 'w';

	Signature += GetPtrTypeFlagSignature (Flags);
	Signature += pClassType->GetSignature ();
	return Signature;
}

void
CClassPtrType::PrepareTypeString ()
{
	m_TypeString += m_pTargetType->GetTypeString ();

	if (m_Flags & EPtrTypeFlag__AllMask)
	{
		m_TypeString += ' ';
		m_TypeString += GetPtrTypeFlagString (m_Flags);
	}

	if (m_PtrTypeKind != EClassPtrType_Normal)
	{
		m_TypeString += ' ';
		m_TypeString += GetClassPtrTypeKindString (m_PtrTypeKind);
	}

	m_TypeString += m_TypeKind == EType_ClassRef ? "&" : "*";
}

void
CClassPtrType::PrepareLlvmType ()
{
	m_pLlvmType = llvm::PointerType::get (m_pTargetType->GetIfaceStructType ()->GetLlvmType (), 0);
}

void
CClassPtrType::EnumGcRoots (
	CGcHeap* pGcHeap,
	void* p
	)
{
	TInterface* pIface = *(TInterface**) p;
	if (!pGcHeap->ShouldMark (pIface))
		return;

	TObject* pObject = pIface->m_pObject;
	
	if (m_PtrTypeKind == EClassPtrType_Normal)
		pGcHeap->MarkValue (pObject, pObject->m_pType);
	else
		pGcHeap->MarkRange (pObject, pObject->m_pType->GetSize ()); // actually can only keep headers. but whatever
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
