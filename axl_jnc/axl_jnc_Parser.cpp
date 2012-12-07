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
	size_t BitCount
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
		CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (
			pNamespace,
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
		else if (pOldItem)
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
				Result = pGlobalFunction->AddOverload (pFunction);
				if (!Result)
					return NULL;
			}
		}
		else if (pDeclarator->IsSimple ())
		{
			CGlobalFunction* pGlobalFunction = m_pModule->m_FunctionMgr.CreateGlobalFunction (Name, pFunction);
			pNamespace->AddItem (pGlobalFunction);
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
			pPropertyType->m_Name = Name;
			pNamespace->AddItem (pPropertyType);
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
			CVariable* pVariable = m_pModule->m_VariableMgr.CreateVariable (Name, pType);
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

CFunctionFormalArg*
CParser::DeclareFormalArg (
	CDeclFunctionSuffix* pArgSuffix,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator,
	const CValue& DefaultValue
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier);
	if (!pType)
		return NULL;

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("qualified declarator for formal argument"));
		return NULL;
	}

	CFunctionFormalArg* pArg = AXL_MEM_NEW (CFunctionFormalArg);
	pArg->m_Name = pDeclarator->GetName ()->GetShortName ();
	pArg->m_pType = pType;
	pArg->m_DefaultValue = DefaultValue;
	pArgSuffix->m_ArgList.InsertTail (pArg);

	return pArg;
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
