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
	m_Endianness = EEndianness_LittleEndian;
	m_StorageKind = EStorage_Undefined;
	m_AccessKind = EAccess_Undefined;
	m_pAutoEvType = NULL;
	m_pAttributeBlock = NULL;
	m_pLastDeclaredItem = NULL;
}

bool
CParser::IsTypeSpecified ()
{
	if (m_TypeSpecifierStack.IsEmpty ())
		return false;

	CTypeSpecifier* pTypeSpecifier = m_TypeSpecifierStack.GetBack ();
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
	if (m_pLastDeclaredItem)
	{
		if (m_pLastDeclaredItem->GetItemKind () == EModuleItem_Function)
		{
			CFunction* pFunction = (CFunction*) m_pLastDeclaredItem;
			if (pFunction->IsOrphan ())
			{
				err::SetFormatStringError (_T("orphan function '%s' without a body"), pFunction->m_Tag);
				return false;
			}
		}

		return true;
	}

	ASSERT (pTypeSpecifier);
	CType* pType = pTypeSpecifier->GetType ();
	if (!pType || !(pType->GetFlags () & ETypeFlag_Named))
	{
		err::SetFormatStringError (_T("invalid declaration (no declarator, no named type)"));
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

	CGlobalNamespace* pNamespace = GetGlobalNamespace ((CGlobalNamespace*) pCurrentNamespace, Name.m_First, Pos);
	if (!pNamespace)
		return NULL;

	rtl::CBoxIteratorT <rtl::CString> It = Name.m_List.GetHead ();
	for (; It; It++)
	{
		pNamespace = GetGlobalNamespace (pNamespace, *It, Pos);
		if (!pNamespace)
			return NULL;
	}
	
	OpenNamespace (pNamespace);
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

void
CParser::OpenNamespace (CNamespace* pNamespace)
{
	m_NamespaceStack.Append (m_pModule->m_NamespaceMgr.GetCurrentNamespace ());
	m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);
}

void
CParser::CloseNamespace ()
{
	if (m_NamespaceStack.IsEmpty ())
		return;

	CNamespace* pNamespace = m_NamespaceStack.GetBackAndPop ();
	m_pModule->m_NamespaceMgr.SetCurrentNamespace (pNamespace);
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

	m_pModule->m_NamespaceMgr.SetCurrentNamespace (pScope);
	return pScope;
}

void
CParser::CloseScope (const CToken::CPos& Pos)
{
	CScope* pScope = m_pModule->m_NamespaceMgr.GetCurrentScope ();
	if (!pScope)
		return;

	pScope->m_EndPos = Pos;

	m_pModule->m_OperatorMgr.ProcessDestructList (&pScope->m_DestructList);
	m_pModule->m_NamespaceMgr.SetCurrentNamespace (pScope->m_pParentNamespace);
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
		OpenNamespace (pNamedType->m_pExtensionNamespace);
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

	OpenNamespace (pNamespace);
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
		(m_StorageKind == EStorage_Typedef || TypeKind == EType_Function ||	TypeKind == EType_Property))
	{
		err::SetFormatStringError (_T("unused modifier '%s'"), GetPtrTypeFlagString (DataPtrTypeFlags));
		return false;
	}

	if (DeclaratorKind == EDeclarator_SimpleName)
	{
		return
			m_StorageKind == EStorage_Typedef ? DeclareTypedef (pType, pDeclarator) :
			TypeKind == EType_Function ? DeclareFunction ((CFunctionType*) pType, pDeclarator) :
			TypeKind == EType_Property ? DeclareProperty ((CPropertyType*) pType, pDeclarator) :
			DeclareData (pType, pDeclarator, DataPtrTypeFlags, HasInitializer);		
	}
	else
	{
		if (TypeKind != EType_Function || m_StorageKind == EStorage_Typedef)
		{
			err::SetFormatStringError (_T("qualified declarators are only allowed for functions"));
			return false;
		}

		return DeclareFunction ((CFunctionType*) pType, pDeclarator);
	}
}

void
CParser::AssignDeclarationAttributes (
	CModuleItem* pItem,
	const CToken::CPos& Pos
	)
{
	CModuleItemDecl* pDecl = pItem->GetItemDecl ();
	if (!pDecl)
		return;

	pDecl->m_StorageKind = m_StorageKind;
	pDecl->m_AccessKind = m_AccessKind;
	pDecl->m_Pos = Pos;
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
	ASSERT (pDeclarator->GetDeclaratorKind () == EDeclarator_SimpleName);

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CAlias* pAlias = pNamespace->CreateAlias (Name, pType);
	if (!pAlias)
		return false;

	AssignDeclarationAttributes (pAlias, pDeclarator->GetPos ());
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
			NamespaceKind == ENamespace_Property && (FunctionKind == EFunction_Getter || FunctionKind == EFunction_Setter) ?
				((CProperty*) pNamespace)->GetStorageKind () : 
				EStorage_Undefined;
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

	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (
		FunctionKind,
		(CFunctionType*) pType,
		pDeclarator->GetArgList ()
		);

	pFunction->m_pParentNamespace = pNamespace;
	pFunction->m_DeclaratorName = *pDeclarator->GetName ();
	pFunction->m_Tag = pNamespace->CreateQualifiedName (pFunction->m_DeclaratorName);

	AssignDeclarationAttributes (pFunction, pDeclarator->GetPos ());

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
		if (DeclaratorKind == EDeclarator_QualifiedName)
		{
			err::SetFormatStringError (_T("invalid qualified declarator '%s' in type extension"), pFunction->m_Tag);
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

		if (DeclaratorKind == EDeclarator_QualifiedName && m_StorageKind != EStorage_Override)
		{
			err::SetFormatStringError (_T("only overrides could be qualified, '%s' is not an override"), pFunction->m_Tag);
			return false;
		}

		return ((CClassType*) pNamespace)->AddMethodMember (pFunction);

	case ENamespace_Property:
		if (DeclaratorKind == EDeclarator_QualifiedName)
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

		if (DeclaratorKind == EDeclarator_QualifiedName)
		{
			pFunction->m_pOrphanNamespace = pNamespace;
			m_pModule->m_FunctionMgr.m_OrphanFunctionArray.Append (pFunction);
			return true;
		}
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
	ASSERT (pDeclarator->GetDeclaratorKind () == EDeclarator_SimpleName);

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

	rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
	CProperty* pProperty = m_pModule->m_FunctionMgr.CreateProperty (Name, QualifiedName);
	
	AssignDeclarationAttributes (pProperty, Pos);

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

		pProperty->m_pParentNamespace = pNamespace;
	}
	
	return pProperty;
}

bool
CParser::DeclareData (	
	CType* pType,
	CDeclarator* pDeclarator,
	int PtrTypeFlags,
	bool HasInitializer
	)
{
	ASSERT (pDeclarator->GetDeclaratorKind () == EDeclarator_SimpleName);

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

	AssignDeclarationAttributes (pDataItem, pDeclarator->GetPos ());
	return true;
}

CFunctionFormalArg*
CParser::CreateFormalArg (
	CDeclFunctionSuffix* pArgSuffix,
	CTypeSpecifier* pTypeSpecifier,
	CDeclarator* pDeclarator,
	const CValue& DefaultValue
	)
{
	CType* pType;
	rtl::CString Name;

	CDeclarator EmptyDeclarator;

	if (!pDeclarator)
	{
		pDeclarator = &EmptyDeclarator;
		pDeclarator->SetTypeSpecifier (pTypeSpecifier);
	}
	else
	{
		if (pDeclarator->GetDeclaratorKind () != EDeclarator_SimpleName)
		{
			err::SetFormatStringError (_T("only simple name declarators allowed for formal argument"));
			return NULL;
		}

		Name = pDeclarator->GetName ()->GetShortName ();
	}

	pType = pDeclarator->CalcType ();
	if (!pType)
		return NULL;

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

	pEnumType->m_pParentNamespace = pNamespace;

	AssignDeclarationAttributes (pEnumType, m_LastMatchedToken.m_Pos);
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

	pStructType->m_pParentNamespace = pNamespace;

	AssignDeclarationAttributes (pStructType, m_LastMatchedToken.m_Pos);
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

	pUnionType->m_pParentNamespace = pNamespace;
	
	AssignDeclarationAttributes (pUnionType, m_LastMatchedToken.m_Pos);
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
		if (!pClassType)
			return NULL;
	}

	pClassType->m_pParentNamespace = pNamespace;

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
	
	AssignDeclarationAttributes (pClassType, m_LastMatchedToken.m_Pos);
	return pClassType;
}

CClassType*
CParser::CreateAutoEvType (CDeclarator* pDeclarator)
{
	bool Result;

	CNamespace* pNamespace = m_pModule->m_NamespaceMgr.GetCurrentNamespace ();
	CClassType* pAutoEvType = NULL;

	rtl::CString Name = pDeclarator->GetName ()->GetShortName ();
	if (Name.IsEmpty ())
	{
		pAutoEvType = m_pModule->m_TypeMgr.CreateUnnamedClassType ();
	}
	else
	{
		rtl::CString& QualifiedName = pNamespace->CreateQualifiedName (Name);
		pAutoEvType = m_pModule->m_TypeMgr.CreateClassType (Name, QualifiedName);
		if (!pAutoEvType)
			return NULL;
	}

	rtl::CStdListT <CFunctionFormalArg>* pArgList = pDeclarator->GetArgList ();
	if (pArgList) 
	{
		// modify constructor
	}

	if (!Name.IsEmpty ())
	{
		Result = pNamespace->AddItem (pAutoEvType);
		if (!Result)
			return NULL;
	}

	pAutoEvType->m_pParentNamespace = pNamespace;

	AssignDeclarationAttributes (pAutoEvType, pDeclarator->GetPos ());

	CFunctionType* pMethodType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	pAutoEvType->m_Flags |= EClassTypeFlag_AutoEv;
	pAutoEvType->AddMethodMember (m_pModule->m_FunctionMgr.CreateFunction (EFunction_PreConstructor, pMethodType));
	pAutoEvType->AddMethodMember (m_pModule->m_FunctionMgr.CreateFunction (EFunction_Destructor, pMethodType));
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

	CFunctionType* pFunctionType = (CFunctionType*) m_pModule->m_TypeMgr.GetStdType (EStdType_SimpleFunction);
	CFunction* pFunction = m_pModule->m_FunctionMgr.CreateFunction (EFunction_AutoEv, pFunctionType);
	m_pAutoEvType->AddMethodMember (pFunction);

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

	CClosure* pClosure = HandlerValue.CreateClosure ();
	pClosure->GetArgList ()->InsertHead (m_AutoEvConstructorThisValue);
	
	rtl::CBoxIteratorT <CValue> PropertyValue = m_BindablePropertyList.GetHead ();
	for (; PropertyValue; PropertyValue++)
	{
		CValue EventValue;
		Result = 
			m_pModule->m_OperatorMgr.GetPropertyOnChangeEvent (*PropertyValue, &EventValue) &&
			m_pModule->m_OperatorMgr.MulticastOperator (EventValue, HandlerValue, EMulticastOp_Add);

		if (!Result)
			return false;
	}

	m_pAutoEvConstructorBlock = m_pModule->m_ControlFlowMgr.GetCurrentBlock ();

	// remove event handlers in destructor

	m_pModule->m_FunctionMgr.m_pCurrentFunction = m_pAutoEvType->GetDestructor ();
	m_pModule->m_ControlFlowMgr.SetCurrentBlock (m_pAutoEvDestructorBlock);

	pClosure = HandlerValue.CreateClosure ();
	pClosure->GetArgList ()->InsertHead  (m_AutoEvDestructorThisValue);

	PropertyValue = m_BindablePropertyList.GetHead ();
	for (; PropertyValue; PropertyValue++)
	{
		CValue EventValue;
		Result = 
			m_pModule->m_OperatorMgr.GetPropertyOnChangeEvent (*PropertyValue, &EventValue) &&
			m_pModule->m_OperatorMgr.MulticastOperator (EventValue, HandlerValue, EMulticastOp_Remove);

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
CParser::SetFunctionBody (rtl::CBoxListT <CToken>* pBody)
{
	if (!m_pLastDeclaredItem)
	{
		err::SetFormatStringError (_T("declaration without declarator cannot have a function body"));
		return false;
	}

	EModuleItem ItemKind = m_pLastDeclaredItem->GetItemKind ();
	if (ItemKind != EModuleItem_Function)
	{
		err::SetFormatStringError (_T("'%s' cannot have a function body"), GetModuleItemKindString (m_pLastDeclaredItem->GetItemKind ()));
		return false;
	}

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
		break;

	case EModuleItem_Property:
		pValue->SetProperty ((CProperty*) pItem);
		break;

	case EModuleItem_EnumConst:
		pValue->SetConstInt32 (((CEnumConst*) pItem)->GetValue ());
		break;

	case EModuleItem_StructField:
		return m_pModule->m_OperatorMgr.GetFieldMember (m_ThisValue, (CStructField*) pItem, &Coord, pValue);

	default:
		err::SetFormatStringError (_T("%s '%s' cannot be used as expression"), GetModuleItemKindString (pItem->GetItemKind ()), Name);
		return false;
	};

	return true;
}

bool
CParser::SetThis (CValue* pValue)
{
	if (m_ThisValue.IsEmpty ())
	{
		err::SetFormatStringError (_T("function '%s' has no 'this' pointer"), m_pModule->m_FunctionMgr.GetCurrentFunction ()->m_Tag);
		return false;
	}

	*pValue = m_ThisValue;
	return true;
}

//.............................................................................

} // namespace jnc {
} // namespace axl {
