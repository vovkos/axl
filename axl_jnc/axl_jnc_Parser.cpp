#include "stdafx.h"
#include "axl_jnc_Parser.h"
#include "axl_jnc_Closure.h"

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
	m_pAutoEvType = NULL;
}

bool
CParser::IsTypeSpecified ()
{
	if (m_TypeSpecifierStack.IsEmpty ())
		return false;

	CTypeSpecifierModifiers* pTypeSpecifier = m_TypeSpecifierStack.GetBack ();
	return 
		pTypeSpecifier->GetType () != NULL ||
		pTypeSpecifier->GetTypeModifiers () & (ETypeModifier_Signed | ETypeModifier_Unsigned | ETypeModifier_Event);
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
	CDeclSpecifiers* pDeclSpecifiers, // could be NULL
	CDeclarator* pDeclarator,
	CClassType* pClassType,
	size_t BitCount,
	bool HasInitializer
	)
{
	bool Result;

	int FunctionModifiers;

	CType* pType = pDeclarator->GetType (pDeclSpecifiers, &FunctionModifiers);
	if (!pType)
		return NULL;

	EStorageClass StorageClass = pDeclSpecifiers ? pDeclSpecifiers->GetStorageClass () : EStorageClass_Undefined;
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
			if (FunctionModifiers & (ETypeModifier_Cdecl | ETypeModifier_Stdcall))
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
		CEventType* pEventType = (CEventType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleEvent);

		if (pClassType)
		{
			pNewItem = pClassType->CreatePropertyMember (Name, pPropertyType);

			if (pPropertyType->GetFlags () & EPropertyTypeFlag_IsBindable)
				pPropertyType->m_pEventFieldMember = pClassType->CreateFieldMember (pEventType);
		}
		else
		{
			pPropertyType->m_PropertyKind = EProperty_Global;
			pPropertyType->m_Name = Name;

			if (pPropertyType->GetFlags () & EPropertyTypeFlag_IsBindable)
				pPropertyType->m_pEventVariable = m_pModule->m_VariableMgr.CreateVariable (Name, pEventType, false);

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
	{
		pType = m_pModule->m_TypeMgr.CreateUnnamedStructType (PackFactor);
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pType = m_pModule->m_TypeMgr.GetStructType (Name, QualifiedName, PackFactor);
	}

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

	if (!Name.IsEmpty ())
	{
		Result = pNamespace->AddItem (pType);
		if (!Result)
			return NULL;
	}
	
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
	{
		pType = m_pModule->m_TypeMgr.CreateUnnamedClassType (TypeKind);
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pType = m_pModule->m_TypeMgr.GetClassType (TypeKind, Name, QualifiedName, PackFactor);
	}

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

	if (!Name.IsEmpty ())
	{
		Result = pNamespace->AddItem (pType);
		if (!Result)
			return NULL;
	}
	
	return pType;
}

CFunction*
CParser::DeclareClassPreConstructor (
	CClassType* pClassType,
	CDeclarator* pDeclarator
	)
{
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

	return pClassType->CreatePreConstructor ();
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

	return pClassType->CreateConstructor (pType);
}

CFunction*
CParser::DeclareClassDestructor (
	CClassType* pClassType,
	CDeclarator* pDeclarator
	)
{
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

	return pClassType->CreateDestructor ();
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

CClassType*
CParser::DeclareAutoEvType (
	rtl::CString& Name,
	CDeclarator* pDeclarator
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CClassType* pAutoEvType = NULL;

	if (Name.IsEmpty ())
	{
		pAutoEvType = m_pModule->m_TypeMgr.CreateUnnamedClassType (EType_Class);
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pAutoEvType = m_pModule->m_TypeMgr.GetClassType (EType_Class, Name, QualifiedName);
	}

	if (!pDeclarator->m_SuffixList.IsEmpty ())
	{
		CDeclFunctionSuffix* pSuffix = (CDeclFunctionSuffix*) *pDeclarator->m_SuffixList.GetTail ();
		ASSERT (pSuffix->GetSuffixKind () == EDeclSuffix_FormalArg);
	}

	if (!Name.IsEmpty ())
	{
		Result = pNamespace->AddItem (pAutoEvType);
		if (!Result)
			return NULL;
	}

	pAutoEvType->m_Flags |= EClassTypeFlag_IsAutoEv;
	pAutoEvType->CreatePreConstructor ();
	pAutoEvType->CreateDestructor ();
	return pAutoEvType;
}

bool
CParser::PreAutoEvBlock (CClassType* pAutoEvType)
{
	bool Result;

	Result = m_pModule->m_FunctionMgr.Prologue (
		pAutoEvType->GetPreConstructor (), 
		m_LastMatchedToken.m_Pos, 
		&m_AutoEvConstructorThisValue
		);

	if (!Result)
		return false;	

	m_pAutoEvConstructorBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	Result = m_pModule->m_FunctionMgr.Prologue (
		pAutoEvType->GetDestructor (), 
		m_LastMatchedToken.m_Pos, 
		&m_AutoEvDestructorThisValue
		);

	if (!Result)
		return false;	

	m_pAutoEvDestructorBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();
		
	m_pAutoEvType = pAutoEvType;
	return true;
}

bool
CParser::PostAutoEvBlock ()
{
	bool Result;

	ASSERT (m_pAutoEvType && m_pAutoEvConstructorBlock && m_pAutoEvDestructorBlock);

	m_pModule->m_FunctionMgr.m_pCurrentFunction = m_pAutoEvType->GetPreConstructor ();
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (m_pAutoEvConstructorBlock);

	Result = m_pModule->m_FunctionMgr.Epilogue (m_LastMatchedToken.m_Pos);
	if (!Result)
		return false;	

	m_pModule->m_FunctionMgr.m_pCurrentFunction = m_pAutoEvType->GetDestructor ();
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (m_pAutoEvDestructorBlock);

	Result = m_pModule->m_FunctionMgr.Epilogue (m_LastMatchedToken.m_Pos);
	if (!Result)
		return false;	

	m_pAutoEvType = NULL;
	m_pAutoEvConstructorBlock = NULL;
	m_pAutoEvDestructorBlock = NULL;

	m_AutoEvConstructorThisValue.SetVoid ();
	m_AutoEvDestructorThisValue.SetVoid ();
	return true;
}

bool
CParser::PreAutoEvExpression ()
{
	ASSERT (m_pAutoEvType);

	CFunctionType* pShortType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunctionType* pFullType = m_pAutoEvType->GetSimpleMethodType ();

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateAnonymousFunction (pFullType);
	pFunction->m_FunctionKind = EFunction_Method;
	pFunction->m_Tag.Format (_T("%s.expr"), m_pAutoEvType->GetQualifiedName ());
	pFunction->m_pShortType = pShortType;
	pFunction->m_pClassType = m_pAutoEvType;

	bool Result = m_pModule->m_FunctionMgr.Prologue (pFunction, m_CurrentToken.m_Pos, &m_ThisValue);
	if (!Result)
		return false;

	return true;
}

bool
CParser::PostAutoEvExpression (const CValue& Value)
{
	ASSERT (m_pAutoEvType && m_pAutoEvConstructorBlock && m_pAutoEvDestructorBlock);

	CValue HandlerValue = m_pModule->m_FunctionMgr.GetCurrentFunction ();

	bool Result = m_pModule->m_FunctionMgr.Epilogue (m_LastMatchedToken.m_Pos);
	if (!Result)
		return false;

	// add event handlers in constructor

	m_pModule->m_FunctionMgr.m_pCurrentFunction = m_pAutoEvType->GetPreConstructor ();
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (m_pAutoEvConstructorBlock);

	CClosure* pClosure = HandlerValue.CreateClosure (EClosure_Function);
	pClosure->GetArgList ()->InsertHead (m_AutoEvConstructorThisValue);
	
	rtl::CBoxIteratorT <CValue> PropertyValue = m_BindablePropertyList.GetHead ();
	for (; PropertyValue; PropertyValue++)
	{
		CValue EventValue;
		Result = 
			m_pModule->m_OperatorMgr.OnChangeOperator (*PropertyValue, &EventValue) &&
			m_pModule->m_OperatorMgr.EventOperator (EventValue, HandlerValue, EEventOp_AddHandler);

		if (!Result)
			return false;
	}

	m_pAutoEvConstructorBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	// remove event handlers in destructor

	m_pModule->m_FunctionMgr.m_pCurrentFunction = m_pAutoEvType->GetDestructor ();
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (m_pAutoEvDestructorBlock);

	pClosure = HandlerValue.CreateClosure (EClosure_Function);
	pClosure->GetArgList ()->InsertHead  (m_AutoEvDestructorThisValue);

	PropertyValue = m_BindablePropertyList.GetHead ();
	for (; PropertyValue; PropertyValue++)
	{
		CValue EventValue;
		Result = 
			m_pModule->m_OperatorMgr.OnChangeOperator (*PropertyValue, &EventValue) &&
			m_pModule->m_OperatorMgr.EventOperator (EventValue, HandlerValue, EEventOp_RemoveHandler);

		if (!Result)
			return false;
	}

	m_pAutoEvDestructorBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	m_BindablePropertyList.Clear ();
	m_ThisValue.SetVoid ();
	return true;
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
	if (!pItem)
	{
		err::SetFormatStringError (_T("declaration without declarator cannot have a function body"));
		return false;
	}

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
	bool Result;

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

		Result = m_pModule->m_OperatorMgr.MemberOperator (m_ThisValue, Name, pValue);
		if (!Result)
			return false;
		
		break;

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
