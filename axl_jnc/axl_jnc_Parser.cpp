#include "stdafx.h"
#include "axl_jnc_Parser.h"
#include "axl_jnc_Closure.h"

namespace axl {
namespace jnc {

//.............................................................................

CParser::CParser ()
{
	m_pModule = GetCurrentThreadModule ();
	m_Stage = EStage_Pass1;
	m_StructPackFactor = 8;
	m_DefaultStructPackFactor = 8;
	m_StorageKind = EStorage_Undefined;
	m_AccessKind = EAccess_Undefined;
	m_pAttributeBlock = NULL;
	m_pLastDeclaredItem = NULL;
	m_AutoEvBindSiteCount = 0;
	m_AutoEvBindSiteTotalCount = 0;
	m_pMemberNewTargetType = NULL;
}

bool
CParser::ParseTokenList (
	ESymbol Symbol, 
	const rtl::CConstBoxListT <CToken>& TokenList,
	bool IsBuildingAst
	)
{
	ASSERT (!TokenList.IsEmpty ());

	bool Result;

	Create (Symbol, IsBuildingAst);

	rtl::CBoxIteratorT <CToken> Token = TokenList.GetHead ();
	for (; Token; Token++)
	{
		Result = ParseToken (&*Token);
		if (!Result)
		{
			err::EnsureSrcPosError (m_pModule->GetFilePath (), Token->m_Pos.m_Line, Token->m_Pos.m_Col);
			return false;
		}
	}

	// important: process EOF token, it might actually trigger actions!

	CToken::CPos Pos = TokenList.GetTail ()->m_Pos;

	CToken EofToken;
	EofToken.m_Token = 0;
	EofToken.m_Pos = Pos;
	EofToken.m_Pos.m_p += Pos.m_Length;
	EofToken.m_Pos.m_Offset += Pos.m_Length;
	EofToken.m_Pos.m_Col += Pos.m_Length;
	EofToken.m_Pos.m_Length = 0;

	Result = ParseToken (&EofToken);
	if (!Result)
	{
		err::EnsureSrcPosError (m_pModule->GetFilePath (), EofToken.m_Pos.m_Line, EofToken.m_Pos.m_Col);
		return false;
	}

	return true;
}

bool
CParser::IsTypeSpecified ()
{
	if (m_TypeSpecifierStack.IsEmpty ())
		return false;

	CTypeSpecifier* pTypeSpecifier = m_TypeSpecifierStack.GetBack ();
	return 
		pTypeSpecifier->GetType () != NULL ||
		pTypeSpecifier->GetTypeModifiers () & ETypeModifierMask_Integer;
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

	EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Type:
		return (CType*) pItem;

	case EModuleItem_Typedef:
		return ((CTypedef*) pItem)->GetType ();

	default:
		return NULL;
	}
}

bool
CParser::IsEmptyDeclarationTerminatorAllowed (CTypeSpecifier* pTypeSpecifier)
{
	if (!m_pLastDeclaredItem)
	{
		ASSERT (pTypeSpecifier);
		CType* pType = pTypeSpecifier->GetType ();
		if (!pType || !(pType->GetFlags () & ETypeFlag_Named))
		{
			err::SetFormatStringError (_T("invalid declaration (no declarator, no named type)"));
			return false;
		}

		if (pTypeSpecifier->GetTypeModifiers ())
		{
			err::SetFormatStringError (_T("unused modifier '%s'"), GetTypeModifierString (pTypeSpecifier->GetTypeModifiers ()));
			return false;
		}

		return true;
	}
	else if (
		m_pLastDeclaredItem->GetItemKind () == EModuleItem_Function && 
		((CFunction*) m_pLastDeclaredItem)->IsOrphan ())
	{
		err::SetFormatStringError (_T("orphan function '%s' without a body"), m_pLastDeclaredItem->m_Tag);
		return false;
	}

	return true;
}

bool
CParser::SetStorageKind (EStorage StorageKind)
{
	if (m_StorageKind)
	{
		err::SetFormatStringError (
			_T("more than one storage specifier specifiers ('%s' and '%s')"), 
			GetStorageKindString (m_StorageKind),
			GetStorageKindString (StorageKind)
			);
		return false;
	}

	m_StorageKind = StorageKind;
	return true;
}

bool
CParser::SetAccessKind (EAccess AccessKind)
{
	if (m_AccessKind)
	{
		err::SetFormatStringError (
			_T("more than one access specifiers ('%s' and '%s')"), 
			GetAccessKindString (m_AccessKind),
			GetAccessKindString (AccessKind)
			);
		return false;
	}

	m_AccessKind = AccessKind;
	return true;
}

CGlobalNamespace*
CParser::OpenGlobalNamespace (
	const CQualifiedName& Name,
	const CToken::CPos& Pos
	)
{
	CNamespace* pCurrentNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	if (pCurrentNamespace->GetNamespaceKind () != ENamespace_Global)
	{
		err::SetFormatStringError (_T("cannot open global namespace in '%s'"), GetNamespaceKindString (pCurrentNamespace->GetNamespaceKind ()));
		return NULL;
	}

	CGlobalNamespace* pNamespace = GetGlobalNamespace ((CGlobalNamespace*) pCurrentNamespace, Name.GetFirstName (), Pos);
	if (!pNamespace)
		return NULL;

	rtl::CBoxIteratorT <rtl::CString> It = Name.GetNameList ().GetHead ();
	for (; It; It++)
	{
		pNamespace = GetGlobalNamespace (pNamespace, *It, Pos);
		if (!pNamespace)
			return NULL;
	}
	
	m_pModule->m_NamespaceMgr.OpenNamespace (pNamespace);
	return pNamespace;
}

CGlobalNamespace*
CParser::GetGlobalNamespace (
	CGlobalNamespace* pParentNamespace,
	const rtl::CString& Name,
	const CToken::CPos& Pos
	)
{
	CGlobalNamespace* pNamespace;

	CModuleItem* pItem = pParentNamespace->FindItem (Name);
	if (!pItem)
	{
		rtl::CString QualifiedName = pParentNamespace->CreateQualifiedName (Name);

		pNamespace = m_pModule->m_NamespaceMgr.CreateGlobalNamespace (Name, QualifiedName);
		pNamespace->m_Pos = Pos;
		pNamespace->m_pParentNamespace = pParentNamespace;
		pParentNamespace->AddItem (pNamespace);
	}
	else
	{
		if (pItem->GetItemKind () != EModuleItem_Namespace)
		{
			err::SetFormatStringError (_T("'%s' exists and is not a namespace"), pParentNamespace->CreateQualifiedName (Name));
			return NULL;
		}

		pNamespace = (CGlobalNamespace*) pItem;
	}

	return pNamespace;
}

bool
CParser::OpenTypeExtension (
	const CQualifiedName& Name,
	const CToken::CPos& Pos
	)
{
	CType* pType = FindType (Name);
	if (!pType)
	{
		err::SetFormatStringError (_T("'%s' is not a type"), Name.GetFullName ());
		return false;
	}

	CNamedType* pNamedType;

	EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case EType_Enum:
	case EType_Struct:
	case EType_Union:
	case EType_Class:
		pNamedType = (CNamedType*) pType;
		break;

	case EType_Import:
		err::SetFormatStringError (_T("extension namespaces for '%s' is not supported yet"), pType->GetTypeString ());
		return false;

	default:
		err::SetFormatStringError (_T("'%s' could not have an extension namespace"), pType->GetTypeString ());
		return false;
	}

	if (pNamedType->m_pExtensionNamespace)
	{
		m_pModule->m_NamespaceMgr.OpenNamespace (pNamedType->m_pExtensionNamespace);
		return true;
	}
	
	CGlobalNamespace* pNamespace = m_pModule->m_NamespaceMgr.CreateGlobalNamespace (
		_T("extension"), 
		pNamedType->CreateQualifiedName (_T("extension"))
		);

	pNamespace->m_NamespaceKind = ENamespace_TypeExtension;
	pNamespace->m_Pos = Pos;
	pNamespace->m_pParentNamespace = pNamedType;
	pNamedType->m_pExtensionNamespace = pNamespace;

	m_pModule->m_NamespaceMgr.OpenNamespace (pNamespace);
	return true;
}

bool
CParser::Declare (
	CDeclarator* pDeclarator,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	ASSERT (pDeclarator);

	m_pLastDeclaredItem = NULL;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	int DataPtrTypeFlags;
	CType* pType = pDeclarator->CalcType (&DataPtrTypeFlags);
	if (!pType)
		return NULL;

	EDeclarator DeclaratorKind = pDeclarator->GetDeclaratorKind ();
	int PostModifiers = pDeclarator->GetPostDeclaratorModifiers ();
	EType TypeKind = pType->GetTypeKind ();

	if (PostModifiers != 0 && TypeKind != EType_Function)
	{
		err::SetFormatStringError (_T("unused post-declarator modifier '%s'"), GetPostDeclaratorModifierString (PostModifiers));
		return false;
	}

	if (m_StorageKind == EStorage_Typedef || 
		TypeKind == EType_Function ||	
		TypeKind == EType_Property || 
		TypeKind == EType_AutoEv) // not data
	{
		if (DataPtrTypeFlags)
		{
			err::SetFormatStringError (_T("unused modifier '%s'"), GetPtrTypeFlagString (DataPtrTypeFlags));
			return false;
		}

		if (DataPtrTypeFlags)
		{
			err::SetFormatStringError (_T("unused modifier '%s'"), GetPtrTypeFlagString (DataPtrTypeFlags));
			return false;
		}
	}

	switch (m_StorageKind)
	{
	case EStorage_Typedef:
		return DeclareTypedef (pType, pDeclarator);

	case EStorage_Alias:
		return DeclareAlias (pType, pDeclarator, pInitializer);

	default:
		switch (TypeKind)
		{
		case EType_Function:
			return DeclareFunction ((CFunctionType*) pType, pDeclarator);

		case EType_Property:
			return DeclareProperty ((CPropertyType*) pType, pDeclarator);

		case EType_AutoEv:
			return DeclareAutoEv ((CAutoEvType*) pType, pDeclarator);			

		default:
			switch (DeclaratorKind)
			{
			case EDeclarator_PropValue:
				return DeclarePropValue (pType, DataPtrTypeFlags, pInitializer);

			case EDeclarator_OnChange:
				return DeclareOnChange (pType, DataPtrTypeFlags);

			default:
				return DeclareData (pType, pDeclarator, DataPtrTypeFlags, pInitializer);		
			}
		}		
	}
}

void
CParser::AssignDeclarationAttributes (
	CModuleItem* pItem,
	CNamespace* pNamespace,
	const CToken::CPos& Pos
	)
{
	CModuleItemDecl* pDecl = pItem->GetItemDecl ();
	ASSERT (pDecl);

	if (!m_AccessKind)
		m_AccessKind = pNamespace->GetCurrentAccessKind ();

	pDecl->m_AccessKind = m_AccessKind;

	// don't overwrite storage unless explicit

	if (m_StorageKind)
		pDecl->m_StorageKind = m_StorageKind;

	pDecl->m_Pos = Pos;
	pDecl->m_pParentNamespace = pNamespace;
	pDecl->m_pAttributeBlock = m_pAttributeBlock;

	m_pAttributeBlock = NULL;
	m_pLastDeclaredItem = pItem;
}

bool
CParser::DeclareTypedef (
	CType* pType,
	CDeclarator* pDeclarator
	)
{
	ASSERT (m_StorageKind == EStorage_Typedef);

	bool Result;

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("invalid typedef declarator"));
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);

	CTypedef* pTypedef = m_pModule->m_TypeMgr.CreateTypedef (Name, QualifiedName, pType);
	AssignDeclarationAttributes (pTypedef, pNamespace, pDeclarator->GetPos ());

	Result = pNamespace->AddItem (pTypedef);
	if (!Result)
		return false;

	return true;
}

bool
CParser::DeclareAlias (
	CType* pType,
	CDeclarator* pDeclarator,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	ASSERT (m_StorageKind == EStorage_Alias);

	bool Result;

	if (pType->GetTypeKind () != EType_Void)
	{
		err::SetFormatStringError (_T("alias cannot have type"));
		return false;
	}

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("invalid alias declarator"));
		return false;
	}

	if (!pInitializer)
	{
		err::SetFormatStringError (_T("alias must have an initializer"));
		return false;
	}

	CParser Parser;
	Parser.m_pModule = m_pModule;
	Parser.m_Stage = EStage_Pass2;

	Result = Parser.ParseTokenList (ESymbol_expression_save_value_s, *pInitializer);
	if (!Result)
		return false;

	pType = Parser.m_ExpressionValue.GetType ();

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);

	CAlias* pAlias = m_pModule->m_VariableMgr.CreateAlias (Name, QualifiedName, pType, pInitializer);
	AssignDeclarationAttributes (pAlias, pNamespace, pDeclarator->GetPos ());

	Result = pNamespace->AddItem (pAlias);
	if (!Result)
		return false;

	return true;
}

bool
CParser::DeclareFunction (
	CFunctionType* pType,
	CDeclarator* pDeclarator
	)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();
	EDeclarator DeclaratorKind = pDeclarator->GetDeclaratorKind ();
	int PostModifiers = pDeclarator->GetPostDeclaratorModifiers ();
	EFunction FunctionKind = pDeclarator->GetFunctionKind ();
	bool HasArgs = !pType->GetArgArray ().IsEmpty ();

	if (DeclaratorKind == EDeclarator_UnaryBinaryOperator)
	{
		ASSERT (!FunctionKind);
		FunctionKind = HasArgs ? 
			EFunction_BinaryOperator : 
			EFunction_UnaryOperator;
	}

	ASSERT (FunctionKind);
	int FunctionKindFlags = GetFunctionKindFlags (FunctionKind);

	if ((FunctionKindFlags & EFunctionKindFlag_NoStorage) && m_StorageKind)
	{
		err::SetFormatStringError (_T("'%s' cannot have storage specifier"), GetFunctionKindString (FunctionKind));
		return false;
	}

	if ((FunctionKindFlags & EFunctionKindFlag_NoArgs) && HasArgs)
	{
		err::SetFormatStringError (_T("'%s' cannot have arguments"), GetFunctionKindString (FunctionKind));
		return false;
	}

	if (!m_StorageKind)
	{
		m_StorageKind = 
			FunctionKind == EFunction_StaticConstructor || FunctionKind == EFunction_StaticDestructor ? EStorage_Static :
			NamespaceKind == ENamespace_Property ? ((CProperty*) pNamespace)->GetStorageKind () : EStorage_Undefined;
	}
		
	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		if (m_StorageKind)
		{
			err::SetFormatStringError (_T("invalid storage '%s' in property template"), GetStorageKindString (m_StorageKind));
			return false;
		}

		if (PostModifiers)
		{
			err::SetFormatStringError (_T("unused post-declarator modifier '%s'"), GetPostDeclaratorModifierString (PostModifiers));
			return false;
		}

		return ((CPropertyTemplate*) pNamespace)->AddMethod (FunctionKind, pType);
	}

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (FunctionKind, pType);
	pFunction->m_DeclaratorName = *pDeclarator->GetName ();
	pFunction->m_Tag = pNamespace->CreateQualifiedName (pFunction->m_DeclaratorName);

	AssignDeclarationAttributes (pFunction, pNamespace, pDeclarator->GetPos ());

	if (PostModifiers & EPostDeclaratorModifier_Const)
		pFunction->m_ThisArgTypeFlags = EPtrTypeFlag_Const;

	switch (FunctionKind)
	{
	case EFunction_Named:
		pFunction->m_Name = pDeclarator->GetName ()->GetShortName ();
		pFunction->m_QualifiedName = pNamespace->CreateQualifiedName (pFunction->m_Name);
		pFunction->m_Tag = pFunction->m_QualifiedName;
		break;

	case EFunction_UnaryOperator:
		pFunction->m_UnOpKind = pDeclarator->GetUnOpKind ();
		pFunction->m_Tag.AppendFormat (_T(".unary operator %s"), GetUnOpKindString (pFunction->m_UnOpKind));
		break;

	case EFunction_BinaryOperator:
		pFunction->m_BinOpKind = pDeclarator->GetBinOpKind ();
		pFunction->m_Tag.AppendFormat (_T(".binary operator %s"), GetBinOpKindString (pFunction->m_BinOpKind));
		break;

	case EFunction_CastOperator:
		pFunction->m_pCastOpType = pDeclarator->GetCastOpType ();
		pFunction->m_Tag.AppendFormat (_T(".cast operator %s"), pFunction->m_pCastOpType);
		break;

	default:
		pFunction->m_Tag.AppendFormat (_T(".%s"), GetFunctionKindString (FunctionKind));
	}

	EType TypeKind;
	
	switch (NamespaceKind)
	{
	case ENamespace_TypeExtension:
		if (!pDeclarator->IsSimple ())
		{
			err::SetFormatStringError (_T("invalid declarator '%s' in type extension"), pFunction->m_Tag);
			return false;
		}

		if (pFunction->IsVirtual ())
		{
			err::SetFormatStringError (_T("invalid storage '%s' in type extension"), GetStorageKindString (pFunction->m_StorageKind));
			return false;
		}

		break;

	case ENamespace_Type:
		if (pDeclarator->IsQualified () && m_StorageKind != EStorage_Override)
		{
			err::SetFormatStringError (_T("only overrides could be qualified, '%s' is not an override"), pFunction->m_Tag);
			return false;
		}

		TypeKind = ((CNamedType*) pNamespace)->GetTypeKind ();
		switch (TypeKind)
		{
		case EType_Struct:
			return ((CStructType*) pNamespace)->AddMethod (pFunction);

		case EType_Union:
			return ((CUnionType*) pNamespace)->AddMethod (pFunction);

		case EType_Class:
			return ((CClassType*) pNamespace)->AddMethod (pFunction);

		default:
			err::SetFormatStringError (_T("method members are not allowed in '%s'"), ((CNamedType*) pNamespace)->GetTypeString ());
			return false;
		}

	case ENamespace_Property:
		if (pDeclarator->IsQualified ())
		{
			err::SetFormatStringError (_T("invalid qualified declarator '%s' in property"), pFunction->m_Tag);
			return false;
		}

		return ((CProperty*) pNamespace)->AddMethod (pFunction);
	
	default:
		if (PostModifiers)
		{
			err::SetFormatStringError (_T("unused post-declarator modifier '%s'"), GetPostDeclaratorModifierString (PostModifiers));
			return false;
		}

		if (m_StorageKind)
		{
			err::SetFormatStringError (_T("invalid storage specifier '%s' for a global function"), GetStorageKindString (m_StorageKind));
			return false;
		}

		if (pDeclarator->IsQualified ())
		{
			pFunction->m_pOrphanNamespace = pNamespace;
			m_pModule->m_FunctionMgr.m_OrphanFunctionArray.Append (pFunction);
			return true;
		}

		pFunction->m_StorageKind = EStorage_Static;
	}

	if (FunctionKind != EFunction_Named)
	{
		err::SetFormatStringError (
			_T("invalid '%s' at '%s' namespace"), 
			GetFunctionKindString (FunctionKind),
			GetNamespaceKindString (NamespaceKind)
			);
		return false;
	}

	return pNamespace->AddFunction (pFunction);
}

bool
CParser::DeclareProperty (
	CPropertyType* pType,
	CDeclarator* pDeclarator
	)
{
	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("invalid property declarator"));
		return false;
	}

	CProperty* pProperty = CreateProperty (
		pDeclarator->GetName ()->GetShortName (), 
		pDeclarator->GetPos (), 
		m_StructPackFactor
		);

	return pProperty && pProperty->Create (pType);
}

CPropertyTemplate*
CParser::CreatePropertyTemplate ()
{
	CPropertyTemplate* pPropertyTemplate = m_pModule->m_FunctionMgr.CreatePropertyTemplate ();
	pPropertyTemplate->m_TypeModifiers = GetTypeSpecifier ()->ClearTypeModifiers (ETypeModifierMask_Property);
	return pPropertyTemplate;
}

CProperty*
CParser::CreateProperty (
	const rtl::CString& Name,
	const CToken::CPos& Pos,
	size_t PackFactor
	)
{
	bool Result;

	m_pLastDeclaredItem = NULL;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		err::SetFormatStringError (_T("property templates cannot have property memberts"));
		return NULL;
	}

	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
	CProperty* pProperty = m_pModule->m_FunctionMgr.CreateProperty (Name, QualifiedName);
	
	AssignDeclarationAttributes (pProperty, pNamespace, Pos);

	EType TypeKind;

	switch (NamespaceKind)
	{
	case ENamespace_Type:
		TypeKind = ((CNamedType*) pNamespace)->GetTypeKind ();
		switch (TypeKind)
		{
		case EType_Struct:
			Result = ((CStructType*) pNamespace)->AddProperty (pProperty);
			break;

		case EType_Union:
			Result = ((CUnionType*) pNamespace)->AddProperty (pProperty);
			break;

		case EType_Class:
			Result = ((CClassType*) pNamespace)->AddProperty (pProperty);
			break;

		default:
			err::SetFormatStringError (_T("property members are not allowed in '%s'"), ((CNamedType*) pNamespace)->GetTypeString ());
			return NULL;
		}

		if (!Result)
			return NULL;

		break;

	case ENamespace_Property:
		Result = ((CProperty*) pNamespace)->AddProperty (pProperty);
		if (!Result)
			return false;

		break;

	default:
		if (m_StorageKind)
		{
			err::SetFormatStringError (_T("invalid storage specifier '%s' for a global property"), GetStorageKindString (m_StorageKind));
			return false;
		}

		Result = pNamespace->AddItem (pProperty);
		if (!Result)
			return NULL;

		pProperty->m_StorageKind = EStorage_Static;
	}
	
	return pProperty;
}

bool
CParser::DeclareAutoEv (
	CAutoEvType* pType,
	CDeclarator* pDeclarator
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();
	EDeclarator DeclaratorKind = pDeclarator->GetDeclaratorKind ();

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("invalid autoev declarator (qualified autoev not supported yet)"));
		return false;
	}

	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		err::SetFormatStringError (_T("property templates cannot have autoev memberts"));
		return false;
	}

	if (m_StorageKind && m_StorageKind != EStorage_Static && m_StorageKind != EStorage_Member)
	{
		err::SetFormatStringError (_T("invalid storage '%s' for autoev"), GetStorageKindString (m_StorageKind));
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
			err::SetFormatStringError (_T("invalid declarator '%s' in type extension"), pAutoEv->m_Tag);
			return false;
		}

		break;

	case ENamespace_Type:
		if (((CNamedType*) pNamespace)->GetTypeKind () != EType_Class)
		{
			err::SetFormatStringError (_T("method members are not allowed in '%s'"), ((CNamedType*) pNamespace)->GetTypeString ());
			return false;
		}

		if (pDeclarator->IsQualified () && m_StorageKind != EStorage_Override)
		{
			err::SetFormatStringError (_T("only overrides could be qualified, '%s' is not an override"), pAutoEv->m_Tag);
			return false;
		}

		Result = ((CClassType*) pNamespace)->AddAutoEv (pAutoEv);
		if (!Result)
			return false;

		break;

	case ENamespace_Property:
		if (pDeclarator->IsQualified ())
		{
			err::SetFormatStringError (_T("invalid qualified declarator '%s' in property"), pAutoEv->m_Tag);
			return false;
		}

		Result = ((CProperty*) pNamespace)->AddAutoEv (pAutoEv);
		if (!Result)
			return false;

		break;
	
	default:
		if (m_StorageKind)
		{
			err::SetFormatStringError (_T("invalid storage specifier '%s' for a global AutoEv"), GetStorageKindString (m_StorageKind));
			return false;
		}

		pAutoEv->m_StorageKind = EStorage_Static;

		Result = pNamespace->AddItem (pAutoEv);
		if (!Result)
			return false;
	}

	return pAutoEv->Create (pType);
}

bool
CParser::DeclareData (	
	CType* pType,
	CDeclarator* pDeclarator,
	int PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	bool Result;

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("invalid data declarator"));
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		err::SetFormatStringError (_T("property templates cannot have data fields"));
		return false;
	}

	if (pType->GetTypeKind () == EType_Class) 
		pType = ((CClassType*) pType)->GetClassPtrType ();

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	size_t BitCount = pDeclarator->GetBitCount ();

	CModuleItem* pDataItem = NULL;

	if (NamespaceKind == ENamespace_Property)
	{
		pDataItem = ((CProperty*) pNamespace)->CreateField (Name, pType, BitCount, PtrTypeFlags, pInitializer);
		AssignDeclarationAttributes (pDataItem, pNamespace, pDeclarator->GetPos ());
	}
	else if (NamespaceKind != ENamespace_Type || m_StorageKind == EStorage_Static)
	{
		CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();

		switch (m_StorageKind)
		{
		case EStorage_Undefined:
			m_StorageKind = pScope ? EStorage_Stack : EStorage_Static;
			break;

		case EStorage_Static:
		case EStorage_Heap:
			break;

		case EStorage_Stack:
			if (!pScope)
			{
				err::SetFormatStringError (_T("can only use 'stack' storage specifier for local variables"));
				return false;
			}

			break;

		default:
			err::SetFormatStringError (_T("invalid storage specifier '%s' for variable"), GetStorageKindString (m_StorageKind));
			return false;
		}

		CVariable* pVariable = m_pModule->m_VariableMgr.CreateVariable (
			Name, 
			pNamespace->CreateQualifiedName (Name),
			pType, 
			PtrTypeFlags,
			pInitializer
			);

		AssignDeclarationAttributes (pVariable, pNamespace, pDeclarator->GetPos ());

		Result = pNamespace->AddItem (pVariable);
		if (!Result)
			return false;

		if (pScope)
		{
			pVariable->m_pScope = pScope;

			Result = 
				m_pModule->m_VariableMgr.AllocateVariable (pVariable) &&
				m_pModule->m_VariableMgr.InitializeVariable (pVariable);

			if (!Result)
				return false;
		}
	}
	else
	{
		CStructField* pField;
		CNamedType* pNamedType =  (CNamedType*) pNamespace;
		EType NamedTypeKind = pNamedType->GetTypeKind ();

		switch (NamedTypeKind)
		{
		case EType_Class:
			pField = ((CClassType*) pNamedType)->CreateField (Name, pType, BitCount, PtrTypeFlags, pInitializer);
			break;

		case EType_Struct:
			pField = ((CStructType*) pNamedType)->CreateField (Name, pType, BitCount, PtrTypeFlags, pInitializer);
			break;

		case EType_Union:
			pField = ((CUnionType*) pNamedType)->CreateField (Name, pType, BitCount, PtrTypeFlags, pInitializer);
			break;

		default:
			err::SetFormatStringError (_T("field members are not allowed in '%s'"), pNamedType->GetTypeString ());
			return false;
		}

		AssignDeclarationAttributes (pField, pNamespace, pDeclarator->GetPos ());
	}

	return true;
}

bool
CParser::DeclarePropValue (	
	CType* pType,
	int PtrTypeFlags,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	if (m_StorageKind)
	{
		err::SetFormatStringError (_T("'propvalue' cannot have a storage specifier"));
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	if (pType->GetTypeKind () == EType_Class) 
		pType = ((CClassType*) pType)->GetClassPtrType ();

	CFunctionType* pGetterType = m_pModule->m_TypeMgr.GetFunctionType (pType, NULL, 0, 0);

	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		CPropertyTemplate* pPropertyTemplate = (CPropertyTemplate*) pNamespace;
		pPropertyTemplate->m_pAuPropValueType = pType;
		pPropertyTemplate->m_TypeModifiers |= ETypeModifier_AutoGet;
		return pPropertyTemplate->AddMethod (EFunction_Getter, pGetterType);
	}
	
	if (NamespaceKind != ENamespace_Property)
	{
		err::SetFormatStringError (_T("'propvalue' is only allowed in property body"));
		return false;
	}

	CProperty* pProperty = (CProperty*) pNamespace;
	pProperty->m_pAuPropValueType = pType;
	pProperty->m_TypeModifiers |= ETypeModifier_AutoGet;

	CFunction* pGetter = m_pModule->m_FunctionMgr.CreateFunction (EFunction_Getter, pGetterType);
	
	m_StorageKind = pProperty->GetStorageKind ();
	AssignDeclarationAttributes (pGetter, pNamespace, m_LastMatchedToken.m_Pos);
	return pProperty->AddMethod (pGetter);
}

bool
CParser::DeclareOnChange (
	CType* pType,
	int PtrTypeFlags
	)
{
	if (m_StorageKind)
	{
		err::SetFormatStringError (_T("'onchange' cannot have a storage specifier"));
		return false;
	}
	
	if (pType->Cmp (CGetType (m_pModule, EStdType_SimpleMulticast)) != 0 || !(PtrTypeFlags & EPtrTypeFlag_Event))
	{
		err::SetFormatStringError (_T("invalid type for 'onchange' event"));
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		CPropertyTemplate* pPropertyTemplate = (CPropertyTemplate*) pNamespace;
		if (pPropertyTemplate->m_TypeModifiers & ETypeModifier_Bindable)
		{
			err::SetFormatStringError (_T("property already has 'onchange' event"));
			return false;
		}

		pPropertyTemplate->m_TypeModifiers |= ETypeModifier_Bindable;
		return true;
	}
	
	if (NamespaceKind != ENamespace_Property)
	{
		err::SetFormatStringError (_T("'onchange' is only allowed in property body"));
		return false;
	}

	CProperty* pProperty = (CProperty*) pNamespace;
	if (pProperty->m_TypeModifiers & ETypeModifier_Bindable)
	{
		err::SetFormatStringError (_T("property already has 'onchange' event"));
		return false;
	}

	pProperty->m_TypeModifiers |= ETypeModifier_Bindable;
	m_pLastDeclaredItem = pProperty; // allow empty declaration terminator
	return true;
}

CFunctionArg*
CParser::CreateFormalArg (
	CDeclFunctionSuffix* pArgSuffix,
	CDeclarator* pDeclarator,
	rtl::CBoxListT <CToken>* pInitializer
	)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	int PtrTypeFlags = 0;
	CType* pType = pDeclarator->CalcType (&PtrTypeFlags);
	if (!pType)
		return NULL;

	if (pType->GetTypeKind () == EType_Class)
		pType = ((CClassType*) pType)->GetClassPtrType ();

	rtl::CString Name;

	if (pDeclarator->IsSimple ())
	{
		Name = pDeclarator->GetName ()->GetShortName ();
	}
	else if (pDeclarator->GetDeclaratorKind () != EDeclarator_Undefined)
	{
		err::SetFormatStringError (_T("invalid formal argument declarator"));
		return NULL;
	}

	CFunctionArg* pArg = m_pModule->m_TypeMgr.CreateFunctionArg (Name, pType, PtrTypeFlags, pInitializer);
	AssignDeclarationAttributes (pArg, pNamespace, pDeclarator->GetPos ());

	pArgSuffix->m_ArgArray.Append (pArg);

	return pArg;
}

CEnumType*
CParser::CreateEnumType (
	const rtl::CString& Name,
	CType* pBaseType,
	int Flags
	)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CEnumType* pEnumType = NULL;

	if (Name.IsEmpty ())
	{
		pEnumType = m_pModule->m_TypeMgr.CreateUnnamedEnumType (pBaseType);
		pEnumType->m_Flags = EEnumTypeFlag_Exposed;
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pEnumType = m_pModule->m_TypeMgr.CreateEnumType (Name, QualifiedName, pBaseType, Flags);
		if (!pEnumType)
			return NULL;

		bool Result = pNamespace->AddItem (pEnumType);
		if (!Result)
			return NULL;
	}

	AssignDeclarationAttributes (pEnumType, pNamespace, m_LastMatchedToken.m_Pos);
	return pEnumType;
}

CStructType*
CParser::CreateStructType (
	const rtl::CString& Name,
	rtl::CBoxListT <CType*>* pBaseTypeList,
	size_t PackFactor
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CStructType* pStructType = NULL;

	if (Name.IsEmpty ())
	{
		pStructType = m_pModule->m_TypeMgr.CreateUnnamedStructType (PackFactor);
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pStructType = m_pModule->m_TypeMgr.CreateStructType (Name, QualifiedName, PackFactor);
		if (!pStructType)
			return NULL;
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
				Result = pStructType->AddBaseType ((CStructType*) pBaseType) != NULL;
				if (!Result)
					return NULL;
				break;

			case EType_Import:
				err::SetFormatStringError (_T("'%s': imports in inheritance list are not supported yet"), pBaseType->GetTypeString ());
				return NULL;

			default:
				err::SetFormatStringError (_T("'%s' cannot be inherited from '%s'"), pStructType->GetTypeString (), pBaseType->GetTypeString ());
				return NULL;
			}
		}
	}

	if (!Name.IsEmpty ())
	{
		Result = pNamespace->AddItem (pStructType);
		if (!Result)
			return NULL;
	}

	AssignDeclarationAttributes (pStructType, pNamespace, m_LastMatchedToken.m_Pos);
	return pStructType;
}

CUnionType*
CParser::CreateUnionType (const rtl::CString& Name)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CUnionType* pUnionType = NULL;

	if (Name.IsEmpty ())
	{
		pUnionType = m_pModule->m_TypeMgr.CreateUnnamedUnionType ();
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pUnionType = m_pModule->m_TypeMgr.CreateUnionType (Name, QualifiedName);
		if (!pUnionType)
			return NULL;

		Result = pNamespace->AddItem (pUnionType);
		if (!Result)
			return NULL;
	}

	AssignDeclarationAttributes (pUnionType, pNamespace, m_LastMatchedToken.m_Pos);
	return pUnionType;
}

CClassType*
CParser::CreateClassType (
	const rtl::CString& Name,
	rtl::CBoxListT <CType*>* pBaseTypeList,
	size_t PackFactor
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CClassType* pClassType;

	if (Name.IsEmpty ())
	{
		pClassType = m_pModule->m_TypeMgr.CreateUnnamedClassType (PackFactor);
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pClassType = m_pModule->m_TypeMgr.CreateClassType (Name, QualifiedName, PackFactor);
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
				Result = pClassType->AddBaseType ((CStructType*) pBaseType) != NULL;
				if (!Result)
					return false;
				break;

			case EType_Class:
				Result = pClassType->AddBaseType ((CClassType*) pBaseType) != NULL;
				if (!Result)
					return false;
				break;

			case EType_Import:
				err::SetFormatStringError (_T("'%s': imports in inheritance list are not supported yet"), pBaseType->GetTypeString ());
				return false;

			default:
				err::SetFormatStringError (_T("'%s' cannot be inherited from '%s'"), pClassType->GetTypeString (), pBaseType->GetTypeString ());
				return false;
			}
		}
	}

	if (!Name.IsEmpty ())
	{
		Result = pNamespace->AddItem (pClassType);
		if (!Result)
			return NULL;
	}
	
	AssignDeclarationAttributes (pClassType, pNamespace, m_LastMatchedToken.m_Pos);
	return pClassType;
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
	rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
	CClassType* pClassType = m_pModule->m_TypeMgr.CreateClassType (Name, QualifiedName);
	pClassType->m_Flags |= EClassTypeFlag_AutoEv;

	Result = pNamespace->AddItem (pClassType);
	if (!Result)
		return NULL;

	AssignDeclarationAttributes (pClassType, pNamespace, m_LastMatchedToken.m_Pos);

	CFunctionType* pStarterType = pFunctionSuffix ? m_pModule->m_TypeMgr.GetFunctionType (
		NULL, 
		pFunctionSuffix->GetArgArray (),
		pFunctionSuffix->GetFunctionTypeFlags ()
		) : 
		(CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);

	CAutoEvType* pAutoEvType = m_pModule->m_TypeMgr.GetAutoEvType (pStarterType);	

	CAutoEv* pAutoEv = m_pModule->m_FunctionMgr.CreateUnnamedAutoEv ();
	
	Result = 
		pClassType->AddAutoEv (pAutoEv) &&
		pAutoEv->Create (pAutoEvType);

	if (!Result)
		return false;

	if (pTokenList)
		pAutoEv->SetBody (pTokenList);

	return pClassType;
}

bool
CParser::FinalizeAutoEv ()
{
	bool Result;

	CAutoEv* pAutoEv = m_pModule->m_FunctionMgr.GetCurrentAutoEv ();
	ASSERT (pAutoEv);

	bool IsMember = pAutoEv->IsMember ();

	CStructType* pBindSiteType = (CStructType*) m_pModule->m_TypeMgr.GetStdType (EStdType_AutoEvBindSite);
	CStructField* pEventPtrField = *pBindSiteType->GetFieldList ().GetHead ();
	CStructField* pCookieField = *pBindSiteType->GetFieldList ().GetTail ();

	CValue BindSiteArrayValue;
	Result = m_pModule->m_OperatorMgr.GetAutoEvField (pAutoEv, EAutoEvField_BindSiteArray, &BindSiteArrayValue);
	if (!Result)
		return false;

	rtl::CIteratorT <TAutoEvHandler> Handler = m_AutoEvHandlerList.GetHead ();
	size_t i = 0;
	for (; Handler; Handler++)
	{
		CFunction* pFunction = Handler->m_pFunction;

		rtl::CBoxIteratorT <CValue> Value = Handler->m_BindSiteList.GetHead (); 
		for (; Value; Value++, i++)
		{
			CValue OnChangeValue;
			Result = m_pModule->m_OperatorMgr.GetAuPropertyField (*Value, EAuPropertyField_OnChange, &OnChangeValue);
			if (!Result)
				return false;
	
			CValue HandlerValue = pFunction;
			if (IsMember)
			{
				Result = CreateMemberClosure (&HandlerValue);
				if (!Result)
					return false;
			}

			CValue IdxValue (i, EType_SizeT);
			CValue CookieValue;
			CValue BindSiteValue;
			CValue DstOnChangeValue;
			CValue DstCookieValue;

			Result = 
				m_pModule->m_OperatorMgr.BinaryOperator (EBinOp_AddAssign, OnChangeValue, HandlerValue, &CookieValue) &&
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

	ASSERT (i == pAutoEv->m_BindSiteCount);
	return true;
}

bool
CParser::FinalizeAutoEvOnChangeClause ()
{
	CAutoEv* pAutoEv = m_pModule->m_FunctionMgr.GetCurrentAutoEv ();
	ASSERT (pAutoEv);

	if (m_AutoEvBindSiteList.IsEmpty ())
	{
		err::SetFormatStringError (_T("no bindable sites found"));
		return false;
	}
	
	TAutoEvHandler* pHandler = AXL_MEM_NEW (TAutoEvHandler);
	pHandler->m_pFunction = pAutoEv->CreateHandler ();
	pHandler->m_BindSiteList.TakeOver (&m_AutoEvBindSiteList);
	m_AutoEvHandlerList.InsertTail (pHandler);

	return m_pModule->m_FunctionMgr.Prologue (pHandler->m_pFunction, m_LastMatchedToken.m_Pos);
}

void
CParser::FinalizeAutoEvOnChangeStmt ()
{
	m_pModule->m_FunctionMgr.Epilogue (m_LastMatchedToken.m_Pos);
}

bool
CParser::AutoEvExpressionStmt (rtl::CBoxListT <CToken>* pTokenList)
{
	ASSERT (!pTokenList->IsEmpty ());	

	bool Result;

	CAutoEv* pAutoEv = m_pModule->m_FunctionMgr.GetCurrentAutoEv ();
	ASSERT (pAutoEv);

	CParser Parser;
	Parser.m_pModule = m_pModule;
	Parser.m_Stage = EStage_Pass2;

	Result = Parser.ParseTokenList (ESymbol_expression, *pTokenList);
	if (!Result)
		return false;

	if (Parser.m_AutoEvBindSiteList.IsEmpty ())
	{
		err::SetFormatStringError (_T("no bindable sites found"));
		return false;
	}

	TAutoEvHandler* pHandler = AXL_MEM_NEW (TAutoEvHandler);
	pHandler->m_pFunction = pAutoEv->CreateHandler ();
	pHandler->m_BindSiteList.TakeOver (&Parser.m_AutoEvBindSiteList);
	m_AutoEvHandlerList.InsertTail (pHandler);

	Result = m_pModule->m_FunctionMgr.Prologue (pHandler->m_pFunction, pTokenList->GetHead ()->m_Pos);
	if (!Result)
		return false;

	Result = Parser.ParseTokenList (ESymbol_expression, *pTokenList);
	if (!Result)
		return false;

	m_pModule->m_FunctionMgr.Epilogue (pTokenList->GetTail ()->m_Pos);
	return true;
}

bool
CParser::CountAutoEvBindSites ()
{
	if (!m_AutoEvBindSiteCount)
	{
		err::SetFormatStringError (_T("no bindable sites found"));
		return false;
	}

	m_AutoEvBindSiteTotalCount += m_AutoEvBindSiteCount;
	m_AutoEvBindSiteCount = 0;
	return true;
}

bool
CParser::NewOperator_s (
	EStorage StorageKind, 
	CType* pType, 
	CValue* pResultValue
	)
{
	if (m_pMemberNewTargetType && StorageKind == EStorage_Member)
	{
		bool Result = m_pMemberNewTargetType->AddMemberNewType (pType);
		if (!Result)
			return false;
	}

	pResultValue->SetType (m_pModule->m_OperatorMgr.GetNewOperatorResultType (pType));
	return true;
}

bool
CParser::SetFunctionBody (rtl::CBoxListT <CToken>* pBody)
{
	if (!m_pLastDeclaredItem)
	{
		err::SetFormatStringError (_T("declaration without declarator cannot have a function body"));
		return false;
	}

	EModuleItem ItemKind = m_pLastDeclaredItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Function:
		{
		CFunction* pFunction = (CFunction*) m_pLastDeclaredItem;
		if (pFunction->HasBody ())
		{
			err::SetFormatStringError (_T("'%s' already has a body"), pFunction->m_Tag);
			return false;
		}

		if (pFunction->GetStorageKind () == EStorage_Abstract)
		{
			err::SetFormatStringError (_T("'%s' is abstract and hence cannot have a body"), pFunction->m_Tag);
			return false;
		}

		pFunction->SetBody (pBody);
		}
		break;

	case EModuleItem_AutoEv:
		{
		CAutoEv* pAutoEv = (CAutoEv*) m_pLastDeclaredItem;
		if (pAutoEv->HasBody ())
		{
			err::SetFormatStringError (_T("'%s' already has a body"), pAutoEv->m_Tag);
			return false;
		}

		pAutoEv->SetBody (pBody);
		}
		break;

	default:
		err::SetFormatStringError (_T("'%s' cannot have a function body"), GetModuleItemKindString (m_pLastDeclaredItem->GetItemKind ()));
		return false;
	}

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
	
	CBaseTypeCoord Coord;
	pItem = pNamespace->FindItemTraverse (Name, &Coord);
	if (!pItem)
	{
		if (Name == _T("__ScopeLevel")) // tmp
			return m_pModule->m_OperatorMgr.CalcScopeLevelValue (m_pModule->m_NamespaceMgr.GetCurrentScope (), pValue);

		err::SetFormatStringError (_T("undeclared identifier '%s'"), Name);
		return false;
	}

	EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Type:
		pValue->SetType ((CType*) pItem);
		break;

	case EModuleItem_Typedef:
		pValue->SetType (((CTypedef*) pItem)->GetType ());
		break;

	case EModuleItem_Alias:
		return m_pModule->m_OperatorMgr.EvaluateAlias (((CAlias*) pItem)->GetInitializer (), pValue);

	case EModuleItem_Variable:
		pValue->SetVariable ((CVariable*) pItem);
		break;

	case EModuleItem_Function:
		pValue->SetFunction ((CFunction*) pItem);

		if (((CFunction*) pItem)->IsMember ())
		{
			Result = CreateMemberClosure (pValue);
			if (!Result)
				return false;
		}

		break;

	case EModuleItem_Property:
		pValue->SetProperty ((CProperty*) pItem);

		if (((CProperty*) pItem)->IsMember ())
		{
			Result = CreateMemberClosure (pValue);
			if (!Result)
				return false;
		}

		break;

	case EModuleItem_AutoEv:
		pValue->SetAutoEv ((CAutoEv*) pItem);

		if (((CAutoEv*) pItem)->IsMember ())
		{
			Result = CreateMemberClosure (pValue);
			if (!Result)
				return false;
		}

		break;

	case EModuleItem_EnumConst:
		pValue->SetConstInt64 (
			((CEnumConst*) pItem)->GetValue (),
			((CEnumConst*) pItem)->GetParentEnumType ()->GetBaseType ()
			);
		break;

	case EModuleItem_StructField:
		return m_pModule->m_OperatorMgr.GetField (
			(CStructField*) pItem, 
			&Coord, 
			pValue
			);

	default:
		err::SetFormatStringError (_T("%s '%s' cannot be used as expression"), GetModuleItemKindString (pItem->GetItemKind ()), Name);
		return false;
	};

	return true;
}

bool
CParser::LookupIdentifierType (
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

	case EModuleItem_Typedef:
		pValue->SetType (((CTypedef*) pItem)->GetType ());
		break;

	case EModuleItem_Variable:
		pValue->SetType (((CVariable*) pItem)->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin));
		break;

	case EModuleItem_Alias:
		pValue->SetType (((CAlias*) pItem)->GetType ());
		break;

	case EModuleItem_Function:
		{
		CFunction* pFunction = (CFunction*) pItem;
		pValue->SetFunctionTypeOverload (pFunction->GetTypeOverload ());

		if (((CFunction*) pItem)->IsMember ())
		{
			Result = CreateMemberClosure (pValue);
			if (!Result)
				return false;
		}
		}
		break;

	case EModuleItem_Property:
		pValue->SetType (((CProperty*) pItem)->GetType ()->GetPropertyPtrType (EType_PropertyRef, EPropertyPtrType_Thin));

		if (((CProperty*) pItem)->IsMember ())
		{
			Result = CreateMemberClosure (pValue);
			if (!Result)
				return false;
		}

		break;

	case EModuleItem_AutoEv:
		pValue->SetType (((CAutoEv*) pItem)->GetType ()->GetAutoEvPtrType (EType_AutoEvRef, EAutoEvPtrType_Thin));

		if (((CAutoEv*) pItem)->IsMember ())
		{
			Result = CreateMemberClosure (pValue);
			if (!Result)
				return false;
		}

		break;

	case EModuleItem_EnumConst:
		pValue->SetType (((CEnumConst*) pItem)->GetParentEnumType ()->GetBaseType ());
		break;

	case EModuleItem_StructField:
		pValue->SetType (((CStructField*) pItem)->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin));
		break;
		
	default:
		err::SetFormatStringError (_T("%s '%s' cannot be used as expression"), GetModuleItemKindString (pItem->GetItemKind ()), Name);
		return false;
	};

	return true;
}

bool
CParser::CreateMemberClosure (CValue* pValue)
{
	CValue ThisValue;

	bool Result = pValue->GetValueKind () == EValue_Type ? 
		GetThisValueType (&ThisValue) : 
		GetThisValue (&ThisValue);

	if (!Result)
		return false;

	CClosure* pClosure = pValue->CreateClosure ();
	pClosure->GetArgList ()->InsertHead (ThisValue);
	return true;
}

bool
CParser::GetThisValue (CValue* pValue)
{
	*pValue = m_pModule->m_FunctionMgr.GetThisValue ();
	if (pValue->IsEmpty ())
	{
		err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag);
		return false;
	}

	return true;
}

bool
CParser::GetThisValueType (CValue* pValue)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	if (!pFunction->IsMember ())
	{
		err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag);
		return false;
	}

	pValue->SetType (pFunction->GetThisType ());
	return true;
}

bool
CParser::GetAuPropertyField (
	CValue* pValue,
	EAuPropertyField Field
	)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	if (!pFunction->m_pProperty)
	{
		err::SetFormatStringError (_T("function '%s' has no '%s' field"), pFunction->m_Tag, GetAuPropertyFieldString (Field));
		return false;
	}

	return m_pModule->m_OperatorMgr.GetAuPropertyField (pFunction->m_pProperty, Field, pValue);
}

bool
CParser::GetAuPropertyFieldType (
	CValue* pValue,
	EAuPropertyField Field
	)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	if (!pFunction->m_pProperty)
	{
		err::SetFormatStringError (_T("function '%s' has no '%s' field"), pFunction->m_Tag, GetAuPropertyFieldString (Field));
		return false;
	}

	CPropertyType* pPropertyType = pFunction->m_pProperty->GetType ();
	CStructField* pMember = pPropertyType->GetAuField (Field);
	if (!pMember)
	{
		err::SetFormatStringError (_T("'%s' has no '%s' field"), pPropertyType->GetTypeString (), GetAuPropertyFieldString (Field));
		return false;
	}

	pValue->SetType (pMember->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin));
	return true;
}

void
CParser::PreStatement ()
{
	m_pModule->m_ControlFlowMgr.ResetJumpFlag ();
}

bool
CParser::PrepareCurlyInitializerNamedItem (
	TCurlyInitializer* pInitializer, 
	const tchar_t* pName
	)
{
	CValue MemberValue;

	bool Result = m_pModule->m_OperatorMgr.MemberOperator (
		pInitializer->m_TargetValue, 
		pName, 
		&pInitializer->m_MemberValue
		);

	if (!Result)
		return false;

	pInitializer->m_Index = -1;
	pInitializer->m_Count++;
	m_CurlyInitializerTargetValue = pInitializer->m_MemberValue;
	return true;
}

bool
CParser::PrepareCurlyInitializerIndexedItem (TCurlyInitializer* pInitializer)
{
	if (pInitializer->m_Index == -1)
	{
		err::SetFormatStringError (_T("indexed-baded initializer cannot be used after named-based initializer"));
		return false;
	}

	bool Result = m_pModule->m_OperatorMgr.MemberOperator (
		pInitializer->m_TargetValue, 
		pInitializer->m_Index, 
		&pInitializer->m_MemberValue
		);

	if (!Result)
		return false;

	pInitializer->m_Index++;
	pInitializer->m_Count++;
	m_CurlyInitializerTargetValue = pInitializer->m_MemberValue;
	return true;
}

bool
CParser::SkipCurlyInitializerItem (TCurlyInitializer* pInitializer)
{
	if (pInitializer->m_Index == -1)
	{
		err::SetFormatStringError (_T("indexed-baded initializer cannot be used after named-based initializer"));
		return false;
	}

	pInitializer->m_Index++;
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
