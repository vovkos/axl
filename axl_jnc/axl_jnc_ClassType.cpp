#include "stdafx.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CModuleItem*
CClassType::FindMember (
	const tchar_t* pName,
	bool Traverse
	)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Find (pName); 
	if (It)
		return It->m_Value;

	if (!Traverse)
		return NULL;

	size_t Count = m_BaseTypeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CType* pBaseType = m_BaseTypeArray [i];
		EType TypeKind = pBaseType->GetTypeKind ();

		CModuleItem* pMember = NULL;

		switch (TypeKind)
		{
		case EType_Struct:
		case EType_Union:
			pMember = ((CStructType*) pBaseType)->FindMember (pName, true);
			break;

		case EType_Interface:
		case EType_Class:
			pMember = ((CClassType*) pBaseType)->FindMember (pName, true);
			break;
		}

		if (pMember)
			return pMember;
	}

	return NULL;
}

CClassFieldMember*
CClassType::CreateFieldMember (
	const rtl::CString& Name,
	CType* pType
	)
{
	if (!m_pFieldBlockType)
		m_pFieldBlockType = m_pModule->m_TypeMgr.CreateUnnamedStructType (EType_Struct, m_PackFactor);

	CClassFieldMember* pMember = AXL_MEM_NEW (CClassFieldMember);
	pMember->m_Name = Name;
	pMember->m_pField = m_pFieldBlockType->CreateMember (Name, pType);
	m_FieldMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

CClassMethodMember*
CClassType::CreateMethodMember (
	const rtl::CString& Name,
	CFunction* pFunction
	)
{
	CClassMethodMember* pMember = AXL_MEM_NEW (CClassMethodMember);
	pMember->m_Name = Name;
	pMember->m_Function.AddOverload (pFunction);
	m_MethodMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

CClassPropertyMember*
CClassType::CreatePropertyMember (
	const rtl::CString& Name,
	CProperty* pProperty
	)
{
	CClassPropertyMember* pMember = AXL_MEM_NEW (CClassPropertyMember);
	pMember->m_Name = Name;
	pMember->m_pProperty = pProperty;
	m_PropertyMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

llvm::StructType* 
CClassType::GetLlvmType ()
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
