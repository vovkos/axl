#include "stdafx.h"
#include "axl_jnc_Namespace.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

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
	ASSERT (IsNamed ());

	rtl::CString QualifiedName = GetQualifiedName ();
	QualifiedName.Append (_T('.'));
	QualifiedName.Append (pName);
	return QualifiedName;
}

CModuleItem*
CNamespace::FindItem (
	const tchar_t* pName,
	bool Traverse
	)
{
	rtl::CHashTableMapIteratorT <const tchar_t*, CModuleItem*> It = m_ItemMap.Find (pName); 
	if (It)
		return It->m_Value;

	if (!Traverse)
		return NULL;

	CNamespace* pNamespace = m_pParentNamespace;
	while (pNamespace)
	{
		It = pNamespace->m_ItemMap.Find (pName); 
		if (It)
			return It->m_Value;

		pNamespace = pNamespace->m_pParentNamespace;
	}

	return NULL;
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
