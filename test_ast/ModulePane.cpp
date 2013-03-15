#include "stdafx.h"
#include "test_ast.h"
#include "MainFrm.h"
#include "ModulePane.h"
#include "AstDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//.............................................................................

BEGIN_MESSAGE_MAP(CModulePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, AFX_IDW_PANE_FIRST, OnDblClk)
	ON_NOTIFY(TVN_GETINFOTIP, AFX_IDW_PANE_FIRST, OnGetInfoTip)	
END_MESSAGE_MAP()

int CModulePane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_TreeCtrl.Create(
		WS_CHILD | WS_VISIBLE | TVS_LINESATROOT | TVS_HASLINES | TVS_HASBUTTONS | TVS_INFOTIP, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST));
	
	m_TreeCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);

	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

void CModulePane::OnSize(UINT nType, int cx, int cy) 
{
	m_TreeCtrl.MoveWindow(0, 0, cx, cy);
}

bool
CModulePane::Build (jnc::CModule* pModule)
{
	Clear ();

	jnc::CGlobalNamespace* pGlobalNamespace = pModule->m_NamespaceMgr.GetGlobalNamespace ();
	AddNamespace (NULL, pGlobalNamespace);

	return true;
}

void
CModulePane::Clear ()
{
	m_TreeCtrl.DeleteAllItems ();
}

bool
CModulePane::AddItemAttributes (
	HTREEITEM hParent,
	jnc::CModuleItem* pItem
	)
{
	jnc::CModuleItemDecl* pDecl = pItem->GetItemDecl ();
	if (!pDecl)
		return false;

	jnc::CAttributeBlock* pAttributeBlock = pDecl->GetAttributeBlock ();
	if (!pAttributeBlock)
		return false;

	HTREEITEM hAttributes = m_TreeCtrl.InsertItem (_T("attributes"), hParent);

	rtl::CString ItemName;

	rtl::CIteratorT <jnc::CAttribute> Attribute = pAttributeBlock->GetAttributeList ().GetHead ();
	for (; Attribute; Attribute++)
	{
		jnc::CValue* pValue = Attribute->GetValue ();

		if (pValue)
			ItemName.Format (_T("%s = %s"));
		else
			ItemName = Attribute->GetName ();
		HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hAttributes);

		m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) *Attribute);
	}

	m_TreeCtrl.Expand (hAttributes, TVE_EXPAND);
	return true;
}

void
CModulePane::AddNamespace (
	HTREEITEM hParent,
	jnc::CGlobalNamespace* pNamespace
	)
{
	HTREEITEM hItem;
		
	jnc::EModuleItem ItemKind = pNamespace->GetItemKind ();

	if (ItemKind == jnc::EModuleItem_Scope)
	{
		hItem = m_TreeCtrl.InsertItem (_T("scope"), hParent);
	}
	else if (!pNamespace->GetParentNamespace ())
	{
		hItem = m_TreeCtrl.InsertItem (_T("global"), hParent);
	}
	else
	{
		rtl::CString Name = rtl::CString::Format_s (_T("namespace %s"), pNamespace->GetName ());
		hItem = m_TreeCtrl.InsertItem (Name, hParent);
	}

	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pNamespace);

	size_t Count = pNamespace->GetItemCount ();
	for (size_t i = 0; i < Count; i++)
	{
		jnc::CModuleItem* pItem = pNamespace->GetItem (i);
		AddItem (hItem, pItem);
	}

	m_TreeCtrl.Expand (hItem, TVE_EXPAND);
}

void
CModulePane::AddItem (
	HTREEITEM hParent,
	jnc::CModuleItem* pItem
	)
{
	jnc::EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case jnc::EModuleItem_Namespace:
		AddNamespace (hParent, (jnc::CGlobalNamespace*) pItem);
		break;

	case jnc::EModuleItem_Type:
		AddType (hParent, (jnc::CType*) pItem);
		break;

	case jnc::EModuleItem_Alias:
		AddAlias (hParent, (jnc::CAlias*) pItem);
		break;

	case jnc::EModuleItem_Variable:
		AddVariable (hParent, (jnc::CVariable*) pItem);
		break;

	case jnc::EModuleItem_Function:
		AddFunction (hParent, (jnc::CFunction*) pItem);
		break;

	case jnc::EModuleItem_Property:
		AddProperty (hParent, (jnc::CProperty*) pItem);
		break;

	case jnc::EModuleItem_AutoEv:
		AddAutoEv (hParent, (jnc::CAutoEv*) pItem);
		break;

	case jnc::EModuleItem_EnumConst:
		AddEnumConst (hParent, (jnc::CEnumConst*) pItem);
		break;

	case jnc::EModuleItem_StructField:
		AddStructField (hParent, (jnc::CStructField*) pItem);
		break;

	default:
		rtl::CString Name;
		Name.Format (_T("item %x of kind %d"), pItem, ItemKind);
		HTREEITEM hItem = m_TreeCtrl.InsertItem (Name, hParent);
		m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pItem);
	}
}

void
CModulePane::AddType (
	HTREEITEM hParent,
	jnc::CType* pType
	)
{
	rtl::CString ItemName = pType->GetTypeString ();

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pType);

	AddItemAttributes (hItem, pType);

	jnc::EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case jnc::EType_Enum:
		AddEnumTypeMembers (hItem, (jnc::CEnumType*) pType);
		break;

	case jnc::EType_Struct:
		AddStructTypeMembers (hItem, (jnc::CStructType*) pType);
		break;

	case jnc::EType_Union:
		AddUnionTypeMembers (hItem, (jnc::CUnionType*) pType);
		break;

	case jnc::EType_Class:
		AddClassTypeMembers (hItem, (jnc::CClassType*) pType);
		break;

	case jnc::EType_Property:
		AddPropertyTypeMembers (hItem, (jnc::CPropertyType*) pType);
		break;
	}
}

void
CModulePane::AddEnumConst (
	HTREEITEM hParent,
	jnc::CEnumConst* pMember
	)
{
	HTREEITEM hItem = m_TreeCtrl.InsertItem (pMember->GetName (), hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pMember);
}

void
CModulePane::AddValue (
	HTREEITEM hParent,
	const tchar_t* pName,
	jnc::CType* pType,
	jnc::CModuleItem* pItem
	)
{
	rtl::CString ItemName;
	ItemName.Format (
		_T("%s %s"), 
		pType->GetTypeString (),
		pName
		);

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pItem);
}

void
CModulePane::AddEnumTypeMembers (
	HTREEITEM hParent,
	jnc::CEnumType* pType
	)
{
	rtl::CIteratorT <jnc::CEnumConst> Member = pType->GetConstList ().GetHead ();
	for (; Member; Member++)
		AddEnumConst (hParent, *Member);
	
	m_TreeCtrl.Expand (hParent, TVE_EXPAND);
}
/*
void
CModulePane::AddStructClassTypeMembers (
	HTREEITEM hParent,
	jnc::CStructClassType* pType
	)
{
	rtl::CString ItemName;

	size_t Count = pType->GetBaseTypeCount ();
	if (Count)
	{
		HTREEITEM hInheritanceItem = m_TreeCtrl.InsertItem (_T("Inheritance"), hParent);

		for (size_t i = 0; i < Count; i++)
		{
			jnc::CType* pBaseType = pType->GetBaseType (i);

			ItemName = pBaseType->GetTypeString ();
			HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hInheritanceItem);
			m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pBaseType);
		}
	}

	Count = pType->GetGenericArgumentCount ();
	if (Count)
	{
		HTREEITEM hGenericItem = m_TreeCtrl.InsertItem (_T("Generic"), hParent);

		for (size_t i = 0; i < Count; i++)
		{
			jnc::CImportType* pBaseType = pType->GetGenericArgument (i);

			ItemName = pBaseType->GetTypeString ();
			HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hGenericItem);
			m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pBaseType);
		}
	}

}
*/
void
CModulePane::AddStructTypeMembers (
	HTREEITEM hParent,
	jnc::CStructType* pType
	)
{
//	AddStructClassTypeMembers (hParent, pType);

	rtl::CIteratorT <jnc::CStructField> Member = pType->GetFieldList ().GetHead ();
	for (; Member; Member++)
		AddStructField (hParent, *Member);

	m_TreeCtrl.Expand (hParent, TVE_EXPAND);
}

void
CModulePane::AddUnionTypeMembers (
	HTREEITEM hParent,
	jnc::CUnionType* pType
	)
{
//	AddStructClassTypeMembers (hParent, pType);

	rtl::CIteratorT <jnc::CStructField> Member = pType->GetFieldList ().GetHead ();
	for (; Member; Member++)
		AddStructField (hParent, *Member);

	m_TreeCtrl.Expand (hParent, TVE_EXPAND);
}

void
CModulePane::AddClassTypeMembers (
	HTREEITEM hParent,
	jnc::CClassType* pType
	)
{
//	AddStructClassTypeMembers (hParent, pType);
	
	size_t Count = pType->GetItemCount ();
	for (size_t i = 0; i < Count; i++)
	{
		jnc::CModuleItem* pMember = pType->GetItem (i);
		AddItem (hParent, pMember);
	}

	m_TreeCtrl.Expand (hParent, TVE_EXPAND);
}

void
CModulePane::AddPropertyTypeMembers (
	HTREEITEM hParent,
	jnc::CPropertyType* pType
	)
{
	rtl::CString ItemName;
	ItemName = pType->GetTypeString ();

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pType);
	
	jnc::CFunctionType* pGetterType = pType->GetGetterType ();
	jnc::CFunctionTypeOverload* pSetterType = pType->GetSetterType ();

	AddType (hItem, pGetterType);
	
	size_t SetterOverloadCount = pSetterType->GetOverloadCount ();
	for (size_t i = 0; i < SetterOverloadCount; i++)
	{
		jnc::CFunctionType* pOverload = pSetterType->GetOverload (i);
		AddType (hItem, pOverload);
	}
	
	m_TreeCtrl.Expand (hItem, TVE_EXPAND);
}


void
CModulePane::AddAlias (
	HTREEITEM hParent,
	jnc::CAlias* pAlias
	)
{
	jnc::CModuleItem* pTarget = pAlias->GetTarget ();
	jnc::EModuleItem TargetKind = pTarget->GetItemKind ();

	rtl::CString ItemName;
	HTREEITEM hItem;

	switch (TargetKind)
	{
	case jnc::EModuleItem_Type:
		ItemName.Format (
			_T("typedef %s %s"), 
			((jnc::CType*) pTarget)->GetTypeString (),
			pAlias->GetName ()
			);

		hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
		m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pAlias);
		break;

	case jnc::EModuleItem_EnumConst:
		AddItem (hParent, pTarget);
		break;

	default:
		ItemName.Format (_T("alias %s"),  pAlias->GetName ());

		hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
		m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pAlias);

		AddItem (hItem, pTarget);
	}
}

void
CModulePane::AddVariable (
	HTREEITEM hParent,
	jnc::CVariable* pVariable
	)
{
	rtl::CString ItemName;
	ItemName.Format (
		_T("%s %s"), 
		pVariable->GetType ()->GetTypeString (),
		pVariable->GetName ()
		);

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pVariable);
}

void
CModulePane::AddFunction (
	HTREEITEM hParent,
	jnc::CFunction* pFunction
	)
{
	if (!pFunction->IsOverloaded ())
	{
		AddFunctionImpl (hParent, pFunction);
	}
	else
	{
		size_t Count = pFunction->GetOverloadCount ();

		rtl::CString ItemName;
		ItemName.Format (
			_T("function %s (%d overloads)"), 
			pFunction->m_Tag, 
			Count		
			);

		HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
		for (size_t i = 0; i < Count; i++)
		{
			jnc::CFunction* pOverload = pFunction->GetOverload (i);
			AddFunctionImpl (hItem, pOverload);
		}

		m_TreeCtrl.Expand (hItem, TVE_EXPAND);
	}
}

void
CModulePane::AddFunctionImpl (
	HTREEITEM hParent,
	jnc::CFunction* pFunction
	)
{
	jnc::CFunctionType* pType = pFunction->GetType ();

	rtl::CString Name = pFunction->GetFunctionKind () == jnc::EFunction_Named ?
		pFunction->GetName () : 
		jnc::GetFunctionKindString (pFunction->GetFunctionKind ());

	rtl::CString ItemName;
	ItemName.Format (
		_T("%s %s %s"), 
		pType->GetReturnType ()->GetTypeString (),
		Name,
		pFunction->CreateArgString ()
		);

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pFunction);
}

void
CModulePane::AddProperty (
	HTREEITEM hParent,
	jnc::CProperty* pProperty
	)
{
	HTREEITEM hItem = m_TreeCtrl.InsertItem (pProperty->GetName (), hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pProperty);
	
	jnc::CFunction* pGetter = pProperty->GetGetter ();
	jnc::CFunction* pSetter = pProperty->GetSetter ();

	AddFunction (hItem, pGetter);
	
	if (pSetter)
		AddFunction (hItem, pSetter);
	
	m_TreeCtrl.Expand (hItem, TVE_EXPAND);
}

void
CModulePane::AddAutoEv (
	HTREEITEM hParent,
	jnc::CAutoEv* pAutoEv
	)
{
	rtl::CString ItemName;

	if (!pAutoEv->IsNamed ())
		ItemName.Format (
			_T("autoev %s"), 
			pAutoEv->GetStarter ()->CreateArgString ()
			);
	else
		ItemName.Format (
			_T("autoev %s"), 
			pAutoEv->GetName (),
			pAutoEv->GetStarter ()->CreateArgString ()
			);

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) (jnc::CModuleItem*) pAutoEv);
}

rtl::CString
GetNamespaceTip (jnc::CGlobalNamespace* pNamespace)
{
	return rtl::CString::Format_s (_T("namespace %s"), pNamespace->GetQualifiedName ());
}

inline
jnc::CType* 
UnImportType (jnc::CType* pType)
{
	if (pType->GetTypeKind () != jnc::EType_Import)
		return pType;

	jnc::CImportType* pImportType = (jnc::CImportType*) pType;
	jnc::CType* pExternType = pImportType->GetExternType ();
	return pExternType ? pExternType : pType;
}

rtl::CString
GetVariableTip (jnc::CVariable* pVariable)
{
	jnc::CType* pType = pVariable->GetType ();
	pType = UnImportType (pType);
	
	return rtl::CString::Format_s (
		_T("%s %s (%d bytes)"), 
		pType->GetTypeString (), 
		pVariable->GetQualifiedName (),
		pType->GetSize ()
		);
}

rtl::CString
GetFunctionTip (jnc::CFunction* pFunction)
{
	jnc::CFunctionType* pType = pFunction->GetType ();

	rtl::CString TipText;
	TipText.Format (
		_T("%s %s"), 
		pType->GetReturnType ()->GetTypeString (),
		pFunction->CreateArgString ()
		);

	jnc::CScope* pScope = pFunction->GetScope ();
	if (!pScope)
		return TipText;

	jnc::CToken::CPos Pos = pScope->GetBeginPos ();
	jnc::CToken::CPos PosEnd = pScope->GetEndPos ();
	
	TipText.Append (_T("\n"));
	TipText.Append (
		Pos.m_p, 
		PosEnd.m_p - Pos.m_p + PosEnd.m_Length
		); 

	return TipText;
}

rtl::CString
GetPropertyTip (jnc::CProperty* pProperty)
{
	jnc::CPropertyType* pType = pProperty->GetType ();

	rtl::CString TipText;
	TipText.Format (
		_T("%s property %s"), 
		pType->GetReturnType ()->GetTypeString (),
		pProperty->GetName ()
		);

	return TipText;
}

rtl::CString
GetAutoEvTip (jnc::CAutoEv* pAutoEv)
{
	jnc::CAutoEvType* pType = pAutoEv->GetType ();

	rtl::CString TipText;
	TipText.Format (
		_T("autoev %s %s"), 
		pAutoEv->GetName (),
		pAutoEv->GetStarter ()->CreateArgString ()
		);

	return TipText;
}

rtl::CString
GetStructFieldTip (jnc::CStructField* pMember)
{
	jnc::CType* pType = pMember->GetType ();
	pType = UnImportType (pType);
	
	return rtl::CString::Format_s (
		_T("%s %s.%s (%d bytes)"), 
		pType->GetTypeString (), 
		pMember->GetParentType ()->GetQualifiedName (),
		pMember->GetName (),
		pType->GetSize ()
		);
}

rtl::CString
GetEnumConstTip (jnc::CEnumConst* pMember)
{
	rtl::CString TipText = pMember->GetName ();
	
	if (!pMember->HasExpression ())
		return TipText;

	rtl::CBoxIteratorT <jnc::CToken> First = pMember->GetExpressionFirstToken ();
	rtl::CBoxIteratorT <jnc::CToken> Last = pMember->GetExpressionLastToken ();

	TipText.Append (_T(" = "));
	TipText.Append (
		First->m_Pos.m_p, 
		Last->m_Pos.m_p - First->m_Pos.m_p + Last->m_Pos.m_Length
		); 

	return TipText;
}

rtl::CString
CModulePane::GetItemTip (jnc::CModuleItem* pItem)
{
	jnc::CProperty* pProperty = (jnc::CProperty*) pItem;

	jnc::EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case jnc::EModuleItem_Namespace:
		return GetNamespaceTip ((jnc::CGlobalNamespace*) pItem);

	case jnc::EModuleItem_Variable:
		return GetVariableTip ((jnc::CVariable*) pItem);

	case jnc::EModuleItem_Function:
		return GetFunctionTip ((jnc::CFunction*) pItem);

	case jnc::EModuleItem_Property:
		return GetPropertyTip ((jnc::CProperty*) pItem);

	case jnc::EModuleItem_AutoEv:
		return GetAutoEvTip ((jnc::CAutoEv*) pItem);

	case jnc::EModuleItem_Type:
		return ((jnc::CType*) pItem)->GetTypeString ();

	case jnc::EModuleItem_Alias:
		return ((jnc::CAlias*) pItem)->GetQualifiedName ();

	case jnc::EModuleItem_StructField:
		return GetStructFieldTip ((jnc::CStructField*) pItem);

	case jnc::EModuleItem_EnumConst:
		return GetEnumConstTip ((jnc::CEnumConst*) pItem);

	default:		
		return rtl::CString::Format_s (_T("item %x of kind %d"), pItem, ItemKind);
	}
}

void 
CModulePane::OnDblClk (NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem ();
	if (!hItem)
		return;

	jnc::CModuleItem* pItem = (jnc::CModuleItem*) m_TreeCtrl.GetItemData (hItem);
	if (!pItem)
		return;

	jnc::CModuleItemDecl* pDecl = pItem->GetItemDecl ();
	if (!pItem)
		return;

	CEditView* pView = GetMainFrame ()->GetDocument ()->GetView ();

	int Offset1 = pView->GetEditCtrl ().LineIndex (pDecl->GetPos ().m_Line);
	int Offset2 = pView->GetEditCtrl ().LineIndex (pDecl->GetPos ().m_Line + 1);

	pView->GetEditCtrl ().SetSel (Offset1, Offset1);
	pView->GetEditCtrl ().SetSel (Offset1, Offset2);

	if (pItem->GetItemKind () == jnc::EModuleItem_Function)
	{
		jnc::CFunction* pFunction = (jnc::CFunction*) pItem;
		GetMainFrame ()->m_FunctionAstPane.Build (pFunction->GetAst ());
	}

	*pResult = 0;
}

void 
CModulePane::OnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVGETINFOTIP* pInfoTip = (NMTVGETINFOTIP*) pNMHDR;
	jnc::CModuleItem* pItem = (jnc::CModuleItem*) pInfoTip->lParam;
	if (!pItem)
		return;

	rtl::CString Tip = GetItemTip (pItem);

	size_t CopyLength = Tip.GetLength ();
	if (CopyLength > (size_t) pInfoTip->cchTextMax)
		CopyLength = pInfoTip->cchTextMax;

	strncpy (
		pInfoTip->pszText, 
		Tip,
		CopyLength
		);

	pInfoTip->pszText [CopyLength] = 0;	
}

//.............................................................................
