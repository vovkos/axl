#include "stdafx.h"
#include "test_ast.h"
#include "MainFrm.h"
#include "AstPane.h"
#include "AstDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//.............................................................................

BEGIN_MESSAGE_MAP(CAstPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(NM_DBLCLK, AFX_IDW_PANE_FIRST, OnDblClk)
	ON_NOTIFY(TVN_GETINFOTIP, AFX_IDW_PANE_FIRST, OnGetInfoTip)	
END_MESSAGE_MAP()

int CAstPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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

void CAstPane::OnSize(UINT nType, int cx, int cy) 
{
	m_TreeCtrl.MoveWindow(0, 0, cx, cy);
}

bool
CAstPane::Build (jnc::CParser::CAst* pAstRoot)
{
	Clear ();
	AddAst (NULL, pAstRoot);
	return true;
}

HTREEITEM 
CAstPane::AddAst (
	HTREEITEM hParent,
	jnc::CParser::CAst* pAst
	)
{
	const tchar_t* pSymbolName = jnc::CParser::GetSymbolName (pAst->m_Kind);
	
	HTREEITEM hItem = m_TreeCtrl.InsertItem (pSymbolName, hParent);
	m_TreeCtrl.SetItemData (hItem, (DWORD_PTR) pAst);

	size_t Count = pAst->m_Children.GetCount ();
	for (size_t i = 0; i < Count; i++)
	{
		jnc::CParser::CAst* pChild = pAst->m_Children [i];
		AddAst (hItem, pChild);
	}

	m_TreeCtrl.Expand (hItem, TVE_EXPAND);
	return hItem;
}

void
CAstPane::Clear ()
{
	m_TreeCtrl.DeleteAllItems ();
}

void 
CAstPane::OnDblClk(NMHDR* pNMHDR, LRESULT* pResult)
{
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem ();
	if (!hItem)
		return;

	jnc::CParser::CAst* pAst = (jnc::CParser::CAst*) m_TreeCtrl.GetItemData (hItem);

	CEditView* pView = GetMainFrame ()->GetDocument ()->GetView ();

	pView->GetEditCtrl ().SetSel (
		(int) pAst->m_FirstToken.m_Pos.m_Offset, 
		(int) pAst->m_LastToken.m_Pos.m_Offset + pAst->m_LastToken.m_Pos.m_Length
		);

	*pResult = 0;
}

void 
CAstPane::OnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMTVGETINFOTIP* pInfoTip = (NMTVGETINFOTIP*) pNMHDR;
	jnc::CParser::CAst* pAst = (jnc::CParser::CAst*) pInfoTip->lParam;

	CString NodeText (
		pAst->m_FirstToken.m_Pos.m_p, 
		pAst->m_LastToken.m_Pos.m_p + pAst->m_LastToken.m_Pos.m_Length - 
		pAst->m_FirstToken.m_Pos.m_p
		); 

	size_t CopyLength = NodeText.GetLength ();
	if (CopyLength > (size_t) pInfoTip->cchTextMax)
		CopyLength = pInfoTip->cchTextMax;

	strncpy (
		pInfoTip->pszText, 
		NodeText,
		CopyLength
		);

	pInfoTip->pszText [CopyLength] = 0;	
}
