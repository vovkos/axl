#include "stdafx.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Parser.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionMgr::CFunctionMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	m_pCurrentFunction = NULL;
}

void
CFunctionMgr::Clear ()
{
	m_FunctionList.Clear ();
	m_PropertyList.Clear ();
	m_GlobalFunctionList.Clear ();
	m_GlobalPropertyList.Clear ();
}

CFunction*
CFunctionMgr::CreateFunction (
	const rtl::CString& Tag,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CFunction* pFunction = AXL_MEM_NEW (CFunction);
	pFunction->m_pModule = m_pModule;
	pFunction->m_pType = pType;
	pFunction->m_Tag = Tag;
	
	if (pArgList)
	{
		pFunction->m_ArgList.TakeOver (pArgList);
	}
	else
	{
		// create default argument names

		size_t Count = pType->GetArgCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CFunctionFormalArg* pArg = AXL_MEM_NEW (CFunctionFormalArg);
			pArg->m_Name.Format (_T("a%d"), i + 1);
			pArg->m_pType = pType->GetArgType (i);
			pFunction->m_ArgList.InsertTail (pArg);
		}
	}

	m_FunctionList.InsertTail (pFunction);
	return pFunction;
}

CProperty*
CFunctionMgr::CreateProperty (
	CPropertyType* pType,
	CFunction* pGetter,
	const CFunctionOverload& Setter
	)
{
	CProperty* pProperty = AXL_MEM_NEW (CProperty);
	pProperty->m_pModule = m_pModule;
	pProperty->m_pType = pType;
	pProperty->m_pGetter = pGetter;
	pProperty->m_Setter = Setter;
	m_PropertyList.InsertTail (pProperty);
	return pProperty;
}

CProperty*
CFunctionMgr::CreateProperty (CPropertyType* pPropertyType)
{
	CFunctionTypeOverload* pSetterType = pPropertyType->GetSetterType ();

	CFunction* pGetter = CreateFunction (_T("getter"), pPropertyType->GetGetterType (), NULL);
			
	CFunctionOverload Setter;

	size_t Count = pSetterType->GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pSetterTypeOverload = pSetterType->GetType (i);
		CFunction* pSetterOverload = CreateFunction (_T("setter"), pSetterTypeOverload, NULL);
		Setter.AddOverload (pSetterOverload);
	}

	return CreateProperty (pPropertyType, pGetter, Setter);
}

CGlobalFunction*
CFunctionMgr::CreateGlobalFunction (
	const rtl::CString& Name,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CFunction* pFunction = CreateFunction (Name, pType, pArgList);
	CGlobalFunction* pGlobalFunction = AXL_MEM_NEW (CGlobalFunction);
	pGlobalFunction->m_Name = Name;
	pGlobalFunction->m_pFunction = pFunction;
	pFunction->m_pOverload = pGlobalFunction;
	m_GlobalFunctionList.InsertTail (pGlobalFunction);
	return pGlobalFunction;
}

CGlobalProperty*
CFunctionMgr::CreateGlobalProperty (
	const rtl::CString& Name,
	CPropertyType* pType,
	CFunction* pGetter,
	const CFunctionOverload& Setter
	)
{
	CProperty* pProperty = CreateProperty (pType, pGetter, Setter);
	return CreateGlobalProperty (Name, pProperty);
}

CGlobalProperty*
CFunctionMgr::CreateGlobalProperty (
	const rtl::CString& Name,
	CProperty* pProperty
	)
{
	CGlobalProperty* pGlobalProperty = AXL_MEM_NEW (CGlobalProperty);
	pGlobalProperty->m_Name = Name;
	pGlobalProperty->m_pProperty = pProperty;
	m_GlobalPropertyList.InsertTail (pGlobalProperty);
	return pGlobalProperty;
}

bool
CFunctionMgr::CompileFunctions ()
{
	bool Result;

	CSetCurrentThreadModule ScopeModule (m_pModule);

	rtl::CIteratorT <jnc::CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		jnc::CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		CNamespace* pNamespace = NULL;

		EFunction FunctionKind = pFunction->GetFunctionKind ();
		switch (FunctionKind)
		{
		case EFunction_Global:
			{
			CGlobalFunction* pGlobalFunction = (CGlobalFunction*) pFunction->GetOverload ();
			pNamespace = pGlobalFunction->GetParentNamespace ();
			break;
			}

		default:
			ASSERT (false);
		}

		m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);

		m_pCurrentFunction = pFunction;
		pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));
		m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

		jnc::CParser Parser;
		Parser.m_Stage = jnc::CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;
		Parser.Create (jnc::ESymbol_compound_stmt, true); 
			
		rtl::CBoxIteratorT <jnc::CToken> Token = pFunction->GetBodyFirstToken ();
		for (; Token; Token++)
		{
			Result = Parser.ParseToken (&*Token);
			if (!Result)
			{
				err::PushSrcPosError (m_pModule->GetFilePath (), Token->m_Pos.m_Line, Token->m_Pos.m_Col);
				return false;
			}
		}

		// llvm::verifyFunction (pFunction->GetLlvmFunction (), llvm::PrintMessageAction);

		pFunction->m_Ast = Parser.GetAst ();
		jnc::CParser::CAstNode* pAstNode = pFunction->m_Ast->GetRoot ();

		ASSERT (pAstNode->m_Kind == jnc::ESymbol_compound_stmt);
		jnc::CParser::CCompoundStmt* pBody = (jnc::CParser::CCompoundStmt*) pAstNode;
		pFunction->m_pScope = pBody->m_pScope;


	}

	m_pModule->m_NamespaceMgr.SetGlobalNamespace (); // just in case

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
