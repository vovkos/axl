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
}

bool
CParser::IsTypeSpecified ()
{
	if (m_TypeSpecifierStack.IsEmpty ())
		return false;

	CTypeSpecifier* pTypeSpecifier = m_TypeSpecifierStack.GetBack ();
	return 
		pTypeSpecifier->GetType () != NULL ||
		pTypeSpecifier->GetProperty () != NULL;
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
		pItem = pNamespace->FindItemTraverse (Name);
		if (!pItem)
			return NULL;
	}

	pItem = UnAliasItem (pItem);

	return pItem->GetItemKind () == EModuleItem_Type ? (CType*) pItem : NULL;
}

CModuleItem*
CParser::Declare (
	CDeclSpecifiers* pDeclSpecifiers,
	CDeclarator* pDeclarator
	)
{
	bool Result;

	EPropertyAccessor PropertyAccessorKind = pDeclarator->GetPropertyAccessorKind ();
	if (PropertyAccessorKind)
	{
		err::SetFormatStringError (_T("cannot declare '%s' property accessor"), GetPropertyAccessorString (PropertyAccessorKind));
		return NULL;
	}

	CType* pType = pDeclarator->GetType (pDeclSpecifiers);
	if (!pType)
		return NULL;

	EStorageClass StorageClass = pDeclSpecifiers->GetStorageClass ();
	EType TypeKind = pType->GetTypeKind ();
	rtl::CString Name = pDeclarator->GetName ();
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CModuleItem* pOldItem = pNamespace->FindItem (Name);
	CModuleItem* pNewItem = NULL;

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
		if (pOldItem)
		{
			if (pOldItem->GetItemKind () != EModuleItem_GlobalFunction)
			{	
				err::SetFormatStringError (_T("redefinition of '%s'"), Name);
				return NULL;
			}

			CGlobalFunction* pGlobalFunction = (CGlobalFunction*) pOldItem;
			CFunction* pOverload = m_pModule->m_FunctionMgr.CreateFunction (				
				Name,
				(CFunctionType*) pType, 
				pDeclarator->GetArgList ()
				); 

			Result = pGlobalFunction->AddOverload (pOverload);
			if (!Result)
				return NULL;
		}
		else 
		{
			CGlobalFunction* pGlobalFunction = m_pModule->m_FunctionMgr.CreateGlobalFunction (				
				Name,
				(CFunctionType*) pType, 
				pDeclarator->GetArgList ()
				);

			pGlobalFunction->m_Pos = pDeclarator->m_Pos;

			Result = pNamespace->AddItem (pGlobalFunction);
			if (!Result)
				return NULL;

			pNewItem = pGlobalFunction->GetFunction ();
		}
	}
	else if (TypeKind == EType_Property)
	{
		if (pOldItem)
		{	
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return NULL;
		}

		CProperty* pProperty = pDeclSpecifiers->GetProperty ();
		if (!pProperty)
		{
			pProperty = m_pModule->m_FunctionMgr.CreateProperty ((CPropertyType*) pType);
			SetSimplePropertyPos (pProperty, pDeclarator->m_Pos);
		}

		CGlobalProperty* pGlobalProperty = m_pModule->m_FunctionMgr.CreateGlobalProperty (Name, pProperty);
		Result = pNamespace->AddItem (pGlobalProperty);
		if (!Result)
			return NULL;

		pGlobalProperty->m_Pos = pDeclarator->m_Pos;
		pNewItem = pProperty;
	}
	else
	{
		if (pOldItem)
		{	
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return NULL;
		}

		CVariable* pVariable = m_pModule->m_VariableMgr.CreateVariable (Name, pType);
		Result = pNamespace->AddItem (pVariable);
		if (!Result)
			return NULL;

		pNewItem = pVariable;
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
	CDeclarator* pDeclarator
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier);
	if (!pType)
		return NULL;

	rtl::CString Name = pDeclarator->GetName ();
	CStructMember* pMember = pStructType->CreateMember (Name, pType);
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
	CDeclarator* pDeclarator
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier);
	if (!pType)
		return NULL;

	rtl::CString Name = pDeclarator->GetName ();
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

CModuleItem*
CParser::DeclareClassMember (
	CClassType* pClassType,
	CDeclSpecifiers* pDeclSpecifiers,
	CDeclarator* pDeclarator
	)
{
	CType* pType = pDeclarator->GetType (pDeclSpecifiers);
	if (!pType)
		return NULL;

	EStorageClass StorageClass = pDeclSpecifiers->GetStorageClass ();
	EType TypeKind = pType->GetTypeKind ();
	rtl::CString Name = pDeclarator->GetName ();
	CModuleItem* pOldItem = pClassType->FindItem (Name);
	CModuleItem* pNewItem = NULL;

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

		pNewItem = pClassType->CreateAlias (Name, pType);
	}
	else if (TypeKind == EType_Function)
	{
		CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (Name, (CFunctionType*) pType, pDeclarator->GetArgList ());
		CClassMethod* pMethod = pClassType->CreateMethod (Name, pFunction);
		if (!pMethod)
			return NULL;

		pNewItem = pFunction;
	}
	else if (TypeKind == EType_Property)
	{
		CProperty* pProperty = pDeclSpecifiers->GetProperty ();
		if (!pProperty)
		{
			pProperty = m_pModule->m_FunctionMgr.CreateProperty ((CPropertyType*) pType);
			SetSimplePropertyPos (pProperty, pDeclarator->m_Pos);
		}

		pNewItem = pClassType->CreatePropertyMember (Name, pProperty);
	}
	else
	{
		pNewItem = pClassType->CreateFieldMember (Name, pType);
	}

	if (!pNewItem)
		return NULL;

	m_pModule->m_AttributeMgr.AssignAttributeSet (pNewItem);
	pNewItem->m_Pos = pDeclarator->m_Pos;
	return pNewItem;
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

	if (pDeclarator->GetPropertyAccessorKind ())
	{
		err::SetFormatStringError (_T("illegal property accessor in formal argument declaration"));
		return NULL;
	}

	return pArgSuffix->AddArg (pDeclarator->GetName (), pType, DefaultValue);
}

CFunction*
CParser::DeclarePropertyAccessor (
	CPropertyBlock* pBlock,
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

	CFunction* pAccessor; 

	EPropertyAccessor PropertyAccessorKind = pDeclarator->GetPropertyAccessorKind ();
	switch (PropertyAccessorKind)
	{
	case EPropertyAccessor_Get:
		if (pBlock->m_pGetter)
		{
			err::SetFormatStringError (_T("multiple property getters specified"));
			return NULL;		
		}

		pAccessor = m_pModule->m_FunctionMgr.CreateFunction (
			_T("getter"),
			(CFunctionType*) pType, 
			pDeclarator->GetArgList ()
			);

		pBlock->m_pGetter = pAccessor;
		break;

	case EPropertyAccessor_Set:
		pAccessor = m_pModule->m_FunctionMgr.CreateFunction (
			_T("setter"),
			(CFunctionType*) pType, 
			pDeclarator->GetArgList ()
			);

		Result =  pBlock->m_Setter.AddOverload (pAccessor);
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

CProperty*
CParser::CreateProperty (
	CFunction* pGetter,
	const CFunctionOverload& Setter
	)
{
	if (!pGetter)
	{
		err::SetFormatStringError (_T("property has no getter"));
		return NULL;
	}
	
	CFunctionTypeOverload SetterType;

	size_t Count = Setter.GetOverloadCount ();
	for (size_t i = 0; i < Count; i++)
	{
		CFunction* pSetter = Setter.GetFunction (i);
		bool Result = SetterType.AddOverload (pSetter->GetType ());
		if (!Result)
			return NULL;
	}

	CPropertyType* pType = m_pModule->m_TypeMgr.GetPropertyType (pGetter->GetType (), SetterType);
	return m_pModule->m_FunctionMgr.CreateProperty (pType, pGetter, Setter);
}

void
CParser::SetSimplePropertyPos (
	CProperty* pProperty,
	const CToken::CPos& Pos
	)
{
	CFunction* pGetter = pProperty->GetGetter ();
	pGetter->m_Pos = Pos;

	CFunctionOverload* pSetter = pProperty->GetSetter ();
	if (pSetter)
	{
		size_t Count = pSetter->GetOverloadCount ();
		for (size_t i = 0; i < Count; i++)
		{
			CFunction* pSetterOverload = pSetter->GetFunction (i);
			pSetterOverload->m_Pos = Pos;
		}
	}
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
	CModuleItem* pItem = pNamespace->FindItemTraverse (Name);
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

	case EModuleItem_EnumMember:
		pValue->SetConstInt32 (((CEnumMember*) pItem)->GetValue ());
		break;

	case EModuleItem_GlobalFunction:
		pValue->SetFunctionOverload ((CGlobalFunction*) pItem);
		break;

	case EModuleItem_GlobalProperty:
		pValue->SetProperty (((CGlobalProperty*) pItem)->GetProperty ());
		break;

	default:
		err::SetFormatStringError (_T("%s '%s' cannot be used as expression"), pItem->GetItemKindString (), Name);
		return false;
	};

	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
