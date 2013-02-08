#include "stdafx.h"
#include "axl_jnc_FunctionMgr.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Parser.h"

namespace axl {
namespace jnc {

//.............................................................................

CFunctionMgr::CThunk::CThunk ()
{
	m_ThunkKind = EThunk_Undefined;
	m_pTargetFunctionType = NULL;
	m_pTargetFunction = NULL;
	m_pClosureType = NULL;
	m_pThunkFunction = NULL;
}

//.............................................................................

CFunctionMgr::CFunctionMgr ()
{
	m_pModule = GetCurrentThreadModule ();
	ASSERT (m_pModule);

	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));
	m_pCurrentFunction = NULL;
}

void
CFunctionMgr::Clear ()
{
	m_FunctionList.Clear ();
	m_PropertyList.Clear ();
	m_PropertyTemplateList.Clear ();
	m_OrphanFunctionArray.Clear ();
	m_GlobalAutoEvTypeArray.Clear ();
	m_AutoPropertyArray.Clear ();
	m_ThunkList.Clear ();
	m_ThunkFunctionMap.Clear ();
	m_ThunkPropertyMap.Clear ();

	memset (m_StdFunctionArray, 0, sizeof (m_StdFunctionArray));
	m_pCurrentFunction = NULL;
}

CFunction*
CFunctionMgr::CreateFunction (
	EFunction FunctionKind,
	CFunctionType* pType,
	rtl::CStdListT <CFunctionFormalArg>* pArgList
	)
{
	CFunction* pFunction = AXL_MEM_NEW (CFunction);
	pFunction->m_pModule = m_pModule;
	pFunction->m_FunctionKind = FunctionKind;
	pFunction->m_pType = pType;
	pFunction->m_TypeOverload.AddOverload (pType);

	if (pArgList)
		pFunction->m_ArgList.TakeOver (pArgList);

	m_FunctionList.InsertTail (pFunction);
	return pFunction;
}

CProperty*
CFunctionMgr::CreateProperty (
	const rtl::CString& Name,
	const rtl::CString& QualifiedName
	)
{
	CProperty* pProperty = AXL_MEM_NEW (CProperty);
	pProperty->m_pModule = m_pModule;
	pProperty->m_Name = Name;
	pProperty->m_QualifiedName = QualifiedName;
	pProperty->m_Tag = QualifiedName;
	m_PropertyList.InsertTail (pProperty);
	return pProperty;
}

CPropertyTemplate*
CFunctionMgr::CreatePropertyTemplate ()
{
	CPropertyTemplate* pPropertyTemplate = AXL_MEM_NEW (CPropertyTemplate);
	m_PropertyTemplateList.InsertTail (pPropertyTemplate);
	return pPropertyTemplate;
}

bool
CFunctionMgr::ResolveOrphanFunctions ()
{
	bool Result;

	size_t Count = m_OrphanFunctionArray.GetCount ();

	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pFunction = m_OrphanFunctionArray [i];
		
		Result = pFunction->ResolveOrphan ();
		if (!Result)
			return false;
	}

	return true;
}

bool
CFunctionMgr::CompileFunctions ()
{
	bool Result;

	CSetCurrentThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	// (0) layout of properties

	rtl::CIteratorT <CProperty> Property = m_PropertyList.GetHead ();
	for (; Property; Property++)
	{
		Result = Property->CalcLayout ();
		if (!Result)
			return false;
	}

	// (1) global aev

	size_t Count = m_GlobalAutoEvTypeArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CClassType* pType = m_GlobalAutoEvTypeArray [i];

		CNamespace* pNamespace = pType->GetParentNamespace ();
		m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);

		CParser Parser;
		Parser.m_Stage = CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;
		// Parser.m_ThisValue = ThisValue; 
		
		Parser.Create (ESymbol_autoev_body, true); 
		
		CParser::CSymbolNode_autoev_body* pSymbol = (CParser::CSymbolNode_autoev_body*) Parser.GetPredictionTop ();
		pSymbol->m_Arg.pType = pType;
			
		rtl::CBoxIteratorT <CToken> Token = pType->GetAutoEvBody ()->GetHead ();
		for (; Token; Token++)
		{
			Result = Parser.ParseToken (&*Token);
			if (!Result)
			{
				err::PushSrcPosError (m_pModule->GetFilePath (), Token->m_Pos.m_Line, Token->m_Pos.m_Col);
				return false;
			}
		}
	}

	// (2) functions

	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		// set namespace

		CNamespace* pNamespace = pFunction->GetParentNamespace ();
		m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);

		// prologue

		CValue ThisValue;
		Result = Prologue (pFunction, pFunction->GetBody ()->GetHead ()->m_Pos, &ThisValue);
		if (!Result)
			return false;

		// parse body

		CParser Parser;
		Parser.m_Stage = CParser::EStage_Pass2;
		Parser.m_pModule = m_pModule;
		Parser.m_ThisValue = ThisValue;
		Parser.Create (ESymbol_compound_stmt, true); 
			
		rtl::CBoxIteratorT <CToken> Token = pFunction->GetBody ()->GetHead ();
		for (; Token; Token++)
		{
			Result = Parser.ParseToken (&*Token);
			if (!Result)
			{
				err::PushSrcPosError (m_pModule->GetFilePath (), Token->m_Pos.m_Line, Token->m_Pos.m_Col);
				return false;
			}
		}

		pFunction->m_Ast = Parser.GetAst ();

		// epilogue

		Result = Epilogue (pFunction->GetBody ()->GetTail ()->m_Pos);
		if (!Result)
			return false;
	}

	// (3) thunks

	rtl::CIteratorT <CThunk> Thunk = m_ThunkList.GetHead ();
	for (; Thunk; Thunk++)
	{
		CThunk* pThunk = *Thunk;

		Result = pThunk->m_pClosureType ?
			CompileClosureThunk (*Thunk) :
			CompileDirectThunk (*Thunk);

		if (!Result)
			return false;
	}

	// (4) auto-properties

	Count = m_AutoPropertyArray.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CProperty* pProperty = m_AutoPropertyArray [i];
		Result = CompileAutoPropertyAccessors (pProperty);
		if (!Result)
			return false;
	}

	m_pModule->m_NamespaceMgr.SetGlobalNamespace (); // just in case
	return true;
}

bool
CFunctionMgr::Prologue (
	CFunction* pFunction,
	const CToken::CPos& Pos,
	CValue* pThisValue
	)
{
	bool Result;

	m_pCurrentFunction = pFunction;

	// get this-value for methods

	CClassType* pThisType = pFunction->GetClassType ();
	CClassType* pArgThisType = pFunction->GetStorageKind () == EStorage_Virtual ? 
		pFunction->GetVirtualOriginClassType () : 
		pThisType;

	// create entry block and scope

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);
	m_pModule->m_ControlFlowMgr.ResetHasReturn ();

	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (Pos);
	pFunction->m_pScope = pScope;

	// store arguments

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

	if (pThisType)
	{
		llvm::Value* pLlvmArg = LlvmArg;
		CValue ArgValue (pLlvmArg, pArgThisType);			
		Result = CreateThisValue (ArgValue, pThisType, pThisValue);
		if (!Result)
			return false;

		LlvmArg++;
	}

	rtl::CIteratorT <CFunctionFormalArg> Arg = pFunction->GetArgList ().GetHead ();
	for (; Arg; Arg++, LlvmArg++)
	{
		CFunctionFormalArg* pArg = *Arg;
		llvm::Value* pLlvmArg = LlvmArg;

		CVariable* pArgVariable = m_pModule->m_VariableMgr.CreateVariable (
			pArg->GetName (), 
			pArg->GetType (), 
			true
			);

		CValue ArgValue (pLlvmArg, pArg->GetType ());

		m_pModule->m_LlvmBuilder.CreateStore (ArgValue, pArgVariable);
		pScope->AddItem (pArgVariable);
	}

	// store scope level

	pFunction->m_pScopeLevelVariable = m_pModule->m_VariableMgr.CreateVariable (
		_T("ScopeLevel"), 
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT), 
		true
		);

	CValue ScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateLoad (m_pModule->m_VariableMgr.GetScopeLevelVariable (), NULL, &ScopeLevelValue);
	m_pModule->m_LlvmBuilder.CreateStore (ScopeLevelValue, pFunction->m_pScopeLevelVariable);

	return true;
}

bool
CFunctionMgr::Epilogue (const CToken::CPos& Pos)
{
	ASSERT (m_pCurrentFunction);
	CFunction* pFunction = m_pCurrentFunction;
	
	// ensure return

	CBasicBlock* pCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
	if (!pCurrentBlock->HasReturn ()) 
	{
		CType* pReturnType = pFunction->GetType ()->GetReturnType ();
		if (pReturnType->GetTypeKind () == EType_Void)
		{
			m_pModule->m_ControlFlowMgr.Return ();
		}
		else if (!(m_pModule->m_ControlFlowMgr.GetFlags () & EControlFlowMgrFlag_HasReturn))
		{
			err::SetFormatStringError (
				_T("function '%s' must return a '%s' value"),
				pFunction->m_Tag,
				pReturnType->GetTypeString ()
				);
			return false;
		}
		else if (pCurrentBlock->GetFlags () & EBasicBlockFlag_Jumped)
		{
			err::SetFormatStringError (
				_T("not all control paths in function '%s' return a value"),
				pFunction->m_Tag
				);
			return false;
		}
		else if (!pCurrentBlock->HasTerminator ())
		{	
			// make LLVM happy
			m_pModule->m_LlvmBuilder.CreateUnreachable (); 
		}
	}

	m_pModule->m_NamespaceMgr.CloseScope (Pos);

	try 
	{
		llvm::verifyFunction (*pFunction->GetLlvmFunction (), llvm::ReturnStatusAction);
	}
	catch (err::CError Error)
	{
		err::SetFormatStringError (
			_T("LLVM verification fail for '%s': %s"), 
			pFunction->m_Tag,
			Error->GetDescription ()
			);

		return false;
	}

	m_pCurrentFunction = NULL;
	return true;
}

CFunction*
CFunctionMgr::GetDirectThunkFunction (
	CFunction* pTargetFunction,
	CFunctionType* pThunkFunctionType,
	bool HasUnusedClosure
	)
{
	char SignatureChar = 'D';
	EThunk ThunkKind = EThunk_Direct;

	if (HasUnusedClosure)
	{
		SignatureChar = 'U';
		ThunkKind = EThunk_DirectUnusedClosure;
		pThunkFunctionType = pThunkFunctionType->GetAbstractMethodMemberType ();
	}

	rtl::CStringA Signature;
	Signature.Format (
		"%c%x.%s", 
		HasUnusedClosure ? 'U' : 'D',
		pTargetFunction, 
		pThunkFunctionType->GetSignature ()
		);

	rtl::CStringHashTableMapIteratorAT <CFunction*> Thunk = m_ThunkFunctionMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;
	
	CFunction* pThunkFunction = CreateFunction (EFunction_Thunk, pThunkFunctionType);
	pThunkFunction->m_Tag = _T("_direct_thunk_function");

	CThunk* pThunk = AXL_MEM_NEW (CThunk);
	pThunk->m_ThunkKind = ThunkKind;
	pThunk->m_pTargetFunctionType = pTargetFunction->GetType ();
	pThunk->m_pTargetFunction = pTargetFunction;
	pThunk->m_pThunkFunction = pThunkFunction;
	m_ThunkList.InsertTail (pThunk);

	Thunk->m_Value = pThunkFunction;
	return pThunkFunction;
}

CFunction*
CFunctionMgr::GetClosureThunkFunction (
	CFunctionType* pTargetFunctionType,
	CFunction* pTargetFunction,
	CClassType* pClosureType,
	const rtl::CArrayT <size_t>& ClosureMap,
	CFunctionType* pThunkFunctionType
	)
{
	rtl::CStringA Signature;
	Signature.Format (_T("'C'%s.%x.%s"), 
		pTargetFunctionType->GetSignature (),
		pTargetFunction, 
		pThunkFunctionType->GetSignature ()
		);

	size_t Count = ClosureMap.GetCount ();
	for (size_t i = 0; i < Count; i++)
		Signature.AppendFormat (_T(".%d"), ClosureMap [i]);

	rtl::CStringHashTableMapIteratorAT <CFunction*> Thunk = m_ThunkFunctionMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;
	
	pThunkFunctionType = pClosureType->GetMethodMemberType (pThunkFunctionType);
	
	CFunction* pThunkFunction = CreateFunction (EFunction_Thunk, pThunkFunctionType);
	pThunkFunction->m_Tag = _T("_closure_thunk_function");

	CThunk* pThunk = AXL_MEM_NEW (CThunk);
	pThunk->m_ThunkKind = EThunk_Closure;
	pThunk->m_pTargetFunctionType = pTargetFunctionType;
	pThunk->m_pTargetFunction = pTargetFunction;
	pThunk->m_pClosureType = pClosureType;
	pThunk->m_ClosureMap = ClosureMap;
	pThunk->m_pThunkFunction = pThunkFunction;
	m_ThunkList.InsertTail (pThunk);
	
	Thunk->m_Value = pThunkFunction;
	return pThunkFunction;
}

CProperty*
CFunctionMgr::GetDirectThunkProperty (
	CProperty* pTargetProperty,
	CPropertyType* pThunkPropertyType,
	bool HasUnusedClosure
	)
{
	char SignatureChar = 'D';
	EThunk ThunkKind = EThunk_Direct;

	if (HasUnusedClosure)
	{
		SignatureChar = 'U';
		ThunkKind = EThunk_DirectUnusedClosure;
		pThunkPropertyType = pThunkPropertyType->GetAbstractPropertyMemberType ();
	}

	rtl::CStringA Signature;
	Signature.Format (
		"%c%x.%s", 
		HasUnusedClosure ? 'U' : 'D',
		pTargetProperty, 
		pThunkPropertyType->GetSignature ()
		);

	rtl::CStringHashTableMapIteratorAT <CProperty*> Thunk = m_ThunkPropertyMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;
	
	CProperty* pThunkProperty = CreateProperty (rtl::CString (), rtl::CString ());
	pThunkProperty->m_Tag = _T("_direct_thunk_property");
	pThunkProperty->m_pGetter = GetDirectThunkFunction (
		pTargetProperty->m_pGetter, 
		pThunkPropertyType->GetGetterType (),
		HasUnusedClosure
		);

	CFunctionTypeOverload* pThunkSetterType = pThunkPropertyType->GetSetterType ();
	size_t SetterCount = pThunkSetterType->GetOverloadCount ();

	// all the checks should have been done at CheckCast ()

	ASSERT (SetterCount == 0 || pTargetProperty->m_pSetter); 
	
	for (size_t i = 0; i < SetterCount; i++)
	{
		CFunctionType* pThunkFunctionType = pThunkSetterType->GetOverload (i);
		CFunction* pTargetSetter = pTargetProperty->m_pSetter->ChooseOverload (pThunkFunctionType->GetArgTypeArray ());
		ASSERT (pTargetSetter);

		CFunction* pThunkFunction = GetDirectThunkFunction (
			pTargetSetter, 
			pThunkFunctionType,
			HasUnusedClosure
			);

		if (!pThunkProperty->m_pSetter)
		{
			pThunkProperty->m_pSetter = pThunkFunction;
		}
		else
		{
			bool Result = pThunkProperty->m_pSetter->AddOverload (pThunkFunction);
			ASSERT (Result);
		}
	}

	Thunk->m_Value = pThunkProperty;
	return pThunkProperty;
}

CProperty*
CFunctionMgr::GetClosureThunkProperty (
	CPropertyType* pTargetPropertyType,
	CProperty* pTargetProperty,
	CClassType* pClosureType,
	const rtl::CArrayT <size_t>& ClosureMap,
	CPropertyType* pThunkPropertyType
	)
{
	rtl::CStringA Signature;
	Signature.Format (_T("'C'%s.%x.%s"), 
		pTargetPropertyType->GetSignature (),
		pTargetProperty, 
		pThunkPropertyType->GetSignature ()
		);

	size_t Count = ClosureMap.GetCount ();
	for (size_t i = 0; i < Count; i++)
		Signature.AppendFormat (_T(".%d"), ClosureMap [i]);

	rtl::CStringHashTableMapIteratorAT <CProperty*> Thunk = m_ThunkPropertyMap.Goto (Signature);
	if (Thunk->m_Value)
		return Thunk->m_Value;
	
	pThunkPropertyType = pClosureType->GetPropertyMemberType (pThunkPropertyType);
	
	CProperty* pThunkProperty = CreateProperty (rtl::CString (), rtl::CString ());
	pThunkProperty->m_Tag = _T("_closure_thunk_property");

	// i use ASSERT () cause all the checks should have been done at CheckCast ()

	if (pTargetProperty)
	{
		pThunkProperty->m_pGetter = GetClosureThunkFunction (
			pTargetProperty->m_pGetter->GetType (), 
			pTargetProperty->m_pGetter, 
			pClosureType,
			ClosureMap, 
			pThunkPropertyType->GetGetterType ()
			);

		CFunctionTypeOverload* pThunkSetterType = pThunkPropertyType->GetSetterType ();
		size_t SetterCount = pThunkSetterType->GetOverloadCount ();

		ASSERT (SetterCount == 0 || pTargetProperty->m_pSetter); 
	
		for (size_t i = 0; i < SetterCount; i++)
		{
			CFunctionType* pThunkFunctionType = pThunkSetterType->GetOverload (i);
			CFunction* pTargetSetter = pTargetProperty->m_pSetter->ChooseOverload (pThunkFunctionType->GetArgTypeArray ());
			ASSERT (pTargetSetter);

			CFunction* pThunkFunction = GetClosureThunkFunction (
				pTargetSetter->GetType (), 
				pTargetSetter,
				pClosureType,
				ClosureMap,
				pThunkFunctionType
				);

			if (!pThunkProperty->m_pSetter)
			{
				pThunkProperty->m_pSetter = pThunkFunction;
			}
			else
			{
				bool Result = pThunkProperty->m_pSetter->AddOverload (pThunkFunction);
				ASSERT (Result);
			}
		}
	}
	else
	{
		pThunkProperty->m_pGetter = GetClosureThunkFunction (
			pTargetPropertyType->GetGetterType (), 
			NULL, 
			pClosureType,
			ClosureMap, 
			pThunkPropertyType->GetGetterType ()
			);

		CFunctionTypeOverload* pTargetSetterType = pTargetPropertyType->GetSetterType ();
	
		CFunctionTypeOverload* pThunkSetterType = pThunkPropertyType->GetSetterType ();
		size_t SetterCount = pThunkSetterType->GetOverloadCount ();

		for (size_t i = 0; i < SetterCount; i++)
		{
			CFunctionType* pThunkFunctionType = pThunkSetterType->GetOverload (i);

			size_t j = pTargetSetterType->ChooseOverload (pThunkFunctionType->GetArgTypeArray ());
			ASSERT (j != -1);

			CFunctionType* pTargetFunctionType = pTargetSetterType->GetOverload (j);

			CFunction* pThunkFunction = GetClosureThunkFunction (
				pTargetFunctionType,
				NULL,
				pClosureType,
				ClosureMap,
				pThunkFunctionType
				);

			if (!pThunkProperty->m_pSetter)
			{
				pThunkProperty->m_pSetter = pThunkFunction;
			}
			else
			{
				bool Result = pThunkProperty->m_pSetter->AddOverload (pThunkFunction);
				ASSERT (Result);
			}
		}
	}

	Thunk->m_Value = pThunkProperty;
	return pThunkProperty;
}

bool
CFunctionMgr::CompileDirectThunk (CThunk* pThunk)
{
	ASSERT (pThunk->m_pTargetFunction);

	bool Result;

	rtl::CArrayT <CType*> TargetArgTypeArray = pThunk->m_pTargetFunction->GetType ()->GetArgTypeArray ();
	rtl::CArrayT <CType*> ThunkArgTypeArray = pThunk->m_pThunkFunction->GetType ()->GetArgTypeArray ();

	size_t TargetArgCount = TargetArgTypeArray.GetCount ();
	size_t ThunkArgCount = ThunkArgTypeArray.GetCount ();

	char Buffer [256];
	rtl::CArrayT <CValue> ArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgArray.SetCount (TargetArgCount);

	CFunction* pFunction = pThunk->m_pThunkFunction;

	m_pCurrentFunction = pFunction;
	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

	// handle the first thunk argument (it's either NULL or actual interface)

	size_t i = 0;

	if (TargetArgCount == ThunkArgCount)
	{
		CValue IfaceValue (LlvmArg, m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr));
		m_pModule->m_LlvmBuilder.CreateBitCast (IfaceValue, TargetArgTypeArray [0], &IfaceValue);
		ArgArray [0] = IfaceValue;
		i++;
	}
	else
	{
		ASSERT (ThunkArgCount == TargetArgCount + 1);
		ThunkArgTypeArray.Remove (0); 
		LlvmArg++;
	}

	for (; i < TargetArgCount; i++, LlvmArg++)
	{
		CValue ArgValue (LlvmArg, ThunkArgTypeArray [i]);
		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgTypeArray [i]);
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	CValue ReturnValue;
	m_pModule->m_LlvmBuilder.CreateCall (
		pThunk->m_pTargetFunction, 
		pThunk->m_pTargetFunction->GetType (),
		ArgArray,
		ArgArray.GetCount (),
		&ReturnValue
		);

	return pFunction->GetType ()->GetReturnType ()->GetTypeKind () != EType_Void ?
		m_pModule->m_ControlFlowMgr.Return (ReturnValue) :
		m_pModule->m_ControlFlowMgr.Return ();
}

bool
CFunctionMgr::CompileClosureThunk (CThunk* pThunk)
{
	ASSERT (pThunk->m_pClosureType);

	bool Result;

	rtl::CArrayT <CType*> TargetArgTypeArray = pThunk->m_pTargetFunctionType->GetArgTypeArray ();
	rtl::CArrayT <CType*> ThunkArgTypeArray = pThunk->m_pThunkFunction->GetType ()->GetArgTypeArray ();
	rtl::CArrayT <size_t> ClosureMap = pThunk->m_ClosureMap;

	size_t TargetArgCount = TargetArgTypeArray.GetCount ();
	size_t ThunkArgCount = ThunkArgTypeArray.GetCount ();
	size_t ClosureArgCount = pThunk->m_ClosureMap.GetCount ();

	char Buffer [256];
	rtl::CArrayT <CValue> ArgArray (ref::EBuf_Stack, Buffer, sizeof (Buffer));
	ArgArray.SetCount (TargetArgCount);

	CFunction* pFunction = pThunk->m_pThunkFunction;

	m_pCurrentFunction = pFunction;
	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();

	CValue ClosureValue (LlvmArg++, pThunk->m_pClosureType->GetClassPtrType ());

	CStructType* pClosureFieldStructType = pThunk->m_pClosureType->GetFieldStructType ();
	ASSERT (pClosureFieldStructType);

	rtl::CIteratorT <CStructMember> ClosureMember = pClosureFieldStructType->GetMemberList ().GetHead ();

	CValue PfnValue;

	if (pThunk->m_pTargetFunction)
	{
		PfnValue = pThunk->m_pTargetFunction;
	}
	else
	{
		Result = m_pModule->m_OperatorMgr.GetClassFieldMemberValue (ClosureValue, *ClosureMember, &PfnValue);
		if (!Result)
			return false;

		ClosureMember++;
	}
	
	size_t i = 0;
	size_t iThunk = 1; // skip ClosureValue arg
	size_t iClosure = 0;

	for (; i < TargetArgCount && iClosure < ClosureArgCount; i++)
	{
		CValue ArgValue;

		if (i == pThunk->m_ClosureMap [iClosure])
		{		
			Result = m_pModule->m_OperatorMgr.GetClassFieldMemberValue (ClosureValue, *ClosureMember, &ArgValue);

			if (!Result)
				return false;

			ClosureMember++;
			iClosure++;
		}
		else
		{
			ArgValue = CValue (LlvmArg, ThunkArgTypeArray [iThunk]);
			LlvmArg++;
			iThunk++;
		}

		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgTypeArray [i]);
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	for (; i < TargetArgCount; i++, iThunk, LlvmArg++)
	{
		CValue ArgValue (LlvmArg, ThunkArgTypeArray [iThunk]);

		Result = m_pModule->m_OperatorMgr.CastOperator (&ArgValue, TargetArgTypeArray [i]);
		if (!Result)
			return false;

		ArgArray [i] = ArgValue;
	}	

	CValue ReturnValue;

	m_pModule->m_LlvmBuilder.CreateCall (
		PfnValue, 
		pThunk->m_pTargetFunctionType,
		ArgArray,
		ArgArray.GetCount (),
		&ReturnValue
		);

	return pFunction->GetType ()->GetReturnType ()->GetTypeKind () != EType_Void ?
		m_pModule->m_ControlFlowMgr.Return (ReturnValue) :
		m_pModule->m_ControlFlowMgr.Return ();
}

bool
CFunctionMgr::CompileAutoPropertyAccessors (CProperty* pProperty)
{
	CFunction* pGetter = pProperty->GetGetter ();
	CFunction* pSetter = pProperty->GetSetter ();

	err::SetFormatStringError (_T("auto-properties are not implemented yet"));
	return false;

/*	if (pType->GetParentClassType ())
	{
		err::SetFormatStringError (_T("member auto-properties are not implemented yet"));
		return false;
	}

	ASSERT (pType->m_pAutoVariable);

	// save

	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	// getter

	m_pCurrentFunction = pGetter;

	pGetter->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pGetter->m_pBlock);
	m_pModule->m_ControlFlowMgr.Return (pType->m_pAutoVariable);
	
	// setter

	m_pCurrentFunction = pSetter;

	pSetter->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSetter->m_pBlock);
	
	llvm::Function::arg_iterator LlvmArg = pSetter->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, NULL);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue1, pType->m_pAutoVariable);
	m_pModule->m_ControlFlowMgr.Return ();

	// restore

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;
*/
	return true;
}

bool
CFunctionMgr::JitFunctions (llvm::ExecutionEngine* pExecutionEngine)
{
/*	err::SetFormatStringError (_T("cancel LLVM jitting for now"));
	return false;
	*/
	CSetCurrentThreadModule ScopeModule (m_pModule);
	llvm::ScopedFatalErrorHandler ScopeErrorHandler (LlvmFatalErrorHandler);

	rtl::CIteratorT <CFunction> Function = m_FunctionList.GetHead ();
	for (; Function; Function++)
	{
		CFunction* pFunction = *Function;

		if (!pFunction->HasBody ())
			continue;

		try 
		{
			pFunction->m_pfn = pExecutionEngine->getPointerToFunction (pFunction->GetLlvmFunction ());
		}
		catch (err::CError Error)
		{
			err::SetFormatStringError (
				_T("LLVM jitting fail for '%s': %s"), 
				pFunction->m_Tag,
				Error->GetDescription ()
				);

			return false;
		}

		ASSERT (pFunction->m_pfn);
	}

	return true;
}

CFunction*
CFunctionMgr::GetStdFunction (EStdFunc Func)
{
	ASSERT (Func >= 0 && Func < EStdFunc__Count);

	if (m_StdFunctionArray [Func])
		return m_StdFunctionArray [Func];

	CFunction* pFunction;

	switch (Func)
	{
	case EStdFunc_OnRuntimeError:
		pFunction = CreateOnRuntimeError ();
		break;

	case EStdFunc_CheckNullPtr:
		pFunction = CreateCheckNullPtr ();
		break;

	case EStdFunc_CheckScopeLevel:
		pFunction = CreateCheckScopeLevel ();
		break;

	case EStdFunc_CheckDataPtrRange:
		pFunction = CreateCheckDataPtrRange ();
		break;

	case EStdFunc_CheckClassPtrScopeLevel:
		pFunction = CreateCheckClassPtrScopeLevel ();
		break;

	case EStdFunc_DynamicCastInterface:
		pFunction = CreateDynamicCastInterface ();
		break;

	case EStdFunc_EventOperator:
		pFunction = CreateEventOperator ();
		break;

	case EStdFunc_FireSimpleEvent:
		pFunction = CreateFireSimpleEvent ();
		break;

	case EStdFunc_HeapAllocate:
		pFunction = CreateHeapAllocate ();
		break;

	default:
		ASSERT (false);
		pFunction = NULL;
	}

	m_StdFunctionArray [Func] = pFunction;
	return pFunction;
}

CFunction*
CFunctionMgr::CreateInternalFunction (
	const tchar_t* pTag,
	CFunctionType* pType 
	)
{
	CFunction* pFunction = CreateFunction (EFunction_Internal, pType);
	pFunction->m_Tag = pTag;
	return pFunction;
}

// void
// jnc.OnRuntimeError (
//		int Error,
//		int8* pCodeAddr,
//		int8* pDataAddr
//		);

CFunction*
CFunctionMgr::CreateOnRuntimeError ()
{
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (_T("jnc.OnRuntimeError"), pType);
}

// void 
// jnc.CheckNullPtr (
//		int8* p,
//		int Error
//		);

CFunction*
CFunctionMgr::CreateCheckNullPtr ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (_T("jnc.CheckNullPtr"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [0]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("iface_success"));
	
	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	RuntimeError (ArgValue2, ArgValue1);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}


// void 
// jnc.CheckDataPtrScopeLevel (
//		size_t SrcScopeLevel
//		size_t DstScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCheckScopeLevel ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (_T("jnc.CheckScopeLevel"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_success"));
	
	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateGt_u (ArgValue1, ArgValue2, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();
	RuntimeError (ERuntimeError_ScopeMismatch, NullValue);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void 
// jnc.CheckDataPtrRange (
//		int8* p,
//		size_t Size,
//		jnc.sptrv Validator,
//		int Error
//		);

CFunction*
CFunctionMgr::CreateCheckDataPtrRange ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
		m_pModule->m_TypeMgr.GetStdType (EStdType_DataPtrValidator),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int)
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (_T("jnc.CheckDataPtrRange"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);
	CValue ArgValue3 (LlvmArg++, ArgTypeArray [2]);
	CValue ArgValue4 (LlvmArg++, ArgTypeArray [3]);

	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_success"));
	CBasicBlock* pCmp2Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_cmp2"));
	CBasicBlock* pCmp3Block = m_pModule->m_ControlFlowMgr.CreateBlock (_T("sptr_cmp3"));
	
	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	CValue CmpValue;
	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pCmp2Block, pCmp2Block);

	CValue RegionBeginValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue3, 0, NULL, &RegionBeginValue);
	m_pModule->m_LlvmBuilder.CreateLt_u (ArgValue1, RegionBeginValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pCmp3Block, pCmp3Block);	

	CValue PtrEndValue;
	CValue RegionEndValue;
	m_pModule->m_LlvmBuilder.CreateExtractValue (ArgValue3, 1, NULL, &RegionEndValue);
	m_pModule->m_LlvmBuilder.CreateGep (ArgValue1, ArgValue2, NULL ,&PtrEndValue);
	m_pModule->m_LlvmBuilder.CreateGt_u (PtrEndValue, RegionEndValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	RuntimeError (ArgValue4, ArgValue1);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// void 
// jnc.CheckClassPtrScopeLevel (
//		object p,
//		size_t DstScopeLevel
//		);

CFunction*
CFunctionMgr::CreateCheckClassPtrScopeLevel ()
{
	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT),
	};
	
	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (NULL, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (_T("jnc.CheckClassPtrScopeLevel"), pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);

	CBasicBlock* pNoNullBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_nonull"));
	CBasicBlock* pFailBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_fail"));
	CBasicBlock* pSuccessBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("scope_success"));

	CValue CmpValue;

	CValue NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr)->GetZeroValue ();

	m_pModule->m_LlvmBuilder.CreateEq_i (ArgValue1, NullValue, &CmpValue);
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pSuccessBlock, pNoNullBlock, pNoNullBlock);

	static size_t LlvmIndexArray [] = 
	{
		0, // TInterfaceHdr**
		0, // TInterfaceHdr* 
		1, // TObject** 
	};

	CValue ObjPtrValue;
	m_pModule->m_LlvmBuilder.CreateGep (ArgValue1, LlvmIndexArray, countof (LlvmIndexArray), NULL, &ObjPtrValue); // TObject** ppObject
	m_pModule->m_LlvmBuilder.CreateLoad (ObjPtrValue, NULL, &ObjPtrValue);  // TObject* pObject
	
	CValue SrcScopeLevelValue;
	m_pModule->m_LlvmBuilder.CreateGep2 (ObjPtrValue, 1, NULL, &SrcScopeLevelValue);     // size_t* pScopeLevel
	m_pModule->m_LlvmBuilder.CreateLoad (SrcScopeLevelValue, NULL, &SrcScopeLevelValue); // size_t ScopeLevel

	m_pModule->m_LlvmBuilder.CreateGt_u (SrcScopeLevelValue, ArgValue2, &CmpValue); // SrcScopeLevel > DstScopeLevel
	m_pModule->m_ControlFlowMgr.ConditionalJump (CmpValue, pFailBlock, pSuccessBlock);

	NullValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();
	RuntimeError (ERuntimeError_ScopeMismatch, NullValue);

	m_pModule->m_ControlFlowMgr.Follow (pSuccessBlock);
	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

// object
// jnc.DynamicCastInterface (
//		object p,
//		int8* pType
//		);

CFunction*
CFunctionMgr::CreateDynamicCastInterface ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (_T("jnc.DynamicCastInterface"), pType);
}

// void
// jnc.EventOperator (
//		jnc.event* pEvent,
//		void* pfn,
//		object pIface,
//		int OpKind
//		);

CFunction*
CFunctionMgr::CreateEventOperator ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleEvent)->GetDataPtrType (EDataPtrType_Unsafe),
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
		m_pModule->m_TypeMgr.GetStdType (EStdType_ObjectPtr),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_Int),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (_T("jnc.EventOperator"), pType);
}

// void
// jnc.FireSimpleEvent (jnc.event* pEvent);

CFunction*
CFunctionMgr::CreateFireSimpleEvent ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleEvent)->GetDataPtrType (EDataPtrType_Unsafe),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (_T("jnc.FireSimpleEventOperator"), pType);
}

// int8*
// jnc.HeapAllocate (int8* pType);

CFunction*
CFunctionMgr::CreateHeapAllocate ()
{
	CType* pReturnType = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr);
	
	CType* ArgTypeArray [] =
	{
		m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr),
	};

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	return CreateInternalFunction (_T("jnc.HeapAllocate"), pType);
}

CFunction* 
CFunctionMgr::CreateClassInitializer (CClassType* pClassType)
{
	CValue VTablePtrValue;
	bool Result = pClassType->GetVTablePtrValue (&VTablePtrValue);
	if (!Result)
		return NULL;

	CFunction* pPrevCurrentFunction = m_pCurrentFunction;
	CBasicBlock* pPrevCurrentBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	
	CType* ArgTypeArray [] =
	{
		pClassType->GetClassStructType ()->GetDataPtrType (EDataPtrType_Unsafe),
		m_pModule->m_TypeMgr.GetPrimitiveType (EType_SizeT)
	};

	rtl::CString Tag;
	Tag = pClassType->m_Tag;
	Tag.Append (_T(".$init"));

	CFunctionType* pType = m_pModule->m_TypeMgr.GetFunctionType (pReturnType, ArgTypeArray, countof (ArgTypeArray));
	CFunction* pFunction = CreateInternalFunction (Tag, pType);

	m_pCurrentFunction = pFunction;

	pFunction->m_pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("function_entry"));

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pFunction->m_pBlock);

	llvm::Function::arg_iterator LlvmArg = pFunction->GetLlvmFunction ()->arg_begin();
	
	CValue ArgValue1 (LlvmArg++, ArgTypeArray [0]);
	CValue ArgValue2 (LlvmArg++, ArgTypeArray [1]);

	CValue TypeValue (&pClassType, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr));
	CValue ObjectPtrValue;
	CValue IfacePtrValue;
	CValue PtrValue;

	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 0, NULL, &ObjectPtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (ArgValue1, 1, NULL, &IfacePtrValue);

	// store CClassType*

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (TypeValue, PtrValue);

	// store ScopeLevel

	m_pModule->m_LlvmBuilder.CreateGep2 (ObjectPtrValue, 1, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ArgValue2, PtrValue);

	InitializeInterface (pClassType, ObjectPtrValue, IfacePtrValue, VTablePtrValue);

	CFunction* pPreConstructor = pClassType->GetPreConstructor ();
	if (pPreConstructor)
		m_pModule->m_LlvmBuilder.CreateCall (pPreConstructor, pPreConstructor->GetType (), IfacePtrValue, NULL);

	m_pModule->m_ControlFlowMgr.Return ();

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pPrevCurrentBlock);
	m_pCurrentFunction = pPrevCurrentFunction;

	return pFunction;
}

bool
CFunctionMgr::InitializeInterface (
	CClassType* pClassType,
	const CValue& ObjectPtrValue,
	const CValue& IfacePtrValue,
	const CValue& VTablePtrValue
	)
{
	CValue IfaceHdrPtrValue;
	CValue PtrValue;

	// TInterface

	m_pModule->m_LlvmBuilder.CreateGep2 (IfacePtrValue, 0, NULL, &IfaceHdrPtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (IfaceHdrPtrValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (VTablePtrValue, PtrValue);
	m_pModule->m_LlvmBuilder.CreateGep2 (IfaceHdrPtrValue, 1, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateStore (ObjectPtrValue, PtrValue);

	// base types

	rtl::CIteratorT <CClassBaseType> BaseType = pClassType->GetBaseTypeList ().GetHead ();
	for (; BaseType; BaseType++)
	{
		CClassType* pBaseClassType = BaseType->GetType ();
		CValue BaseClassPtrValue;
		CValue BaseClassVTablePtrValue;

		m_pModule->m_LlvmBuilder.CreateGep2 (
			IfacePtrValue, 
			BaseType->GetFieldBaseType ()->GetLlvmIndex (), 
			NULL, 
			&BaseClassPtrValue
			);

		m_pModule->m_LlvmBuilder.CreateGep2 (
			VTablePtrValue, 
			BaseType->GetVTableIndex (), 
			NULL, 
			&BaseClassVTablePtrValue
			);

		m_pModule->m_LlvmBuilder.CreateBitCast (
			BaseClassVTablePtrValue, 
			pBaseClassType->GetVTableStructType ()->GetDataPtrType (EDataPtrType_Unsafe),
			&BaseClassVTablePtrValue
			);

		InitializeInterface (pBaseClassType, ObjectPtrValue, BaseClassPtrValue, BaseClassVTablePtrValue);
	}
	
	return true;
}

bool
CFunctionMgr::CreateThisValue (
	const CValue& ArgValue,
	CClassType* pResultType,
	CValue* pResultValue
	)
{
	ASSERT (ArgValue.GetType ()->GetTypeKind () == EType_ClassPtr);
	CClassType* pSrcType = ((CClassPtrType*) ArgValue.GetType ())->GetTargetType ();

	// adjust the pointer: argument is the pointer to the interface which DECLARES the method
	// what we want as 'this' value is the pointer to the class which IMPLEMENTS the method

	CClassBaseTypeCoord Coord;
	intptr_t Offset = 0;

	if (pResultType != pSrcType)
	{
		bool Result = pResultType->FindBaseType (pSrcType, &Coord);
		ASSERT (Result);

		Offset = -(intptr_t) Coord.m_FieldCoord.m_Offset;
	}

	if (!Offset)
	{
		m_pModule->m_LlvmBuilder.CreateBitCast (ArgValue, pResultType, pResultValue);
		return true;
	}

	CValue PtrValue;
	m_pModule->m_LlvmBuilder.CreateBitCast (ArgValue, m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr), &PtrValue);
	m_pModule->m_LlvmBuilder.CreateGep (PtrValue, Offset, NULL, &PtrValue);
	m_pModule->m_LlvmBuilder.CreateBitCast (PtrValue, pResultType, pResultValue);
	return true;
}

bool
CFunctionMgr::RuntimeError (
	const CValue& ErrorValue,
	const CValue& DataAddrValue
	)
{
	CFunction* pOnRuntimeError = GetStdFunction (EStdFunc_OnRuntimeError);

	// TODO: calc real code address

	CValue CodeAddrValue = m_pModule->m_TypeMgr.GetStdType (EStdType_BytePtr)->GetZeroValue ();

	m_pModule->m_LlvmBuilder.CreateCall3 (
		pOnRuntimeError, 
		pOnRuntimeError->GetType (),
		ErrorValue, 
		CodeAddrValue,
		DataAddrValue,
		NULL
		);

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

