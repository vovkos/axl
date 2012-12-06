#include "stdafx.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_ClassType.h"

namespace axl {
namespace jnc {

//.............................................................................

rtl::CString
CQualifiedName::GetFullName () const
{
	if (m_List.IsEmpty ())
		return m_First;

	rtl::CString Name = m_First;	
	rtl::CBoxIteratorT <rtl::CString> It = m_List.GetHead ();
	for (; It; It++)
	{
		Name.Append ('.');
		Name.Append (*It);
	}

	return Name;
}

void
CQualifiedName::Copy (const CQualifiedName& Name)
{
	m_First = Name.m_First;
	m_List.Clear ();

	rtl::CBoxIteratorT <rtl::CString> It = Name.m_List.GetHead ();
	for (; It; It++)
		m_List.InsertTail (*It);
}

void
CQualifiedName::TakeOver (CQualifiedName* pName)
{
	m_First = pName->m_First;
	m_List.TakeOver (&pName->m_List);
	pName->Clear ();
}

//.............................................................................

CModuleItem*
UnAliasItem (CModuleItem* pItem)
{
	while (pItem->GetItemKind () == EModuleItem_Alias)
	{
		CAlias* pAlias = (CAlias*) pItem;
		pItem = pAlias->GetTarget ();
	}

	return pItem;
}

//. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

CNamespace*
GetItemNamespace (CModuleItem* pItem)
{
	pItem = UnAliasItem (pItem);

	EModuleItem ItemKind = pItem->GetItemKind ();
	if (ItemKind == EModuleItem_Namespace)
		return (CGlobalNamespace*) pItem;

	if (ItemKind != EModuleItem_Type)
		return NULL;

	CType* pType = (CType*) pItem;	
	EType TypeKind = pType->GetTypeKind ();

	switch (TypeKind)
	{
	case EType_Enum:
	case EType_Enum_c:
	case EType_Struct:
	case EType_Union:
	case EType_Interface:
	case EType_Class:
		return ((CNamedType*) pType);

	default:
		return NULL;
	}
}

//.............................................................................

rtl::CString 
CName::GetQualifiedName ()
{
	if (!m_QualifiedName.IsEmpty ())
		return m_QualifiedName;

	if (m_pParentNamespace && m_pParentNamespace->IsNamed ())
		m_QualifiedName = m_pParentNamespace->CreateQualifiedName (m_Name);
	else
		m_QualifiedName = m_Name;

	return m_QualifiedName;
}

//.............................................................................

void
CNamespace::Clear ()
{
	m_ItemArray.Clear (); 
	m_ItemMap.Clear (); 
	m_AliasList.Clear ();
}

rtl::CString
CNamespace::CreateQualifiedName (const tchar_t* pName)
{
	if (!IsNamed ())
		return pName;

	rtl::CString QualifiedName = GetQualifiedName ();
	QualifiedName.Append (_T('.'));
	QualifiedName.Append (pName);
	return QualifiedName;
}

CModuleItem*
CNamespace::FindItem (const tchar_t* pName)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Find (pName); 
	return It ? It->m_Value : NULL;
}

CModuleItem*
CNamespace::FindItemTraverse (const tchar_t* pName)
{
	for (CNamespace* pNamespace = this; pNamespace; pNamespace = pNamespace->m_pParentNamespace)
	{
		CModuleItem* pItem;

		switch (pNamespace->m_NamespaceKind)
		{
		case ENamespace_Struct:
			pItem = ((CStructType*) pNamespace)->FindItemWithBaseTypeList (pName);
			break;

		case ENamespace_Class:
			pItem = ((CClassType*) pNamespace)->FindItemWithBaseTypeList (pName);
			break;

		default:
			pItem = pNamespace->FindItem (pName);
		}
			
		if (pItem)
			return pItem;
	}

	return NULL;
}

CModuleItem*
CNamespace::FindItemTraverse (
	const CQualifiedName& Name,
	bool IsStrict
	)
{
	CModuleItem* pItem = FindItemTraverse (Name.m_First);
	if (!pItem)
		return NULL;

	rtl::CBoxIteratorT <rtl::CString> NameIt = Name.m_List.GetHead ();

	if (IsStrict) // direct members only
		for (; NameIt; NameIt++)
		{
			CNamespace* pNamespace = GetItemNamespace (pItem);
			if (!pNamespace)
				return NULL;

			pItem = pNamespace->FindItem (*NameIt);
			if (!pItem)
				return NULL;
		}
	else
		for (; NameIt; NameIt++)
		{
			CNamespace* pNamespace = GetItemNamespace (pItem);
			if (!pNamespace)
				return NULL;

			pItem = pNamespace->FindItemTraverse (*NameIt);
			if (!pItem)
				return NULL;
		}


	return pItem;
}

bool
CNamespace::AddItem (
	CModuleItem* pItem,
	CName* pName
	)	
{
	ASSERT (pName->m_pParentNamespace == NULL);

	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Goto (pName->m_Name);
	if (It->m_Value)
	{
		SetRedefinitionError (pName->m_Name);
		return false;
	}

	pName->m_pParentNamespace = this;
	m_ItemArray.Append (pItem);
	It->m_Value = pItem;
	return true;
}

bool
CNamespace::ExposeEnumMembers (CEnumType* pType)
{
	rtl::CIteratorT <CEnumMember> Member = pType->GetFirstMember ();
	for (; Member; Member++)
	{
		CAlias* pAlias = CreateAlias (Member->GetName (), *Member);
		if (!pAlias)
			return false;
	}

	return true;	
}

CAlias*
CNamespace::CreateAlias (
	const rtl::CString& Name,
	CModuleItem* pTarget
	)
{
	CAlias* pAlias = AXL_MEM_NEW (CAlias);
	pAlias->m_Name = Name;
	pAlias->m_pTarget = pTarget;
	m_AliasList.InsertTail (pAlias);
	bool Result = AddItem (pAlias);
	if (!Result)
		return NULL;

	return pAlias;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
