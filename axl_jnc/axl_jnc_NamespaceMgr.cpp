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

void
CNamespaceMgr::SetCurrentNamespace (CNamespace* pNamespace)
{
	if (!pNamespace)
		pNamespace = &m_GlobalNamespace;

	m_pCurrentNamespace = pNamespace;
	m_pCurrentScope = pNamespace->m_NamespaceKind == ENamespace_Scope ? (CScope*) pNamespace : NULL;
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
				bool Result = ((CDerivableType*) pType)->FindBaseType (pTargetType);
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
CNamespaceMgr::CreateScope ()
{
	CScope* pScope = AXL_MEM_NEW (CScope);
	pScope->m_pModule = m_pModule;
	m_ScopeList.InsertTail (pScope);
	return pScope;
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
