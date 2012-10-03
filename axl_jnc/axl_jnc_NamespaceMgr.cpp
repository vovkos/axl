#include "stdafx.h"
#include "axl_jnc_NamespaceMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

void
CNamespaceMgr::Clear ()
{
	m_GlobalNamespace.Clear ();
	m_NamespaceList.Clear ();
	m_ScopeList.Clear ();
	m_NamespaceStack.Clear ();
	m_pCurrentNamespace = &m_GlobalNamespace;
}

CGlobalNamespace*
CNamespaceMgr::CreateNamespace (const rtl::CString& Name)
{
	CGlobalNamespace* pNamespace = AXL_MEM_NEW (CGlobalNamespace);
	m_NamespaceList.InsertTail (pNamespace);
	pNamespace->m_Name = Name;
	return pNamespace;
}

CScope*
CNamespaceMgr::CreateScope ()
{
	CScope* pScope = AXL_MEM_NEW (CScope);
	m_ScopeList.InsertTail (pScope);
	return pScope;
}

CGlobalNamespace*
CNamespaceMgr::OpenNamespace (
	const CToken::CPos& Pos,
	const CQualifiedIdentifier& Identifier
	)
{
	CNamespace* pNamespace = OpenNamespace (Pos, Identifier.m_First);
	if (!pNamespace)
		return NULL;

	rtl::CBoxIteratorT <rtl::CString> It = Identifier.m_List.GetHead ();
	for (; It; It++)
	{
		pNamespace = OpenNamespace (Pos, *It);
		if (!pNamespace)
			return NULL;
	}
	
	return m_pCurrentNamespace;
}

CGlobalNamespace*
CNamespaceMgr::OpenNamespace (
	const CToken::CPos& Pos,
	const rtl::CString& Identifier
	)
{
	CGlobalNamespace* pNamespace;

	CModuleItem* pItem = m_pCurrentNamespace->FindItem (Identifier, false);
	if (!pItem)
	{
		pNamespace = CreateNamespace (Identifier);
		pNamespace->m_Pos = Pos;
		m_pCurrentNamespace->AddItem (pNamespace);
	}
	else
	{
		if (pItem->GetItemKind () != EModuleItem_Namespace)
		{
			err::SetFormatStringError (_T("'%s' exists and is not a namespace"), m_pCurrentNamespace->CreateQualifiedName (Identifier));
			return NULL;
		}

		pNamespace = (CGlobalNamespace*) pItem;
	}

	m_NamespaceStack.Append (pNamespace);
	m_pCurrentNamespace = pNamespace;
	return pNamespace;
}

void
CNamespaceMgr::CloseNamespace (size_t Count)
{
	size_t StackCount = m_NamespaceStack.GetCount ();

	if (Count < StackCount)
		StackCount -= Count;
	else
		StackCount =  0;

	m_NamespaceStack.SetCount (StackCount);

	m_pCurrentNamespace = StackCount ? m_NamespaceStack [StackCount - 1] : &m_GlobalNamespace;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
