#include "stdafx.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_StructType.h"
#include "axl_jnc_ClassType.h"

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
	switch (ItemKind)
	{
	case EModuleItem_Namespace:
		return (CGlobalNamespace*) pItem;

	case EModuleItem_Property:
		return (CProperty*) pItem;

	case EModuleItem_Type:
		break;

	default:
		return NULL;
	}

	CType* pType = (CType*) pItem;	
	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Enum:
	case EType_Struct:
	case EType_Union:
	case EType_Class:
		return ((CNamedType*) pType);

	default:
		return NULL;
	}
}

//.............................................................................

const tchar_t*
GetNamespaceKindString (ENamespace NamespaceKind)
{
	static const tchar_t* StringTable [ENamespace__Count] = 
	{
		_T("undefined-namespace-kind"),  // ENamespace_Undefined = 0,
		_T("global"),                    // ENamespace_Global,
		_T("scope"),                     // ENamespace_Scope,
		_T("named-type"),                // ENamespace_Type,
		_T("named-type-extension"),      // ENamespace_TypeExtension,
		_T("property"),                  // ENamespace_Property,
		_T("property-template"),         // ENamespace_PropertyTemplate,
	};

	return NamespaceKind >= 0 && NamespaceKind < ENamespace__Count ? 
		StringTable [NamespaceKind] : 
		StringTable [ENamespace_Undefined];
}

//.............................................................................

CNamespace::CNamespace ()
{
	m_NamespaceKind = ENamespace_Undefined;
	m_CurrentAccessKind = EAccess_Public;
	m_pParentNamespace = NULL;
}

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
	if (m_QualifiedName.IsEmpty ())
		return pName;

	rtl::CString QualifiedName = m_QualifiedName;
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
CNamespace::FindItem (const CQualifiedName& Name)
{
	CModuleItem* pItem = FindItem (Name.m_First);
	if (!pItem)
		return NULL;

	rtl::CBoxIteratorT <rtl::CString> NameIt = Name.m_List.GetHead ();
	for (; NameIt; NameIt++)
	{
		CNamespace* pNamespace = GetItemNamespace (pItem);
		if (!pNamespace)
			return NULL;

		pItem = pNamespace->FindItem (*NameIt);
		if (!pItem)
			return NULL;
	}

	return pItem;
}

CModuleItem*
CNamespace::FindItemTraverse (const tchar_t* pName)
{
	for (CNamespace* pNamespace = this; pNamespace; pNamespace = pNamespace->m_pParentNamespace)
	{
		CModuleItem* pItem;

		if (pNamespace->m_NamespaceKind == ENamespace_Type)
		{
			CNamedType* pNamedType = (CNamedType*) pNamespace;
			EType NamedTypeKind = pNamedType->GetTypeKind ();
			switch (NamedTypeKind)
			{
			case EType_Class:
				pItem = ((CClassType*) pNamedType)->FindItemWithBaseTypeList (pName);
				break;

			case EType_Struct:
				pItem = ((CStructType*) pNamedType)->FindItemWithBaseTypeList (pName);
				break;

			default:
				pItem = pNamespace->FindItem (pName);
			}
		}
		else
		{
			pItem = pNamespace->FindItem (pName);
		}
			
		if (pItem)
			return pItem;
	}

	return NULL;
}

CModuleItem*
CNamespace::FindItemTraverse (const CQualifiedName& Name)
{
	CModuleItem* pItem = FindItemTraverse (Name.m_First);
	if (!pItem)
		return NULL;

	rtl::CBoxIteratorT <rtl::CString> NameIt = Name.m_List.GetHead ();
	for (; NameIt; NameIt++)
	{
		CNamespace* pNamespace = GetItemNamespace (pItem);
		if (!pNamespace)
			return NULL;
		pItem = pNamespace->FindItem (*NameIt);
		if (!pItem)
			return NULL;
	}

	return pItem;
}

bool
CNamespace::AddItem (
	CModuleItem* pItem,
	CModuleItemName* pName
	)	
{
	rtl::CStringHashTableMapIteratorT <CModuleItem*> It = m_ItemMap.Goto (pName->m_Name);
	if (It->m_Value)
	{
		SetRedefinitionError (pName->m_Name);
		return false;
	}

	m_ItemArray.Append (pItem);
	It->m_Value = pItem;
	return true;
}

bool
CNamespace::AddFunction (CFunction* pFunction)
{
	CModuleItem* pOldItem = FindItem (pFunction->GetName ());
	if (!pOldItem)
		return AddItem (pFunction);

	if (pOldItem->GetItemKind () != EModuleItem_Function)
	{	
		SetRedefinitionError (pFunction->m_Name);
		return false;
	}

	CFunction* pFunctionOverload = (CFunction*) pOldItem;
	return pFunctionOverload->AddOverload (pFunction);
}

bool
CNamespace::ExposeEnumMembers (CEnumType* pType)
{
	rtl::CIteratorT <CEnumMember> Member = pType->GetMemberList ().GetHead ();
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

} // namespace jnc {
} // namespace axl {
