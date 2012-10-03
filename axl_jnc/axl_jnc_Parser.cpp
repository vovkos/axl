#include "stdafx.h"
#include "axl_jnc_Parser.h"

namespace axl {
namespace jnc {

//.............................................................................

bool 
CParser::IsType (const CQualifiedIdentifier& Identifier)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	return false;
}

CType*
CParser::FindType (const CQualifiedIdentifier& Identifier)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	return NULL;
}

bool
CParser::Declare (
	CNamedType* pType,
	const CToken::CPos& Pos
	)
{
	m_pModule->m_AttributeMgr.AssignAttributeSet (pType);
	pType->m_Pos = Pos;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	if (!pType->IsNamed ())
		return true;
	
	CModuleItem* pOldItem = pNamespace->FindItem (pType->GetName ());
	if (pOldItem)
	{
		// TODO: some checks

		return true;
	}
	
	pNamespace->AddItem (pType);
	return true;
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
		return false;
	}

	CType* pType = pDeclarator->GetType (pDeclSpecifiers, &m_pModule->m_TypeMgr);
	if (!pType)
		return false;

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
				return false;
			}

			return true;
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
				return false;
			}

			CGlobalFunction* pGlobalFunction = (CGlobalFunction*) pOldItem;
			pOverload = m_pModule->m_FunctionMgr.CreateFunction (				
				(CFunctionType*) pType, 
				pDeclarator->GetArgList ()
				); 

			Result = pGlobalFunction->AddOverload (pOverload);
			if (!Result)
				return false;
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
				return false;
		}

		pNewItem = pOverload;
	}
	else if (TypeKind == EType_Property)
	{
		if (pOldItem)
		{	
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return false;
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
			return false;

		pGlobalProperty->m_Pos = pDeclarator->m_Pos;
		pNewItem = pProperty;
	}
	else
	{
		if (pOldItem)
		{	
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return false;
		}

		CVariable* pVariable = m_pModule->m_VariableMgr.CreateVariable (Name, pType);
		Result = pNamespace->AddItem (pVariable);
		if (!Result)
			return false;

		pNewItem = pVariable;
	}

	if (!pNewItem)
		return false;

	m_pModule->m_AttributeMgr.AssignAttributeSet (pNewItem);
	pNewItem->m_Pos = pDeclarator->m_Pos;
	return true;
}

bool
CParser::DeclareStructMember (
	CStructType* pStructType,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier, &m_pModule->m_TypeMgr);
	if (!pType)
		return false;

	EType TypeKind = pType->GetTypeKind ();
	if (TypeKind == EType_Function ||
		TypeKind == EType_Event ||
		TypeKind == EType_Property
		)
	{
		err::SetFormatStringError (_T("'%s' is illegal type for struct member"), pType->GetTypeString ());
		return false;
	}

	rtl::CString Name = pDeclarator->GetName ();
	CStructMember* pOldMember = pStructType->FindMember (Name);

	if (pOldMember)
	{
		err::SetFormatStringError (_T("redefinition of '%s'"), Name);
		return false;
	}

	CStructMember* pNewMember = pStructType->CreateMember (Name, pType, 0);

	m_pModule->m_AttributeMgr.AssignAttributeSet (pNewMember);
	pNewMember->m_Pos = pDeclarator->m_Pos;

	return true;
}

bool
CParser::DeclareClassMember (
	CClassType* pClassType,
	CDeclSpecifiers* pDeclSpecifiers,
	CDeclarator* pDeclarator
	)
{
	bool Result;

	CType* pType = pDeclarator->GetType (pDeclSpecifiers, &m_pModule->m_TypeMgr);
	if (!pType)
		return false;

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
				return false;
			}

			return true;
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
				return false;
			}

			CClassMethodMember* pMethodMember = (CClassMethodMember*) pOldItem;
			Result = pMethodMember->GetFunction ()->AddOverload (pFunction);
			if (!Result)
				return false;
		}

		pNewItem = pFunction;
	}
	else if (TypeKind == EType_Property)
	{
		if (pOldItem)
		{	
			err::SetFormatStringError (_T("redefinition of '%s'"), Name);
			return false;
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
			return false;
		}

		pNewItem = pClassType->CreateFieldMember (Name, pType, 0);
	}

	if (!pNewItem)
		return false;

	m_pModule->m_AttributeMgr.AssignAttributeSet (pNewItem);
	pNewItem->m_Pos = pDeclarator->m_Pos;
	return true;
}

bool
CParser::DeclareFormalArg (
	CDeclFormalArgSuffix* pArgSuffix,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator,
	CValue* pDefaultValue
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier, &m_pModule->m_TypeMgr);
	if (!pType)
		return false;

	if (pDeclarator->GetPropertyAccessorKind ())
	{
		err::SetFormatStringError (_T("illegal property accessor in formal argument declaration"));
		return false;
	}

	return pArgSuffix->AddArg (pDeclarator->GetName (), pType, pDefaultValue) != NULL;
}

bool
CParser::DeclarePropertyAccessor (
	CPropertyBlock* pBlock,
	CTypeSpecifierModifiers* pTypeSpecifier,
	CDeclarator* pDeclarator
	)
{
	CType* pType = pDeclarator->GetType (pTypeSpecifier, &m_pModule->m_TypeMgr);
	if (!pType)
		return false;

	if (pType->GetTypeKind () != EType_Function)
	{
		err::SetFormatStringError (_T("illegal property accessor type '%s'"), pType->GetTypeString ());
		return false;
	}

	CFunction* pAccessor = m_pModule->m_FunctionMgr.CreateFunction (
		(CFunctionType*) pType, 
		pDeclarator->GetArgList ()
		);

	if (!pAccessor)
		return false;

	bool Result;

	EPropertyAccessor PropertyAccessorKind = pDeclarator->GetPropertyAccessorKind ();
	switch (PropertyAccessorKind)
	{
	case EPropertyAccessor_Get:
		if (pBlock->m_pGetter)
		{
			err::SetFormatStringError (_T("multiple property getters specified"));
			return false;		
		}

		pBlock->m_pGetter = pAccessor;
		break;

	case EPropertyAccessor_Set:
		Result =  pBlock->m_Setter.AddOverload (pAccessor);
		if (!Result)
			return false;
	
		break;

	default:
		ASSERT (false);
	}

	m_pModule->m_AttributeMgr.AssignAttributeSet (pAccessor);
	pAccessor->m_Pos = pDeclarator->m_Pos;
	return true;
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

//.............................................................................

} // namespace axl {
} // namespace jnc {
