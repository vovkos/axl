#include "stdafx.h"
#include "axl_jnc_NamespaceMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CNamespaceMgr::CNamespaceMgr ()
{
	m_pModule = NULL;
	m_pCurrentNamespace = &m_GlobalNamespace;
	m_pCurrentScope = NULL;
}

void
CNamespaceMgr::Clear ()
{
	m_GlobalNamespace.Clear ();
	m_NamespaceList.Clear ();
	m_ScopeList.Clear ();
	m_NamespaceStack.Clear ();
	m_pCurrentNamespace = &m_GlobalNamespace;
	m_pCurrentScope = NULL;
}

CGlobalNamespace*
CNamespaceMgr::CreateNamespace (const rtl::CString& Name)
{
	CGlobalNamespace* pNamespace = AXL_MEM_NEW (CGlobalNamespace);
	m_NamespaceList.InsertTail (pNamespace);
	pNamespace->m_Name = Name;
	return pNamespace;
}

CGlobalNamespace*
CNamespaceMgr::OpenNamespace (
	const CToken::CPos& Pos,
	const CQualifiedName& Name
	)
{
	ASSERT (!m_pCurrentScope);

	CGlobalNamespace* pNamespace = OpenNamespace (Pos, Name.m_First);
	if (!pNamespace)
		return NULL;

	rtl::CBoxIteratorT <rtl::CString> It = Name.m_List.GetHead ();
	for (; It; It++)
	{
		pNamespace = OpenNamespace (Pos, *It);
		if (!pNamespace)
			return NULL;
	}
	
	ASSERT (m_pCurrentNamespace == pNamespace);
	return pNamespace;
}

CGlobalNamespace*
CNamespaceMgr::OpenNamespace (
	const CToken::CPos& Pos,
	const rtl::CString& Name
	)
{
	ASSERT (!m_pCurrentScope);

	CGlobalNamespace* pNamespace;

	CModuleItem* pItem = m_pCurrentNamespace->FindItem (Name);
	if (!pItem)
	{
		pNamespace = CreateNamespace (Name);
		pNamespace->m_Pos = Pos;
		m_pCurrentNamespace->AddItem (pNamespace);
	}
	else
	{
		if (pItem->GetItemKind () != EModuleItem_Namespace)
		{
			err::SetFormatStringError (_T("'%s' exists and is not a namespace"), m_pCurrentNamespace->CreateQualifiedName (Name));
			return NULL;
		}

		pNamespace = (CGlobalNamespace*) pItem;
	}

	m_NamespaceStack.Append (pNamespace);
	m_pCurrentNamespace = pNamespace;
	return pNamespace;
}

void
CNamespaceMgr::OpenNamespace (CNamespace* pNamespace)
{
	ASSERT (!m_pCurrentScope);

	if (!pNamespace->m_pParentNamespace)
		pNamespace->m_pParentNamespace = m_pCurrentNamespace;
	else
		ASSERT (pNamespace->m_pParentNamespace == m_pCurrentNamespace);

	m_NamespaceStack.Append (pNamespace);
	m_pCurrentNamespace = pNamespace;
}

void
CNamespaceMgr::CloseNamespace (size_t Count)
{
	ASSERT (!m_pCurrentScope);

	size_t StackCount = m_NamespaceStack.GetCount ();

	if (Count < StackCount)
		StackCount -= Count;
	else
		StackCount =  0;

	m_NamespaceStack.SetCount (StackCount);

	m_pCurrentNamespace = StackCount ? m_NamespaceStack [StackCount - 1] : &m_GlobalNamespace;
}

CScope*
CNamespaceMgr::OpenScope (const CToken::CPos& Pos)
{
	CScope* pScope = AXL_MEM_NEW (CScope);
	pScope->m_Pos = Pos;
	m_ScopeList.InsertTail (pScope);
	m_NamespaceStack.Append (pScope);
	m_pCurrentNamespace = pScope;
	m_pCurrentScope = pScope;
	return pScope;
}

void
CNamespaceMgr::CloseScope (const CToken::CPos& Pos)
{
	if (!m_pCurrentScope)
		return;

	ASSERT (
		m_pCurrentScope == m_pCurrentNamespace &&
		m_pCurrentScope == m_NamespaceStack.GetBack ()
		);

	m_pCurrentScope->m_PosEnd = Pos;

	size_t StackCount = m_NamespaceStack.GetCount ();
	ASSERT (StackCount);

	StackCount--;

	m_NamespaceStack.SetCount (StackCount);
	m_pCurrentNamespace = StackCount ? m_NamespaceStack [StackCount - 1] : &m_GlobalNamespace;
	m_pCurrentScope = m_pCurrentNamespace->GetNamespaceKind () == ENamespace_Scope ? (CScope*) m_pCurrentNamespace : NULL;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
