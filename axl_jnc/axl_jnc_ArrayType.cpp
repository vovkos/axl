#include "stdafx.h"
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
	m_TypeString.Format (_T("%s [%d]"), m_pRootType->GetTypeString (), m_ElementCount);
	
	CType* pElementType = m_pElementType;
	while (pElementType->GetTypeKind () == EType_Array)
	{
		CArrayType* pArrayType = (CArrayType*) pElementType;
		m_TypeString.AppendFormat (_T(" [%d]"), pArrayType->m_ElementCount);
		pElementType = pArrayType->m_pElementType;
	}
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

