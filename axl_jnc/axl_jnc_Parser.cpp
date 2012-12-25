#include "stdafx.h"
#include "axl_jnc_Parser.h"

namespace axl {
namespace jnc {

//.............................................................................

CParser::CParser ()
{
	m_pModule = GetCurrentThreadModule ();
	m_StructPackFactor = 8;
	m_DefaultStructPackFactor = 8;
	m_Endianness = EEndianness_LittleEndian;
	m_Stage = EStage_Pass1;
	m_pThisType = NULL;
}

bool
CParser::IsTypeSpecified ()
{
	if (m_TypeSpecifierStack.IsEmpty ())
		return false;

	CTypeSpecifier* pTypeSpecifier = m_TypeSpecifierStack.GetBack ();
	return pTypeSpecifier->GetType () != NULL;
}

CType*
CParser::FindType (const CQualifiedName& Name)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	CModuleItem* pItem;

	if (m_Stage == EStage_Pass1)
	{
		pItem = pNamespace->FindItem (Name.GetShortName ());
		if (!pItem)
			return m_pModule->m_TypeMgr.GetImportType (Name, pNamespace);				
	}
	else
	{	
		pItem = pNamespace->FindItemTraverse (Name, false);
		if (!pItem)
			return NULL;
	}

	pItem = UnAliasItem (pItem);

	return pItem->GetItemKind () == EModuleItem_Type ? (CType*) pItem : NULL;
}

CModuleItem*
CParser::Declare (
	CDeclSpecifiers* pDeclSpecifiers,
	CDeclarator* pDeclarator,
	CClassType* pClassType,
	size_t BitCount,
	bool HasInitializer
	)
{
	bool Result;

	CType* pType = pDeclarator->GetType (pDeclSpecifiers);
	if (!pType)
		return NULL;

	EStorageClass StorageClass = pDeclSpecifiers->GetStorageClass ();
	EType TypeKind = pType->GetTypeKind ();

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	rtl::CString Name;

	CModuleItem* pOldItem = NULL;
	CModuleItem* pNewItem = NULL;
	
	if (pDeclarator->IsSimple ())
	{
		Name = pDeclarator->GetName ()->GetShortName ();
		pOldItem = pNamespace->FindItem (Name);
	}
	else if (TypeKind != EType_Function || StorageClass == EStorageClass_Typedef)
	{
		err::SetFormatStringError (_T("qualified declarators are only allowed for functions"));
		return NULL;
	}

	if (StorageClass == EStorageClass_Typedef)
	{
		if (pType->GetTypeKind () == EType_Function)
		{
			CFunctionType* pFunctionType = (CFunctionType*) pType;
			if (pFunctionType->GetCallingConvention ())
				pType = pFunctionType->GetPointerType (EType_Pointer_u);
			else
				pType = pFunctionType->GetFunctionPointerType ();
		}
		else if (pType->GetTypeKind () == EType_Property)
		{
			CPropertyType* pPropertyType = (CPropertyType*) pType;
			pPropertyType->m_PropertyKind = EProperty_Pointer;
			pPropertyType->m_Name = Name;
			pPropertyType->m_pParentNamespace = pNamespace;
			pPropertyType->m_Tag = pPropertyType->GetQualifiedName ();
			pPropertyType->TagAccessors ();
			pType = pPropertyType->GetPropertyPointerType ();
		}

		if (pOldItem)
		{
			if (pOldItem->GetItemKind () != EModuleItem_Alias || ((CAlias*) pOldItem)->GetTarget () != pType)
			{	
				err::SetFormatStringError (_T("redefinition of '%s'"), Name);
				return NULL;
			}

			return pOldItem;
		}

		pNewItem = pNamespace->CreateAlias (Name, pType);
	}
	else if (TypeKind == EType_Function)
	{
		CFunctionType* pFunctionType = (CFunctionType*) pType;
		CFunction* pFunction;
		
		if (pOldItem)
		{
			if (pOldItem->GetItemKind () != EModuleItem_GlobalFunction)
			{	
				err::SetFormatStringError (_T("redefinition of '%s'"), Name);
				return NULL;
			}

			CGlobalFunction* pGlobalFunction = (CGlobalFunction*) pOldItem;
			pFunction = pGlobalFunction->FindOverload (pFunctionType);
			if (!pFunction)
			{
				pFunction = m_pModule->m_FunctionMgr.CreateFunction (
					*pDeclarator->GetName (),
					pFunctionType, 
					pDeclarator->GetArgList ()
					);

				pFunction->m_FunctionKind = EFunction_Global;

				Result = pGlobalFunction->AddOverload (pFunction);
				if (!Result)
					return NULL;
			}
		}
		else
		{
			pFunction = m_pModule->m_FunctionMgr.CreateFunction (
				*pDeclarator->GetName (),
				pFunctionType, 
				pDeclarator->GetArgList ()
				);

			EPropertyAccessor AccessorKind = pDeclarator->GetPropertyAccessorKind ();
			if (AccessorKind)
			{
				pFunction->m_PropertyAccessorKind = AccessorKind;
				pFunction->m_Tag.AppendFormat (_T(".%s"), GetPropertyAccessorString (AccessorKind));
			}

			if (pClassType)
			{
				if (!pDeclarator->GetName ()->IsSimple ())
				{
					err::SetFormatStringError (_T("invalid class method member name '%s'"), pFunction->GetTag ());
					return NULL;
				}

				if (!pDeclarator->GetPropertyAccessorKind ())
				{
					Result = pClassType->CreateMethodMember (Name, pFunction) != NULL;
					if (!Result)
						return NULL;
				}
				else 
				{
					pClassType->AddMethodFunction (pFunction);
				}

			}
			else if (pDeclarator->IsSimple ())
			{
				CGlobalFunction* pGlobalFunction = m_pModule->m_FunctionMgr.CreateGlobalFunction (Name, pFunction);
				pNamespace->AddItem (pGlobalFunction);
			}
		}

		pNewItem = pFunction;
	}
	else if (TypeKind == EType_Property)
	{
		if (pOldItem)
		{	
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return NULL;
		}

		CPropertyType* pPropertyType = (CPropertyType*) pType;

		if (pClassType)
		{
			pNewItem = pClassType->CreatePropertyMember (Name, pPropertyType);
		}
		else
		{
			pPropertyType->m_PropertyKind = EProperty_Global;
			pPropertyType->m_Name = Name;
			pNamespace->AddItem (pPropertyType);
			pPropertyType->m_Tag = pPropertyType->GetQualifiedName ();
			pPropertyType->TagAccessors ();

			pNewItem = pPropertyType;
		}
	}
	else
	{
		if (pOldItem)
		{	
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return NULL;
		}

		if (pClassType)
		{
			pNewItem = pClassType->CreateFieldMember (Name, pType, BitCount);
		}
		else
		{
			CVariable* pVariable = m_pModule->m_VariableMgr.CreateVariable (Name, pType, HasInitializer);
			pNamespace->AddItem (pVariable);
			pNewItem = pVariable;
		}
	}

	if (!pNewItem)
		return NULL;

	m_pModule->m_AttributeMgr.AssignAttributeSet (pNewItem);
	pNewItem->m_Pos = pDeclarator->m_Pos;
	return pNewItem;
}


CFunction*
CParser::DeclarePropertyAccessor (
	CPropertyType* pPropertyType,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator
	)
{
	bool Result; 

	CType* pType = pDeclarator->GetType (pTypeSpecifier);
	if (!pType)
		return NULL;

	if (pType->GetTypeKind () != EType_Function)
	{
		err::SetFormatStringError (_T("illegal property accessor type '%s'"), pType->GetTypeString ());
		return NULL;
	}

	CFunctionType* pFunctionType = (CFunctionType*) pType;
	EPropertyAccessor PropertyAccessorKind = pDeclarator->GetPropertyAccessorKind ();
	
	CFunction* pAccessor = m_pModule->m_FunctionMgr.CreatePropertyAccessorFunction (
		PropertyAccessorKind, 
		pFunctionType, 
		pDeclarator->GetArgList ()
		);

	switch (PropertyAccessorKind)
	{
	case EPropertyAccessor_Get:
		if (pPropertyType->m_pGetter)
		{
			err::SetFormatStringError (_T("multiple property getters specified"));
			return NULL;		
		}

		pPropertyType->m_pGetter = pAccessor;
		break;

	case EPropertyAccessor_Set:
		Result = pPropertyType->m_Setter.AddOverload (pAccessor);
		if (!Result)
			return NULL;

		break;

	default:
		ASSERT (false);
	}

	m_pModule->m_AttributeMgr.AssignAttributeSet (pAccessor);
	pAccessor->m_Pos = pDeclarator->m_Pos;
	return pAccessor;
}

CEnumType*
CParser::DeclareEnumType (
	EType TypeKind,
	rtl::CString& Name
	)
{
	if (Name.IsEmpty ())
		return m_pModule->m_TypeMgr.CreateUnnamedEnumType (TypeKind);

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
	CEnumType* pType = m_pModule->m_TypeMgr.GetEnumType (TypeKind, Name, QualifiedName);

	bool Result = pNamespace->AddItem (pType);
	if (!Result)
		return NULL;

	return pType;
}

CStructType*
CParser::DeclareStructType (
	rtl::CString& Name,
	rtl::CBoxListT <CType*>* pBaseTypeList,
	size_t PackFactor
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CStructType* pType = NULL;

	if (Name.IsEmpty ())
		return m_pModule->m_TypeMgr.CreateUnnamedStructType (PackFactor);

	rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
	pType = m_pModule->m_TypeMgr.GetStructType (Name, QualifiedName, PackFactor);

	if (pBaseTypeList)
	{
		rtl::CBoxIteratorT <CType*> BaseType = pBaseTypeList->GetHead ();
		for (; BaseType; BaseType++)
		{
			CType* pBaseType = *BaseType;
			EType BaseTypeKind = pBaseType->GetTypeKind ();

			switch (BaseTypeKind)
			{
			case EType_Struct:
				Result = pType->AddBaseType ((CStructType*) pBaseType) != NULL;
				if (!Result)
					return false;
				break;

			case EType_Import:
				err::SetFormatStringError (_T("'%s': imports in inheritance list are not supported yet"), pBaseType->GetTypeString ());
				return false;

			default:
				err::SetFormatStringError (_T("'%s' cannot be inherited from '%s'"), pType->GetTypeString (), pBaseType->GetTypeString ());
				return false;
			}
		}
	}

	Result = pNamespace->AddItem (pType);
	if (!Result)
		return NULL;
	
	return pType;
}

CStructMember*
CParser::DeclareStructMember (
	CStructType* pStructType,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator,
	size_t BitCount
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier);
	if (!pType)
		return NULL;

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("qualified declarators for a struct member"));
		return NULL;
	}

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	CStructMember* pMember = pStructType->CreateMember (Name, pType, BitCount);
	if (!pMember)
		return NULL;

	m_pModule->m_AttributeMgr.AssignAttributeSet (pMember);
	pMember->m_Pos = pDeclarator->m_Pos;

	return pMember;
}

CUnionType*
CParser::DeclareUnionType (rtl::CString& Name)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CUnionType* pType = NULL;

	if (Name.IsEmpty ())
		return m_pModule->m_TypeMgr.CreateUnnamedUnionType ();

	rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
	pType = m_pModule->m_TypeMgr.GetUnionType (Name, QualifiedName);

	Result = pNamespace->AddItem (pType);
	if (!Result)
		return NULL;
	
	return pType;
}

CUnionMember*
CParser::DeclareUnionMember (
	CUnionType* pUnionType,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator,
	size_t BitCount
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier);
	if (!pType)
		return NULL;

	if (BitCount != -1)
	{
		pType = m_pModule->m_TypeMgr.GetBitFieldType (pType, 0, BitCount);
		if (!pType)
			return NULL;
	}

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("qualified declarator for union member"));
		return NULL;
	}

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	CUnionMember* pMember = pUnionType->CreateMember (Name, pType);
	if (!pMember)
		return NULL;

	m_pModule->m_AttributeMgr.AssignAttributeSet (pMember);
	pMember->m_Pos = pDeclarator->m_Pos;

	return pMember;
}

CClassType*
CParser::DeclareClassType (
	EType TypeKind,
	rtl::CString& Name,
	rtl::CBoxListT <CType*>* pBaseTypeList,
	size_t PackFactor
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CClassType* pType = NULL;

	if (Name.IsEmpty ())
		return m_pModule->m_TypeMgr.CreateUnnamedClassType (TypeKind);

	rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
	pType = m_pModule->m_TypeMgr.GetClassType (TypeKind, Name, QualifiedName, PackFactor);

	if (pBaseTypeList)
	{
		rtl::CBoxIteratorT <CType*> BaseType = pBaseTypeList->GetHead ();
		for (; BaseType; BaseType++)
		{
			CType* pBaseType = *BaseType;
			EType BaseTypeKind = pBaseType->GetTypeKind ();

			switch (BaseTypeKind)
			{
			case EType_Class:
			case EType_Interface:
				Result = pType->AddBaseType ((CClassType*) pBaseType) != NULL;
				if (!Result)
					return false;
				break;

			case EType_Import:
				err::SetFormatStringError (_T("'%s': imports in inheritance list are not supported yet"), pBaseType->GetTypeString ());
				return false;

			default:
				err::SetFormatStringError (_T("'%s' cannot be inherited from '%s'"), pType->GetTypeString (), pBaseType->GetTypeString ());
				return false;
			}
		}
	}

	Result = pNamespace->AddItem (pType);
	if (!Result)
		return NULL;
	
	return pType;
}

CFunction*
CParser::DeclareClassPreConstructor (
	CClassType* pClassType,
	CDeclarator* pDeclarator
	)
{
	if (pClassType->m_pPreConstructor)
	{
		err::SetFormatStringError (_T("'%s' already has a pre-constructor"), pClassType->GetQualifiedName ());
		return NULL;
	}

	CDeclSpecifiers DeclSpecifiers;
	DeclSpecifiers.SetType (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void));

	CFunctionType* pType = (CFunctionType*) pDeclarator->GetType (&DeclSpecifiers);
	ASSERT (pType && pType->GetTypeKind () == EType_Function); 
	
	rtl::CArrayT <CType*> ArgTypeArray = pType->GetArgTypeArray ();
	if (!ArgTypeArray.IsEmpty ())
	{
		err::SetFormatStringError (_T("pre-constructor must have no arguments"), pClassType->GetQualifiedName ());
		return NULL;
	}

	ArgTypeArray.Insert (0, pClassType);
	CFunctionType* pFullType = m_pModule->m_TypeMgr.GetFunctionType (pType->GetReturnType (), ArgTypeArray, pType->GetFlags ());
	
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateAnonimousFunction (pFullType);
	pFunction->m_FunctionKind = EFunction_PreConstructor;
	pFunction->m_Tag.Format (_T("%s.preconstruct"), pClassType->GetQualifiedName ());
	pFunction->m_pShortType = pType;
	pFunction->m_pClassType = pClassType;
	pClassType->m_pPreConstructor = pFunction;
	return pFunction;
}

CFunction*
CParser::DeclareClassConstructor (
	CClassType* pClassType,
	CDeclarator* pDeclarator
	)
{
	CDeclSpecifiers DeclSpecifiers;
	DeclSpecifiers.SetType (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void));

	CFunctionType* pType = (CFunctionType*) pDeclarator->GetType (&DeclSpecifiers);
	ASSERT (pType && pType->GetTypeKind () == EType_Function); 

	rtl::CArrayT <CType*> ArgTypeArray = pType->GetArgTypeArray ();
	ArgTypeArray.Insert (0, pClassType);
	CFunctionType* pFullType = m_pModule->m_TypeMgr.GetFunctionType (pType->GetReturnType (), ArgTypeArray, pType->GetFlags ());

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateAnonimousFunction (pFullType);
	pFunction->m_FunctionKind = EFunction_Constructor;
	pFunction->m_Tag.Format (_T("%s.construct"), pClassType->GetQualifiedName ());
	pFunction->m_pShortType = pType;
	pFunction->m_pClassType = pClassType;

	bool Result = pClassType->m_Constructor.AddOverload (pFunction);
	if (!Result)
		return NULL;

	return pFunction;
}

CFunction*
CParser::DeclareClassDestructor (
	CClassType* pClassType,
	CDeclarator* pDeclarator
	)
{
	if (pClassType->m_pDestructor)
	{
		err::SetFormatStringError (_T("'%s' already has a destructor"), pClassType->GetQualifiedName ());
		return NULL;
	}

	CDeclSpecifiers DeclSpecifiers;
	DeclSpecifiers.SetType (m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void));

	CFunctionType* pType = (CFunctionType*) pDeclarator->GetType (&DeclSpecifiers);
	ASSERT (pType && pType->GetTypeKind () == EType_Function); 

	rtl::CArrayT <CType*> ArgTypeArray = pType->GetArgTypeArray ();
	if (!ArgTypeArray.IsEmpty ())
	{
		err::SetFormatStringError (_T("destructor must have no arguments"));
		return NULL;
	}

	ArgTypeArray.Insert (0, pClassType);
	CFunctionType* pFullType = m_pModule->m_TypeMgr.GetFunctionType (pType->GetReturnType (), ArgTypeArray, pType->GetFlags ());

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateAnonimousFunction (pFullType);
	pFunction->m_FunctionKind = EFunction_Destructor;
	pFunction->m_Tag.Format (_T("%s.destruct"), pClassType->GetQualifiedName ());
	pFunction->m_pShortType = pType;
	pFunction->m_pClassType = pClassType;
	pClassType->m_pDestructor = pFunction;
	return pFunction;
}

CFunctionFormalArg*
CParser::DeclareFormalArg (
	CDeclFunctionSuffix* pArgSuffix,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator,
	const CValue& DefaultValue
	)
{
	CType* pType;
	rtl::CString Name;

	if (!pDeclarator)
	{
		pType = CDeclarator::GetType_s (pTypeSpecifier);
	}
	else
	{
		if (!pDeclarator->IsSimple ())
		{
			err::SetFormatStringError (_T("qualified declarator for formal argument"));
			return NULL;
		}

		pType = pDeclarator->GetType (pTypeSpecifier);
		if (!pType)
			return NULL;

		Name = pDeclarator->GetName ()->GetShortName ();
	}

	CFunctionFormalArg* pArg = AXL_MEM_NEW (CFunctionFormalArg);
	pArg->m_Name = Name;
	pArg->m_pType = pType;
	pArg->m_DefaultValue = DefaultValue;
	pArgSuffix->m_ArgList.InsertTail (pArg);

	return pArg;
}

bool
CParser::SwitchCaseLabel (
	CSwitchStmt* pSwitchStmt,
	intptr_t Value,
	const CToken::CPos& ClosePos,
	const CToken::CPos& OpenPos
	)
{
	rtl::CHashTableMapIteratorT <intptr_t, CBasicBlock*> It = pSwitchStmt->m_CaseMap.Goto (Value);
	if (It->m_Value)
	{
		err::SetFormatStringError (_T("redefinition of label (%d) of switch statement"), Value);
		return false;
	}

	m_pModule->m_NamespaceMgr.CloseScope (ClosePos);

	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("switch_case"));
	m_pModule->m_ControlFlowMgr.Follow (pBlock);
	It->m_Value = pBlock;

	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (OpenPos);
	pScope->m_pBreakBlock = pSwitchStmt->m_pFollowBlock;
	return true;
}

bool
CParser::SwitchDefaultLabel (
	CSwitchStmt* pSwitchStmt,
	const CToken::CPos& ClosePos,
	const CToken::CPos& OpenPos
	)
{
	if (pSwitchStmt->m_pDefaultBlock)
	{
		err::SetFormatStringError (_T("redefinition of 'default' label of switch statement"));
		return false;
	}

	m_pModule->m_NamespaceMgr.CloseScope (ClosePos);

	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("switch_default"));
	m_pModule->m_ControlFlowMgr.Follow (pBlock);
	pSwitchStmt->m_pDefaultBlock = pBlock;

	CScope* pScope = m_pModule->m_NamespaceMgr.OpenScope (OpenPos);
	pScope->m_pBreakBlock = pSwitchStmt->m_pFollowBlock;
	return true;
}

bool
CParser::FinalizeSwitchStmt (CSwitchStmt* pSwitchStmt)
{
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSwitchStmt->m_pSwitchBlock);

	CBasicBlock* pDefaultBlock = pSwitchStmt->m_pDefaultBlock ? 
		pSwitchStmt->m_pDefaultBlock : 
		pSwitchStmt->m_pFollowBlock;

	m_pModule->m_LlvmBuilder.CreateSwitch (
		pSwitchStmt->m_Value, 
		pDefaultBlock,
		pSwitchStmt->m_CaseMap.GetHead (),
		pSwitchStmt->m_CaseMap.GetCount ()
		);

	m_pModule->m_ControlFlowMgr.SetCurrentBlock (pSwitchStmt->m_pFollowBlock);
	return true;
}

bool
CParser::FinalizeConditionalExpr (
	CConditionalExpr* pConditionalExpr,
	CValue* pResultValue
	)
{
	bool Result;

	CValue TrueValue = pConditionalExpr->m_TrueValue;
	CValue FalseValue = pConditionalExpr->m_FalseValue;

	CType* pType;
	CType* pTrueType = TrueValue.GetType ();
	CType* pFalseType = FalseValue.GetType ();

	if (!pTrueType->IsNumericType () && !pFalseType->IsNumericType ())
	{
		pType = pTrueType;
	}
	else
	{
		CType* pMaxOpType = pTrueType->GetTypeKind () > pFalseType->GetTypeKind () ? pTrueType : pFalseType;
		pType = GetArithmeticOperatorResultTypeKind (pMaxOpType);
	}

	Result = m_pModule->m_OperatorMgr.CastOperator (&FalseValue, pType);
	if (!Result)
		return false;

	CBasicBlock* pElseBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock (); // might have changed

	m_pModule->m_ControlFlowMgr.Jump (pConditionalExpr->m_pPhiBlock, pConditionalExpr->m_pThenBlock);	

	Result = m_pModule->m_OperatorMgr.CastOperator (&TrueValue, pType);
	if (!Result)
		return false;

	CBasicBlock* pThenBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock (); // might have changed

	m_pModule->m_ControlFlowMgr.Follow (pConditionalExpr->m_pPhiBlock);
	m_pModule->m_LlvmBuilder.CreatePhi (TrueValue, pThenBlock, FalseValue, pElseBlock, pResultValue);
	return true;
}

bool
CParser::SetFunctionBody (
	CModuleItem* pItem,
	rtl::CBoxListT <CToken>* pBody
	)
{
	EModuleItem ItemKind = pItem->GetItemKind ();
	if (ItemKind != EModuleItem_Function)
	{
		err::SetFormatStringError (_T("'%s' cannot have a function body"), pItem->GetItemKindString ());
		return false;
	}

	CFunction* pFunction = (CFunction*) pItem;
	if (pFunction->HasBody ())
	{
		err::SetFormatStringError (_T("'%s' already has a body"), pFunction->GetTag ());
		return false;
	}

	pFunction->SetBody (pBody);
	return true;
}

bool
CParser::LookupIdentifier (
	const rtl::CString& Name,
	CValue* pValue
	)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CModuleItem* pItem = NULL;
	
	pItem = pNamespace->FindItemTraverse (Name);
	if (!pItem)
	{
		err::SetFormatStringError (_T("undeclared identifier '%s'"), Name);
		return false;
	}

	EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Type:
		pValue->SetType ((CType*) pItem);
		break;

	case EModuleItem_Variable:
		pValue->SetVariable ((CVariable*) pItem);
		break;

	case EModuleItem_Property:
		pValue->SetProperty ((CPropertyType*) pItem);
		break;

	case EModuleItem_GlobalFunction:
		pValue->SetFunctionOverload ((CGlobalFunction*) pItem);
		break;

	case EModuleItem_EnumMember:
		pValue->SetConstInt32 (((CEnumMember*) pItem)->GetValue ());
		break;

	case EModuleItem_ClassMember:
		if (m_ThisValue.IsEmpty ())
		{
			err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->GetTag ());
			return false;
		}

		return m_pModule->m_OperatorMgr.MemberOperator (m_ThisValue, Name, pValue);

	default:
		err::SetFormatStringError (_T("%s '%s' cannot be used as expression"), pItem->GetItemKindString (), Name);
		return false;
	};

	return true;
}

bool
CParser::SetThis (CValue* pValue)
{
	if (m_ThisValue.IsEmpty ())
	{
		err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->GetTag ());
		return false;
	}

	*pValue = m_ThisValue;
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
