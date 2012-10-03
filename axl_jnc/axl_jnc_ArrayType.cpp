#include "stdafx.h"
#include "axl_jnc_ArrayType.h"

namespace axl {
namespace jnc {

//.............................................................................

CType* 
CArrayType::GetRootType (CType* pBaseType)
{
	CType* pType = pBaseType;

	while (pType->GetTypeKind () == EType_Array)
	{
		CArrayType* pArrayType = (CArrayType*) pType;
		pType = pArrayType->GetBaseType ();
	}

	return pType;
}

rtl::CString
CArrayType::CreateTypeString (
	CType* pBaseType,
	size_t ElementCount
	)
{
	CType* pRootType = GetRootType(pBaseType);
	
	rtl::CString String;
	String.Format (_T("%s [%d]"), pRootType->GetTypeString (), ElementCount);
	
	while (pBaseType->GetTypeKind () == EType_Array)
	{
		CArrayType* pArrayType = (CArrayType*) pBaseType;
		String.AppendFormat (_T(" [%d]"), pArrayType->m_ElementCount);
		pBaseType = pArrayType->m_pBaseType;
	}

	return String;
}


//.............................................................................

} // namespace axl {
} // namespace jnc {
