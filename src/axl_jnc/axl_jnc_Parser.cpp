#include "pch.h"
#include "axl_jnc_Parser.llk.h"
#include "axl_jnc_Closure.h"

namespace axl {
namespace jnc {

//.............................................................................

CParser::CParser ()
{
	m_pModule = GetCurrentThreadModule ();
	m_Stage = EStage_Pass1;
	m_Flags = 0;
	m_StructPackFactor = 8;
	m_DefaultStructPackFactor = 8;
	m_StorageKind = EStorage_Undefined;
	m_AccessKind = EAccess_Undefined;
	m_pAttributeBlock = NULL;
	m_pLastDeclaredItem = NULL;
	m_pAutoEvType = NULL;
	m_AutoEvBindSiteCount = 0;
	m_AutoEvBindSiteTotalCount = 0;
	m_pConstructorType = NULL;
	m_pConstructorProperty = NULL;
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
		if (!Name.IsSimple ())
			return m_pModule->m_TypeMgr.GetNamedImportType (Name, pNamespace);

		rtl::CString ShortName = Name.GetShortName ();
		pItem = pNamespace->FindItem (ShortName);
		if (!pItem)
			return m_pModule->m_TypeMgr.GetNamedImportType (ShortName, pNamespace);				
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
			err::SetFormatStringError ("invalid declaration (no declarator, no named type)");
			return false;
		}

		if (pTypeSpecifier->GetTypeModifiers ())
		{
			err::SetFormatStringError ("unused modifier '%s'", GetTypeModifierString (pTypeSpecifier->GetTypeModifiers ()).cc ());
			return false;
		}

		return true;
	}
	else if (m_pLastDeclaredItem->GetFlags () & EModuleItemFlag_Orphan)
	{
		err::SetFormatStringError ("orphan '%s' without a body", m_pLastDeclaredItem->m_Tag.cc ());
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
			"more than one storage specifier specifiers ('%s' and '%s')", 
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
			"more than one access specifiers ('%s' and '%s')", 
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
		err::SetFormatStringError ("cannot open global namespace in '%s'", GetNamespaceKindString (pCurrentNamespace->GetNamespaceKind ()));
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
			err::SetFormatStringError ("'%s' exists and is not a namespace", pParentNamespace->CreateQualifiedName (Name).cc ());
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
		err::SetFormatStringError ("'%s' is not a type", Name.GetFullName ().cc ());
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

	case EType_NamedImport:
		err::SetFormatStringError ("extension namespaces for '%s' is not supported yet", pType->GetTypeString ().cc ());
		return false;

	default:
		err::SetFormatStringError ("'%s' could not have an extension namespace", pType->GetTypeString ().cc ());
		return false;
	}

	if (pNamedType->m_pExtensionNamespace)
	{
		m_pModule->m_NamespaceMgr.OpenNamespace (pNamedType->m_pExtensionNamespace);
		return true;
	}
	
	CGlobalNamespace* pNamespace = m_pModule->m_NamespaceMgr.CreateGlobalNamespace (
		"extension", 
		pNamedType->CreateQualifiedName ("extension")
		);

	pNamespace->m_NamespaceKind = ENamespace_TypeExtension;
	pNamespace->m_Pos = Pos;
	pNamespace->m_pParentNamespace = pNamedType;
	pNamedType->m_pExtensionNamespace = pNamespace;

	m_pModule->m_NamespaceMgr.OpenNamespace (pNamespace);
	return true;
}

bool
CParser::Declare (CDeclarator* pDeclarator)
{
	m_pLastDeclaredItem = NULL;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	uint_t PtrTypeFlags;
	CType* pType = pDeclarator->CalcType (&PtrTypeFlags);
	if (!pType)
		return false;

	EDeclarator DeclaratorKind = pDeclarator->GetDeclaratorKind ();
	uint_t PostModifiers = pDeclarator->GetPostDeclaratorModifiers ();
	EType TypeKind = pType->GetTypeKind ();

	if (PostModifiers != 0 && TypeKind != EType_Function)
	{
		err::SetFormatStringError ("unused post-declarator modifier '%s'", GetPostDeclaratorModifierString (PostModifiers).cc ());
		return false;
	}

	if (PtrTypeFlags && m_StorageKind == EStorage_Typedef)
	{
		err::SetFormatStringError ("unused modifier '%s'", GetPtrTypeFlagString (PtrTypeFlags).cc ());
		return false;
	}

	switch (m_StorageKind)
	{
	case EStorage_Typedef:
		return DeclareTypedef (pType, pDeclarator);

	case EStorage_Alias:
		return DeclareAlias (pType, pDeclarator);

	default:
		switch (TypeKind)
		{
		case EType_Function:
			return DeclareFunction ((CFunctionType*) pType, pDeclarator);

		case EType_Property:
			return DeclareProperty ((CPropertyType*) pType, pDeclarator, PtrTypeFlags);

		default:
			switch (DeclaratorKind)
			{
			case EDeclarator_PropValue:
				return DeclarePropValue (pType, pDeclarator, PtrTypeFlags);

			default:
				return IsClassType (pType, EClassType_AutoEvIface) ?
					DeclareAutoEv ((CAutoEvClassType*) pType, pDeclarator, PtrTypeFlags) :				
					DeclareData (pType, pDeclarator, PtrTypeFlags);
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

	pItem->m_Flags |= EModuleItemFlag_User;

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
		err::SetFormatStringError ("invalid typedef declarator");
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);

	CModuleItem* pItem;

	if (IsClassType (pType, EClassType_AutoEvIface))
	{
		pType = m_pModule->m_TypeMgr.CreateAutoEvType (Name, QualifiedName, (CClassType*) pType, NULL);
		pItem = pType;
	}
	else
	{
		CTypedef* pTypedef = m_pModule->m_TypeMgr.CreateTypedef (Name, QualifiedName, pType);
		pItem = pTypedef;
	}

	if (!pItem)
		return false;

	AssignDeclarationAttributes (pItem, pNamespace, pDeclarator->GetPos ());

	Result = pNamespace->AddItem (pItem, pItem->GetItemDecl ());
	if (!Result)
		return false;

	return true;
}

bool
CParser::DeclareAlias (
	CType* pType,
	CDeclarator* pDeclarator
	)
{
	ASSERT (m_StorageKind == EStorage_Alias);

	bool Result;

	if (pType->GetTypeKind () != EType_Void)
	{
		err::SetFormatStringError ("'alias' cannot have type");
		return false;
	}

	if (!pDeclarator->m_Constructor.IsEmpty ())
	{
		err::SetFormatStringError ("'alias' cannot have constructor");
		return false;
	}

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError ("invalid 'alias' declarator");
		return false;
	}

	if (pDeclarator->m_Initializer.IsEmpty ())
	{
		err::SetFormatStringError ("'alias' must have an initializer");
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
	rtl::CBoxListT <CToken>* pInitializer = &pDeclarator->m_Initializer;

	CParser Parser;
	Parser.m_pModule = m_pModule;
	Parser.m_Stage = EStage_Pass2;

	Result = Parser.ParseTokenList (ESymbol_expression_save_value_s, *pInitializer);
	if (!Result)
		return false;

	pType = Parser.m_ExpressionValue.GetType ();

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
	uint_t PostModifiers = pDeclarator->GetPostDeclaratorModifiers ();
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
	uint_t FunctionKindFlags = GetFunctionKindFlags (FunctionKind);

	if ((FunctionKindFlags & EFunctionKindFlag_NoStorage) && m_StorageKind)
	{
		err::SetFormatStringError ("'%s' cannot have storage specifier", GetFunctionKindString (FunctionKind));
		return false;
	}

	if ((FunctionKindFlags & EFunctionKindFlag_NoArgs) && HasArgs)
	{
		err::SetFormatStringError ("'%s' cannot have arguments", GetFunctionKindString (FunctionKind));
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
			err::SetFormatStringError ("invalid storage '%s' in property template", GetStorageKindString (m_StorageKind));
			return false;
		}

		if (PostModifiers)
		{
			err::SetFormatStringError ("unused post-declarator modifier '%s'", GetPostDeclaratorModifierString (PostModifiers).cc ());
			return false;
		}

		bool Result = ((CPropertyTemplate*) pNamespace)->AddMethod (FunctionKind, pType);
		if (!Result)
			return false;

		m_pLastDeclaredItem = pType;
		return true;
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
		pFunction->m_Tag.AppendFormat (".unary operator %s", GetUnOpKindString (pFunction->m_UnOpKind));
		break;

	case EFunction_BinaryOperator:
		pFunction->m_BinOpKind = pDeclarator->GetBinOpKind ();
		pFunction->m_Tag.AppendFormat (".binary operator %s", GetBinOpKindString (pFunction->m_BinOpKind));
		break;

	case EFunction_CastOperator:
		pFunction->m_pCastOpType = pDeclarator->GetCastOpType ();
		pFunction->m_Tag.AppendFormat (".cast operator %s", pFunction->m_pCastOpType);
		break;

	default:
		pFunction->m_Tag.AppendFormat (".%s", GetFunctionKindString (FunctionKind));
	}

	EType TypeKind;
	
	switch (NamespaceKind)
	{
	case ENamespace_TypeExtension:
		if (!pDeclarator->IsSimple ())
		{
			err::SetFormatStringError ("invalid declarator '%s' in type extension", pFunction->m_Tag.cc ());
			return false;
		}

		if (pFunction->IsVirtual ())
		{
			err::SetFormatStringError ("invalid storage '%s' in type extension", GetStorageKindString (pFunction->m_StorageKind));
			return false;
		}

		break;

	case ENamespace_Type:
		if (pDeclarator->IsQualified () && m_StorageKind != EStorage_Override)
		{
			err::SetFormatStringError ("only overrides could be qualified, '%s' is not an override", pFunction->m_Tag.cc ());
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
			err::SetFormatStringError ("method members are not allowed in '%s'", ((CNamedType*) pNamespace)->GetTypeString ().cc ());
			return false;
		}

	case ENamespace_Property:
		if (pDeclarator->IsQualified ())
		{
			err::SetFormatStringError ("invalid qualified declarator '%s' in property", pFunction->m_Tag.cc ());
			return false;
		}

		return ((CProperty*) pNamespace)->AddMethod (pFunction);

	default:
		if (PostModifiers)
		{
			err::SetFormatStringError ("unused post-declarator modifier '%s'", GetPostDeclaratorModifierString (PostModifiers).cc ());
			return false;
		}

		if (m_StorageKind)
		{
			err::SetFormatStringError ("invalid storage specifier '%s' for a global function", GetStorageKindString (m_StorageKind));
			return false;
		}

		if (pDeclarator->IsQualified ())
		{
			pFunction->m_Flags |= EModuleItemFlag_Orphan;
			return true;
		}

		pFunction->m_StorageKind = EStorage_Static;
	}

	if (!pNamespace->GetParentNamespace ()) // module constructor / destructor
		switch (FunctionKind)
		{
		case EFunction_Constructor:
			return pFunction->GetModule ()->SetConstructor (pFunction);

		case EFunction_Destructor:
			return pFunction->GetModule ()->SetDestructor (pFunction);
		}

	if (FunctionKind != EFunction_Named)
	{
		err::SetFormatStringError (
			"invalid '%s' at '%s' namespace", 
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
	CDeclarator* pDeclarator,
	uint_t PtrTypeFlags
	)
{
	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError ("invalid property declarator");
		return false;
	}

	CProperty* pProperty = CreateProperty (
		pDeclarator->GetName ()->GetShortName (), 
		pDeclarator->GetPos ()
		);

	if (!pProperty)
		return false;

	pProperty->m_PtrTypeFlags = PtrTypeFlags;
	return pProperty->Create (pType);
}

CPropertyTemplate*
CParser::CreatePropertyTemplate ()
{
	CPropertyTemplate* pPropertyTemplate = m_pModule->m_FunctionMgr.CreatePropertyTemplate ();
	uint_t Modifiers = GetTypeSpecifier ()->ClearTypeModifiers (ETypeModifier_Property | ETypeModifier_Bindable);

	if (Modifiers & ETypeModifier_Bindable)
		pPropertyTemplate->m_TypeFlags = EPropertyTypeFlag_Bindable;

	return pPropertyTemplate;
}

CProperty*
CParser::CreateProperty (
	const rtl::CString& Name,
	const CToken::CPos& Pos
	)
{
	bool Result;

	m_pLastDeclaredItem = NULL;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		err::SetFormatStringError ("property templates cannot have property members");
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
			err::SetFormatStringError ("property members are not allowed in '%s'", ((CNamedType*) pNamespace)->GetTypeString ().cc ());
			return NULL;
		}

		if (!Result)
			return NULL;

		break;

	case ENamespace_Property:
		Result = ((CProperty*) pNamespace)->AddProperty (pProperty);
		if (!Result)
			return NULL;

		break;

	default:
		if (m_StorageKind && m_StorageKind != EStorage_Static)
		{
			err::SetFormatStringError ("invalid storage specifier '%s' for a global property", GetStorageKindString (m_StorageKind));
			return NULL;
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
	CAutoEvClassType* pType,
	CDeclarator* pDeclarator,
	uint_t PtrTypeFlags
	)
{
	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError ("invalid autoev declarator");
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	CNamedType* pParentType = NULL;

	switch (NamespaceKind)
	{
	case ENamespace_Property:
		pParentType = ((CProperty*) pNamespace)->GetParentType ();
		break;

	case ENamespace_Type:
		pParentType = (CNamedType*) pNamespace;
		break;
	}

	if (pParentType && pParentType->GetTypeKind () != EType_Class)
	{
		err::SetFormatStringError ("'%s' cannot contain autoev members", pParentType->GetTypeString ().cc ());
		return false;
	}

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
	
	pType = m_pModule->m_TypeMgr.CreateAutoEvType (Name, QualifiedName, (CAutoEvClassType*) pType, (CClassType*) pParentType);
	AssignDeclarationAttributes (pType, pNamespace, pDeclarator->GetPos ());

	return DeclareData (pType, pDeclarator, PtrTypeFlags);
}

bool
CParser::DeclareData (	
	CType* pType,
	CDeclarator* pDeclarator,
	uint_t PtrTypeFlags
	)
{
	bool Result;

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError ("invalid data declarator");
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	switch (NamespaceKind)
	{
	case ENamespace_TypeExtension:
	case ENamespace_PropertyTemplate:
		err::SetFormatStringError ("'%s' cannot have data fields", GetNamespaceKindString (NamespaceKind));
		return false;
	}

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	size_t BitCount = pDeclarator->GetBitCount ();
	rtl::CBoxListT <CToken>* pConstructor = &pDeclarator->m_Constructor;
	rtl::CBoxListT <CToken>* pInitializer = &pDeclarator->m_Initializer;

	if (IsAutoSizeArrayType (pType))
	{
		if (pInitializer->IsEmpty ())
		{
			err::SetFormatStringError ("auto-size array '%s' should have initializer", pType->GetTypeString ().cc ());
			return false;
		}

		CArrayType* pArrayType = (CArrayType*) pType;
		Result = m_pModule->m_OperatorMgr.ParseAutoSizeArrayInitializer (*pInitializer, &pArrayType->m_ElementCount);
		if (!Result)
			return false;

		if (m_Stage == EStage_Pass2)
		{
			Result = pArrayType->EnsureLayout ();
			if (!Result)
				return false;
		}
	}

	CModuleItem* pDataItem = NULL;

	if (NamespaceKind == ENamespace_Property)
	{
		CProperty* pProperty = (CProperty*) pNamespace;

		if (pProperty->GetParentType ())
		{
			pDataItem = pProperty->CreateField (Name, pType, BitCount, PtrTypeFlags, pConstructor, pInitializer);
			AssignDeclarationAttributes (pDataItem, pNamespace, pDeclarator->GetPos ());
		}
		else
		{
			CVariable* pVariable = m_pModule->m_VariableMgr.CreateVariable (
				EStorage_Static,
				Name, 
				pNamespace->CreateQualifiedName (Name),
				pType, 
				PtrTypeFlags,
				pConstructor,
				pInitializer
				);

			AssignDeclarationAttributes (pVariable, pNamespace, pDeclarator->GetPos ());

			Result = pNamespace->AddItem (pVariable);
			if (!Result)
				return false;

			pDataItem = pVariable;
		}
	}
	else if (
		NamespaceKind != ENamespace_Type || 
		m_StorageKind == EStorage_Static ||
		m_StorageKind == EStorage_Thread
		)
	{
		CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();

		switch (m_StorageKind)
		{
		case EStorage_Undefined:
			m_StorageKind = pScope ? pType->GetTypeKind () == EType_Class ? 
				EStorage_Heap : 
				EStorage_Stack : 
				EStorage_Static;
			break;

		case EStorage_Static:
			break;

		case EStorage_Thread:
			if (!pScope && (!pConstructor->IsEmpty () || !pInitializer->IsEmpty ()))
			{
				err::SetFormatStringError ("global 'thread' variables cannot have constructors or initializers");
				return false;
			}

			break;

		case EStorage_Heap:
		case EStorage_Stack:
			if (!pScope)
			{
				err::SetFormatStringError ("can only use '%s' storage specifier for local variables", GetStorageKindString (m_StorageKind));
				return false;
			}

			break;

		default:
			err::SetFormatStringError ("invalid storage specifier '%s' for variable", GetStorageKindString (m_StorageKind));
			return false;
		}

		CVariable* pVariable = m_pModule->m_VariableMgr.CreateVariable (
			m_StorageKind,
			Name, 
			pNamespace->CreateQualifiedName (Name),
			pType, 
			PtrTypeFlags,
			pConstructor,
			pInitializer
			);

		AssignDeclarationAttributes (pVariable, pNamespace, pDeclarator->GetPos ());

		Result = pNamespace->AddItem (pVariable);
		if (!Result)
			return false;

		if (pScope)
		{
			pVariable->m_pScope = pScope;

			Result = m_pModule->m_VariableMgr.AllocatePrimeInitializeVariable (pVariable);
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
			pField = ((CClassType*) pNamedType)->CreateField (Name, pType, BitCount, PtrTypeFlags, pConstructor, pInitializer);
			break;

		case EType_Struct:
			pField = ((CStructType*) pNamedType)->CreateField (Name, pType, BitCount, PtrTypeFlags, pConstructor, pInitializer);
			break;

		case EType_Union:
			pField = ((CUnionType*) pNamedType)->CreateField (Name, pType, BitCount, PtrTypeFlags, pConstructor, pInitializer);
			break;

		default:
			err::SetFormatStringError ("field members are not allowed in '%s'", pNamedType->GetTypeString ().cc ());
			return false;
		}

		if (!pField)
			return false;

		AssignDeclarationAttributes (pField, pNamespace, pDeclarator->GetPos ());
	}

	return true;
}

bool
CParser::DeclarePropValue (	
	CType* pType,
	CDeclarator* pDeclarator,
	uint_t PtrTypeFlags
	)
{
	if (m_StorageKind)
	{
		err::SetFormatStringError ("'propvalue' cannot have a storage specifier");
		return false;
	}

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	CFunctionType* pGetterType = m_pModule->m_TypeMgr.GetFunctionType (pType, NULL, 0, 0);

	if (NamespaceKind != ENamespace_Property)
	{
		err::SetFormatStringError ("'propvalue' is only allowed in property body");
		return false;
	}

	size_t BitCount = pDeclarator->GetBitCount ();
	rtl::CBoxListT <CToken>* pConstructor = &pDeclarator->m_Constructor;
	rtl::CBoxListT <CToken>* pInitializer = &pDeclarator->m_Initializer;
	
	CProperty* pProperty = (CProperty*) pNamespace;
	bool Result = pProperty->CreatePropValue (pType, PtrTypeFlags, BitCount, pConstructor, pInitializer);
	if (!Result)
		return false;

	m_StorageKind = pProperty->GetStorageKind ();
	AssignDeclarationAttributes (pProperty->GetPropValue (), pNamespace, m_LastMatchedToken.m_Pos);
	return true;
}

CFunctionArg*
CParser::CreateFormalArg (
	CDeclFunctionSuffix* pArgSuffix,
	CDeclarator* pDeclarator
	)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	uint_t PtrTypeFlags = 0;
	CType* pType = pDeclarator->CalcType (&PtrTypeFlags);
	if (!pType)
		return NULL;

	switch (m_StorageKind)
	{
	case EStorage_Undefined:
		m_StorageKind = EStorage_Stack;
		// and fall through

	case EStorage_Stack:
		if (pType->GetTypeKindFlags () & ETypeKindFlag_Ptr)
			pType = m_pModule->m_TypeMgr.GetCheckedPtrType (pType);

		break;

	case EStorage_Nullable:
		if (!(pType->GetTypeKindFlags () & ETypeKindFlag_Ptr))
		{
			err::SetFormatStringError ("'nullable' can only be applied to pointers");
			return NULL;
		}

		break;

	default:
		err::SetFormatStringError ("invalid storage '%s' for argument", GetStorageKindString (m_StorageKind));
		return NULL;
	}

	rtl::CString Name;

	if (pDeclarator->IsSimple ())
	{
		Name = pDeclarator->GetName ()->GetShortName ();
	}
	else if (pDeclarator->GetDeclaratorKind () != EDeclarator_Undefined)
	{
		err::SetFormatStringError ("invalid formal argument declarator");
		return NULL;
	}

	rtl::CBoxListT <CToken>* pInitializer = &pDeclarator->m_Initializer;

	CFunctionArg* pArg = m_pModule->m_TypeMgr.CreateFunctionArg (Name, pType, PtrTypeFlags, pInitializer);
	AssignDeclarationAttributes (pArg, pNamespace, pDeclarator->GetPos ());

	pArgSuffix->m_ArgArray.Append (pArg);

	return pArg;
}

CEnumType*
CParser::CreateEnumType (
	const rtl::CString& Name,
	CType* pBaseType,
	uint_t Flags
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
		rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
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
		rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
		pStructType = m_pModule->m_TypeMgr.CreateStructType (Name, QualifiedName, PackFactor);
		if (!pStructType)
			return NULL;
	}

	if (pBaseTypeList)
	{
		rtl::CBoxIteratorT <CType*> BaseType = pBaseTypeList->GetHead ();
		for (; BaseType; BaseType++)
		{
			Result = pStructType->AddBaseType (*BaseType) != NULL;
			if (!Result)
				return NULL;
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
		rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
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
		rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
		pClassType = m_pModule->m_TypeMgr.CreateClassType (Name, QualifiedName, PackFactor);
	}

	if (pBaseTypeList)
	{
		rtl::CBoxIteratorT <CType*> BaseType = pBaseTypeList->GetHead ();
		for (; BaseType; BaseType++)
		{
			Result = pClassType->AddBaseType (*BaseType) != NULL;
			if (!Result)
				return NULL;
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

bool
CParser::CountAutoEvBindSites ()
{
	if (!m_AutoEvBindSiteCount)
	{
		err::SetFormatStringError ("no bindable sites found");
		return false;
	}

	m_AutoEvBindSiteTotalCount += m_AutoEvBindSiteCount;
	m_AutoEvBindSiteCount = 0;
	return true;
}

bool
CParser::FinalizeAutoEv ()
{
	ASSERT (m_pAutoEvType);
	ASSERT (!m_AutoEvHandlerList.IsEmpty ());

	bool Result = m_pAutoEvType->BindHandlers (m_AutoEvHandlerList);
	if (!Result)
		return false;

	m_AutoEvHandlerList.Clear ();
	return true;
}

bool
CParser::FinalizeAutoEvOnChangeClause ()
{
	ASSERT (m_pAutoEvType);

	if (m_AutoEvBindSiteList.IsEmpty ())
	{
		err::SetFormatStringError ("no bindable sites found");
		return false;
	}
	
	TAutoEvHandler* pHandler = AXL_MEM_NEW (TAutoEvHandler);
	pHandler->m_pFunction = m_pAutoEvType->CreateHandler ();
	pHandler->m_BindSiteList.TakeOver (&m_AutoEvBindSiteList);
	m_AutoEvHandlerList.InsertTail (pHandler);

	return m_pModule->m_FunctionMgr.Prologue (pHandler->m_pFunction, m_LastMatchedToken.m_Pos);
}

bool
CParser::AutoEvExpressionStmt (rtl::CBoxListT <CToken>* pTokenList)
{
	ASSERT (m_pAutoEvType);
	ASSERT (!pTokenList->IsEmpty ());	

	bool Result;

	ASSERT (m_pAutoEvType);

	CParser Parser;
	Parser.m_pModule = m_pModule;
	Parser.m_Stage = EStage_Pass2;
	Parser.m_pAutoEvType = m_pAutoEvType;

	Result = Parser.ParseTokenList (ESymbol_expression, *pTokenList);
	if (!Result)
		return false;

	if (Parser.m_AutoEvBindSiteList.IsEmpty ())
	{
		err::SetFormatStringError ("no bindable sites found");
		return false;
	}

	TAutoEvHandler* pHandler = AXL_MEM_NEW (TAutoEvHandler);
	pHandler->m_pFunction = m_pAutoEvType->CreateHandler ();
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
CParser::CallBaseTypeMemberConstructor (
	const CQualifiedName& Name,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	ASSERT (m_pConstructorType || m_pConstructorProperty);

	CNamespace* pNamespace = m_pModule->m_FunctionMgr.GetCurrentFunction ()->GetParentNamespace ();
	CModuleItem* pItem = pNamespace->FindItem (Name);
	if (!pItem)
	{
		err::SetFormatStringError ("name '%s' is not found", Name.GetFullName ().cc ());
		return false;
	}

	CType* pType = NULL;
	
	EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Type:
		return CallBaseTypeConstructor ((CType*) pItem, pArgList);
	
	case EModuleItem_Typedef:
		return CallBaseTypeConstructor (((CTypedef*) pItem)->GetType (), pArgList);

	case EModuleItem_Property:
		err::SetFormatStringError ("property construction is not yet implemented");
		return false;

	case EModuleItem_StructField:
		return CallFieldConstructor ((CStructField*) pItem, pArgList);

	case EModuleItem_Variable:
		err::SetFormatStringError ("static field construction is not yet implemented");
		return false;

	default:
		err::SetFormatStringError ("'%s' cannot be used in base-type-member construct list");
		return false;
	}
}

bool
CParser::CallBaseTypeConstructor (
	CType* pType,
	rtl::CBoxListT <CValue>* pArgList
	)
{	
	ASSERT (m_pConstructorType || m_pConstructorProperty);

	bool Result;

	if (m_pConstructorProperty)
	{
		err::SetFormatStringError ("'%s.construct' cannot have base-type constructor calls", m_pConstructorProperty->m_Tag.cc ());
		return false;
	}

	CBaseTypeSlot* pBaseTypeSlot = m_pConstructorType->FindBaseType (pType);
	if (!pBaseTypeSlot)
	{
		err::SetFormatStringError (
			"'%s' is not a base type of '%s'", 
			pType->GetTypeString ().cc (), 
			m_pConstructorType->GetTypeString ().cc ()
			);
		return false;
	}

	if (pBaseTypeSlot->m_Flags & EModuleItemFlag_Constructed)
	{
		err::SetFormatStringError ("'%s' is already constructed", pType->GetTypeString ().cc ());
		return false;
	}

	CFunction* pConstructor = pBaseTypeSlot->GetType ()->GetConstructor ();
	if (!pConstructor)
	{
		err::SetFormatStringError ("'%s' has no constructor", pType->GetTypeString ().cc ());
		return false;
	}

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	ASSERT (ThisValue);

	pArgList->InsertHead (ThisValue);

	Result = m_pModule->m_OperatorMgr.CallOperator (pConstructor, pArgList);
	if (!Result)
		return false;

	pBaseTypeSlot->m_Flags |= EModuleItemFlag_Constructed;
	return true;
}

bool
CParser::CallFieldConstructor (
	CStructField* pField,
	rtl::CBoxListT <CValue>* pArgList
	)
{
	ASSERT (m_pConstructorType || m_pConstructorProperty);

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
	ASSERT (ThisValue);

	bool Result;

	if (m_pConstructorProperty)
	{
		err::SetFormatStringError ("property field construction is not yet implemented");
		return false;
	}

	if (pField->GetParentNamespace () != m_pConstructorType)
	{
		err::SetFormatStringError (
			"'%s' is not an immediate field of '%s'", 
			pField->GetName ().cc (),
			m_pConstructorType->GetTypeString ().cc ()
			);
		return false;
	}

	if (pField->GetFlags () & EModuleItemFlag_Constructed)
	{
		err::SetFormatStringError ("'%s' is already constructed", pField->GetName ().cc ());
		return false;
	}

	if (!(pField->GetType ()->GetTypeKindFlags () & ETypeKindFlag_Derivable) ||
		!((CDerivableType*) pField->GetType ())->GetConstructor ())
	{
		err::SetFormatStringError ("'%s' has no constructor", pField->GetName ().cc ());
		return false;
	}

	CFunction* pConstructor = ((CDerivableType*) pField->GetType ())->GetConstructor ();

	CValue FieldValue;
	Result = 
		m_pModule->m_OperatorMgr.GetField (ThisValue, pField, NULL, &FieldValue) &&
		m_pModule->m_OperatorMgr.UnaryOperator (EUnOp_Addr, &FieldValue);

	if (!Result)
		return false;

	pArgList->InsertHead (FieldValue);

	Result = m_pModule->m_OperatorMgr.CallOperator (pConstructor, pArgList);
	if (!Result)
		return false;

	pField->m_Flags |= EModuleItemFlag_Constructed;
	return true;
}

bool
CParser::FinalizeBaseTypeMemberConstructBlock ()
{
	ASSERT (m_pConstructorType || m_pConstructorProperty);

	CValue ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();

	bool Result;

	if (m_pConstructorProperty)
		return 
			m_pConstructorProperty->CallMemberPropertyConstructors (ThisValue) &&
			m_pConstructorProperty->CallMemberFieldConstructors (ThisValue);
	
	ASSERT (ThisValue);

	Result = 
		m_pConstructorType->CallBaseTypeConstructors (ThisValue) &&
		m_pConstructorType->CallMemberPropertyConstructors (ThisValue) &&
		m_pConstructorType->CallMemberFieldConstructors (ThisValue);

	if (!Result)
		return false;
	
	CFunction* pPreConstructor = m_pConstructorType->GetPreConstructor ();
	if (!pPreConstructor)
		return true;
		
	return m_pModule->m_OperatorMgr.CallOperator (pPreConstructor, ThisValue);
}

bool
CParser::NewOperator_s (
	EStorage StorageKind, 
	CType* pType, 
	CValue* pResultValue
	)
{
	pResultValue->SetType (m_pModule->m_OperatorMgr.GetNewOperatorResultType (pType));
	return true;
}

bool
CParser::SetFunctionBody (rtl::CBoxListT <CToken>* pBody)
{
	if (!m_pLastDeclaredItem)
	{
		err::SetFormatStringError ("declaration without declarator cannot have a body");
		return false;
	}

	CType* pType;

	EModuleItem ItemKind = m_pLastDeclaredItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Function:
		return ((CFunction*) m_pLastDeclaredItem)->SetBody (pBody);

	case EModuleItem_Typedef:
		pType = ((CTypedef*) m_pLastDeclaredItem)->GetType ();
		break;

	case EModuleItem_Type:
		pType = (CType*) m_pLastDeclaredItem;
		break;

	case EModuleItem_Variable:
		pType = ((CVariable*) m_pLastDeclaredItem)->GetType ();
		break;

	case EModuleItem_StructField:
		pType = ((CStructField*) m_pLastDeclaredItem)->GetType ();
		break;

	default:
		err::SetFormatStringError ("'%s' cannot have a body", GetModuleItemKindString (m_pLastDeclaredItem->GetItemKind ()));
		return false;
	}

	if (!IsClassType (pType, EClassType_AutoEv))
	{
		err::SetFormatStringError ("only functions and autoevs can have bodies, not '%s'", pType->GetTypeString ().cc ());
		return false;
	}
	
	return ((CAutoEvClassType*) pType)->SetBody (pBody);
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
		if (Name == "__ScopeLevel") // tmp
			return m_pModule->m_OperatorMgr.CalcScopeLevelValue (m_pModule->m_NamespaceMgr.GetCurrentScope (), pValue);

		err::SetFormatStringError ("undeclared identifier '%s'", Name.cc ());
		return false;
	}

	CValue ThisValue;

	EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Namespace:
		pValue->SetNamespace ((CGlobalNamespace*) pItem);
		break;

	case EModuleItem_Typedef:
		pItem = ((CTypedef*) pItem)->GetType ();
		// and fall through

	case EModuleItem_Type:	
		if (!(((CType*) pItem)->GetTypeKindFlags () & ETypeKindFlag_Named))
		{
			err::SetFormatStringError ("'%s' cannot be used as expression", ((CType*) pItem)->GetTypeString ().cc ());
			return false;
		}

		pValue->SetNamespace ((CNamedType*) pItem);
		break;

	case EModuleItem_Alias:
		return m_pModule->m_OperatorMgr.EvaluateAlias (((CAlias*) pItem)->GetInitializer (), pValue);

	case EModuleItem_Variable:
		if (m_Flags & EFlag_ConstExpression)
		{
			err::SetFormatStringError ("variable '%s' cannot be used in const expression", Name.cc ());
			return false;
		}

		pValue->SetVariable ((CVariable*) pItem);
		break;

	case EModuleItem_Function:
		if (m_Flags & EFlag_ConstExpression)
		{
			err::SetFormatStringError ("function '%s' cannot be used in const expression", Name.cc ());
			return false;
		}

		pValue->SetFunction ((CFunction*) pItem);

		if (((CFunction*) pItem)->IsMember ())
		{
			Result = CreateMemberClosure (pValue);
			if (!Result)
				return false;
		}

		break;

	case EModuleItem_Property:
		if (m_Flags & EFlag_ConstExpression)
		{
			err::SetFormatStringError ("property '%s' cannot be used in const expression", Name.cc ());
			return false;
		}

		pValue->SetProperty ((CProperty*) pItem);

		if (((CProperty*) pItem)->IsMember ())
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
		if (m_Flags & EFlag_ConstExpression)
		{
			err::SetFormatStringError ("field '%s' cannot be used in const expression", Name.cc ());
			return false;
		}

		ThisValue = m_pModule->m_FunctionMgr.GetThisValue ();
		if (!ThisValue)
		{
			err::SetFormatStringError (
				"function '%s' has no 'this' pointer", 
				m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag.cc () // thanks a lot gcc
				);
			return false;
		}

		if (Coord.m_ParentNamespaceLevel)
		{
			if (Coord.m_ParentNamespaceLevel > 1 ||				
				!m_pAutoEvType || 
				!m_pAutoEvType->GetField (EAutoEvField_Parent)
				)
			{
				err::SetFormatStringError (
					"'%s' is not accessible from '%s'", 
					Name.cc (),
					m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag.cc ()
					);
				return false;
			}
			
			CStructField* pParentField = m_pAutoEvType->GetField (EAutoEvField_Parent);
			Result = m_pModule->m_OperatorMgr.GetField (&ThisValue, pParentField);
			if (!Result)
				return false;
		}

		return m_pModule->m_OperatorMgr.GetField (
			ThisValue,
			(CStructField*) pItem, 
			&Coord, 
			pValue
			);

	default:
		err::SetFormatStringError (
			"%s '%s' cannot be used as expression", 
			GetModuleItemKindString (pItem->GetItemKind ()), 
			Name.cc ()
			);
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
		err::SetFormatStringError ("undeclared identifier '%s'", Name.cc ());
		return false;
	}

	EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Namespace:
		pValue->SetNamespace ((CGlobalNamespace*) pItem);
		break;

	case EModuleItem_Typedef:
		pItem = ((CTypedef*) pItem)->GetType ();
		// and fall through

	case EModuleItem_Type:	
		if (!(((CType*) pItem)->GetTypeKindFlags () & ETypeKindFlag_Named))
		{
			err::SetFormatStringError ("'%s' cannot be used as expression", ((CType*) pItem)->GetTypeString ().cc ());
			return false;
		}

		pValue->SetNamespace ((CNamedType*) pItem);
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

	case EModuleItem_EnumConst:
		pValue->SetType (((CEnumConst*) pItem)->GetParentEnumType ()->GetBaseType ());
		break;

	case EModuleItem_StructField:
		pValue->SetType (((CStructField*) pItem)->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin));
		break;
		
	default:
		err::SetFormatStringError ("'%s' cannot be used as expression", Name.cc ());
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

	pValue->InsertToClosureHead (ThisValue);
	return true;
}


bool
CParser::GetCountOf (
	CType* pType,
	CValue* pValue
	)
{
	if (pType->GetTypeKind () != EType_Array)
	{
		err::SetFormatStringError ("'countof' operator is only applicable to arrays, not to '%s'", pType->GetTypeString ().cc ());
		return false;
	}

	pValue->SetConstSizeT (((CArrayType*) pType)->GetElementCount ());
	return true;
}

bool
CParser::GetThisValue (CValue* pValue)
{
	*pValue = m_pModule->m_FunctionMgr.GetThisValue ();
	if (pValue->IsEmpty ())
	{
		err::SetFormatStringError ("function '%s' has no 'this' pointer", m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag.cc ());
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
		err::SetFormatStringError ("function '%s' has no 'this' pointer", m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag.cc ());
		return false;
	}

	pValue->SetType (pFunction->GetThisType ());
	return true;
}

bool
CParser::GetOnChange (CValue* pValue)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	if (!pFunction->m_pProperty)
	{
		err::SetFormatStringError (
			"function '%s' has no 'onchange' field", 
			pFunction->m_Tag.cc ()
			);
		return false;
	}

	return m_pModule->m_OperatorMgr.GetPropertyOnChange (pFunction->m_pProperty, pValue);
}

bool
CParser::GetOnChangeType (CValue* pValue)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	if (!pFunction->m_pProperty)
	{
		err::SetFormatStringError (
			"function '%s' has no 'onchange' field", 
			pFunction->m_Tag.cc ()
			);
		return false;
	}

	return m_pModule->m_OperatorMgr.GetPropertyOnChangeType (pFunction->m_pProperty, pValue);
}

bool
CParser::GetPropValue (CValue* pValue)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	if (!pFunction->m_pProperty)
	{
		err::SetFormatStringError (
			"function '%s' has no 'propvalue' field", 
			pFunction->m_Tag.cc ()
			);
		return false;
	}

	return m_pModule->m_OperatorMgr.GetPropertyPropValue (pFunction->m_pProperty, pValue);
}

bool
CParser::GetPropValueType (CValue* pValue)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	if (!pFunction->m_pProperty)
	{
		err::SetFormatStringError (
			"function '%s' has no 'propvalue' field", 
			pFunction->m_Tag.cc ()
			);
		return false;
	}

	return m_pModule->m_OperatorMgr.GetPropertyPropValueType (pFunction->m_pProperty, pValue);
}

bool
CParser::PrepareCurlyInitializerNamedItem (
	TCurlyInitializer* pInitializer, 
	const char* pName
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
		err::SetFormatStringError ("indexed-baded initializer cannot be used after named-based initializer");
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
		err::SetFormatStringError ("indexed-baded initializer cannot be used after named-based initializer");
		return false;
	}

	pInitializer->m_Index++;
	return true;
}

bool
CParser::AppendFmtLiteral (
	TLiteral* pLiteral,
	const void* p,
	size_t Length
	)
{
	bool Result;

	if (!pLiteral->m_FmtLiteralValue)
	{
		Result = m_pModule->m_OperatorMgr.NewOperator (
			EStorage_Stack, 
			GetSimpleType (m_pModule, EStdType_FmtLiteral),
			NULL,
			&pLiteral->m_FmtLiteralValue
			);

		if (!Result)
			return false;
	}

	CFunction* pAppend = m_pModule->m_FunctionMgr.GetStdFunction (EStdFunc_AppendFmtLiteral_a);

	pLiteral->m_BinData.Append ((uchar_t*) p, Length); 
	Length = pLiteral->m_BinData.GetCount ();

	CValue LiteralValue;
	LiteralValue.SetCharArray (pLiteral->m_BinData, Length);
	Result = m_pModule->m_OperatorMgr.CastOperator (&LiteralValue, GetSimpleType (m_pModule, EType_Char)->GetDataPtrType_c ());
	if (!Result)
		return false;

	pLiteral->m_BinData.Clear ();

	CValue LengthValue;
	LengthValue.SetConstSizeT (Length);

	CValue ResultValue;
	m_pModule->m_LlvmIrBuilder.CreateCall3 (
		pAppend,
		pAppend->GetType (),
		pLiteral->m_FmtLiteralValue,
		LiteralValue,
		LengthValue,
		&ResultValue
		);

	return true;
}

bool
CParser::AppendFmtLiteralValue (
	TLiteral* pLiteral,
	const CValue& RawSrcValue,
	const rtl::CString& FmtSpecifierString
	)
{
	ASSERT (pLiteral->m_FmtLiteralValue);

	EStdFunc AppendFunc;

	CValue SrcValue;
	bool Result = m_pModule->m_OperatorMgr.PrepareOperand (RawSrcValue, &SrcValue);
	if (!Result)
		return false;
	
	CType* pType = SrcValue.GetType ();
	if (pType->GetTypeKindFlags () & ETypeKindFlag_Integer)
	{
		static EStdFunc FuncTable [2] [2] = 
		{
			{ EStdFunc_AppendFmtLiteral_i32, EStdFunc_AppendFmtLiteral_ui32 }, 
			{ EStdFunc_AppendFmtLiteral_i64, EStdFunc_AppendFmtLiteral_ui64 },
		};

		size_t i1 = pType->GetSize () > 4;
		size_t i2 = (pType->GetTypeKindFlags () & ETypeKindFlag_Unsigned) != 0;

		AppendFunc = FuncTable [i1] [i2];
	}
	else if (pType->GetTypeKindFlags () & ETypeKindFlag_Fp)
	{
		AppendFunc = EStdFunc_AppendFmtLiteral_f;
	}
	else if (IsCharArrayType (pType) || IsCharPtrType (pType))
	{
		AppendFunc = EStdFunc_AppendFmtLiteral_p;
	}
	else
	{
		err::SetFormatStringError ("don't know how to format '%s'", pType->GetTypeString ().cc ());
		return false;
	}

	CFunction* pAppend = m_pModule->m_FunctionMgr.GetStdFunction (AppendFunc);	
	CType* pArgType = pAppend->GetType ()->GetArgArray () [2]->GetType ();

	CValue ArgValue;
	Result = m_pModule->m_OperatorMgr.CastOperator (SrcValue, pArgType, &ArgValue);
	if (!Result)
		return false;

	CValue FmtSpecifierValue;
	if (!FmtSpecifierString.IsEmpty ())
	{
		FmtSpecifierValue.SetCharArray (FmtSpecifierString, FmtSpecifierString.GetLength () + 1);
		m_pModule->m_OperatorMgr.CastOperator (&FmtSpecifierValue, GetSimpleType (m_pModule, EType_Char)->GetDataPtrType_c ());
	}
	else
	{
		FmtSpecifierValue = GetSimpleType (m_pModule, EType_Char)->GetDataPtrType_c ()->GetZeroValue ();
	}

	CValue ResultValue;
	m_pModule->m_LlvmIrBuilder.CreateCall3 (
		pAppend,
		pAppend->GetType (),
		pLiteral->m_FmtLiteralValue,
		FmtSpecifierValue,
		ArgValue,
		&ResultValue
		);

	return true;
}

bool
CParser::FinalizeLiteral (
	TLiteral* pLiteral,
	CValue* pResultValue
	)
{
	if (!pLiteral->m_FmtLiteralValue)
	{
		if (pLiteral->m_LastToken == EToken_Literal)
			pLiteral->m_BinData.Append (0);

		pResultValue->SetCharArray (pLiteral->m_BinData, pLiteral->m_BinData.GetCount ());	
		return true;
	}
	
	AppendFmtLiteral (pLiteral, NULL, 0);
	
	CValue PtrValue;
	CValue SizeValue;

	m_pModule->m_LlvmIrBuilder.CreateGep2 (pLiteral->m_FmtLiteralValue, 0, NULL, &PtrValue);
	m_pModule->m_LlvmIrBuilder.CreateLoad (PtrValue, NULL, &PtrValue);

	m_pModule->m_LlvmIrBuilder.CreateGep2 (pLiteral->m_FmtLiteralValue, 2, NULL, &SizeValue);
	m_pModule->m_LlvmIrBuilder.CreateLoad (SizeValue, NULL, &SizeValue);

	pResultValue->SetThinDataPtr (
		PtrValue.GetLlvmValue (),
		GetSimpleType (m_pModule, EType_Char)->GetDataPtrType (EDataPtrType_Thin),
		CValue ((int64_t) 0, GetSimpleType (m_pModule, EType_SizeT)),
		PtrValue,
		SizeValue
		);	 

	return true;
}

bool
CParser::FinalizeLiteral_s (
	TLiteral* pLiteral,
	CValue* pResultValue
	)
{
	CType* pType;

	if (!pLiteral->m_FmtLiteralValue)
	{
		size_t Count = pLiteral->m_BinData.GetCount ();

		if (pLiteral->m_LastToken == EToken_Literal)
			Count++;

		pType = m_pModule->m_TypeMgr.GetArrayType (EType_Char, Count);
	}
	else
	{
		pType = GetSimpleType (m_pModule, EType_Char)->GetDataPtrType ();
	}

	pResultValue->SetType (pType);
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {

