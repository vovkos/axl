#pragma once

#include "..\axl_jnc\axl_jnc_Parser.h"

//.............................................................................

class CAstPane : public CDockablePane
{
protected:
	CTreeCtrl m_TreeCtrl;

public:
	bool
	Build (jnc::CParser::CAst* pAstRoot);

	void
	Clear ();

protected:
	HTREEITEM 
	AddAst (
		HTREEITEM hParent,
		jnc::CParser::CAst* pAst
		);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblClk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetInfoTip(NMHDR* pNMHDR, LRESULT* pResult);

	DECLARE_MESSAGE_MAP()
};

//.............................................................................

