#include "stdafx.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

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
	case EType_Struct:
	case EType_Union:
		return ((CStructType*) pType);

	case EType_Interface:
	case EType_Class:
		return ((CClassType*) pType);

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

CImport*
CImport::Goto (const CQualifiedName& Name)
{
	CImport* pImport;

	// the root of qualified name

	rtl::CHashTableMapIteratorT <const tchar_t*, CImport*> It = m_ImportMap.Goto (Name.m_First);
	if (It->m_Value)
	{
		pImport = It->m_Value;
	}
	else
	{
		pImport = AXL_MEM_NEW (CImport);
		pImport->m_Name = Name.m_First;
		m_ImportList.InsertTail (pImport);
		It->m_Value = pImport;
	}

	// now walk the qualified name

	rtl::CBoxIteratorT <rtl::CString> NameIt = Name.m_List.GetHead ();
	for (; NameIt; NameIt++)
	{
		It = m_ImportMap.Goto (*NameIt);
		if (It->m_Value)
		{
			pImport = It->m_Value;
		}
		else
		{
			pImport = AXL_MEM_NEW (CImport);
			pImport->m_Name = *NameIt;
			m_ImportList.InsertTail (pImport);
			It->m_Value = pImport;
		}
	}

	return pImport;
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
	ASSERT (IsNamed ());

	rtl::CString QualifiedName = GetQualifiedName ();
	QualifiedName.Append (_T('.'));
	QualifiedName.Append (pName);
	return QualifiedName;
}

CModuleItem*
CNamespace::FindItem (const tchar_t* pName)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Find (pName); 
	return It ? It->m_Value : NULL;
}

CModuleItem*
CNamespace::FindItemTraverse (const CQualifiedName& Name)
{
	// first find the root of qualified name

	CNamespace* pNamespace = this;
	CModuleItem* pItem = NULL;

	while (pNamespace)
	{
		rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Find (Name.m_First); 
		if (It)
		{
			pItem = It->m_Value;
			break;
		}

		pNamespace = pNamespace->m_pParentNamespace;
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

		rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = pNamespace->m_ItemMap.Find (Name.m_First); 
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

	rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Goto (pName->m_Name);
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("redefinition of '%s'"), pName->m_Name);
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
	ASSERT (pType->GetTypeKind () == EType_EnumC);

	rtl::CIteratorT <CEnumMember> Member = pType->GetFirstMember ();
	for (; Member; Member++)
	{
		CAlias* pAlias = CreateAlias (Member->GetName (), *Member);
		if (!pAlias)
			return false;
	}

	size_t Count = pType->GetBaseTypeCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CEnumType* pBaseType = pType->GetBaseType (i);
		if (pBaseType->GetTypeKind () == EType_Enum) 
			ExposeEnumMembers (pBaseType);
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
