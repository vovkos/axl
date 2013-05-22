#include "pch.h"
#include "axl_jnc_ArrayType.h"

namespace axl {
namespace jnc {

//.............................................................................

CArrayType::CArrayType ()
{
	m_TypeKind = EType_Array;
	m_pElementType = NULL;
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

//.............................................................................

} // namespace jnc {
} // namespace axl {

