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
}

bool
CParser::ParseTokenList (
	ESymbol Symbol, 
	const rtl::CConstBoxListT <CToken>& TokenList,
	bool IsBuildingAst
	)
{
	bool Result;

	Create (Symbol, IsBuildingAst);

	rtl::CBoxIteratorT <CToken> Token = TokenList.GetHead ();
	for (; Token; Token++)
	{
		Result = ParseToken (&*Token);
		if (!Result)
		{
			err::PushSrcPosError (m_pModule->GetFilePath (), Token->m_Pos.m_Line, Token->m_Pos.m_Col);
			return false;
		}
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

	pItem = UnAliasItem (pItem);

	if (pItem->GetItemKind () != EModuleItem_Type)
		return NULL;
	
	CType* pType = (CType*) pItem;
	return pType->GetTypeKind () != EType_Property ? pType : NULL;
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

	EModuleItem ItemKind = m_pLastDeclaredItem->GetItemKind ();
	switch (ItemKind)
	{
	case EModuleItem_Function:
		if (((CFunction*) m_pLastDeclaredItem)->IsOrphan ())
		{
			err::SetFormatStringError (_T("orphan function '%s' without a body"), m_pLastDeclaredItem->m_Tag);
			return false;
		}
			
		break;
			
	case EModuleItem_Property:
		if (pTypeSpecifier->GetTypeModifiers () & ~ETypeModifierMask_Property)
		{
			err::SetFormatStringError (_T("unused modifier '%s'"), GetTypeModifierString (pTypeSpecifier->GetTypeModifiers () & ~ETypeModifierMask_Property));
			return false;
		}

		break;
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

CScope*
CParser::OpenScope (const CToken::CPos& Pos)
{
	CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();
	ASSERT (pFunction);

	CScope* pCurrentScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	
	CScope* pScope = m_pModule->m_NamespaceMgr.CreateScope ();
	pScope->m_pFunction = pFunction;
	pScope->m_Level = pCurrentScope ? pCurrentScope->GetLevel () + 1 : 1;
	pScope->m_BeginPos = Pos;
	pScope->m_EndPos = Pos;
	pScope->m_pParentNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	m_pModule->m_NamespaceMgr.OpenNamespace (pScope);
	return pScope;
}

void
CParser::CloseScope (const CToken::CPos& Pos)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	ASSERT (pScope);

	pScope->m_EndPos = Pos;

	m_pModule->m_OperatorMgr.ProcessDestructList (&pScope->m_DestructList);
	m_pModule->m_NamespaceMgr.CloseNamespace ();
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
	bool HasInitializer
	)
{
	ASSERT (pDeclarator);

	m_pLastDeclaredItem = NULL;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();

	if (!m_AccessKind)
		m_AccessKind = pNamespace->GetCurrentAccessKind ();

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

	if (DataPtrTypeFlags && 
		(m_StorageKind == EStorage_Typedef || 
		TypeKind == EType_Function ||	
		TypeKind == EType_Property || 
		TypeKind == EType_AutoEv))
	{
		err::SetFormatStringError (_T("unused modifier '%s'"), GetPtrTypeFlagString (DataPtrTypeFlags));
		return false;
	}

	if (m_StorageKind == EStorage_Typedef)
		return DeclareTypedef (pType, pDeclarator);

	switch (TypeKind)
	{
	case EType_Function:
		return DeclareFunction ((CFunctionType*) pType, pDeclarator);

	case EType_Property:
		return DeclareProperty ((CPropertyType*) pType, pDeclarator);

	case EType_AutoEv:
		return DeclareAutoEv ((CAutoEvType*) pType, pDeclarator);			

	default:
		if (DeclaratorKind != EDeclarator_PropValue)
			return DeclareData (pType, pDeclarator, DataPtrTypeFlags, HasInitializer);		

		if (pDeclarator->IsQualified () || pDeclarator->GetBitCount ())
		{
			err::SetFormatStringError (_T("invalid propvalue declarator"));
			return false;
		}

		return DeclarePropValue (pType, DataPtrTypeFlags, HasInitializer);
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

	// don't overwrite unless explicit

	if (m_AccessKind)
		pDecl->m_AccessKind = m_AccessKind;

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

	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("invalid typedef declarator"));
		return false;
	}

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CAlias* pAlias = pNamespace->CreateAlias (Name, pType);
	if (!pAlias)
		return false;

	AssignDeclarationAttributes (pAlias, pNamespace, pDeclarator->GetPos ());
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

	if (DeclaratorKind == EDeclarator_UnaryBinaryOperator)
	{
		ASSERT (!FunctionKind);
		FunctionKind = pType->HasArgs () ? 
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

	if ((FunctionKindFlags & EFunctionKindFlag_NoArgs) && pType->HasArgs ())
	{
		err::SetFormatStringError (_T("'%s' cannot have arguments"), GetFunctionKindString (FunctionKind));
		return false;
	}

	if (!m_StorageKind)
	{
		m_StorageKind = 
			FunctionKind == EFunction_StaticConstructor ? EStorage_Static :
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

		return ((CPropertyTemplate*) pNamespace)->AddMethodMember (FunctionKind, pType);
	}

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (FunctionKind, pType, pDeclarator->GetArgList ());
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
		if (((CNamedType*) pNamespace)->GetTypeKind () != EType_Class)
		{
			err::SetFormatStringError (_T("method members are not allowed in '%s'"), ((CNamedType*) pNamespace)->GetTypeString ());
			return false;
		}

		if (pDeclarator->IsQualified () && m_StorageKind != EStorage_Override)
		{
			err::SetFormatStringError (_T("only overrides could be qualified, '%s' is not an override"), pFunction->m_Tag);
			return false;
		}

		return ((CClassType*) pNamespace)->AddMethodMember (pFunction);

	case ENamespace_Property:
		if (pDeclarator->IsQualified ())
		{
			err::SetFormatStringError (_T("invalid qualified declarator '%s' in property"), pFunction->m_Tag);
			return false;
		}

		return ((CProperty*) pNamespace)->AddMethodMember (pFunction);
	
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

	CProperty* pProperty = CreatePropertyImpl (
		pDeclarator->GetName ()->GetShortName (), 
		pDeclarator->GetPos (), 
		m_StructPackFactor
		);

	if (!pProperty)
		return false;

	bool Result = pProperty->Create (pType);
	if (!Result)
		return false;

	return true;
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
	size_t PackFactor
	)
{
	CProperty* pProperty = CreatePropertyImpl (Name, m_LastMatchedToken.m_Pos, PackFactor);
	pProperty->m_TypeModifiers = GetTypeSpecifier ()->ClearTypeModifiers (ETypeModifierMask_Property);
	return pProperty;
}

CProperty*
CParser::CreatePropertyImpl (
	const rtl::CString& Name,
	const CToken::CPos& Pos,
	size_t PackFactor
	)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	ENamespace NamespaceKind = pNamespace->GetNamespaceKind ();

	if (NamespaceKind == ENamespace_PropertyTemplate)
	{
		err::SetFormatStringError (_T("property templates cannot have property memberts"));
		return false;
	}

	rtl::CString QualifiedName = pNamespace->CreateQualifiedName (Name);
	CProperty* pProperty = m_pModule->m_FunctionMgr.CreateProperty (Name, QualifiedName);
	
	AssignDeclarationAttributes (pProperty, pNamespace, Pos);

	switch (NamespaceKind)
	{
	case ENamespace_Type:
		if (((CNamedType*) pNamespace)->GetTypeKind () != EType_Class)
		{
			err::SetFormatStringError (_T("property members are not allowed in '%s'"), ((CNamedType*) pNamespace)->GetTypeString ());
			return false;
		}

		Result = ((CClassType*) pNamespace)->AddPropertyMember (pProperty);
		if (!Result)
			return false;

		break;

	case ENamespace_Property:
		Result = ((CProperty*) pNamespace)->AddPropertyMember (pProperty);
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

	rtl::CStdListT <CFunctionFormalArg>* pList = pDeclarator->GetArgList ();

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

		Result = ((CClassType*) pNamespace)->AddAutoEvMember (pAutoEv);
		if (!Result)
			return false;

		break;

	case ENamespace_Property:
		if (pDeclarator->IsQualified ())
		{
			err::SetFormatStringError (_T("invalid qualified declarator '%s' in property"), pAutoEv->m_Tag);
			return false;
		}

		Result = ((CProperty*) pNamespace)->AddAutoEvMember (pAutoEv);
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

	return pAutoEv->Create (pType, pDeclarator->GetArgList ());
}

bool
CParser::DeclareData (	
	CType* pType,
	CDeclarator* pDeclarator,
	int PtrTypeFlags,
	bool HasInitializer
	)
{
	if (!pDeclarator->IsSimple ())
	{
		err::SetFormatStringError (_T("invalid data declarator"));
		return false;
	}

	bool Result;

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
		pDataItem = ((CProperty*) pNamespace)->CreateFieldMember (m_StorageKind, Name, pType, BitCount, PtrTypeFlags);
	}
	else if (NamespaceKind != ENamespace_Type)
	{
		CFunction* pFunction = m_pModule->m_FunctionMgr.GetCurrentFunction ();

		CVariable* pVariable = m_pModule->m_VariableMgr.CreateVariable (
			pFunction ? EVariable_Local : EVariable_Global,
			Name, 
			pNamespace->CreateQualifiedName (Name),
			pType, 
			PtrTypeFlags
			);

		if (pFunction && !HasInitializer)
			m_pModule->m_LlvmBuilder.CreateStore (pType->GetZeroValue (), pVariable);

		Result = pNamespace->AddItem (pVariable);
		if (!Result)
			return false;

		pDataItem = pVariable;
	}
	else
	{
		CNamedType* pNamedType =  (CNamedType*) pNamespace;
		EType NamedTypeKind = pNamedType->GetTypeKind ();
		
		switch (NamedTypeKind)
		{
		case EType_Class:
			pDataItem = ((CClassType*) pNamedType)->CreateFieldMember (m_StorageKind, Name, pType, BitCount, PtrTypeFlags);
			break;

		case EType_Struct:
			pDataItem = ((CStructType*) pNamedType)->CreateFieldMember (Name, pType, BitCount, PtrTypeFlags);
			break;

		case EType_Union:
			pDataItem = ((CUnionType*) pNamedType)->CreateFieldMember (Name, pType, BitCount, PtrTypeFlags);
			break;

		default:
			err::SetFormatStringError (_T("field members are not allowed in '%s'"), pNamedType->GetTypeString ());
			return false;
		}
	}

	if (!pDataItem)
		return false;

	AssignDeclarationAttributes (pDataItem, pNamespace, pDeclarator->GetPos ());
	return true;
}

bool
CParser::DeclarePropValue (	
	CType* pType,
	int PtrTypeFlags,
	bool HasInitializer
	)
{
	bool Result;

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
		return pPropertyTemplate->AddMethodMember (EFunction_Getter, pGetterType);
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
	Result = pProperty->AddMethodMember (pGetter);
	if (!Result)
		return false;

	AssignDeclarationAttributes (pGetter, pNamespace, m_LastMatchedToken.m_Pos);
	return true;
}

CFunctionFormalArg*
CParser::CreateFormalArg (
	CDeclFunctionSuffix* pArgSuffix,
	CDeclarator* pDeclarator,
	const CValue& DefaultValue
	)
{
	CType* pType = pDeclarator->CalcType ();
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

	CFunctionFormalArg* pArg = AXL_MEM_NEW (CFunctionFormalArg);
	pArg->m_Name = Name;
	pArg->m_pType = pType;
	pArg->m_DefaultValue = DefaultValue;
	pArgSuffix->m_ArgList.InsertTail (pArg);
	return pArg;
}

CEnumType*
CParser::CreateEnumType (
	const rtl::CString& Name,
	int Flags
	)
{
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CEnumType* pEnumType = NULL;

	if (Name.IsEmpty ())
	{
		pEnumType = m_pModule->m_TypeMgr.CreateUnnamedEnumType ();
		pEnumType->m_Flags = EEnumTypeFlag_Exposed;
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pEnumType = m_pModule->m_TypeMgr.CreateEnumType (Name, QualifiedName, Flags);
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

	CType* pReturnType = m_pModule->m_TypeMgr.GetPrimitiveType (EType_Void);
	CFunctionType* pStarterType = pFunctionSuffix ? m_pModule->m_TypeMgr.GetFunctionType (
		pReturnType, 
		pFunctionSuffix->GetArgTypeArray (),
		pFunctionSuffix->GetFunctionTypeFlags ()
		) : 
		(CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);

	CAutoEvType* pAutoEvType = m_pModule->m_TypeMgr.GetAutoEvType (pStarterType);	

	CAutoEv* pAutoEv = m_pModule->m_FunctionMgr.CreateUnnamedAutoEv ();
	
	Result = 
		pClassType->AddAutoEvMember (pAutoEv) &&
		pAutoEv->Create (pAutoEvType, pFunctionSuffix ? pFunctionSuffix->GetArgList () : NULL);

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
	CStructField* pEventPtrField = *pBindSiteType->GetFieldMemberList ().GetHead ();
	CStructField* pCookieField = *pBindSiteType->GetFieldMemberList ().GetTail ();

	CValue BindSiteArrayValue;
	Result = m_pModule->m_OperatorMgr.GetAutoEvBindSiteArray (pAutoEv, &BindSiteArrayValue);
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
			Result = m_pModule->m_OperatorMgr.GetAuPropertyFieldMember (*Value, EAuPropertyField_OnChange, &OnChangeValue);
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
				m_pModule->m_OperatorMgr.GetStructFieldMember (BindSiteValue, pEventPtrField, NULL, &DstOnChangeValue) &&
				m_pModule->m_OperatorMgr.GetStructFieldMember (BindSiteValue, pCookieField, NULL, &DstCookieValue) &&
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

	CloseScope (ClosePos);

	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("switch_case"));
	m_pModule->m_ControlFlowMgr.Follow (pBlock);
	It->m_Value = pBlock;

	CScope* pScope = OpenScope (OpenPos);
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

	CloseScope (ClosePos);

	CBasicBlock* pBlock = m_pModule->m_ControlFlowMgr.CreateBlock (_T("switch_default"));
	m_pModule->m_ControlFlowMgr.Follow (pBlock);
	pSwitchStmt->m_pDefaultBlock = pBlock;

	CScope* pScope = OpenScope (OpenPos);
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
CParser::FinalizeConditionalExpr_t (
	const CValue& TrueValue,
	const CValue& FalseValue,
	CValue* pResultValue
	)
{

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

	pResultValue->SetType (pType);
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
		return m_pModule->m_OperatorMgr.GetFieldMember (
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

	case EModuleItem_Variable:
		pValue->SetType (((CVariable*) pItem)->GetType ()->GetDataPtrType (EType_DataRef, EDataPtrType_Thin));
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
CParser::GetAuPropertyFieldMember (
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

	return m_pModule->m_OperatorMgr.GetAuPropertyFieldMember (pFunction->m_pProperty, Field, pValue);
}

bool
CParser::GetAuPropertyFieldMemberType (
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

//.............................................................................

} // namespace jnc {
} // namespace axl {
