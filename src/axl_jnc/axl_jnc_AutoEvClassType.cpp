#include "pch.h"
#include "axl_jnc_AutoEvClassType.h"
#include "axl_jnc_Module.h"
#include "axl_jnc_Parser.llk.h"

namespace axl {
namespace jnc {

//.............................................................................

CAutoEvClassType::CAutoEvClassType ()
{
	m_ClassTypeKind = EClassType_AutoEv;
	m_BindSiteCount = 0;
	memset (m_FieldArray, 0, sizeof (m_FieldArray));
	memset (m_MethodArray, 0, sizeof (m_MethodArray));
}

CFunction*
CAutoEvClassType::CreateHandler ()
{
	return CreateUnnamedMethod (
		EStorage_Member,
		EFunction_AutoEvHandler,
		(CFunctionType*) (CType*) m_pModule->GetSimpleType (EStdType_SimpleFunction)
		);
}

bool
CAutoEvClassType::SetBody (rtl::CBoxListT <CToken>* pTokenList)
{
	if (!m_Body.IsEmpty ())
	{
		err::SetFormatStringError ("'%s' already has a body", m_Tag.cc ());
		return false;
	}

	m_Body.TakeOver (pTokenList);
	m_pModule->MarkForCompile (this);
	return true;
}

bool
CAutoEvClassType::CalcLayout ()
{
	bool Result;

	if (m_Body.IsEmpty ())
	{
		err::SetFormatStringError ("autoev '%s' has no body", m_Tag.cc ()); // thanks a lot gcc
		return false;
	}

	// scan

	CParser Parser;
	Parser.m_Stage = CParser::EStage_AutoEvScan;
	Parser.m_pModule = m_pModule;
	Parser.m_pAutoEvType = this;

	m_pModule->m_NamespaceMgr.OpenNamespace (this);

	Result = Parser.ParseTokenList (ESymbol_autoev_body_0, m_Body, false);
	if (!Result)
		return false;

	m_pModule->m_NamespaceMgr.CloseNamespace ();

	ASSERT (Parser.m_AutoEvBindSiteTotalCount);
	m_BindSiteCount = Parser.m_AutoEvBindSiteTotalCount;

	CType* pBindSiteType = m_pModule->m_TypeMgr.GetStdType (EStdType_AutoEvBindSite);
	CArrayType* pBindSiteArrayType = pBindSiteType->GetArrayType (m_BindSiteCount);
	m_FieldArray [EAutoEvField_BindSiteArray] = CreateField (pBindSiteArrayType);
	
	Result = CClassType::CalcLayout ();
	if (!Result)
		return false;

	return true;
}

bool
CAutoEvClassType::CompileConstructor ()
{
	ASSERT (m_pConstructor);

	bool Result;

	size_t ArgCount = m_pConstructor->GetType ()->GetArgArray ().GetCount ();
	ASSERT (ArgCount == 1 || ArgCount == 2);
	
	CValue ArgValueArray [2];
	m_pModule->m_FunctionMgr.InternalPrologue (m_pConstructor, ArgValueArray, ArgCount);

	if (ArgCount == 2)
	{
		CStructField* pField = m_FieldArray [EAutoEvField_Parent];
		ASSERT (pField);
		
		CValue ParentFieldValue;
		Result = 
			m_pModule->m_OperatorMgr.GetClassField (ArgValueArray [0], pField, NULL, &ParentFieldValue) &&
			m_pModule->m_OperatorMgr.StoreDataRef (ParentFieldValue, ArgValueArray [1]);

		if (!Result)
			return false;
	}

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool
CAutoEvClassType::CompileDestructor ()
{
	ASSERT (m_pDestructor);

	bool Result;

	CValue ArgValue;
	m_pModule->m_FunctionMgr.InternalPrologue (m_pDestructor, &ArgValue, 1);

	Result = CallStopMethod ();
	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.InternalEpilogue ();
	return true;
}

bool
CAutoEvClassType::CallStopMethod ()
{
	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	ASSERT (ThisValue);

	CValue StopMethodValue = m_MethodArray [EAutoEvMethod_Stop];
	StopMethodValue.InsertToClosureHead (ThisValue);
	return m_pModule->m_OperatorMgr.CallOperator (StopMethodValue);
}

bool
CAutoEvClassType::CompileStartMethod ()
{
	bool Result;

	CFunction* pStartMethod = m_MethodArray [EAutoEvMethod_Start];
	CFunction* pStopMethod = m_MethodArray [EAutoEvMethod_Stop];

	Result = m_pModule->m_FunctionMgr.Prologue (pStartMethod, m_Body.GetHead ()->m_Pos);
	if (!Result)
		return false;

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	ASSERT (ThisValue);

	// stop

	Result = CallStopMethod ();
	if (!Result)
		return false;

	// save arguments

	rtl::CArrayT <CFunctionArg*> ArgArray = pStartMethod->GetType ()->GetArgArray ();
	size_t ArgCount = ArgArray.GetCount ();
	size_t i = 1;

	rtl::CIteratorT <CStructField> ArgField = m_FirstArgField;
	llvm::Function::arg_iterator LlvmArg = pStartMethod->GetLlvmFunction ()->arg_begin();
	LlvmArg++;

	for (; i < ArgCount; i++, LlvmArg++, ArgField++)
	{
		CFunctionArg* pArg = ArgArray [i];
		llvm::Value* pLlvmArg = LlvmArg;
		CStructField* pArgField = *ArgField;

		if (!pArg->IsNamed ())
			continue;

		CValue ArgValue (pLlvmArg, pArg->GetType ());

		CValue StoreValue;
		Result = m_pModule->m_OperatorMgr.GetField (ThisValue, pArgField, NULL, &StoreValue);
		if (!Result)
			return false;
			
		m_pModule->m_LlvmBuilder.CreateStore (ArgValue, StoreValue);
	}

	// compile start

	CParser Parser;
	Parser.m_Stage = CParser::EStage_Pass2;
	Parser.m_pModule = m_pModule;
	Parser.m_pAutoEvType = this;

	Result = Parser.ParseTokenList (ESymbol_autoev_body, m_Body, true);
	if (!Result)
		return false;

	// modify state

	CValue StateValue;
	Result = 
		m_pModule->m_OperatorMgr.GetField (ThisValue, m_FieldArray [EAutoEvField_State], NULL, &StateValue) &&
		m_pModule->m_OperatorMgr.StoreDataRef (StateValue, CValue ((int64_t) 1, EType_Int_p));

	if (!Result)
		return false;

	// done

	Result = m_pModule->m_FunctionMgr.Epilogue (m_Body.GetTail ()->m_Pos);
	if (!Result)
		return false;

	return true;
}

bool
CAutoEvClassType::BindHandlers (const rtl::CConstListT <TAutoEvHandler>& HandlerList)
{
	bool Result;

	CStructType* pBindSiteType = (CStructType*) m_pModule->m_TypeMgr.GetStdType (EStdType_AutoEvBindSite);
	CStructField* pEventPtrField = *pBindSiteType->GetFieldList ().GetHead ();
	CStructField* pCookieField = *pBindSiteType->GetFieldList ().GetTail ();

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	ASSERT (ThisValue);

	CValue BindSiteArrayValue;
	Result = m_pModule->m_OperatorMgr.GetField (
		ThisValue,
		m_FieldArray [EAutoEvField_BindSiteArray], 
		NULL, 
		&BindSiteArrayValue
		);

	if (!Result)
		return false;

	rtl::CIteratorT <TAutoEvHandler> Handler = HandlerList.GetHead ();
	size_t i = 0;
	for (; Handler; Handler++)
	{
		CFunction* pFunction = Handler->m_pFunction;

		rtl::CBoxIteratorT <CValue> Value = Handler->m_BindSiteList.GetHead (); 
		for (; Value; Value++, i++)
		{
			CValue OnChangeValue;
			Result = m_pModule->m_OperatorMgr.GetPropertyOnChange (*Value, &OnChangeValue);
			if (!Result)
				return false;
	
			CValue HandlerValue = pFunction;
			HandlerValue.InsertToClosureHead (ThisValue);

			CValue IdxValue (i, EType_SizeT);
			CValue AddMethodValue;
			CValue CookieValue;
			CValue BindSiteValue;
			CValue DstOnChangeValue;
			CValue DstCookieValue;

			Result = 
				m_pModule->m_OperatorMgr.MemberOperator (OnChangeValue, "Add", &AddMethodValue) &&
				m_pModule->m_OperatorMgr.CallOperator (AddMethodValue, HandlerValue, &CookieValue) &&
				m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, BindSiteArrayValue, IdxValue, &BindSiteValue) &&
				m_pModule->m_OperatorMgr.GetStructField (BindSiteValue, pEventPtrField, NULL, &DstOnChangeValue) &&
				m_pModule->m_OperatorMgr.GetStructField (BindSiteValue, pCookieField, NULL, &DstCookieValue) &&
				m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, &OnChangeValue) &&
				m_pModule->m_OperatorMgr.StoreDataRef (DstOnChangeValue, OnChangeValue) &&
				m_pModule->m_OperatorMgr.StoreDataRef (DstCookieValue, CookieValue);

			if (!Result)
				return false;
		}
	}

	ASSERT (i == m_BindSiteCount);
	return true;
}

bool
CAutoEvClassType::CompileStopMethod ()
{
	bool Result;

	CStructType* pBindSiteType = (CStructType*) m_pModule->m_TypeMgr.GetStdType (EStdType_AutoEvBindSite);
	CStructField* pEventPtrField = *pBindSiteType->GetFieldList ().GetHead ();
	CStructField* pCookieField = *pBindSiteType->GetFieldList ().GetTail ();

	m_pModule->m_FunctionMgr.InternalPrologue (m_MethodArray [EAutoEvMethod_Stop]);

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	ASSERT (ThisValue);

	CBasicBlock* pUnadviseBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("unadvise_block");
	CBasicBlock* pFollowBlock = m_pModule->m_ControlFlowMgr.CreateBlock ("follow_block");

	CValue StateValue; 
	CValue StateCmpValue; 
	CValue BindSiteArrayValue;

	Result = 
		m_pModule->m_OperatorMgr.GetField (ThisValue, m_FieldArray [EAutoEvField_State], NULL, &StateValue) &&
		m_pModule->m_ControlFlowMgr.ConditionalJump (StateValue, pUnadviseBlock, pFollowBlock);

	if (!Result)
		return false;

	Result = m_pModule->m_OperatorMgr.GetField (ThisValue, m_FieldArray [EAutoEvField_BindSiteArray], NULL, &BindSiteArrayValue);
	if (!Result)
		return false;

	for (size_t i = 0; i < m_BindSiteCount; i++)
	{
		CValue IdxValue (i, EType_SizeT);
		CValue BindSiteValue;
		CValue OnChangeValue;
		CValue CookieValue;
		CValue RemoveMethodValue;

		Result = 
			m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_Idx, BindSiteArrayValue, IdxValue, &BindSiteValue) &&
			m_pModule->m_OperatorMgr.GetStructField (BindSiteValue, pEventPtrField, NULL, &OnChangeValue) &&
			m_pModule->m_OperatorMgr.GetStructField (BindSiteValue, pCookieField, NULL, &CookieValue) &&
			m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Indir, &OnChangeValue) &&
			m_pModule->m_OperatorMgr.MemberOperator (OnChangeValue, "Remove", &RemoveMethodValue) &&
			m_pModule->m_OperatorMgr.CallOperator (RemoveMethodValue, CookieValue);

		if (!Result)
			return false;
	}

	Result = m_pModule->m_OperatorMgr.StoreDataRef (StateValue, CValue ((int64_t) 0, EType_Int_p));
	ASSERT (Result);
	
	m_pModule->m_ControlFlowMgr.Follow (pFollowBlock);

	m_pModule->m_FunctionMgr.InternalEpilogue ();

	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

/*

//.............................................................................

bool
CParser::DeclareAutoEv (
	CAutoEvClassType* pType,
	CDeclarator* pDeclarator
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();
	EDeclarator DeclaratorKind = pDeclarator->GetDeclaratorKind ();

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError ("invalid autoev declarator (qualified autoev not supported yet)");
		return false;
	}

	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		err::SetFormatStringError ("property templates cannot have autoev memberts");
		return false;
	}

	if (m_StorageKind && m_StorageKind != EStorage_Static)
	{
		err::SetFormatStringError ("invalid storage '%s' for autoev", GetStorageKindString (m_StorageKind));
		return false;
	}

	if (!m_StorageKind && NamespaceKind == ENamespace_Property)
		m_StorageKind = ((CProperty*) pNamespace)->GetStorageKind ();
		
	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);

	CAutoEv* pAutoEv = m_pModule->m_FunctionMgr.CreateAutoEv (Name, QualifiedName);
	AssignDeclarationAttributes (pAutoEv, pNamespace, pDeclarator->GetPos ());

	switch (NamespaceKind)
	{
	case ENamespace_TypeExtension:
		if (!pDeclarator->IsSimple ())
		{
			err::SetFormatStringError ("invalid declarator '%s' in type extension", pAutoEv->m_Tag.cc ());
			return false;
		}

		break;

	case ENamespace_Type:
		if (((CNamedType*) pNamespace)->GetTypeKind () != EType_Class)
		{
			err::SetFormatStringError ("method members are not allowed in '%s'", ((CNamedType*) pNamespace)->GetTypeString ().cc ());
			return false;
		}

		if (pDeclarator->IsQualified () && m_StorageKind != EStorage_Override)
		{
			err::SetFormatStringError ("only overrides could be qualified, '%s' is not an override", pAutoEv->m_Tag.cc ());
			return false;
		}

		Result = ((CClassType*) pNamespace)->AddAutoEv (pAutoEv);
		if (!Result)
			return false;

		break;

	case ENamespace_Property:
		if (pDeclarator->IsQualified ())
		{
			err::SetFormatStringError ("invalid qualified declarator '%s' in property", pAutoEv->m_Tag.cc ());
			return false;
		}

		Result = ((CProperty*) pNamespace)->AddAutoEv (pAutoEv);
		if (!Result)
			return false;

		break;
	
	default:
		if (m_StorageKind)
		{
			err::SetFormatStringError ("invalid storage specifier '%s' for a global AutoEv", GetStorageKindString (m_StorageKind));
			return false;
		}

		pAutoEv->m_StorageKind = EStorage_Static;

		Result = pNamespace->AddItem (pAutoEv);
		if (!Result)
			return false;
	}

	return pAutoEv->Create (pType);
}

CClassType*
CParser::CreateAutoEvClassType (
	const rtl::CString& Name,
	CDeclFunctionSuffix* pFunctionSuffix,
	rtl::CBoxListT <CToken>* pTokenList
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
	CClassType* pClassType = m_pModule->m_TypeMgr.CreateClassType (EClassType_AutoEv, Name, QualifiedName);

	Result = pNamespace->AddItem (pClassType);
	if (!Result)
		return NULL;

	AssignDeclarationAttributes (pClassType, pNamespace, m_LastMatchedToken.m_Pos);

	CFunctionType* pStartMethodType = pFunctionSuffix ? m_pModule->m_TypeMgr.GetFunctionType (
		NULL, 
		pFunctionSuffix->GetArgArray (),
		pFunctionSuffix->GetFunctionTypeFlags ()
		) : 
		(CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);

	CAutoEvClassType* pAutoEvType = m_pModule->m_TypeMgr.GetAutoEvType (pStartMethodType);	

	CAutoEv* pAutoEv = m_pModule->m_FunctionMgr.CreateUnnamedAutoEv ();
	
	Result = 
		pClassType->AddAutoEv (pAutoEv) &&
		pAutoEv->Create (pAutoEvType);

	if (!Result)
		return NULL;

	if (pTokenList)
		pAutoEv->SetBody (pTokenList);

	return pClassType;
}

//.............................................................................

*/
