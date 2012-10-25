#include "stdafx.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

bool
CStructClassType::AddBaseType (CType* pType)
{
	if (m_BaseTypeArray.Find (pType) != -1)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return false;
	}

	m_BaseTypeArray.Append (pType);
	return true;
}

bool
CStructClassType::AddGenericArgument (CImportType* pType)
{
	if (m_GenericArgumentArray.Find (pType) != -1)
	{
		err::SetFormatStringError (_T("multiple generic argument names '%s'"), pType->GetTypeString ());
		return false;
	}

	m_GenericArgumentArray.Append (pType);
	return true;
}

//.............................................................................

CStructMember*
CStructType::FindMember (
	const tchar_t* pName,
	bool Traverse
	)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Find (pName); 
	if (It)
		return (CStructMember*) It->m_Value;

	if (!Traverse)
		return NULL;

	size_t Count = m_BaseTypeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CType* pBaseType = m_BaseTypeArray [i];
		EType TypeKind = pBaseType->GetTypeKind ();

		CStructMember* pMember = NULL;

		switch (TypeKind)
		{
		case EType_Struct:
		case EType_Union:
			pMember = ((CStructType*) pBaseType)->FindMember (pName, true);
			break;
		}

		if (pMember)
			return pMember;
	}

	return NULL;
}

CStructMember*
CStructType::CreateMember (
	const rtl::CString& Name,
	CType* pType
	)
{
	CStructMember* pMember = AXL_MEM_NEW (CStructMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_Offset = 0; // TODO: calc
	m_MemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

llvm::StructType* 
CStructType::GetLlvmType ()
{
	if (m_Flags & ETypeFlag_IsLlvmReady)
		return (llvm::StructType*) m_pLlvmType;

	llvm::StructType* pLlvmType = NULL;
	
	m_pLlvmType = pLlvmType;
	m_Flags |= ETypeFlag_IsLlvmReady;
	return pLlvmType;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {

