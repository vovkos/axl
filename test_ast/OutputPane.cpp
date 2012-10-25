#include "stdafx.h"
#include "test_ast.h"
#include "OutputPane.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//.............................................................................

BEGIN_MESSAGE_MAP(COutputPane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

int COutputPane::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	VERIFY(m_LogCtrl.Create(
		WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL |
		ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_READONLY, 
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST));

	m_LogCtrl.ModifyStyleEx(0, WS_EX_CLIENTEDGE);
	m_LogCtrl.SetFont (&theApp.m_Font);

	EnableDocking(CBRS_ALIGN_ANY);

	return 0;
}

void COutputPane::OnSize(UINT nType, int cx, int cy) 
{
	m_LogCtrl.MoveWindow(0, 0, cx, cy);
}

//.............................................................................
