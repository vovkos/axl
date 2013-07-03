#include "pch.h"
#include "axl_jnc_ArrayType.h"

namespace axl {
namespace jnc {

//.............................................................................

CArrayType::CArrayType ()
{
	m_TypeKind = EType_Array;
	m_pElementType = NULL;
	m_pElementType_i = NULL;
	m_pRootType = NULL;
	m_ElementCount = 0;
}

void
CArrayType::PrepareTypeString ()
{
	rtl::CString String;
	m_TypeString.Format (
		"%s [%d]", 
		m_pRootType->GetTypeString ().cc (), // thanks a lot gcc
		m_ElementCount
		);
	
	CType* pElementType = m_pElementType;
	while (pElementType->GetTypeKind () == EType_Array)
	{
		CArrayType* pArrayType = (CArrayType*) pElementType;
		m_TypeString.AppendFormat (" [%d]", pArrayType->m_ElementCount);
		pElementType = pArrayType->m_pElementType;
	}
}

bool
CArrayType::CalcLayout ()
{
	if (m_pElementType_i)
		m_pElementType = m_pElementType_i->GetActualType ();

	bool Result = m_pElementType->EnsureLayout ();
	if (!Result)
		return false;

	m_pRootType = m_pElementType->GetTypeKind () == EType_Array ? 
		((CArrayType*) m_pElementType)->GetRootType () :	
		m_pElementType;

	m_TypeString.Clear (); // ensure updated type string

	uint_t RootTypeFlags = m_pRootType->GetFlags ();
	if (RootTypeFlags & ETypeFlag_Pod)
		m_Flags |= ETypeFlag_Pod;
	else if (RootTypeFlags & ETypeFlag_GcRoot)
		m_Flags |= ETypeFlag_GcRoot;

	m_Size = m_pElementType->GetSize () * m_ElementCount;
	m_AlignFactor = m_pElementType->GetAlignFactor ();
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

