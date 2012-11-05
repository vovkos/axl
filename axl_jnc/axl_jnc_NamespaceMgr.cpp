#include "stdafx.h"
#include "axl_jnc_NamespaceMgr.h"
#include "axl_jnc_Module.h"

namespace axl {
namespace jnc {

//.............................................................................

CNamespaceMgr::CNamespaceMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_pCurrentNamespace = &m_GlobalNamespace;
	m_pCurrentScope = NULL;
}

void
CNamespaceMgr::Clear ()
{
	m_GlobalNamespace.Clear ();
	m_NamespaceList.Clear ();
	m_ScopeList.Clear ();
	m_pCurrentNamespace = &m_GlobalNamespace;
	m_pCurrentScope = NULL;
}

CGlobalNamespace*
CNamespaceMgr::CreateNamespace (const rtl::CString& Name)
{
	CGlobalNamespace* pNamespace = AXL_MEM_NEW (CGlobalNamespace);
	pNamespace->m_pModule = m_pModule;
	pNamespace->m_Name = Name;
	m_NamespaceList.InsertTail (pNamespace);
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

	m_pCurrentNamespace = pNamespace;
	return pNamespace;
}

void
CNamespaceMgr::OpenNamespace (CNamespace* pNamespace)
{
	if (!pNamespace->m_pParentNamespace)
		pNamespace->m_pParentNamespace = m_pCurrentNamespace;
	else
		ASSERT (pNamespace->m_pParentNamespace == m_pCurrentNamespace);

	m_pCurrentNamespace = pNamespace;
}

void
CNamespaceMgr::CloseNamespace (size_t Count)
{
	ASSERT (!m_pCurrentScope);

	CNamespace* pNamespace = m_pCurrentNamespace;

	for (size_t i = 0; pNamespace && i < Count; i++)
		pNamespace = pNamespace->m_pParentNamespace;

	m_pCurrentNamespace = pNamespace ? pNamespace : &m_GlobalNamespace;
}

CScope*
CNamespaceMgr::OpenScope (	
	const CToken::CPos& Pos,
	CBasicBlock* pBreakBlock,
	CBasicBlock* pContinueBlock
	)
{
	CScope* pScope = AXL_MEM_NEW (CScope);
	pScope->m_pModule = m_pModule;
	pScope->m_Pos = Pos;
	pScope->m_pBreakBlock = pBreakBlock;
	pScope->m_pContinueBlock = pContinueBlock;
	m_ScopeList.InsertTail (pScope);
	OpenNamespace (pScope);
	m_pCurrentScope = pScope;
	return pScope;
}

void
CNamespaceMgr::CloseScope (const CToken::CPos& Pos)
{
	if (!m_pCurrentScope)
		return;

	ASSERT (m_pCurrentScope == m_pCurrentNamespace);
	m_pCurrentScope->m_PosEnd = Pos;

	m_pCurrentNamespace = m_pCurrentScope->m_pParentNamespace;
	ASSERT (m_pCurrentNamespace);

	m_pCurrentScope = m_pCurrentNamespace->GetNamespaceKind () == ENamespace_Scope ? (CScope*) m_pCurrentNamespace : NULL;
}

CScope*
CNamespaceMgr::FindBreakScope (size_t Level)
{
	size_t i = 0;
	CScope* pScope = m_pCurrentScope;
	for (; pScope; pScope = pScope->GetParentScope ())
	{
		if (pScope->GetBreakBlock ())
		{
			i++;
			if (i >= Level)
				break;
		}
	}

	return pScope;
}

CScope*
CNamespaceMgr::FindContinueScope (size_t Level)
{
	size_t i = 0;
	CScope* pScope = m_pCurrentScope;
	for (; pScope; pScope = pScope->GetParentScope ())
	{
		if (pScope->GetContinueBlock ())
		{
			i++;
			if (i >= Level)
				break;
		}
	}

	return pScope;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
