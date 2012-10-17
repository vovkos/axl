#include "stdafx.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_Parser.h"

namespace axl {
namespace jnc {

//.............................................................................

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
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CFunction* pFunction = AXL_MEM_NEW (CFunction);
	pFunction->m_pType = pType;
	
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
			pArg->m_pType = pType->GetArg (i);
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

	CFunction* pGetter = CreateFunction (pPropertyType->GetGetterType (), NULL);
			
	CFunctionOverload Setter;

	size_t Count = pSetterType->GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunctionType* pSetterTypeOverload = pSetterType->GetType (i);
		CFunction* pSetterOverload = CreateFunction (pSetterTypeOverload, NULL);
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
	CFunction* pFunction = CreateFunction (pType, pArgList);
	return CreateGlobalFunction (Name, pFunction);
}

CGlobalFunction*
CFunctionMgr::CreateGlobalFunction (
	const rtl::CString& Name,
	CFunction* pFunction
	)
{
	CGlobalFunction* pGlobalFunction = AXL_MEM_NEW (CGlobalFunction);
	pGlobalFunction->m_Name = Name;
	pGlobalFunction->m_pFunction = pFunction;
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

	rtl::CIteratorT <jnc::CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		jnc::CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		jnc::CParser Parser;
		Parser.m_Stage = jnc::CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;
		Parser.Create (jnc::ESymbol_function_body, true); 
		
		rtl::CBoxIteratorT <jnc::CToken> Token = pFunction->GetBodyFirstToken ();
		for (; Token; Token++)
		{
			Result = Parser.ParseToken (&*Token);
			if (!Result)
			{
				err::PushSrcPosError (m_pModule->m_FilePath, Token->m_Pos.m_Line, Token->m_Pos.m_Col);
				return false;
			}
		}

		pFunction->m_Ast = Parser.GetAst ();
		jnc::CParser::CAstNode* pAstNode = pFunction->m_Ast->GetRoot ();

		ASSERT (pAstNode->m_Kind == jnc::ESymbol_function_body);
		jnc::CParser::CFunctionBody* pBody = (jnc::CParser::CFunctionBody*) pAstNode;
		pFunction->m_pBlock = pBody->m_pBlock;
		pFunction->m_pScope = pBody->m_pScope;
	}

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
