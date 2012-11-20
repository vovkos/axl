#include "stdafx.h"
#include "axl_jnc_ClassType.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CClassBaseType*
CClassType::AddBaseType (CClassType* pType)
{
	size_t AlignFactor = pType->GetAlignFactor ();
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	size_t FieldOffset = GetFieldOffset (AlignFactor);
	SetSize (FieldOffset + pType->GetSize ());

	size_t MethodTableIndex = m_MethodTable.GetCount ();
	m_MethodTable.SetCount (MethodTableIndex + pType->GetMethodCount ());

	CClassBaseType* pBaseType = CreateBaseTypeImpl (pType, FieldOffset, MethodTableIndex);
	
	rtl::CIteratorT <CClassBaseType> BaseType = pType->GetFirstBaseType ();
	for (; BaseType; BaseType++)
	{
		CClassBaseType* pSubBaseType = CreateBaseTypeImpl (
			BaseType->m_pType, 
			FieldOffset + BaseType->m_FieldOffset,
			MethodTableIndex + BaseType->m_MethodTableIndex
			);

		if (!pSubBaseType)
			return NULL;
	}

	rtl::CIteratorT <CClassFieldMember> FieldMember = pType->GetFirstFieldMember ();
	for (; FieldMember; FieldMember++) 
	{
		CClassFieldMember* pFieldMember = CreateFieldMemberImpl (
			FieldMember->m_Name,
			FieldMember->m_pType,
			FieldOffset + FieldMember->m_Offset 
			);

		if (!pFieldMember)
			return NULL;
	}

	rtl::CIteratorT <CClassMethodMember> MethodMember = pType->GetFirstMethodMember ();
	for (; MethodMember; MethodMember++) 
	{
		rtl::CIteratorT <CClassMethod> Method = MethodMember->GetFirstOverload ();
		ASSERT (Method);

		CClassMethod* pMethod = CreateMethodImpl (
			MethodMember->m_Name, 
			Method->m_pFunction, 
			MethodTableIndex + Method->m_MethodTableIndex
			);

		if (!pMethod)
			return NULL;

		CClassMethodMember* pMethodMember = pMethod->GetMethodMember ();

		for (Method++; Method; Method++)
		{
			pMethod = CreateMethodImpl (
				pMethodMember,
				Method->m_pFunction,
				MethodTableIndex + Method->m_MethodTableIndex
				);
			
			if (!pMethod)
				return NULL;
		}
	}

	rtl::CIteratorT <CClassPropertyMember> PropertyMember = pType->GetFirstPropertyMember ();
	for (; PropertyMember; PropertyMember++) 
	{
		CClassPropertyMember* pPropertyMember = CreatePropertyMemberImpl (
			PropertyMember->m_Name,
			PropertyMember->m_pProperty,
			MethodTableIndex + PropertyMember->m_MethodTableIndex 
			);

		if (!pPropertyMember)
			return NULL;
	}

	return pBaseType;
}

CClassBaseType*
CClassType::CreateBaseTypeImpl (
	CClassType* pType,
	size_t FieldOffset,
	size_t MethodTableIndex
	)
{
	rtl::CStringHashTableMapIteratorAT <CClassBaseType*> It = m_BaseTypeMap.Goto (pType->GetSignature ());
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("'%s' is already a base type"), pType->GetTypeString ());
		return NULL;
	}

	CClassBaseType* pBaseType = AXL_MEM_NEW (CClassBaseType);
	pBaseType->m_pType = pType;
	pBaseType->m_FieldOffset = FieldOffset;
	pBaseType->m_MethodTableIndex = MethodTableIndex;
	m_BaseTypeList.InsertTail (pBaseType);
	It->m_Value = pBaseType;

	return pBaseType;
}

CClassFieldMember*
CClassType::CreateFieldMember (
	const rtl::CString& Name,
	CType* pType
	)
{
	size_t AlignFactor = pType->GetAlignFactor ();
	if (AlignFactor > m_AlignFactor)
		m_AlignFactor = AlignFactor;

	size_t Offset = GetFieldOffset (AlignFactor);
	CClassFieldMember* pMember = CreateFieldMemberImpl (Name, pType, Offset);
	if (!pMember)
		return NULL;

	SetSize (Offset + pType->GetSize ());
	return pMember;
}

CClassMethod*
CClassType::CreateMethod (
	const rtl::CString& Name,
	CFunction* pFunction
	)
{
	CClassMethodMember* pMethodMember = NULL;

	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Goto (Name);
	if (!It->m_Value)
	{
		pMethodMember = AXL_MEM_NEW (CClassMethodMember);
		pMethodMember->m_Name = Name;
		m_MethodMemberList.InsertTail (pMethodMember);
	}
	else
	{
		CModuleItem* pItem = It->m_Value;
		if (pItem->GetItemKind () == EModuleItem_ClassMember)
		{
			CClassMember* pMember = (CClassMember*) pItem;
			if (pMember->GetMemberKind () == EClassMember_Method)
				pMethodMember = (CClassMethodMember*) pMember;
		}

		if (!pMethodMember)
		{
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return NULL;
		}
	}

	ASSERT (pMethodMember);

	size_t MethodTableIndex = m_MethodTable.GetCount ();
	m_MethodTable.SetCount (MethodTableIndex + 1);

	return CreateMethodImpl (pMethodMember, pFunction, MethodTableIndex);
}

CClassPropertyMember*
CClassType::CreatePropertyMember (
	const rtl::CString& Name,
	CProperty* pProperty
	)
{
	return NULL;
}

CClassFieldMember*
CClassType::CreateFieldMemberImpl (
	const rtl::CString& Name,
	CType* pType,
	size_t Offset
	)
{
	CClassFieldMember* pMember = AXL_MEM_NEW (CClassFieldMember);
	pMember->m_Name = Name;
	pMember->m_pType = pType;
	pMember->m_Offset = Offset;
	m_FieldMemberList.InsertTail (pMember);

	bool Result = AddItem (pMember);
	if (!Result)
		return NULL;

	return pMember;
}

CClassMethod*
CClassType::CreateMethodImpl (
	const rtl::CString& Name,
	CFunction* pFunction,
	size_t MethodTableIndex
	)
{
	CClassMethodMember* pMember;

	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Goto (Name);
	if (It->m_Value)
	{
		CModuleItem* pItem = It->m_Value;
		if (pItem->GetItemKind () != EModuleItem_ClassMember ||
			((CClassMember*) pItem)->GetMemberKind () != EClassMember_Method)
		{
			SetRedefinitionError (Name);
			return NULL;
		}

		pMember = (CClassMethodMember*) pItem;
	}
	else
	{
		pMember = AXL_MEM_NEW (CClassMethodMember);
		pMember->m_Name = Name;
		
		bool Result = AddItem (pMember);
		if (!Result)
			return NULL;
	}

	return CreateMethodImpl (pMember, pFunction, MethodTableIndex);
}

CClassMethod*
CClassType::CreateMethodImpl (
	CClassMethodMember* pMember,
	CFunction* pFunction,
	size_t MethodTableIndex
	)
{
	CClassMethod* pMethod = AXL_MEM_NEW (CClassMethod);
	pMethod->m_pMethodMember = pMember;
	pMethod->m_pFunction = pFunction;
	pMethod->m_MethodTableIndex = MethodTableIndex;
	pMember->m_OverloadList.InsertTail (pMethod);
	return pMethod;
}


CClassPropertyMember*
CClassType::CreatePropertyMemberImpl (
	const rtl::CString& Name,
	CProperty* pProperty,
	size_t MethodTableIndex
	)
{
	CClassPropertyMember* pMember = AXL_MEM_NEW (CClassPropertyMember);
	pMember->m_Name = Name;
	pMember->m_pProperty = pProperty;
	pMember->m_MethodTableIndex = MethodTableIndex;
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
