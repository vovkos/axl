#include "stdafx.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

rtl::CString
CQualifiedName::GetFullName () const
{
	if (m_List.IsEmpty ())
		return m_First;

	rtl::CString Name;	
	rtl::CBoxIteratorT <rtl::CString> It = m_List.GetHead ();
	for (; It; It++)
	{
		Name.Append (*It);
		Name.Append ('.');
	}

	Name.Append (m_First);
	return Name;
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
CNamespace::FindItemTraverse (const CQualifiedName& Name)
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It;

	// first find the root of qualified name

	CNamespace* pNamespace = this;
	CModuleItem* pItem = NULL;

	It = pNamespace->m_ItemMap.Find (Name.m_First); 
	if (It)
	{
		pNamespace = this;
		pItem = It->m_Value;
	}
	else
	{
		pNamespace = m_pParentNamespace;
		while (pNamespace)
		{
			It = pNamespace->m_ItemMap.Find (Name.m_First); 
			if (It)
			{
				pItem = It->m_Value;
				break;
			}

			pNamespace = pNamespace->m_pParentNamespace;
		}
	}

	if (!pItem)
		return NULL;

	// now walk the qualified name

	rtl::CBoxIteratorT <rtl::CString> NameIt = Name.m_List.GetHead ();
	for (; NameIt; NameIt++)
	{
		pNamespace = GetItemNamespace (pItem);
		if (!pNamespace)
			return NULL;

		It = pNamespace->m_ItemMap.Find (Name.m_First); 
		if (!It)
			return NULL;

		pItem = It->m_Value;
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
