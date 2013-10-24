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

bool
CNamespaceMgr::AddStdItems ()
{
	CGlobalNamespace* pJnc = CreateGlobalNamespace ("jnc", &m_GlobalNamespace);

	return
		m_GlobalNamespace.AddItem (pJnc) &&
		m_GlobalNamespace.AddItem (m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_StrLen)) &&
		m_GlobalNamespace.AddItem (m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_Rand)) &&
		m_GlobalNamespace.AddItem (m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_Printf)) &&
		pJnc->AddItem ((CClassType*) m_pModule->m_TypeMgr.GetStdType (EStdType_Scheduler)) &&
		pJnc->AddItem (m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_RunGc)) &&
		pJnc->AddItem (m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_RunGcWaitForDestructors)) &&
		pJnc->AddItem (m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_GetCurrentThreadId)) &&
		pJnc->AddItem (m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_CreateThread)) &&
		pJnc->AddItem (m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_Sleep));
}

void
CNamespaceMgr::SetSourcePos (const CToken::CPos& Pos)
{
	if (!(m_pModule->GetFlags () & EModuleFlag_DebugInfo))
		return;

	ASSERT (m_pCurrentScope);

	llvm::DebugLoc LlvmDebugLoc = m_pModule->m_LlvmDiBuilder.GetDebugLoc (m_pCurrentScope, Pos);
	m_pModule->m_LlvmIrBuilder.SetCurrentDebugLoc (LlvmDebugLoc);
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

	if (m_pCurrentScope)
		pScope->m_Flags |= m_pCurrentScope->m_Flags & (EScopeFlag_CanThrow | EScopeFlag_HasFinally);
	else if (pFunction->GetType ()->GetFlags () & EFunctionTypeFlag_Pitcher)
		pScope->m_Flags |= EScopeFlag_CanThrow;

	if (m_pModule->GetFlags () & EModuleFlag_DebugInfo)
		pScope->m_LlvmDiScope = (llvm::DIScope) m_pModule->m_LlvmDiBuilder.CreateLexicalBlock (m_pCurrentScope, Pos);

	m_ScopeList.InsertTail (pScope);

	OpenNamespace (pScope);
	SetSourcePos (Pos);
	return pScope;
}

void
CNamespaceMgr::CloseScope (const CToken::CPos& Pos)
{
	CScope* pScope = m_pCurrentScope;
	ASSERT (pScope);

	pScope->m_EndPos = Pos;
	SetSourcePos (Pos);

	if (m_pModule->m_ControlFlowMgr.GetCurrentBlock ()->GetFlags () & EBasicBlockFlag_Reachable)
	{
		pScope->m_DestructList.RunDestructors ();
		m_pModule->m_OperatorMgr.NullifyGcRootList (pScope->GetGcRootList ());
	}

	if (pScope->m_Flags & EScopeFlag_FinallyDefined)
		m_pModule->m_ControlFlowMgr.EndFinally ();

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
	CNamespace* pParentNamespace
	)
{
	if (!pParentNamespace)
		pParentNamespace = &m_GlobalNamespace;

	rtl::CString QualifiedName = pParentNamespace->CreateQualifiedName (Name);

	CGlobalNamespace* pNamespace = AXL_MEM_NEW (CGlobalNamespace);
	pNamespace->m_pModule = m_pModule;
	pNamespace->m_Name = Name;
	pNamespace->m_QualifiedName = QualifiedName;
	pNamespace->m_Tag = QualifiedName;
	pNamespace->m_pParentNamespace = pParentNamespace;
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

CScope*
CNamespaceMgr::FindCatchScope ()
{
	CScope* pScope = m_pCurrentScope;
	for (; pScope; pScope = pScope->GetParentScope ())
	{
		if (pScope->m_pCatchBlock)
			break;
	}

	return pScope;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
