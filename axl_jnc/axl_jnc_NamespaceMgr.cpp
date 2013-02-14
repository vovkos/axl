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

#pragma AXL_TODO ("refine open / close handling: move it to CParser")

CGlobalNamespace*
CNamespaceMgr::OpenNamespace (
	const CToken::CPos& Pos,
	const CQualifiedName& Name
	)
{
	ASSERT (!m_pCurrentScope);

	m_NamespaceStack.Append (m_pCurrentNamespace);

	CGlobalNamespace* pNamespace = OpenNamespaceImpl (Pos, Name.m_First, false);
	if (!pNamespace)
		return NULL;

	rtl::CBoxIteratorT <rtl::CString> It = Name.m_List.GetHead ();
	for (; It; It++)
	{
		pNamespace = OpenNamespaceImpl (Pos, *It, false);
		if (!pNamespace)
			return NULL;
	}
	
	ASSERT (m_pCurrentNamespace == pNamespace);
	return pNamespace;
}

CGlobalNamespace*
CNamespaceMgr::OpenNamespaceImpl (
	const CToken::CPos& Pos,
	const rtl::CString& Name,
	bool AddToStack
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

	if (AddToStack)
		m_NamespaceStack.Append (m_pCurrentNamespace);

	m_pCurrentNamespace = pNamespace;
	return pNamespace;
}

void
CNamespaceMgr::OpenNamespace (CNamespace* pNamespace)
{
	m_NamespaceStack.Append (m_pCurrentNamespace);

	if (!pNamespace->m_pParentNamespace)
		pNamespace->m_pParentNamespace = m_pCurrentNamespace;
	else
		ASSERT (pNamespace->m_pParentNamespace == m_pCurrentNamespace);

	m_pCurrentNamespace = pNamespace;
}

void
CNamespaceMgr::CloseNamespace ()
{
	ASSERT (!m_pCurrentScope);

	if (!m_NamespaceStack.IsEmpty ())
		m_pCurrentNamespace = m_NamespaceStack.GetBackAndPop ();
}

CScope*
CNamespaceMgr::OpenScope (	
	const CToken::CPos& Pos,
	CBasicBlock* pBreakBlock,
	CBasicBlock* pContinueBlock
	)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	CScope* pScope = AXL_MEM_NEW (CScope);
	pScope->m_Level = m_pCurrentScope ? m_pCurrentScope->m_Level + 1 : 1;
	pScope->m_pModule = m_pModule;
	pScope->m_pFunction = pFunction;
	pScope->m_BeginPos = Pos;
	pScope->m_EndPos = Pos;
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

	m_pModule->m_OperatorMgr.ProcessDestructList (&m_pCurrentScope->m_DestructList);

	ASSERT (m_pCurrentScope == m_pCurrentNamespace);
	m_pCurrentScope->m_EndPos = Pos;

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
		if (pScope->m_pBreakBlock)
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
		if (pScope->m_pContinueBlock)
		{
			i++;
			if (i >= Level)
				break;
		}
	}

	return pScope;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
