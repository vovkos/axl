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
	jnc::CAttributeSet* pAttributeSet = pItem->GetAttributeSet ();
	if (!pAttributeSet)
		return false;

	HTREEITEM hAttributes = m_TreeCtrl.InsertItem (_T("attributes"), hParent);

	rtl::CString ItemName;

	rtl::CIteratorT <jnc::CAttribute> Attribute = pAttributeSet->GetFirstAttribute ();
	for (; Attribute; Attribute++)
	{
		jnc::CValue* pValue = Attribute->GetValue ();

		if (pValue)
			ItemName.Format (_T("%s = %s"));
		else
			ItemName = Attribute->GetName ();
		HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hAttributes);

		m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) *Attribute);
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

	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pNamespace);

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

	case jnc::EModuleItem_GlobalFunction:
		AddGlobalFunction (hParent, (jnc::CGlobalFunction*) pItem);
		break;

	case jnc::EModuleItem_GlobalProperty:
		AddGlobalProperty (hParent, (jnc::CGlobalProperty*) pItem);
		break;

	case jnc::EModuleItem_EnumMember:
		AddEnumMember (hParent, (jnc::CEnumMember*) pItem);
		break;

	case jnc::EModuleItem_StructMember:
		AddStructMember (hParent, (jnc::CStructMember*) pItem);
		break;

	case jnc::EModuleItem_ClassMember:
		AddClassMember (hParent, (jnc::CClassMember*) pItem);
		break;

	default:
		rtl::CString Name;
		Name.Format (_T("item %x of kind %d"), pItem, ItemKind);
		HTREEITEM hItem = m_TreeCtrl.InsertItem (Name, hParent);
		m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pItem);
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
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pType);

	AddItemAttributes (hItem, pType);

	jnc::EType TypeKind = pType->GetTypeKind ();
	switch (TypeKind)
	{
	case jnc::EType_Enum:
	case jnc::EType_EnumC:
		AddEnumTypeMembers (hItem, (jnc::CEnumType*) pType);
		break;

	case jnc::EType_Struct:
	case jnc::EType_Union:
		AddStructTypeMembers (hItem, (jnc::CStructType*) pType);
		break;

	case jnc::EType_Class:
	case jnc::EType_Interface:
		AddClassTypeMembers (hItem, (jnc::CClassType*) pType);
		break;
	}
}

void
CModulePane::AddEnumMember (
	HTREEITEM hParent,
	jnc::CEnumMember* pMember
	)
{
	HTREEITEM hItem = m_TreeCtrl.InsertItem (pMember->GetName (), hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pMember);
}

void
CModulePane::AddStructMember (
	HTREEITEM hParent,
	jnc::CStructMember* pMember
	)
{
	rtl::CString ItemName;
	ItemName.Format (
		_T("%s %s"), 
		pMember->GetType ()->GetTypeString (),
		pMember->GetName ()
		);

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pMember);
}

void
CModulePane::AddClassMember (
	HTREEITEM hParent,
	jnc::CClassMember* pMember
	)
{
	jnc::EClassMember MemberKind = pMember->GetMemberKind ();

	switch (MemberKind)
	{
	case jnc::EClassMember_Field:
		AddStructMember (hParent, ((jnc::CClassFieldMember*) pMember)->GetField ());
		break;

	case jnc::EClassMember_Method:
		AddFunction (hParent, pMember->GetName (), ((jnc::CClassMethodMember*) pMember)->GetFunction ());
		break;

	case jnc::EClassMember_Property:
		AddProperty (hParent, pMember->GetName (), ((jnc::CClassPropertyMember*) pMember)->GetProperty ());
		break;
	}
}

void
CModulePane::AddEnumTypeMembers (
	HTREEITEM hParent,
	jnc::CEnumType* pType
	)
{
	rtl::CIteratorT <jnc::CEnumMember> Member = pType->GetFirstMember ();
	for (; Member; Member++)
		AddEnumMember (hParent, *Member);
	
	m_TreeCtrl.Expand (hParent, TVE_EXPAND);
}

void
CModulePane::AddStructTypeMembers (
	HTREEITEM hParent,
	jnc::CStructType* pType
	)
{
	rtl::CString ItemName;

	rtl::CIteratorT <jnc::CStructMember> Member = pType->GetFirstMember ();
	for (; Member; Member++)
		AddStructMember (hParent, *Member);

	m_TreeCtrl.Expand (hParent, TVE_EXPAND);
}

void
CModulePane::AddClassTypeMembers (
	HTREEITEM hParent,
	jnc::CClassType* pType
	)
{
	rtl::CString ItemName;

	size_t Count = pType->GetItemCount ();
	for (size_t i = 0; i < Count; i++)
	{
		jnc::CModuleItem* pMember = pType->GetItem (i);
		AddItem (hParent, pMember);
	}

	m_TreeCtrl.Expand (hParent, TVE_EXPAND);
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
		m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pTarget);
		break;

	case jnc::EModuleItem_EnumMember:
		AddItem (hParent, pTarget);
		break;

	default:
		ItemName.Format (_T("alias %s"),  pAlias->GetName ());

		hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
		m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pAlias);

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
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pVariable);
}

void
CModulePane::AddFunction (
	HTREEITEM hParent,
	const rtl::CString& Name,
	jnc::CFunction* pFunction
	)
{
	jnc::CFunctionType* pType = pFunction->GetType ();

	rtl::CString ItemName;
	ItemName.Format (
		_T("%s %s %s"), 
		pType->GetReturnType ()->GetTypeString (),
		Name,
		pFunction->CreateArgString ()
		);

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pFunction);
}

void
CModulePane::AddFunction (
	HTREEITEM hParent,
	const rtl::CString& Name,
	jnc::CFunctionOverload* pFunction
	)
{
	if (!pFunction->IsOverloaded ())
	{
		AddFunction (hParent, Name, pFunction->GetFunction ());
		return;
	}

	size_t Count = pFunction->GetOverloadCount ();

	rtl::CString ItemName;
	ItemName.Format (
		_T("function %s (%d overloads)"), 
		Name, 
		Count		
		);

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	for (size_t i = 0; i < Count; i++)
	{
		jnc::CFunction* pOverload = pFunction->GetFunction (i);
		AddFunction (hItem, Name, pOverload);
	}

	m_TreeCtrl.Expand (hItem, TVE_EXPAND);
}

void
CModulePane::AddProperty (
	HTREEITEM hParent,
	const rtl::CString& Name,
	jnc::CProperty* pProperty
	)
{
	rtl::CString ItemName;
	ItemName.Format (
		_T("%s %s"), 
		pProperty->GetType ()->GetTypeString (),
		Name
		);

	HTREEITEM hItem = m_TreeCtrl.InsertItem (ItemName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pProperty);
	
	jnc::CFunction* pGetter = pProperty->GetGetter ();
	jnc::CFunctionOverload* pSetter = pProperty->GetSetter ();

	AddFunction (hItem, _T("get"), pGetter);
	
	if (!pSetter->IsEmpty ())
		AddFunction (hItem, _T("set"), pSetter);
	
	m_TreeCtrl.Expand (hItem, TVE_EXPAND);
}

void
CModulePane::AddGlobalFunction (
	HTREEITEM hParent,
	jnc::CGlobalFunction* pFunction
	)
{
	AddFunction (hParent, pFunction->GetName (), pFunction);
}

void
CModulePane::AddGlobalProperty (
	HTREEITEM hParent,
	jnc::CGlobalProperty* pProperty
	)
{
	AddProperty (hParent, pProperty->GetName (), pProperty->GetProperty ());
}

rtl::CString
CModulePane::GetItemTip (jnc::CModuleItem* pItem)
{
	jnc::EModuleItem ItemKind = pItem->GetItemKind ();
	switch (ItemKind)
	{
	case jnc::EModuleItem_Namespace:
		return ((jnc::CNamespace*) pItem)->GetQualifiedName ();

	case jnc::EModuleItem_Variable:
		return ((jnc::CVariable*) pItem)->GetQualifiedName ();

	case jnc::EModuleItem_GlobalFunction:
		return ((jnc::CGlobalFunction*) pItem)->GetQualifiedName ();

	case jnc::EModuleItem_Type:
		return ((jnc::CType*) pItem)->GetTypeString ();

	case jnc::EModuleItem_Alias:
		return ((jnc::CAlias*) pItem)->GetQualifiedName ();

	default:		
		return rtl::CString::Format_s (_T("item %x of kind %d"), pItem, ItemKind);
	}
}

void 
CModulePane::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem ();
	if (!hItem)
		return;

	jnc::CModuleItem* pItem = (jnc::CModuleItem*) m_TreeCtrl.GetItemData (hItem);
	if (!pItem)
		return;

	CEditView* pView = GetMainFrame ()->GetDocument ()->GetView ();

	int Offset1 = pView->GetEditCtrl ().LineIndex (pItem->GetPos ().m_Line);
	int Offset2 = pView->GetEditCtrl ().LineIndex (pItem->GetPos ().m_Line + 1);
	
	pView->GetEditCtrl ().SetSel (Offset1, Offset1);
	pView->GetEditCtrl ().SetSel (Offset1, Offset2);

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
