#include "pch.h"
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
	m_NamespaceStack.Clear ();
	m_ScopeList.Clear ();
	m_pCurrentNamespace = &m_GlobalNamespace;
	m_pCurrentScope = NULL;
}

void
CNamespaceMgr::OpenNamespace (CNamespace* pNamespace)
{
	m_NamespaceStack.Append (m_pCurrentNamespace);
	m_pCurrentNamespace = pNamespace;
	m_pCurrentScope = pNamespace->m_NamespaceKind == ENamespace_Scope ? (CScope*) pNamespace : NULL;
}

void
CNamespaceMgr::CloseNamespace ()
{
	if (m_NamespaceStack.IsEmpty ())
		return;

	m_pCurrentNamespace = m_NamespaceStack.GetBackAndPop ();
	m_pCurrentScope = m_pCurrentNamespace->m_NamespaceKind == ENamespace_Scope ? (CScope*) m_pCurrentNamespace : NULL;
}

CScope*
CNamespaceMgr::OpenScope (const CToken::CPos& Pos)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	CScope* pScope = AXL_MEM_NEW (CScope);
	pScope->m_pModule = m_pModule;
	pScope->m_pFunction = pFunction;
	pScope->m_Level = m_pCurrentScope ? m_pCurrentScope->GetLevel () + 1 : 1;
	pScope->m_BeginPos = Pos;
	pScope->m_EndPos = Pos;
	pScope->m_pParentNamespace = m_pCurrentNamespace;
	m_ScopeList.InsertTail (pScope);

	OpenNamespace (pScope);
	return pScope;
}

void
CNamespaceMgr::CloseScope (const CToken::CPos& Pos)
{
	ASSERT (m_pCurrentScope);

	m_pCurrentScope->m_EndPos = Pos;
	m_pModule->m_OperatorMgr.ProcessDestructList (m_pCurrentScope->GetDestructList ());
	CloseNamespace ();
}

EAccess
CNamespaceMgr::GetAccessKind (CNamespace* pTargetNamespace)
{
	CNamespace* pNamespace = m_pCurrentNamespace;

	if (!pTargetNamespace->IsNamed ())
	{
		for (; pNamespace; pNamespace = pNamespace->m_pParentNamespace)
		{
			if (pNamespace == pTargetNamespace)
				return EAccess_Protected;
		}

		return EAccess_Public;
	}

	if (pTargetNamespace->m_NamespaceKind != ENamespace_Type)
	{
		for (; pNamespace; pNamespace = pNamespace->m_pParentNamespace)
		{
			if (!pNamespace->IsNamed ())
				continue;

			if (pNamespace == pTargetNamespace || 
				pTargetNamespace->m_QualifiedName.Cmp (pNamespace->m_QualifiedName) == 0 ||
				pTargetNamespace->m_FriendSet.Find (pNamespace->m_QualifiedName))
				return EAccess_Protected;
		}

		return EAccess_Public;
	}

	CNamedType* pTargetType = (CNamedType*) pTargetNamespace;

	for (; pNamespace; pNamespace = pNamespace->m_pParentNamespace)
	{
		if (!pNamespace->IsNamed ())
			continue;

		if (pNamespace == pTargetNamespace || 
			pTargetNamespace->m_QualifiedName.Cmp (pNamespace->m_QualifiedName) == 0 ||
			pTargetNamespace->m_FriendSet.Find (pNamespace->m_QualifiedName))
			return EAccess_Protected;

		if (pNamespace->m_NamespaceKind == ENamespace_Type)
		{
			CNamedType* pType = (CNamedType*) pNamespace;
			EType TypeKind = pType->GetTypeKind ();
			if (TypeKind == EType_Class || TypeKind == EType_Struct)
			{
				bool Result = ((CDerivableType*) pType)->FindBaseTypeTraverse (pTargetType);
				if (Result)
					return EAccess_Protected;
			}
		}
	}

	return EAccess_Public;
}

CGlobalNamespace*
CNamespaceMgr::CreateGlobalNamespace (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	CGlobalNamespace* pNamespace = AXL_MEM_NEW (CGlobalNamespace);
	pNamespace->m_pModule = m_pModule;
	pNamespace->m_Name = Name;
	pNamespace->m_QualifiedName = QualifiedName;
	pNamespace->m_Tag = QualifiedName;
	m_NamespaceList.InsertTail (pNamespace);
	return pNamespace;
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
