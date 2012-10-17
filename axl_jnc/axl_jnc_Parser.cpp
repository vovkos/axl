#include "stdafx.h"
#include "axl_jnc_Parser.h"

namespace axl {
namespace jnc {

//.............................................................................

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
	TraceSymbolStack ();

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	if (m_Stage == EStage_Pass1)
		return m_pModule->m_TypeMgr.GetImportType (Name, pNamespace);

	CModuleItem* pItem = pNamespace->FindItemTraverse (Name);
	if (!pItem)
		return NULL;

	pItem = UnAliasItem (pItem);

	return pItem->GetItemKind () == EModuleItem_Type ? (CType*) pItem : NULL;
}

CClassType*
CParser::DeclareClassType (
	EType TypeKind,
	rtl::CString& Name
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CClassType* pType = NULL;

	if (Name.IsEmpty ())
		return m_pModule->m_TypeMgr.CreateUnnamedClassType (TypeKind);

	rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
	pType = m_pModule->m_TypeMgr.GetClassType (TypeKind, Name, QualifiedName);

	Result = pNamespace->AddItem (pType);
	if (!Result)
		return NULL;
	
	return pType;
}

CStructType*
CParser::DeclareStructType (
	EType TypeKind,
	rtl::CString& Name
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CStructType* pType = NULL;

	if (Name.IsEmpty ())
		return m_pModule->m_TypeMgr.CreateUnnamedStructType (TypeKind);

	rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
	pType = m_pModule->m_TypeMgr.GetStructType (TypeKind, Name, QualifiedName);

	Result = pNamespace->AddItem (pType);
	if (!Result)
		return NULL;
	
	return pType;
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

CModuleItem*
CParser::Declare (
	CDeclSpecifiers* pDeclSpecifiers,
	CDeclarator* pDeclarator,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	bool Result;

	EPropertyAccessor PropertyAccessorKind = pDeclarator->GetPropertyAccessorKind ();
	if (PropertyAccessorKind)
	{
		err::SetFormatStringError (_T("cannot declare '%s' property accessor"), GetPropertyAccessorString (PropertyAccessorKind));
		return NULL;
	}

	CType* pType = pDeclarator->GetType (pDeclSpecifiers, &m_pModule->m_TypeMgr);
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
		CFunction* pOverload;

		if (pOldItem)
		{
			if (pOldItem->GetItemKind () != EModuleItem_GlobalFunction)
			{	
				err::SetFormatStringError (_T("redefinition of '%s'"), Name);
				return NULL;
			}

			CGlobalFunction* pGlobalFunction = (CGlobalFunction*) pOldItem;
			pOverload = m_pModule->m_FunctionMgr.CreateFunction (				
				(CFunctionType*) pType, 
				pDeclarator->GetArgList ()
				); 

			Result = pGlobalFunction->AddOverload (pOverload);
			if (!Result)
				return NULL;
		}
		else 
		{
			pOverload = m_pModule->m_FunctionMgr.CreateFunction (				
				(CFunctionType*) pType, 
				pDeclarator->GetArgList ()
				);

			CGlobalFunction* pGlobalFunction = m_pModule->m_FunctionMgr.CreateGlobalFunction (Name, pOverload);
			pGlobalFunction->m_Pos = pDeclarator->m_Pos;

			Result = pNamespace->AddItem (pGlobalFunction);
			if (!Result)
				return NULL;
		}

		pNewItem = pOverload;
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

CStructMember*
CParser::DeclareStructMember (
	CStructType* pStructType,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier, &m_pModule->m_TypeMgr);
	if (!pType)
		return NULL;

	EType TypeKind = pType->GetTypeKind ();
	if (TypeKind == EType_Function ||
		TypeKind == EType_Event ||
		TypeKind == EType_Property
		)
	{
		err::SetFormatStringError (_T("'%s' is illegal type for struct member"), pType->GetTypeString ());
		return NULL;
	}

	rtl::CString Name = pDeclarator->GetName ();
	CStructMember* pOldMember = pStructType->FindMember (Name);

	if (pOldMember)
	{
		err::SetFormatStringError (_T("redefinition of '%s'"), Name);
		return NULL;
	}

	CStructMember* pNewMember = pStructType->CreateMember (Name, pType, 0);

	m_pModule->m_AttributeMgr.AssignAttributeSet (pNewMember);
	pNewMember->m_Pos = pDeclarator->m_Pos;

	return pNewMember;
}

CModuleItem*
CParser::DeclareClassMember (
	CClassType* pClassType,
	CDeclSpecifiers* pDeclSpecifiers,
	CDeclarator* pDeclarator
	)
{
	bool Result;

	CType* pType = pDeclarator->GetType (pDeclSpecifiers, &m_pModule->m_TypeMgr);
	if (!pType)
		return NULL;

	EStorageClass StorageClass = pDeclSpecifiers->GetStorageClass ();
	EType TypeKind = pType->GetTypeKind ();
	rtl::CString Name = pDeclarator->GetName ();
	CModuleItem* pOldItem = pClassType->FindMember (Name);
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
		CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction ((CFunctionType*) pType, pDeclarator->GetArgList ());

		if (!pOldItem)
		{
			pClassType->CreateMethodMember (Name, pFunction);
		}
		else
		{		 
			if (pOldItem->GetItemKind () != EModuleItem_ClassMember ||
				((CClassMember*) pOldItem)->GetMemberKind () != EClassMember_Method)
			{	
				err::SetFormatStringError (_T("redefinition of '%s'"), Name);
				return NULL;
			}

			CClassMethodMember* pMethodMember = (CClassMethodMember*) pOldItem;
			Result = pMethodMember->GetFunction ()->AddOverload (pFunction);
			if (!Result)
				return NULL;
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

		CProperty* pProperty = pDeclSpecifiers->GetProperty ();
		if (!pProperty)
		{
			pProperty = m_pModule->m_FunctionMgr.CreateProperty ((CPropertyType*) pType);
			SetSimplePropertyPos (pProperty, pDeclarator->m_Pos);
		}

		pClassType->CreatePropertyMember (Name, pProperty);
		pNewItem = pProperty;
	}
	else
	{
		if (pOldItem)
		{	
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return NULL;
		}

		pNewItem = pClassType->CreateFieldMember (Name, pType, 0);
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
	CValue* pDefaultValue
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier, &m_pModule->m_TypeMgr);
	if (!pType)
		return NULL;

	if (pDeclarator->GetPropertyAccessorKind ())
	{
		err::SetFormatStringError (_T("illegal property accessor in formal argument declaration"));
		return NULL;
	}

	return pArgSuffix->AddArg (pDeclarator->GetName (), pType, pDefaultValue);
}

CFunction*
CParser::DeclarePropertyAccessor (
	CPropertyBlock* pBlock,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier, &m_pModule->m_TypeMgr);
	if (!pType)
		return NULL;

	if (pType->GetTypeKind () != EType_Function)
	{
		err::SetFormatStringError (_T("illegal property accessor type '%s'"), pType->GetTypeString ());
		return NULL;
	}

	CFunction* pAccessor = m_pModule->m_FunctionMgr.CreateFunction (
		(CFunctionType*) pType, 
		pDeclarator->GetArgList ()
		);

	if (!pAccessor)
		return NULL;

	bool Result;

	EPropertyAccessor PropertyAccessorKind = pDeclarator->GetPropertyAccessorKind ();
	switch (PropertyAccessorKind)
	{
	case EPropertyAccessor_Get:
		if (pBlock->m_pGetter)
		{
			err::SetFormatStringError (_T("multiple property getters specified"));
			return NULL;		
		}

		pBlock->m_pGetter = pAccessor;
		break;

	case EPropertyAccessor_Set:
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
		err::SetFormatStringError (_T("'%s' cannot have a function body"), CModuleItem::GetItemKindString (ItemKind));
		return false;
	}

	CFunction* pFunction = (CFunction*) pItem;
	pFunction->SetBody (pBody);
	return true;
}

//.............................................................................

} // namespace axl {
} // namespace jnc {
